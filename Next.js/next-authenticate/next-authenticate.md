@[TOC](Next.js 路由守衛應用)

# 前言

這篇文檔要來總結的是關於在項目中實踐對於 Next 路由守衛、身份認證等機制的實現方法，以及在 Next 中具體要如何使用，和一些自己的採坑紀錄。

# 正文

## 需求背景

在我的項目中，需求其實也很常見。對於登陸，我使用的是 JWT token 的認證方式，並且存儲於前端 cookie 瀏覽器中。需求就是希望可以在用戶登陸前檢查 cookie 的 token，如果有 token 才可以允許所有路由放行，如果無 token 則表示用戶尚未登陸或是 token 過期，這種情況下就要重定向到登陸頁，並且對於其他路由攔截用戶訪問請求。

## 錯誤示範

Next 提供了許多關於用戶安全認證的高級 api 以及中間件，但經過調研發現 Next 不像 vue 或者 react 提供了很完整的路由守衛生態。

最先開始我想的比較簡單，就是直接通過獲取 cookie 來判斷，並做出相應的路由控制，代碼如下：

- 下面為了簡單起見，不給出實際的項目代碼，模擬有一個 `/login`, `/`，並且在 localStorage 中直接存儲一個值，通過是否為空進行用戶是否登陸的判斷。

```js
// index.js

import React, { useEffect } from "react";
import { useRouter } from "next/router";

export default function Home() {
  const router = useRouter();

  const logout = () => {
    localStorage.removeItem("token");
    router.push("/login");
  };

  useEffect(() => {
    if (!localStorage.getItem("token")) {
      router.push("/login");
    }
  });

  return (
    <>
      <div>Home</div>
      <button onClick={() => logout()}>logout</button>
    </>
  );
}
```

```js
// login.js

import React from "react";
import { useRouter } from "next/router";

export default function Login() {
  const router = useRouter();

  const login = () => {
    localStorage.setItem("token", 1);
    router.push("/");
  };

  return (
    <>
      <div>Login</div>
      <button onClick={() => login()}>login</button>
    </>
  );
}
```

代碼的邏輯其實很簡單，但是這其實是會有問題的。我們來看看效果：

![](https://obohe.com/i/2021/07/29/iph1le.gif)

可以看到其實這是存在 bug 的。我們發現這樣的實現方案之下，如過用戶仍然強制訪問應該被禁止訪問的 url，會出現 flash 的情況。Flash 就是說雖然確實最後仍然導向正確的路由，但是用戶仍然可以看到『一閃』不該看到的頁面。

其實這麼做在一般的 react 項目是沒問題的，但是在 Next 中這麼做始終無法規避這個 bug。原因其實是因為 Next 採用的是 SSR 的渲染策略，而一般的 react 項目都僅僅是完全的瀏覽器端渲染策略。

所以造成 flash 的原因就是因為，在 SSR 的渲染策略下，同一份代碼其實會經過兩次渲染階段，服務端渲染和客戶端渲染，而因為這麼實現的路由控制邏輯，包括 `useRouter`, `localStorage` 都是瀏覽器、客戶端才有的東西，在 Node 環境下是沒有這些東西和概念的，所以在服務端渲染的階段，Next 就根本不知道你有這樣的路由控制考量，因此仍然會返回 `/` 頁面，等到客戶端渲染才執行我們所預期的路由規則。

理解了之後，原因就是要理清 SSR 渲染機制下的一些概念和階段。所以其實對於 SSR，我們想要做到路由攔截控制，就必須在服務端渲染階段就要做到判斷邏輯的執行，以及重定向到正確的頁面。

## HOC 輪子

對於在使用 `getInitialProps`，而非 `getServerSideProps` 或是 `getStaticProps` 的場景，我實現了一個高階組件輪子，代碼如下：

```js
// components/Auth.jsx

import React from "react";
import { redirect } from "../utils/index";

export function Auth(Component) {
  return class AuthComponent extends React.Component {
    static async getInitialProps({ ...ctx }) {
      // 下面是伪代码，这边视情况向服务器请求当前 token 或是可以标识当前登录用户的 flag
      const token = await getTokenApi();
      if (!token) {
        redirect(ctx, "/login");
        return {
          token: null,
        };
      }
      return {
        token,
      };
    }

    render() {
      return <Component {...this.props} />;
    }
  };
}
```

```js
// utils/index.js

export const redirect = (ctx, target) => {
  if (ctx.res) {
    // server side
    ctx.res.writeHead(303, { Location: target });
    ctx.res.end();
  } else {
    // client side
    Router.replace(target);
  }
};
```

`Auth` 組件是一個高階組件，實現的就是根據一些認證數據去做路由是否重定向的工作。具體使用方式，就是將 `Auth` 組件包裝在需要認證才能夠訪問的頁面，以 `index` 頁面為例：

```js
import React from "react";
import { useRouter } from "next/router";
import { Auth } from "../components/Auth";

function Home() {
  // ...
}

// HOC 使用
export default Auth(Home);
```

## redirect

上面 HOC 的方式當然是沒問題的，不過其實隨著 Next 的生態發展之快，`getInitialProps` 其實已經提倡都通過 `getStaticProps` 和 `getServerSideProps` 來代替，所以上面的 HOC 就不那麼合適。

因此，Next 也提供了一個 `redirect` 供我們使用，以 `getServerSideProps` 為例，使用方式如下：

```js
export const getServerSideProps = async (ctx) => {

  // 下面是伪代码，视情况向服务器请求当前 token 或是可以标识当前登录用户的 flag
  const res = await ...

  // 一樣根據實際項目前後端接口約定
  // status 如果為 500 則表示當前無用戶登陸，且 payload = {}
  // status 如果為 200 則表示當前用戶已登陸，且 payload = { ... 用戶信息 }
  const { status, payload } = res.data

  if (status === 500) {
    // 服務端重定向
    return {
      redirect: {
        destination: '/login',
        permanent: false,
      },
    }
  }

  const { username } = payload

  return {
    props: {
      username,
      // ...
    },
  }
}
```
上面就是完整的重定向邏輯，`getStaticProps` 也是一樣的使用方式。通過預先判斷，並 return 一個對象包含 `redirect` 屬性，`redirect` 又有一個 `destination`，也就是要重定向到的路由，就可以解決 flash 的問題。
 
# 結語

這份文檔總結了關於在 Next.js 中路由重定向的問題，提供了兩種方案，具體都可以視情況使用。值得留意的就是，無論是哪種方案，其實原理都是要強制在服務端就進行路由的重定向和一些邏輯判斷，因為我們需要避免 SSR 的服務端渲染階段所導致的 flash 問題。


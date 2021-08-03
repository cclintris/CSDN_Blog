@[TOC](Next.js 基礎入門總結)

# 前言

之前幾天總結了關於 SSR 的基礎應用以及相關概念，並且也針對了騰訊雲國際站官網的首頁詳細了解了關於其 SSR 和 React 的應用及實現。這邊要來總結的是關於 Next.js 基本的入門使用，因為其實之前並沒有接觸過 SSR，也就沒有對 Next 做相關了解。本文檔關注於 Next.js 的基礎概念和基礎使用，比較適合 Next.js 的初學者。

# 正文

下面開始總結關於 Next.js 的一些基礎概念，並且會搭配一些實際代碼案例和展示來幫助理解。

## 創建 Next 項目

首先我們先來創建一個 Next 項目，其實也簡單就跟創建 react 項目一樣，直接使用 Next 提供的腳手架：

```js
npx create-next-app [項目名稱]
```

創建好之後，文件目錄應如下：

![](https://obohe.com/i/2021/07/26/h5no7n.png)

## Head

首先先介紹一個 Next 提供比較基本的標籤，`Head` 標籤。這個標籤可以添加於所有的頁面中，作用就是可以將 `Head` 中的 html 標籤加到頁面的 `head` 標籤中。

```js
import React from "react";
import Head from "next/head";

export default function home() {
  return (
    <>
      <Head>
        <title>Home page Head tag</title>
      </Head>
      <p>Home page</p>
    </>
  );
}
```

打開 f12，我們可以看到如下：

![](https://obohe.com/i/2021/07/26/h5ns8s.png)

這個標籤可以用於添加一些基本的標籤，像是 `title`, `meta` 等，有助於提高網頁的 SEO。

## pages

在 Next 中，pages 是一個很重要的概念，或是也可以想成說 pages 就是一個基本組織單位。

看到 `pages` 目錄，下面我們就會放所有的頁面，這些文件可以是 js, jsx, ts, tsx 類型，而本質上其實就是 React 組件。`pages` 可以就想成是根目錄 `/`，而所有位於 `pages` 下的文件都是一個頁面，而路由就是根據文件名作為路由。

```js
// home.jsx

export default function Home() {
  return <div>Home page</div>;
}
```

以上面的代碼為例，我們就可以通過 `/home` 為路由訪問到 Home 頁面。

## 動態路由

一般放在 pages 文件夾下的就是靜態路由，就像上面的 `/home`。而動態路由在 Next 中也提供了相應的規則實現。比如說，我們如果創建一個 `pages/posts/[id].jsx`，那這個意思就是說，我們現在可以通過形如 `/posts/1`, `/posts/2` 的方式去訪問頁面。

## 路由

Next 本身就是一個基於 SSR 的框架，所以當然也就分為客戶端渲染和服務端渲染。而我們常常會需要做路由跳轉的工作，這邊就來介紹我自己在項目實踐中使用作客戶端路由的一些方法。

### Link

`Link` 標籤是 Next 原生提供的一個標籤。其作用其實就是做路由的控制，其中需要包含一個 `a` 標籤，來看看具體代碼：

假設今天有三個頁面：

- /pages/index.js
- /pages/about.js
- /pages/posts/[id].js

導航組件：

```js
import Link from "next/link";

export default function Nav() {
  return (
    <>
      <Link href="/">
        <a>Home</a>
      </Link>
      <Link href="/about">
        <a>About</a>
      </Link>
      <Link href="/posts/1">
        <a>Post</a>
      </Link>
    </>
  );
}
```

### useRouter

上面的 `Link` 其實就是最基本控制客戶端路由的方法，但 `Link` 其實頂多就是幫我們做路由跳轉的工作，但其實關於路由還有許多有用的信息。Next 就提供了一個 hook `useRouter` 讓我們使用。

通過 `useRouter` 我們可以獲取到頁面的路由對象：

```js
import { useRouter } from "next/router";

function Route() {
  // router 就是路由對象
  const router = useRouter();
}
```

下面就整理一下關於 `useRouter` 路由對象一些我使用到感覺比較實用的屬性，直接通過簡單的代碼來看看路由對象。詳細當然還是查閱官方文檔好：

```js
import React, { useEffect } from "react";
import { useRouter } from "next/router";

export default function Project() {
  const router = useRouter();
  useEffect(() => {
    console.log(router);
  });
  return <div>Project page</div>;
}
```

![](https://obohe.com/i/2021/07/28/is2960.png)

主要可以看到幾個比較重要的屬性以及區分如下：

- `asPath` 以及 `pathname` 都表示當前的路由。但是 `pathname` 是最原生的路由，而 `asPath` 則會帶上動態路由的實際參數。

- `query` 默認是 `{}`。動態路由的情況下，`query` 幫我們將路由參數解析成了 js 對象，可以很方便地讓我們獲取到這些動態路由信息。

- `push` 方法非常實用，可以幫我們做到類似 `Link` 的功能，但是功能強大一些。

```js
router.push(url, as, options);
```

| 參數    | 描述                                                                        |
| :------ | :-------------------------------------------------------------------------- |
| url     | 跳轉的目標 url                                                              |
| as      | 跳轉後的目標頁面瀏覽器所顯示的 url 別名                                     |
| options | 有 `scroll`, `shallow`, `locale` 三個屬性。官方文檔都講得很清楚，這邊不細說 |

- `prefetch` 方法做的就是預取某個 url。通常就是用在當你知道即將會要重定向到某個 url 時，這時候可以配合 `useEffect`，在裡面先 `prefetch`，這樣實際 `push` 時就會更快。其實這個方法是為了提升路由跳轉的問題，且根據官方文檔，只有與 `Link` 無法使用時才使用 `prefetch` 才有用，因為 `Link` 是默認幫我們做了 `prefetch` 的性能優化的。

```js
router.prefetch(url, as);
```

- `back` 方法就是簡單的執行 `window.history.back` 方法，挺實用的，就是回到上一個訪問過的 url。

- `reload` 方法就是簡單的執行 `window.location.reload` 方法，也有他的實用場景，就是強制當前頁面的全部刷新，相當於按了 f12。不過注意，調用 `reload` 將會刷新掉所有與狀態相關的數據，像是 state, redux 等狀態全部都會因為刷新也消失，所以要注意使用場景。

## \_app.js

新建好的 Next 項目都會有一個 `_app.js` 文件，默認如下：

```js
import "../styles/globals.css";

function MyApp({ Component, pageProps }) {
  return <Component {...pageProps} />;
}

export default MyApp;
```

可以看到這個文件其實也是一個 react 組件，而所做的是無非就是接受一個 Component 以及該 Component 所需要的 props，然後返回一個 render 好 props 的完整的 Component。

會有這樣的設計是因為 SSR 渲染策略的需要。Next 會在 SSR 服務端渲染的階段，將生成好了 Component 以及獲取到的 props ( 通故 `getStaticProps`, `getServerSideProps` ) 通過該組件將 props 注入到 Component 中，最後才能返回出一個完整的 HTML 文件。

所以其實這個 `MyApp` 組件就像一個全局的過濾器，所以通過這個文件我們可以做到一些全局的配置。例如，假設我們希望所有頁面都需要一個 `Header` 組件，我們就可以這樣添加：

```js
import "../styles/globals.css";
import Header from "xxx";

function MyApp({ Component, pageProps }) {
  return (
    <>
      <Header />
      <Component {...pageProps} />
    </>
  );
}

export default MyApp;
```

我們如果需要每個頁面渲染時都要有全局的狀態，我們也可以通過直接往 `pageProps` 這個 js 對象添加屬性來使每個頁面都會擁有的狀態。

## \_document.js

有時候我們想自定義頁面的 `Head` 標籤或是 `body` 標籤，Next 也提供了相應的措施。通過新建這個 `_document.js`，我們就可以自定義這些屬性。

下面給出我自己在項目中為了要補充語義化 SEO 優化，所以自定義的 `_document.js` 如下：

```js
/**
 * @file Page for overriding default document to improve SEO
 * @author trislin
 */

import Document, { Html, Head, Main, NextScript } from "next/document";

class MyDocument extends Document {
  static async getInitialProps(ctx) {
    const initialProps = await Document.getInitialProps(ctx);
    return { ...initialProps };
  }

  render() {
    return (
      <Html>
        <Head>
          <title>Next.js Blogger</title>
          <meta name="description" content="A blogger based on Next.js" />
          <meta httpEquiv="Content-Type" content="text/html; charset=utf-8" />
          <meta name="viewport" content="width=device-width, initial-scale=1" />
        </Head>
        <body>
          <Main />
          <NextScript />
        </body>
      </Html>
    );
  }
}

export default MyDocument;
```
打開控制台就會發現 `Head` 確實裡面多了自定義了這些標籤。

## 獲取數據

對於 SSR 來說分為服務端以及客戶端渲染，這部分獲取數據主要就是關注於 Next 為每個頁面、組件獲取所需的 props 的策略。

### getServerSideProps

`getServerSideProps` 會在每次請求 (每次 request) 頁面或是組件的時候都執行一次。

```js
export async function getServerSideProps(context) {
  return {
    props: {},
  }
}
```
關於 `getServerSideProps` 的 context 參數非常重要，也很常用。直接截官網圖，說得非常清楚：

![](https://obohe.com/i/2021/07/28/stb1dj.png)

### getStaticProps

`getStaticProps` 會在 build 階段獲取數據，也就表示說，只會獲取一次，數據就再也不會改變了，因此 `static` 表示靜態。使用方式其實就是要 export 出這個函數，如下：

```js
export async function getStaticProps(context) {
  return {
    props: {},
  }
}
```
這邊 return 的 props 就會被當成該組件的 props 參數傳進去渲染組件。

關於 `getStaticProps` 的 context 參數一樣直接截官網圖，說得非常清楚：

![](https://obohe.com/i/2021/07/28/vqtre3.png)

### getStaticPaths

`getStaticPaths` 用於動態路由的場景，而且必須與 `getStaticProps` 連用才行。

`getStaticPaths` 所做的就是，對於動態路由，Next 希望在 build 階段就要知道需要根據路由的動態參數生成出多少份 HTML。比如說，假設今天有 `pages/posts/[id].js`，然後假設現在系統中有 id 為 1~3 的 posts，Next 就會希望渲染動態路由 `/posts/[id]` 之前就先生成出三份 HTML，`/posts/1`、`/posts/2`、`/posts/3`。

所以為此我們就要配合 `getStaticPaths`，用法如下：

```js
export async function getStaticPaths() {
  return {
    paths: [
      { params: { ... } }
    ],
    fallback: true or false
  };
}
```
看到 return 的一個重要的屬性就是 `paths` 數組。其中每個元素都是一個對象，形如 `{ params: { ... } }`。以上述違例，Next 就規定 `getStaticPaths` 要返回如下：

```js
return {
  paths: [
    { params: { id: '1' } },
    { params: { id: '2' } },
    { params: { id: '3' } },
  ],
  fallback: ...
}
```
`getStaticProps` 的 context 中有一個 `params` 參數，通過這個參數，Next 就會將當前的參數對象 (eg. `{ id: '1' }`) 動態賦值給 `params` 參數。並且這麼一來，Next 就可以在 build 階段生成出所有動態路由對應的 HTML。

### 小結

這邊關於 Next 數據獲取的方案，整理如下表格：

|屬性|getServerSideProps|getStaticProps|
|:-----|:-----|:-----|
|預渲染形式|服務端渲染 SSR|靜態生成 Static Generation|
|生成 HTML 時機|每次 request 重新生成|只有一次，build 階段|
|運行階段|每次 request|只有一次，build 階段|
|運行環境|服務端|服務端|
|動態路由|不支持|與 getStaticPaths 連用|

關於這兩個函數，還有一些值得注意的點，整理如下：

1. 我們可以看到 `getStaticProps` 的 context 對象中沒有 req, res, query。原因也很簡單，就是因為 `getStaticProps` 是在 build 階段運行的，所以那些請求時才有的關於 http 的一些參數，自然只會存在於 `getServerSideProps`。

2. `getStaticProps` 和 `getServerSideProps` 是不可以同時使用的。

3. 上面說 `getStaticProps` 只會在 build 階段運行一次，且數據也就靜態的固定下來。不過一般我們在開發環境下 (`npm run dev`)，其實會發現如果數據變了，頁面 UI 也會變，也就意味每次 request 也調用了 `getStaticProps`。原因是因為 `getStaticProps` 只有在生產環境下 (npm run build -> npm run start) 才是靜態的，在開發環境下 (npm run dev) `getStaticProps` 仍然會在每次 request 時都會被調用。

4. `getStaticProps` 只能在 page 級頁面下使用。但是 `getServerSideProps` 可以隨意用。

# 結語

這篇文檔大致上總結了在本次練手項目中，學習 Next.js 以及使用的一些心得，包括個人感覺比較常用和重要的知識點以及概念。當然關於 Next.js 還有更多高級 api 和功能，具體都可以去查閱官方文檔。

# 參考

| 參考             | 鏈接                   |
| :--------------- | :--------------------- |
| Next.js 官方文檔 | https://www.nextjs.cn/ |

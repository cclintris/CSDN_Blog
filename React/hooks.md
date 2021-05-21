@[TOC](關於 React 中的 Hooks)

# 前言
這篇要來討論關於 react 的 hooks。學過 react 的人肯定都知道什麼是 hooks，也就是說 hooks 可以說是 react 必會的東西，所幸通過這篇我來整理下關於 hooks 的知識點以及一些個人理解。這篇應該會很長，不過整體來說應該還算是詳細的。

長文慎入！

# 正文
話不多說，馬上進入 react hooks 的世界吧！

## 什麼是 Hooks? 
在正式進入 react hooks 之前，我覺得首先要先知道 hooks 到底是啥，到底為了解決什麼。

我們大部分 React 類組件可以保存狀態，而函數組件不能？ 並且類組件具有生命周期，而函數組件卻不能？

自從 Reactive0.14.0 以來，我們有兩種方法來創建組件，類或函數。區別在於，如果組件需要具有狀態或需要使用生命周期方法，則必須使用類。否則，如果它只是呈現一些簡單的 UI，我們可以使用一個函數，其實無所謂。

如果我們不用使用類，而是總是使用函數，那該怎麽辦呢？

> 有時候，完美無缺的安裝只需要一個函數。不用方法。不用類。也不用框架。只需要一個函數。
>
> ——John Carmack. OculusVR首席技術官。

當然，我們需要找到一種方法來添加功能組件擁有狀態和生命周期方法的能力，但是假設我們這樣做了，我們能得到什麽好處呢?其實好處可太多了，我們不再需要調用 `super(props)`，不再需要考慮 `bind` 方法或 `this` 關鍵字，也不再需要使用類。所有 “小” 問題都會消失。

總之，可以說 hooks 非常強大，可以做到很多事情，且又可以幫我們避免掉一些瑣碎又難處理的問題，而且 js 系的人應該都聽過這麼一句話

> js 中所有東西都是函數

那既然這樣，都用函數不是太好了。

大概就是這樣，接下來就進入 hooks 的世界，看看到底 hooks 哪裏好了。

## useState
首先第一個要介紹的 hook 就是 `useState`。我們馬上先來看看 `useState` 怎麼用，再來說它的用途以及強大在哪裏：
```js
import React, { useState } from "react";

import { Button } from "antd";

const Hooks = () => {
  const [count, setCount] = useState(0);

  return (
    <div>
      <Button type="primary" onClick={() => setCount(count + 1)}>
        +
      </Button>
      <div>{count}</div>
    </div>
  );
};

export default Hooks;
```
效果如下：
![](https://riyugo.com/o/210510/ne0d1w.gif)

`useState` 這個 hook 其實是用於對一個組件聲明 state 的，就像使用 class component 我們把 `this.state = {...}` 放在 `constructor()` 差不多的意思。

`useState` 接受一個參數，並且會返回一個數組，其中會有一個狀態初值就是我們給定的參數，然後還會有一個函數用於改變這個狀態。所以上面代碼的意思就是，通過 `useState` 我們為 Hooks 這個組件聲明了一個 count 狀態，以及一個可以改變這個 count 的 setCount 方法。

注意到，`useState` 其實可以接受任何參數的，譬如說這樣：
```js
import React, { useState } from "react";

import { Button } from "antd";

const Hooks = () => {
  const [{ count1, count2 }, setCount] = useState({ count1: 10, count2: 20 });

  return (
    <div>
      <Button
        type="primary"
        onClick={() => setCount((state) => ({ count1: state.count1 + 1 }))}
      >
        +
      </Button>
      <div>count1: {count1}</div>
      <div>count2: {count2}</div>
    </div>
  );
};

export default Hooks; 
```
這樣的確是沒問題的，但是看看效果其實有一個 bug:
![](https://riyugo.com/o/210510/nsbow8.gif)

這個原因是因為，我們通過 `useState` 返回的更改 state 的函數，並傳入一個 updater，但是 `useState` 並不會幫我們跟原本的 state 進行合併 merge 的動作，所以這樣寫相當於 count2 直接沒了。正確寫法如下：
```js
import React, { useState } from "react";

import { Button } from "antd";

const Hooks = () => {
  const [{ count1, count2 }, setCount] = useState({ count1: 10, count2: 20 });

  return (
    <div>
      <Button
        type="primary"
        onClick={() =>
          setCount((state) => ({ ...state, count1: state.count1 + 1 }))
        }
      >
        +
      </Button>
      <div>count1: {count1}</div>
      <div>count2: {count2}</div>
    </div>
  );
};

export default Hooks;
``` 
效果如下：
![](https://riyugo.com/o/210510/nvyc8o.gif)

簡單來說，`useState` 就是 class component 中的 state，不過 state 只能有一個，`useState` 可以有很多個，我們就可以更好的做組件狀態的管理，這在項目越來越大就很有用，比較好維護，這是 hooks 的天然優勢。

## 自定義 Hooks
有了 `useState` 的基本了解後，感覺也沒特別強大方便啊！這邊就來說說到底 `useState` 可以怎麼使用，到底怎麼強大了。

表單是再常見不過的需求，一般來說可以這麼寫表單組件：
```js
import React, { useState } from "react";

const Hooks = () => {
  const [email, setEmail] = useState("");
  const [password, setPassword] = useState("");

  return (
    <div>
      <input
        type="email"
        name="email"
        value={email}
        onChange={(e) => setEmail(e.target.value)}
      />
      <input
        type="password"
        name="password"
        value={password}
        onChange={(e) => setPassword(e.target.value)}
      />
    </div>
  );
};

export default Hooks;
```
但是其實我們對於 form 中的每個字段都要做這些綁定，`useState` 什麼的不免覺得麻煩，而且如果應用中有不只一個 form，那簡直是太麻煩了。

這時候就體現出 hooks 的強大之處，我們要來自定義 hook。根據 react 中 hook 的命名規範，自定義的 hook 也都以 `use...` 開頭。

我們自定義一個 hook 叫做 `useForm`，如下：
```js
import { useState } from "react";

const useForm = (initValues) => {
  const [values, setValues] = useState(initValues);

  return [
    values,
    (e) => {
      setValues({
        ...values,
        [e.target.name]: e.target.value,
      });
    },
  ];
};

export default useForm;
```
有了這麼一個 hook 後我們就可以這麼使用：
```js
import React  from "react";
import useForm from "../../hooks/useForm";

const Hooks = () => {
  const [values, handleChange] = useForm({ email: "", password: "" });

  return (
    <div>
      <input
        type="email"
        name="email"
        value={values.email}
        onChange={handleChange}
      />
      <input
        type="password"
        name="password"
        value={values.password}
        onChange={handleChange}
      />
    </div>
  );
};

export default Hooks;
```
只需要一個 `useForm`，而不用一直 `useState`，而且更大的好處是，我們可以在應用中隨意的復用這個 `useForm` hook，使得我們的邏輯可以得到很好的復用，且不會與 UI 耦合在一起，大大提升代碼的結構性。

## useEffect
`useEffect` 是一個非常強大且重要的 hook。先來看看一個基本的 `useEffect` 應該長什麼樣子：
```js
useEffect(() => {
    // 邏輯
})
```
`useEffect` 可以做到類似 `componentDidMount` 和 `componentWillUnmount` 的效果，如果不清楚可以先去看看 [關於 React 組件生命週期](https://blog.csdn.net/weixin_46803507/article/details/116433318)。具體用法如下：
```js
useEffect(() => {
    // componentDidMount
    console.log('componentDidMount')

    // componentWillUnmount
    return () => {
        console.log('componentWillUnmount')
    }
})
```
我們來看看效果：
![](https://riyugo.com/o/210511/ae59w.gif)

當 Hooks 組件掛載完成會調打印出 `componentDidMount`，而當 Hooks 組件銷毀前會打印出 `componentWillUnmount`，這就是 `useEffect` 可以做到的其中一個功能，相當於 class component 的 `componentDidMount` 和 `componentWillUnmount` 鉤子。

如上我們已經知道了 `useEffect` 的基本功能，但 `useEffect` 當然不只如此。其實 `useEffect` 後面還可以接一個參數，是一個數組，這個數組中的就是所謂的 dependency，什麼意思呢？意思就是說，`useEffect` 還可以幫我們做到當 dependency 被更新過才會執行 `useEffect` 中的邏輯。來看看例子，馬上就明白了：
```js
import React, { useEffect, useState } from "react";
import useForm from "../../hooks/useForm";

const Hooks = () => {
  const [values, handleChange] = useForm({ email: "", password: "" });

  useEffect(() => {
      console.log('email change mount')
  }, [values.email])

  return (
    <div>
      <input
        type="email"
        name="email"
        value={values.email}
        placeholder="email"
        onChange={handleChange}
      />
      <input
        type="password"
        name="password"
        placeholder="password"
        value={values.password}
        onChange={handleChange}
      />
    </div>
  );
};

export default Hooks;
```
![](https://riyugo.com/o/210511/5brxp9.gif)
可以看到，現在只有當 effect `email` 輸入框的值發生變化才會觸發 `useEffect` 的邏輯。這其實就是 class component 的 `componentDidUpdate` 鉤子所做到的事情。

當然我們可以有很多 `useEffect`，這些 `useEffect` 都會順序執行。綜合上述，`useEffect` 可以說是 `componentDidMount`、`componentWillUnmount`、`componentDidUpdate` 這三個生命週期鉤子的合併，非常強大。

下面來看看一個實際的例子，異步請求數據，請求 url 為 <a>http://numbersapi.com/43/trivia</a>，再常見不過了。

首先我們先寫一個自定義的 hook：
```js
import { useEffect } from "react";

const useFetch = (url) => {
  useEffect(() => {
    fetch(url)
      .then((res) => res.text())
      .then((res) => console.log(res));
  }, [url]);
};

export default useFetch;
```
在具體組件中就可以直接使用，來看看效果：
![](https://riyugo.com/o/210511/yt3al.gif)

沒問題，那如果我們希望這個值返回給我們以便於渲染於頁面上，我們該怎麼做呢？看看下面代碼：
```js
import { useState, useEffect } from "react";

const useFetch = (url) => {
  const [result, setResult] = useState({ data: "", loading: false });

  useEffect(() => {
    setResult({
      data: "",
      loading: true,
    });
    fetch(url)
      .then((res) => res.text())
      .then((res) =>
        setResult({
          data: res,
          loading: false,
        })
      );

    // 你好好試的話會發現不加下面這段 unmount 代碼會有一個 warning，下面也會用另一個 hook 來解決這個問題

    // return () => {
    //   setResult({
    //     data: "",
    //     loading: false,
    //   });
    // };
  }, [url]);

  return result;
};

export default useFetch;
```
```js
import React, { useEffect, useState } from "react";
import useFetch from "../../hooks/useFetch";

const Hooks = () => {
  const { data, loading } = useFetch("http://numbersapi.com/43/trivia");

  return (
    <div>{loading ? "loading..." : data}</div>
  );
};

export default Hooks;
```
效果如下：
![](https://riyugo.com/o/210511/21b8w6.gif)

現在再來看看如果我動態的改變 url 的效果，更好的感受 `useEffect` 的強大之處：
```js
import React, { useEffect, useState } from "react";
import useFetch from "../../hooks/useFetch";

import { Button } from "antd";

const Hooks = () => {
  const [count, setCount] = useState(0);
  const { data, loading } = useFetch(`http://numbersapi.com/${count}/trivia`);

  return (
    <div>
      <Button type="primary" onClick={() => setCount((count) => count + 1)}>
        change url
      </Button>
      <div>{loading ? "loading..." : data}</div>
    </div>
  );
};

export default Hooks;
```
效果如下：
![](https://riyugo.com/o/210511/26uk0u.gif)

`useEffect` 做到每次 url 這個 dependency 被改變就會調用一次裡面的邏輯，重新獲取數據，渲染頁面。

最後再嘗試一個需求，加深一下 `useEffect` 感受。我現在希望，這個 count 值不要刷新就沒有，我希望即使刷新還是要是最近一次的 count，當然數據也要是相對應得。代碼實現如下：
```js
import React, { useEffect, useState } from "react";
import useFetch from "../../hooks/useFetch";

import { Button } from "antd";

const Hooks = () => {
  const [count, setCount] = useState(
    localStorage.getItem("count") === null
      ? 0
      : JSON.parse(localStorage.getItem("count"))
  );
  const { data, loading } = useFetch(`http://numbersapi.com/${count}/trivia`);

  useEffect(() => {
    // 緩存在 localStorage 中
    localStorage.setItem("count", JSON.stringify(count));
  }, [count]);

  return (
    <div>
      <Button type="primary" onClick={() => setCount((count) => count + 1)}>
        change url
      </Button>
      <div>{loading ? "loading..." : data}</div>
    </div>
  );
};

export default Hooks;
```
效果如下：
![](https://riyugo.com/o/210511/2iln4f.gif)

所以其實可以感覺到 `useEffect` 這個 hook 的強大之處，簡單來說整合了 `componentDidMount`、`componentWillUnmount`、`componentDidUpdate` 三個鉤子，以及 dependency 的控制，都給了我們很多彈性的空間，具體還要實際項目中試試看更有體會吧。

## useRef
接下來介紹的 hook 是 `useRef`。顧名思義，use Reference，其實這個 hook 就是可以幫我們獲得 react 組件或是說元素的引用。來看一個實際的例子，直接上代碼連用法一起看看：
```js
import React, { useEffect, useRef, useState } from "react";
import useForm from "../../hooks/useForm";

import { Button } from "antd";

const Hooks = () => {
  const [values, handleChange] = useForm({ email: "", password: "" });

  const emailRef = useRef();

  return (
    <div>
      <Button type="primary" onClick={() => emailRef.current.focus()}>
        focus email
      </Button>
      <div>
        <input
          type="email"
          name="email"
          value={values.email}
          placeholder="email"
          onChange={handleChange}
          ref={emailRef}
        />
        <input
          type="password"
          name="password"
          placeholder="password"
          value={values.password}
          onChange={handleChange}
        />
      </div>
    </div>
  );
};

export default Hooks;
```
效果如下：
![](https://riyugo.com/o/210511/krfflh.gif)

應該挺清楚的，就是這麼一個基本用法。通過 `useRef` hook 可以獲取相應的 DOM 對象或是 react 組件。使用時要通過 `.current` 來獲取相應的 ref 對象。

再來看一個例子，介紹 `useRef` 的第二個強大的地方。

其實是剛剛上面 `useEffect` 就提到過，我們自定義的 `useFetch` 鉤子。會發現如果我們在數據還在 loading 就銷毀這個組件，react 會報一個 warning:
```
Warning: Can't perform a React state update on an unmounted component. This is a no-op, but it indicates a memory leak in your application. To fix, cancel all subscriptions and asynchronous tasks in a useEffect cleanup function.
```
上面給出的一個方式就是在 `useEffect` 中加上 unmount 的邏輯，我們再來看看用 `useRef` 這個鉤子可以怎麼做：
```js
import { useState, useEffect, useRef } from "react";

const useFetch = (url) => {
  const [result, setResult] = useState({ data: "", loading: false });
  const isCurrent = useRef(true);

  useEffect(() => {
    return () => {
      isCurrent.current = false;
    };
  }, []);

  useEffect(() => {
    setResult({
      data: "",
      loading: true,
    });
    fetch(url)
      .then((res) => res.text())
      .then((res) => {
        if (isCurrent.current) {
          setResult({
            data: res,
            loading: false,
          });
        }
      });
  }, [url]);

  return result;
};

export default useFetch;
```
我們通過一個 ref 對象作為一個標記位，表示組件是否還存在，並且當組件要銷毀時將該標記位改成 false，並判斷如果為 true 我才改變 state。

這邊就來解釋 `useRef` 的第二個用處，保存數據。我們都知道，state 可以跨渲染週期，也就是說即使多次渲染組件 state 還是保持不變，但是一個問題是，一旦 state 改變就會引起組件的重新渲染。

但是 `useRef` 則不會，`useRef` 創建出來的 ref 對象同樣可以跨越渲染週期，但是它的改變是不會引起組件的重新渲染的，所以我們可以用 `useRef` 來保存一些數據，但同時不希望這些數據的任何改動影響到組件的渲染。            

## useLayoutEffect
這邊介紹一個 `useLayoutEffect` hook，其實這個 hook 並不是那麼常用，通常就用 `useEffect` 就可以解決大部分需求了，不過第一次學習完整點。

其實 `useLayoutEffect` 的函數簽名跟 `useEffect` 相同，但它會在所有的 DOM 變更之後同步調用 effect。可以使用它來讀取 DOM 布局並同步觸發重渲染。在瀏覽器執行繪制之前，`useLayoutEffect` 內部的更新計劃將被同步刷新。

- 與 `useEffect` 的區別

`useEffect` 渲染時是異步執行，並且要等到瀏覽器將所有變化渲染到屏幕後才會被執行。在本次更新完成後，再開啟一個任務調度，在下次任務調度中執行。

`useLayoutEffect` 是和 `componentDidMount`，`componentDidUpdate` 執行時機一樣，並且是同步執行的。也就是說是在瀏覽器將所有變化渲染到屏幕之前執行的。

- `useEffect` 使用時機

一般情况，把初始化请求放在 `useEffect`。因为 `useLayoutEffect` 的执行时机和 `componentDidMount`，`componentDidUpdate` 一样，其中的代码可能会导致代码长时间执行，导致页面无法渲染从而卡顿。所以一般情况下，我们会把初始化请求放在 `useEffect` 中，这样保证不会影响本次页面的渲染，性能更高一些。

- `useLayoutEffect` 的使用時機

有 DOM 操作时使用 `useLayoutEffect`。

DOM 执行 `useLayoutEffect` 是在浏览器将虚拟dom元素真实地更新到屏幕之前执行的，在 `useLayoutEffect` 中改变dom，其实就是在之前生成的虚拟dom树上更新修改，然后再渲染到屏幕上，这样还是只有一次回流、重绘。

如果放在 `useEffect` 里，`useEffect` 的函数会在组件渲染到屏幕之后执行（已经回流重绘一次），此时对 DOM 进行修改，会触发浏览器再次进行回流、重绘，增加了性能上的损耗。

## useCallback
我們直接從代碼上來看，然後引出 `useCallback` 這個 hook 的用途。
```js
const App = () => {
  const [count, setCount] = useState(0)

  return (
    <div>
      <Hello increment = {() => setCount(count + 1)}/>
      <div>count:{count}</div>
    </div>
  )
}

const Hello = React.memo(({ increment }) => {
  const renders = useRef(0)
  console.log('render', renders.current++)

  return (
    <button onClick={increment}>increment</button>
  )
})
```
先講一下關於 `React.memo` 這個東西，其實這個用法就是說只有當傳過來的 props 發生了改變，我才要重新渲染這個組件。所以只有當 `increment` 發生改變才會重新渲染 `Hello` 組件。

但是我們會發現，每次點擊 button，控制台都會打印出 `render xxx` 的信息。這是因為，每次的點擊都改變了 count 這個 state，所以 App 就會重新渲染，那當然，`increment` 也就相當於每次都會被重新創建一次，所以每次就都觸發了 props 的改變。

但是這樣問題在哪裡呢？問題在於其實 `increment` 根本沒有改變啊，真正改變的其實就是 count 而已，但是每次都還是引起了 Hello 組件的渲染。那我們來看看 `useCallback` 這個 hook 究竟能幫我們幹嘛！

其實跟 `useEffect` 有一點點像，`useCallback` 可以幫我們創建出根據 dependency 是否改變來重新創建具體代碼目標。有點抽象，直接來看看上面代碼怎麼通過 `useCallback` 改善：
```js
const App = () => {
  const [count, setCount] = useState(0)

  const increment = useCallback(() => {
    setCount(count => count + 1)
  }, [setCount])

  return (
    <div>
      <Hello increment = {increment}/>
      <div>count:{count}</div>
    </div>
  )
}

const Hello = React.memo(({ increment }) => {
  const renders = useRef(0)
  console.log('render', renders.current++)

  return (
    <button onClick={increment}>increment</button>
  )
})
```
上面通過 `useCallback` 創建出了 increment，所以這時候因為沒有依賴 count 所以就不會每次都渲染 Hello 組件了。注意要使用 updater，不然如果 dependency 仍然有 count，那還是沒用的。

總結一下關於 `useCallback` 這個 hook 的理解。在 react 中我們經常面臨一個子組件渲染優化的問題，尤其是在向子組件傳遞函數props 時，每次 render 都會創建新函數，導致子組件不必要的渲染，浪費性能，這個時候，就是 `useCallback` 的用武之地了，`useCallback` 可以保證，無論 render 多少次，我們的函數都是同一個函數，減小不斷創建的開銷，常用於 react 的性能優化。

其實 `useCallback` 這個 hook 可以說是 `useMemo` 的語法糖，下面馬上就會介紹 `useMemo`。

## useMemo
`useMemo` 這個 hook 可以用於計算屬性上，類似於 vue 中的 computed() 鉤子。

我們就從一個例子來看，假設我們今天想請求一個第三方數據，url 為 `https://raw.githubusercontent.com/ajzbc/kanye.rest/master/quotes.json`，並要計算出最長的字符串。看看我們可以怎麼做：
```js
import React, { useState } from "react";
import useFetch from "../../hooks/useFetch";

import { Button } from "antd";

const Hooks = () => {
  const [count, setCount] = useState(0);
  const { data } = useFetch(
    "https://raw.githubusercontent.com/ajzbc/kanye.rest/master/quotes.json"
  );

  const computeLongestWord = arr => {
    if (!arr) {
      return [];
    }

    console.log('computing longest word')

    let longestWord = "";
    JSON.parse(arr).forEach((sentence) => {
      sentence.split(" ").forEach((word) => {
        if (word.length > longestWord.length) {
          longestWord = word;
        }
      });
    });

    return longestWord;
  };

  return (
    <div>
      <Button type="primary" onClick={() => setCount((count) => count + 1)}>increment</Button>
      <div>count : {count}</div>
      <div>{computeLongestWord(data)}</div>
    </div>
  );
};

export default Hooks;
```
效果如下：
![](https://riyugo.com/o/210512/r71958.gif)

但其實我們這樣會發現一個問題，我們打開控制台看看，如果我們不斷增加 count 會發生什麼：

![](https://riyugo.com/o/210512/rccqsx.gif)

我們發現，我們每次 count 增加改變 state 所以引起多次的渲染，連帶也每次都重新計算了一遍 `computeLongestWord`，但是實際上我們的數據源根本沒改變，結果也當然不會改變，但卻每次都要重新計算，就很傻逼，浪費大量性能。

這時候就要用上 `useMemo` hook 了，使用 `useMemo` 接受第一個參數是一個回調函數，其中 return 的值就會存在目標變量上，第二個參數就是 dependency，表示只有當 dependecy 發生改變我們才要重新計算。來看看具體代碼：
```js
import React, { useMemo, useState } from "react";
import useFetch from "../../hooks/useFetch";

import { Button } from "antd";

const computeLongestWord = (arr) => {
  if (!arr) {
    return [];
  }

  console.log("computing longest word");

  let longestWord = "";
  JSON.parse(arr).forEach((sentence) => {
    sentence.split(" ").forEach((word) => {
      if (word.length > longestWord.length) {
        longestWord = word;
      }
    });
  });

  return longestWord;
};

const Hooks = () => {
  const [count, setCount] = useState(0);
  const { data } = useFetch(
    "https://raw.githubusercontent.com/ajzbc/kanye.rest/master/quotes.json"
  );

  const longestWord = useMemo(() => computeLongestWord(data), [data, computeLongestWord]);

  return (
    <div>
      <Button type="primary" onClick={() => setCount((count) => count + 1)}>
        increment
      </Button>
      <div>count : {count}</div>
      <div>{longestWord}</div>
    </div>
  );
};

export default Hooks;
```
來看看效果怎麼樣：
![](https://riyugo.com/o/210512/shl4ji.gif)

可以看到，現在不會每次都重新計算 `longestWord`，在邏輯上跟性能上都比較好。

對於用過 vue 的來說，我個人覺得 `useMemo` 就跟 `computed()` 勾子基本相同，主要就是用於計算屬性上，而在 react 中還能更加顯式的指定計算屬性的依賴數組。

## useReducer
這邊介紹 `useReducer` hook，其實跟 `useState` 也挺像的，或者可以說是一個 `useState` 的加強版。我們一樣從例子出發，來引出 `useReducer` 這個 hook，直接上代碼：
```js
import React, { useReducer } from "react";

import { Button } from "antd";

function reducer(state, action) {
  switch (action.type) {
    case "INCREMENT":
      return state + 1;
    case "DECREMENT":
      return state - 1;
    default:
      return state;
  }
}

const Hooks = () => {
  const [count, dispatch] = useReducer(reducer, 0);

  return (
    <div>
      <div>count : {count}</div>
      <Button type="primary" onClick={() => dispatch({ type: "INCREMENT" })}>
        increment
      </Button>
      <Button type="plain" onClick={() => dispatch({ type: "DECREMENT" })}>
        decrement
      </Button>
    </div>
  );
};

export default Hooks;
```
其實對 redux 熟悉的話，就會發現這跟 redux 的 reducer action 根本就一樣？對，簡單來說，`useReducer` 這個 hook 就是 redux 的窮人版，差別就在於 `useReducer` 的數據是屬於組件本身的，不像 redux 是一個頂層的單一數據源，可以共享給所有組件。如果還不熟悉 redux 歡迎先去看看 [淺析 React Redux 的概念以及使用](https://blog.csdn.net/weixin_46803507/article/details/116499240)，再回來看就很簡單了。

簡單來說就是，`useReducer` 接受一個 reducer 和一個 state 初值做為參數，並返回一個 state 以及一個 dispatch 函數用於分發 action，然後 action 就會到傳進去的 reducer 做狀態計算，進而改變 state。

下面再來看看我們用 `useReducer` 做一個簡單的 todolist 應用，通過輸入 todo 名稱添加，然後如果點擊也可以划掉去除，然後還要時時展示 todo 當前的數量。來看看代碼實現：
```js
import React, { useReducer, useState } from "react";

function todoReducer(state, action) {
  switch (action.type) {
    case "ADD_TODO":
      return {
        todos: [...state.todos, { text: action.todo, completed: false }],
        todosCount: state.todosCount + 1,
      };
    case "REMOVE_TODO":
      console.log(state.todos)
      return {
        todos: state.todos.map((todo, idx) =>
          idx === action.idx ? { ...todo, completed: true } : todo
        ),
        todosCount: state.todosCount - 1,
      };
    default:
      return state;
  }
}

const Hooks = () => {
  const [{ todos, todosCount }, dispatch] = useReducer(todoReducer, {
    todos: [],
    todosCount: 0,
  });
  const [todo, setTodo] = useState("");

  return (
    <div>
      <form
        onSubmit={(e) => {
          e.preventDefault();
          dispatch({ type: "ADD_TODO", todo });
          setTodo("");
        }}
      >
        <input
          type="text"
          value={todo}
          onChange={(e) => setTodo(e.target.value)}
        />
      </form>
      {todos.map((todo, idx) => {
        return (
          <div
            key={todo.text}
            onClick={() => dispatch({ type: "REMOVE_TODO", idx })}
            style={{
              textDecoration: todo.completed ? "line-through" : "",
            }}
          >
            {todo.text}
          </div>
        );
      })}
      <div>todosCount : {todosCount}</div>
    </div>
  );
};

export default Hooks;
```
具體效果如下：
![](https://riyugo.com/o/210513/fqqa7.gif)

可以看到，通過 `useReducer` 我們可以避免很多的 `setState`，我們只要把全部修改 state 的邏輯都放在 reducer 就好，然後由 action 去分發行為就好。

所以其實 `useState` 跟 `useReducer` 可以說是基本一樣的東西，不過當狀態相對複雜，使用 `useReducer` 會是更好的選擇，在維護上也更容易些。

那麼 `useReducer` 跟 redux 的選擇上面也提到過了，如果你這個 state 就只要給單一組件用，那使用 `useReducer` 就夠了，如果這個 state 是要共享給很多組件的，那使用 redux 才對。

## useContext
最後最後(真是太累寫不動了qwq)，介紹本篇最後一個 hook，`useContext`。

context 其實就是上下文的意思，所以簡單來說，`useContext` 這個 hook 就是允許創建上下文，並讓不同組件都可以獲取到該上下文的東西。先來看一個最最最基本的用法：

- UserContext.js
```js
import { createContext } from "react";

export const UserContext = createContext(null);
```
通過 `createContext` 我們可以創建出一個上下文對象。

- Hooks.jsx
```js
import React from "react";
import { BrowserRouter as Router, Link, Route } from "react-router-dom";
import About from "./About";
import Home from "./Home";
import { UserContext } from "../../context/UserContext";

const Hooks = () => {
  return (
    <Router>
      <div>
        <nav>
          <ul>
            <li>
              <Link to="/">Home</Link>
            </li>
            <li>
              <Link to="/about">About</Link>
            </li>
          </ul>
        </nav>

        <UserContext.Provider value="hello from context">
          <Route exact path="/" component={Home}></Route>
          <Route path="/about" component={About}></Route>
        </UserContext.Provider>
      </div>
    </Router>
  );
};

export default Hooks;
```
獲取到上下文對象後，我們可以通過 `xxxProvider` 的標籤將所有希望可以獲取上下文對象的組件包裹起來。

通過 `value` 相當於在 UserContext 這個上下文對象上註冊一個值 `hello from context`。

- Home.jsx
```js
import React, { useContext } from 'react'
import { UserContext } from '../../context/UserContext'

const Home = () => {
    const message = useContext(UserContext)

    return (
        <div>
            <h2>Home</h2>
            <div>{message}</div>
        </div>
    )
}

export default Home;
```
這樣一來，我們就可以在 Home 組件裡面通過 `useContext` 這個 hook 來獲取上下文對象 UserContext。

當然我們通過 value 要傳什麼都可以，但是要注意的是，如果在任意組件中改變了上下文的值，其他引用到上下文的組件也都會發生改變(阿就同一個上下文啊，廢話hhh)。

那這有什麼用，其實很有用的。一個典型場景就是用戶登陸登出，假設用戶登陸了那麼應該所有組件只要有跟用戶相關的數據都要顯示，當用戶登出了當然也要同步消失。這個就可以使用 `useContext` 來實現，下面給出代碼，不過其中一些部分就是模擬一下，其實也無關緊要：
```js
// 模擬一下而已
const login = async () => {
  return {
    id: 3,
    username: "cclin",
    email: "cclin@tx.qq",
  };
};

const Hooks = () => {
  const [user, setUser] = useState(null); // 頂層狀態
  const userProvider = useMemo(() => ({ user, setUser }), [user, setUser]);

  return (
    <Router>
      <div>
        <nav>
          <ul>
            <li>
              <Link to="/">Home</Link>
            </li>
            <li>
              <Link to="/about">About</Link>
            </li>
          </ul>
        </nav>

        <UserContext.Provider value={userProvider}>
          <Route exact path="/" component={Home}></Route>
          <Route path="/about" component={About}></Route>
        </UserContext.Provider>
      </div>
    </Router>
  );
};

const About = () => {
  // 訂閱頂層狀態
  const { user, setUser } = useContext(UserContext);

  return (
    <div>
      <h2>About</h2>
      <pre>{JSON.stringify(user, null, 2)}</pre>
      {user ? (
        <button
          onClick={() => {
            // async call logout
            setUser(null);
          }}
        >
          logout
        </button>
      ) : (
        <button
          onClick={async () => {
            const user = await login();
            setUser(user);
          }}
        >
          login
        </button>
      )}
    </div>
  );
};

const Home = () => {
  // 訂閱頂層狀態
  const { user, setUser } = useContext(UserContext);

  return (
    <div>
      <h2>Home </h2>
      <pre>{JSON.stringify(user, null, 2)}</pre>
      {user ? (
        <button
          onClick={() => {
            // async call logout
            setUser(null);
          }}
        >
          logout
        </button>
      ) : (
        <button
          onClick={async () => {
            const user = await login();
            setUser(user);
          }}
        >
          login
        </button>
      )}
    </div>
  );
};
```
來看一下效果：
![](https://riyugo.com/o/210513/2lnd1u.gif)

可以看到，通過在頂層的 Hooks 中存狀態信息，並讓 About, Home 通過 `useContext` 訂閱頂層狀態達到數據的一致性。

簡單來說，通過 `createContext` 和 `useContext` 可以做到跨越組件傳遞上下文，非常強大。配合 `useReducer`，將所有數據存在一個頂層組件中，並通過 `useContext` 進行對所有訂閱頂層組件的子組件的更新上下文，甚至就像是一個小範圍的 redux，足以見到 hooks 的強大。


# 結語
這篇詳細介紹了 react hooks 的概念，以及一些比較重要的 hooks 和基本用法，也附上了不少示例和展示 gif，個人認為還是算挺詳細的，對於初學者來說應該是很好的入門。這篇也僅僅是作為 react 初學者對於 hooks 淺淺的理解，可能對於一些用法還不是用得那麼 6，而且也要在實際項目中實際用代碼寫過才能感受到 class component 跟 function component + react hooks 到底有什麼區別。

希望這篇文章對不懂 hooks 的人有些幫助，若有錯誤也歡迎大老們多多指教！這篇太累了，先撤了～

# 參考
|參考|鏈接|
|:-----|:-----|
|React官方文檔|https://react.docschina.org/docs/hooks-intro.html|
|为什么会出现React Hooks?|https://segmentfault.com/a/1190000019974949|
|useEffect和useLayoutEffect的区别|https://blog.csdn.net/yingzizizizizizzz/article/details/107773111|
|React Hooks系列之useCallback|https://blog.csdn.net/weixin_43720095/article/details/104965398|
|React Hooks: useState 和 useReducer 有什么区别？|https://zhuanlan.zhihu.com/p/336837522|
|React Hooks究竟是什么呢？|https://zhuanlan.zhihu.com/p/79127886|


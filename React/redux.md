@[TOC](淺析 React Redux 的概念以及使用)

# 前言

學習 react 的過程中，就一直看到什麼 redux，後來稍微了解一點點之後，如果你跟我一樣有一點點使用過 vue 的經驗的話，我自己感覺和 vuex 的作用有點類似，所以個人認為是必須要學會的東西。然而發現 redux 其實是一塊有點難咬的骨頭，不是那麼隨隨便便看看文檔，百度一下就可以學會的東西，所以同樣的，希望通過寫篇博客的方式，學習好關於 redux 的基本知識點。

# 正文

在真正進入 redux 前，我想先引出，或是說先說說為什麼我們需要 redux，他又幫我們做了什麼？

## 為什麼需要 Redux?

先說，react 本身是一個非常輕量級的框架，其實根本不一定要用到 redux，在不需要使用 redux 的情況下硬要用只是增加應用的複雜度而已。

好的，試想如果今天要寫一個稍微複雜一點點的應用，肯定會有不少組件。那麼這些組件之間肯定會有一些數據要共享，像下圖情況，假設我們把組件的關係抽象成如下樹形結構，如果有一個上層組件需要用到下層組件的數據，那只用 react 寫會變得很麻煩很混亂，因為組件之間的關係導致傳值(props)太麻煩了。

這時候就是 redux 上場的時候了。我們可以通過 redux 幫我們將需要在多個組件之間共享的數據抽取出來到一個公共的 store 數據層，這樣所有需要對這些數據訪問的組件都只要和 store 打交道就好。
![](https://riyugo.com/o/210507/fp0owg.png)

這主要就是 redux 最強大的地方，關於 redux 其實是 flux 的一個實現，關於 flux 之後有機會會再寫篇博客。那大概知道 redux 在幹嘛了之後，接下來我們就來進入 redux 的世界吧！

## Redux 工作流程

下圖是官方文檔對 redux 的圖示：
![](https://riyugo.com/o/210507/fulk6j.png)

簡單來說，redux 就是一個數據層的框架，它把所有數據都存在 store 中，store 就是一個公共存儲數據的區域，組件可以從 store 中拿數據，也可以對 store 中的數據進行修改。

我們把 redux 整個流程比喻成圖書館的流程，個人覺得有助於理解：

`React component` 就是要借書的人，`action creators` 就是當我要借書時，會跟圖書館管理員說我要借什麼書，比如說『我要借 type 為 科幻類 name 為 哈利波特的書』，那麼這個語句的表達(也就是說這句話)，就是所謂的 `action`。

`Store` 就是圖書館管理員，他負責他負責整個圖書館所有書的管理。`Reducer` 則相當於是一個記錄本，因為圖書館管理員是沒辦法記住所有存書情況的，就需要這麼一個記錄本，來查一下你要借的書有沒有，在哪裡之類的。

轉換成代碼理解如下：

- 獲取數據

組件要去獲取 Store 裏的數據，跟 Store 說我要獲取數據這句話就是 Action Creators，Action Creators 創建了這句話(action)之後告訴 Store，Store 接收了你要獲取數據這句話後，Store 並不知道你要什麼樣的數據，它得去查一下應該給你什麼樣的數據，到底給你什麼樣的數據。Reducer 知道應該給組件什麼樣的數據，Reducer 會按照 action 告訴 Store 應該給組件什麼樣的數據，Store 知道後會把對應的數據給到組件。

- 修改數據

如果想改變 Store 裏的數據，組件先要跟 Store 說我要改變一個數據，一樣通過 Action Creators 先說一句話(action)我要改變一個數據，Store 接收了你這句話知道你要改變數據了可是它不知道應該怎麼幫助你去改變數據，它就會去問 Reducer ，Reducer 接收了你傳過來的這句話之後，Reducer 會告訴 Store 的數據應該如何被修改，Store 修改好數據之後就會告訴組件數據修改完了你可以重新來獲取數據了，這樣組件再重新獲取最新的數據。

## Redux 準備

要使用 redux 首先當然先安裝點東西啦～

```js
// npm
npm install redux
npm install react-redux

// yarn
yarn add redux
yarn add react-redux
```

下面的會簡單用 redux 開發一個 counter 計數器應用，會通過 create-react-app 所建立的 react 項目進行介紹，如果還不會使用 create-react-app 的人歡迎先去看看 [React 教程之使用 create-react-app 构建你的第一个 react 应用](https://blog.csdn.net/moshowgame/article/details/91970581)。

## Store

Store 就是所有數據存儲的地方，在 react 中為一個單一數據源，你整個應用就只能有一個 store。

我們通過 Redux 提供了一個函數 `createStore()` 來創建 store。

```js
// index.js

import { createStore } from "redux";
const store = createStore(
    /* function : reducer */
);
```

`createStore` 其實裡面要放一個函數的，下面我們會再提到。

如果我們想得到某一時間點的 store 數據，Redux 也提通了一個函數 `getState`:

```js
const state = store.getState();
```
創建好 store 之後，還有一個東西 `Provider`。這是 react-redux 提供的一個組件，作用就是使其包裹的子組件都可以拿到 store 中的 state 數據。
```js
// index.js
import { Provider } from 'react-redux';
import { createStore } from "redux";
const store = createStore(
    /* function : reducer */
);

ReactDOM.render(
  <Provider store={store}>
    <App />
  </Provider>,
  document.getElementById('root')
);
```
對於想要在任意組件中獲取某個 state 的數據，可以這麼使用：
```js
import { useSelector } from "react-redux";

export default function App() {
    const count = useSelector((state) => state.counter.count)
    return (
        <div>
            <h1>{count}</h1>
        </div>
    )
}
```

## Action

剛剛上面講到了 action，那我們馬上就來看看到底 action 是什麼東西。

state 的變化會導致 view 的變化，但是用戶其實接觸不到 state，用戶只跟 view 打交道，所以當然 state 的變化就必須由 view 導致。

其實 action 就只是一個簡單的 js 對象，其中有一個 `type` 屬性是必須的，表示的就是這個 action 的名稱，reducer 也就是根據這個 `type` 決定具體怎麼操縱數據，下面講到 reducer 還會說。

```js
const action = {
  type: "INCREMENT",
  payload: 5,
};
```

上面就是一個 action，action 的名稱是 `INCREMENT`，且攜帶一個信息是數字 5。

可以理解為，action 就是描述一下你希望發生的事情，是改變 store 的唯一辦法，它可以運送數據到 store 去。

## Action Creator

對於用戶而言，可能要通過 view 發送很多的 action，如果要一個一個自己寫就太麻煩了，所以 action creator 所做的就是為我們生成對應的 action，這個函數就叫做 action creator。

其實 action creator 所做就是返回一個 action。

```js
// /actions/index.js

export const increment = (payload) => {
  return {
    type: "INCREMENT",
    payload,
  };
};

export const decrement = (payload) => {
  return {
    type: "DECREMENT",
    payload,
  };
};
```

我們就直接在新建的 actions.js 中寫了兩個 action，一個(increment)希望為 store 中的一個值遞增，另一個(decrement)遞減。

那到底 view 怎麼分發 action 呢？Redux 提供了一個 `dispatch()` 函數，也可以說是由 view 發起 action 的唯一途徑，接受的參數就是一個 action，如下：

```js
import { createStore } from "redux";
const store = createStore();
/* function : reducer */

store.dispatch({
  type: "INCREMENT",
  payload: 5,
});
```

結合 action creator，可以寫成如下：

```js
store.dispatch(increment(5));
```

在具體組件裡面也可以如下使用：

```js
// Counter.jsx
import { useDispatch } from "react-redux"
import { increment, decrement } from "../../store/actions/index"

export default function Counter() {
    const dispatch = useDispatch()
    return (
        // 舉例來說
        <div>
            <Button onClick={dispatch(increment())}>+</Button>
            <Button onClick={dispatch(decrement())}>-</Button>
        </div>
    )
}
```
## Reducer

### 基本概念

當 store 收到一個 action 後，必須要返回一個新的 state，這樣 view 才會變化。Reducer 之所以叫 reducer 就是描述 state 這樣一個計算的過程。Reducer 是函數，接受當前 state 和 action 做為參數，並且要返回一個新的 state。
```js
const reducer  = (state, action) => {
    // ...
    // return newState
}
```
以計數器為例子，我們來看看 reducer 可以怎麼寫：
```js
// /reducers/counter.js

const defaultState = {
    count: 0
}

const counterReducer = (state = defaultState, action) => {
    switch(action.type) {
        case 'INCREMENT':
            return state.count + 1
        case 'DECREMENT':
            return state.count - 1
        default:
            return state
    }
}

export default counterReducer;
```
這邊回到上面 store，其實上面一開始提到通過 `createStore()` 創建 store，但是裡面說要傳進去一個參數，一個函數。其實就是要把 reducer 傳給 `createStore`，這樣以後只要用戶向 store 發來一個 action，store 就會自動調用該 reducer，就知道要怎麼按 action 操作 state。

可以看到，counterReducer 根據 action 的 `type` 做了相應的邏輯處理並返回了不同的 newState。

### 使用 `Object.assign()`

上面我們確實完成了一個簡單的 reducer，不過這樣的 reducer 有什麼不好呢？

> 我們不希望直接修改 state

所以我們通過 `Object.assign()` 建立一個副本。而且注意不能 `Object.assign(state, {...})`，因為 `Object.assign()` 是會將後面的對象合併然後改變第一個參數的值的，所以我們應該要將第一個參數設為 `{}`，`Object.assign({}, state, {...})`。

```js
// /reducers/counter.js

const defaultState = {
    count: 0
}

const counterReducer = (state = defaultState, action) => {
    switch(action.type) {
        case 'INCREMENT':
            return Object.assign({}, state, {
                count: state.count + 1
            })
        case 'DECREMENT':
            return Object.assign({}, state, {
                count: state.count - 1
            })
        default:
            return state
    }
}

export default counterReducer;
```

### 拆分 reducer

不過在實際在項目中，我們可能會有多個 reducer，可能對應不同業務對象，而且隨著需求增長，reducer 函數會變得很大，也就是說可能會有許多的文件裡面都定義了不同的 reducer，但是其實 `createStore()` 只能傳一個 reducer 進去。

為此，Redux 也提供了一個函數 `combineReducers()`，這邊就來看看怎麼做到拆分 reducers，可以讓代碼不那麼凌亂(或許其實還有更好的組織方式，後面陸續更新)。

假設今天還有一個 reducer，是針對用戶登陸與否的一個標示判斷，於是我在 /reducers/user.js 下再新增了一個文件專門用來處例用戶相關的 reducer 狀態管理。
```js
// /reducers/user.js

const defaultState = {
    isloggedIn: false
}

const isloggedIn = (state = defaultState, action) => {
    switch(action.type) {
        case 'SIGN_IN':
            return Object.assign({}, state, {
                isloggedIn: !state.isloggedIn
            })
        default:
            return state
    }
}

export default isloggedIn;
```
那連同上面的 /reducers/counter.js 現在有兩個 reducer，這時候我們就再建立 /reducers/index.js，這就是根 reducer 文件，在這裡面就用到了 `combineReducers`。

`combineReducers` 簡單來說做的就是把多個 reducer 合併成一個大的 reducer。
```js
// /reducers/index.js

import counterReducer from './counter';
import isloggedIn from './user';
import { combineReducers } from 'redux';

const allReducers = combineReducers({
    counterReducers,
    isloggedIn
})

export default allReducers;
```
接下來我們就可以把一開始 store 的代碼補齊了：
```js
// index.js

import { createStore } from 'redux';
import allReducers from "./store/reducers/index";
const store = createStore(
  allReducers
); 
```
# 計數器完整代碼
```js
// /actions/index.js

export const increment = () => {
    return {
        type: 'INCREMENT'
    }
}

export const decrement = () => {
    return {
        type: 'DECREMENT'
    }
}
```
```js
//  /reducers/counter.js
const defaultState = {
    count: 0
}

const counterReducer = (state = defaultState, action) => {
    switch(action.type) {
        case 'INCREMENT':
            return Object.assign({}, state, {
                count: state.count + 1
            })
        case 'DECREMENT':
            return Object.assign({}, state, {
                count: state.count - 1
            })
        default:
            return state
    }
}

export default counterReducer;

//  /reducers/user.js
const defaultState = {
    isloggedIn: false
}

const isloggedIn = (state = defaultState, action) => {
    switch(action.type) {
        case 'SIGN_IN':
            return Object.assign({}, state, {
                isloggedIn: !state.isloggedIn
            })
        default:
            return state
    }
}

export default isloggedIn;

//  /reducers/index.js
import counterReducer from './counter';
import isloggedIn from './user';
import {combineReducers} from 'redux';

const allReducers = combineReducers({
    counter: counterReducer,
    loggedin: isloggedIn,
})

export default allReducers;
```
```js
// index.js

import React from 'react';
import ReactDOM from 'react-dom';
import './index.css';
import App from './App';
import reportWebVitals from './reportWebVitals';

import { Provider } from 'react-redux';
import { createStore } from 'redux';
import allReducers from "./store/reducers/index";

const store = createStore(
  allReducers,
  window.__REDUX_DEVTOOLS_EXTENSION__ && window.__REDUX_DEVTOOLS_EXTENSION__()
);

ReactDOM.render(
  <Provider store={store}>
    <App />
  </Provider>,
  document.getElementById('root')
);

// If you want to start measuring performance in your app, pass a function
// to log results (for example: reportWebVitals(console.log))
// or send to an analytics endpoint. Learn more: https://bit.ly/CRA-vitals
reportWebVitals();
```
```js
// ReduxExample.jsx

import React from "react";
import { useSelector, useDispatch } from "react-redux";
import { increment, decrement } from "../../store/actions/index";
import "../style/ReduxExample.css";

export default function Redux() {
  const count = useSelector((state) => state.counter.count);
  const dispatch = useDispatch();

  return (
    <div className="ReduxBox">
      <h1>{count}</h1>
      <Button type="primary" onClick={() => dispatch(increment())}>
        +
      </Button>
      <Button onClick={() => dispatch(decrement())}>-</Button>
    </div>
  );
}
```

# 結語
本篇大致上介紹了 redux 的相關概念，其實真的相對還是很基本的。其實有些人可能會發現上面都沒有提到如果想要做到異步 action 該怎麼做，這也是很重要的一個 redux 的部分，貌似有一些實現方案，中間件、redux-saga、thunk 之類的，暫時也還不會。不過之後慢慢學習都會陸續記錄下來這些東西，畢竟學習是一個漫長的過程，痛並快樂著(個屁)。

# 參考

| 參考                   | 鏈接                                                                              |
| :--------------------- | :-------------------------------------------------------------------------------- |
| Redux 官方文檔         | https://redux.js.org/tutorials/essentials/part-1-overview-concepts                |
| Redux 阮一峰的网络日志 | https://www.ruanyifeng.com/blog/2016/09/redux_tutorial_part_one_basic_usages.html |
|MDN 官方文檔|https://developer.mozilla.org/zh-CN/docs/Web/JavaScript/Reference/Global_Objects/Object/assign|


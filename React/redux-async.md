@[TOC](Redux 中間件以及異步 action)

# 前言
前幾天寫過一篇關於 redux 基礎的使用以及概念，若不清楚可以先去看看 [淺析 React Redux 的概念以及使用](https://blog.csdn.net/weixin_46803507/article/details/116499240)，但是其實會發現，那篇裡面的 action 都是同步的。本篇就要來談談 redux 的異步 action。

# 正文
在進入 redux 的異步 action 之前，先來說說中間件，因為這是進入異步 action 的一大門檻。

## 中間件(middleware)
我們都知道，redux 中最重要的就是三個概念：

- store
- action
- reducer 

再次附上官方對於 redux 數據流的示意圖：

![](https://riyugo.com/o/210507/fulk6j.png)

簡單來說就是由 UI 發起 action，而 action 會通過 dispatch 這個動作被分發給 reducer，然後 reducer 才是具體知道要怎麼改變 store 裡的數據的角色。

### 中間件概念
那什麼是中間件呢？其實中間件就是在 action 被 dispatch 到 reducer 之前的一個攔截器。來看看下圖，非常清楚：

![](https://riyugo.com/o/210509/otqqwa.png)

所以其實中間件可以做很多的事情，我們可以在 action 被分發之前打印一些類似日誌之類的提示，我們可以改變我們的 action，可以改變 action 中的 payload，甚至可以 drop 掉 action。中間件給了我們很多在 action 具體被分發到 reducer 之前很多自行操作的空間。

如圖所示，我們可以有很多個 middleware，就像一個管道一樣，如果有多個 middleware，則第一個中間件會處理完後會將 action 繼續交由下一個中間件，直到最後一個中間件就會把 action 交給 reducer。

那麼怎麼使用中間件呢？一個中間件如下：
```js
const middleware = store => {
    return next => {
        return action => {
            // 具體中間件的邏輯
        }
    }
}
```
首先看到這是一個三層函數，最外層函數返回一個函數，依序。其中這三個參數的含義跟作用如下：

- `store`：store 就是指 redux 中的 store，這邊我們可以獲取 store
- `next`：next 用於將 action 分發下去。<strong>這邊注意</strong>，如果自定義中間件一定要記得將 action 分發下去，不然就相當於 drop action，也就表示 action 根本永遠都不會到達 reducer
- `action`：action 指的是用戶發起的 action。一旦使用的中間件，則每一次發起 action 都會在到達 reducer 之前被中間件攔截

利用一點 ES6 提供的語法糖，我們可以稍稍簡化上面中間件的樣子：
```js
const middleware = store => next => action => {
    // 具體中間件的邏輯
}
```

下面我們來實際看看中間件的代碼，感受一下中間件能做什麼。我們以之前開發過的計數器應用為例，還是先附上計數器的代碼，同樣也是用 redux 開發的。

- 計數器完整代碼

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
import 'antd/dist/antd.css';
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

import { Button } from "antd";

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
效果如下：
![](https://riyugo.com/o/210509/qjr2eb.gif)

那麼假設我今天想要做一個功能，我希望我的 counter 在累加到 10 之後就不再累加上去，且如果繼續按 `+` 反而還要往回減。(我知道這個功能很智障，但就只是看看中間件的使用方式和效果而已)。

這時候我們就可以依賴中間件去做到這件事情，因為利用中間件我們就可以在用戶 dispatch action 到 reducer 之間攔截起來，添加我們自己的邏輯。自定義中間件如下：

```js
const capAtTen = store => next => action => {
    const count = store.getState().counter.count
    if(count >= 10) {
        // 利用 next 分發一個新的 action 給 reducer
        next({type: 'DECREMENT'})
    }
    // 若 count < 10，則仍分發原本的 action
    next(action)
}
```
### applyMiddleware()
很好，中間件寫完了。讓這個中間件生效的方式就是，我們借助 redux 提供的 `applyMiddleware` 函數，具體用法就是放在 `createStore` 中做為參數，如下：
```js
import { createStore, applyMiddleware } from 'redux';
import allReducers from "./store/reducers/index";

const store = createStore(
    allReducers,
    applyMiddleware(capAtTen)
)
```
這邊要注意的是，上面有提到我們可能會有不只一個中間件，所以在 `applyMiddleware` 中的參數順序是有影響的，參數順序就決定了中間件處理的順序。

來看看添加了 `capAtTen` 這個中間件的效果：
![](https://riyugo.com/o/210509/qxdaiq.gif)

可以看到我們確實在 count 達到 10 之後若仍然繼續點擊 `+` 更改了 action。

### redux-logger
這邊推薦一個 redux-logger library，這個庫可以為我們很清楚的打印出分發的 action 以及 state 的前後變化，個人認為是個非常棒的工具。在這邊介紹則是因為 redux-logger 中提供的 `logger` 實際上也是個中間件。
```js
// npm 
npm install redux-logger

// yarn 
yarn add redux-logger
```
具體用法也很簡單：
```js
// index.js

import { logger } from 'redux-logger';

const store = createStore(
  allReducers,
  applyMiddleware(logger, capAtTen),
);
```
## 異步 action
終於講完了中間件，現在可以進入本篇的重點了。下面我們會一個簡單的異步請求功能為場景，說明如何在 redux 中做到異步請求。我們想要展示所有 posts 的標題，而這些數據我們要向 jsonplaceholder 這個第三方平台請求，請求的 url 為 `https://jsonplaceholder.typicode.com/posts`。

我們先寫好 reducer，如下：
```js
const defaultState = []

const postReducer = (state = defaultState, action) => {
    switch(action.type) {
        case 'FETCH_POSTS':
            return action.payload
        default:
            return state
    }
}

export default postReducer
```
接著才是重點，我們來編寫 action creator。

上述就很明顯是一個異步請求。其實想要做到在 redux 中分發異步請求，我們可能第一個想到的就是把我們的 action creator 做一點修改，加上 `async` 關鍵字，期望分發的是一個異步函數。
```js
export const fetchPosts = async () => {
    const res = await axios.get('https://jsonplaceholder.typicode.com/posts')
    return {
        type: 'FETCH_POSTS',
        payload: res.data
    }
}
```
但是這樣你會發現 react 會直接崩掉，會報以下錯：
```
Error: Actions must be plain objects. Use custom middleware for async actions.
```
報錯的原因就是因為 react 跟我們說 action 必須只能返回一個 js 對象，但我們沒有。你會說有啊，看起來有啊！那我們就來證明沒有，我們將之前正確的 action 放到 babel 解釋器中，轉換如下：

![](https://riyugo.com/o/210509/12ty14l.png)

沒問題，確實是返回一個 js 對象。那我們把上面報錯的 action 丟進去會轉換成什麼呢？

![](https://riyugo.com/o/210510/3xb2f.png)

可以看到，在 switch 語句中不一定會返回一個 js 對象，有可能會會返回一個 axios 對象。這在 redux 的 action 是不允許的。

所以很明顯這樣做是不行的。其實在 redux 中想做到異步請求，我們必須借助 `redux-thunk`。

### redux-thunk
redux-thunk 其實也是一個中間件，它使我們的 action 不僅能夠返回一個 js 對象，還能返回一個 function。因此我們可以把 `fetchPosts` 改成如下：
```js
export const fetchPosts = () => {
    return async (dispatch, getState) => {
        const res = await axios.get('https://jsonplaceholder.typicode.com/posts')
        console.log('fetchPost', res.data)
        dispatch({
            type: 'FETCH_POSTS',
            payload: res.data
        })
    }
}
```
這樣一來就大功告成了！利用 redux-thunk 我們返回一個 async function，然後在裡面通過 `dispatch` 分發 action，最後才會到 reducer。來看看效果：
![](https://riyugo.com/o/210510/91usn.png)

# 結語
這篇介紹了關於 redux 中間件的概念，以及如何在 redux 中正確的完成異步的調用，在實際前後端分離的 web 開發中是很常見的需求。後面也會陸續再更新關於 react 更多學習的心得，如果本篇有誤歡迎指正，也希望看完對你有一點點幫助！

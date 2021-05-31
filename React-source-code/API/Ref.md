@[TOC](React 源碼解析 - API 系列 - React.createRef/forwardRef)

# 前言
這篇一樣繼續來分析 react 中的 `createRef()` 以及 `forwardRef()` 這兩個 api，首先也會先說說 ref 的概念以及在 react 中的使用方式，然後再來分析 react 中關於這兩個方法的源碼實現。

# 正文
首先先來看看 ref 在 react 中的基本用法以及一些概念，當然如果熟悉的人就跳過直接看源碼吧！

## 關於 ref
其實 ref 也就是 reference 的縮寫，就是個引用。我們可以來看看 react 中關於 ref 有哪些重要的概念以及用法吧～

### ref 的由來
在典型的 react 數據流中，或者說是 react 標榜推崇的數據流模式中，props 是父組件與子組件交互的唯一方式。要修改一個子組件，你需要使用新的 props 來重新渲染它，但是，在某些情況下，你需要在典型數據流之外強制修改子組件/元素。

我們來看看 react ref 具體有哪些用法：

在 react 16.3 以前比較常使用的，通常 ref 可以通過綁定字符串(string ref)或是回調函數(callback ref)獲取。

### string ref
```js
// string ref

class App extends React.Component {
    componentDidMount() {
        this.refs.myRef.focus()
    }

    render() {
        return <input ref="myRef" />
    }
}
```
這是最原始的 ref 使用方式，可是這種方式即將被淘汰，而且官方也說得很清楚：

> 如果你目前還在使用 `this.refs.textInput` 這種方式訪問 refs ，我們建議用回調函數或 createRef API 的方式代替。

具體壞處參考這篇 [一文搞懂 React ref](https://blog.csdn.net/qianyu6200430/article/details/110508114)，寫得還算清楚。

### callback ref
```js
// callback ref

class App extends React.Component {
    componentDidMount() {
        this.myRef.focus()
    }

    render() {
        return <input ref={ele => this.myRef = ele} />
    }
}
```
這邊說下對 callback ref 的理解，因為會與 `createRef` 牽涉到一些比較，搞懂下還是好點。

其實 react 在掛載組件之前，會調用 ref 的回調函數並傳入目標 dom 元素，當卸載組件時同樣會再次調用，不過這時候參數變為 null。在 `ComponentDidMount` 以及 `ComponentDidUpdate` 之前，react 會保證所有 ref 都是最新的。

callback ref 的使用方式也會影響到 react 的性能。如果我們以內聯函數的形式直接定義在 dom 元素上，那在組件更新的過程中這個回調函數會被執行兩次。因為 react 更新組件相當於先卸載再掛載，所以每次更新 react 都要先傳入 null 清空舊組件(第一次)，然後再傳入 dom 元素進行新的函數實例的掛載(第二次)。

我們通過將 ref 回調函數綁定在 class 上的方式可以避免這種問題，不過其實大部分情況下無所謂的。整體來說，callback ref 是比較推薦的，且性能上也比較好的方式。

### createRef
在 react 16.3 之後，新的提案引入了一個新的 api，也就是本文的主角 - `createRef`。來看看具體怎麼用：
```js
// createRef

class App extends React.Component {
    constructor(props) {
        super(props)
        this.myRef = React.createRef()
    }
    
    componentDidMount() {
        this.myRef.current.focus()
    }

    render() {
        <input ref={this.myRef} />
    }
}
```
`createRef` 優點是相對於 callback ref 更加直觀些，也避免了一些對於回調函數理解上的問題。createRef 缺點則是性能略低於 callback ref。

通過 `createRef` 創建的 ref 的值會根據節點類型而有所不同：

當 ref 屬性用於 HTML 元素時，構造函數中使用 React.createRef() 創建的 ref 接收底層 DOM 元素作為其 current 屬性。

- 當 ref 屬性用於自定義 class 組件時，ref 對象接收組件的掛載實例作為其 current 屬性。

- 默認情況下，你不能在函數組件上使用 ref 屬性（可以在函數組件內部使用），因為它們沒有實例。

- 如果要在函數組件中使用 ref，你可以使用 `forwardRef`（可與 `useImperativeHandle` 結合使用）或者可以將該組件轉化為 class 組件。下面我們也會對 `forwardRef` 做了解。

### useRef
上面了解了三種基本使用 ref 的方式，我們可以很方便的取得 dom 元素的引用，並做一些修改或是動作。不過其實要知道，我們是不可以在 class 組件中用上面的三種方式使用 ref 的，因為上面三種使用方式其實都是建立在有狀態的組件中，也可以看到都有 `this` 的出現，但其實在函數組件中根本沒有 `this` 的概念，沒有實例。

`useRef` 其實是一個 hook，其實也越來越提倡函數組件配合 react hooks，關於 hooks 可以去看看 [關於 React 中的 Hooks - 完全上手指南](https://blog.csdn.net/weixin_46803507/article/details/116727901)，非常詳細，就不在這邊展開來說。

### forwardRef
在正式進入源碼分析之前還是要先知道怎麼用(廢話)。看下面一個例子：

```js
function App() {
    const inputEl = useRef(null)
    const btnClick = () => {
        inputEl.current.focus()
    }

    return (
        <>
            <input ref={inputEl} type="text"/>
            <button onClick={btnClick}>focus</button>
        </>
    )
}
```
也沒什麼好展示的，總之就是按個按鈕可以觸發輸入框的焦點事件。

當我們點擊 button，先通過 `useRef` 創建一個 ref 對象inputEl，然後再將 inputEl 賦值給 input 的 ref，最後，通過 `inputEl.current.focus()` 就可以讓 input 聚焦。
然後，我們再想下，如果 input 不是個普通的 dom 元素，而是個組件，該怎麽辦呢？

這就牽扯到另外一個 api - `forwardRef`。

修改下上面的代碼，我們通過 `forwardRef` 把 input 單獨封裝成一個組件：
```js
const InputWithFocusButton = React.forwardRef((props, ref) => {
    return <input ref={ref} type="text"/>
})
```
再來看看如何使用這個組件：
```js
function App() {
    const inputEl = useRef(null)
    const btnClick = () => {
        inputEl.current.focus()
    }

    return (
        <>
            <InputWithFocusButton ref={inputEl} />
            <button onClick={btnClick}>focus</button>
        </>
    )
}
```
可以看到 `React.forwardRef` 接受一個渲染函數，其接收 `props` 和 `ref` 參數並返回一個 `ReactNode`。
這樣我們就可以將父組件中創建的 ref 轉發進子組件，並賦值給子組件的 input 元素，進而可以調用它的 focus 方法。

看到這邊，通過 `useRef` + `forwardRef`，我們就可以在函數式組件中使用 ref 了，我們再堅持一下，下面還要介紹最後一個重要的知識點 `useImperativeHandle`，結合上面兩個 api，讓你的代碼更加完美。

### useImperativeHandle
`useImperativeHandle` 其實也是一個 hook，不過之前學習 hooks 的時候漏掉了。

有時候，我們可能不想將整個子組件暴露給父組件，而只是暴露出父組件需要的值或者方法，這樣可以讓代碼更加明確。而`useImperativeHandle` 這個 api 就是幫助我們做這件事的。

```js
useImperativeHandle(ref, createHandle, [deps])
```
- ref：定義了 current 對象的 ref 
- createHandle：一個函數，返回值是一個對象，即子組件希望對外暴露的事件
- 對象 [deps]：即依賴列表，當監聽的依賴發生變化，`useImperativeHandle` 才會重新將子組件的實例屬性輸出到父組件 
- ref 的 current 屬性上，如果為空數組，則不會重新輸出。

接續上面 `forwardRef` 的例子來看看怎麼配合 `useImperativeHandle`，以及為什麼這樣更好：
```js
import React, { useRef, useImperativeHandle } from 'react';
import ReactDOM from 'react-dom';

const FancyInput = React.forwardRef((props, ref) => {
  const inputRef = useRef();
  useImperativeHandle(ref, () => ({
    focus: () => {
      inputRef.current.focus();
    }
  }));

  return <input ref={inputRef} type="text" />
});

const App = props => {
  const fancyInputRef = useRef();

  return (
    <div>
      <FancyInput ref={fancyInputRef} />
      <button
        onClick={() => fancyInputRef.current.focus()}
      >focus</button>
    </div>
  )
}

ReactDOM.render(<App />, root);
```
上面這個例子中與直接轉發 ref 不同，直接轉發 ref 是將 `React.forwardRef` 中函數上的 ref 參數直接應用在了返回元素的 ref 屬性上，其實父、子組件引用的是同一個 ref 的 current 對象，官方不建議使用這樣的 ref 透傳和共用。

而使用 `useImperativeHandle` 後，可以讓父、子組件分別有自己的 ref，通過 `React.forwardRef` 將父組件的 ref 透傳過來，通過 `useImperativeHandle` 方法來自定義開放給父組件的 current。

`useImperativeHandle` 的第一個參數是定義 current 對象的 ref，第二個參數是一個函數，返回值是一個對象，也就是這個 ref 的 current 對象。這樣可以像上面的案例一樣，通過自定義父組件的 ref 來使用子組件 ref 的某些方法。


## React.createRef
上面介紹了那麼多關於 ref 的用法，終於可以進入 `React.createRef` 的源碼分析了。

### 源碼

- 源碼地址：https://github.com/facebook/react/blob/master/packages/react/src/ReactCreateRef.js

```js

/**
 * Copyright (c) Facebook, Inc. and its affiliates.
 *
 * This source code is licensed under the MIT license found in the
 * LICENSE file in the root directory of this source tree.
 * @flow
 */

import type {RefObject} from 'shared/ReactTypes';

// an immutable object with a single mutable value
export function createRef(): RefObject {
  const refObject = {
    current: null,
  };
  if (__DEV__) {
    Object.seal(refObject);
  }
  return refObject;
}
```
### 分析

#### RefObject
其實可以看到整體來說 `createRef` 的邏輯相對簡單，就是返回一個帶有 `current` 屬性的 `refObject`，類性為 `RefObject`，那我們當然也要來看看 `RefObject` 是什麼～

```js
export type RefObject = {|
  current: any,
|};
```
其實也沒什麼，就是定義一個類型而已。

## React.forwardRef

### 源碼

- 源碼地址：https://github.com/facebook/react/blob/master/packages/react/src/ReactForwardRef.js

```js
/**
 * Copyright (c) Facebook, Inc. and its affiliates.
 *
 * This source code is licensed under the MIT license found in the
 * LICENSE file in the root directory of this source tree.
 */

import {REACT_FORWARD_REF_TYPE, REACT_MEMO_TYPE} from 'shared/ReactSymbols';

export function forwardRef<Props, ElementType: React$ElementType>(
  render: (props: Props, ref: React$Ref<ElementType>) => React$Node,
) {
  if (__DEV__) {
    if (render != null && render.$$typeof === REACT_MEMO_TYPE) {
      console.error(
        'forwardRef requires a render function but received a `memo` ' +
          'component. Instead of forwardRef(memo(...)), use ' +
          'memo(forwardRef(...)).',
      );
    } else if (typeof render !== 'function') {
      console.error(
        'forwardRef requires a render function but was given %s.',
        render === null ? 'null' : typeof render,
      );
    } else {
      if (render.length !== 0 && render.length !== 2) {
        console.error(
          'forwardRef render functions accept exactly two parameters: props and ref. %s',
          render.length === 1
            ? 'Did you forget to use the ref parameter?'
            : 'Any additional parameter will be undefined.',
        );
      }
    }

    if (render != null) {
      if (render.defaultProps != null || render.propTypes != null) {
        console.error(
          'forwardRef render functions do not support propTypes or defaultProps. ' +
            'Did you accidentally pass a React component?',
        );
      }
    }
  }

  const elementType = {
    $$typeof: REACT_FORWARD_REF_TYPE,
    render,
  };
  if (__DEV__) {
    let ownName;
    Object.defineProperty(elementType, 'displayName', {
      enumerable: false,
      configurable: true,
      get: function() {
        return ownName;
      },
      set: function(name) {
        ownName = name;

        // The inner component shouldn't inherit this display name in most cases,
        // because the component may be used elsewhere.
        // But it's nice for anonymous functions to inherit the name,
        // so that our component-stack generation logic will display their frames.
        // An anonymous function generally suggests a pattern like:
        //   React.forwardRef((props, ref) => {...});
        // This kind of inner function is not used elsewhere so the side effect is okay.
        if (!render.name && !render.displayName) {
          render.displayName = name;
        }
      },
    });
  }
  return elementType;
}
```
### 分析
關於 `__DEV__` 的代碼暫時先跳過。我們看到，`forwardRef` 接受一個 `render` 函數作為參數，而這個 `render` 函數有兩個參數，`props` 和 `ref`，並且返回類型要求要是一個 `ReactNode`。

關於這部分對於源碼的理解有一個值得注意的點。一旦在父組件中用 jsx 引用了通過 `React.forwardRef` 包裝的子組件，那其實本質上相當於：

```js
React.createElement(React.forwardRef(child), ...)
```
這樣又包裹了一層，此時的 `??typeof` 是`REACT_ELEMENT_TYPE`，`type` 是 `React.forwardRef(Child)`，`type` 裏面的 `??typeof` 是`REACT_FORWARD_REF_TYPE`。
 
# 結語
這篇算是比較詳細的覆蓋了 react 中關於 ref 的各種用法，源碼中對於 `React.createRef` 和 `React.forwardRef` 的源碼分析則相對簡單一點。感覺 ref 還是挺重要的，也通過這篇全面一點的做一個了解。若有錯誤歡迎大老們指正，也希望本篇文章對你有一點點幫助～


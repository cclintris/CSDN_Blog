@[TOC](React 源碼解析 - API 系列 - React.createContext())

# 前言

這篇只有一個重點，就是 react 中的 `context`。個人覺得這個東西還是挺重要的，在實際項目中也都有很好的使用時機，所以學會 `context` 還是蠻重要的。同樣這篇也會先疏理關於 `context` 的一些概念以及用法，畢竟連用都不會用，哪來的源碼分析，然後才會對 `createContext` 這個 api 做源碼閱讀和個人理解的分享。

# 正文

關於 `context` 的使用和基本概念會在這邊做整理，熟悉的當然可以先跳過啦～

## Context 概念

Context 意思就是上下文，等到理解後其實就會發現這個名字取的真好。

Context 設計的目的其實就是為了在多個組件之間共享數據，例如當前登入的用戶信息、應用主題、應用語言等等。來看看下面代碼：

```js
class App extends React.Component {
  render() {
    return <Toolbar theme="dark" />;
  }
}

function Toolbar(props) {
  return <ThemedButton theme={props.theme} />;
}

class ThemedButton extends React.Component {
  render() {
    return <Button theme={this.props.theme} />;
  }
}
```

可以看到，我們從最上層的組件 `App` 一路將 `theme` 這個參數往下傳到最終渲染的下層組件 `ThemedButton`，中間還經過一個 `Toolbar`。但這樣其實超醜的，當項目越來越大這種只通過 props 的單向數據流將會變得太難維護，而且從邏輯上來說，`theme` 就是這三個組件都要共享的數據。

這時候就可以使用上 context，來看看 context 怎麼幫我們簡化問題。

```js
const ThemeContext = React.createContext("light");

class App extends React.Component {
  render() {
    <ThemeContext.Provider value="dark">
      <Toolbar />
    </ThemeContext.Provider>;
  }
}

function Toolbar() {
  return <ThemedButton />;
}

class ThemedButton extends React.Component {
  static contextType = ThemeContext;
  render() {
    return <Button theme={this.context} />;
  }
}
```

我們首先通過 `createContext` 創建了一個上下文，默認值是 light，並且用 `<ThemeContext.Provider>` 的標籤將 `Toolbar` 包住，其中還有一個 value 值為 dark，這樣一來，`Toolbar` 以下的所有組件就都可以獲取到該上下文的數據，在此例中是 dark。可以看到 `Toolbar` 中我們也無需聲明任何傳遞 props 之類的東西。對於這段代碼可能會有很多還看不懂的，下面都會一一做講解別急，總之 context 就是這樣使用，目的就是要避免在組件中各種傳遞 props。

## Context API

### React.createContext

```js
const MyContext = React.createContext(defaultValue);
```

這個 api 也就是本篇等等要看得源碼解析，就是用於創建一個 context 對象。當 react 渲染一個訂閱了這個 context 對象的組件，這個組件會從組件樹中離自身最近的那個匹配的 `Provider` 讀取到當前的 context 值。

只有當組件樹中找不到匹配的 `Provider` 時 defaultValue 才會生效，不過通常也不會發生這種情況。

### Context.Provider

```js
<MyContext.Provider value={/* 某个值 */}>
```

每個通過 `React.createContext` 創建出來的 context 對象都會返回一個 `Provider React` 組件，他的作用就是允許消費者組件訂閱 context 的變化。

`Provider` 接受一個 value 屬性，傳遞給消費者組件。一個 `Provider` 可以和多個消費組件有對應關系。多個 `Provider` 也可以嵌套使用，裏層的會覆蓋外層的數據。

當 `Provider` 的 value 值發生變化時，它內部的所有消費組件都會重新渲染。`Provider` 及其內部 `consumer` 組件都不受制於 `shouldComponentUpdate` 函數，因此當 `consumer` 組件在其祖先組件退出更新的情況下也能更新。

## Class.contextType

```js
class MyClass extends React.Component {
  componentDidMount() {
    let value = this.context;
    /* 在组件挂载完成后，使用 MyContext 组件的值来执行一些有副作用的操作 */
  }
  componentDidUpdate() {
    let value = this.context;
    /* ... */
  }
  componentWillUnmount() {
    let value = this.context;
    /* ... */
  }
  render() {
    let value = this.context;
    /* 基于 MyContext 组件的值进行渲染 */
  }
}
MyClass.contextType = MyContext;
```

掛載在 class 上的 `contextType` 屬性會被重賦值為一個由 `React.createContext()` 創建的 Context 對象。此屬性能讓你使用 this.context 來消費最近 Context 上的那個值。你可以在任何生命周期中訪問到它，包括 render 函數中。

## Context.consumer

```js
<MyContext.Consumer>
  {value => /* 基于 context 值进行渲染*/}
</MyContext.Consumer>
```

一個 React 組件可以訂閱 context 的變更，此組件可以讓你在函數式組件中可以訂閱 context。

這種方法需要一個函數作為子元素，這個函數接收當前的 context 值，並返回一個 `ReactNode`。傳遞給函數的 value 值就相當於組件樹上方離這個 context 最近的 `Provider` 提供的 value 值。如果沒有對應的 `Provider`，value 參數等同於傳遞給 `createContext()` 的 defaultValue，就跟上面講得一樣。

## React.createContext 源碼分析

大概了幾了 context 幹嘛後，終於可以進入本文的正題了，來看看 `React.createContext` 這個 api 的源碼。

### 源碼

- 源碼地址：https://github.com/facebook/react/blob/master/packages/react/src/ReactContext.js

```js
/**
 * Copyright (c) Facebook, Inc. and its affiliates.
 *
 * This source code is licensed under the MIT license found in the
 * LICENSE file in the root directory of this source tree.
 *
 * @flow
 */

import { REACT_PROVIDER_TYPE, REACT_CONTEXT_TYPE } from "shared/ReactSymbols";

import type { ReactContext } from "shared/ReactTypes";

export function createContext<T>(defaultValue: T): ReactContext<T> {
  // TODO: Second argument used to be an optional `calculateChangedBits`
  // function. Warn to reserve for future use?

  const context: ReactContext<T> = {
    $$typeof: REACT_CONTEXT_TYPE,
    // As a workaround to support multiple concurrent renderers, we categorize
    // some renderers as primary and others as secondary. We only expect
    // there to be two concurrent renderers at most: React Native (primary) and
    // Fabric (secondary); React DOM (primary) and React ART (secondary).
    // Secondary renderers store their context values on separate fields.
    _currentValue: defaultValue,
    _currentValue2: defaultValue,
    // Used to track how many concurrent renderers this context currently
    // supports within in a single renderer. Such as parallel server rendering.
    _threadCount: 0,
    // These are circular
    Provider: (null: any),
    Consumer: (null: any),
  };

  context.Provider = {
    $$typeof: REACT_PROVIDER_TYPE,
    _context: context,
  };

  let hasWarnedAboutUsingNestedContextConsumers = false;
  let hasWarnedAboutUsingConsumerProvider = false;
  let hasWarnedAboutDisplayNameOnConsumer = false;

  if (__DEV__) {
    // A separate object, but proxies back to the original context object for
    // backwards compatibility. It has a different $$typeof, so we can properly
    // warn for the incorrect usage of Context as a Consumer.
    const Consumer = {
      $$typeof: REACT_CONTEXT_TYPE,
      _context: context,
    };
    // $FlowFixMe: Flow complains about not setting a value, which is intentional here
    Object.defineProperties(Consumer, {
      Provider: {
        get() {
          if (!hasWarnedAboutUsingConsumerProvider) {
            hasWarnedAboutUsingConsumerProvider = true;
            console.error(
              "Rendering <Context.Consumer.Provider> is not supported and will be removed in " +
                "a future major release. Did you mean to render <Context.Provider> instead?"
            );
          }
          return context.Provider;
        },
        set(_Provider) {
          context.Provider = _Provider;
        },
      },
      _currentValue: {
        get() {
          return context._currentValue;
        },
        set(_currentValue) {
          context._currentValue = _currentValue;
        },
      },
      _currentValue2: {
        get() {
          return context._currentValue2;
        },
        set(_currentValue2) {
          context._currentValue2 = _currentValue2;
        },
      },
      _threadCount: {
        get() {
          return context._threadCount;
        },
        set(_threadCount) {
          context._threadCount = _threadCount;
        },
      },
      Consumer: {
        get() {
          if (!hasWarnedAboutUsingNestedContextConsumers) {
            hasWarnedAboutUsingNestedContextConsumers = true;
            console.error(
              "Rendering <Context.Consumer.Consumer> is not supported and will be removed in " +
                "a future major release. Did you mean to render <Context.Consumer> instead?"
            );
          }
          return context.Consumer;
        },
      },
      displayName: {
        get() {
          return context.displayName;
        },
        set(displayName) {
          if (!hasWarnedAboutDisplayNameOnConsumer) {
            console.warn(
              "Setting `displayName` on Context.Consumer has no effect. " +
                "You should set it directly on the context with Context.displayName = '%s'.",
              displayName
            );
            hasWarnedAboutDisplayNameOnConsumer = true;
          }
        },
      },
    });
    // $FlowFixMe: Flow complains about missing properties because it doesn't understand defineProperty
    context.Consumer = Consumer;
  } else {
    context.Consumer = context;
  }

  if (__DEV__) {
    context._currentRenderer = null;
    context._currentRenderer2 = null;
  }

  return context;
}
```

### 分析

接下來我們來一一分析每一個點(看源碼的過程還是好辛苦的害～太菜了)

#### context

- 源碼

```js
const context: ReactContext<T> = {
  $$typeof: REACT_CONTEXT_TYPE,
  // As a workaround to support multiple concurrent renderers, we categorize
  // some renderers as primary and others as secondary. We only expect
  // there to be two concurrent renderers at most: React Native (primary) and
  // Fabric (secondary); React DOM (primary) and React ART (secondary).
  // Secondary renderers store their context values on separate fields.
  _currentValue: defaultValue,
  _currentValue2: defaultValue,
  // Used to track how many concurrent renderers this context currently
  // supports within in a single renderer. Such as parallel server rendering.
  _threadCount: 0,
  // These are circular
  Provider: (null: any),
  Consumer: (null: any),
};
```

首先看到 context 是一個 `ReactContext` 類型。來看看源碼，定義了 `ReactContext` 類型需要什麼東西以及各自是什麼類型：

```js
export type ReactContext<T> = {
  $$typeof: Symbol | number,
  Consumer: ReactContext<T>,
  Provider: ReactProviderType<T>,
  _currentValue: T,
  _currentValue2: T,
  _threadCount: number,
  // DEV only
  _currentRenderer?: Object | null,
  _currentRenderer2?: Object | null,
  // This value may be added by application code
  // to improve DEV tooling display names
  displayName?: string,
  ...
};
```

##### $$typeof

`$$typeof` 其實就是以前提到的 `React.createElement` 這個方法中，源碼中的第一個參數就是 type，也就是該 `$$typeof`，具體通過 Symbol.for() 註冊了 `react.context` 值到全局環境中，具體想深入的可以去看 https://github.com/facebook/react/blob/master/packages/shared/ReactSymbols.js 源碼文件。

##### _currentValue

其實注釋也寫得挺明白的，就是說最多只會有兩個渲染器同時併發執行，具體的兩個是 React Native 和 React Fabric，關於 react 渲染器這部分個人不是很清楚，之後學習到也會再更新。

不過暫且先接受這個概念，`_currentValue` 和 `_currentValue2` 就是我們使用 `<Provider value={/* defaultValue */}>` 時，就是把 value 賦值給 `_currentValue`。其實這兩個的作用是一模一樣的，只是分別給兩個渲染器的 context 用的。

##### _threadCount

`_threadCount` 是用來紀錄對於單個 context 來說同時有多少個渲染器在併發使用，初值就設為 0。

##### Provider

然後看到 `Provider` 是一個 `ReactProviderType`，因此來看看源碼中這是個什麼類型：

```js
export type ReactProviderType<T> = {
  $$typeof: Symbol | number,
  _context: ReactContext<T>,
  ...
};
```

然後接著我們就看到：

```js
context.Provider = {
  $$typeof: REACT_PROVIDER_TYPE,
  _context: context,
};
```

看到這邊我們也就知道為什麼我們使用 `<MyContext.Provider value={ /* V */ }/>` 時，消費者組件可以訂閱到 context 的變化。因為 `Provider` 的 _context 被設為一開始 `React.createContext` 中的 context 對象，看到最後也就知道返回了該 context 給我們。而 `V` 則會被覆蓋給 context 中的 `_currentValue`。

##### Consumer

看到 `Consumer` 是 也是一個 `React.Context` 類型。並且注意到最後的 else 做了一件事：
```js
context.Consumer = context;
```
就是因為這一句，所以我們在使用 `<MyContext.Consumer>{ value => /* ... */}<MyContext.Consumer/>` 時，才能夠獲得 `Provider` 提供的最新的值。

#### 總結

總結一下，所以 `React.createContext` 這個函數就是單純的返回一個 context 對象，而其中就包含了 `Provider`, `Consumer` 兩個最重要的組件。

看完上面以了更深刻的理解後，最後通過一個例子來過一遍源碼的整個流程：

```js
const { Provider, Consumer } = React.createContext({ theme: "green" })
class MessageList extends React.Component {
  render() {
    //Provider 组件遍历子组件，并且有一个属性 value 用来提供数据
    return (
      <Provider value={{ theme: "pink" }}>
        <Content />
      </Provider>
    )
  }
}
//中间组件
function Content() {
  return (
    <div>
      <Button />
    </div>
  )
}
//接收组件,如果子组件是Consumer的话,将value作为参数值,传递给新创建的Consumer渲染一个函数组件
function Button() {
  return (
    <Consumer>
      {({ theme }) => (
        <button
          style={{ backgroundColor: theme }}>
          Toggle Theme
        </button>
      )}
    </Consumer>
  )
}
```
![](https://seikim.com/i/2021/06/10/tvvgh6.png)

所以其實 `Provider` 跟 `Consumer` 兩個都不是真正的組件，實際上在使用 `<Provider>` 跟 `<Consumer>` 時一樣會通過 `React.createElement` 這個方法去實際創建並返回 `ReactElement` 給我們。

`React.createContext` 所做的事情其實就是幫我們封裝好 `Provider` 跟 `Consumer` 封裝好以便於給外部使用時 `React.createElement` 創建 `<Provider>` 跟 `<Consumer>` 有對應的參數。

# 結語
這篇介紹了 react 的 context 的一些基本用法，然後又對 `React.createContext` 的源碼部分進行了自己的分析理解，是有對 context 理解了一點，不過具體還是要自己使用看看才好，感受更深。


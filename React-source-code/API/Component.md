@[TOC](React 源碼解析 - API 系列 - React.Component/PureComponent)

# 前言
這篇繼續解析 react 源碼，接續上一篇延續 API 系列，這篇要了解的是 react 源碼的 React.Component 和 PureComponent 兩個同樣很重要的 API。

# 正文

## React.Component
我們都知道在寫 react 組件時，都會這樣寫：
```js
class App extends React.Component {
    constructor(props) {
        super(props)
        this.state = {}
    }

    componentDidMount() {
        // ...
    }

    render() {
        return (
            <div>App</div>
        )
    }
}
```
那當然重點就是分析 `React.Component` 這個東西到底是什麼啦～

### 源碼

- 源碼地址：https://github.com/facebook/react/blob/master/packages/react/src/ReactBaseClasses.js

```js
/**
 * Copyright (c) Facebook, Inc. and its affiliates.
 *
 * This source code is licensed under the MIT license found in the
 * LICENSE file in the root directory of this source tree.
 */

import invariant from 'shared/invariant';

import ReactNoopUpdateQueue from './ReactNoopUpdateQueue';

const emptyObject = {};
if (__DEV__) {
  Object.freeze(emptyObject);
}

/**
 * Base class helpers for the updating state of a component.
 */
//幫助更新組件狀態的基類
function Component(props, context, updater) {
  this.props = props;
  this.context = context;
  // If a component has string refs, we will assign a different object later.
  this.refs = emptyObject;
  // We initialize the default updater but the real one gets injected by the
  // renderer.
  //雖然給updater賦了默認值，但真正的updater是在renderer中註冊的
  this.updater = updater || ReactNoopUpdateQueue;
}

//原型上赋了一个flag
Component.prototype.isReactComponent = {};

/**
 * Sets a subset of the state. Always use this to mutate
 * state. You should treat `this.state` as immutable.
 *
 * There is no guarantee that `this.state` will be immediately updated, so
 * accessing `this.state` after calling this method may return the old value.
 *
 * this.state並不是立即更新的，所以在調用this.setState後可能不能拿到新值
 *
 * There is no guarantee that calls to `setState` will run synchronously,
 * as they may eventually be batched together.  You can provide an optional
 * callback that will be executed when the call to setState is actually
 * completed.
 *
 * 不能保證 this.state 是同步的（它也不是異步的，這很特別下面會詳細說明)，使用回調 updater 獲取最新值
 *
 * When a function is provided to setState, it will be called at some point in
 * the future (not synchronously). It will be called with the up to date
 * component arguments (state, props, context). These values can be different
 * from this.* because your function may be called after receiveProps but before
 * shouldComponentUpdate, and this new state, props, and context will not yet be
 * assigned to this.
 *
 * @param {object|function} partialState Next partial state or function to
 *        produce next partial state to be merged with current state.
 * @param {?function} callback Called after state is updated.
 * @final
 * @protected
 */


// partialState：要更新的state，可以是Object/Function
// callback： setState({xxx},callback)
Component.prototype.setState = function(partialState, callback) {
  // 判断setState中的partialState是否符合条件，
  // 如果不符合则抛出Error
  invariant(
    typeof partialState === 'object' ||
      typeof partialState === 'function' ||
      partialState == null,
    'setState(...): takes an object of state variables to update or a ' +
      'function which returns an object of state variables.',
  );
  this.updater.enqueueSetState(this, partialState, callback, 'setState');
};

/**
 * Forces an update. This should only be invoked when it is known with
 * certainty that we are **not** in a DOM transaction.
 *
 * You may want to call this when you know that some deeper aspect of the
 * component's state has changed but `setState` was not called.
 *
 * This will not invoke `shouldComponentUpdate`, but it will invoke
 * `componentWillUpdate` and `componentDidUpdate`.
 *
 * @param {?function} callback Called after update is complete.
 * @final
 * @protected
 */
Component.prototype.forceUpdate = function(callback) {
  this.updater.enqueueForceUpdate(this, callback, 'forceUpdate');
};
```
### 分析
這邊開始對上面的源代碼做分析，包括 import 進來的一些其他代碼。

#### invariant

- 源碼
```js
/**
 * Use invariant() to assert state which your program assumes to be true.
 *
 * Provide sprintf-style format (only %s is supported) and arguments
 * to provide information about what broke and what you were
 * expecting.
 *
 * The invariant message will be stripped in production, but the invariant
 * will remain to ensure logic does not differ in production.
 */

/**
 * 根据条件抛出固定格式的错误，允许使用 %s 作为变量的占位符
 * 
 * @param condition
 * @param format
 * @param a
 * @param b
 * @param c
 * @param d
 * @param e
 * @param f
 */
export default function invariant(condition, format, a, b, c, d, e, f) {
  validateFormat(format);
 
  if (!condition) {
    let error;
    if (format === undefined) {
      error = new Error(
        'Minified exception occurred; use the non-minified dev environment ' +
          'for the full error message and additional helpful warnings.',
      );
    } else {
      const args = [a, b, c, d, e, f];
      let argIndex = 0;
      error = new Error(
        format.replace(/%s/g, function() {
          return args[argIndex++];
        }),
      );
      error.name = 'Invariant Violation';
    }
 
    error.framesToPop = 1; // we don't care about invariant's own frame
    throw error;
  }
}
```
`invariant()` 這個函數其實也不算是源碼重點所以先不過多深究。總之這個函數就是做到一個斷言的效果，用於斷言程序中應該為 true 的狀態條件。

`@condition` 是要斷言的條件條件表達式，`@format` 是斷言條件不為真時拋出的錯誤的消息的模版。剩下的 `a b c d e f` 其實就只是允許六個用於組織錯誤信息的參數。

總而言之，`invariant()` 做的就是斷言一個應該為真的條件判斷式，如果不為真就會拋出拋出一個可以自定義格式的 error。

#### ReactNoopUpdateQueue
在 `Component` 這個類中(es5 還沒有 class，用 function 實現類)，初始化了一些東西，包括 `this.updater = updater || ReactNoopUpdateQueue;`，在下面的 `setState` 會用到，所以當然就要去看看 `ReactNoopUpdateQueue` 到底是什麼～

- 源碼
```js
/**
 * This is the abstract API for an update queue.
 */
const ReactNoopUpdateQueue = {
  /**
   * Checks whether or not this composite component is mounted.
   * @param {ReactClass} publicInstance The instance we want to test.
   * @return {boolean} True if mounted, false otherwise.
   * @protected
   * @final
   */
  isMounted: function(publicInstance) {
    return false;
  },

  /**
   * Forces an update. This should only be invoked when it is known with
   * certainty that we are **not** in a DOM transaction.
   *
   * You may want to call this when you know that some deeper aspect of the
   * component's state has changed but `setState` was not called.
   *
   * This will not invoke `shouldComponentUpdate`, but it will invoke
   * `componentWillUpdate` and `componentDidUpdate`.
   *
   * @param {ReactClass} publicInstance The instance that should rerender.
   * @param {?function} callback Called after component is updated.
   * @param {?string} callerName name of the calling function in the public API.
   * @internal
   */
  enqueueForceUpdate: function(publicInstance, callback, callerName) {
    warnNoop(publicInstance, 'forceUpdate');
  },

  /**
   * Replaces all of the state. Always use this or `setState` to mutate state.
   * You should treat `this.state` as immutable.
   *
   * There is no guarantee that `this.state` will be immediately updated, so
   * accessing `this.state` after calling this method may return the old value.
   *
   * @param {ReactClass} publicInstance The instance that should rerender.
   * @param {object} completeState Next state.
   * @param {?function} callback Called after component is updated.
   * @param {?string} callerName name of the calling function in the public API.
   * @internal
   */
  enqueueReplaceState: function(
    publicInstance,
    completeState,
    callback,
    callerName,
  ) {
    warnNoop(publicInstance, 'replaceState');
  },

  /**
   * Sets a subset of the state. This only exists because _pendingState is
   * internal. This provides a merging strategy that is not available to deep
   * properties which is confusing. TODO: Expose pendingState or don't use it
   * during the merge.
   *
   * @param {ReactClass} publicInstance The instance that should rerender.
   * @param {object} partialState Next partial state to be merged with state.
   * @param {?function} callback Called after component is updated.
   * @param {?string} Name of the calling function in the public API.
   * @internal
   */
  enqueueSetState: function(
    publicInstance,
    partialState,
    callback,
    callerName,
  ) {
    warnNoop(publicInstance, 'setState');
  },
};

export default ReactNoopUpdateQueue;
```
可以看到其實 `ReactNoopUpdateQueue` 就是一個 js 對象，定義了一些屬性方法。下面繼續看 `setState` 怎麼使用 `ReactNoopUpdateQueue`。

#### setState
我們看到，在 Component 的原型上定義了一個 `setState`，這樣使得所有 Component 的實例都可以使用這個方法。

```js
// partialState：要更新的state，可以是Object/Function
// callback： setState({xxx},callback)
Component.prototype.setState = function(partialState, callback) {
  // 判断setState中的partialState是否符合条件，
  // 如果不符合则抛出Error
  invariant(
    typeof partialState === 'object' ||
      typeof partialState === 'function' ||
      partialState == null,
    'setState(...): takes an object of state variables to update or a ' +
      'function which returns an object of state variables.',
  );
  this.updater.enqueueSetState(this, partialState, callback, 'setState');
};
```
首先 `setState` 調用了 `invariant()` 判斷 partialState 的類型是否符合要求，不符合就會拋出 error。對於 `setState` 有兩種用法，一種就是參數是 `object`，另一種則是用 updater 函數，參數就是一個 `function`，所以源碼中也就是檢查這兩種類型。如果不熟悉 `setState` 的人可以先去看看 [深入理解 React 的 setState](https://blog.csdn.net/weixin_46803507/article/details/116460027)。

接下來，重點就是調用 `ReactNoopUpdateQueue` 的 `enqueueSetState` 這個方法。

#### forceUpdate
`forceUpdate` 則相對單純一點，源碼如下：
```js
Component.prototype.forceUpdate = function(callback) {
  this.updater.enqueueForceUpdate(this, callback, 'forceUpdate');
};
```
`forceUpdate` 其實是 react 有狀態組件中的其中一個鉤子函數，作用就是強制進行重新渲染。而且使用 `forceUpdate` 時雖然會觸發 `componentWillUpdate()` 和 `componentShouldUpdate()`，但是是不會觸發 `componentShouldUpdate()`，因為強制渲染其實就相當於強制更新，所以忽略 `componentShouldUpdate()` 是很合理的。不清楚的歡迎先去看看 [關於 React 組件生命週期](https://blog.csdn.net/weixin_46803507/article/details/116433318#forceUpdate_213)。

重點就是調用 `ReactNoopUpdateQueue` 的 `enqueueForceUpdate` 這個方法。

#### 小結
這部分介紹了 `React.Component` 這個超重要的 api，可以說是寫 react 組件必須使用的一個函數。這邊小結一下關於 `React.Component` 目前的理解：

- `Component` 本質上是一個類，使用 es5 的實現方式

```js
class Component {
  constructor(props, context, updater) {
    this.props = props;
    this.context = context;
    this.refs = emptyObject;
    this.updater = updater || ReactNoopUpdateQueue;
  }
}
```
- `setState()` 是 Component 原型上的方法，其本質是調用ReactNoopUpdateQueue 中的 `enqueueSetState()` 方法，之後的文章會專門分析 `enqueueSetState()`，更好的理解 `setState`。

- `forceUpdate()` 同上。

其實會發現，`React.Component()` 只涉及了 `props`/`context`/`refs`/`updater`/`isReactComponent`/`setState`/`forceUpdate`，其他均沒有自己實現，包括一些生命週期鉤子函數，原本以為都會在這個類中實現，所以可能還要再找找具體源碼實現在哪裡了。

## PureComponent
上面的 `React.Component` 大家肯定都非常熟悉，但是可能對於 `PureComponent` 不是那麼熟悉，畢竟比較少用，所以看源碼前先來了解下什麼是 `PureComponent`。

### 什麼是 PureComponent
復用性強的組件：如果一個組件的渲染只依賴於外界傳進去的 props 和自己的 state，而並不依賴於其他的外界的任何數據(例如 redux 什麼的)，也就是說像純函數一樣，給它什麽，它就吐出（渲染）什麽。這種組件的復用性是最強的，也就是 Pure Component 或稱 Dumb Component。

用法跟 Component 也一模一樣：
```js
class A extends React.PureComponent { 
  // ...
}
```
### 源碼

- 源碼地址：https://github.com/facebook/react/blob/master/packages/react/src/ReactBaseClasses.js

```js
function ComponentDummy() {}
ComponentDummy.prototype = Component.prototype;

/**
 * Convenience component with default shallow equality check for sCU.
 */
function PureComponent(props, context, updater) {
  this.props = props;
  this.context = context;
  // If a component has string refs, we will assign a different object later.
  this.refs = emptyObject;
  this.updater = updater || ReactNoopUpdateQueue;
}

const pureComponentPrototype = (PureComponent.prototype = new ComponentDummy());
pureComponentPrototype.constructor = PureComponent;
// Avoid an extra prototype jump for these methods.
Object.assign(pureComponentPrototype, Component.prototype);
pureComponentPrototype.isPureReactComponent = true;

export {Component, PureComponent};
```
### 分析

#### 創建 PureComponent
重點看最後三行做了什麽，我們來一一解析下為什麼要這麼寫。

看到 `const pureComponentPrototype = (PureComponent.prototype = new ComponentDummy());`。上面新建了一個 `ComponentDummy`，並繼承自 `Component` 原型，而這邊 `PureComponent` 原型其實就是一個 `ComponentDummy` 實例。

這麼做的目的其實是為了減少內存消耗。如果讓 `PureComponent.prototype` 直接等於 `Component` 的實例對象的話（繼承原型），會多繼承 `Component` 的 constructor，但是`PureComponent` 已經有自己的 constructor 了，這樣就會多消耗一些內存。

所以先新建 `ComponentDummy`，只繼承 `Component` 的原型，不包括 constructor，以此來節省內存。

接著，`pureComponentPrototype.constructor = PureComponent`。原型的 constructor 等於自身，覆蓋掉`Component.prototype` 的 constructor（Component）
。

上面的所有動作就是讓 `PureComponent` 繼承 `Component`，那麽為什麽還要多寫一句 `Object.assign(...)`呢？繼續看下去分析。

#### Object.assign()
上面的做的是就是讓 `PureComponent` 繼承 `Component` 所有屬性，不過其實這麼做只是將 `PureComponent` 的 prototype 淺拷貝 `Component` 的 prototype 的所有屬性。而且這樣會有一個問題：

```js
PureComponent.prototype.__proto__ === ComponentDummy.prototype === Component.prototype //true
```
這樣的問題在於 `PureComponent` 只是繼承了 `Component` 的所有屬性，但是實際上 `PureComponent` 跟 `Component` 應該跟 `Component` 是相當級別的，所以這樣還要通過 `__proto__` 才能訪問到實際的 `Component` 屬性就需要做改進。

所以才加了 `Object.assign(pureComponentPrototype, Component.prototype)`，這樣所有 `Component.prototype` 的屬性都會在 `PureComponent.prototype` 上，無需再通過 `__proto__` 上查找了。
```js
PureComponent.prototype === Component.prototype
// true
```

# 結語 
這篇分享了 `React.Component` 以及 `PureComponent` 源碼中的一些理解和思想，感覺還是有一些點沒有理解的很透徹，然後還有 `React.Component` 中的 `setState` 和 `forceUpdate` 具體的源碼分析沒有在這篇講到，之後的源碼分析系列也會提到。若有錯誤歡迎指正，先撤了～


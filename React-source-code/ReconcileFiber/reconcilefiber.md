@[TOC](React 深入理解：Fiber 與 Reconciliation)

# 前言
React Fiber 這個東西一直都聽著，感覺好牛的一個東西，但是感覺好難入門，感到非常難受。先前情提要一下，React Fiber 是 React v16.x 推出的新的架構，而本文的另一個主角 Reconcilation 是 React 的 diff 算法，這兩個東西都是 react 中超級重要的東西！

本文就要來研究下 React Fiber 與 Reconcilation，來看看 fiber 到底是什麼東西？並且 reconcilation 跟 fiber 又是什麼關係，怎麼工作的？而由於是 react 非常核心的機制，所以多會涉及到很多的概念。這篇估計會蠻長的，希望自己可以堅持寫完學習完，想看的也斟酌下。

# 正文
廢話不多說，首先就先來了解 fiber 到底是什麼吧～

## 從 DOM 到 Fiber 對象
畢竟 fiber 不是一個那麼簡單的東西，所以我們先從一些基本概念看起，來看看 dom, vdom, react 元素, fiber 對象之間的關係吧～

### DOM

文檔對象模型，實際上就是一個樹，樹中的每一個節點就是 HTML 元素（Element），每個節點實際上是一個 JS 對象，這個對象除了包含了該元素的一些屬性，還提供了一些接口（方法），以便編程語言可以插入和操作 DOM。但是 DOM 本身並沒有針對動態 UI 的 Web 應用程序進行太多的支持和優化。因此，當一個頁面有很多元素需要更新時，直接通過 `document` 更新相對應的 DOM 會使得程序變得很慢，因為瀏覽器需要重新渲染所有的樣式和 HTML。

### VDOM

為了優化 “真實” DOM 的更新，出現了 「Virtual DOM」的概念。本質來說，Virtual DOM 是真實 DOM 的模擬，它實際上也是一棵樹。真實的 DOM 樹由真實的 DOM 元素組成，而 Virtual DOM 樹是由 Virtual DOM 元素組成。

當 React 組件的狀態發生變化時，會產生一棵新的 Virtual DOM 樹，然後 React 會使用 diff 算法去比較新、舊兩棵 Virtual DOM 樹，得到其中的差異，然後將「差異」更新到真實的 DOM 樹中，從而完成 UI 的更新。

要說明一點是：這裏並不是說使用了 Virtual DOM 就可以加快 DOM  的操作速度，而是說 React 讓頁面在不同狀態之間變化時，使用了次數盡量少的 DOM 操作來完成。

### React 元素(React Element)
在 react 中，react 給 vdom 元素取名叫做 react element，也就是說 react 中的 vdom 是由一堆 react element 構成的，每個節點都是 react 元素。

其實之前就寫過一篇關於 react element 的源碼分析，不過在這邊再複習一遍：

我們來看看源碼中關於 react element 的定義：

`/package/shared/ReactElementType.js`

```js
export type ReactElement = {|
  $$typeof: any,
  type: any,
  key: any,
  ref: any,
  props: any,
  // ReactFiber
  _owner: any,
  ...
|};
```
- `$$typeof`：React 元素的標誌，是一個 Symbol 類型

- `type`：React 元素的類型。如果是自定義組件（composite component），那麽 type 字段的值就是一個 class 或 function component；如果是原生 HTML，如 div、span 等，那麽 type 的值就是一個字符串（'div'、'span'）

- `key`：是 react 元素的 key，在執行 diff 算法的時候會用到

- `ref`：React 元素的 ref 屬性，當 React 元素變成真實的 DOM 之後，會返回真實的 DOM 節點引用

- `props`：React 元素的屬性，是一個 js 對象

- `_owner`： 負責創建這個 React 元素的組件，另外從代碼中的註釋 “// ReactFiber” 可以知道，它裏面包含了 React 元素關聯的 fiber 對象實例，不懂沒關係，下面還會繼續講到 fiber

當我們寫 react 組件時，不論是 class 組件還是 function 組件，這個我們寫的 jsx 都會經過一個 jsx compiler 進行編譯，而其中就會調用 `createElement` 這個方法。

關於 `React.createElement` 這個方法，我寫過一篇比較詳細的，有興趣的歡迎去看看 [React 源碼解析 - API 系列 - React.createElement/ReactElement](https://blog.csdn.net/weixin_46803507/article/details/117261068)，這邊我就簡單說一下。

(下面的一些圖示感謝百度上前端大老們的分享)。

調用 `React.createElement` 時，實際上就是會為我們創建出來一個 react 元素。

![](https://seikim.com/i/2021/06/23/m9ib76.jpg)

在上面 ClickCounter 組件這個例子中， `<button>` 和 `<span>` 是 `<ClickCounter>` 的子組件。而 `<ClickCounter>` 組件其實它本身其實也是一個組件。它是 `<App>` 組件的子組件：

```js
class App extends React.Component {
    ...
    render() {
        return [
            <ClickCounter />
        ]
    }
}
```
所以當調用了 `App` 組件的 `render` 時，也會創造出相應的 react element：

![](https://seikim.com/i/2021/06/23/m9ibhh.jpg)

當執行最終的 `ReactDOM.render` 時，大致的 vdom tree 就像下面這樣：

![](https://seikim.com/i/2021/06/23/maqycb.jpg)

### Fiber 對象
當我們創建一個 react element 時，還會創建一個與該 react element 相關聯的 fiber node，而這個 fiber node 就是一個 Fiber 對象的實例。

Fiber 對象是一個用於保存 `組件狀態`、`組件對應的 DOM 的信息`、以及 `工作任務(work)` 的數據結構。Fiber 對象負責管理組件實例的更新，渲染任務以及與其他 fiber node 的關係。每個組件(react element)都會有一個與之對應的 Fiber 對象實例(fiber node)，而和 react element 不同的是，fiber node 不需要在每一次 組件/界面 更新時都重新創建一遍。

在執行 reconcilation 這個算法的期間，組件 render 方法所返回的 react element 信息(js 對象屬性)都會被合併到各自對應的 fiber node 中。這些 fiber node 也因此組成了一顆與 vdom tree(由 react element 構成)相對應的 fiber node tree。`再次強調`，`每個 react element 都會有一個與之對應的 fiber node`。

講了那麼多，不如還是還看看源碼中 Fiber 對象的定義：

`/package/react-reconciler/src/ReactInternalTypes.js`

```js
// A Fiber is work on a Component that needs to be done or was done. There can
// be more than one per component.
export type Fiber = {|
  // These first fields are conceptually members of an Instance. This used to
  // be split into a separate type and intersected with the other Fiber fields,
  // but until Flow fixes its intersection bugs, we've merged them into a
  // single type.

  // An Instance is shared between all versions of a component. We can easily
  // break this out into a separate object to avoid copying so much to the
  // alternate versions of the tree. We put this on a single object for now to
  // minimize the number of objects created during the initial render.

  // Tag identifying the type of fiber.
  tag: WorkTag,

  // Unique identifier of this child.
  key: null | string,

  // The value of element.type which is used to preserve the identity during
  // reconciliation of this child.
  elementType: any,

  // The resolved function/class/ associated with this fiber.
  type: any,

  // The local state associated with this fiber.
  stateNode: any,

  // Conceptual aliases
  // parent : Instance -> return The parent happens to be the same as the
  // return fiber since we've merged the fiber and instance.

  // Remaining fields belong to Fiber

  // The Fiber to return to after finishing processing this one.
  // This is effectively the parent, but there can be multiple parents (two)
  // so this is only the parent of the thing we're currently processing.
  // It is conceptually the same as the return address of a stack frame.
  return: Fiber | null,

  // Singly Linked List Tree Structure.
  child: Fiber | null,
  sibling: Fiber | null,
  index: number,

  // The ref last used to attach this node.
  // I'll avoid adding an owner field for prod and model that as functions.
  ref:
    | null
    | (((handle: mixed) => void) & {_stringRef: ?string, ...})
    | RefObject,

  // Input is the data coming into process this fiber. Arguments. Props.
  pendingProps: any, // This type will be more specific once we overload the tag.
  memoizedProps: any, // The props used to create the output.

  // A queue of state updates and callbacks.
  updateQueue: mixed,

  // The state used to create the output
  memoizedState: any,

  // Dependencies (contexts, events) for this fiber, if it has any
  dependencies: Dependencies | null,

  // Bitfield that describes properties about the fiber and its subtree. E.g.
  // the ConcurrentMode flag indicates whether the subtree should be async-by-
  // default. When a fiber is created, it inherits the mode of its
  // parent. Additional flags can be set at creation time, but after that the
  // value should remain unchanged throughout the fiber's lifetime, particularly
  // before its child fibers are created.
  mode: TypeOfMode,

  // Effect
  flags: Flags,
  subtreeFlags: Flags,
  deletions: Array<Fiber> | null,

  // Singly linked list fast path to the next fiber with side-effects.
  nextEffect: Fiber | null,

  // The first and last fiber with side-effect within this subtree. This allows
  // us to reuse a slice of the linked list when we reuse the work done within
  // this fiber.
  firstEffect: Fiber | null,
  lastEffect: Fiber | null,

  lanes: Lanes,
  childLanes: Lanes,

  // This is a pooled version of a Fiber. Every fiber that gets updated will
  // eventually have a pair. There are cases when we can clean up pairs to save
  // memory if we need to.
  alternate: Fiber | null,

  // Time spent rendering this Fiber and its descendants for the current update.
  // This tells us how well the tree makes use of sCU for memoization.
  // It is reset to 0 each time we render and only updated when we don't bailout.
  // This field is only set when the enableProfilerTimer flag is enabled.
  actualDuration?: number,

  // If the Fiber is currently active in the "render" phase,
  // This marks the time at which the work began.
  // This field is only set when the enableProfilerTimer flag is enabled.
  actualStartTime?: number,

  // Duration of the most recent render time for this Fiber.
  // This value is not updated when we bailout for memoization purposes.
  // This field is only set when the enableProfilerTimer flag is enabled.
  selfBaseDuration?: number,

  // Sum of base times for all descendants of this Fiber.
  // This value bubbles up during the "complete" phase.
  // This field is only set when the enableProfilerTimer flag is enabled.
  treeBaseDuration?: number,

  // Conceptual aliases
  // workInProgress : Fiber ->  alternate The alternate used for reuse happens
  // to be the same as work in progress.
  // __DEV__ only

  _debugSource?: Source | null,
  _debugOwner?: Fiber | null,
  _debugIsCurrentlyTiming?: boolean,
  _debugNeedsRemount?: boolean,

  // Used to verify that the order of hooks does not change between renders.
  _debugHookTypes?: Array<HookType> | null,
|};
```
上面就是源碼中關於 Fiber 對象的定義，非常多屬性，下面就來介紹下一些比較重要的屬性。

- `tag`：這個字段定義了 fiber node 的類型。在 reconcilation 算法中，它被用於決定一個 fiber node 所需要完成的 work 是什麼。

上面這個 `tag` 屬性的類型是 `WorkTag`，那當然就再去找找 `WorkTag` 的源碼啦：

```js
/**
 * Copyright (c) Facebook, Inc. and its affiliates.
 *
 * This source code is licensed under the MIT license found in the
 * LICENSE file in the root directory of this source tree.
 *
 * @flow
 */

export type WorkTag =
  | 0
  | 1
  | 2
  | 3
  | 4
  | 5
  | 6
  | 7
  | 8
  | 9
  | 10
  | 11
  | 12
  | 13
  | 14
  | 15
  | 16
  | 17
  | 18
  | 19
  | 20
  | 21
  | 22
  | 23
  | 24;

export const FunctionComponent = 0;
export const ClassComponent = 1;
export const IndeterminateComponent = 2; // Before we know whether it is function or class
export const HostRoot = 3; // Root of a host tree. Could be nested inside another node.
export const HostPortal = 4; // A subtree. Could be an entry point to a different renderer.
export const HostComponent = 5;
export const HostText = 6;
export const Fragment = 7;
export const Mode = 8;
export const ContextConsumer = 9;
export const ContextProvider = 10;
export const ForwardRef = 11;
export const Profiler = 12;
export const SuspenseComponent = 13;
export const MemoComponent = 14;
export const SimpleMemoComponent = 15;
export const LazyComponent = 16;
export const IncompleteClassComponent = 17;
export const DehydratedFragment = 18;
export const SuspenseListComponent = 19;
export const ScopeComponent = 21;
export const OffscreenComponent = 22;
export const LegacyHiddenComponent = 23;
export const CacheComponent = 24;
```

- `key`：這個字段和 react element 的 key 含義跟內容都一樣。其實這個 key 就是當創建一個 react element 時，順帶創建一個對應的 fiber node 時直接拷貝過來的，目的就是作為 children 中每一個 item 的唯一標示。被用於幫助 react 計算出哪個 item 被修改了，哪個 item 是新增的，哪個 item 被刪除了。

- `type`：這個字段表示與這個 fiber node 相關連的 react element 的類型。所以可以想到，其實這個字段也是從 react element 的 type 那裏直接拷貝過來的。

- `updateQueue`：這個字段用來存儲組件狀態更新，回調和 DOM 更新任務隊列。fiber node 就是通過這個字段，來管理 fiber node 所對應的 react element 的渲染，更新任務。關於這個字段之後還會再寫一篇關於 react hooks，從 hooks 的角度去看 fiber node 的這個字段。

> 這邊額外補充一個小知識點，就是這個 `updateQueue` 的類型，是個 mixed。這其實是應用了一個叫 Flow.js 的靜態檢查 js 類型的工具，同樣是 facebook 開發的，可以看看這篇 [Flow.js学习笔记一](https://blog.csdn.net/weixin_33769207/article/details/93018457)。

- `memoizedState`：已經被更新到真實 DOM 後的 state(已經被渲染到真實 DOM 上的 state)

- `memoizedProps`：已經被更新到真實 DOM 後的 props(已經被渲染到真實 DOM 上的 props)

- `pendingProps`：等待被更新到真實 DOM 的 props

- `return`：這個字段相當於是一個指針，指向的是父 fiber node

- `child`：這個字段相當於是一個指針，指向的是子 fiber node

- `sibling`：這個字段相當於是一個指針，指向的是兄弟 fiber node

- `nextEffect`：指向下一個帶有 side effect 的 fiber node

- `firstEffect`：指向第一個帶有 side effect 的 fiber node

- `lastEffect`：指向最後一個帶有 side effect 的 fiber node

對於最後三個屬性我們提到了一個東西叫做 side effect，不懂也先不要慌張，馬上就會講到什麼是 side effect。在講 side effect 之前我們先借著上面那個 `<ClickCounter>` 的例子，來看看它的 fiber node 長什麼樣子～

- `<ClickCounter>`

```js
class App extends React.Component {
  constructor(props) {
    this.props = props
    this.state = { count: 0 }
    this.handleClick = this.handleClick.bind(this)
  }

  handleClick() {
    this.setState((state) => {
      return { count: state.count + 1 } 
    })
  }

  render() {
    return {
      <button key='1' onClick={ this.handleCLick }>Update counter</button>
      <span key='2'>{ this.state.count }</span>
    }
  }
}
```
上面的 `<ClickCounter>` 的 fiber node 應該長這樣(這邊一樣只包含上面幾個比較重要的屬性，而非完整的 fiber node)：

```js
{
  stateNode: new ClickCounter,
  type: ClickCounter,
  alternate: null,
  key: null,
  updateQueue: null,
  memoizedState: {count: 0},
  pendingProps: {},
  memoizedProps: {},
  tag: 1,
  effectTag: 0,
  nextEffect: null
}
```
![](https://seikim.com/i/2021/06/25/2hjdil.jpg)

`span` 的 fiber node：

```js
{
  stateNode: new HTMLSpanElement,
  type: "span",
  alternate: null,
  key: "2",
  updateQueue: null,
  memoizedState: null,
  pendingProps: {children: 0},
  memoizedProps: {children: 0},
  tag: 5,
  effectTag: 0,
  nextEffect: null
}
```
![](https://seikim.com/i/2021/06/25/2hybhg.jpg)

上面 `<ClickCounter>` 例子中，我們就可以得到一個如下的 vdom tree 跟 fiber node tree，每個節點呈現一一對應的映射關係：

![](https://seikim.com/i/2021/06/25/2hycwf.jpg)

#### Side Effects
在 Fiber 的類型中，有三個屬性：`firstEffect`, `nextEffect`, `lastEffect`，都是指向帶有 side effects 的 fiber node。那到底什麼是 side effects？

寫過 react 組件的都知道，其實 react 組件本質上就是一個函數，這個函數可以接受 props 和 state 作為參數，然後通過計算，最終返回 react element。 在這整個過程中，會進行一些操作，比如說更改 DOM 結構，調用組件生命週期函數等等，react 就把這些操作統稱為 『side effect』，簡稱 『effect』，也就是常說的副作用，其實很好理解。

大部分組件的 state 和 props 的更新都會導致 side effect 的產生。此外，我們還可以通過 `useEffect` 這個 hook 來自定義一些 effect。其實 fiber node 的 effect 會以一個循環鏈表的數據結構存儲，然後 fiber node 的 `updateQueue` 會指向這個 effect 循環鏈表。

![](https://seikim.com/i/2021/06/25/2hyagy.jpg)

#### Effects list
在一個 fiber node tree 中，有一些 fiber node 是有 effect 需要處理的，而有些也是沒有的。為了加快整棵 fiber node tree 的 effect 處理速度，react 還為那些帶有 effect 需要處理的 fiber node 構建了一個鏈表，這個鏈表就叫做 『effects list』。

這個鏈表存儲那些帶有 effect 的 fiber node。而維護這麼一個 effects list 的原因就是：因為遍歷一整棵 fiber node tree 的速度比遍歷一個 list 遠遠來得慢，對於那些沒有 effect 的 fiber node，其實我們是沒有必要花時間去迭代遍歷它的，而這個 effect list 通過 fiber node 的 `firstEffect`、`lastEffect`、`nextEffect` 三個屬性來維護。

舉個例子感受的比較清楚，下面有一個 fiber node tree，其中顏色高量的節點是帶有 effect 需要處理的 fiber node。假設我們的更新流程會導致 H 被插入到 DOM 中，而 C、D 將會改變自身的屬性，G 將會調用自身的生命週期方法。

![](https://seikim.com/i/2021/06/25/2hyfrj.jpg)

那麼，這個 fiber node tree 的 effect list 將會把這些節點存儲到 effect list 中，這樣 react 在遍歷 fiber node tree 的時候就可以跳過其他沒有任何 effect 任務需要處理的 fiber node 了。

![](https://seikim.com/i/2021/06/25/2hyfch.jpg)

## React 架構
![](https://seikim.com/i/2021/06/23/2asmuv.jpg)

上圖就是 react 整體的架構，下面就來說說上面的一些東西～

- `任務調度器(Scheduler)`：Scheduler 決定渲染任務優先級，將高優先級的任務優先交給 reconciler，當 class 組件調用 render() 時 (或者 function 組件的 return) 時，實際上並不是馬上就開始渲染這個組件，其實此時只是會返回『渲染信息』(該選染什麼東西的描述)。而該描述包含了用戶自己寫的 react 組件(例如 `<MyComponent>`)，或是原生的 html 標籤(如 `<div></div>`)。然後 react 會通過 scheduler 來決定在未來的某個時間點再來執行這個組件的渲染任務。

- `協調器(Reconciler)`：Reconciler 負責找出新舊兩棵 vdom(react element tree)的差異，並把這個差異告訴 renderer。關於 reconciler 怎麼運作的，下面再來詳細研究。 

- `渲染器(Renderer)`：Renderer 負責將 reconciler 通知的差異更新到真實的 DOM 上，從而更新 UI。不同的平台、瀏覽器會有不同的 renderer。DOM 只是 React 能夠適配的渲染標準之一。其他主要渲染平台還有 IOS，android(通過 React Native 這個 renderer 來完成)。

## 關於 Reconciler 
React 是一個用於構建用戶介面的 JavaScript 類庫。React 的核心機制就是跟蹤組件狀態變化，然後將更新的狀態即時反映到用戶界面上。

使用 React 時，組件中的 `render()` 函數就是用來創建一棵 react element tree，也就是 vdom。當我們調用 `setState`，即下一個 state 或是 props 更新時，`render()` 函數就會返回一棵不同的 react element tree。接下來，React 將會使用 diff 算法去計算兩棵樹的差異，然後高效的更新 UI。這個牛逼的 diff 算法就是 reconcilation 算法。

Reconcilation 算法主要做了兩件事情：

1. 找出兩棵 react element tree 的差異

2. 將差異更新到真實 DOM，從而完成 UI 更新

### Stack Reconciler
在 React 15.x 版本以及之前的版本，Reconciliation 算法采用了棧調和器（ Stack Reconciler ）來實現，但是這個時期的棧調和器存在一些缺陷：不能暫停渲染任務，不能切分任務，無法有效平衡組件更新渲染與動畫相關任務的執行順序，即不能劃分任務的優先級（這樣有可能導致重要任務卡頓、動畫掉幀等問題）。

### Fiber Reconciler
為了解決 Stack Reconciler 中固有的問題，以及一些歷史遺留問題，在 React 16 版本推出了新的 Reconciliation 算法的調和器—— Fiber 調和器（Fiber Reconciler）來替代棧調和器。Fiber Reconciler 將會利用調度器（Scheduler）來幫忙處理組件渲染/更新的工作。

此外，引入 fiber 這個概念後，原來的 react element tree 有了一棵對應的 fiber node tree。在 diff 兩棵 react element tree 的差異時，Fiber Reconciler 會基於 fiber node tree 來使用 diff 算法，通過 fiber node 的 `return`、`child`、`sibling` 等屬性能更方便的遍歷 fiber node tree，從而更高效地完成 diff 算法。

## 深入 Reconciliation 工作流程
上面提到，reconciliation 算法主要做了兩件事情：

1. 找出兩棵 react element tree 的差異

2. 將差異更新到真實 DOM，從而完成 UI 更新

下面就要圍繞這兩個點，來研究 reconciliation 算法是怎麼運作的。

### 找出兩棵 React element tree 差異

#### 三大策略
在對比兩棵 react element tree 的差異時，React 制定了三個策略：

1. 只對同級的 react element 進行對比。如果一個 DOM 節點在前後兩次更新中跨越了層級，那麼 React 不會嘗試重用。

2. 兩個不同類型 (react element 中 `type` 字段不一樣) 的 react element 會產生不同的 react element tree。比方說元素 `div` 變為 `span`，react 會銷毀 `div` 及其子孫節點，並新建 `span` 以及其子孫節點。

3. 開發者可以通過 `key` 屬性來暗示哪些子元素在不同的渲染下要保持穩定。
下面用一個例子來說明 `key` 的作用：

```js
// 更新前
<div>
    <p key="frontend">前端</p>
    <h3 key="cclin">cclin</h3>
</div>

// 更新後
<div>
    <h3 key="cclin">cclin</h3>
    <p key="frontend">前端</p>
</div>
```
假如沒有 key，react 會認為 div 的第一個節點由 p 變為 h3，第二個子節點由 h3 變為 p，這也符合上面的第二條規則，因此會銷毀並新建相應的 DOM 節點。

但當我們加上 key 後，其實就指明了節點前後的對應關係，React 就知道 key 為 "frontend" 的 p 在更新後仍然存在，所以 DOM 節點可以被復用，只是需要交換下順序而已。

#### Diff 算法具體過程
下面深入 diff 算法會看到一些源碼，地址如下：

`/packages/react-reconciler/srcReactChildFiber.new.js`

根據上述的第一個策略："只對同級的 react element 進行對比"，意思就是只對同級的節點做對比。同級的意思就是：只屬於同一父節點的那些節點即為同級節點。看下圖：

![](https://seikim.com/i/2021/06/24/p59mlj.jpg)

具體在對比某個節點時，可分為兩種情況：

1. 若新舊節點(react element)的類型(type)或 key 其中有一個不同，則 DOM 不會被復用，直接銷毀。

2. 若新舊節點(react element)的類型(type)和 key 完全相同，則直接復用 DOM。

##### 對比不同 type 的 react element
當對比得出 react element 的 type 不同時，react 會銷毀原來的節點以及其子孫節點，然後重新創建一個新的節點以及其子孫節點。例如：

```js
// 舊
<div>
    <A />
    <div>
        <B />
        <C />
    </div>
</div>
s
// 新
<div>
    <A />
    <span>
        <B />
        <C />
    </span>
</div>
```
上面例子中，原來節點類型為 `div`，更新後變成了 `span`，react 會發現其中的不同，會銷毀 div 節點以及 B,C 節點，然後重新創建一個類型為 span 的 react element 以及其子節點(B,C)。

![](https://seikim.com/i/2021/06/24/pa1925.jpg)

##### 對比相同 type 的 react element(Composite Component / Host Component)

當對比得出 react element 的 type 相同時，react 會先保留該 react element 對應的 DOM 節點(復用 DOM)，然後只對比更新後有改變的屬性。例如說：

```js
// 舊
<div className="before" title="stuff" />

// 新
<div className="after" title="stuff" />
```
通過對比，react 知道只需要修改 DOM 元素上的 `className` 屬性即可。

當更新的是 `style` 時，react 也只會更新有所改變的屬性，例如：

```js
// 舊
<div style={{color: 'red', fontWeight: 'bold'}} />

// 新
<div style={{color: 'green', fontWeight: 'bold'}} />
```
通過對比，react 知道只需要修改 DOM 元素上的 `color` 樣式，`fontWeight` 則無需修改。

上面所舉的例子都是 Host Component，也就是原生的 html 標籤。假如對比的是相同 type 的 Composite Component(自定義的 react 組件)，此時主要就是看組件的 `props` 跟 `state` 有沒有改變，假如有，就要更新組件以及其子組件。

##### 同級只有一個節點
這種情況相對簡單，就是對比新舊兩個節點而已，根據上面說的兩種情況判斷就可以了。

##### 同級有多個節點
當同級有多個節點時，可以分為三種情況：

1. 節點更新(type, 屬性更新)

2. 節點新增或是刪除

3. 節點移動位置

對於同級有多個節點的 diff 算法，一定屬於上述的一種或多種情況。之前我還看到一篇 React 團隊發的小文章，說他們發現在日常開發中，相對於增加和刪除，更新節點發生的頻率還更高，所以 React 的 diff 算法會優先判斷並處理節點的更新。

針對同級的多個節點，我們可以將其看作是一個 list，其實實際上同級的 react element 各自對應的 fiber node 會通過字段 `sibling` 來連接成一個單向鏈表。Diff 算法將會對新的同級節點鏈表進行 2 次遍歷。

1. 第一輪遍歷：處理更新的節點(節點對應的 DOM 可復用，只需更新其中改變的屬性)

2. 第二輪遍歷：處理新增、刪除、移動的節點

###### 第一輪遍歷

> 下面統一一下用詞，分別把『舊 react element tree 的同級節點』『新 react element tree 的同級節點』稱為『舊同級節點鏈表』和『新同級節點鏈表』。

1. 遍歷『舊同級節點鏈表』和『新同級節點鏈表』，從第一個節點開始遍歷(i=0)，判斷新舊節點的 type 和 key 是否相同，如果都相同，則說明對應的 DOM 可以直接復用。

2. 如果這個節點對應的 DOM 可復用，則 i++，判斷下一組新舊節點的 type 和 key，判斷對應的 DOM 是否可以復用，如果可以，則重複步驟 2

3. 如果發現某組新舊節點對應的 DOM 不可復用，則結束遍歷

4. 如果『舊同級節點鏈表』或『新同級節點鏈表』遍歷完了，則結束遍歷

因為這邊主要是要介紹整個 fiber 跟 reconcile 的概念和思路，所以下面先給出很粗淺的代碼實現。源碼中當然不是這樣，關於這部分之後還會單獨寫一篇來分析 react 源碼中一個叫 `reconcileChildrenArray()` 的函數，這篇為了不太過冗長，不寫在這邊了。

簡單實現下，大概如下：

```js
// newNodeList 为 新同級節點鏈表
// oldNodeList 为 舊同級節點鏈表
for (let i = 0; i < newNodeList.length; i++) {
    if (!oldNodeList[i]) break;  // 如果「旧同级节点链表」已经遍历完了，则结束遍历
    if (newNodeList[i].key=== oldNodeList[i].key && 
        newNodeList[i].type === oldNodeList[i].type) {
        continue;  // 对应的 DOM 可复用，则继续遍历
    } else {
        break; // 对应的 DOM 不可复用，则结束遍历
    }
}
```
對於上述第一次遍歷的流程會產生兩種結果：

- `結果一`：在步驟三結束了遍歷，此時『舊同級節點鏈表』和『新同級節點鏈表』都沒有遍歷完，舉個例子：

```js
// 舊
<li key="0">0</li>
<li key="1">1</li>
<li key="2">2</li>

// 新
<li key="0">0</li>
<li key="1">1</li>
<div key="2">2</div>
<li key="3">3</li>
```
![](https://seikim.com/i/2021/06/24/z1uqe5.jpg)

- `結果二`：如果是在步驟四結束遍歷，那麼可能是『新同級節點鏈表』遍歷完，或者『舊同級節點鏈表』遍歷完，又或者他們同時遍歷完。例如：

```js
// 舊
<li key="0" className="a">0</li>
<li key="1" className="b">1</li>
            
// 新
//「新同级节点链表」和「旧同级节点链表」同时遍历完
<li key="0" className="aa">0</li>
<li key="1" className="bb">1</li>
```
![](https://seikim.com/i/2021/06/24/z62wnd.jpg)

```js
// 舊
<li key="0" className="a">0</li>
<li key="1" className="b">1</li>

// 新 
//「新同级节点链表」没遍历完，「旧同级节点链表」就遍历完了
<li key="0" className="aa">0</li>
<li key="1" className="bb">1</li>
<li key="2" className="cc">2</li>
```
![](https://seikim.com/i/2021/06/24/z6z3q8.jpg)

```js
// 舊
<li key="0" className="a">0</li>
<li key="1" className="b">1</li>

// 新
//「新同级节点链表」遍历完了，「旧同级节点链表」还没遍历完
<li key="0" className="aa">0</li>
```
![](https://seikim.com/i/2021/06/24/z8cztg.jpg)

###### 第二輪遍歷
第二輪遍歷時，主要就是遍歷『新同級節點鏈表』中剩下還沒被遍歷處理過的點。

假如第一輪遍歷完的結果為『結果二』：

1. 如果是『新同級節點鏈表』沒有遍歷完，而『舊同級節點鏈表』遍歷完的情況，那麼就說明有節點新增，源碼的做法就是要在新增的節點上打一個 `Placement` 標記(`newFiber.flag |= Placement`)。

![](https://seikim.com/i/2021/06/25/as1ay.jpg)

2. 如果是『新同級節點鏈表』已經遍歷完，『舊同級節點鏈表』沒有遍歷完的情況，就表示有節點被刪除了，這個即將要被刪除的節點就會被打上一個 `Deletion` 標記(`returnFiber.flags |= Deletion`)

假如第一輪遍歷完的結果為『結果一』：

假如為結果一，說明新舊同級節點鏈表都沒有遍歷完，這意味有的節點在這次更新中可能改變了`位置`！接下來問題就變成要處理位置變換的節點。處理節點位置變化的 2 大主要思路就是：

1. 剩下的節點中，那些需要『右』移

2. 如果要移，那要移到什麼位置

由於有節點改變了位置，所以我們不能再通過節點的索引來對比新舊節點了。這時候，我們就要利用 `key` 來將新舊節點對應上。

在遍歷「新同級節點鏈表」時，為了能快速在「舊同級節點鏈表」中找到對應的舊節點，React 會將「舊同級節點鏈表」中還沒被處理過的節點以 map 的形式存放起來，其中 key 屬性為 `key`，value 為 `fiber node`，這個 map 叫做 `existingChildren`：

```js
const existingChildren = mapRemainingChildren(returnFiber, oldFiber);
```
existingChildren 是如何發揮作用的呢？在第二輪遍歷時：

1. 假如遍歷到的「新同級節點」A 的 key 在 `existingChildren` 中可以找到，則說明在「舊同級節點鏈表」中可以找到一個和 A 的 key 相同的「舊同級節點」A1。由於是通過 map 進行匹配的，很明確的一點就是 A 和 A1 的 key 是相同的，接下來就是判斷它們的 type 是否相同：

- 假如 key 相同、type 也相同，說明該節點對應的 DOM 可復用，只是位置發生了變化

- 假如 key 相同、type 不同，則該節點對應的 DOM 不可復用，需要銷毀原來的節點，並重新插入一個新的節點

2. 假如遍歷到的「新同級節點」A 的 key 在 `existingChildren` 中找不到，則說明在「舊同級節點鏈表」中找不到和 A 的 key 相同的「舊同級節點」A1，那就說明 A 是一個 `新增節點`

解決了 `新節點如何對應找到舊節點的問題` 後。接下來我們來看看具體在第二輪循環的時候如何處理節點新增、刪除、移動的。

其實新增和刪除節點的情況很好理解，其實上面講“兩種結果”的時候已經說明了新增、刪除的情況了。下面我們重點來研究一下節點移動的情況。在前面曾經說過，處理節點的位置變化，主要抓住兩個點：

1. 哪個節點需要向右移？

2. 向右移動到哪個位置？

以上兩個問題實際上涉及到的是 `方向` 和 `位移`，如果想要明確這兩個東西，就需要一個「基準點」，或者說「參考點」。React 使用 `lastPlacedIndex` 這個變量來存放「參考點」。我們可以在源碼的 `reconcileChildrenArray()` 函數的開頭，看到：

```js
let lastPlacedIndex = 0;
```
`lastPlacedIndex` 這個變量表示當前最後一個可復用的節點，對應的是「舊同級節點鏈表」中的索引。（這個定義理解起來可能有點繞，不過沒關系，等下看兩個例子就知道它究竟存的什麽東西了）

在遍歷剩下的「新同級節點鏈表」時，每一個新節點會通過 `existingChildren` 找到對應的舊節點，然後就可以得到舊節點的索引 `oldIndex`（即在「舊同級節點鏈表」中的位置）。

接下來會進行以下判斷：

- 假如 `oldIndex >= lastPlacedIndex`，代表該復用節點不需要移動位置，並將 `lastPlacedIndex = oldIndex`

- 假如 `oldIndex < lastPlacedIndex`，代表該節點需要向右移動，並且該節點需要移動到上一個遍歷到的新節點的後面

#### Diff 算法 Example 1
上述就是處理節點移動的邏輯。看完之後當時本人還是非常之懵，此時真的是需要配合一些實際例子來理解感受，才能理解的更到位～

```js
// 舊
<li key="a">a</li>
<li key="b">b</li>
<li key="c">c</li>
<li key="d">d</li>

// 新
<li key="a">a</li>
<li key="c">c</li>
<li key="d">d</li>
<li key="b">b</li>
```
![](https://seikim.com/i/2021/06/25/1ub0ng.jpg)

首先進行的是第一輪的循環：

![](https://seikim.com/i/2021/06/25/1uq3y8.jpg)

第一輪循環結束，接著第二輪：

剛剛第一遍循環只處理了第一個節點 a，目前「舊同級節點鏈表」中還有 b、c、d 還未被遍歷處理，「新同級節點列表」中還有 c、d、b 還未被遍歷處理。新、舊同級節點鏈表均沒有完成遍歷(屬於結果一)，也就是說，沒有節點新增或刪除，說明有節點變化了位置。因此接下來的第二輪循環，主要是處理節點的位置移動。在開始處理之前，先把「舊同級節點鏈表」中未被遍歷處理的的 b、c、d 節點以 map 的形式存放到 `existingChildren` 中。

「新同級節點列表」遍歷到節點c:

![](https://seikim.com/i/2021/06/25/1xe219.jpg)

![](https://seikim.com/i/2021/06/25/1xe4ux.jpg)

![](https://seikim.com/i/2021/06/25/1xe7n7.jpg)

第二輪遍歷到此結束，最終，節點 a、c、d 對應的 DOM 節點都沒有移動，而節點 b 對應的 DOM 則會被標記為“需要移動”。

於是，經過兩輪循環後，React 就知道了，想要從「舊同級節點鏈表」變成「新同級節點鏈表」那樣子，需要「舊同級節點鏈表」經過以下每個節點的操作：

1. 節點 a 位置不變
2. 節點 b 向右移動到節點 d 的後面
3. 節點 c 位置不變
4. 節點 d 位置不變

![](https://seikim.com/i/2021/06/25/1zq24u.jpg)

#### Diff 算法 Example 2
假設現有的新舊兩個同級節點鏈表：

```js
// 舊
<li key="a">a</li>
<li key="b">b</li>
<li key="c">c</li>
<li key="d">d</li>

// 新
<li key="d">d</li>
<li key="a">a</li>
<div key="b">b</div>
<li key="c">c</li>
```
![](https://seikim.com/i/2021/06/25/21wtmn.jpg)

首先進行的是第一輪的循環：

![](https://seikim.com/i/2021/06/25/21ws3e.jpg)

第一輪循環結束，接著第二輪：

![](https://seikim.com/i/2021/06/25/24tn50.jpg)

![](https://seikim.com/i/2021/06/25/24tmi0.jpg)

![](https://seikim.com/i/2021/06/25/24tpck.jpg)

![](https://seikim.com/i/2021/06/25/24tlou.jpg)

經過兩輪循環後，React 就知道了，想要從「舊同級節點鏈表」變成「新同級節點鏈表」那樣子，需要「舊同級節點鏈表」經過以下每個節點的操作：

1. 節點 d 位置不變
2. 節點 a 向右移動到節點 d 的後面
3. 在節點 a 後面插入一個新的節點 b，其類型為 div，然後刪除原來的節點 b
4. 節點 c 向右移動到節點 b 的後面

![](https://seikim.com/i/2021/06/25/25h2iw.jpg)

上述每個節點各自的“操作”（work）—— “移動到哪裏”、“位置不變”、“插入新的，刪掉舊的” 等等，會存放到節點各自對應的 fiber node 中。等到渲染階段（Render phase）時，React 會讀取並執行這些 “操作”，從而完成對真實 DOM 的更新。

### 更新差異到真實 DOM，更新 UI
經過上面的對比找出了「差異」之後，React 知道了“哪些 react element 要被刪除”、“哪些 react element 需要添加子節點”、“哪些 react element 位置需要移動”、“哪些 react element 的屬性需要更新”等等的一系列操作，這些操作會被看作一個個更新任務（work）。每個 react element 自身的更新任務（work）會存儲在與這個 react element 對應的 `fiber node` 中。

在 渲染階段（Render phase），Reconciliation 會從 fiber node tree 最頂端的節點開始，重新對整棵 fiber node tree 進行 `深度優先遍歷`，遍歷樹中的每一個 fiber node，處理 fiber node 中存儲的 work。

遍歷一次 fiber node tree 執行其中的 work 的這個過程被稱作一次 `work loop`。當一個 fiber node 自己和其所有子節點（child）分支上的 work 都被完成了，此時這個 fiber node 的 work 才算完成。一旦一個 fiber node 的 work 完成了，也就是說這個 fiber node 被結束了，React 會接著去處理它的兄弟節點（fiber node `silbing` 字段所指向的 fiber node）的 work，在完成這個兄弟節點（sibling）的 work 後，就會繼續移步到下一個 sibling 節點......以此類推。當所有的 sibling 節點的 work 都處理完成後，React 才會回溯到 parent 節點（通過 fiber node `return` 字段一步步回溯）。

React 的開發者在這裏做了一個優化，也就是前面提到過的 `Effect List`。在每次更新要重新開始一次新的 work loop 時，React 會跳過那些已經處理過的 fiber node，只會去處理那些帶有未完成 work 的 fiber node。舉個例子，如果在組件樹的深層去調用 `setState()` 方法的話，那麽 React 雖然還是會從 fiber node tree 的頂部的節點開始遍歷，但是它會跳過前面所有的父節點，直奔那個調用了 `setState()` 方法的子節點。

當 `work loop` 結束後（也就是遍歷完整棵 fiber node tree 後），就會準備進入 `commit` 階段（Commit phase）。在 commit 階段，React 會去更新真實 DOM 樹，從而完成 UI 的更新渲染。

# 結語
這篇前前後後寫了好幾天，大概有5,6天吧才完成，而且老實說，寫到後面前後都有點亂掉了，實在是概念有點複雜且多，而且整體難度上來說也真的是蠻複雜的。畢竟 Fiber 跟 Reconciliation 的 diff 算法可以算是 React 這個如此龐大的體系中非常核心的兩大支柱，要真正完全的通透理解可還不是那麼容易。

本篇介紹了 React 中的 fiber 到底是個什麼東西，以及非常重要的 reconciliation 的 diff 算法，整體上來說個人感覺真的已經非常清晰了。不過本篇仍然停留在概念的層面上，對於像是 diff、還有 work loop、render 等等其實都還沒有深入到 react 源碼的層次，之後都會一一堅持下去繼續學習，再分享出來。這篇真的寫得蠻長的，也是真寫不動了，能看完我也真的佩服你，看完的話相信多多少少有些幫助的，先撤了886~

# 源碼查看

|源碼|鏈接|
|:-----|:-----|
|ReactElement 类型定义|package/shared/ReactElementType.js|
|fiber 类型定义|packages/react-reconciler/src/ReactInternalTypes.js|
|创建 fiber|packages/react-reconciler/src/ReactFiber.new.js|
|diff 过程|`reconcileChildrenFibers` 函数（/packages/react-reconciler/src/ReactChildFiber.new.js）|

# 參考

|參考|鏈接|
|:------|:------|
|Reconciliation 官方文档|https://react.html.cn/docs/reconciliation.html|
|React Virtual DOM 官方文档|https://reactjs.org/docs/faq-internals.html|
|React源码揭秘3 Diff算法详解|https://juejin.cn/post/6844904167472005134#heading-4|
|Inside Fiber: in-depth overview of the new reconciliation algorithm in React|https://medium.com/react-in-depth/inside-fiber-in-depth-overview-of-the-new-reconciliation-algorithm-in-react-e1c04700ef6e|
|React Fiber Architecture|https://github.com/acdlite/react-fiber-architecture|


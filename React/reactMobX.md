@[TOC](React + MobX - 完全上手指南)

# 前言

之前在學習 react 的狀態管理時當然第一個學到的就是 redux，關於 redux 如果有興趣或是還不清楚，可以先去看看 [淺析 React Redux 的概念以及使用](https://blog.csdn.net/weixin_46803507/article/details/116499240)。而本篇的主角是 MobX，這篇會介紹關於 MobX 的一些概念以及基本用法，當然也會講到跟 redux 的區別，包括優勢選型等比較。

# 正文

MobX 其實跟 redux 一樣也是一個用於做全局狀態管理的一個工具，不過經過下面的介紹你就會發現 MobX 比 redux 方便簡單很多，所以其實 MobX 也蠻流行的。我會在介紹完 MobX 後再統一說說跟 redux 的對比。下面一樣以 todo 應用作為場景說說 MobX 的實現，廢話不多說進入 MobX 吧～

## MobX 準備工作
首先，要在 react 應用中使用 MobX 我們當然要先裝 MobX 的包：

```js
// npm
npm install mobx mobx-react

// yarn
yarn add mobx mobx-react
```

## MobX 基本使用

其實 MobX 的概念很簡單，首先當然還是先要設計我們 todo 的 store。我們當然也是先建一個 store 文件夾存放狀態管理相關的所有代碼。

### Store + Action
如果對 redux 有了解的人會知道，在 redux 中，redux 只有一個統一的大的 store，而具體怎麼處理，是根據你分發的 action 中的 `type` 進行不同的 reducer 處理，然後返回新的 state。

不過在 MobX 不是這樣的。MobX 不是單一數據源，在 MobX 可以有多個 store，我們可以根據不同的業務將狀態分成各自的 store，其實在某種意義上更直觀一點。在 MobX 中，我們借住 ES6 的 `class` 實現 store，來看看代碼：

```ts
// /store/todoStore.ts
import { makeAutoObservable } from "mobx";

export interface Todo {
  id: number;
  text: string;
  done: boolean;
}

const removeTodo = (todos: Todo[], id: number): Todo[] =>
  todos.filter((todo) => todo.id !== id);

const addTodo = (todos: Todo[], text: string): Todo[] => [
  ...todos,
  {
    id: Math.max(0, Math.max(...todos.map(({ id }) => id))) + 1,
    text,
    done: false,
  },
];

// Todo 總數據源
class TodoStore {
  todos: Todo[] = [];
  newTodo: string = "";

  constructor() {
    makeAutoObservable(this);
  }

  addTodo() {
    this.todos = addTodo(this.todos, this.newTodo);
    this.newTodo = "";
  }

  removeTodo(id: number) {
    this.todos = removeTodo(this.todos, id);
  }

  loadTodo(url: string) {
    fetch(url)
      .then((res) => res.json())
      .then((data) => (this.todos = data));
  }
}

const todoStore = new TodoStore();
export default todoStore;
```
上面代碼就是 MobX 中的 store 的寫法。通過 `class` 定義我們的 todo 應用狀態，其中我們可以直接在 `class` 中直接定義狀態以及更新狀態的函數。最後我們 `new` 出一個 todoStore 實例並 export 出去，這樣保證我們的 react 應用中只會有一個 todo 全局狀態。

可以看到比起 redux 要寫一堆 actions, reducers 等等，顯然 MobX 相對非常的方便，也好理解，而且還可以分很多個 store。

唯一不一樣的地方就是 todoStore 這個 `class` 中的 `constructor`。這邊我們引入 mobx 提供的 `makeAutoObservable` 函數，`makeAutoObservable(this)` 的用意就是說當我們的應用啟動，new 出 todoStore 的唯一實例的同時，讓 todoStore 變得可被觀察，也就是說可以追蹤 todoStore 所有 state 的變化，並通知整個應用中所有引用了 todoStore 的組件。

### 組件中 MobX 生效
MobX 中上面就定義好了整個 store 以及相應的 actions 了，因為可以直接對 state 做更改，所以也沒有了 reducer 的概念。接下來就是要在具體的組件中獲取 store。

這邊直接以添加 todo 的例子來看看怎麼使用：
```ts
// /components/TodoAdd.tsx
 
import * as React from "react";
import { Button, Input, Grid } from "@chakra-ui/react";

import todoStore from "../store/todoStore";
import { observer } from "mobx-react";

function TodoAdd() {
  return (
    <Grid pt={2} templateColumns="5fr 1fr" columnGap="3">
      <Input
        placeholder="New todo"
        value={todoStore.newTodo}
        onChange={(e) => (todoStore.newTodo = e.target.value)}
      />
      <Button onClick={() => todoStore.addTodo()}>Add Todo</Button>
    </Grid>
  );
}

export default observer(TodoAdd);
```
我們直接將 export 的 todoStore 引入，這樣就可以直接使用 todoStore 中的 state 跟 action。

不一樣的地方在我們引入了 mobx-react 提供的 `observer` 函數。這個 `observer` 函數的目的就是讓該組件和所有經過 `makeAutoObservable` 的 store(可能不只一個)配合起來，該組件中只要依賴了被 observe 的 state 發生了變化就會同步更新視圖。

`observer` 函數會返回一個新的組件簽名，默認就是原組件名，我們也可以改變名字。總之就是將原組件做了一層包裝，使其和 `makeAutoObservable` 的 store 綁定起來。

```ts
// /components/TodoList.tsx

import * as React from "react";
import { Button, Input, Flex, Checkbox, Heading } from "@chakra-ui/react";

import todoStore, { Todo } from "../store/todoStore";
import { observer } from "mobx-react";

function TodoListItems() {
  return (
    <>
      {todoStore.todos.map((todo: Todo) => (
        <Flex pt={2} key={todo.id}>
          <Checkbox onClick={() => (todo.done = !todo.done)} />
          <Input
            mx={2}
            value={todo.text}
            onChange={(e) => (todo.text = e.target.value)}
          />
          <Button onClick={() => todoStore.removeTodo(todo.id)}>Delete</Button>
        </Flex>
      ))}
    </>
  );
}

const TodoListItemsObserver = observer(TodoListItems);

function TodoList() {
  return (
    <>
      <Heading>Todo List</Heading>
      <TodoListItemsObserver />
    </>
  );
}

export default TodoList;
```
```ts
// /components/TopBar.tsx

import * as React from "react";
import { Button, Grid } from "@chakra-ui/react";
import { ColorModeSwitcher } from "./ColorModeSwitcher";
import todoStore from '../store/todoStore';

function TopBar() {
  const load = () => {
    todoStore.loadTodo("https://raw.githubusercontent.com/jherr/todos-four-ways/master/data/todos.json")
  }

  return (
    <Grid pt={2} templateColumns="1fr 1fr" columnGap="3">
      <ColorModeSwitcher />
      <Button onClick={load}>Load</Button>
    </Grid>
  );
}

export default TopBar;
```
## MobX Decorators
上面其實就是 MobX 大概的用法，不過我個人覺得要把組件外面還要包一層 `observer` 有點醜，而且這麼做把代碼跟狀態又有點硬編碼在一起。

這邊介紹另一種使用 MobX 的方式，decorators。Decorator 的中文是裝飾器，如果有用過 springboot 的可能就知道，其實跟 spring 中的註釋(annotation)很像。在具體使用 MobX decorators 之前有一些工作要準備。

### MobX Decorators 準備工作
在 react 中使用 MobX 如果想配合著 decorators 使用有一些配置工作，個人感覺過程有不少坑，在此紀錄一下。

首先我們要安裝兩個額外的包：

- react-app-rewired
- react-app-rewire-mobx

```js
// npm 
npm install react-app-rewired react-app-rewire-mobx

// yarn 
yarn add react-app-rewired react-app-rewire-mobx
```
其實我們的目的就是要重寫 cra 的配置，讓 cra 可以認識 decorators，所以其實有另一個方法是 `npm eject` 彈出配置，不過因為這個過程是不可逆的，所以通常比較不推薦。

`react-app-rewired` 的功能就是協助我們重寫 cra 這個 react 腳手架的配置。`react-app-rewired` 提供了一個 `injectBabelPlugin`，通過這個函數我們可以改變配置加入一些 babel plugin。

根據官方文檔，我們需要再 /src 下面新建一個名為 `config-overrides.js` 的文件，並在該文件中我們就可以開始添加新的 babel 插件更改配置等等，這邊我們加上一個 `babel-plugin-styled-components` 作為示範(當然也要先裝，npm/yarn)，因為在代碼中嘗試使用了一點點 styled-components。

要注意的是，在 `config-overrides.js` 中只能使用 CommonJS 的語法，不支持 ES6 import/export 的語法。
 
```js
// config-overrides.js

const { injectBabelPlugin } = require("react-app-rewired");
const rewireMobX = require("react-app-rewire-mobx");

module.exports = function override(config, env) {
  config = injectBabelPlugin("babel-plugin-styled-components", config);
  config = rewireMobX(config, env);

  return config;
};
```
具體代碼如上，最後還有一個要注意的點。我們還要到 package.json 中修改一下 scripts 的配置，因為我們通過 `react-app-rewired` 重寫了一些配置，為了讓我們的配置在啟動時被啟用，我們就不能再使用 default cra 為我們提供了 react 腳本，而要使用 `react-app-rewired` 提供了，更改如下：
```json
  "scripts": {
-   "start": "react-scripts start",
+   "start": "react-app-rewired start",
-   "build": "react-scripts build",
+   "build": "react-app-rewired build",
-   "test": "react-scripts test",
+   "test": "react-app-rewired test",
    "eject": "react-scripts eject"
}
```
github 上源項目中特別提醒不要更改 `eject` 的腳本，原因就是因為當我們不再需要 `react-app-rewired` 後我們仍然可以通過執行 default 的 `eject` 來重置配置，且 `react-app-rewired` 也沒有提供對 `eject` 的腳本，所以不要亂來hhh。

### 使用 MobX Decorators
經過上面的配置我們就可以在 react 中使用 mobx 裝飾器了，接下來就介紹怎麼使用以及一些特別有用的裝飾器。

下面會實現一個簡易更改主題的效果，配合 mobx 裝飾器以及 styled-components。

同樣我們通過 `class` 來組織我們的 store，這邊我們設計一個 theme 屬性，通過 theme 的變化要觸發相應的視圖渲染，也就是說 theme 這個 state 應該要是 `observable`，這邊引出第一個 decorator。

#### @observable
通過 `@observable` 就可以使我們的 state 變成可觀察的，也就是說 state 的變化會引發通知任何依賴該 state 的組件進行重新渲染。
```js
import { observable } from 'mobx';

@observable 
theme = "day";
```

#### @action
`@action` 裝飾器可以允許我們對 `@observable` 的 state 進行更改。
```js
@action
toggleTheme = () => {
  this.theme = this.theme === "day" ? "night" : "day";
};
```

#### @computed
`@computed` 可以說是計算屬性，這個裝飾器可以允許我們基於 observable 的 state 返回關於 state 的計算屬性，其實跟 vue 中的 `computed` 很像。
```js
@observable
todos = [];

@action
addTodo = (todo) => {
  this.todos.push(todo);
};

@computed
get todoCount() {
  return this.todos.length;
};
```
#### Store 代碼
```js
// /stores/UiStore.js

import { action, observable } from "mobx";

class UiStore {
  @observable
  theme = "day";

  @action
  toggleTheme = () => {
    this.theme = this.theme === "day" ? "night" : "day";
  };
}

const uiStore = new UiStore();
export default uiStore;
```
```js
// /stores/TodoStore.js

import { action, observable, computed } from "mobx";

class TodoStore {
  @observable
  todos = [];

  @action
  addTodo = (todo) => {
    this.todos.push(todo);
  };

  @computed
  get todoCount() {
    return this.todos.length;
  }
}

const todoStore = new TodoStore();
export default todoStore;
```

#### Provider
在 MobX 中我們可能會有很多個 store，而我們必須要使這些 store 和應用裝配起來。這邊跟 redux 很像，通過一個全局的 `Provider` 標籤將所有需要裝配的 store 裝配起來：

```js
// /stores/Store.js

import uiStore from "./UiStore";
import todoStore from "./TodoStore";

class Store {
  constructor() {
    this.uiStore = uiStore;
    this.todoStore = todoStore;
  }
}

const store = new Store();
export default store;
```

```js
// index.js

import React from "react";
import ReactDOM from "react-dom";
import "./index.css";
import App from "./App";
import registerServiceWorker from "./registerServiceWorker";

import { Provider } from "mobx-react";
import store from './stores/Store';

ReactDOM.render(
  <Provider store={store}>
    <App />
  </Provider>,
  document.getElementById("root")
);
registerServiceWorker();
```
我們新建一個 store 作為應用的 根 store，並且通過 `Provider` 與整個應用裝配在一起。

#### @inject
上面我們做好了 mobx 中的 store 的設計，也跟應用裝配在一起，現在就是要在組件中實際用上這些 store。

`@inject` 裝飾器加在類組件之上，告訴組件要注入哪些 store。
```js
@inject('store')
class App extends Component {
  // ...
}
```
#### @observer
`@observer` 裝飾器其實跟上面不用裝飾器的 `observer()` 作用一樣，就是讓組件中只要依賴了被 observe 的 state 發生了變化就會同步更新視圖，與 `@inject` 的 store 有關。
```js
@inject('store')
@observer
class App extends Component {
  render() {
    const { uiStore, todoStore } = this.props.store;


    return (
      // code
    )
  }
}
```
## MobX 異步
上面還算詳細的介紹了 mobx 的用法以及配合 react 的方式。不過注意到，上面對 store 數據的操作都是同步的，這邊就來介紹一下在 mobx 中的異步怎麼做，其實也很簡單的。

其實異步一樣是 action，最一開始基本的 mobx 中其實有寫到一個異步，如下：
```ts
@action
loadTodo(url: string) {
  fetch(url)
    .then((res) => res.json())
    .then((data) => (this.todos = data));
}
```
感覺上好像沒什麼問題，但其實這是比較不好的寫法。如果我們的 react 應用啟用了 `use Strict` 的檢查，那就上面代碼會出現以下報錯：
```
Unhandled Rejection(Error): [mobx] Since strict-mode is enabled, changing observed observable values outside actions are not allowed.
```
原因就是因為，在 mobx 中規定 action 對 observable state 的修改必須在 action 體內，而上面代碼顯然把對 todos 的更改放在了 `then` 回調函數內。

當然，第一種方式就是再多寫一個 action 專門用於改變 state 的值，類似 setter 概念，不過如果有很多 state 那這種方式顯然不好。

所以我們借住 mobx 提供了一個 `runInAction` 函數，它接受一個函數作為參數，裡面寫我們的邏輯就好，很方便，如下：
```js
@action
loadTodos = (url) => {
  fetch(url)
    .then((res) => res.json())
    .then((data) => {
      runInAction(() => {
        this.todos = data;
      });
    });
};
```
或是你喜歡用 `async/await` 當然也可以：
```js
@action
loadTodos = async url => {
  const res = await fetch(url);
  const data = await res.json();
  runInAction(() => {
    this.todos = data;
  })
}
```
## MobX vs Redux
最後，就來說說 mobx 跟 redux 到底差別在什麼地方，個人感覺在了解技術的同時也要知道到底有什麼區別。

- Redux 原理

![](https://riyugo.com/i/2021/05/17/phl5jn.png)

- MobX 原理

![](https://riyugo.com/i/2021/05/17/pgz7q1.png)

總結來說，mobx 和 redux 有以下差異：

- 函数式和面向对象
- 单一store和多store
- JavaScript对象和可观察对象
- state 不可变（Immutable）和可变（Mutable）

這邊分享一篇關於 mobx 跟 redux 的比較，還算詳細，[你需要Mobx还是Redux？](https://juejin.cn/post/6844903562095362056#heading-3)。
 
# 結語
本篇介紹了 MobX 這個 react 狀態管理工具，整體應該還算詳細，使用上感覺確實比 redux 直觀一點，不過還是要在具體項目中實際實踐才能更有感覺。希望看完這篇對不知道 mobx 的你有一些幫助，若有錯誤或不完整也歡迎請多指教！

# 參考
|參考|鏈接|
|:-----|:-----|
|MobX - document|https://doc.ebichu.cc/mobx/refguide/object.html|
|react-app-rewired|https://github.com/timarney/react-app-rewired#how-to-rewire-your-create-react-app-project|
|Rewire create-react-app to use MobX|https://www.npmjs.com/package/react-app-rewire-mobx/v/1.0.8|
|react-app-rewired使用|http://wmm66.com/index/article/detail/id/165.html|
|create-react-app + mobx + yarn 架子，你要的轮子|https://www.jianshu.com/p/181be41fba6b|
|终于讲清楚了nodejs中exports和module.exports的区别|https://blog.csdn.net/qq_31967569/article/details/82461499|
|你需要Mobx还是Redux？|https://juejin.cn/post/6844903562095362056|


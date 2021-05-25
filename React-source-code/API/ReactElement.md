@[TOC](React 源碼解析 - API 系列 - React.createElement() 與 ReactElement())

# 前言

本人最近就在學習 react，其實也還不能算是非常熟練 react，但是總是算是有了一點點基礎。其實以前也寫過一點 vue，但是總是感覺一知半解，所以學習 react 決定對於一些比較重要的地方要深入點，於是開始了 react 源碼的學習。這篇從 `React.createElement()` 和 `ReactElement()` 這兩個比較基礎但是重要的 api 開始。

# 正文

## jsx 轉換 js

學 react 肯定就已經知道 jsx 是什麼了，我們寫 react 時其實都是用 jsx 而不是 js，而從 jsx 轉換到 js 就會用到 `React.createElement()`，所以我們先熟悉一下 jsx 和 js。

我們先來看一個對比：

- jsx 語法

```html
<div id="one" class="two">
  <span id="spanOne">this is spanOne</span>
  <span id="spanTwo">this is spanTwo</span>
</div>
```

- 轉換成 js 語法

```js
React.createElement(
  "div",
  { id: "one", class: "two" },
  React.createElement("span", { id: "spanOne" }, "this is spanOne"),
  React.createElement("span", { id: "spanTwo" }, "this is spanTwo")
);
```

`React.createElement()` 這個方法的簽名如下：

> React.createElement("标签名", "Object : 標籤的 props", 'children 子节点 1', 'children 子节点 2','...')

上面就是基本的 jsx 轉換 js 的規則，那麼組件呢，我們都知道可以用 `function` 寫組件，那麼這樣的 jsx 轉成 js 會長怎樣呢？看下面：

- jsx 語法

```js
function Div() {}

<Div id="one" class="two"></Div>;
```

- 轉換成 js 語法

```js
React.createElement(Div, {} , xxx );
```
如果標籤名大寫，那就代表是一個 react 組件，小寫就是 html 代碼。

## React.createElement()

- 源碼地址：https://github.com/facebook/react/blob/master/packages/react/src/ReactElement.js

- 作用：這個方法的作用就是創建 react 元素，實現 jsx 到 js 的轉換。

### 源碼

```js
/**
 * Create and return a new ReactElement of the given type.
 * See https://reactjs.org/docs/react-api.html#createelement
 */

//注意：react只寫了3個參數，實際上，從第3個參數往後都是children
export function createElement(type, config, children) {
  let propName;

  // Reserved names are extracted
  const props = {};

  let key = null;
  let ref = null;
  let self = null;
  let source = null;

  // 賦給標籤的props不為空時
  if (config != null) {
    if (hasValidRef(config)) {
      ref = config.ref;

      if (__DEV__) {
        warnIfStringRefCannotBeAutoConverted(config);
      }
    }
    if (hasValidKey(config)) {
      //防止是Number
      key = '' + config.key;
    }

    self = config.__self === undefined ? null : config.__self;
    source = config.__source === undefined ? null : config.__source;
    // Remaining properties are added to a new props object
    for (propName in config) {
      //如果config中的屬性不是標簽原生屬性，則放入props對象中
      if (
        hasOwnProperty.call(config, propName) &&
        !RESERVED_PROPS.hasOwnProperty(propName)
      ) {
        props[propName] = config[propName];
      }
    }
  }

  // Children can be more than one argument, and those are transferred onto
  // the newly allocated props object.
  //子元素数量
  const childrenLength = arguments.length - 2;
  if (childrenLength === 1) {
    props.children = children;
  } else if (childrenLength > 1) {
    const childArray = Array(childrenLength);
    for (let i = 0; i < childrenLength; i++) {
      childArray[i] = arguments[i + 2];
    }
    //如果是development环境的话
    if (__DEV__) {
      //冻结array
      if (Object.freeze) {
        Object.freeze(childArray);
      }
    }
    //开发中写的this.props.children就是子元素的集合
    props.children = childArray;
  }

  // Resolve default props
  //为传入的props设置默认值
  if (type && type.defaultProps) {
    const defaultProps = type.defaultProps;
    for (propName in defaultProps) {
      if (props[propName] === undefined) {
        props[propName] = defaultProps[propName];
      }
    }
  }
  if (__DEV__) {
    //一旦ref或key存在
    if (key || ref) {
      //如果type是组件的话，赋值displayName
      const displayName =
        typeof type === 'function'
          ? type.displayName || type.name || 'Unknown'
          : type;
      if (key) {
        defineKeyPropWarningGetter(props, displayName);
      }
      if (ref) {
        defineRefPropWarningGetter(props, displayName);
      }
    }
  }
  return ReactElement(
    type,
    key,
    ref,
    self,
    source,
    ReactCurrentOwner.current,
    props,
  );
}
```
我在源碼上加上了一些註解，建議先過一遍，也對接下來的分析會先有個印象。

### 分析

#### hasValidRef()

- 作用：判斷是否設置了 `ref` 屬性。

- 源碼

```js
//判斷是否設置了ref的屬性,true有，false沒有
function hasValidRef(config) {
  //如果是development環境的話
  if (__DEV__) {
    //如果config中存在ref屬性的話
    if (hasOwnProperty.call(config, 'ref')) {
      //返回對象config的屬性ref 的get對象
      const getter = Object.getOwnPropertyDescriptor(config, 'ref').get;
      if (getter && getter.isReactWarning) {
        return false;
      }
    }
  }
  return config.ref !== undefined;
}
```
- `__DEV__` 其實沒什麼，只是表示測試環境，不是特別重要。

- 雖然其實 `if (__DEV__)` 裡面不是特別重要(因為是給 react 內部測試用)，但是這邊也順便來學學裡面用到的 `Object.getOwnPropertyDescriptor`。

`Object.getOwnPropertyDescriptor` 作用返回一個屬性的描述對象。
```js
const obj = {
  a: 1,
};

console.log(Object.getOwnPropertyDescriptor(obj, "a"));

// { value: 1, writable: true, enumerable: true, configurable: true }
```
#### hasValidKey()

- 作用：判斷是否設置了 `key`，跟 `hasValidRef()` 一樣。

- 源碼

```js
function hasValidKey(config) {
  if (__DEV__) {
    if (hasOwnProperty.call(config, 'key')) {
      const getter = Object.getOwnPropertyDescriptor(config, 'key').get;
      if (getter && getter.isReactWarning) {
        return false;
      }
    }
  }
  return config.key !== undefined;
}
```
#### children
在 `createElement()` 中雖然只傳三個參數，但從第三個參數開始其實都屬於 children，具體源碼中是利用了 `arguments` 來獲取剩下參數。

這邊對於還不清楚 `arguments` 的人這邊也做一個理解整理。

當我們在 js 中在調用一個函數的時候，我們經常會給這個函數傳遞一些參數，js 把傳入到這個函數的全部參數存儲在一個叫做arguments 的東西裏面，那麽這到底是什麽東西？

我們也都知道，在 js 中萬物皆對象，甚至數組字符串函數都是對象。所以這個叫做 arguments 的東西其實也是個對象，而且是一個特殊的對象，它的屬性名是按照傳入參數的序列來的，第1個參數的屬性名是’0’，第2個參數的屬性名是’1’，以此類推，並且它還有length屬性，存儲的是當前傳入函數參數的個數，很多時候我們把這種對象叫做`類數組對象`，這個概念非常重要。

```js
function add(a, b) {
  console.log(arguments)
  console.log(args)
  return a + b
}

const res = add(1, 2)
console.log(res)

// [Arguments] { '0': 1, '1': 2 }
// 3
```
#### return
最後 `return` 調用了 `ReactElement()` 方法，主要重要的就是把處理過的 type, key, ref, props 傳給 `ReactElement()`，接下來當然就再來看看本篇第二個 api，`ReactElement()`。

## `ReactElement()`

- 源碼地址：https://github.com/facebook/react/blob/master/packages/react/src/ReactElement.js

- 作用：創建出實際的 React 元素，其實如果在控制台中打印出一個 react 組件會長下面這樣：

![](https://riyugo.com/i/2021/05/25/rbn9y6.png)

### 源碼
```js
/**
 * Factory method to create a new React element. This no longer adheres to
 * the class pattern, so do not use new to call it. Also, instanceof check
 * will not work. Instead test $$typeof field against Symbol.for('react.element') to check
 * if something is a React Element.
 *
 * @param {*} type
 * @param {*} props
 * @param {*} key
 * @param {string|object} ref
 * @param {*} owner
 * @param {*} self A *temporary* helper to detect places where `this` is
 * different from the `owner` when React.createElement is called, so that we
 * can warn. We want to get rid of owner and replace string `ref`s with arrow
 * functions, and as long as `this` and owner are the same, there will be no
 * change in behavior.
 * @param {*} source An annotation object (added by a transpiler or otherwise)
 * indicating filename, line number, and/or other information.
 * @internal
 */
const ReactElement = function(type, key, ref, self, source, owner, props) {
  const element = {
    // This tag allows us to uniquely identify this as a React Element
    //標示element的類型
    $$typeof: REACT_ELEMENT_TYPE,

    // Built-in properties that belong on the element
    //設置元素的內置屬性
    type: type,
    key: key,
    ref: ref,
    props: props,

    // Record the component responsible for creating this element.
    _owner: owner,
  };

  if (__DEV__) {
    // The validation flag is currently mutative. We put it on
    // an external backing store so that we can freeze the whole object.
    // This can be replaced with a WeakMap once they are implemented in
    // commonly used development environments.
    element._store = {};

    // To make comparing ReactElements easier for testing purposes, we make
    // the validation flag non-enumerable (where possible, which should
    // include every environment we run tests in), so the test framework
    // ignores it.
    Object.defineProperty(element._store, 'validated', {
      configurable: false,
      enumerable: false,
      writable: true,
      value: false,
    });
    // self and source are DEV only properties.
    Object.defineProperty(element, '_self', {
      configurable: false,
      enumerable: false,
      writable: false,
      value: self,
    });
    // Two elements created in two different places should be considered
    // equal for testing purposes and therefore we hide it from enumeration.
    Object.defineProperty(element, '_source', {
      configurable: false,
      enumerable: false,
      writable: false,
      value: source,
    });
    if (Object.freeze) {
      Object.freeze(element.props);
      Object.freeze(element);
    }
  }

  return element;
};
```
### 分析
關於 `if(__DEV__)` 中的源碼這邊不做說明，畢竟那是給人家 react 測試用的，跟我也沒多大關係hhh。

這個方法其實就是將經過了 `React.createElement()` 處理的一些東西，像是 type, key, ref, props 等包裝成一個統一對象，該對象會有一個標記 `$$typeof` 值為 `REACT_ELEMENT_TYPE`，作用就是可以確保是一個 React Element 類型，這跟渲染到實際的 DOM 上有關，這個我們後面還會再別篇源碼分析系列提到。

## 小結
根據源碼，我們就知道實際上當我們用 jsx 創建一個 react 元素時：
```html
<Foo color="#fff">hi</Foo>
```
實際上是會調用 `React.createElement()`，像下面這樣：
```js
React.createElement('Foo', {color: '#fff'}, 'hi')
```
然後 `React.createElement()` 會返回給我們一個實際的 react 元素，其實就是一個普通的 js 對象：
```js
{
  type: 'Foo',
  props: {
    color: '#fff',
    children: 'hi',
  },
  key: null,
  ref: null,
  $$typeof: Symbol.for('react.element'),
}
```
這邊對於一個點還沒釐清，就是在 `React.createElement()` 中的 return 調用了 `ReactElement()`，其中就包含了這麼一個參數 `ReactCurrentOwner.current`。這個部分等到後面解析 Fiber 的源碼時會再提到。

# 結語
本篇大致上解析了 react 到底是如何將我們寫的 jsx 轉換成 js，以及具體的 react 元素是什麼，源碼中涉及到的主要是 `React.createElement()` 還有 `ReactElement()` 這兩個方法。這篇是本菜雞第一篇關於源碼的解析，如有不全還請各位大佬多多指教啦，也希望自己可以堅持下去這個 react 源碼系列的更新與學習～


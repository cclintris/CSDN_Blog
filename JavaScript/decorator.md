@[TOC](關於 ES7 提案 Decorator)

# 前言
會寫這篇是因為這陣子正在學習 react，因此也學了一個 react 協助狀態管理的庫 MobX，而在 MobX 中就有一個東西叫做 decorators(裝飾器)，覺得這個東西還挺神奇的。後來和超悠閒大老聊天才發現原來 MobX 這個裝飾器的原理就是 es6 提供的 decorators，還是太菜了。所以趕緊來補補這塊還沒掌握的知識點。

# 正文
在正式進入 decorators 之前，我們是不能直接在 js 文件中使用的，需要借助 babel 的功能，具體是要靠一個核心插件 `babel-plugin-transform-decorators-legacy`，這個插件我們在 MobX 那篇也提過要用到 [React + MobX - 完全上手指南](https://blog.csdn.net/weixin_46803507/article/details/116938570)。下面就開始我們的準備工作吧！

## decorators 準備工作
要做這些前置工作是因為目前本地 File 協議還不支持 decorator，因為其實 decorator 是 es7 的提案。現在只支持在服務器上或是 localhost。所以下面我們就是要來準備 localhost 環境。

- 初始化項目

首先我們新建一個文件夾，並且執行 `npm init` 初始化項目，這時候文件夾下應該會出現一個 package.json 文件了。

- 安裝依賴

我們先安裝如下依賴，主要目的就是通過 webpack 以及 webpack-dev-server 的配置使我們的 js 文件經過 webpack 處理，並加入所需的 babel 依賴和插件，這樣就可以在我們的 js 文件中使用 decorators，並通過瀏覽器看到效果打印等等。

- `webpack`
- `webpack-cli`
- `webpack-dev-server` 
- `babel-core`
- `babel-loader@7 `
- `babel-plugin-transform-decorators-legacy`

```
npm install webpack webpack-cli webpack-dev-server babel-core babel-loader@7 babel-plugin-transform-decorators-legacy --save-dev
```
不過因為 webpack 的東西版本越來越多，很容易出現不一致的問題，總之上面的依賴直接想跑應該是不行的，會報一個 `Error: Cannot find module 'webpack-cli/bin/config-yargs'`，就是因為 webpack-cli 和 webpack-dev-server 版本不一致造成。

解決辦法是要降低 webpack-cli 版本：
```
npm i webpack-cli@3.3.12 -D 
```
- webpack.config.js

在項目文件下新建一個 webpack.config.js 文件，配置 webpack，代碼如下：
```js
const path = require("path");
module.exports = {
  entry: path.join(__dirname, "index.js"),
  output: {
    path: path.join(__dirname),
    filename: "bundle.js",
  },
  module: {
    rules: [
      {
        test: /\.js$/,
        loader: "babel-loader",
        exclude: path.join(__dirname, "node_modules"),
        options: {
          plugins: ["transform-decorators-legacy"],
        },
      },
    ],
  },
};
```
- index.html

新建一個 index.html 如下：
```html
<!DOCTYPE html>
<html lang="en">
  <head>
    <meta charset="UTF-8" />
    <title>js decorator</title>
  </head>
  <body>
    <script src="bundle.js"></script>
  </body>
</html>
```
- index.js

隨便寫什麼都可以，不過一定要建，因為 webpack.config.js 中用到這個 index.js 作為入口。

- 配置 scripts

在 package.json 中配置啟動命令 `start`，如下：
```json
"scripts": {
"test": "echo \"Error: no test specified\" && exit 1",
"start": "webpack-dev-server --config webpack.config.js"
},
```

到這邊應該配置安裝等工作都差不多完成了，文件結構如下：
```
/ src
    / node_modules
    index.html
    index.js
    package-lock.json
    package.json
    webpack-config.js
```
接下來我們只要在命令行的 /src 下運行 `npm run start` 就可以訪問 `http://localhost:8080/`，終於可以開始本文的主角 decorators 了。

## 類的修飾
其實 decorator 就是裝飾器，其實就是一個函數，用來修改類的行為。而裝飾器對類的修改是發生在編譯期間的，而不是在運行期間。
```js
function testable(target) {
  target.isTestable = true;
}

@testable
class MyTestableClass {}

console.log(MyTestableClass.isTestable);
// true
```
上面代碼的作用其實就是通過 `@testable` 這個修飾器修改了類 `MyTestableClass` 的行為，加上了一個靜態屬性 `isTestable`。

實際上，修飾器的行為底層是這樣：
```js
@decorator
class A() {}

// 其實就等同於

class A() {}
A = decorator(A) || A
```
所以不難理解，其實修飾器就是在編譯期間執行的函數。

修飾器的第一個參數 `target`，就是要被修改的目標類。我們也可以像下面這樣：
```js
function testable(isTestable) {
  return function(target) {
    target.isTestable = isTestable
  }
}

@testable(false)
class MyTestableClass {}

console.log(MyTestableClass.isTestable);
// false

@testable(true)
class MyClass() {}

console.log(MyClass.isTestable)
// true
```
前面例子都是為類添加靜態屬性，如果想添加實例屬性，可以通過 `prototype` 來添加：
```js
function testable(target) {
  target.isTestable = true;
  target.prototype.protoTest = [];
}

@testable
class MyTestableClass {}

console.log(MyTestableClass.isTestable);
const myTestableInstance = new MyTestableClass();
console.log(myTestableInstance)
console.log(myTestableInstance.protoTest)
```
執行如下：
![](https://riyugo.com/i/2021/05/23/r9zmf2.png)

上面代碼中，修飾器 `@testable` 在目標類的 `prototype` 對象上添加了 `protoTest` 屬性，所以可以在實例上調用。

## 方法的修飾
修飾器不僅可以修飾類，他還可以修飾類的屬性，來看看下面例子，我們有這麼一個類：
```js
class Person {
  constructor(firstname, lastname) {
    this.firstname = firstname;
    this.lastname = lastname;
  }
  @readonly
  name() {
    return `${this.firstname}${this.lastname}`;
  }
}
```
上面代碼就是用 `@readonly` 修飾器來修飾 Person 類的 name 方法。

修飾方法的時候，這時候修飾器接受 3 個參數：

- 參數 1：要修飾的目標對象  
- 參數 2：要修飾的屬性名稱
- 參數 3：該屬性的描述對象

具體的修飾器定義如下：
```js
function decorator(target, name, descriptor) {
  // descriptor 對象如下
  {
    value: specifiedFunction,
    enumerable: false,
    configurable: true,
    writable: true
  }
}
```
所以我們的 `@readonly` 修飾器可以這麼寫：
```js
function readonly(target, name, descriptor) {
  descriptor.writable = false
  return descriptor
}
```
上面代碼說明，`@readonly` 修飾器會修改屬性的描述對象，然後被修改的描述對象會再用來定義屬性。

我們看到 `descriptor` 對象中有個 enumerable 屬性，就是用來描述屬性是否可以遍歷，所以通過下面的修飾器，我們可以使屬性變的不可遍歷。
```js
function nonenumerable(target, name, descriptor) {
  descriptor.enumerable = false
  return descriptor
}
```
再來看一個例子，下面的 `@log` 修飾器可以起到類似於輸出日誌的作用。
```js
class Math {
  @log
  add(a, b) {
    return a + b;
  }
}

function log(target, name, descriptor) {
  let oldVal = descriptor.value;

  descriptor.value = function () {
    console.log(`Calling "${name}" with`, arguments);
    return oldVal.apply(null, arguments);
  };
  return descriptor;
}

const math = new Math();
math.add(2, 4);
```
說明下上面代碼吧～可以看到我們的 `log` 函數內先通握 descriptor.value 獲得了 `@log` 修飾器具體修飾的方法，並且對 descriptor.value 重新定義，其中打印了調用被修飾方法的日誌信息，最後再將 this 重新指向原本的 descriptor.value。

當然，一個方法可以有多個修飾器，而當有多個修飾器時，就會一層層由外而內進入，然後由內而外執行，就像下面例子所示：
```js
function dec(id) {
  console.log("evaluate", id);
  return function (target, name, descriptor) {
    console.log("execute", id);
  };
}

class Example {
  @dec(1)
  @dec(2)
  method() {}
}

const ex = new Example();
ex.method();

// evaluate 1
// evaluate 2
// execute 2
// execute 1
```
還有一個很有用的就是通過修飾器我們也可以對類型做檢查，非常實用：
```js
function checkNumberType(target, name, descriptor) {
  descriptor.value = function () {
    for (let param of arguments) {
      if (typeof param !== "number") {
        throw new TypeError(`Type of ${name} is not compatible!`);
      }
    }
  };
  return descriptor;
}

class Math {
  @checkNumberType
  add(a, b) {
    return a + b;
  }
}

const math = new Math();

math.add(1, '2');
// Uncaught TypeError: Type of add is not compatible!

math.add(1, 2)
// 沒有拋出錯誤
```
## 函數的修飾
你以為修飾器也能修飾函數嗎？錯！！！不可以的，修飾器只能用於修飾方法和類，而不能修飾一般函數的原因就是因為函數存在提升。
```js
var counter = 0;

var add = function () {
  counter++;
};

@add
function foo() {}
```
事實上這段代碼根本就無法通過編譯，不過還是藉這個例子解釋一下。上面代碼的意圖可能是，希望通過 `@add` 修飾器使得執行了 foo() 後 counter 變成 1，但是即使這段代碼通過編譯，counter 也仍然還是會是 0。原因就是因為函數存在狀態提升，所以其實上面的代碼實際執行的情況如下：
```js
var counter
var add

@add
function foo() {}

counter = 0
add = function() {}
```
當然你可能會說那不要用 var，用 let 是不是就可以了？其實還是不行的，因為目前的標準也還沒有全面禁止 var，所以為了避免這種情況發生，總之，修飾器就是不能用於修飾一般函數，只能用於修飾 class 和 method。

## core-decorators.js
core-decorators.js 是一個第三方庫，提供了一些常見的需求的修飾器，既然講到了 decorators 就來看看！

### @autobind
`@autobind` 修飾器可以將方法中的 this 強制綁定到原始對象上。
```js
import { autobind } from "core-decorators";

class Person {
  @autobind
  getPerson() {
    return this;
  }
}

const person = new Person()
const getPerson = person.getPerson
console.log(getPerson() === person)
```
### @readonly
`@readonly` 修飾器可以使得屬性或是方法不可寫。
```js
import { readonly } from 'core-decorators';

class Meal {
  @readonly
  entree = 'steak';
}

var dinner = new Meal();
dinner.entree = 'salmon';
// Cannot assign to read only property 'entree' of [object Object]
```

### @override
`@override` 修飾器檢查子類的方法，是否正確覆蓋了父類的同名方法，如果不正確會報錯。

```js
import { override } from 'core-decorators';

class Parent {
  speak(first, second) {}
}

class Child extends Parent {
  @override
  speak() {}
  // SyntaxError: Child#speak() does not properly override Parent#speak(first, second)
}

// or

class Child extends Parent {
  @override
  speaks() {}
  // SyntaxError: No descriptor matching Child#speaks() was found on the prototype chain.
  //
  //   Did you mean "speak"?
}
```

### @deprecate (别名@deprecated)
`@deprecate` 或 `@deprecated` 修飾器在控製臺顯示一條警告，表示該方法將廢除。
```js
import { deprecate } from 'core-decorators';

class Person {
  @deprecate
  facepalm() {}

  @deprecate('We stopped facepalming')
  facepalmHard() {}

  @deprecate('We stopped facepalming', { url: 'http://knowyourmeme.com/memes/facepalm' })
  facepalmHarder() {}
}

let person = new Person();

person.facepalm();
// DEPRECATION Person#facepalm: This function will be removed in future versions.

person.facepalmHard();
// DEPRECATION Person#facepalmHard: We stopped facepalming

person.facepalmHarder();
// DEPRECATION Person#facepalmHarder: We stopped facepalming
//
//     See http://knowyourmeme.com/memes/facepalm for more details.
//
```
 
### @suppressWarnings
`@suppressWarnings` 修飾器抑制 decorated 修飾器導致的 console.warn() 調用。例外是，異步代碼(async)發出的調用除外。

```js
import { deprecate, suppressWarnings } from 'core-decorators';

class Person {
  @deprecate
  facepalm() {}

  @suppressWarnings
  facepalmWithoutWarning() {
    this.facepalm();
  }
}

let person = new Person();

person.facepalmWithoutWarning();
// no warning is logged
```
## Mixin
我們也可以通過修飾器實現 mixin 模式。Mixin 模式就是一種對象繼承的替代方案，就是一種『混入』的思想，所以才取名為 mix-in，意思其實就是在一個對象中混入另一個對象的方法。
```js
const Foo = {
  foo() {
    console.log('foo')
  }
}

class Mixin {}

Object.assign(Mixin.prototype, Foo)

const mixin = new Mixin()
mixin.foo()
```
上面代碼就是 mixin 的一個簡單實現。首先有一個 Foo 對象，然後有一個 class Mixin，接著我們通過 `Object.assign()` 這個方法將 Foo 這個對象和 Mixin.prototype 做一個合併，這樣每一個 Mixin 實例就都會具有 foo() 這個方法。

那我們來看看修飾器怎麼做到 mixin 的實現～

```js
const Foo = {
  foo() {
    console.log('foo')
  },
  foo1() {
    console.log('foo1')
  }
}

function mixins(...obj) {
  return function(target) {
    Object.assign(target.prototype, ...obj)
  }
}

@mixins(Foo)
class MyClass {}

const myClass = new MyClass()
myClass.foo()
myClass.foo1()
// foo
// foo1
```
我們通過上面的 `@mixins` 修飾器就可以實現 mixin 模式，其實本質還是 `Object.assign()`。

## decorators 總結
最後對 decorators 的用法做一個簡單的總結表格如下：

|修飾目標|function 參數列表|描述對象結構|
|:-----:|:-----:|:-----:|
|類(class)|(target)|x|
|方法(class method)|(target, name, descriptor)|value: Function<br /> enumerable: false<br />configurable: true<br /> writable: true|
|屬性(class field)|(target, name, descriptor)|value: Function<br /> enumerable: false<br />configurable: true<br /> writable: true|

# 結語
這篇大致上介紹了關於 es7 裝飾器的提案，整體來說大概算是一個入門吧，之後如果有學習到新的用法或是等到該提案正式隨著 es7 發布都會持續再做學習更新，若本篇有誤歡迎多多指出！

# 參考
|參考|鏈接|
|:-----|:-----|
|core-decorators github|https://github.com/jayphelps/core-decorators|
|ES6 修饰器|http://caibaojian.com/es6/decorator.html|
|es6中Decorator简单使用|http://www.qiutianaimeili.com/html/page/2019/08/8318lqsszrj.html|
|关于webpack配置webpack-dev-server 报错问题|https://www.cnblogs.com/fsg6/p/14117791.html|


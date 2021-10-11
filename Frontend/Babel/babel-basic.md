@[TOC](前端編譯 : Babel 基礎入門)

# 前言

在寫這篇前一樣先來說說為什麼要寫這篇，也算是做一個紀錄吧。寫這篇有兩個主要原因，第一個就是因為自己在項目中常常發現裡面有 babel 的東西，也就是大概知道幹嘛，但是都很不明所以，所以常常對於配置的問題沒辦法獨力解決，總是要去 call help，這其實就是能力太菜的一種表現吧。第二個原因是因為在實習期間，組裡的同事之間有一次吃飯就聊到了關於前端編譯的東西，聽下來感覺對於前端來說，也是進階的一個重要部分。所幸就藉著這篇，記錄一下關於學習 babel 基礎知識的過程吧。

話不多說，進入正文吧～

# 正文

這篇會涵蓋關於 babel 的基礎知識，個人認為應該還是算是比較完整的一份學習指南～(對於像我一樣的菜雞)

## 什麼是 Babel ?

Babel 簡單來說，就是一個編譯器，使用 babel 可以使用(或者說創建)下一代的 JavaScript，以及下一代 JavaScript 工具。

作為一門語言，js 的不斷發展帶來了很多新的規範，而使用 babel 可以讓你在這些新的規範普及之前就可以提前使用它們。

Babel 通過將最新標準的 js 代碼編譯成已經目前可以工作的代碼來實現這樣的功能，這個過程就被稱為 `源代碼到源代碼` 的編譯，也被稱為 `轉換`。

大家用那麼多箭頭函數，那就以箭頭函數來舉例。例如說，babel 可以將最新的 ES2015 箭頭函數語法從：

```js
const square = (n) => n * n;
```

轉換成下面內容：

```js
const square = function sqaure(n) {
  return n * n;
};
```

上述只是一個例子而已，babel 可以做更多事情。因為 Babel 支持語法擴展，所以像是 React 的 jsx 語法或是靜態類型檢查的 flow 語法， babel 也是能做『轉換』的工作的。

更進一步來說，在 Babel 的概念中，一切都是插件，而每個人可以充分利用 babel 強大的能力來創造屬於自己的插件，且 babel 被組織成幾個核心模塊，允許用戶利用這些模塊來構建自己的 js 工具。

下面就來說說 babel 內建的一些工具以及這個生態中一些有用重要的工具。

## Babel 模塊介紹

因為 JavaScript 社區沒有標準的構建工具，框架或平臺等，Babel 官方性與其他所有的主要工具進行了集成。無論是來自 Gulp、Browserify，或是 Webpack 等，無論打包工具是什麽，Babel 都存在一些官方性的集成。

> 這篇將參考諸如 `node` 和 `npm` 之類的命令行工具。在繼續進行任何操作之前，最好先熟悉一下這兩個工具。

### babel-cli

Babel 的 CLI 是從命令行使用 babel 編譯文件最簡單基本的方法。

首先我們先在全局安裝它：

```js
// 如果安裝失敗可以試試把 npm 源換成國內淘寶

npm config set registry https://registry.npm.taobao.org/
npm config get registry

// 安裝 babel-cli
npm install --global babel-cli
```

如果出現下面這樣的報錯：
![](https://s3.bmp.ovh/imgs/2021/10/e5d9d9618184c3c5.png)

那麼是因為沒有管理員權限，加上 `sudo` 就可以了：

```js
sudo npm install --global babel-cli
```

最後在命令行輸入 `babel -V` 確認是否安裝成功：

![](https://s3.bmp.ovh/imgs/2021/10/c41174fac54618a1.png)

接著我們就來試試如何使用 babel ～

我們可以像這樣編譯我們的 js 文件：

```js
babel babel-test.js
```

這會將編譯後的輸出直接轉儲到終端，如果想要寫入文件，可以這樣：

```js
// --out-file 參數
babel babel-test.js --out-file compiled.js

// -o 參數
babel babel-test.js -o compiled.js
```

如果想將整個目錄重新編譯成一個新目錄，可以使用 `--out-dir` 或是 `-d` 來完成。

```js
$ babel src --out-dir lib
// or
$ babel src -d lib
```

### 從項目中運行 Babel CLI

雖然您可以在電腦上全局安裝 Babel CLI，但最好逐個項目在本地安裝它。

這有兩個主要原因：

1. 同一台電腦上的不同項目可能取決於 Babel 的不同版本，從而允許你一次更新一個版本。

2. 這意味著你對工作的環境沒有隱式依賴，使您的項目更加可移植且易於配置升級等等。

我們可以通過運行以下命令在本地安裝 Babel CLI：

```js
npm install --save-dev babel-cli
```

這邊不會說得太多，因為不是本文重點。總之，就是先準備一個 npm 項目，然後通過上述命令安裝好 babel-cli，接著可以在 `package.json` 中添加一個 `script` 字段並配置用 babel 構建的命令，然後 npm 啟動即可。

`package.json` 可能如下：

```json
  {
    "name": "my-project",
    "version": "1.0.0",
+   "scripts": {
+     "build": "babel src -d lib"
+   },
    "devDependencies": {
      "babel-cli": "^6.0.0"
    }
  }
```

然後 `npm run build` 即可。關於 npm 項目的構建等等這邊暫不攢述，網路上也有很多相關資料。

### babel-register

運行 babel 的下一個最常見的方法就是通過 `babel-register`。通過這個方式，只需要文件即可運行 babel，非常輕量方便。

但是要注意的一點就是，`babel-register` 無法提供生產使用。部署以這種方式編譯的的代碼通常被認為是不好的作法。不過鑒於我也是 babel 的初學者，就當作是了解一下也沒什麼不好啦～

首先我們先在項目中創建一個 `index.js` 文件。

```js
console.log("Hello world!");
```

這時候如果我們就單純使用 `node index.js` 來運行該文件，那 babel 不會編譯他，因此我們需要通過某種方式將 babel 注入。首先，設置 `babel-register`。

```js
npm install --save-dev babel-register
```

接下來，我們要在項目中創建一個 `register.js`，代碼如下：

```js
require("babel-register");
require("./index");
```

這麼做就是在 node 的模塊系統中註冊 `babel` 並開始編譯每個 `require` 的文件。

現在執行 `node register.js`，實際上的效果就是 `index.js` 會先通過 babel 編譯，然後才會具體被執行。

這邊要注意的一點是，你不可以在要被編譯的文件中註冊 babel，否則你實際希望通過 babel 編譯再執行的代碼不會被編譯。

```js
require("babel-register");

// not compiled!
console.log("Hello world!");
```

### babel-node

接下來介紹一種使用 babel 比較常見的方式，`babel-node`。最近和一個同學的合作項目中因為開發到前端的 SDK，打包過程和發包過程就用到了這個 `babel-node`，所以就剛好也來學習一下。

首先請確保已經安裝了 `babel-cli`。然後其實也就是把所有執行 `node` 的地方都換成 `babel-node` 就好了。

```json
  {
    "scripts": {
-     "script-name": "node script.js"
+     "script-name": "babel-node script.js"
    }
  }
```

### babel-core

接下來介紹的這個 `babel-core`，它的用途在於如果出於某種原因需要在代碼中使用 babel，那就是 `babel-core` 的用武之地。

安裝 `babel-core`：

```js
npm install --save-dev babel-core
```

試試下面代碼看看輸出什麼吧～

```js
const babel = require("babel-core");

const res = babel.transform("console.log('hello world');");
console.log(res);

// {
//   metadata: {
//     usedHelpers: [],
//     marked: [],
//     modules: { imports: [], exports: [Object] }
//   },
//   options: {
//     filename: 'unknown',
//     filenameRelative: 'unknown',
//     inputSourceMap: undefined,
//     env: {},
//     mode: undefined,
//     retainLines: false,
//     highlightCode: true,
//     suppressDeprecationMessages: false,
//     presets: [],
//     plugins: [],
//     ignore: [],
//     only: undefined,
//     code: true,
//     metadata: true,
//     ast: true,
//     extends: undefined,
//     comments: true,
//     shouldPrintComment: undefined,
//     wrapPluginVisitorMethod: undefined,
//     compact: 'auto',
//     minified: false,
//     sourceMap: undefined,
//     sourceMaps: undefined,
//     sourceMapTarget: 'unknown',
//     sourceFileName: 'unknown',
//     sourceRoot: undefined,
//     babelrc: true,
//     sourceType: 'module',
//     auxiliaryCommentBefore: undefined,
//     auxiliaryCommentAfter: undefined,
//     resolveModuleSource: undefined,
//     getModuleId: undefined,
//     moduleRoot: undefined,
//     moduleIds: false,
//     moduleId: undefined,
//     passPerPreset: false,
//     parserOpts: false,
//     generatorOpts: false,
//     basename: 'unknown'
//   },
//   ignored: false,
//   code: "console.log('hello world');",
//   ast: Node {
//     type: 'File',
//     start: 0,
//     end: 27,
//     loc: SourceLocation { start: [Position], end: [Position] },
//     program: Node {
//       type: 'Program',
//       start: 0,
//       end: 27,
//       loc: [SourceLocation],
//       sourceType: 'module',
//       body: [Array],
//       directives: []
//     },
//     comments: [],
//     tokens: [
//       [Token], [Token],
//       [Token], [Token],
//       [Token], [Token],
//       [Token], [Token]
//     ]
//   },
//   map: null
// }
```

可以看到，有三個比較重要的屬性 `code`、`map`、`ast`，可以猜到可能跟什麼語法樹有關係吧。

沒錯，其實 babel 就是一個概念或是說一個大匯總，babel 就是一個編譯器，一個 source to source 的編譯器，通常會被稱為 transpiler(不是 compiler 喔)。而具體怎麼編譯怎麼解析你提供給 babel 的源代碼，其實就是這個 `babel-core` 的核心功能。

仔細一點說，其實 `babel-core` 被分為三個模塊：

- `@babel/parser`：接受源碼，進行語法分析和詞法分析，生成 AST

- `@babel/traverse`：接受 AST，對其遍歷，根據 `preset`、`plugin`進行邏輯處理，進行替換、刪除、添加節點。

- `@babel/generator`：接受最終生成的 AST，並將其轉換為代碼字符串。

![](https://s3.bmp.ovh/imgs/2021/10/d8660a6d5ed28789.jpeg)

## 配置 Babel

上面介紹了關於 babel 基本的概念和基本使用 babel 的東西，但是會發現僅僅運行 babel 除了將 js 文件簡單編譯後，然後可能從一個位置複製到另一個位置之外沒有執行任何其他操作。

這是因為我們沒有告訴 babel 該做什麼事情。接下來就還說說怎麼告訴 babel 我們希望他做什麼，主要就是通過安裝 `plugins` 或 `presets`(plugin 組) 為 babel 提供操作說明。

### .babelrc

在我們開始告訴 babel 做什麼起前，首先我們需要創建一個 babel 配置文件，從這樣開始：

```js
{
    "presets": [],
    "plugins": []
}
```

該文件是配置 babel 以執行所需操作的方式。當然還有其他方式可以配置 babel，但這個方法約定俗成大家都比較推薦常用的方法。

### babel-preset-es2015

首先要做的，告訴 babel 要將 ES2016，也就是 ES6 編譯為 ES5，也就是當今大多數 js 環境中可用的版本。

我們通過安裝 `es2015` Babel 預設來做到這一點，當然現在瀏覽器環境也支持了大多數的 ES6 特性，不過還是安全起見。

```js
npm install --save-dev babel-preset-es2015
```

然後將預設添加到 `.babelrc` 文件中：

```json
  {
    "presets": [
+     "es2015"
    ],
    "plugins": []
  }
```

### babel-preset-react

設置 react 一樣也就是裝一下 react 的 babel 預設，然後設置一下就好：

```js
npm install --save-dev babel-preset-react
```

```json
  {
    "presets": [
      "es2015",
+     "react"
    ],
    "plugins": []
  }
```

## 執行 Babel 生成的代碼

目前我們已經可以使用 babel 編譯代碼，也能簡單通過配置 babel 使他做一些事情，這邊就來說說要怎麼執行 babel 生成的代碼。

### babel-polyfill

Babel 默認只轉換新的 JavaScript 語法，而不轉換新的 API。 例如，`Iterator`、`Generator`、`Set`、`Maps`、`Proxy`、`Reflect`、`Symbol`、`Promise` 等全局對象，以及一些定義在全局對象上的方法（比如 `Object.assign`）都不會轉譯。 如果想使用這些新的對象和方法，則需要為當前環境提供一個 `polyfill`。

下面代碼有需要編譯的 ES6 箭頭函數語法：

```js
function addAll() {
  return Array.from(arguments).reduce((a, b) => a + b);
}
```

經過 babel 編譯後會變成這樣：

```js
"use strict";

function addAll() {
  return Array.from(arguments).reduce(function (a, b) {
    return a + b;
  });
}
```

我們看到箭頭函數的部分確實被 babel 編譯成了 ES5 的語法，但是注意到 `Array.from` 並非在每個 js 環境中都存在，因此也可能經過這樣編譯的代碼在特定環境下還是不能正常運行，大概有可能會報這樣的錯：

```js
Uncaught TypeError: Array.from is not a function
```

為了解決這個問題，我們要使用一個叫做 `polyfill` 的東西。簡單來說，`polyfill` 是一段代碼，該代碼會複製當前運行時不存在的 API，允許你在當前環境使用之前能提前使用的 `Array.from` 等 API。

要使用 babel polyfill 也很簡單，只要安裝一下相應的 npm 包：

```js
npm install --save babel-polyfill
```

然後只需要將 polyfill 包含在任何需要他的文件頂部就行了：

```js
import "babel-polyfill";

// ...
```

來看看一個例子，感受一下 babel-polyfill 幫我們做了什麼：

下面很簡單的代碼：

```js
import "babel-polyfill";

const key = "babel";

const obj = {
  [key]: "polyfill",
};
```

經過 babel-polyfill 編譯後會變成這樣：

```js
"use strict";

require("babel-polyfill");

function _defineProperty(obj, key, value) {
  if (key in obj) {
    Object.defineProperty(obj, key, {
      value: value,
      enumerable: true,
      configurable: true,
      writable: true,
    });
  } else {
    obj[key] = value;
  }
  return obj;
}

var key = "babel";

var obj = _defineProperty({}, key, "polyfill");
```

可以看到，babel-polyfill 幫我們加入了很多 helper 函數，確保我們的代碼可以在任何環境下正常運行，而不會發生沒有被編譯好的失誤。

### babel-runtime

`babel-polyfill` 解決了 Babel 不轉換新 API 的問題，但是直接在代碼中插入幫助函數，會導致汙染了全局環境，並且不同的代碼文件中包含重復的代碼，導致編譯後的代碼體積變大。（比如：上述的幫助函數 `_defineProperty` 有可能在很多的代碼模塊文件中都會被插入），這樣明顯是不太好的。

所幸 babel 也提供了一個叫 `babel-runtime` 的東西，提供了單獨的包`babel-runtime` 用以提供編譯模塊的工具函數。想使用 `babel-runtime` 首先先安裝相關依賴：

```js
npm install --save-dev babel-plugin-transform-runtime
npm install --save babel-runtime
```

然後更新 `.babelrc`：

```json

  {
    "plugins": [
+     "transform-runtime",
    ]
  }
```

來看看使用 `babel-runtime` 編譯同樣剛剛的代碼會變成什麼樣～

```js
"use strict";

var _defineProperty2 = __webpack_require__(
  "./node_modules/babel-runtime/helpers/defineProperty.js"
);

var _defineProperty3 = _interopRequireDefault(_defineProperty2);

var _assign = __webpack_require__(
  "./node_modules/babel-runtime/core-js/object/assign.js"
);

var _assign2 = _interopRequireDefault(_assign);

function _interopRequireDefault(obj) {
  return obj && obj.__esModule ? obj : { default: obj };
}

var key = "babel";
var obj = (0, _defineProperty3.default)(
  {},
  key,
  (0, _assign2.default)({}, { key: "polyfill" })
);
```

可以看到上述轉換後的代碼中 `_defineProperty` 幫助函數是通過`babel-runtime` 下的模塊引用的， 同時 `Object.assign` 也變成了模塊引用, 這樣可以避免自行引入 polyfill 時導致的汙染全局命名空間的問題，而且代碼體積確實也小了不少。

關於 babel-runtime 和 babel-polyfill 這部分其實本人自己也還不是特別熟悉，畢竟也沒有真的在實際項目中自己應用到，只能說是很基本粗淺的理解，日後如果有機會實際應用再更新。

## 配置 Babel (進階)

到這邊為止，大部分介紹的都是通過 babel `presets` 進行預設配置，告訴 babel 我們希望它幫我們幹什能。但是我們可以看到 `.babelrc` 裡面還有另一個配置項 `plugins`，這邊就來說說 `presets` 跟 `plugins` 有什麼區別吧！

### presets vs plugins

其實這個區分也很好懂，babel `presets` 相當於是預設配置插件的一個集合，而如果想做更特定專一的事情，那我們就可以通過手動指定 `plugins` 來實現。

Babel 插件一般盡可能拆成小的力度，開發者可以按需引進。比如對 ES6 轉 ES5 的功能，Babel 官方拆成了 20+ 個插件。

這樣的好處顯而易見，既提高了性能，也提高了擴展性。比如開發者想要體驗 ES6 的箭頭函數特性，那他只需要引入 `transform-es2015-arrow-functions` 插件就可以，而不是加載 ES6 全家桶。

但很多時候，逐個插件引入的效率比較低下。比如在項目開發中，開發者想要將所有 ES6 的代碼轉成 ES5，插件逐個引入的方式令人抓狂，不單費力，而且容易出錯。

這個時候，可以采用 `presets` 的方式進行引入。

可以簡單的把 Babel `Preset` 視為 Babel `Plugin` 的集合。比如`babel-preset-es2015` 就包含了所有跟 ES6 轉換有關的插件。

舉個例子來說，對於下面這段代碼，用到了 ES6 的 class，所以在某些只允許 ES5 的環境下就會報錯。

```js
class Test {
  constructor(name) {
    this.name = name;
  }

  logger() {
    console.log("Hello", this.name);
  }
}
```

所以我們就希望先通過 babel 幫我們編譯好。當然這邊我們可以直接引入整個 `babel-preset-es2015` 這樣 babel 就會幫我們把所有 ES6 的語法轉換成 ES5 的語法，但是單單就這個例子來說，引入整個 `preset` 就太多餘了，所以可以引入特定的 `plugin`，如下：

```js
npm install --save-dev babel-plugin-transform-es2015-classes
```

修改 `.babelrc`:

```json
  {
+   "plugins": [
+     "transform-es2015-classes"
+   ]
  }
```

然後看看編譯效果：

```js
var _createClass = (function () {
  function defineProperties(target, props) {
    for (var i = 0; i < props.length; i++) {
      var descriptor = props[i];
      descriptor.enumerable = descriptor.enumerable || false;
      descriptor.configurable = true;
      if ("value" in descriptor) descriptor.writable = true;
      Object.defineProperty(target, descriptor.key, descriptor);
    }
  }
  return function (Constructor, protoProps, staticProps) {
    if (protoProps) defineProperties(Constructor.prototype, protoProps);
    if (staticProps) defineProperties(Constructor, staticProps);
    return Constructor;
  };
})();

function _classCallCheck(instance, Constructor) {
  if (!(instance instanceof Constructor)) {
    throw new TypeError("Cannot call a class as a function");
  }
}

let Test = (function () {
  function Test(name) {
    _classCallCheck(this, Test);

    this.name = name;
  }

  _createClass(Test, [
    {
      key: "logger",
      value: function logger() {
        console.log("Hello", this.name);
      },
    },
  ]);

  return Test;
})();
```

可以看到，僅僅通過特定的 `plugin` 我們也能成功操作 babel。關於 `plugins` 可以去官方看，有一大堆支持 babel 的插件。

### plugins 選項

許多插件還具有將其配置為不同行為的 `option` 。例如，許多 `transform plugins` 都具有 `loose` 模式，該模式會放棄某些規範行為，而傾向於使用更簡單，性能更高的代碼。

如果想添加一些選項，直接修改 `.babelrc` 就好：

```json
  {
    "plugins": [
-     "transform-es2015-classes"
+     ["transform-es2015-classes", { "loose": true }]
    ]
  }
```

每個插件支持的選項可能也都大不相同，具體都根據自己使用的插件進行配置就好了。

# 結語

這篇寫的個人認為比較簡單，內容也不太多，就只是 babel 的入門指南而已，都是非常基本的概念和知識點而已。以後對於 babel 還有學習到更多的東西都會再記錄下來分享。

# 參考資料

| 參考                                             | 鏈接                                                       |
| :----------------------------------------------- | :--------------------------------------------------------- |
| Babel 官方文檔                                   | https://www.babeljs.cn/                                    |
| babel,babel-core 是什么关系？分不清他们的职责？  | https://www.zhihu.com/question/277409645/answer/1710872706 |
| babel-runtime 和 babel-polyfill 的作用介绍和使用 | https://www.jianshu.com/p/73ba084795ce                     |
| Babel：plugin、preset 的区别与使用               | https://www.cnblogs.com/dapengFly/p/9876915.html           |

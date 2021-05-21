@[TOC](關於 JavaScript 的 Prototype)

# 前言
在 ES6 之前，js 並不像 C++, Java， 這些語言都有所謂的 class(類) 以及 instance(實例)的概念，但是 js 並不具有這樣表達面向對象的能力。而 js 的世界中，可以說是萬物都是函數，因此，我們就來談談到底如何使用 js 實現面向對象，從而引出本篇的主角，js 的 Prototype。

# 正文

## 為甚麼要有 prototype?
首先當然先要有一個「模子」，然後用 `new` 關鍵字來創建其實例。使用的就是 js 中的 function。看看如下例子:
```js
function Book(name, pages) {
    this.name = name;
    this.pages = pages;
    this.comment = null;

    this.set_comment =  function(comment) {
        this.comment = comment;
    }
}

let Thinking_In_Java = new Book("Thinking in Java", 600);
let Cpp_Primer = new Book("Cpp Primer", 1000);

Thinking_In_Java.set_comment("Java Bible!");
Cpp_Primer.set_comment("C++ Bible!");

console.log(Thinking_In_Java.comment);
console.log(Cpp_Primer.comment);

console.log(Thinking_In_Java.set_comment === Cpp_Primer.set_comment);
```
輸出如下:
![](https://wtfhhh.oss-cn-beijing.aliyuncs.com/proto-1.png)

我們看到，最後一個 console 輸出竟是 false。這表示在 js 中，如果只是在一開始的「類」模板中單純通過函數添加方法，對於兩個 new 出來的實例，各自的方法會是不同的函數。那設想一下，如果今天有 n 個實例，那豈不是要維護 n 個方法，如果類模板中不只一個方法，那麼就多出了好多明明就在做一樣的事情，但卻要各自維護的方法。這顯然不是我們希望的，因此出現了 `prototype`。

## prototype 是甚麼?
`prototype` 就是為了解決上述的問題，將共用的屬性或是方法，不用每次建立實例都建立一份，而是提出來放到公共的 prototype 即可。承上例子，我們就可以將 `set_comment` 這個共用的方法放到 `Book.prototype`，稱為 Book 的原型。
```js
function Book(name, pages) {
    this.name = name;
    this.pages = pages;
    this.comment = null;
}

Book.prototype.set_comment = function(comment) {
    this.comment = comment;
}

let Thinking_In_Java = new Book("Thinking in Java", 600);
let Cpp_Primer = new Book("Cpp Primer", 1000);

Thinking_In_Java.set_comment("Java Bible!");
Cpp_Primer.set_comment("C++ Bible!");

console.log(Thinking_In_Java.comment);
console.log(Cpp_Primer.comment);

console.log(Thinking_In_Java.set_comment === Cpp_Primer.set_comment);
```
輸出如下:
![](https://wtfhhh.oss-cn-beijing.aliyuncs.com/proto-2.png)
如上，通過 `prototype` 添加的方法就是同一個函數了!

## 請勿修改原生的原型
在這裡都是在自己建立的類實例上設定原型，千萬不要嘗試修改原生原型(例如 String.prototype)。

## 關於建構子
這邊補充一下，也為了接下去的內容做一些鋪墊。
關於建構子...
- 在 js 中，除了沒有類之外，其實也沒有實例的概念，因此統一下說法:
1. 只要函式前有 `new`，這個函式就是建構子
2. 只要函式前有 `new` 來做呼叫，就叫做建構子呼叫

- `new` 到底做了甚麼?它的工作如下:
1. 建立一個新的實例
2. 將該實例的 `__proto__` 指向建構子的 `prototype`，形成原型串鏈
3. 將建構子的 this 指向 new 出來的新實例
4. 返回這個實例

這邊提到了原型串鏈的概念，就是接下來要介紹的內容。

## 原型串鏈(Prototype Chain)
對於編程有一點基礎的人都知道，若在目前執行的範疇找不到這個變數的時候，就往外層的範疇搜尋，持續搜尋直到找到為止，或直到最外層的全域範疇。

同理，當查找實例的屬性或方法時，若在本身這個實例上找不到，就會往更上一層尋找，直到串鏈的尾端，也就是 `Object.prototype`，若還是找不到，就返回 undefined。而這整個尋找過程就是依循 `.__proto__` 來找。

每個對象在建立之初都會有一個 `__proto__` 內部屬性，它可用來存取另一個相連的對象的內部屬性 Prototype 的值，而 Prototype 則存放其建構子原型的位置。

看文字其實蠻頭疼的，來看看下面例子，很清楚:
```js
function Book(name, pages) {
    this.name = name;
    this.pages = pages;
    this.comment = null;
}

Book.prototype.set_comment = function(comment) {
    this.comment = comment;
}

let book = new Book("Book", 100);
book.set_comment("good book");

console.log(book.__proto__ === Book.prototype);
console.log(Book.__proto__);
console.log(Book.prototype);
```
輸出如下:
![](https://wtfhhh.oss-cn-beijing.aliyuncs.com/proto-3.png)

進一步分析一下:
- 原型鏈結構如下
```js
book {
    name: "Book"
    pages: "100"
    comment: "good book"
    __proto__ → Book.prototype { 
                    set_comment: <function>
                    __proto__ → Object.prototype
                    constructor → Book {
                                    __proto__ → Function.prototype
                                    prototype → Book.prototype
                                  }
                } 
}
```
由於 book 找不到 `set_comment` 方法，因此就循著 `__proto__` 屬性找到 Book.prototype 而找到 `set_comment`。這就是 js 原型鏈的概念。也因為有了原型鏈，可以使 js 體現出類似於面向對象語言的繼承的概念。

### Q1 到底是誰的屬性?
如果不確定的時候，我們可以用 `hasOwnProperty` 來檢查屬性是在當前對象中，還是在原型鏈中。
```js
console.log(book.hasOwnProperty("pages")); // true
console.log(book.hasOwnProperty("set_comment")); // false
```
確實，`pages` 屬性是屬於 book 的，而 `set_comment` 在原型鏈中，不在 book 中。

### Q2 到底是誰的實例?
我們可以用 `instanceof` 確定實例到底是由哪個對象創造的。位於 `instanceof` 左邊的運算元是實例，右邊的是函式，若左邊的實例是由右邊函式所產生的，則會返回 true，否則為 false。`instanceof` 可檢查整條原型鏈的繼承關係。
```js
console.log(book instanceof Book); // true
console.log(book instanceof Object); // true
console.log(book instanceof Function); // true
```

根據上面的原型鏈結構，就可以知道，book 由 Book 產生，而 Book 又由 Object 和 Function 派生，因此輸出都是 true。

### Q3 原型鏈的終點?
```js
console.log(book.__proto__ === Book.prototype); // true
console.log(Book.__proto__ === Function.prototype); // true
console.log(Book.prototype.__proto__ === Object.prototype); // true
console.log(Object.prototype.__proto__); // null
```
於是可以知道，所有原型鏈的終點就是 `Object.prototype`。

# 結語
本篇介紹了關於 JavaScript 原型的概念，對於原型以及原型鏈進行了解釋，也說明了如何用 js 體現出面向對象。希望對不懂原型的人看完多少有點幫助，若有錯誤，還請高人指點啦!

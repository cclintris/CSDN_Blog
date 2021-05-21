@[TOC](ES6 JavaScript 箭頭函數以及對 this 的影響)

# 前言
箭頭函數(Arrow function)可以說是 ES6 一個很重要的東西，所以覺得來整理一下加強這邊的印象還是挺有必要的。對於那些大神們見笑啦，但對於像我一樣的小白，一起看看吧!

# 正文

## 甚麼是箭頭函數?
首先先來看以往的函數我們會怎麼寫:

```js
function sayHI() {
    console.log('hello')
}

sayHI()
```
但是出現箭頭函數之後，我們會這麼寫:

```js
let sayHI = () => {
    console.log('hello')
}

sayHI()
```
> 箭頭函數中，`()`是寫參數，而 `{}` 裡面就是函數體。

再讓我們看一個例子:

```js
let sayHI = () => 'hello'
console.log(sayHI())

let sayHI = function() {
    return 'hello'
}

// 這兩種寫法是等價的
```
上面例子告訴我們，如果函數只要 return 一個值，用箭頭函數寫，可以省略 return。是不是更加簡潔呢!

最後整理一下箭頭函數的寫法:
> 如果參數為空或者有 2 個以上(含2個)，那前面的 `() ` 是必要的。若只有一個參數，() 也是可以省略的喔!

## 箭頭函數跟一般函數對 this 的區別
使用箭頭函數跟一般函數最大的區別就在於 this。以下這句話最為重要，但也有點抽象: " ==在箭頭函數中，this 指稱的對象在函數定義時就固定了，而不會隨著使用時的脈絡而改變。== " 下面來看看一些例子來體會這句話的涵義。

### 例子1
先回顧一下 JavaScript 中的 this，且看下面的代碼:
```js
let fn_ordinary = function() {
    console.log(this)
}

let fn_arrow = () => {
    console.log(this)
}

fn_ordinary()
fn_arrow()
```
結果顯示如下:
![](https://wtfhhh.oss-cn-beijing.aliyuncs.com/arrow-1.png)

如果對於為甚麼輸出長這樣的人，可以去參考另一篇 [淺談 JavaScript 中的 this](https://blog.csdn.net/weixin_46803507/article/details/106594774)。但這個例子好像比較體現不出來箭頭函數跟一般函數的區別，但是換個例子就不一樣了。接著看下去~~

### 例子2
我們用箭頭函數跟一般函數建立兩個函數如下:
```js
let fn_ordinary = function() {
    console.log(this.constructor.name)
    setTimeout(function() {
        console.log('ordinary setTimeout')
        console.log(this.constructor.name)
    }, 1000)
}

let fn_arrow = function() {
    console.log(this.constructor.name)
    setTimeout(() => {
        console.log('arrow setTimeout')
        console.log(this.constructor.name)
    }, 1000)
}

let data = {
    name: 'I am JavaScript Object.'
}

fn_ordinary.call(data)
fn_arrow.call(data)
```
結果顯示如下:
![](https://wtfhhh.oss-cn-beijing.aliyuncs.com/arrow-3.png)

先說說上面代碼一些可能看不懂的地方:
> this.constructor.name : 這樣寫其實只是為了回傳時不想要打印出整個物件的所有內容，只回傳該物件的名稱而已。

> call : 在 call 當中，我們在後面帶入的東西會變成所指稱的 this 對象。

> setTimeout : 這邊最重要的就是 setTimeout 這個函數。要先知道，setTimeout 是一個異步方法，也就是說，這個函數並不是立即執行的，而是等到整個 JS execution context 都被執行完後才會執行。因此，建立 setTimeout 這個函數，和實際執行 setTimeout 這個函數的時機是不一樣的，所以說，這兩個時機的 this 所指稱到的對象會是不一樣的。

有了上面的知識，我們來解析一下輸出結果為什麼是這樣?首先因為一開始調用了 call 方法，所以對於 fn_ordinary 和 fn_arrow 一開始的 this 都是指向物件本身，所以 console.log(this.constructor.name) 輸出都是 `Object`。接下來進入我們的重點! 如果 setTimeout 的回調函數的寫法是一般函數的話，因為它執行的時間點是在整個 JS execution context 執行完才執行，因此這時的 this 所指稱到的物件變成了全局的 Window Object，所以 console.log(this.constructor.name) 就輸出了 Window。但是，如果寫法是箭頭函數的話，就如同前面所說，this 指稱的對象在函數定義時就固定了，因此 this 所指稱到的依舊是物件本身，那麼 console.log(this.constructor.name) 當然就是輸出 Object 啦!

最後再看一個例子加深下印象。

### 例子3
首先我們先在 html 中建立一個 button，然後 JS 去抓取這個按鈕。先看下代碼吧!

```js
let btn = document.querySelector('button')

let fn_ordinary = function() {
    console.log(this.constructor.name)
}

btn.addEventListener('click', fn_ordinary)
```

```js
let btn = document.querySelector('button')

let fn_arrow = () => {
    console.log(this.constructor.name)
}

btn.addEventListener('click', fn_arrow)
```
注意到這兩部分的代碼只差在最後一行，addEventListener 分別註冊了不同的函數。

結果輸出分別如下:
![](https://wtfhhh.oss-cn-beijing.aliyuncs.com/arrow-4.png)
![](https://wtfhhh.oss-cn-beijing.aliyuncs.com/arrow-5.png)

驗證下輸出結果。因為 addEventListener 也會在 JavaScript execution context 執行結束後，網頁被觸發事件時才執行。所以一開始建立函數時，this 都是指向 Window Object，但當 JavaScript execution context 執行結束後，按鈕被觸發時，出現了區別! 這時，如果是用一般函數的寫法，那麼 this 又會被更改，指向 button，但如果是箭頭函數的話，this 依然指向最初定義時的 Window Object，所以輸出才會是那麼一回事!

# 結語
對於箭頭函數，可以說是 ES6 很重要的一個東西，為此寫了這篇博客，主要還是希望加強自己對於 JavaScript 的一些觀念，寫的應該也還算詳細。對於那些也對箭頭函數不明所以的人們，希望這篇文章有起到一些作用，也歡迎大神們給小的些許建議和指導啦!

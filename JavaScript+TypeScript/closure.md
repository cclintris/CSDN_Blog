@[TOC](談談 JavaScript 中的閉包)

# 前言
其實寫這篇關於 js 的閉包不是沒有原因的。原因就是本人之前想說就好玩去投了字節跳動前端開發的實習崗位，然後在面試時被問到 "你知道什麼是閉包嗎?"，"你能說說 js 的閉包嗎?"。當下實在覺得難受，只能說不知道，看來還是太菜了。所以馬上就趕緊來補補知識的盲區。

# 正文

## 先從作用域 (scope) 談起
要理解閉包，首先還是先從作用域 (scope) 談起吧。如果對於 js 的作用域已經了解的人，也可以選擇直接跳過這部分。

對於作用域，我認為最貼切的全是如下:
> 作用域就是一個變數的生存範圍，一旦超出了這個範圍，就無法存取到這個變數，就相當於死掉了

看看下面這個例子:

```js
function func() {
    var a = 10
}

console.log(a) // Uncaught ReferenceError: a is not defined
```
上面這個例子非常基本，但是下面這個例子呢?

```js
var a = 10
function func1() {
    console.log(a)
}

function func2() {
    var a = 20
    func1()
}

func2()
```
你覺得這段程序會輸出甚麼呢?你可能想說是 10，但是看看又發現在 func2 中再次聲明 a = 20，所以好像也可以 20。到底是哪一個呢?

回答 10 的恭喜答對了!為甚麼呢?這邊再介紹一個概念，靜態作用域 (static scope) 和動態作用域 (dynamic scope)。

### 靜態作用域 (static scope)
靜態作用域是說，作用域的判定是在函數聲明時就確定下來了，而且不會隨著運行改變。也就是說，你在哪裡呼叫任一函數是沒關係的，始終都會表現一樣的效果。JavaScript 就是這樣的語言。

### 動態作用域 (dynamic scope)
動態作用域則是說，函數的作用域是隨著運行時才確定下來的。

有了靜態作用域和動態作用域的概念，就明白了。對 func1 來說，他的 a 就是 10，而 func2 裡的 a 根本跟 func1 沒關係，因為 func1 在宣告時就確定 a 是 10 了。而如果今天我們用的是動態作用域的語言，則會輸出 20。

有了基本的作用域概念後，接下來進入本文的重點~

## 閉包 (closure)

### 閉包是什麼?
直接先來看看一個例子:

```js
function outer() {
    let a = 10
    function inner() {
        console.log(a) // 10
    }
    inner()
}

outer()
```
沒什麼特別的，就是這樣，相信也不會有人有問題。但是今天回傳的是這個函數呢?像下面這樣:

```js
function outer() {
    let a = 10
    function inner() {
        console.log(a) // 10
    }
    return inner
}

let func = outer()
func()
```
有發現甚麼神奇的事情發生了嗎?神奇在哪裡?在於一個函數執行完照理來說，應該也要釋放掉所有資源，包括所有變數 ...。所以理論上，當 outer 執行完應該也要釋放掉裡面的 a，所以外部應該是存取不到這個存取不到這個 a 了。但是在 outer 執行完後再呼叫 inner，竟然輸出了 10，表示這個 a 還活著。

這就是閉包的作用。簡單來說，閉包就是在 function 裡面回傳一個 function 的概念。而閉包所造成的效果就像上面例子這樣。outer 中的 a 就像被關在 inner 裡面，也就說只要 inner 存在，這個 a 就會一直被關在 inner 裡面。這也是為什麼會輸出 10 的原因啦!

### 閉包有甚麼用?
閉包的好處在於我可以隱藏某些變數，讓外界訪問不到，而只能透過我所提供的接口去修改變數。這麼說有點抽象，讓我們看看下面的例子。

假設今天我有個餘額的變量，且提供一個方法來減少餘額，但一個限制是我一次只能扣至多 10 元。先來看看如果不用閉包寫:

```js
let balance = 900
function deduce(money) {
    balance -= (money > 10 ? 10 : money)
}

deduce(8)
console.log(balance) // 892
balance -= 100
console.log(balance) // 792
```
我們發現，這樣我們 deduce 接口形同虛設，因為 balance 因為是全局變量，所以還是能被訪問，導致扣除 100 的操作仍然可以執行。但這並不是我們所希望的。

這時就應該使用閉包，來看看怎麼做:

```js
function getWallet() {
    let balance = 900
    return {
        deduce: function(money) {
            balance -= (money > 10 ? 10 : money)
            console.log(balance)
        }
    }
}

let myWallet = getWallet()
myWallet.deduce(8) // 892
myWallet.deduce(20) // 792，只扣款 10 元，符合需求
balance -= 100 // Uncaught ReferenceError: balance is not defined
```
我們首先把想要隱藏的變量，在這邊是 balance，放進 getWallet 函數中。然後看到我們的 getWallet 函數返回的是一個對象，且包含一個屬性 deduce，而 dedece 則是一個方法。因為 deduce 用到了 balance，所以這個閉包就能把 balance 鎖在 deduce 中。這樣子就達到了隱藏資訊的目的，確保這個變數不會隨意地被改動。

這個例子可能無法讓你真正體會到閉包的用途，那就再來下面的這個例子:

假設頁面上有 5 個按鈕，然後我想要第一個按下去時彈出 0，第二個按下去時彈出 1，以此類推。先來看看經典的錯誤寫法：

```js
var btn = document.querySelectorAll('button')
for(var i=0; i<btn.length; i++) {
    btn[i].addEventListener('click', function() {
        alert(i)
    })
}
```
結果發現，不管按哪個鈕，都會跳出 5 ! 為甚麼會這樣呢?首先先得回想一下，什麼是異步方法。這邊的 addEventListener 就是一個異步方法，也就是說，addEventListener 會依序被放到 event queue (任務隊列)，然後等到所有代碼都執行完才依序去執行 event queue 裡的任務。而這時，event queue 裡存的是 5 個 alert(i) 的任務。但是這時 i 已經變成了 5，所以當然不管按哪個按鈕，都會跳出 5 啦 ! 

現在知道原因之後，那回到需求上，我們要怎麼解決呢 ? 閉包這時就派上了用場，因為我們會發現，想要解決這個問題，就是要讓每個 alert(i) 的 i 被綁定到各自正確的 i，而不是最後的 5。那來看看怎麼用閉包實現吧!

```js
var btn = document.querySelectorAll('button')

function getAlert(num) {
    return function() {
        alert(num)
    }
}

for(var i=0; i<btn.length; i++) {
    btn[i].addEventListener('click', getAlert(i))
}
```
注意到，getAlert 回傳了一個 function，也就是說，num 這個變數會被鎖在 getAlert 中。也就是說，每次 event queue 裡增加一個 addEventListener，其實回調的 getAlert 就會鎖住相應的 i。所以當最後要依序執行 5 個 getAlert 時，就會輸出 0, 1, 2, 3, 4。

當然，我們還有另一個更方便的解法，那這就要感謝 ES6 推出的 let。以下代碼不用到閉包，但也能實現同樣的效果。

```js
let btn = document.querySelectorAll('button')

for(let i=0; i<btn.length; i++) {
    btn[i].addEventListener('click', function() {
        alert(i)
    })
}
```
發現把 var 改成 let 就好啦!這是因為 ES6 出現之後，js 多了 block 的概念，所以其實迴圈每跑一圈都會產生一個新的作用域，因此 alert 出來的值就會是你想要的那個值。不像 var，對於 var i 都會是同一個作用域。這也是為甚麼現在都推薦使用 let 和 const，因為這能避免 var 所產生的一些奇奇怪怪的問題。


# 結語
寫完這篇，應該對於 js 的閉包有了更進一步的了解。至少下次面試時，不會再完全不知道閉包是甚麼概念，有甚麼用途。也希望對於那些本來不了解閉包的人們，看完這篇有些許幫助，當然也歡迎各為大神們給小的一些指導和意見啦!


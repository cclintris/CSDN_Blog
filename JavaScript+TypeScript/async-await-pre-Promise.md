@[TOC](ES6 JavaScript 重新認識 Promise)

# 前言
之前自己寫過一篇關於 Promise 的文章，有興趣可以看看 [搞懂 ES6 JavaScript 中的 Promise](https://blog.csdn.net/weixin_46803507/article/details/106742492)，當時可能以為自己完全搞明白了，就又再去看了 async/await 的東西。但是在實際業務中才發現真的還是不太懂，所以這次這篇文章要談得更深入一點，希望把這部分的知識點真正累積起來。

# 正文

## js 同步/異步
首先先稍微介紹下 js 的單線程，這邊不太過深入，畢竟不是本篇的重點。

js 的單線程，其實意思就是永遠都只有一個「主線程」在運行。

而對於 js 來說，又有分為同步任務以及異步任務，所謂的同步任務其實可以理解為順序執行。其實就像程序員的邏輯一樣，在理解代碼時總是順序的閱讀，並不會去併發的看代碼，我們的思維邏輯就是同步的。而異步其實就是併發執行，相當於同時做很多事情，所以跟同步就差別於，同步都是順序的，也就意味同時只會有一個任務在執行，但異步則是說同時可能有不只一個任務在執行。這個概念非常重要，且容易混淆，因為 js 中同步異步的概念和我們日常生活中的概念似乎是相違背的。

而簡單來說，js 的主線程幹的事情其實就是執行腳本，如果碰到同步任務就直接執行，相當於直接把該任務壓棧到主線程棧上，如果碰到的是異步任務，會將任務放到異步隊列中，然後就繼續執行了。具體可以參考 [關於 JavaScript 事件循環](https://blog.csdn.net/weixin_46803507/article/details/113706311)。

關於同步異步的探討，我們先講到這邊，繼續再看下去，後面會再與這種機制關聯起來，會更加深刻。

## 場景描述
在開發 Web 時，現在通常都是前後端分離的。今天假設你是一個前端，肯定碰到過這樣的狀況 — 就是假設今天有一個 `data` 數據是要向後端發送請求來獲得的(一開始我們假設是 undefined)，而又有一個方法，是要用到這個 `data` 的，也就意味說，要想符合我們預期的執行這個方法，勢必得先拿到這個 `data`，且就意味著說，勢必要等後端返回這個數據給前端之後，我們才能執行方法。

不過這就引出一個問題：萬一今天後端處理請求的速度很慢，或是說萬一今天這個 `data` 很大，要很久才能返回。那麼基於 js 是單線程的，就表示所有事情都因為這個 `data` 而延誤了，必須等到後端返回 `data` 給我們，我們才能繼續接下去執行。

這顯然不是我們希望的，所以 js 提供的就是異步的機制，接著讓我們看看用 Promise 怎麼實現。

## Promise
因為 js 是單線程的，所以導致所有網絡操作，像是上面提到前端發 Ajax 請求給後端的操作，都必須是異步執行的。而其實異步執行可以通過回調函數實現:
```js
function callback() {
    console.log("callback...")
}

console.log("before setTimeout")

setTimeout(callback, 1000)

console.log("after setTimeout")
```
運行結果如下:
```
before setTimeout
after setTimeout
(等待 1 秒鐘)
callback...
```
肉眼感覺到的效果就是，異步操作會在將來的某個時間點觸發一個函數。

來看一個簡單的例子先 : 生成一個 0~2 的數字，如果小於 1，則等待一段時間後返回成功，否則返回失敗:
```js
function test(resolve, reject) {
    const timeout = Math.random() * 2
    console.log('setTimeout: ', timeout)
    setTimeout(function() {
        if(timeout < 1) {
            console.log("call resolve")
            resolve('200 ok')
        }else {
            console.log("call reject")
            reject('500 fail')
        }
    }, timeout * 1000)
}
```
這個 `test()` 函數有兩個參數，`resolve` 跟 `reject`，如果執行成功，就會調用到 `resolve('200 ok')`，否則就會調用 `reject('500 fail')`。這邊我們先不管 `resolve` 跟 `reject` 是什麼，我們可以看出來，`test()` 函數其實並不關心 `resolve` 跟 `reject` 具體怎麼做，他只關心自己的邏輯而已。

這時我們就可以引入 Promise 了。有了一個 `test()` 執行函數，我們就可以用 Promise 對象來執行:
```js
function test(resolve, reject) {
    const timeout = Math.random() * 2
    console.log('setTimeout: ', timeout)
    setTimeout(function() {
        if(timeout < 1) {
            console.log("call resolve")
            resolve('200 ok')
        }else {
            console.log("call reject")
            reject('500 fail')
        }
    }, timeout * 1000)
}

let pm1 = new Promise(test)
pm1.then(function(result) {
    console.log("成功:", result)
})
pm1.catch(function(err) {
    console.log("失敗:", err)
})
```
運行結果可能如下:
```
setTimeout:  0.9565183057072693
(等一下)
call resolve
成功: 200 ok
```
變量 `p1` 是一個 Promise 對象，由它來負責執行 `test()` 函數。而由於 `test()` 函數內部是異步的(setTimeout)，所以我們其實也不知道它什麼時候會做完，甚麼時候會返回結果。這時候 js 的作法就是不管了，先把這個異步的任務放到一邊，繼續執行同步任務。而 Promise 的作用就在於，當 `test()` 異步任務終於做完時，如果成功(resolve)，就會調用 `then`，如果失敗(reject)，則會調用 `catch`。

而 Promise 可以簡化起來寫，上面可以寫成如下:
```js

function test(resolve, reject) {
    const timeout = Math.random() * 2
    console.log('setTimeout: ', timeout)
    setTimeout(function() {
        if(timeout < 1) {
            console.log("call resolve")
            resolve('200 ok')
        }else {
            console.log("call reject")
            reject('500 fail')
        }
    }, timeout * 1000)
}

new Promise(test).then(function(result) {
    console.log("成功:", result)
}).catch(function(err) {
    console.log("失敗:", err)
})
```
有了對於 Promise 的基本了解，讓我們回到前面的場景。假設今天有一個 `data`，我們需要向後端請求才能獲取(我們用 setTimeout 來模擬 Ajax 異步請求)，但是我又有一個任務是要打印該 `data`。

按照程序員自己同步的思維可能會寫成這樣:
```js
let data = undefined

function show() {
    console.log(data)
}
console.log("前端發請求")

setTimeout(function() {
    data = 10
}, 1000) // 模擬後端 1 秒後才回傳數據

console.log("後端返回數據")
show(data)
```
運行結果如下:
```
前端發請求
後端返回數據
undefined
```
按照我們的邏輯，首先 `前端發請求`，然後 setTimeout 任務用於模擬後端處理，模擬需要 1 秒鐘，然後這時候我們以為 data 被置為 10(模擬後端返回數據)，接著 `後端返回數據`，然後打印 data，應該要為 10 阿!怎麼會是 undefined??

這就是因為 js 看到異步任務時，為了不想要阻塞主線程，就把這樣的任務放到異步隊列了，所以當遇到 setTimeout，js 其實就把 setTimeout 放到一邊先，然後就繼續做下去了，當執行到 `show(data)` 時，data 就當然還是 undefined。其實是等到 `show(data)` 執行完後(同步任務都做完)，js 才去異步任務隊列中，發現有一個 setTimeout 異步任務，把它拉到主線程上去執行。這時候其實 data 才被賦值，相當於後端現在才返回數據，但是你前端早早就用到這個數據了，才會出現這樣不一致的問題。

歸根到底，其實就是 js 為了要提高效率，為了不想讓這些需要耗時等待的操作阻塞整個主線程，所以提供了異步的機制。但是顯然這樣的異步機制不符合我們的邏輯，且也不符合我們的需求，所以說白了，就是我們又要讓這樣的異步任務同步化，讓異步任務串行執行，就像我們希望的那樣。於是，Promise 就派上用場，來看看 Promise 怎麼幫助我們解決問題:
```js
let data = undefined

function show() {
    console.log("後端返回數據")
    console.log(data)
}

function ajax(resolve, reject) {
    console.log("前端發請求")
    setTimeout(function() {
        data = 10 
        if(data) {
            resolve("後端處理成功")
        }else {
            reject("後端處理失敗")
        }
    }, 1000) // 模擬後端 1 秒後才回傳數據
}

new Promise(ajax).then(function(result) {
    console.log("200 Success:", result)
    show()
}).catch(function(err) {
    console.log("404 Not found:", err)
})

console.log("繼續做其他不需要用到 data 的事情...")
```
運行結果如下:
```
前端發請求
繼續做其他不需要用到 data 的事情...
(等待 1 秒鐘)
200 Success: 後端處理成功
後端返回數據
10
```
利用 Promise，我們就做到了，最後打印出來的 data 確實也是我們希望的 10。而這樣的好處就在於，我們前端發送了請求之後，就不用管了，不用管後端甚麼時候才會返回 data，也不用管後端要多久處理，也不用管後端處理會不會有問題，反正，等到後端做完(無論正確與否)，Promise 都會再幫我們接後端返回的信息。而這段時間，我們就無需等待後端，無需等待網絡開銷等等，而可以繼續做其他不需要用到 data 的事情，等到 data 回來了，再去做需要 data 的那些任務(`show`)。

## Promise 鏈式調用
最後要來說說 Promise 的鏈式調用。你可能會常常在別的博客上看到這麼一句話：「Promise 的 then 所回傳的東西其實也是個 Promise，並且可以作為下一個 then 的參數，所以 Promise 是支持鏈式調用的。」都怪自己不求甚解，隨便看看過就以為懂了，實際上根本不懂不會用。後來經過大佬指點，好像終於有點懂了。來看看下面的代碼:
```js
const p = new Promise((resolve, reject) => {
    const n = 1
    console.log(`n = ${n}`)
    if (n > 0.5) {
        resolve(n)
    } else {
        reject(n)
    }
})

p.then(res => {
    console.log(`res 1 = ${res}`) 
    return 'then 1 result'
}).catch(err => {
    console.log(`err 1 = ${err}`)
    return 'catch 1 result'
}).then(res => {
    console.log(`res 2 = ${res}`)
    return Promise.reject('then 2 result')
}).catch(err => {
    console.log(`err 2 = ${err}`)
    return Promise.resolve('catch 2 result') 
}).then(res => {
    console.log(`res 3 = ${res}`)
}).catch(err => {
    console.log(`err 3 = ${err}`) 
})
```
運行結果如下:
```
n = 1
res 1 = 1
res 2 = then 1 result
err 2 = then 2 result
res 3 = catch 2 result
```
這時候再回去看看上面那句話，當時我就很矇了。首先，就是 new 出一個 Promise 對象，裡面也很正常的丟入一個參數為 resolve, reject 的函數(雖然不是異步的但也沒關係，這邊只是為了演示)，但是，第一個 then 怎麼那麼奇怪??catch 也是，怎麼那麼奇怪。奇怪在哪裡呢?奇怪在於，不是說 then 返回的其實也是個 Promise 嗎，但怎麼都看起來像是就只是返回一個普通的字符串阿。那既然返回的好像不是一個 Promise，它又是怎麼被下面的 then, catch 接到的呢?

經過大佬的指點，原來是這樣的。其實無論你在 then 或 catch 中確實是返回一個 Promise 對象也好，或是 return 一個單純的東西也好，其實它都經過的 `Promise.resolve()`。甚麼意思呢，就是說，其實上面代碼的第一段 then/catch 其實等價於:
```js
p.then(function(res) {
    console.log(`res 1 = ${res}`)
    return Promise.resolve('then 1 result')
}).catch(function(err) {
    console.log(`err 1 = ${err}`)
    return Promise.resolve('catch 1 result')
})
```
這麼一來就解開疑惑了，而值得注意的是，無論是 then(resolve 狀態的 promise)，還是 catch(reject 狀態的 promise)，它的返回值都是一個 resolve 狀態的 promise，這邊隱含的意義就是，無論是 then 還是 catch，其實都仍就算是一個「預期中」的結果。

至於為甚麼大家常常在說的 Promise 鏈式調用好像都是很多個 then 構成的 then Promise 鏈，其實只是因為，通常業務場景下，第一個 Promise 對象 resolve 完會先被 then 給接到，而這才是我們「預期，希望」的，所以即使後面還要處理也是接著這個 then，再接著 then。而至於第一個 Promise reject 後則會被 catch 給接到，通常就是甚麼異常阿，錯誤之類的，通常也就不會再有後續的處理了。


# 結語
Promise 其實就是這樣，對於新手可能不是很好理解，但是理解了之後會豁然開朗的。也不知道這次自己真的懂了沒，但是確實感覺有更加理解了 Promise 了一點，還得感謝本人的大佬同學高人指點。

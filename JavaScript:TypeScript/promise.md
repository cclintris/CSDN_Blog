@[TOC](搞懂 ES6 JavaScript 中的 promise)

# 前言
首先，這篇博客不會太短，因為要理解 js 的 promise 並不是特別容易的一件事情，在我個人看來，它算是 ES6 的一大進步，但也是學習上的一大門檻。ES6 promise 改善了以往使用 callback 所會造成的 "callback hell"，且也更好理解清楚。這篇算是來整理一下對於 promise 的學習歷程，也方便以後有個參照。

# 正文

## callback
要想完全理解 promise 的好處以及概念，首先我們還是先從 callback 這個東西開始介紹。

### callback 是什麼
callback 顧名思義，就是回頭呼叫的意思，也就是我們常聽到的回調函數。更詳細點說，回調函數就是「在一個函式執行完後，才要執行的函式」。直接來看看 callback 該怎麼使用吧!

假設我有個 doSecond 函數，想要在 doFirst 函數執行完後才去執行，就可以這麼用 callback

```js
function doSecond() {
    console.log('do second')
}

function doFirst(callback) {
    console.log('do first')
    callback()
}

doFirst(doSecond)
```
結果顯示如下:
![](https://wtfhhh.oss-cn-beijing.aliyuncs.com/prom-1.png)

可能你會覺得這樣多此一舉，覺得 callback 根本沒有存在的必要，覺得那我這麼寫不就好了:

```js
function doFirst() {
    console.log('do first')
}

function doSecond() {
    console.log('do second')
}

doFirst()
doSecond()
```
結果顯示如下:
![](https://wtfhhh.oss-cn-beijing.aliyuncs.com/prom-1.png)

確實在這個例子看不出為什麼要用 callback，但是換成下面這個例子就不一樣了。

```js
let doFirst = function() {
    setTimeout(function() {
        console.log('do first')
    }, 3000)
}

let doSecond = function() {
    console.log('do second')
}

doFirst()
doSecond()
```
如果你以為會是先等待 3 秒然後輸出 do First，接著輸出 do second，那你就錯了!你會發現，它會直接先輸出 do second，然後等待 3 秒才輸出 do first。

```js
do second
// 等待三秒
do first
```
這其實是因為 js 裡面同步和異步的概念。因為 setTimeout 是一個異步操作，所以 doSecond 會先執行，而 doFirst 會被放進任務隊列當中，等到同步操作都結束了，才會開始執行任務隊列中的任務。

但這顯然不是我們想達成的效果。這時候 callback 就派上用場了。來看看 callback 怎麼做到的:

```js
let doFirst = function(callback) {
    setTimeout(function() {
        console.log('do first')
        callback()
    }, 3000)
}

let doSecond = function() {
    console.log('do second')
}

doFirst(doSecond)
```
```js
// 等待三秒
do first
do second
```
> 總結一下，其實 callback function 比較好的解釋是：「以參數型態傳入另一個函式的函式」。而所造成的效果就是，可以保證一個函式會在另一個函式執行後才執行。

### callback hell
但是 callback 有什麼問題呢?試想一下，假設今天有很多個 api，且我每個 api 之間都得等到上一個 api 執行完了才能接下去執行，這樣會發生什麼事?

```js
let api1 = (callback) => {
    setTimeout(() => {
        console.log('Done api1')
        callback()
    }, 2000)
}

let api2 = (callback) => {
    setTimeout(() => {
        console.log('Done api2')
        callback()
    }, 1000)
}

let api3 = (callback) => {
    setTimeout(() => {
        console.log('Done api3')
        callback()
    }, 1000)
}

let done = () => {
    console.log('All api done')
}

let main = (callback) => {
    api1(() => {
        api2(() => {
            api3(() => {
                callback()
            })
        })
    })
}

main(done)
```
```js
// 等待2秒
Done api1
// 等待1秒
Done api2
// 等待1秒
Done api3
All api done
```
這還只是只有三個 api 的情況。如果有更多要 ==依序執行==的 api 的話，可以想見，代碼的易讀性會有多差，且會有多不好理解和維護。這就是所謂的 callback hell。

## promise
了解了 callback 之後，可以來了解 promise 了。其實，promise 本質上可以說是跟 callback 很像，只是它是 ES6 提供的語法糖，更好理解，也更加值觀。

### promise 是甚麼
首先先看看 MDN 官方的解釋:
> Promise 对象是一个代理对象（代理一个值），被代理的值在Promise对象创建时可能是未知的。它允许你为异步操作的成功和失败分别绑定相应的处理方法（handlers）。 这让异步方法可以像同步方法那样返回值，但并不是立即返回最终执行结果，而是一个能代表未来出现的结果的promise对象

本人一開始看完這段話，心理瘋狂 OS。真的是完全無言，什麼都看不懂，大概是我理解力太差了。看了資料後，總算有了些心得，整理一下。帶入場景是我感覺更好理解的方式。

>promise，顧名思義，就是承諾的意思。

就像 cc 要過生日了，然後 popo 承諾 cc 說要送他一雙球鞋。但是有可能 popo 只是隨口一說阿，不知道到底 popo 會不會真的送 cc。我們無法斷定 popo 給 cc 的 promise 會不會實現。

所以 promise 其實有三種狀態:
1. pending: cc 不知道 popo 到底會不會送禮物，只能等生日當天到來
2. fulfilled: 生日當天到了，popo 也確實履行承諾，送了 cc 球鞋
3. rejected: 生日當天到了，但是 popo 沒有履行承諾，也就是說 cc 並沒有收到來自 popo 的禮物

知道了 promise 這個對象的三種狀態後，來看看 promise 有什麼規則:
1. 前面提到 promise 本質就是 callback。所以也是用來改善異步操作的。這邊要記得，只有異步操作可以改變 promise 的狀態，這個 promise 的第一個規則。
2. 可以把 promise 這個對象想成米飯。不是有這麼一句話，生米煮成熟飯，意思是說事情不能回到過去了，發生就是發生了。promise 也一樣，狀態改變後，是不能反悔的。而狀態之間只有兩種可能的轉換:
> 1. pending -> fulfilled
> 2. pending -> rejected

其實這也很好理解。就是說，cc 等待的這時間都算是 pending 的狀態。那等到生日當天，popo 要馬送了禮物，要馬就是沒送。不可能說，發現了 popo 沒送 (pending -> rejected)，就回到幾天前 (pending) 再等待一次，希望這次 popo 會送 (pending -> fulfilled)。這是不可能的，也不合理啊!其實很好理解的，是不是?

現在對 promise 有個基本概念之後，就來看看要怎麼寫 promise!

### promise 語法
MDN 官方的 promise 語法:
```js
new Promise(function(resolve, reject) {
    ...
})
```
接續上面的例子，來看看上面的 resolve 跟 reject 是什麼吧!

先把上面截至目前的劇情寫成代碼:

```js
// popoForget = false => popo 沒有忘記
// popoForget = true => popo 忘了
let popoForget = false

let ccGetShoe = new Promise(function(resolve, reject) {
    if(!popoForget) {
        let shoe = {
            brand: 'Nike',
            price: 3000
        }
        resolve(shoe)
    }else{
        let err = new Error("popo forget cc birthday")
        reject(err)
    }
})

let testPromise = function() {
    ccGetShoe.then(function(fulfilled) {
        console.log(fulfilled)
    }).catch(function(rejected) {
        console.log(rejected.message)
    })
}

testPromise()
```
popoForget = false
![](https://wtfhhh.oss-cn-beijing.aliyuncs.com/prom-2.png)

popoForget = true
![](https://wtfhhh.oss-cn-beijing.aliyuncs.com/prom-3.png)

上面的 ccGetShoe 就是一個 promise 對象。而 resolve 和 reject 則是函數。如果承諾兌現的話，那麼 ccGetShoe 就會從 pending 變為 fulfilled，並調用 resolve，resolve 負責把 shoe 傳出去。反之，ccGetShoe 就會從 pending 變成 rejected，並調用 reject，reject 則會負責把 Error 傳出去。

而當一個 promise 狀態更改後，就會要有相對應的處理方式。對於 fulfilled 的 promise，我們要用 then 來接，對於 rejected 的 promise，則要用 catch 來接。而 then 和 catch 裡面函數(處理方式)的參數就會接到 resolve 或是 reject 函數所傳出來的東西。

### promise 的鏈式調用
promise 的一大特色就在於鏈式調用。那為甚麼可以鏈式調用呢?因為其實 then 和 catch 也同樣返回出一個 promise 對象，就跟 resolve 和 reject 函數一樣，會返回一個 promise 對象。延續上面的例子看看或許會更清楚些。

假設，如果 cc 收到 popo 送的鞋子後就會想要再買一件衣服來搭配。仔細想想，其實這有是一個承諾，所以也可以用 promise 來實現:

```js
// popoForget = false => popo 沒有忘記
// popoForget = true => popo 忘了
let popoForget = false

let ccGetShoe = new Promise(function(resolve, reject) {
    if(!popoForget) {
        let shoe = {
            brand: 'Nike',
            price: 3000
        }
        resolve(shoe)
    }else{
        let err = new Error("popo forget cc birthday")
        reject(err)
    }
})

let ccBuyCloth = function(shoe) {
    let cloth = {
        color: 'blue',
        price: 2000
    }

    let clothAndShoe = {
        msg: 'cc bought' + cloth.price + cloth.color + 'cloth to match shoes',
        shoe: shoe,
        cloth: cloth 
    }
    return Promise.resolve(clothAndShoe)
}

let testPromise = function() {
    ccGetShoe
        .then(ccBuyCloth)
        .then(function(fulfilled) {
            console.log(fulfilled)
        }).catch(function(rejected) {
            console.log(rejected.message)
        })
}

testPromise()
```
解析下上面代碼:
1. ccGetShoe 這個方法跟上面都一樣，就是返回一個 promise。
2. ccBuyCloth 則特別一點。因為 cc 收到鞋子後一定會買衣服來搭配，所以這個 promise 只會是 fulfilled。因此，返回時就直接調用了 Promise.resolve。那為甚麼還要返回 promise 呢，感覺就沒必要了阿，既然我都知道一定要執行了。原因很簡單，看看 testPromise 方法，ccGetShoe 返回一個 promise，所以用 then 接。然後 then 裡又放了 ccBuyCloth，表示如果確實收到鞋子要採取的動作。這時注意到後面依然用了 then 接，所以也就是說， ccBuyCloth 也得返回一個 promise 才行。而因為 ccBuyCloth 返回的 promise 的狀態為 fulfilled，所以回調函數的參數是 fulfilled。
3. 上面的 ccGetShoe.then().then() 就是 promise 的鏈式調用。其實就是一直傳遞 promise 對象，來進行這樣不斷地鏈式調用。而 then 顧名思義就有做完某件事接下來要做的事情的涵義，所以也確實更好理解。

### promise 與異步任務
首先，回憶一下 js 的同步任務和異步任務。有了基本概念後，我們來看看這段代碼會輸出甚麼:

```js
let popoForget = false

let ccGetShoe = new Promise(function(resolve, reject) {
    if(!popoForget) {
        let shoe = {
            brand: 'Nike',
            price: 3000
        }
        resolve(shoe)
    }else{
        let err = new Error("popo forget cc birthday")
        reject(err)
    }
})

let testPromise = function() {
    ccGetShoe.then(function(fulfilled) {
        console.log(fulfilled)
    }).catch(function(rejected) {
        console.log(rejected.message)
    })
}

let testAsync = () => {
    console.log('before promise')

    setTimeout(function() {
        console.log('async operation 1')
    }, 2000)

    // async operation 2
    testPromise()

    console.log('after promise')
}

testAsync()
```
我們都知道，js 引擎會先把同步任務放到棧中，把異步任務放到任務隊列中，然後先把所有同步任務都執行完了，再去把任務隊列的任務搬到棧中執行。那看看 testAsync，首先應該會輸出 before promise，再來輸出 after promise。然後這時同步操作都做完了，開始去執行任務隊列裡面的任務。首先應該會拿到 setTimeout 任務，所以輸出 async operation 1，然後再拿到 testPromise 的 promise 結果並輸出。但是看看實際的輸出是什麼樣子:
![](https://wtfhhh.oss-cn-beijing.aliyuncs.com/prom-4.png)

為什麼是先輸出 promise，才輸出 setTimeout 的任務呢?這就要更深入去探討，異步任務也有分兩類:
>1. microtask(微任務) : promise，mutation observe 會被放到微任務隊列
>2. macrotask(宏任務) : setTimeout，setInterval 會被放到宏任務隊列。
> 當棧的同步任務全都清除後，js 引擎執行異步任務時，會先去找 microtask queue 裡的任務並執行，等到 microtask queue 的任務也都執行完了，再去執行 macrotask queue 的任務。

好啦!那就都沒問題了，確實應該先響應 promise ，才響應 setTimeout。

# 結語
花了三天才寫完了這篇博客，因為 ES6 promise 真的是頗重要的一個知識點，所以花了較長的篇幅來和 callback 比較，並理解整個 promise 的概念。感覺以後去面試也多多少少會問到 promise 的相關範圍，所以搞得更加清楚也挺好的。當時自己在學習 promise 時真的很痛苦，所以希望看完這篇，對那些不了解 promise 的人們有些許幫助啦!

# 參考
[怎么理解JS Promise](https://blog.csdn.net/qq_37860963/article/details/81539118)
[JavaScript - Promise (2)](https://ithelp.ithome.com.tw/articles/10197529)




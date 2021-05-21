@[TOC](關於 JavaScript 事件循環)

# 前言
會寫這篇是因為關於 js 的事件這一大板塊，實在是一個非常重要的知識點，無論是在面試，考試，或是實際代碼端中都無法避開，是一個非常重要卻又龐大的知識點。為了更清楚的搞明白，也為了日後方便自己查閱資料所需，就自己動手寫了這篇。在這篇中，我想將 js 的事件分成兩大部分，事件循環以及事件流來講，其中又會再對不同知識點做更多講解。

# 正文

## 事件循環

### 單線程的 js
首先我們知道，js 是單線程語言，所謂單線程就是說 js 引擎負責解釋和運行代碼的線程只有一個。那為什麼 js 要是單線程的呢?其實也很好理解，因為其實 js 最開始就是為了使網頁動起來的腳本語言，主要就是希望可以將讓靜態的 html 增添交互性，因此也就是操作 html 元素。那假設 js 是多線程，那如果對於同一個 `div`，同時有多個 js 線程在改變它，那最後這個 `div` 會是怎樣的誰都無法知道，而這顯然是我們不希望發生的。 

### 同步任務 vs 異步任務
這邊先給出一個個人認為

那上面知道了，js 是單線程的，所以所有任務都只能在主線程上排隊等待執行。但是萬一有些任務，比如 Ajax 請求，就是特別耗時，且存在很多不確定因素(網絡，設備等等)，無法預測到底甚麼時候該任務才會完成。如果也把這些任務放到等待隊列中，那主線程肯定是會阻塞，這樣會大大降低瀏覽器的效率，很糟糕!

於是瀏覽器就把這些任務分派到異步任務隊列中，並跟他們說：「你們隨便要執行多久吧，等你們執行好了再跟我說喔!」。js 中最常見的異步請求是 `setTimeout`，相信大家都聽過，先來看一個簡單的例子:
```js
console.log('start')

setTimeout(() => {
    console.log('setTimeout is asynchronous')
}, 0);

console.log('end')
```

輸出如下:
![](https://wtfhhh.oss-cn-beijing.aliyuncs.com/event-1.png)
可以看到雖然 `setTimeout` 任務被延時 0 秒，相當於根本沒有延時，但輸出卻是在最後。原因就是 js 引擎會先執行完同步隊列中的任務，再去執行異步隊列中的任務，在此例中也就是 `setTimeout` 任務。

再多補充點，如果同步任務好死不死也有特別耗時，那麼異步任務也不會按時執行。來看看如下例子:
```js
console.log('start')

setTimeout(function() {
    console.log('setTimeout is asynchronous')
}, 1000)


console.log('now')
let list = []

for(let i = 0;i<9999999;i++){
    let now = new Date()
    list.push(i)
}
```
這個例子，輸出仍然如上面一樣。
![](https://wtfhhh.oss-cn-beijing.aliyuncs.com/event-3.png)
但是值得注意的地方在於，我們預期 setTimeout is asynchronous 應該在 1s 後輸出，但實際上我們會發現大約在 2s 後才輸出。原因就是因為同步任務的 for 循環執行太多次，延遲了異步任務被執行的時間。

到這邊先簡單整理下，事件循環的基本流程如下:
>1. 所有任務都在主線程上執行，形成一個執行棧
>2. 首先 js引擎 會把所有同步任務依序裝進執行棧，並一一執行，如果碰到異步任務，就會把該任務放進異步隊列中，也就是所謂的「任務隊列」(FIFO原則)。
>3. 等到所有同步任務都執行完並離開執行棧，js引擎 才會去檢查任務隊列，並將異步任務放到執行棧中執行。
>4. 重複第 3 步直到任務隊列為空

整個這樣的過程就稱為事件循環。

### 宏任務 vs 微任務
上面提到的任務隊列放的就是所有的異步任務，但其實還分得更細。js 還將異步任務分為宏任務(macrotask)與微任務(microtask)，不同的 API 註冊的任次會依序進入自身所對應的隊列中，然後等待事件循環機制將他們一次壓入棧中執行。下面給出一個整理：

宏任務包括:
- plain script(整體 js 代碼)
- setTimeout, setInterval, setImmediate
- I/O
- UI 渲染

微任務包括:
- process.nextTick
- Promise
- MutationObserver(H5 新特性)

可以把整體的 js 代碼也看成是一個根宏任務，主線程也是從這個宏任務開始執行。於是上面的事件循環機制中的異步任務執行緒可以改變一下:
![](https://wtfhhh.oss-cn-beijing.aliyuncs.com/event-4.png)

這邊要在多做點補充，因為當時也是到這邊以為自己都懂了，沒想到還只是一知半解而已。
>一個掘金的老哥（ssssyoki）的文章摘要： js異步有一個機制，就是遇到宏任務，先執行宏任務，將宏任務放入eventqueue，然後再執行微任務，將微任務放入eventqueue。最騷的是，這兩個queue還不是一個queue。當你往外拿(也就是要放到執行棧中時)的時候先從微任務裡拿回掉函數，然後再從宏任務的queue上拿宏任務的回掉函數。 我當時看到這我就服了還有這種騷操作。

所以看到這邊，在結合上圖就知道，其實所謂的「執行」異步任務只是將宏任務或是微任務放入各自 eventqueue 中，真正的執行其實是先從微任務隊列中拿到執行棧，待微任務隊列為空後才接著去拿宏任務隊列上的任務。所謂異步隊列可以說是包含了宏任務隊列以及微任務兩個隊列。其中，兩個隊列都分別按照 FIFO 的原則。

現在就讓我們用 Promise 來理解以上概念，如果對於 js 的 Promise 還不是特別熟悉的，歡迎先去看看 [搞懂 ES6 JavaScript 中的 Promise](https://blog.csdn.net/weixin_46803507/article/details/106742492)。那麼例子如下:
```js
console.log('start')

setTimeout(function() {
    console.log('timeout');
}, 0)

new Promise(function(resolve) {
    console.log('promise');
    // 注意这边调用resolve
    // 不然then方法不会执行
    resolve()
}).then(function() {
    console.log('then');
})

console.log('end');
```

分析一下上面代碼的執行過程:
1. 剛開始打印 `start`
2. 遇到宏任務 setTimeout，放入宏任務隊列中，等待執行
3. 遇到 Promise 回調函數，同步執行，打印 `promise`
4. 當 promise 物件 resolve 後 then 方法屬於微任務，被放入微任務隊列等待執行
5. 打印 `end`
6. 同步任務都執行完，先從微任務隊列拿，執行 then 方法的回調函數，打印 `then`
7. 再從宏任務隊列拿，執行 setTimeout 回掉函數，打印 `timeout`。

實際的打印應該如下:
![](https://wtfhhh.oss-cn-beijing.aliyuncs.com/event-5.png)

把 Promise 變一下，再看看一個例子:
```js
async function async1() {
    console.log('async1 start')
    await async2()
    console.log('async1 end')
}
async function async2() {
    console.log('async2')
}

async1()
console.log('script end')
```

直觀的看代碼想當然我們會認為輸出順序應該是 `async1 start`->`async2`->`async1 end`->`script end`。但當然沒那麼簡單啦!真正理解了 async 後，我們會知道其實 async 是對 Promise 的進一步封裝，具體我會再寫一篇詳細說明，但這邊先接受，async 其實會把 await 後(包括 await 語句本身)的所有東西都當作 Promise 一樣放到 then 函數中。因此，對上面的代碼改寫，就會變如下:
```js
async function async1() {
    console.log('async1 start')
    new Promise(function(resolve) {
        console.log('async2')
        resolve()
    }).then(function() {
        console.log('async1 end')
    })
}

async1()
console.log('script end')
```
這時，根據我們上面對事件循環的理解再分析一下:
1. 輸出 `async1 start`
2. 遇到 Promise，先同步執行輸出 `async2`，then 屬於微任務，要等到 Promise 物件 resolve 後才會被放到微任務隊列中
3. 輸出 `script end`
4. 同步任務都執行完，執行棧空，檢查微任務隊列，執行 then 中的回調函數，輸出 `async1 end`

實際輸出如下:
![](https://wtfhhh.oss-cn-beijing.aliyuncs.com/event-6.png)

# 結語
這篇大致上涵蓋了關於 js 事件循環的概念，個人認為是一個還算重要的知識點，應該也還算詳細。希望看完這篇對大家有所幫助，若有誤，也歡迎多多指教!

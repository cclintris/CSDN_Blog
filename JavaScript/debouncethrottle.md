@[TOC](前端優化 - 防抖與節流)

# 從滾動條監聽的例子說起
要理解防抖與節流，我認為就要先去了解實際的業務場景有什麼問題，從而引出這樣的需求。我們從一個常見的場景來說起，大家一定都知道很多頁面都會有滾動條，其實所謂的拖動滾動條就是一個 `scroll` 事件。那麼我們現在來監聽一個指標，我們來監聽瀏覽器滾動事件，並且返回當前滾動條與頂部的距離。這個需求也不難，代碼如下：
```js
function showTop() {
  let scrollTop = document.body.scrollTop || document.documentElement.scrollTop
  console.log("滚动条位置：" + scrollTop)
}
window.onscroll = showTop
```
把這段腳本貼到瀏覽器中，並且就以百度的網頁為例，開始拖動，效果如下：
![debouncethrottle.gif](https://sc01.alicdn.com/kf/Uf0d9e4a3fdd64951ae6735bbad05b4261.jpg)
可以很清楚地看到，即使是隨便小小的滾動一下都會觸發很多很多次 `scroll` 事件，而且是非常高，即使只是按一下 [下方鍵] 都會觸發 8~9 次。

而在實際的業務場景下，這種情況是很不好的，大量的浪費了系統的性能，會造成性能上的缺陷，因此我們就引出本文的主角，防抖與節流，也就是為了解決這樣的問題，是一種前端優化的技術。

# 防抖(debounce)
基於上述場景，我們提出第一種解決方案，防抖。思路是：我們在第一次觸發事件時不立即執行函數，而是給出一個計時器，或是說期限值，比如說 200ms，然後：

- 如果在 200ms 內沒有再次觸發滾動事件，那麼執行函數
- 如果在 200ms 內再次觸發了滾動事件，那麼當前的計時器要重新歸零計時

既然都提到了計時器，那麼我們借助 `setTimeout()` 來實現，而由於還需要一個變量來保存計時，所以我們使用閉包來實現。我們來看看代碼實現：
```js
/**
 * fn [Function] : 需要防抖的函數
 * delay [Number] : 防抖期限值
 */

function debounce(fn, delay) {
  let timer = null // 閉包
  return function() {
    if(timer) {
      clearTimeout(timer)
    }
    timer = setTimeout(fn, delay)
  }
}
```
具體使用起來如下：
```js
function showTop() {
  let scrollTop = document.body.scrollTop || document.documentElement.scrollTop
  console.log("滚动条位置：" + scrollTop)
}
window.onscroll = debounce(showTop, 1000)
```
現在來看看效果：
![debounce.gif](https://ae02.alicdn.com/kf/U89cdba29b6954cb6a0f2fe755dfe543b2.jpg)

我們會發現，此時如果你一直滑動滾動條，其實是不會觸發滾動事件的，只有當我們停止滾動 1 秒後，才會觸發滾動事件。

其實這樣就算是基本防抖的實現，最後來看看對於防抖的定義：

> 對於短時間內連續觸發的密集事件(例如 `scroll` 事件)，防抖的目的就是限制在某個時間期限內(1000ms)，事件處理函數只會執行 1 次。

# 節流(throttle)
接續上面的防抖，我們再想一下想一下用防抖處理的結果是：
- 如果在時間期限內，用戶就很無聊手很閒，一直滑動滾動條，不斷的觸發滾動事件，只要不超過 1 秒停止動作，就不會觸發滾動事件，理論上就永遠不會觸發滾動事件

但是其實這樣好像也不太對，正常情況下，可能要求的業務場景會是：
- 即使用戶不斷拖動滾動條，也能在某個時間間隔之後觸發函數，給出反饋

那麼這就是節流所要實現的。思路大致上是，我們設計一個類似閘門一樣定期開放的函數，也就是說讓函數在執行完一次之後，會失效一個時間段，過了這個時間段，又會再被重新激活。

這麼做效果就會是，如果短時間內大量觸發一個事件，那麼函數在執行一次之後，在指定期限內，即使用戶仍然繼續觸發事件，也不會響應，直到過了這段時限，用戶的觸發才會再被響應。

我們同樣借助 `setTimeout()` 來實現，並加上一個狀態 `valid` 來標示函數當前是否處於工作狀態。
```js
/**
 * fn [Function] : 需要節流的函數
 * delay [Number] : 節流期限值
 */

function throttle(fn, delay) {
  let valid = true
  return function() {
    if(!valid) {
      return
    }
    valid = false
    setTimeout(() => {
      fn()
      valid = true
    }, delay)
  }
}
```
具體使用起來如下：
```js
function showTop() {
  let scrollTop = document.body.scrollTop || document.documentElement.scrollTop
  console.log("滚动条位置：" + scrollTop)
}
window.onscroll = throttle(showTop, 1000)
```
現在來看看效果：
![throttle.gif](https://ae03.alicdn.com/kf/U7b4e7a74a623480aa6061285fe665eceB.jpg)

我們可以看到，現在如果用戶不斷地拖動滾動條，也會在 1 秒的間隔觸發滾動事件，而如果停止拖動，當然就不會觸發。

# 通用 防抖/節流 API
上面我們通過防抖跟節流核心的思想完成了優化，但是需要注意到的是，因為上面的 `showTop` 本身很簡單，所以我們都沒有考慮到別的問題，實際上，我們還要考慮到參數，以及 this 的問題(上下文環境)。下面我們給出一個通用的防抖以及節流的 API。下面用到了 `apply`，如果不清楚歡迎參考 [理解 JavaScript 中的 call apply bind](https://blog.csdn.net/weixin_46803507/article/details/115263494)。

- 防抖
```js
function debounce(fn, delay) {
  let timer = null
  return function() {
    // 將 this 指向實際返回的函數
    let context = this

    if(timer) {
      clearTimeout(timer)
      timer = null
    }

    timer = setTimeout(() => {
      fn.apply(context, ...arguments)
    }, delay)
  }
}
```
- 節流
```js
function throttle(fn, delay) {
  let valid = true
  return function() {
    // 將 this 指向實際返回的函數
    let context = this

    if(!valid) {
      return 
    }
    valid = false
    setTimeout(() => {
      valid = true
      fn.apply(context, ...arguments)
    }, delay)
  }
}
```
# 場景使用舉例
防抖跟節流的技術非常重要，根據不同場景都可能會要應用到。

1. 比如說，有時候對於搜索匡 `input` 事件，需要支持說每隔多少秒就要搜索輸入，做到一個實時搜索的效果，這就可以使用節流。
2. 或是說，對於頁面的 `resize` 事件，常見於需要做到頁面適配的話，需要根據頁面最終的狀況進行 DOM 的重新渲染，這種時候一般就是使用防抖，因為因為只需要判斷最後一次的變化情況。


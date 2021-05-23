@[TOC](Ajax 原理及手動封裝)

# Ajax 是什麼？
首先，先來簡單的介紹下 Ajax 到底是什麼東西？Ajax 全稱是 Asynchronous JavaScript and XML，也就是異步的 JavaScript 和 XML，是一種建交互式網頁的開發技術，可以在不需要重新整個網頁的情況下與服務端交換數據，並且更新部分網頁。

Ajax 的原理其實是通過 `XMLHttpReqeust` 對象實現的，通過 `XMLHttpReqeust` 對象來向服務器發送異步請求，從服務器獲得數據，並使用 js 操作 DOM 節點來更新頁面。
最![ajax-1.png](https://ae02.alicdn.com/kf/Ucd439f7e8de349de8957faf5d054b018n.jpg)
下面舉個例子：
領導想找小林匯報一下工作，於是委託秘書去叫小李，自己就接著做其他的事情了，直到秘書告訴他說小李已經到了，最後由小李跟領導匯報工作。

`Ajax` 請求數據的流程就與上面場景類似，上述的秘書也就相當於  `XMLHttpReqeust` 對象，領導相當於瀏覽器，小李則相當於服務端。

瀏覽器可以通過 `HTTP` 向後端發送請求，接著做其他事情，等收到返回來的數據再回頭進行相關操作。

# 實現 Ajax 思路
手動封裝 `Ajax` 異步交互需要與服務器邏輯進行配合，大體來說需要完成以下步驟：

1. 創建 `Ajax` 核心對象 `XMLHttpReqeust` 對象
2. 通過 `XMLHttpReqeust` 對象的 `open()` 方法與服務端建立連接
3. 構建請求所需的數據內容，並通過 `XMLHttpReqeust` 對象的 `send()` 方法發送給服務端
4. 通過 `XMLHttpReqeust` 對象提供的 `onreadystatechange` 事件監聽服務端與你的通信狀態
5. 接受並處理服務端向客戶端響應的數據結果
6. 將處理結果更新到 HTML 頁面中

## 創建 XMLHttpReqeust 對象
我們可以通過 `XMLHttpReqeust()` 初始化一個 `XMLHttpReqeust` 實例
```js
const xhr = new XMLHttpReqeust()
```
## 與服務端建立連接
通過 `XmlHttpReqeust` 對象的 `open()` 方法與服務器建立連接。
```js
xhr.open(method, url, [async][, user][, password])
```
參數說明：

- `method`: 表示當前的請求方式，常見的有 `GET` 和 `POST`
- `url`: 服務端 ip 地址
- `async`: bool 值，表示是否為異步執行，默認為 `true`
- `user`: 可選的用戶名，用於認證，默認為 `null`
- `password`: 可選的密碼，用於認證，默認為 `null`

## 向服務端發送數據
通過 `XmlHttpReqeust` 對象的 `send()` 方法將客戶端頁面的數據發送給服務端。
```js
xhr.send([body])
```
`body`: 在 `xhr` 請求中要發送的數據體，如果沒有數據則為 `null`。

如果使用 `GET` 請求發送請求的話，需要注意兩點：

- 將請求數據添加到 `open()` 方法的 `url` 地址參數中
- 發送請求數據的 `send()` 方法的 `body` 參數為 `null`

## 綁定 onreadystatechange 事件屬性
`onreadystatechange` 事件用於監聽服務端的通信狀態，主要監聽的屬性為 `XMLHttpRequest.readyState`。

關於 `XMLHttpRequest.readyState` 有 5 種狀態，這邊給出整理：

|value|狀態|描述|
|:-----|:-----|:-----|
|0|UNSENT(未打開)|open()方法還未被調用|
|1|OPENED(未發送)|send()方法還未被調用|
|2|HEADERS_RECEIVED(已獲取響應頭部)|send()方法還已經被調用，響應頭和響應狀態已經返回|
|3|LOADING(正在下載響應體)|響應體下載中，reponseText 中已經獲取部分數據|
|4|DONE(請求完成)|整個請求過程結束|

只要 `readyState` 值一變化，就會觸發一次 `onreadystatechange` 事件。

這邊我們會用 `XMLHttpRequest.responseText` 來接收服務器的響應結果。舉個例子：
```js
const request = new XMLHttpRequest()
request.onreadystatechange = function(e){
    if(request.readyState === 4){ // 整个请求过程完毕
        if(request.status >= 200 && request.status <= 300){
            console.log(request.responseText) // 服务端返回的结果
        }else if(request.status >=400){
            console.log("错误信息：" + request.status)
        }
    }
}
request.open('POST', 'http://xxxx')
request.send()
```
# 封裝 Ajax
通過上面對 Ajax 的瞭解後，下面就直接上代碼，來看看具體如何封裝一個簡單的 ajax 請求。
```js
// 封裝一個 ajax 請求
function ajax(options) {
    // 創建 XMLHttpRequest 對象
    const xhr = new XMLHttpRequest()

    // 初始化參數內容
    options = options || {}
    options.type = (options.type || 'GET').toUpperCase()
    options.dataType = options.dataType || 'json'
    const params = options.data

    // 發送請求
    if(options.type === 'GET') {
        xhr.open('GET', options.url + '?' + params, true)
        xhr.send(null)
    }else if(options.type === 'POST') {
        xhr.open('POST', options.url, true)
        xhr.send(params)

        // 接收請求
        xhr.onreadystatechange = function() {
            if(xhr.readyState === 4) {
                let status = xhr.status
                if(status >= 200 && status < 300) {
                    options.success && options.success(xhr.responseText, xhr.responseXML)
                }else {
                    options.fail && options.fail(status)
                }
            }
        }
    }
}
```
使用方式則如下：
```js
ajax({
    type: 'post',
    dataType: 'json',
    data: {},
    url: 'https://xxx',
    success: function(text, xml){ // 请求成功后的回调函数
        console.log(text)
    },
    fail: function(status){ // 请求失败后的回调函数
        console.log(status)
    }
})
```


@[TOC](Axios 如何取消重複請求)

# 前言
在 Web 項目的開發過程中，我們常常會遇到重複請求的場景，而如果系統不對重複的請求進行處理，是有可能導致系統出現各種問題的。比如說，如果出現重複的 `POST` 請求就有可能導致服務端產生兩筆相同的紀錄，也就是一種冗余。那到底重複請求是怎麼產生的呢？這邊舉個例子感覺一下：

> 假設頁面中有一個 button，用戶點擊之後會發出一個 Ajax 請求。如果沒有對該 button 進行控制，那當用戶快速點擊該按鈕多次，就會發出重複請求。

反正就是說確實是會出現這樣的問題的，本篇也就是要來討論怎麼去解決這種問題。接下來將會以 `Axios` 為例進行說明，`Axios` 其實是對於 `Ajax` 的一個封裝，對於還不清楚的人可以先去看看 [Ajax 原理及手動封裝](https://blog.csdn.net/weixin_46803507/article/details/115451878)。

# 參考
|參考|鏈接|
|:-----|:-----|
|axios中文文档|http://www.axios-js.com/zh-cn/docs/index.html|
|axios cancelToken原理解析|https://blog.csdn.net/yishengsama/article/details/106416869|

# 正文

## 如何取消請求
`Axios` 其實在底層上是透過一個叫做 `XMLHttpRequest` 的東西發送 http 請求的，如果要取消請求，我們可以通過 `XMLHttpRequest` 提供的一個 `abort()` 方法。
```js
// Ajax

let xhr = new XMLHttpRequest()
xhr.open("GET", "https://mp.csdn.net/console/article", true)
xhr.send()
setTimeout(() => xhr.abort(), 3000)
```
上面是 `Ajax` 的作法，那對於 `Axios` 則可以通過`CancelToken` 來實現。
```js
const CancelToken = axios.CancelToken
const source = CancelToken.source()

axios.get('/user/123', {
  cancelToken: source.token
})

axios.post('/user/12345', {
  name: 'cclin'
}, {
  cancelToken: source.token
})

source.cancel('Operation canceled!'); // 取消请求，参数是可选的
```
這邊稍微注意下，使用 `GET` 的話是把 cancelToken 屬性放在第二個參數，`POST` 是把 cancelToken 屬性放在第三個參數。

此外，也可以通過調用 `CancelToken` 構造函數來實現：
```js
const CancelToken = axios.CancelToken
let cancel

axios.get('/user/123', {
  cancelToken: new CancelToken(function executor(c) {
    cancel = c
  })
})

cancel() // 取消請求
```

現在我們知道在 `Axios` 怎麼用 cancelToken 取消請求了，接下來看看如何判斷重複請求。

## 如何判斷重複請求
其實思路很簡單，當請求方式、請求 url、請求參數都一樣時，我們就可以認為請求重複了。因此在每次發起請求時，我們就可以根據當前請求的請求方式、請求 url 地址、和請求參數構成一個唯一的 `key`，同時為每個請求創建一個專屬的 `CancelToken`，然後把 `key` 跟 `CancelToken` 以 key-value 的形式放到一個 Map 中。放到 Map 是因為這樣可以很方便的判斷是否存在重複的請求。
```js
import qs from 'qs'

const CancelToken = axios.CancelToken
const pendingRequest = new Map()

// GET -> params
// POST -> data
const key = [
    method, url, 
    method === 'GET' ? qs.stringify(params) : qs.stringify(data)
].join('&')

const cancelToken = new CancelToken(function executor(cancel) {
    if(!pendingRequest.has(key)) {
        pendingRequest.set(key, cancel)
    }
})
```
當出現重複請求時，我們就可以用 `cancel` 函數來取消前面已經發出的請求，而在取消請求後，我們也要記得把取消的請求從 `pendingRequest` 中移除。ok，現在我們知道如何取消請求，也知道如何判斷重複請求，下面就來看看如何取消重複請求。

## 如何取消重複請求
我們需要對所有請求都進行處理，所以我們使用 Axios 提供的攔截器機制來實現如何取消重複請求的功能。Axios 的攔截器為開發者提供了請求攔截器以及響應攔截器，作用如下：

- 請求攔截器：在請求發送前統一執行某些操作，比如添加 token 字段(常用於用戶登陸狀態判斷)

- 響應攔截器：在接收到服務器響應後統一執行某些操作，比如發現響應狀態碼是 401 時，路由重定向到登陸頁

### 定義輔助函數
在配置攔截器之前，先來封裝 3 個輔助函數：

- `generateReqKey`：用於根據當前請求生成請求 key
```js
import qs from 'qs'

function generateReqKey(req) {
  const { method, url, params, data } = req
  return [method, url, qs.stringify(params), qs.stringify(data)].join("&")
}
```
- `addPendingRequest`：用於把當前請求 key 添加到 pendingRequest 對象中
```js
const pendingRequest = new Map()
function addPendingRequest(req) {
    const key = generateReqKey(req)
    req.cancelToken = req.cancelToken || new axios.CancelToken((cancel) => {
        if(!pendingRequest.has(key)) {
            pendingRequest.set(key, cancel)
        }
    })
}
```
- `removePendingRequest`：檢查是否存在重複請求，若存在則取消重複請求
```js
function removePendingRequest(req) {
    const key = generateReqKey(req)
    if(pendingRequest.has(key)) {
        const cancel = pendingRequest.get(key)
        cancel(key)
        pendingRequest.delete(key)
    }
}
```
### 設置請求攔截器
```js
axios.interceptors.request.use(
  (request) => {
    removePendingRequest(request) // 检查是否存在重复请求，若存在则取消已发的请求
    addPendingRequest(request) // 把当前请求信息添加到pendingRequest对象中
    return request
  },
  (error) => {
    return Promise.reject(error)
  }
)
```
### 設置響應攔截器
```js
axios.interceptors.response.use(
  (response) => {
    removePendingRequest(response.config) // 从pendingRequest对象中移除请求
    return response
  },
  (error) => {
    removePendingRequest(error.config || {}) // 从pendingRequest对象中移除请求
    if (axios.isCancel(error)) {
      console.log("已取消的重复请求：" + error.message)
    } else {
      // 添加异常处理
    }
    return Promise.reject(error)
  }
)
```
## 測試代碼 + 效果
- 測試代碼

```html
<!DOCTYPE html>
<html lang="zh-cn">
  <head>
    <meta charset="UTF-8" />
    <meta http-equiv="X-UA-Compatible" content="IE=edge" />
    <meta name="viewport" content="width=device-width, initial-scale=1.0" />
    <title>Axios 取消重复请求示例</title>
    <script src="https://cdn.bootcdn.net/ajax/libs/qs/6.9.6/qs.min.js"></script>
    <script src="https://cdn.bootcdn.net/ajax/libs/axios/0.21.1/axios.min.js"></script>
  </head>
  <body>
    <h3>Axios 取消重复请求示例</h3>
    <button onclick="sendRequest()">发起请求</button>
    <script>
      const pendingRequest = new Map();

      function generateReqKey(config) {
        const { method, url, params, data } = config;
        return [method, url, Qs.stringify(params), Qs.stringify(data)].join(
          "&"
        );
      }

      function addPendingRequest(config) {
        const requestKey = generateReqKey(config);
        config.cancelToken =
          config.cancelToken ||
          new axios.CancelToken((cancel) => {
            if (!pendingRequest.has(requestKey)) {
              pendingRequest.set(requestKey, cancel);
            }
          });
      }

      function removePendingRequest(config) {
        const requestKey = generateReqKey(config);
        if (pendingRequest.has(requestKey)) {
          const cancel = pendingRequest.get(requestKey);
          cancel(requestKey);
          pendingRequest.delete(requestKey);
        }
      }

      axios.interceptors.request.use(
        (request) => {
          removePendingRequest(request); // 检查是否存在重复请求，若存在则取消已发的请求
          addPendingRequest(request); // 把当前请求添加到pendingRequest对象中
          return request;
        },
        (error) => {
          return Promise.reject(error);
        }
      );

      axios.interceptors.response.use(
        (response) => {
          removePendingRequest(response.config); // 从pendingRequest对象中移除请求
          return response;
        },
        (error) => {
          removePendingRequest(error.config || {}); // 从pendingRequest对象中移除请求
          if (axios.isCancel(error)) {
            console.log("已取消的重复请求：" + error.message);
          } else {
            // 添加异常处理
          }
          return Promise.reject(error);
        }
      );

      async function sendRequest() {
        const response = await axios.get(
          "https://jsonplaceholder.typicode.com/todos",
          {
            params: {
              userId: 1,
            },
          }
        );
        console.log(response.data);
      }
    </script>
  </body>
</html>
```

- 測試效果

![](https://ftp.bmp.ovh/imgs/2021/04/3cb4747e9bb55107.gif)
![](https://ftp.bmp.ovh/imgs/2021/04/bf611ffa33d99241.png)

可以看到，如果過於頻繁的發送同一請求，或是說，在請求尚未處理完之前就又嘗試發送一個相同請求，通過以上作法，可以有效的避免這樣的問題。

最後通過一張圖讓大家更好的了解整個流程工作的機制：
![](https://ftp.bmp.ovh/imgs/2021/04/9f9747b31b43d969.png)

# 結語
本篇基於我自己最常用的 axios 介紹了關於取消重複請求的方式，其他第三方的庫應該也都有相應的 api 可以做到的，也附上完整的測試代碼和效果，應該還挺清楚的。感覺這其實是還算常見的業務要求，不過之前項目中都沒有加入進去，之後在項目中還要在實踐看看～希望看完這篇對大家多多少少有些幫助！


@[TOC](Python Web Flask後端開發)

# 前言
之所以開始學習 Flask 框架，主要是因為最近一個項目需求。因為我們]數據處理和服務都是使用 python 編寫的，又因為想要最後以網頁的方式呈現，增加用戶體驗，所以前後端的開發就不可避免。前端使用了 React.js 框架，但是後端呢?很自然的，我就希望也能用 python 編寫後端代碼，與數據層也更好的連接。於是就開始了學習 Flask 這個框架。這篇主要介紹 Flask 的用法，並說明自己是如何將 React 和 Flask 連接的。 

# 正文

## Flask 是甚麼 ?
Flask 是一個使用 Python 編寫的輕量級 Web 應用框架。基於 Werkzeug WSGI 工具箱和 Jinja2 模板引擎。簡單來說，就本人使用起來的感覺，Flask 是一個非常方便的後端框架，不需要過多繁瑣複雜配置。

## 使用 Flask
廢話不多說，直接來看看怎麼使用 Flask 吧!

### 安裝 Flask
當然，要使用 Flask 就要先安裝 python。具體在這邊就不多做攢述。
下載網址 : [https://www.python.org/downloads/](https://www.python.org/downloads/)

我使用的開發環境是 PyCharm IDE。
接著就要來安裝 Flask 基本套件。
```
pip install Flask
```
到這邊就都安裝完畢啦!是不是很簡單呢?

### Flask 基本使用
如果尚未了解 React 的，可以先去了解。
官方文件: [React 官方教程](https://react.docschina.org/)

先來看看一個基本的 Flask 例子:
```py
from flask import Flask
app = Flask(__name__)

@app.route("/")
def func():
    return "Hello World"

if __name__ == "__main__":
    app.run(debug=True)
```
上面就是基本的 Flask 後端代碼。app 就是我們建立的 flask 對象，用於最後在 main 中運行起後端 flask 服務器。

其中，`@app.route("...")`中就是要寫上相應的 URL。這邊先提下，Flask 服務器監聽的是本地 localhost 的 5000 端口，所以到時候在前端，要將請求發送到端口 5000 加上相應匹配的 URL。

再接下去之前，如果還沒安裝 Axios 的人可以先安裝下。Axios 是一個基於 Promise 的 HTTP 客戶端，可以協助我們將前後端相連。
- 安裝 Axios
```
npm install axios
```

## React + Flask + Axios 實際案例
我自己的項目需求是，我想要將頁面上用戶對輸入框輸入的值傳給 Flask 後端做完處理後，再將結果回傳至前端，以顯示在頁面上。

### Flask 後端代碼
```py
from flask import Flask, jsonify
app = Flask(__name__)

# solve CORS problem
def after_request(resp):
    resp.headers['Access-Control-Allow-Origin'] = '*'
    return resp

app.after_request(after_request)

@app.route("/Echartreport/<student_id>")
def Echartreport(student_id):
    # print(student_id)
    overall_student_value = []
    specific_student_value = []

    if student_id == "student_id=":
        error = {
            'message' : 'Invalid Input'
        }
        return jsonify(error)
    else:
        '''
        顺序如下：
        1. 代码估计相似度
        2. 代码时间复杂度
        3. 代码风格水平
        4. 代码空间复杂度
        '''
        overall_student_value.append(220)
        overall_student_value.append(410)
        overall_student_value.append(398)
        overall_student_value.append(400)

        specific_student_value.append(120)
        specific_student_value.append(290)
        specific_student_value.append(287)
        specific_student_value.append(320)
        Echartreport = {
            'message' : 'Valid Input',
            'overall_student_value' : overall_student_value,
            'specific_student_value' : specific_student_value,
        }
        return jsonify(Echartreport)


if __name__ == "__main__":
    app.run(debug=True)
```
這邊針對這段 Flask 後端代碼做一些講解:
- 這邊多引入了一個 `jsonify`模塊。用途就是在最後將要返回的數據 json 化，方便前端做處理。
- 注意到 `@app.route("/Echartreport/<student_id>")` 和上面介紹的 URL 寫法不同。尖括號中的表示的是 URL 動態的參數，通常由前端發向後端。
- 最後解釋下 #solve CORS problem 注釋下的 `after_request` 方法。這是為了解決前後端的跨域問題。

### React + Axios 前端代碼
這邊沒有將整個 React 組件代碼附上，僅給出前後端交互的方法代碼。
```js
import axios from 'axios'

get_overall_report(){
        let student_id = document.getElementById("student-id1")
        let student_id_val = String(student_id.value)
        axios.get(`http://localhost:5000/Echartreport/student_id=${student_id_val}`)
        .then(response => {
            let data = response.data
            console.log(data)
            if(data.message === "Invalid Input") {
                message.warning("学生id不可为空!")
            }else if(data.message === "Valid Input") {
                this.setState({
                    overall_student_value : data.overall_student_value,
                    specific_student_value : data.specific_student_value,
                })
            }
        })
        .catch(error => {
            console.log(error)
        })
    }
```
這邊針對這段 React 前端代碼做一些講解:
- 看到前後端適用 axios 連接的。使用 axios 發送一個 get 請求到該端口希望匹配的接口 URL，並用 then 和 catch 接回傳的 promise。若對 promise 尚不了解的，歡迎先去看看 [搞懂 ES6 JavaScript 中的 Promise](https://blog.csdn.net/weixin_46803507/article/details/106742492)


# 結語
這只是本來對於 Flask 的使用一點基本粗淺的理解，後續也將再更新更深入的博客。希望對於同樣是 Flask 書學這的你有些許幫助，也歡迎各位大神們多多指教 !

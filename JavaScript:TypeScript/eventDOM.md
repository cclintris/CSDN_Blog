@[TOC](關於 JavaScript 的事件流)

# 前言
再看這篇前，建議各位可以先去看看 [關於 JavaScript 事件循環](https://blog.csdn.net/weixin_46803507/article/details/113706311)，當然對於已經了解的人們就不用啦!這篇要討論的是 js 的事件流。

# 正文
都知道， 當我們在網頁上做進行某些類型的操作，例如點擊，滑動等等，都會觸發一些相應的事件。也知道，整個網頁的構成其實會被瀏覽器解析成一棵 DOM 樹。而當一個節點產生一個事件時，該事件會在該節點與根結點之間按一定順序傳播，而這個傳播路徑上經過的所有結點都會接收到該事件，這一整個過程就稱為 ==DOM事件流==。

## 甚麼是事件?
js 與 html 之間的交互其實就是通過「事件」實現的。舉凡用戶對網頁的點擊，選定，滑動等等，在 js 的世界中全部都是事件。

而對於事件，當事件發生就要有響應，在 js 中，所謂的響應就是監聽器。就像觀察者模式一樣，事件就是我們的 subject，而當事件發生時，就要通知對應該事件(subject)的所有監聽器(listener)去執行相對的響應。

## 甚麼是事件流?
事件流描述的就是從頁面中接收事件的順序。主要分為以下兩種:
- IE 的事件冒泡
- Netscape 的事件捕獲

## 事件冒泡 vs 事件捕獲
IE 提出的事件流模型是事件冒泡的，就是從下到上，從目標觸發的元素逐級向上傳播，一直到 document 對象。
![](https://wtfhhh.oss-cn-beijing.aliyuncs.com/dom-1.png)

Netscape 提出的事件流模型則是事件捕獲的，與事件冒泡相反，是從上到下的，即從 document 對象逐級向下傳播到目標對象。
![](https://wtfhhh.oss-cn-beijing.aliyuncs.com/dom-2.png)

以上是 DOM0 標準下的事件流機制。後來 ECMAScript 在 DOM2 中對事件流做了進一步的規範。在 DOM2 中，一個事件所包含的事件流分為以下 3 個階段:
1. 事件捕獲階段(capture)
2. 目標階段(target)
3. 事件冒泡階段(bubble)
![](https://wtfhhh.oss-cn-beijing.aliyuncs.com/dom-3.png)

## DOM事件分級
DOM節點有了事件發生，當然就需要做相應的處裡，而 DOM 事件又分為 4 個等級，如下:
![](https://wtfhhh.oss-cn-beijing.aliyuncs.com/dom-4.png)
其中，比較重要的是 DOM0/DOM2，所以下面著重介紹。

## DOM0
DOM0 級事件主要有兩種實現方式，第一種是內聯模型，就是直接將函數名作為 html 標籤中事件屬性的屬性值。如下:
```js
// js code
// eventDOM.js

function btnClick() {
    console.log('Hello World')
}
```

```html
<!-- html code -->

<html>
    <head>
        <title>eventDOM demo</title>
    </head>
    <body>
        <div onclick="btnClick()" style="width: 30px; height: 30px; background-color:black"></div>
        <script type="text/javascript" src="eventDOM.js"></script>
    </body>
</html>
```
但是內聯模型有明顯的缺點，就是它違反了 W3C 對內容(html)和行為(js)分離的要求。所以有了第二種，就是腳本模型(動態綁定模型)。具體作法是通過 js 腳本選定特定 DOM 節點，然後對該節點添加事件屬性以及屬性值。如下:
```js
// js code
// eventDOM.js

let btn = document.getElementById('btn')

let btnClick = function() {
    console.log('Hello World')
}

btn.onclick = btnClick
```

```html
<!-- html code -->

<html>
    <head>
        <title>eventDOM demo</title>
    </head>
    <body>
        <div id="btn" style="width: 30px; height: 30px; background-color:black"></div>
        <script type="text/javascript" src="eventDOM.js"></script>
    </body>
</html>
```
點擊會出現 `Hello World`，沒問題。但腳本模型一樣有缺點，基於上面的 html 代碼，添加一點 js，如下:
```js
// js code
// eventDOM.js

let btn = document.getElementById('btn')

let btnClick = function() {
    console.log('Hello World')
}

let btnClick2 = function() {
    console.log('Hello World again')
}

btn.onclick = btnClick
btn.onclick = btnClick2
```
我們發現，現在點擊只會出現 `Hello World again`。所以腳本模型只允許一個節點添加一次同類型事件，後面的會覆蓋前面的。

最後讓我們再來看一個有趣的例子:
```html
<!-- html code -->

<html>
    <head>
        <title>eventDOM demo</title>
    </head>
    <body>
        <div id="btn3" style="width: 400px; height: 400px; background-color:pink">
            btn3
            <div id="btn2" style="width: 300px; height: 300px; background-color:skyblue">
                btn2
                <div id="btn1" style="width: 200px; height: 200px; background-color:lightgreen">
                    btn1
                </div>
            </div>
        </div>
        <script type="text/javascript" src="eventDOM.js"></script>
    </body>
</html>
```

```js
// js code
// eventDOM.js

let btn1 = document.getElementById('btn1')
let btn2 = document.getElementById('btn2')
let btn3 = document.getElementById('btn3')

btn1.onclick = function() {
    console.log('1')
}

btn2.onclick = function() {
    console.log('2')
}


btn3.onclick = function() {
    console.log('3')
}
```
![](https://wtfhhh.oss-cn-beijing.aliyuncs.com/dom-5.png)

當我們點擊 btn3 時，輸出如下:
![](https://wtfhhh.oss-cn-beijing.aliyuncs.com/dom-6.png)
符合預期，但如果我們點擊 btn1 的話呢?輸出如下:
![](https://wtfhhh.oss-cn-beijing.aliyuncs.com/dom-7.png)
這就有點奇怪了，明明我們為 btn1 只添加了一個監聽器阿，為什麼連同 btn2,btn3 的一起加上去了的感覺?原因就是因為，雖然 DOM0 有 IE 提出的事件冒泡以及 Netscape 提出的事件捕獲兩種模型，但其實 DOM0 只支持事件冒泡。所以，點擊 btn1 的事件流如下:
![](https://wtfhhh.oss-cn-beijing.aliyuncs.com/dom-8.png)
也就是事件流也經過了 btn2,btn3，所以才會觸發他們的事件處理。但是顯然，這並不是我們想要的結果。

## DOM2
進一步規範後，有了 DOM2級的事件處理程序。其中定義了兩個方法:
- addEventListener() 添加事件監聽器
- removeEventListener() 移除事件監聽器

這兩個函數均有三個參數如下表:
|參數|類型|描述|
|:-----|:-----|:-----|
|event|String|監聽的事件名稱，比如'click'。注意這邊都不需要"on"|
|callback|function|觸發事件所要執行的回調函數|
|useCapture|Boolean(default:false)|事件是否在捕獲階段進行處理|

DOM2 中就可以對同一個節點綁定兩個以上的同類型事件監聽器了，看看下面例子:
```html
<!-- html code -->

<html>
    <head>
        <title>eventDOM demo</title>
    </head>
    <body>
        <div id="btn" style="width: 200px; height: 200px; background-color:lightgreen"></div>
        <script type="text/javascript" src="eventDOM.js"></script>
    </body>
</html>
```

```js
// js code
// eventDOM.js

let btn = document.getElementById('btn')

function hello() {
    console.log("Hello World")
}

function helloAgain() {
    console.log("Hello World again")
}

btn.addEventListener('click', hello, false)
btn.addEventListener('click', helloAgain, false)
```
輸出如下:
![](https://wtfhhh.oss-cn-beijing.aliyuncs.com/dom-9.png)
解決了 DOM0 只能綁定一個同類型事件監聽器的缺點啦!值得注意的是，如果綁定同樣的監聽器兩次以上，仍然只會觸發一次。

再回到上面三個 div 的那個例子進行改寫如下:
```html
<!-- html code -->

<html>
    <head>
        <title>eventDOM demo</title>
    </head>
    <body>
        <div id="btn3" style="width: 400px; height: 400px; background-color:pink">
            btn3
            <div id="btn2" style="width: 300px; height: 300px; background-color:skyblue">
                btn2
                <div id="btn1" style="width: 200px; height: 200px; background-color:lightgreen">
                    btn1
                </div>
            </div>
        </div>
        <script type="text/javascript" src="eventDOM.js"></script>
    </body>
</html>
```

```js
// js code
// eventDOM.js

let btn1 = document.getElementById('btn1')
let btn2 = document.getElementById('btn2')
let btn3 = document.getElementById('btn3')

btn1.addEventListener('click', function() {
    console.log('1')
}, true)

btn2.addEventListener('click', function() {
    console.log('2')
}, true)

btn3.addEventListener('click', function() {
    console.log('3')
}, true)
```

注意，這邊我們把 `addEventListener` 的第三個參數設為 true，也就是事件會在捕獲階段觸發。點擊 btn1 輸出如下:
![](https://wtfhhh.oss-cn-beijing.aliyuncs.com/dom-10.png)
看到順序與 DOM0 的順序反過來了。首先最外層(btn3)的節點先被觸發了，而因為第三個參數被設為 true，事件會在捕獲階段就被處理，所以輸出才會是 3,2,1。如果都是 false，就會是 1,2,3。

可見 DOM2 的事件處理機制有了更彈性的操作空間。我們也可以在不同階段綁定事件監聽器，看看下面例子:
```html
<!-- 沿用上一段 html 代碼 -->
```

```js
// js code
// eventDOM.js

btn1.addEventListener('click',function() {
    console.log('btn1 capture')
}, true)
btn1.addEventListener('click',function() {
    console.log('btn1 bubble')
}, false)

btn2.addEventListener('click',function() {
    console.log('btn2 capture')
}, true)
btn2.addEventListener('click',function() {
    console.log('btn2 bubble')
}, false)

btn3.addEventListener('click',function() {
    console.log('btn3 capture')
}, true)
btn3.addEventListener('click',function() {
    console.log('btn3 bubble')
}, false)
```
點擊 btn1 輸出如下:
![](https://wtfhhh.oss-cn-beijing.aliyuncs.com/dom-11.png)

改變一下順序，如下:
```html
<!-- 沿用上一段 html 代碼 -->
```

```js
// js code
// eventDOM.js

btn1.addEventListener('click',function() {
    console.log('btn1 bubble')
}, false)
btn1.addEventListener('click',function() {
    console.log('btn1 capture')
}, true)

btn2.addEventListener('click',function() {
    console.log('btn2 bubble')
}, false)
btn2.addEventListener('click',function() {
    console.log('btn2 capture')
}, true)

btn3.addEventListener('click',function() {
    console.log('btn3 bubble')
}, false)
btn3.addEventListener('click',function() {
    console.log('btn3 capture')
}, true)
```
點擊 btn1 輸出如下:
![](https://wtfhhh.oss-cn-beijing.aliyuncs.com/dom-12.png)
注意 btn1 的輸出。雖然捕獲階段先發生了，但是因為 btn1 本身就是目標節點，所以在這種情況下，總結出規律:在目標元素上不區分冒泡還是捕獲，是根據腳本中的順序來執行。

有時候，我們希望對於某節點，不要再經過冒泡階段了，DOM2 也提供了相應函數，`stopPropagation`。
```html
<!-- 沿用上一段 html 代碼 -->
```

```js
// js code
// eventDOM.js

btn1.addEventListener('click',function() {
    console.log('btn1 capture')
}, true)
btn1.addEventListener('click',function() {
    console.log('btn1 bubble')
}, false)

btn2.addEventListener('click',function(e) {
    e.stopPropagation()
    console.log('btn2 capture')
}, true)
btn2.addEventListener('click',function() {
    console.log('btn2 bubble')
}, false)

btn3.addEventListener('click',function() {
    console.log('btn3 capture')
}, true)
btn3.addEventListener('click',function() {
    console.log('btn3 bubble')
}, false)
```
點擊 btn1 輸出如下:
![](https://wtfhhh.oss-cn-beijing.aliyuncs.com/dom-14.png)
可以看到，因為我們在 btn2 的捕獲階段就阻止了 btn2 的冒泡階段，所以 btn2 在捕獲後就不再繼續執行下去，確保不會冒泡，事件流如下:
![](https://wtfhhh.oss-cn-beijing.aliyuncs.com/dom-15.png)

加強一下前面提到的知識點，如果是在 btn1 阻止冒泡，會變成怎樣呢?
```html
<!-- 沿用上一段 html 代碼 -->
```

```js
// js code
// eventDOM.js

btn1.addEventListener('click',function(e) {
    e.stopPropagation()
    console.log('btn1 capture')
}, true)
btn1.addEventListener('click',function() {
    console.log('btn1 bubble')
}, false)

btn2.addEventListener('click',function() {
    console.log('btn2 capture')
}, true)
btn2.addEventListener('click',function() {
    console.log('btn2 bubble')
}, false)

btn3.addEventListener('click',function() {
    console.log('btn3 capture')
}, true)
btn3.addEventListener('click',function() {
    console.log('btn3 bubble')
}, false)
```
點擊 btn1 輸出如下:
![](https://wtfhhh.oss-cn-beijing.aliyuncs.com/dom-16.png)
雖然我們對 btn1 阻止了冒泡，但是為什麼還是輸出了 `btn bubble`呢?原因就是前面提到了，目標節點不區分 捕獲/冒泡 階段，但是後面也就不會繼續冒泡了，算是個比較特殊的情況，可以稍微留意下。

# 結語
本篇大致介紹了 js 的事件流的各種模型以及階段上的工作任務，個人認為應該還算詳細。雖然個人感覺好像對編程本身沒有太明顯的幫助，但是還是算是 js 的一個重要的知識點，學習下也沒甚麼不好。若內容有誤，還歡迎指點!


@[TOC](關於 HTML5 的 canvas 標籤)

# 前言
學習 canvas 的原因是因為本人之前為了測試一些 css 屬性，所以很 low 的用 div 之類的東西，還調了老半天才做出一個很簡單的圖形畫面。發覺太過麻煩且無腦，於是找到了 canvas 標籤，似乎是個值得學習的知識點。

# 正文

## canvas 是甚麼
`<canvas>` 標籤是 HTML5 推出的新標籤。簡單來說，它是一個允許我們使用腳本語言 (通常為 JavaScript) 在其中繪製圖像。我們可以想成說，`<canvas>` 就是定義出一塊可以繪製的區域，具體怎麼畫，由 JavaScript 來實現。

> Mozilla 程式從 Gecko 1.8 (Firefox 1.5) 開始支持 `<canvas>`， Internet Explorer 從 IE9 開始支持`<canvas>`。Chrome 和 Opera 9+ 也支持  `<canvas>`。

## canvas 基本使用

### canvas 元素
```html
<canvas id="canvas"></canvas>
```
首先當然得先在 html 中寫上 `<canvas>` 標籤。另外，`<canvas>` 還有額外的屬性，width 和 height，單位為 px，可以設定這塊畫布的長寬。(默認是 300 x 150)

### 渲染上下文 (Rending Context)
上面利用 `<canvas>` 創建出了一塊畫布，那麼現在還缺少的是畫筆。`<canvas>` 需要公開一個渲染上下文，其實就是畫筆，利用這個渲染上下文來繪圖。

```js
let canvas = document.getElementById('canvas')
// 獲取 2d 畫筆
let ctx = canvas.getContext('2d')
```

### 檢測支持性
因為我們知道，不是每一個瀏覽器都支持 `<canvas>`，所以在使用 canvas 的畫布和畫筆 (渲染上下文) 前，應該還要先檢測一下這塊畫布是否真的有效。類似代碼如下:

```js
// canvas 畫布
let canvas = document.getElementById('canvas')

if(canvas.getContext) {
    // 支持 canvas，開始繪圖
    // 獲取 2d 畫筆
    let ctx = canvas.getContext('2d')
}else{
    // 不支持 canvas
}
```

## canvas 繪圖概念

### 座標空間
我們使用 `<canvas>` 建立的一塊畫布，事實上可以想成一個座標系統。只不過在 canvas 中，y 軸和我們平常的座標系統顛倒。左上角為原點 (0,0)，而我有區塊在這個 canvas 中的座標都是相對於這個原點的。舉例來說，藍色區域座標為 (x,y)。
![](https://wtfhhh.oss-cn-beijing.aliyuncs.com/canvas.png)

### 繪製矩形
canvas 只支持一種圖形繪製 : 矩形。你會想那 canvas 太爛了吧，只能畫矩形。別急，下面還會介紹 path 的概念，使我們可以繪製更複雜的圖形，但是現在先來看看怎麼用 canvas 繪製矩形吧!

canvas 提供了 3 種針對矩形的方法 :

1. `fillRect(x, y, width, height)` : 繪製填充矩型

2. `strokeRect(x, y, width, height)` : 繪製只帶有邊框的矩形

3. `clearRect(x, y, width, height)` : 清除矩形區域，變為透明

```js
let drawRec = () => {
    let canvas = document.getElementById('canvas-1')
    if(!canvas.getContext) {
        return
    }else{
        let ctx = canvas.getContext('2d')
        ctx.fillRect(10, 10, 100, 50)
        ctx.strokeRect(10, 70, 100, 50)
        ctx.clearRect(15, 15, 50, 25)
    }
}

drawRec()
```
![](https://wtfhhh.oss-cn-beijing.aliyuncs.com/canvas-1.png)

### 繪製路徑 (path)
上面介紹了基本繪製矩形的方式，接下來還看看怎麼利用 path 繪製更多的形狀。首先介紹 path 的概念和幾個關於 path 的指令。

> 要想形成一個圖形，我們可以想成一個圖形其實是由多條 path 構成的一個封閉區域。

canvas 提供了以下針對 path 的方法 :

1. `beginPath()` : 新建一條路徑，畫筆被用作 path
2. `moveTo(x, y)` : 畫筆被移到 (x,y)
3. `lineTo(x, y)` : 繪製一條到 (x,y) 的 path
4. `closePath()` : 結束該路徑
5. `stroke()` : 通過線條描繪圖形輪廓
6. `fill()` : 填充圖形所包含區域

現在馬上來應用看看，我們來畫三角形。
```js
let drawTri = () => {
    let canvas = document.getElementById('canvas-2')
    if(!canvas.getContext) {
        return
    }else{
        let ctx = canvas.getContext('2d')
        ctx.beginPath()
        ctx.moveTo(50, 50)
        ctx.lineTo(200, 50)
        ctx.lineTo(200, 100)
        ctx.closePath()
        ctx.stroke()
    }
}

drawTri()
```
![](https://wtfhhh.oss-cn-beijing.aliyuncs.com/canvas-2.png)

你可能會想說，不是只畫了兩條線嗎 ? 是的，但是 closePath() 會幫我們閉合整個區域了，所以不用擔心。

### 繪製圓弧 (arc)
除了能畫直線，canvas 當然也考慮到了畫圓弧的功能。

canvas 提供了 2 個針對圓弧的方法 :

1. `arc(x, y, r, startAngle, endAngle, anticlockwise)` : 以 (x,y) 為圓心，半徑為 r，從 startAngle 弧度到 endAngle 弧度結束。anticlockwise 是個 bool，true 表示逆時針，false 表示順時針
2. `arcTo(x1, y1, x2, y2, radius)` : 根據 (x1, y1), (x2, y2) 和半徑畫一段圓弧，最後再以直線連接兩個點。

#### arc 例子 1
```js
let drawArc1 = () => {
    let canvas = document.getElementById('canvas-3')
    if(!canvas.getContext) {
        return
    }else{
        let ctx = canvas.getContext('2d')
        ctx.beginPath()
        ctx.arc(50, 50, 40, 0, Math.PI / 2, true)
        ctx.stroke()
    }
}
```
![](https://wtfhhh.oss-cn-beijing.aliyuncs.com/canvas-3.png)

#### arc 例子 2
```js
let drawArc2 = () => {
    let canvas = document.getElementById('canvas-4')
    if(!canvas.getContext) {
        return
    }else{
        let ctx = canvas.getContext('2d')
        ctx.beginPath()
        ctx.arc(50, 50, 40, 0, Math.PI, false)
        ctx.fill()
    }
}

drawArc2()

let drawArc3 = () => {
    let canvas = document.getElementById('canvas-4')
    if(!canvas.getContext) {
        return
    }else{
        let ctx = canvas.getContext('2d')
        ctx.beginPath()
        ctx.arc(150, 50, 40, -Math.PI / 2, Math.PI / 2, true)
        ctx.closePath()
        ctx.stroke()
    }
}

drawArc3()

let drawArc4 = () => {
    let canvas = document.getElementById('canvas-4')
    if(!canvas.getContext) {
        return
    }else{
        let ctx = canvas.getContext('2d')
        ctx.beginPath()
        ctx.arc(200, 60, 30, 0, -Math.PI / 2, true)
        ctx.closePath()
        ctx.stroke()
    }
}

drawArc4()
```
![](https://wtfhhh.oss-cn-beijing.aliyuncs.com/canvas-4.png)

#### arc 例子 3
```js
let drawArc5 = () => {
    let canvas = document.getElementById('canvas-5')
    if(!canvas.getContext) {
        return
    }else{
        let ctx = canvas.getContext('2d')
        ctx.beginPath()
        ctx.moveTo(20,20)
        ctx.arcTo(20, 100, 100, 100, 80)
        ctx.stroke()
    }
}

drawArc5()
```
![](https://wtfhhh.oss-cn-beijing.aliyuncs.com/canvas-5.png)

`arcTo()` 的用法可以理解為這條弧線是以兩條切線構成的。
- 第一條切線 : 起點和控制點1
- 第二條切線 : 控制點1和控制點2
> 所繪製出來的線其實就是這兩條線鄉切的圓弧。

![](https://wtfhhh.oss-cn-beijing.aliyuncs.com/canvas-6.png)

### 添加 canvas 樣式
上面畫過的圖形無論是填充顏色或是圖形輪廓都是黑色的，其實黑色是 canvas 的默認色彩。canvas 也提供了 2 個屬性讓我們可以自定義色彩。

1. `fillStyle = 'color'` : 設置圖形填充色彩
2. `strokeStyle = 'color'` : 設置圖形輪廓色彩

接下來馬上來看看怎麼用吧!

#### fillStyle
```js
let canvasFillStyle = () => {
    let canvas = document.getElementById('canvas-6')
    if(!canvas.getContext) {
        return
    }else{
        let ctx = canvas.getContext('2d')
        ctx.fillStyle = 'blue'
        ctx.fillRect(30, 30, 100, 70)
    }
}

canvasFillStyle()
```
![](https://wtfhhh.oss-cn-beijing.aliyuncs.com/canvas-7.png)

#### strokeStyle
```js
// 隨機返回 [to, from] 之間的一個數
function randomInt(from, to) {
    return parseInt(Math.random() * (to - from + 1) + from)
}

let canvasStrokeStyle = () => {
    let canvas = document.getElementById('canvas-7')
    if(!canvas.getContext) {
        return
    }else{
        let ctx = canvas.getContext('2d')
        for (let i = 0; i < 3; i++){
            for (let j = 0; j < 6; j++){
                ctx.strokeStyle = `rgb(${randomInt(0, 255)},${randomInt(0, 255)},${randomInt(0, 255)})`;
                ctx.strokeRect(j * 50, i * 50, 30, 30);
            }
        }
    }
}

canvasStrokeStyle()
```
![](https://wtfhhh.oss-cn-beijing.aliyuncs.com/canvas-8.png)

### 繪製文本
canvas 提供 2 種方法來繪製文本:

1. `fillText(text, x, y, [, maxWidth])` : 在 (x,y) 填充 text 文本
2. `strokeText(text, x, y, [, maxWidth])` : 在 (x,y) 繪製 text 文本外框

```js
let drawText1 = () => {
    let canvas = document.getElementById('canvas-8')
    if(!canvas.getContext) {
        return
    }else{
        let ctx = canvas.getContext('2d')
        ctx.font = "30px sans-serif"
        ctx.fillText("canvas", 20, 50)
        ctx.strokeText("canvas", 20, 100)
    }
}

drawText1()
```
![](https://wtfhhh.oss-cn-beijing.aliyuncs.com/canvas-9.png)

#### 添加文本樣式
我們要用 canvas 繪製文本時，canvas 也提供了一些屬性來為文本添加樣式。

1. `font` = "[size] [font-family]"
2. `text=align` = "value"。可選值有 `start`,`end`,`right`,`left`,`center`。默認 
3. `direction` = "value"。可選值有 `ltr`,`rtl`,`inherit`。

### canvas 狀態機制
有時候我們會希望暫時保留當前 canvas 的快照，類似於保留當前畫面圖形的所有狀態。對此，canvas 提供了 2 個方法。

1. `save()` : 每調用 save 一次，就會把當前 canvas 的狀態壓棧，相當於數組的 push
2. `restore()` : 每調用 restore 一次，棧頂的 canvas 狀態，也就是當前的狀態就會從棧頂被彈出。恢復到前一次的狀態，相當於數組的 pop

### canvas 變形

#### translate
`translate(x, y)` 是一個方法，使 canvas 的原點移動到 (x, y)。
![](https://wtfhhh.oss-cn-beijing.aliyuncs.com/canvas-10.png)

通常在使用 `translate(x, y)` 時，都會鼓勵要對狀態做保存。來看看下面的例子:

```js
let translate1 = () => {
    let canvas = document.getElementById('canvas-9')
    if(!canvas.getContext) {
        return
    }else{
        let ctx = canvas.getContext('2d')
        ctx.save() // 保留狀態
        ctx.translate(70,70) // 原點被移置 (70, 70)
        ctx.strokeRect(0, 0, 50, 50)
        ctx.restore() // 恢復狀態，// 原點恢復至 (0, 0)
        ctx.fillRect(0, 0, 50, 50)
    }
}

translate1()
```
![](https://wtfhhh.oss-cn-beijing.aliyuncs.com/canvas-11.png)

#### rotate
有時候我們也會想要對圖形做一些旋轉，為此 canvas 提供了一個方法:

1. `rotate(angle)` : 這個方法是以座標圓點，順時針旋轉，angle 單位為弧度
![](https://wtfhhh.oss-cn-beijing.aliyuncs.com/canvas-12.png)

```js
let rotate1 = () => {
    let canvas = document.getElementById('canvas-10')
    if(!canvas.getContext) {
        return
    }else{
        let ctx = canvas.getContext('2d')

        ctx.fillStyle = 'red'
        ctx.save()

        ctx.translate(100, 50)
        ctx.rotate(Math.PI / 4)
        ctx.fillStyle = 'blue'
        ctx.fillRect(0, 0, 60, 60)
        
        ctx.restore()
        ctx.fillRect(0, 0, 60, 60)
    }
}

rotate1()
```
![](https://wtfhhh.oss-cn-beijing.aliyuncs.com/canvas-15.png)

#### scale
有時我們想對現有的圖形放到縮小一定的比例。canvas 提供了一個方法:

1. `scale(x, y)` : x, y 分別表示相對於 x, y 軸的縮放比例。且值必須大於 0，1.0 為分界，小於 1.0 縮小，大於 1.0 則為放大。

```js
let scale1 = () => {
    let canvas = document.getElementById('canvas-11')
    if(!canvas.getContext) {
        return
    }else{
        let ctx = canvas.getContext('2d')
        ctx.translate(30, 30)
        ctx.scale(2.0, 0.5)
        ctx.fillRect(0, 0, 50, 50)
    }
}

scale1()
```
![](https://wtfhhh.oss-cn-beijing.aliyuncs.com/canvas-16.png)

可以看到因為多了 `scale(2.0, 0.5)`，所以在 X 方向上變為 2 倍長，在 Y 方向上縮為 1/2。

#### composite
回想一下上面的例子，我們可以看到，如果發生圖片重疊時，都會將重疊部分顯示新的區塊的樣式。這邊要介紹一個屬性 `globalCompositeOperation`，可以對於圖形重疊時有更多的變化。

> 語法: `globalCompositeOperation = 'value'`

接下來就來看看 canvas 提供了哪些屬性吧!

|value|
|:---------:|
|source-over (default)|
|source-in|
|source-out|
|source-atop|
|destination-over|
|destination-in|
|destination-out|
|destination-atop|
|lighter|
|darken|
|lighten|
|xor|
|copy|

因為本來懶惰要展示的話實在太多圖片了，所以在這邊就不特別展示。大家也可以自己試試，composite 這部分還是比較值觀容易的。

# 結語
終於寫完了關於 HTML5 這個 `canvas` 標籤啦!主要因為知識點有點多，寫起來還是挺麻煩的。對於 `canvas`的使用，或許還得自己嘗試起來才更直接點。希望對於不了解 `canvas` 的各為多少有點幫助，也希望各位大神們若有看到錯誤，也不吝於給小白些指點。


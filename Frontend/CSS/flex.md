@[TOC](CSS3-Flex 彈性盒子布局)

# 前言
前幾天寫了一篇關於 CSS3 Grid 格線布局，覺的應該也要也來搞明白 CSS3 Flex 彈性盒子布局。前情提要，其實對於 flex 和 grid 布局，共同之處就是它們都有外部容器以及內元素的概念。

# 正文
既然都知道 flex 布局有分外部容器跟內元素，那不妨先來看看都有哪些屬性!

## flex 屬性一總攬

### 外部容器
|屬性|描述|
|:----------:|:-----------:|
|display|指明布局系統|
|flex-direction|內元素排列方向|
|flex-wrap|內元素超過範圍是否換行|
|justify-content|水平對齊方式(主軸軸對齊方式)|
|align-items|單行垂直對齊方式(交錯軸對齊方式)|
|align-content|多行垂直對齊方式(交錯軸對齊方式)|


### 內元素
|屬性|描述|
|:----------:|:-----------:|
|order|單個元素的排列順序|
|align-self|該元素的垂直對齊方式|
|flex-grow|該元素佔用容器剩餘的比例|
|flex-shrink|該元素壓縮比例|
|flex-basis|該元素的基準值|

有了初步概念，接下來一一來看每個屬性的效果以及 flex 布局的整體概念吧!

## 外部容器

### display
這個屬性相信不用多說了，用意就只是指定布局系統而已。

```css
.box {
    display: flex
}
```

### axis (主軸)
在 flex 布局的世界裡，首先要知道，有兩個軸，分別是主軸 (main-axis) 和交錯軸 (cross-axis)。看看下面的圖就可以清楚理解了:

![](https://wtfhhh.oss-cn-beijing.aliyuncs.com/flex-1.png)

### flex-direction
flex-direction 就是用來改變上面軸線的方向。有四種取值。

```css
.box {
    display: flex;
    flex-direction: row | row-reverse | column | column-reverse 
}
```

```flex-direction: row```

![](https://wtfhhh.oss-cn-beijing.aliyuncs.com/flex-2.png)

```flex-direction: row-reverse```

![](https://wtfhhh.oss-cn-beijing.aliyuncs.com/flex-5.png)

```flex-direction: column```

![](https://wtfhhh.oss-cn-beijing.aliyuncs.com/flex-4.png)

```flex-direction: column-reverse```

![](https://wtfhhh.oss-cn-beijing.aliyuncs.com/flex-6.png)

### flex-wrap
flex-wrap 用於如果葉面寬度不夠的時候，是否換行。分為換行，不換行，換行後反轉。

```css
.box {
    display: flex;
    flex-wrap: wrap | nowrap | wrap-reverse
}
```
![](https://wtfhhh.oss-cn-beijing.aliyuncs.com/flex-7.png)


### flex-flow
flex-flow 就只是把 flex-direction 跟 flex-wrap 寫在一起而已。

```css
.box {
    display: flex;
    flex-flow: <flex-direction> || <flex-wrap>
}
```

### justify-content
這個屬性可以理解為內元素對於主軸的對齊設定。這個屬性有六個取值。

```css
.box {
    display: flex;
    justify-content: flex-start | flex-end | center | space-around | space-between | space-evenly
}
```
接著來看看各自的效果吧!

`flex-start` : 靠左對齊

![](https://wtfhhh.oss-cn-beijing.aliyuncs.com/flex-8.png)

`flex-end` : 靠右對齊

![](https://wtfhhh.oss-cn-beijing.aliyuncs.com/flex-9.png)

`center` : 置中對齊

![](https://wtfhhh.oss-cn-beijing.aliyuncs.com/flex-10.png)

下面三個都是分散對齊，只不過有些差別。

`space-around` : 左右邊有留間距，最旁邊的間距為內間距的一半

![](https://wtfhhh.oss-cn-beijing.aliyuncs.com/flex-11.png)

`space-between` : 左右邊不留間距 

![](https://wtfhhh.oss-cn-beijing.aliyuncs.com/flex-12.png)

`space-evenly` : 所有間距一致 

![](https://wtfhhh.oss-cn-beijing.aliyuncs.com/flex-13.png)

### align-items
這個屬性可以理解為內元素對於交錯軸的對齊設定。這個屬性有五個取值。

```css
.box {
    display: flex;
    align-items: flex-start | flex-end | center | baseline | stretch
}
```

接著來看看各自的效果吧!

`flex-start` : 靠上對齊

`flex-end` : 靠下對齊

`center` : 置中對齊

`stretch` : 元素裡的內容撐滿對齊

`baseline` : 元素裡的文字內容對齊 flex 基準線 (都在同個高度)

![](https://wtfhhh.oss-cn-beijing.aliyuncs.com/flex-14.png)

### align-content
這個屬性其實就是 align-items 的多行版本。如果元素只有單行是不會起效果的。這個屬性有六個取值。

```css
.box {
    display: flex;
    align-content: flex-start | flex-end | center | space-between | space-around | stretch
}
```

`flex-start` : 緊密靠上對齊

`flex-end` : 緊密靠下對齊

`center` : 緊密置中對齊

`space-between` : 分散對齊（上下不留間距）

`space-around` : 分散對齊（上下有留間距）

`stretch` : 所有元素撐滿上下邊

![](https://wtfhhh.oss-cn-beijing.aliyuncs.com/flex-15.png)


## 內元素

### order
這個屬性可以重新定義內元素的排列順序，順序會依據數值的大小排列。默認值為 0。本人一開始覺得有點抽象，來看例子。

沒有設定 order 前:
![](https://wtfhhh.oss-cn-beijing.aliyuncs.com/flex-16.png)

設定 order:

```css
.box-3 {
    order: -1;
}

.box-4 {
    order: 1;
}
```
設定 order 之後:
![](https://wtfhhh.oss-cn-beijing.aliyuncs.com/flex-17.png)

所以可以清楚的看到，order 會根據大小，由小到大按照 flex-direction 的方向排列。而且，order 也可以為負。

### align-self
這個屬性可以允許你針對任意內元素，更改它相對於交錯軸的對齊方式(相對於主軸則沒辦法更改)。所以這個屬性跟 align-items 一樣，有同樣的屬性。

```css
.box {
    align-self: flex-start | flex-end | center | baseline | stretch
}
```
`flex-start` : 靠上對齊

`flex-end` : 靠下對齊

`center` : 置中對齊

`stretch` : 撐滿上下邊

`baseline` : 基準線對齊

```css
.box-1 {
    align-self: flex-start;
}

.box-2 {
    align-self: flex-end;
}

.box-3 {
    align-self: center;
}
```

![](https://wtfhhh.oss-cn-beijing.aliyuncs.com/flex-18.png)

### flex-grow
這個屬性是一個數值，代表任意元素的伸展性。是在如果還有剩餘空間的話，才有伸展的可能性。數值越大的元素會伸展的更多一些，也就是說，會佔據整個區塊的更大比例。默認值為 0。看看下面的效果也許更清楚些。

沒有設定 flex-grow 前:
![](https://wtfhhh.oss-cn-beijing.aliyuncs.com/flex-8.png)

設定 flex-grow:

```css
.box-2 {
    flex-grow: 1
}

.box-3 {
    flex-grow: 2
}
```
設定 flex-grow 後:
![](https://wtfhhh.oss-cn-beijing.aliyuncs.com/flex-19.png)


### flex-shrink
這個屬性是一個數值，代表任意元素的收縮性。如果外部容器空間不夠容納所有內元素，又不能「超出去」，那 flex 布局就要適當收縮某些元素啦!所以 flex-shrink 的默認值為 1，也就是說，如果你沒設定，其實 flex 布局也會幫你適當收縮各個內元素。而 flex-shrink 目的就在於，你想要針對特定元素做不一樣的收縮比例。值越大，收縮比例就越多。

這邊設定的外部容器寬度只有 600px。但 box1, box2, box3 寬度分別設定為 100px, 300px, 500px。顯然，外部容器無法正常容納所有內元素。

沒有設定 flex-shrink 前:
![](https://wtfhhh.oss-cn-beijing.aliyuncs.com/flex-20.png)

設定 flex-shrink: 
```css
.box-1 {
    width: 100px;
}

.box-2 {
    width: 300px;
    flex-shrink: 2;
}

.box-3 {
    width: 500px;
    flex-shrink: 3;
}
```
設定 flex-shrink 後:
![](https://wtfhhh.oss-cn-beijing.aliyuncs.com/flex-21.png)

### flex-basis
元素的基準值，可使用不同的單位值。

### flex
flex 就是把 flex-grow, flex-shrink, flex-basis 寫在一起而已。

```css
.box {
    flex: <flex-grow> || <flex-shrink> || <flex-basis>
}
```

# 結語
CSS3 Flex 其實已經是非常主流的布局方式，且現在許多 CSS 框架也都是使用 Flex 來作網格排版。希望對於也還尚不了解 Flex 布局的人們多少有些幫助!如果還想了解 CSS3 Grid 布局的話，也歡迎看看這篇 [CSS3-grid-格线布局](https://blog.csdn.net/weixin_46803507/article/details/106573980)。


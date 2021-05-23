@[TOC](CSS3-grid-格線佈局)

# 前言

最近開始了寫博客的計畫，一方面受同學影響，一方面也是覺得該為自己的學習留下一點足跡。廢話不多說，第一篇就來談談css3的grid屬性，還請大佬們多多指教。

# 正文

## grid 的基本屬性與語法

- grid 屬性

|attribute|description|
|:---------:|:-----------:|
|grid-template-columns|網格區塊寬度|
|grid-template-rows|網格區塊高度|
|grid-template-areas|網格區塊名稱|
|grid-column-gap|網格區塊左右間距|
|grid-row-gap|網格區塊上下間距|
|grid-column|指定網格左右的位置|
|grid-row|指定網格上下的位置|

## grid的布局方式

首先還是先搞清楚grid的布局方式，顧名思義，grid代表的是格線。在grid的布局方式下，我們可以把分成兩部分來看，一個是屬於外部容器(grid-container)的屬性，另一個是屬於內部容器(grid-item)的屬性。接下來就來看看有哪些屬性以及用法八!

### 外部容器

#### display
首先必須定義容器所採用的顯示系統。

```css
.grid-box {
    display: grid;
}
```

#### grid-template-columns, grid-template-rows
grid和flex最大的不同在於，grid透過grid-template來定義排版方式，分別由column跟row定義出直排與橫列的格線，內容再依照格線做安排。

```css
.grid-box {
    display: grid;
    grid-template-columns: 100px 200px auto 50px 100px;
    grid-template-rows: 10% 200px auto;
}
```

![](https://wtfhhh.oss-cn-beijing.aliyuncs.com/css-grid.png)
米色部分就是grid-template-columns，而藍色部分則是grid-template-rows。

#### grid-column-gap, grid-row-gap
上面這兩個屬性描述的是行列之間的間距，具體效果在下面例子再一併展示。

#### grid-template-areas
這個屬性的概念可以想成是在grid-container上登記屬於該元素的空間，所以這部分需要搭配內外元素一起理解。

- 內元素: grid-area
- 外元素: grid-template-areas

```css
.grid-box {
    display: grid;
    grid-template-columns: 200px 50px auto 50px 200px;
    grid-template-rows: 25% 100px auto;
    grid-template-areas: 
        "header header header header header"
        "sider main main main main"
        "sider footer footer footer footer";
}

.header {
    grid-area: header;
    background-color: papayawhip;
}

.sider {
    grid-area: sider;
    background-color: skyblue;
}

.main {
    grid-area: main;
    background-color: azure;
}

.footer {
    grid-area: footer;
    background-color: rosybrown;
}
```

![](https://wtfhhh.oss-cn-beijing.aliyuncs.com/css-grid-1.png)


==* 注意: 空間不能分離且是唯一的，錯誤範例: "sider main sider"==

#### 單位
除了大家熟知的px以外，在設定grid-template-columns跟grid-template-rows時也能用其他單位設定。這邊介紹fr(fraction)，還又repeat的用法。

- fr 以及 repeat
fr這個單位能夠將可用的剩餘空間做比例分割，以 1fr 2fr 為例，空間將被分割成 1/3、2/3 兩個大小。
另一個是 repeat，可以重複隔線。


```css
.grid-box {
    display: grid;
    grid-template-columns: 1fr 1fr;
    grid-column-gap: 10px;
    grid-row-gap: 10px;
}
// repeat用法
.grid-box {
    display: grid;
    grid-template-columns: repeat(2, 1fr);
    grid-column-gap: 10px;
    grid-row-gap: 10px;
}
```
![](https://wtfhhh.oss-cn-beijing.aliyuncs.com/css-grid-2.png)


### 內部容器

#### grid-column, grid-row
除了grid-template-areas, 還有span的概念可以用。span代表包含的意思，假設一次要顯示2個，就可寫span 2。

除此之外，也可以指定分割比例。這麼做的概念是以第幾條格線~第幾條格線來計算。所以假使我們將grid-template-columns畫分為5個區域，那麼1/3的意思，就是從第1條線開始，到第3條線結束的整塊區域。

具體效果直接上代碼吧!

```css
.grid-box {
    display: grid;
    grid-template-columns: repeat(5, 20%);
    grid-template-rows: repeat(2, 50%);
    height: 100%;
    text-align: center;
}

#item-one {
    grid-column: 1/3;
    background-color: yellowgreen;
    border-radius: 1px;
    border-style: dashed;
}

#item-two {
    grid-column: 3/6;
    background-color: skyblue;
    border-radius: 1px;
    border-style: dashed;
    border-left: none;
}

#item-three {
    grid-column: span 1;
    border-radius: 1px;
    border-style: dashed;
    border-top: none;
    background-color: green;
}

#item-four {
    grid-column: span 4;
    border-radius: 1px;
    border-style: dashed;
    border-left: none;
    border-top: none;
    background-color: palevioletred;
}
```

![](https://wtfhhh.oss-cn-beijing.aliyuncs.com/css-grid-3.png)

如上圖效果所示，我們將template-columns分成5分，將template分成2分。淺綠色塊grid-column因為設定為1/3，所以區塊佔了一行中的40%(3-1/5)。再看深綠色塊，因為grid-column設定為span 1，所以區塊佔了一行中的20%(1/5)。這樣就一目瞭然啦!

# 結語

上面介紹了一些css3 grid布局方式的一些基本概念和基本屬性及用法，若有錯誤或不全也請各位大神們指正本人，也希望對於一些跟我一樣初學grid的朋友們有幫助!

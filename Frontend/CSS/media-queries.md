@[TOC](CSS3 媒體適應(media-queries))

# 前言

大家都知道，製作網頁時常常會需要做到適配屏幕大小的需求，不能說在這部電腦上展示沒問題，換成另一部，或者說換成手機端就全部樣式都"飛掉"。其實這就是所謂的 RWD(Responsive Web Design) 響應式網頁。而在製作RWD響應式網頁時，最重要的一個技術就是 css 的 media。這篇將會談談我對 media-query 的了解，也幫助自己未來撰寫時，更有個方向與參考。

# 正文

## CSS Media Queries 是什麼？
其實我們顧名思義，就可以知道 media query 的意思就是「query」(查詢)「media」(媒體)的屬性。這樣的機制本身就有附帶邏輯條件，下面一一介紹，而當查詢條件為 true 時就套用樣式，反之，若為 false，則不會套用樣式。

### 使用語法
```css
@media (max-width: 500px) {
    /* 樣式 */
    .style {
        ...
        ...
    }
}

@media print {
    /* 樣式 */
    ...
    ...
}
```

接下來更加深入 media 的世界吧!

## Media 的類型
在 css level4 普及後，現在的 media 類型主要分為以下四種:
|類型|描述|
|:------:|:------------|
|all|所有設備|
|print|印刷裝置，包含使用列印預覽產生的所有畫面 ( 例如列印為 pdf )|
|screen|螢幕裝置|
|speech|朗讀裝置，針對可以「讀出」頁面的設備|

- 以下為與媒體查詢無關的小常識，沒有興趣的可以直接跳過
>上面提到的speech類型設備，因為本人常識不足，所以一開始真的無法想像「朗讀」的概念。原來，針對的是盲人等身障群體，雖然他們看不見，但依舊可以使用手機歐!那怎麼用呢?就是使用帶有朗讀功能的設備。其實現今很多手機都附帶有朗讀的功能了，而因為盲人的聽覺一般是我們的3~4被靈敏，所以就靠這些speech設備，將畫面透過更高頻的頻率朗讀出來在盲人的腦中呈現視覺效果。具體原理可以更多的查詢，在這邊就介紹到這邊啦!


##　Media 的 features
如果說 Media 是查尋的大方向，那麼 features 就是查尋的小方向! feature 必須用括號`()`表示，甚至可以和邏輯關鍵字結合。如果是具有 range 性質，像是高度阿，寬度之類的屬性，也能在前面加上 `max-` 或著 `min-` 的前綴。以下做了四個大致上的分類:

### 頁面大小

|屬性|描述|
|:------------|:----------------------------|
|width|屏幕寬度，支持min-width和max-width|
|height|屏幕高度，支持min-height和max-height|
|aspect-ratio|屏幕長寬比，支持max-aspect-ratio和min-aspect-ratio(寫法為1680/720等)|
|orientation|屏幕旋轉方向，有portrait和landscape可選。portrait為縱向，landscape為橫向|

### 顯示品質

|屬性|描述|
|:------------|:----------------------------|
|resolution|解析度，支持max-resolution和min-resolution|
|overflow-block|當內容包含 block 特性並超過邊界範圍，有四個選項：none、scroll、optional-paged 和 paged。==none== 表示任何超過範圍都不顯示，例如看板，==scroll== 表示可滾動查看超出範圍，例如電腦螢幕，==optional-paged== 表示可手動查看超出的內容，例如簡報，==paged== 表示超出的內容會以分頁顯示，例如印表機|
|overflow-inline|當內容包含 inline 特性並超過邊界範圍，有兩個選項：none 和 scroll|

### 顏色

|屬性|描述|
|:------------|:----------------------------|
|color|輸出裝置的色彩位元數，若數值為 0 則代表黑白裝置，支持 max-color 和 min-color|
|color-index|輸出裝置的色彩索引位元數，支持max-color-index 和 min-color-index|
|monochrome|單色媒體功能，若數值為 0 表示「不是」單色設備|

### 互動

|屬性|描述|
|:------------|:----------------------------|
|hover|裝置具備 hover 的能力，有兩個選項：none 和 hover|


有個大致的概念後，接下來就直接上代碼來看看吧!

## Media-Queries 的例子和效果
有了上面介紹可以查尋的 features 之後，接下來就要關心怎麼做查詢了。 CSS3 提供的 Query 查詢有四種邏輯運算；分別是 ==or==, ==and==, ==not==, ==only==。除了or是用 `,` 表示以外，其他就是直接寫出英文就好。

### OR
`or` 的概念大家一定都不陌生，直接來看看應用。

#### or 應用1
如果今天想要做到，當屏幕寬度小於600px「或」印刷時，字體要變成紅色，背景變為黃色，字體大小為100px，否則字體為紫色，背景為綠色，字體大小為30px，可以這麼做:

```css
.block {
    width: 100%;
    height: 300px;
    background-color: springgreen;
    color: blueviolet;
    font-size: 30px;
}
@media (max-width: 600px), print {
    .block {
        background-color: yellow;
        color: red;
        font-size: 100px;
    }
}
```
![](https://wtfhhh.oss-cn-beijing.aliyuncs.com/media-query-1.png)

- 上圖為全屏的效果，下圖為縮小屏幕後的效果。

![](https://wtfhhh.oss-cn-beijing.aliyuncs.com/media-query-2.png)

#### or 應用2
如果像做到，當裝置是直立「或」寬度小於 200px，就讓文字變成紅色 50px，可以這麼做:
```css
.block {
    font-size: 20px;
}
@media (max-width: 200px), (orientation: portrait) {
    .block {
        font-size: 50px;
        color: red;
    }
}
```

### AND
`and` 的概念大家一定也都不陌生，直接來看看應用。

#### and 應用1
如果今天想做到在「螢幕」大小介於「700px~700px」之間時，字體會是紅色 50px，可以這麼做:
```css
.block {
    font-size: 100px;
    color: aqua;
}
@media screen and (min-width: 700px) and (max-width: 800px) {
    .block {
        font-size: 50px;
        color: red;
    }
}
```
- 圖一為全屏時的效果

![](https://wtfhhh.oss-cn-beijing.aliyuncs.com/media-query-3.png)

- 圖二將屏幕寬度縮小至750px

![](https://wtfhhh.oss-cn-beijing.aliyuncs.com/media-query-4.png)

- 圖三再將屏幕寬度縮小至500px

![](https://wtfhhh.oss-cn-beijing.aliyuncs.com/media-query-5.png)

效果應該都一目瞭然啦!

#### and + or 應用2
當然，and 跟 or 也是能搭配起來一起使用的，讓我們看看下面的例子:
如果今天想做到，除了「螢幕」大小介於「200px~300px」之間時，字體會是紅色 50px，橫向印刷也會是紅色 50px 的字體，可以這麼做:

```css
.block {
    font-size: 100px;
    color: aqua;
}
@media screen and (min-width: 200px) and (max-width: 300px), print and (orientation: landscape) {
    .block {
        font-size: 50px;
        color: red;
    }
}
```

### ONLY
ONLY 可以指定「只有」某種裝置媒體才能套用某些樣式，會寫在 media query 的字首，目前這種撰寫方式越來越少見，最主要是因為使用舊版裝置媒體的人越來越少，因此直接採用 AND 或 OR 的寫法就能夠符合絕大多數的需求。所以在這邊就不再多做介紹。

### NOT
NOT 的目的在於排除一些設備，會寫在 media query 的字首。這裡要注意一下，==NOT 和 ONLY 後方都必須先接 media type ( 像是 screen 或 print )，不然不會起作用。== 接下來看個例子:

#### not 應用1
如果今天想要做到，除了屏幕寬度小於等於 300px，或印刷時為縱向，會出現紅色 50px 的字體，可以這麼做:

```css
.block {
    font-size: 100px;
    color: aqua;
}
@media not screen and (max-width: 300px), print and (orientation: portrait) {
    .block {
        font-size: 50px;
        color: red;
    }
}
```

# 結語
說實話，其實覺得這個媒體適應看起來不是很複雜，也挺容易理解的。自己其實也沒有在實際項目中應用到媒體適應的技術，以後如果有機會實際應用，也許還回來更改這篇博客。總而言之，善用 Media Queries 對於撰寫一個 RWD 網頁，似乎是絕對必要的技能呦！也希望看完這篇，能夠讓那些對媒體適應沒有了解的人有個初步清晰的概念，若有錯誤，也歡迎各位大神們給小白一些指點啦!

@[TOC](JavaScript ES6 中的模板字符串(template-literals)和標籤模板(tagged template))

# 前言

在ES6中多了一個很好用的模板字符串(template-literals)。我們常常會需要在js中「放入HTML」，或者要在一個字符串中嵌入一些變數，又或者，需要很長的字串，且包含換行。這時候，模板字符串會是很棒的工具！

# 正文

##　模板字符串(template-literals)的基本使用

比起一般的字符串我們用的是單引號`('')`/雙引號`("")`，使用模板字符串時我們要用反引號 (在鍵盤左上角，`esc`下面)。

接下來直接來看些例子吧!

舉例來說，如果今天想打印HTML的標籤，在ES6之前我們必須這麼寫 :

```js
let string_es5 = '<div>\n' + '  <span>template literals' + '<span/>\n' + '<div/>' 
console.log(string_es5)
```
但是這樣寫就挺麻煩的，還要自己注意換行等等的細節。有了ES6的字符串模板之後，就能這麼寫啦 :

```js
let string_es6 = `
<div>
    <span>template literals<span/>
<div/>
`
console.log(string_es6)
```
是不是相對簡單，易讀性也提高了不少。

在console中會輸出如下:

![](https://wtfhhh.oss-cn-beijing.aliyuncs.com/temp-str.png)

也就是說，用``包起來的字符串會自動保存所有的換行以及空行歐。

## 在模板字符串中嵌入變數

在模板字符串中，我們還可以以`${...}`的方式插入任何變量:

```js
let myFirstName = 'Stephen'
let myLastName = 'Curry'
let my1Age = 20
let my2Age = 12;
let string_es6 = `My name is ${myFirstName} ${myLastName}, and I am ${my1Age + my2Age} years old.`

console.log(string_es6)
// My name is Stephen Curry, and I am 32 years old.
```

## 進階：標籤模板(tagged template)

接下來要談的標籤模板，非常適合用於過濾輸入者所輸入的信息，把模板本身和輸入變數分離。先來看看基本的概念:

- 標籤模板的用法就是將模板字符串直接接在函數之後。

```js
console.log `I like to play basketball.`
```
在console中會輸出如下:

![](https://wtfhhh.oss-cn-beijing.aliyuncs.com/template-string-2.png)

可見js引擎會將模板字符串解析成Array。那如果模板字符串帶有諸如`${...}`的變數呢?

```js
let myFirstName = 'Stephen'
let myLastName = 'Curry'
let my1Age = 20
let my2Age = 12;

console.log `My name is ${myFirstName} ${myLastName}, and I am ${my1Age + my2Age} years old.`
```

在console中會輸出如下:

![](https://wtfhhh.oss-cn-beijing.aliyuncs.com/template-lit-3.png)

這是甚麼原理呢?
原來如果我們使用標籤模板，也就是直接在函式後面直接接上模板字符串，其實會先將模板字符串切成多個參數。切法是會先將模板字符串中的模板部分切片，共同組成一個數組，作為第一個參數。接著就看你使用了幾個`${...}`，會依序變成函式的第二，第三....個參數。
讓我們來看一下代碼能更加理解：

```js
let myFirstName = 'Stephen'
let myLastName = 'Curry'
let my1Age = 20
let my2Age = 12;

function tagged_template(template) {
    console.log(template)
    console.log(arguments)
}

tagged_template `My name is ${myFirstName} ${myLastName}, and I am ${my1Age + my2Age} years old.`
```

在console中會輸出如下:

![](https://wtfhhh.oss-cn-beijing.aliyuncs.com/template-lit-4.png)

可以看到第一個參數就是那些模板字符串中的模板部分切片組成的數組(template)，arguments則會包含那些`${...}`變數。而template和arguments又共同組成一個Arguments數組。

結合擴展運算符(...)使用，因為不確定`${...}`的數量，所以可以將搭配...的使用，將所有變數組成一個新數組來使用。

```js
let myFirstName = 'Stephen'
let myLastName = 'Curry'
let my1Age = 20
let my2Age = 12;

function tagged_template(template, ...values) {
    console.log(template)
    console.log(values)
}

tagged_template `My name is ${myFirstName} ${myLastName}, and I am ${my1Age + my2Age} years old.`
```

在console中會輸出如下:

![](https://wtfhhh.oss-cn-beijing.aliyuncs.com/template-lit-5.png)

可以看到打印出來的values數組，就是只包含`${...}`變數的所有元素所組成的數組。

等於說是，把不在`${...}`和在`${...}`的，拆成兩個數組來使用。由此可以理解，所謂使用標籤模板(tagged template)，可以達到過濾輸入的涵義，和原有的模板劃分開。

# 結語

模板字符串其實算是js很基本的用法，所以才來加深下印象。也希望看完這篇，對你們有多少的幫助，若有錯誤，也歡迎各大神們指正。
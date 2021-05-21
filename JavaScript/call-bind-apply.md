@[TOC](理解 JavaScript 中的 call apply bind)

# 前言
在 js 中，當我們調用一個函數，默認都是傳入兩個參數，第二個我們平常就常常在使用，也就是 `arguments`，其實就是我們可選傳入的參數，第一個則是隱式的 `this`。在默認情況下，`this` 都指向調用函數的對象本身，但是有時候我們有可能需要改變 `this` 指向，這時候就會用到 call, apply, bind。

建議在看這篇前，先要去了解一下 js 的 `this`，可以參考 [淺談 JavaScript 中的 this](https://blog.csdn.net/weixin_46803507/article/details/106594774)。

# 正文
首先我們搞清楚 call, apply, bind 的來歷。

## call/apply/bind 概述
在 js 中其實所有函數都是 Function 的實例，而且對於 Function 來說，他的原型，Function.prototype 有很多東西，其中，call, apply, bind 就是 Function.prototype 的方法。所以根據原型的規則，所有函數都可以使用這三個方法。

簡單總結一下，就是 call, apply, bind 是 Function 原型的方法，而所有函數都是 Function 的實例。

## call/apply/bind 區別
簡單來說，這三個方法都是用於改變 `this` 的指向，結合代碼來做理解。
```js
function show(obj) {
    console.log(this)
    console.log(`${obj.name} is ${obj.age} years old.`)
}

const person = {
    name: 'cyz',
    age: 20
}

show.call(person, person)

// { name: 'cyz', age: 20 }
// cyz is 20 years old.
```
可以看到上面代碼中，雖然 person 對象自身是沒有 show 方法的，但是通過 `call` 方法可以做到 person 對象來調用 show 方法。

其實這三個方法在功能上沒有多大的區別，基本都是去改變 this 指向，主要區別只是在於方法的形式上和參數有一些不同。

|call|apply|bind|
|:-----|:-----|:-----|
|`函數.call(obj, arg1, arg2 ...)`|`函數.apply(obj, [arg1, arg2 ...])`|`let xxx = 函數.bind(obj, arg1, arg2 ...)`|

可以看到上面整理，對於 call 和 apply，接受的 obj 就是改變後的 this 所指向的對象，而 call 後面則接受參數，apply 接受的則是參數列表的形式。

至於 bind，其實本質上也是改變 this 的指向，只不過 bind 會返回一個函數，而且 bind 方法傳入的參數會預先傳給返回的方法，obj 就是返回函數的 this 指向，且這樣在使用時就不用再傳參。

所以其實下面的三種寫法都是等價的:
```js
1. show.call(person, person)
2. show.apply(person, [person])
3. let f = show.bind(person, person)
   f()
```
而正因為這三個方法可以改變 this 指向，所以 this 才會指向 person，相當於讓 person 作為 this 調用了 show 方法。

## call/apply/bind 用途
理解好了這三個方法的區別後，你可能會想說，到底這三個東西有啥用 ? 這邊就來說說 call, apply, bind 的使用場景以及好處。

試想以下情況，如果今天對象 A 有個方法，而對象 B 因為在某種特殊情況下也需要用到該方法，那這時候到底要直接為 B 添加一個方法屬性，還是希望能借用 A 的。

當然還是要視情況而定，但是如果沒有甚麼特別的要求，我們當然希望借用 A 的方法啦!既省內存，也可以減少系統中代碼的重複率。這其實就是 call, apply, bind 的核心理念，可以借用方法，達到減少內存開銷，減少重複代碼的好處。

這邊來看一個例子，下面我們自己實現了一個簡單的判斷類型的函數:
```js
function isType(data, type) {
    const typeObj = {
        '[object String]': 'string',
        '[object Number]': 'number',
        '[object Boolean]': 'boolean',
        '[object Null]': 'null',
        '[object Undefined]': 'undefined',
        '[object Object]': 'object',
        '[object Array]': 'array',
        '[object Function]': 'function',
        '[object Date]': 'date',
        '[object RegExp]': 'regExp',
        '[object Map]': 'map',
        '[object Set]': 'set',
        '[object HTMLDivElement]': 'dom',
        '[object WeakMap]': 'weakMap',
        '[object Window]': 'window',
        '[object Error]': 'error',
        '[object Arguments]': 'arguments',
    }
    let name = Object.prototype.toString.call(data) // 借用 Object.prototype.toString() 获取数据类型
    let typeName = typeObj[name] || 'unknown' // 匹配数据类型
    return typeName === type // 判断该数据类型是否为传入的类型
}
console.log(
    isType({}, 'object'), // true
    isType([], 'array'), // true
    isType(new Date(), 'object'), // false
    isType(new Date(), 'date'), // true
)
```
分析一下上面代碼的關鍵，那就是 `Object.prototype.toString.call(data)` 語句。可以試試看如果把 call 拿掉就會不對了。

`toString()` 方法相信大家都不陌生，是一個 Object.prototype 的方法，默認情況下會返回形如 `[object xxx]` 的一個字符串。如果調用 `toString()` 的對象沒有重寫該方法，就是默認情況，可以用於判斷對象類型。但是，大多數對象都重寫了 `toString()` 方法，所以如果我們還是想要做到獲取實際對象類型，如上述代碼需求，就必須使用 `call`。使用 `call` 相當於提供了 `toString()` 方法一個上下文，也就是 this，調用的就會是原生的 `toString()` 方法，完成需求。


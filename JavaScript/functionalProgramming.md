@[TOC](JavaScript 函數式編程)

# 什麼是函數式編程
函數式編程英文是 functional programming，其實指的是一種編程範式(programming paradigm)，也就是一套寫代碼的方法。

其實主要的編程反式有三種：

- 命令式編程
- 聲明式編程
- 函數式編程

其實一般我們寫程序大多是命令式編程。函數式編程相比命令式編程來說，更加強調程序執行的結果而非執行的過程，通常是通過若干簡單的函數，或是說簡單的執行單元，讓計算結果不斷推進，一步步推導出複雜的計算，而不是直接設計一個單一的，複雜的計算過程。

舉個例子，將數組每個元素進行平方操作，簡單的一功能，來看看二者的寫法區別：

- 命令式編程
```js
const arr = [1,2,3,4,5]
for(let i=0; i<arr.length; i++) {
    arr[i] = Math.pow(arr[i], 2)
}
console.log(arr)
// [ 1, 4, 9, 16, 25 ]
```

- 函數式編程
```js
const arr = [1,2,3,4,5]
const res = arr.map(num => Math.pow(num, 2))
console.log(arr)
// [ 1, 4, 9, 16, 25 ]
```
簡單來說，就是要把過程邏輯寫成函數，定義好輸入參數，然後只關心輸出結果。

稍微總結一下，函數式編程就是一種描述 集合-集合 之間的轉換關係，通過輸入函數都會返回 <strong>有且只有</strong> 一個結果。
![](https://ftp.bmp.ovh/imgs/2021/04/768f745f9376300b.png)

# 概念

## 純函數
函數式編程目的就是希望盡可能的提高代碼的無狀態性和不變性，要做到這一點，就要學會使用無副作用的函數，也就是純函數。

純函數是對給定的輸入返回一個唯一輸出的函數，並且要求你所有的數據都是不可變的，也就是說，<strong>純函數 = 無狀態 + 數據不可變</strong>。

舉一個簡單的例子：
```js
let double = value => value * 2
```
上面的函數就是一個純函數，符合以下特點：

- 函數內部傳入指定的值，就會返回一個確定的、唯一的值
- 不會造成超出作用域的變化，例如改變引用傳遞的參數

## 高階函數
在我們的編程世界中，說白了其實要處理的也就是 “數據” 與 ”關係“，而所謂的關係其實就是體現為函數。一旦我們找到我們找到正確的映射關係，問題就解決了，剩下的事情就是讓數據『流』過這個函數就好了。大概的意思就像下圖：
![](https://ftp.bmp.ovh/imgs/2021/05/325e656ba88ad6c7.png)

這就是高階函數的作用，以<strong>函數</strong>作為輸入輸出的函數被稱為<strong>高階函數</strong>。

通過高階函數的抽象過程，我們就可以特別注重結果，看看下面例子：
```js
const forEach = function(arr, fn) {
    for(let i=0; i<arr.length; i++) {
        fn(arr[i])
    }
}

const arr = [1,2,3]
forEach(arr, (item) => {
    console.log(item)
})

// 1
// 2
// 3
```
上面通過高階函數 `forEach` 來抽象如何做的邏輯，直接關注做了什麼，丟進去的就包括了函數，因此就是一種高階函數的體現。

## 柯里化(Currying)
當初第一次聽到這個名字覺得太有趣了，因為其實本人是個籃球迷，最喜歡的球員也就是 Stephen Curry，所以覺得竟然計算機的世界裏也出現了偶像，太湊巧了。

回到正題hhh，柯里化是個什麼概念呢？其實柯里化就是把一個多參數的函數轉化為一個嵌套的一元函數的過程。這樣看下來可能有點抽象，不如直接上代碼，更好理解些～

假設有這麼一個二元函數：
```js
let fn = (x, y) => { return x + y }
```
轉換為柯里化函數如下：
```js
const curry = function(fn) {
    return function(x) {
        return function(y) {
            return fn(x, y)
        }
    }
}

let curryfn = curry(fn)
console.log(curryfn(1)(2))
// 3
```
上面的 `curry` 函數只處理二元情況，下面再來實現一個實現多參數的情況：
```js
const curry = function(fn) {
    return function curriedFn(...args) {
        if(args.length < fn.length) {
            return function() {
                return curriedFn(...args.concat([...arguments]))
            }
        }
        return fn(...args)
    }
}

const fn = (x, y, z, q) => { return x + y + z + q }
const curryfn = curry(fn)
console.log(curryfn(1)(2)(3)(-1))
// 5
```

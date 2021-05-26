@[TOC](JavaScript 理解深拷貝以及淺拷貝)

# 前言
之前寫過一篇關於 Java 中的深拷貝以及淺拷貝，如果有興趣的歡迎看看 [Java 理解深拷貝以及淺拷貝](https://blog.csdn.net/weixin_46803507/article/details/114607406)，所幸就一鼓作氣，把 js 中的拷貝也一起學了，之後應該也會寫寫關於 C++ 的拷貝問題。

# 正文
如何區分深拷貝以及淺拷貝，其實很簡單，就是假設 B 是 A 的一個拷貝，當修改 A 時，看看 B 有沒有變，如果 B 變了就是淺拷貝，反之則為深拷貝。直接看代碼，舉個例子:
```js
let a = [0, 1, 2, 3, 4];
let b = a;
console.log(a === b);

a[0] = 1;
console.log("a:", a);
console.log("b:", b);
```
運行結果如下:
![](https://wtfhhh.oss-cn-beijing.aliyuncs.com/jscopy-1.png)

這很明顯就是淺拷貝了，因為修改其一會連帶的修改到另一個。而在用 js 實現淺拷貝和深拷貝之前，我們要來看看一些更基本的概念，幫助更好的理解拷貝問題。

## 基本數據類型 vs 引用數據類型
在 JavaScript 中，基本數據類型包括:
- `number`
- `string`
- `boolean`
- `null`
- `undefined`
- `symbol`
- `BigInt`(未來 ES10 新增)

引用數據類型，則包括像是對象 {a:1}，數組，函數等等。

而這兩類數據在內存中是這樣存儲的:
- 基本數據類型
```js
let a = 1;
```
![](https://wtfhhh.oss-cn-beijing.aliyuncs.com/jscopy-2.png)

```js
let b = a;
```
![](https://wtfhhh.oss-cn-beijing.aliyuncs.com/jscopy-3.png)

所以當你無論是改變 a 還是 b，都不會改變對方，因為他們都各自有各自的內存區塊。不過這也不算是深拷貝，這只是 js 自己的機制，深拷貝是在探討更複雜的引用類型(Object)。

- 引用數據類型
在 js 中，對於引用數據類型，會把名跟值分開，把名存在棧(stack)中，把值存在堆(heap)中，但是棧內存會提供一個引用的地址去指向堆中對應的值，將名和值關聯在一起。

```js
let a = [0,1,2,3,4];
```
![](https://wtfhhh.oss-cn-beijing.aliyuncs.com/jscopy-4.png)

```js
let b = a;
```
![](https://wtfhhh.oss-cn-beijing.aliyuncs.com/jscopy-5.png)
可以看到，當 `b=a` 進行拷貝時，其實複製的是 a 的引用地址，而不是真的把堆裡面的值賦給 b。

```js
a[0] = 1;
```
當我們 `a[0] = 1` 對 a 數組進行修改時，由於 a 和 b 都指向同一個地址，自然 b 也會受到影響。這就是所謂的淺拷貝。
![](https://wtfhhh.oss-cn-beijing.aliyuncs.com/jscopy-6.png)

所以其實深拷貝就是希望做到，在堆中也開闢一塊新的空間存放 a 數組的值，並讓 b 的引用地址指向這塊內存空間就好，就像基本數據類型一樣。
![](https://wtfhhh.oss-cn-beijing.aliyuncs.com/jscopy-7.png)

## 實現深拷貝

### 初級版
最簡單的深拷貝實現可以使用 `JSON.stringify` 以及 `JSON.parse`。看下面代碼:
```js
let obj = {
    a: 1,
    b: 2,
    c: [1,2,3]
}

let objString = JSON.stringify(obj);
let obj1 = JSON.parse(objString)

console.log("obj: ", obj);
console.log("obj1 ", obj1);

obj1.b = 0;
obj1.c = [2,3,4];

console.log("\nobj: ", obj);
console.log("obj1 ", obj1);
```
運行結果如下:
![](https://wtfhhh.oss-cn-beijing.aliyuncs.com/jscopy-8.png)

- `JSON.stringify`: 將 js 對象轉成 json 字符串
- `JSON.parse`: 將 json 字符串傳為 js 對象

可以看到，改變 obj1 無論是基礎數據類型還是引用類型，都不會改變到 obj，兩個對象是獨立的。但為甚麼說這個方法不推薦呢，因為他這個方法無法拷貝 undefined, function 等等。如下:
```js
let obj = {
    a: 1,
    b: 2,
    c: [1,2,3],
    d: function() {
        console.log("d");
    }
}

let objString = JSON.stringify(obj);
let obj1 = JSON.parse(objString)

console.log("obj: ", obj);
console.log("obj1 ", obj1);

obj1.b = 0;
obj1.c = [2,3,4];

console.log("\nobj: ", obj);
console.log("obj1 ", obj1);
```
運行結果如下:
![](https://wtfhhh.oss-cn-beijing.aliyuncs.com/jscopy-9.png)
可以看到，obj1 根本沒有複製到 obj 的 d 屬性(函數)，顯然這不是我們希望的，因此來看看更好的深拷貝吧。

### 中級板
使用 `Object.assign(target, source)` 也可以達到深拷貝，看下面代碼:
```js
let obj = {
    a: 1,
    b: 2,
    c: [1,2,3],
    d: function() {
        console.log("d");
    }
}

let obj1 = Object.assign({}, obj);

console.log("obj: ", obj);
console.log("obj1 ", obj1);

obj1.b = 0;
obj1.c = [2,3,4];

console.log("\nobj: ", obj);
console.log("obj1 ", obj1);
```
運行結果如下:
![](https://wtfhhh.oss-cn-beijing.aliyuncs.com/jscopy-10.png)
好像剛剛的問題解決了，現在的 obj1 基本就是 obj 的一個副本，且改動其一也不會影響到另一個。那這個方法又有甚麼問題呢?來看看下面例子:
```js
let obj = {
    a: 1,
    b: 2,
    c: [1,2,3],
    d: function() {
        console.log("d");
    }
}

let obj1 = Object.assign({}, obj);

console.log("obj: ", obj);
console.log("obj1 ", obj1);

obj1.b = 0;
obj1.c[0] = -1; // 只改數組其中一個元素，而非整個數組重新賦值

console.log("\nobj: ", obj);
console.log("obj1 ", obj1);
```
運行結果如下:
![](https://wtfhhh.oss-cn-beijing.aliyuncs.com/jscopy-11.png)
我們發現問題就是，當我們只改變數組其中一個元素，而非整個數組重新賦值時，竟然 obj 也跟著變了!這顯然是個問題，也就是說，這種方法對於嵌套的引用對象的深拷貝，還是存在一定程度上的安全隱患。那麼最後來看看最優解決方案吧!

### 高級版
最好的深拷貝方案就是使用遞規拷貝，直接看看代碼吧:
```js
/* 
@param: target
*/ 
function deepCopy(target) {
    // 定義一個變量
    let result;
    // 如果當前需要深拷貝的是一個引用類型
    if(typeof target === 'object') {
        if(Array.isArray(target)) { // 如果是一個數組
            result = [];
            for(let i in target) {
                result.push(deepCopy(target[i]));
            }
        }else if(target == null) { // 如果當前值為 null
            result = null;
        }else if(target.constructor === RegExp) { // 如果當前對象是一個 RexExp
            result = target;
        }else { // 否則為一般對象，遞規賦值對象所有值
            result = {};
            for(let i in target) {
                result[i] = deepCopy(target[i]);
            }
        }
    }
    // 如果當前需要深拷貝的是基本數據類型
    else {
        result = target;
    }

    return result;
}
```
我們測試看看這樣的深拷貝效果如何:
```js
let obj1 = {
    a: {
        c: 1,
        d: undefined,
        b: null
    },
    b: function () {
        console.log(this.a)
    },
    c: [
        {
            a: 'c',
            b: 2,
            c: undefined
        },
        'a',
        3
    ]
}

let obj2 = deepCopy(obj1);

console.log("obj1: ", obj1);
console.log("obj2: ", obj2);
```
運行結果如下:
![](https://wtfhhh.oss-cn-beijing.aliyuncs.com/jscopy-12.png)
可以看到，這樣的深拷貝也很好的解決了 undefined, function 等特殊情況，而修改 obj1 或是 obj2 也不會影響到對方，實現了比較完整安全的深拷貝。

# 結語
這篇介紹了 js 一些基本的數據類型的基礎知識，以及淺拷貝深拷貝的區別，最後用 3 種方式分別實現了 js 中的深拷貝。希望這篇文章對你有幫助，若有錯誤或是高見，也歡迎多多指教啦!

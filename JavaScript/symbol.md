@[TOC](關於 JavaScript 的 Symbol)

# js 數據類型
在進入本篇的正題之前，先介紹下 js 的基本數據類型:
- `String`
- `Object`
- `Number`
- `Boolean`
- `Null`
- `Undefined`

這六種相信大家都不陌生，且也都知道各自的用途。但是 ES6 為我們帶來了一種全新的數據類型，Symbol。

# Symbol 設計初衷
為了更好的理解 symbol 這個東西，先要了解為甚麼會需要這樣一個東西，設想一個實際工作的場景:

協作開發相信大家都有這樣的經驗，假設在一個團隊中，程序員 A 寫了一個對象供大家來使用，有天有個程序員 B 使用了這個對象，還為它添加了一些屬性和方法。看起來都沒甚麼問題對吧!

但是隔天，測試告訴程序員 A 說代碼出 bug 了，程序員 A 就心想:怎麼可能，我根本甚麼都沒動阿...。

無奈的 A 只好慢慢排查定位問題，最後才發現，原來是程序員 B 給對象添加方法時，其中一個方法名不小心和原本 A 就寫的一個方法名重名了，所以被覆蓋了。

對象的屬性被覆蓋，這在日常編程中也時常遇到。像程序員 A 這種情況，不管別人怎麼添加屬性或方法，他希望沒有人可以覆蓋他自己寫好了屬性和方法，需要給這些屬性或方法取一個獨一無二的名字，這樣才能從根本上防止這種不小心被覆寫的情況。

這就是 ES6 設計 Symbol 的初衷。

# Symbol 使用
先來看看一個基本用法的例子:
```js
let sb = Symbol();

console.log(sb);
console.log(typeof sb);

// 打印結果
// Symbol()
// symbol
```
我們看到，用一個 Symbol() 函數來創建一個 symbol 類型的變量，打印一下變量 sb，控制台輸出 `Symbol()`，代表一個獨一無二的值，雖然我們看不到它到底長什麼樣子，但是不管，它就是獨一無二的。

那到底怎麼來判斷是不是獨一無二呢?看看下面代碼:
```js
let obj1 = Symbol();
let obj2 = Symbol();

console.log(obj1 === obj2);

console.log(obj1);
console.log(obj2);

// 打印結果
// false
// Symbol()
// Symbol()
```
可以看到，我們使用嚴格相等去做比較，確實是獨一無二的(輸出 false)，但再看看各自的輸出，好像又長的一樣。這其實還蠻困擾的，畢竟眼見為憑，那有沒有辦法讓這兩個 symbol 值看起來不一樣呢?有的，看代碼:
```js
let obj1 = Symbol("obj1");
let obj2 = Symbol("obj2");

console.log(obj1 === obj2);

console.log(obj1);
console.log(obj2);

// 打印結果
// false
// Symbol(obj1)
// Symbol(obj2)
```
我們在 Symbol() 函數中傳入一個字符串參數，然後打印出來就看起來不一樣了。其實呢，這個參數就好像是對這個獨一無二的值的一個描述，可以方便我們去辨別誰是誰。不過千萬不要忘記，即便 <strong>參數一樣，描述一樣，還是兩個完全獨一無二的值</strong>。

了解完 symbol 的一些特性後，那我們結合對象來看看 symbol。
```js
let name = Symbol();

let person = {
    [name]: "Tom"
}

console.log(person[name]);
console.log(person.name);

// 打印結果
// Tom
// undefined
```
首先我們先定義一個 symbol 類型的變量 name，它作為 person 對象的屬性，對應的值是 Tom。接著我們嘗試用兩種方式來獲取 name 屬性。第一種我們使用 `[]`，正確的獲取到了值。第二種使用 `.` 運算符的得到是 undefined，原因是，當我們用 symbol 類型定義對象的屬性時，不能用點運算符獲取對應值。

還有一點要注意，先看下面代碼:
```js
let name = Symbol();

let person = {};

person.name = "Tom";
console.log(person[name]);

person[name] = "Tom";
console.log(person[name]);
```
可以看到，當要象對象添加 symbol 屬性時，也要記得使用 `[]`，否則不會成功。

## 遍歷屬性名
當用 symbol 作為屬性時，該屬性是不會出現在 `for...in` 的，也不會被 `Object.keys()` 獲取到。看看實際案例:
```js
let name = Symbol();

let person = {
    [name]: 'Tom',
    age: 20
}

for(let key in person) {
    console.log(key);
}

console.log(Object.keys(person));

// 打印結果
// age
// [ 'age' ]
```

## getOwnPropertySymbols()
有時候我們就是需要獲得對象的 symbol 類型屬性，這時候可以用 `getOwnPropertySymbols()` 函數。它會找到對象中的所有 symbol 類型屬性並以數組形式返回，如下:
```js
let name = Symbol('name');
let age = Symbol('age');

let person = {
    [name]: 'Tom',
    [age]: 20
}

console.log(Object.getOwnPropertySymbols(person));

// 打印結果
// [ Symbol(name), Symbol(age) ]
```
## Reflect.ownKeys()
不過這樣，要獲取字符串類型的屬性和 symbol 類型屬性要分開獲取感覺還是太麻煩了，所幸 ES6 也提供了相應的函數 `Reflect.ownKeys()`，讓我們可以獲取對象中所有的屬性，無論是什麼類型，如下:
```js
let name = Symbol('name');
let age = Symbol('age');

let person = {
    [name]: 'Tom',
    [age]: 20,
    sex: 'male'
}

console.log(Reflect.ownKeys(person));

// 打印結果
// [ 'sex', Symbol(name), Symbol(age) ]
```

## Symbol.for()
`Symbol.for()` 會根據函數參數名，去全局環境中搜索是否有以該參數為名的 symbol 值，有就返回，沒有就以該參數為名創建一個新的 symbol 值。
```js
let sb1 = Symbol.for('name');
let sb2 = Symbol.for('name');

console.log(sb1 === sb2);

// 打印結果
// true
```
可以看到打印的結果是 true，也就表示 sb1 和 sb2 是根本是同一個東西。但是注意到，在上面的代碼中，我們都是用 `Symbol.for()` 來創建 symbol 值，而不是一開始的 `Symbol()`。

區別在於，`Symbol.for()` 創建的值會直接被登記在全局環境中，供以後的 `Symbol.for()` 來使用，而簡單的 `Symbol()` 就沒有這樣的效果，也就是說，用 `Symbol()` 創建的值是沒辦法被 `Symbol.for()` 找到的。我們可以試試看:
```js
let obj1 = Symbol('name');
let obj2 = Symbol.for('name');

console.log(obj1 === obj2);

// 打印結果
// false
```
可以看到，obj1 的名字也叫做 name，但是對於 obj2 而言是沒辦法在全局環境中找到的，因此會再創建一個全新的 symbol，也因此打印結果為 false。

## Symbol.keyFor()
`Symbol.keyFor()` 會根據參數返回一個已經登記在全局環境中的 symbol 的 key，若沒找到，則返回 undefined。也就是說，必須是由 `Symbol.for()` 創建的 symbol 值才可以，`Symbol()` 創建的是不行的。
```js
let obj1 = Symbol('obj1');
let obj2 = Symbol.for('obj2');

console.log(Symbol.keyFor(obj1));
console.log(Symbol.keyFor(obj2));

// 打印結果
// undefined
// obj2
```
由上面的代碼可以確認，通過 `Symbol()` 創建的 symbol 是不會登記在全局環境中的，而 `Symbol.for()` 創建的才會在全局環境中。

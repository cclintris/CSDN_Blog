@[TOC](ES6 數據結構 Set)

# Set 基本用法
ES6 提供了新的數據結構 Set，其實就跟一般的數組很像，只是 Set 中的成員值都是唯一的，沒有重複的值。Set 本身其實是一個構造函數，就是用於生成 Set 數據結構。
```js
let set = new Set()
[2, 3, 5, 4, 2, 3].map(x => set.add(x))

for(let i of set) {
    console.log(i)
}

// 2 3 5 4
```
結果應該很清楚的表明 Set 的特色。Set 也可以接受一個數組來初始化。
```js
let set = new Set([1,1,2,3,4])
let arr = [...set]
console.log(arr)

// [1,2,3,4]
```
所以其實 Set 也提供了一種將數組去重的方法:
```js
let arr = [1,1,2,3,3,4]
let uniqueArr = [... new Set(arr)]
console.log(uniqueArr)

// [1,2,3,4]
```
不過要注意，當我們將元素加進 Set 時不會發生類型轉換，所以 `5` 和 `'5'` 其實是兩個不同的值。Set 判斷兩個值是否相同的方式類似於 `===` 全等運算符，唯一要注意的是，對 `===` 來說，兩個 NaN 總是不一樣的，但對於 Set 來說，總是一樣的。
```js
console.log(NaN === NaN)

// false

let set = new Set()
const a = NaN
const b = NaN
set.add(a)
set.add(b)
console.log(set)

// Set { NaN }
```
另外，對於 Set 來說，兩個對象總是不相等的。
```js
let set = new Set()
const obj1 = {}
const obj2 = {}
set.add(obj1)
set.add(obj2)
console.log(set)

// Set { {}, {} }
```
# Set 實例的屬性和方法
一個 Set 實例有以下屬性:
- `Set.prototype.constructor:` 構造函數。
- `Set.prototype.size:` 返回 Set 實例成員總數。

而 Set 的方法主要可以分為兩大類，操作方法(操作成員)以及遍歷方法(遍歷成員)。下面進行簡單常見的介紹。

## 操作方法
最常見的操作有四個:
- `add(value):` 添加 value，返回 Set 實例本身
- `delete(value):` 刪除 value，返回 bool，表示成功與否
- `has(value):` 返回 bool，表示 value 在不在 Set 中 
- `clear():` 清除 Set 所有成員

看代碼很清楚:
```js
let set = new Set()
set.add(1).add(2).add(2)
console.log(set.size)
console.log(set.has(1))
set.delete(2)
console.log(set)
set.clear()
console.log(set)

// 2
// true
// Set { 1 }
// Set {}
```
配合 `Array.from()` 方法可以將 Set 轉回數組，因此就又提供了另一種數組去重的方法:
```js
function deduce(arr) {
    return Array.from(new Set(arr))
}
```
## 遍歷方法
Set 提供了四種方法遍歷成員:
- `keys():` 返回鍵名的遍歷器
- `values():` 返回鍵值的遍歷器
- `entries():` 返回鍵值對的遍歷器
- `forEach():` 使用回調函數遍歷每個成員

`keys()`, `values()`, `entries()` 返回的都是遍歷器，不過由於 Set 本身就沒有鍵名，或是說鍵名就是鍵值，所以表現起來的行為感覺一模一樣。
```js
let set = new Set()
set.add('red').add('green').add('blue')

for(let i of set.keys()) {
    console.log(i)
}
// red
// green
// blue

for(let i of set.values()) {
    console.log(i)
}
// red
// green
// blue

for(let i of set.entries()) {
    console.log(i)
}
// ["red", "red"]
// ["green", "green"]
// ["blue", "blue"]
```
`forEach()` 就是對每個成員都做一個自定義的操作，沒有返回值。
```js
let set = new Set([1,2,3,4,5])

set.forEach((value, key) => {
    console.log(++value)
})

// 2
// 3
// 4
// 5
// 5
```
看了這些方法，接下來就來看看 Set 遍歷的一些應用。

- 擴展運算符 `...` 也可以適用於 Set
```js
let set = new Set([1,2,3,4,5])
let arr = [...set]
console.log(set)
console.log(arr)

// Set { 1, 2, 3, 4, 5 }
// [ 1, 2, 3, 4, 5 ]
```
使用 `...` 也可以達到數組去重的效果，上面其實已經有代碼了。
```js
let arr = [1,2,3,4,2,3]
let unique = [...new Set(arr)]
```

- 數組的 `map` 和 `filter` 也可以應用於 Set。
```js
let set = new Set([1, 2, 3])
set = new Set([...set].map(x => x*2))
console.log(set)

// Set { 2, 4, 6 }
```
```js
let set = new Set([1, 2, 3, 4, 5, 6])
set = new Set([...set].filter(x => x%2===0))
console.log(set)

// Set { 2, 4, 6 }
```
- 并集，交集，差集也很容易可以被實現
```js
let a = new Set([1,2,3])
let b = new Set([2,3,4])

const union = new Set([...a, ...b])
console.log(union)

// Set { 1, 2, 3, 4 }

const intersect = new Set([...a].filter(x => b.has(x)))
console.log(intersect)

// Set { 2, 3 }

const difference = new Set([...a].filter(x => !b.has(x)))
console.log(difference)

// Set { 1 }
```
如果想在遍歷 Set 的同時同步的改變原本的 Set，目前是沒有直接的方法。

# WeakSet
WeakSet 結構與 Set 基本一樣，也是不重複的值的集合，不過有兩個區別:

1. WeakSet 成員只能是對象(Object)，其他都不行

2. WeakSet 成員都是弱引用。意思就是說，如果其他對象都不再引用該對象，那系統就會自動回收該對象所佔有的內存空間，好像該對象根本不存在一樣。而正因為如此，所以 WeakSet 是不能遍歷的。
```js
let ws = new WeakSet()
ws.add(1)

// TypeError: Invalid value used in weak set 
```
使用 WeakSet 時，我們可以用一個數組或類數組對象來構造，而該數組(對象)的所有成員都會自動地成為 WeakSet 的成員。
```js
let obj1 = [[1,2], [3,4]]
let ws = new WeakSet(obj1)
console.log(ws)

// WeakSet { [items unknown] }
```
上面代碼中，obj1 本身是一個對象(數組)，但是注意，是 obj1 的成員成為 WeakSet 的成員，而不是 obj1 本身 ! 這就意味說，數組的成員也必須是對象才行。
```js
let obj1 = [1, 2]
let ws = new WeakSet(obj1)
console.log(ws)

// TypeError: Invalid value used in weak set
```
最後介紹下 WeakSet 有三個方法:
- `WeakSet.prototype.add(value):` 向 WeakSet 添加一個成員
- `WeakSet.prototype.delete(value):` 清除 WeakSet 中指定成員
- `WeakSet.prototype.has(value):` 返回 bool，表示 value 是否在 WeakSet 中

```js
let ws = new WeakSet()

let obj1 = {
    name: 'object one'
}

ws.add(obj1)
console.log(ws.has(obj1))

ws.delete(obj1)
console.log(ws.has(obj1))

// true
// false
```
WeakSet 是沒有 `size` 屬性的，所以無法得知 WeakSet 大小，包括 WeakSet 不能遍歷，實際上都是因為 WeakSet 中的成員對象都是弱引用的，隨時可能會消失。

WeakSet 的一個用處就是存儲 DOM 節點，而不用擔心這些節點會消失，因為永遠都被 window 對象引用著。

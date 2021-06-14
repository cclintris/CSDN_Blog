@[TOC](JavaScript 數組常用 API 整理)

# push(...args)
`push` 用於往數組添加元素。
```js
const arr = [1,2,3,4,5]
arr.push(6,7)
console.log(arr)
// [1, 2, 3, 4, 5, 6, 7]
```
# pop
`pop` 用於彈出數組最後一個元素，且返回該元素。
```js
const arr = [1,2,3,4,5]
arr.push(6,7)
let popout = arr.pop()
console.log(popout)
console.log(arr)
// 7
// [ 1, 2, 3, 4, 5, 6 ]
```
# shift
`shift` 相當於對數組左移一位的操作，效果是去除數組的首元素，並返回第一個元素。
```js
const arr = [1,2,3,4,5]
let first = arr.shift()
console.log(first)
console.log(arr)
// 1
// [ 2, 3, 4, 5 ]
```
# unshift([...args])
`unshift` 相當於對數組右移的操作，在開頭添加一個或多個新元素，並返回新數組長度。
```js
const arr = [1,2,3,4,5]
let len = arr.unshift(-1, -2)
console.log(len)
console.log(arr)
// 7
// [-1, -2, 1, 2, 3, 4, 5]
```
# join('[args]')
`join` 可將數組中的元素按照給定連接符連接成一個新的字符串。`join('')` 可以無縫連接。
```js
const arr = [1,2,3,4,5]
console.log(arr.join('!'))
// 1!2!3!4!5
```
# concat(args)
`concat` 可以用於數組的拼接，整個單字其實就是 concatenate，`concat` 會返回一個新數組，不會改變原數組。

這邊注意一下，`concat` 中的 `args` 如果也是個數組，則會將數組中的元素放進原數組中，而不是 `args` 數組本身。
```js
const arr = [1,2,3]
const newArr1 = arr.concat(4,5)
const newArr2 = arr.concat([4,5])
const newArr3 = arr.concat([4,5, [6,7]])
console.log(newArr1)
console.log(newArr2)
console.log(newArr3)

// [ 1, 2, 3, 4, 5 ]
// [ 1, 2, 3, 4, 5 ]
// [ 1, 2, 3, 4, 5, [ 6, 7 ] ]
```
# slice(start[, end])
`slice` 用於數組的截取，從下標 `start` 開始到下標 `end`為止，不包含 `end`。有以下特性：

1. 不會修改原數組，返回一個新的子數組
2. 省略 `end` 參數就是直接截到末尾
3. 支持負數下標，表示倒數第幾個元素

```js
const arr = [1,2,3,4,5,6,7]
const newArr1 = arr.slice(1, 4)
const newArr2 = arr.slice(-3, -1)
console.log(newArr1)
console.log(newArr2)

// [ 2, 3, 4 ]
// [ 5, 6 ]
```
# splice(start, [deleteCount][, value1, value2 ...])
`splice` 用於對數組進行刪除、添加、替換等操作，會直接改變原數組。
|參數|描述|
|:-----|:-----|
|start|規定從哪裡開始 刪除/添加 元素|
|deleteCount|規定刪除多少個元素，可以是 0。如果未定義，則刪除從 start 到結尾所有元素|
|[, value1, value2 ...]|要添加到數組的新元素，會加在 start 之前|

```js
const arr = [1,2,3,4,5]
arr.splice(1, 0, 7, 8, 9)
console.log(arr)
// [1, 7, 8, 9, 2, 3, 4, 5]
```
# reverse()
`reverse` 直接修改原數組成翻轉過的數組。
```js
const arr = [1,2,3,4,5]
arr.reverse()
console.log(arr)

// [5, 4, 3, 2, 1]
```
# sort()
`sort` 直接修改原數組，且按照數組元素的 ASCCI 碼從小到大做排序。我們也可以傳入比較器函數來自定義排序的方式。

比較常見的需求是對數字數組做排序，而在這種情況下需特別注意要傳入自定義的比較器，因為負數的 `-` 會使排序上負數出現在正數之後。下面看看一些例子：
```js
const arr = [1,3,6,1,9,-2,0,-8]
arr.sort((a, b) => { return a-b }) // 升序
arr.sort((a, b) => { return b-a }) // 降序
```
```js
const arr = [1,3,6,1,9,-2,0,-8]
arr.sort(function() { // 隨機打亂
    return Math.random() > 0.5 ? 1 : -1 
})
```
# indexOf(target[, from])
`indexOf` 可以查找 `target` 首次在數組中出現的位置，若有 `from` 參數則從下標 from 開始尋找(由左向右)，返回 `target` 所在下標，若未找到返回 -1。
```js
const arr = [1,2,3,3,4,5,6,7,7,8,9,10]
console.log(arr.indexOf(3))
console.log(arr.lastIndexOf(7))
console.log(arr.indexOf(100))

// 2
// 8
// -1
```

# lastIndexOf(target[, from])
`lastIndexOf` 可以查找 `target` 最後一次在數組中出現的位置，若有 `from` 參數則從下標 from 開始尋找(由右向左)，返回 `target` 所在下標，若未找到返回 -1。
```js
const arr = [1,2,3,3,4,5,6,7,7,8,9,10]
console.log(arr.indexOf(3))
console.log(arr.lastIndexOf(7))
console.log(arr.indexOf(100))

// 2
// 8
// -1
```
# includes(target[, from])
`includes` 用於查找 `target` 元素是否存在於數組，默認從頭開始找，若有 `from` 參數則從下標 from 開始，返回 bool。
```js
const arr = [1,2,3,4,5]
console.log(arr.includes(3))
console.log(arr.includes(2, 2))
// true
// false
```
# find(function(currentValue[, index][, arr])[, thisValue])
`find` 會返回通過給定的測試函數的第一個元素的值。`find` 會為數組中的每個元素都調用一次測試函數，若測試條件返回為 true 就立即返回該元素的值，之後不會再調用測試函數。若都沒有符合條件則返回 undefined。
![arr-1.png](https://ae03.alicdn.com/kf/Uaddc09c1483d419db39561bcbf5e8a794.jpg)

```js
const ages = [7, 8, 2, 18, 20]
let firstAdult = ages.find((age) => { return age >= 18 })
console.log(firstAdult)
// 18 
```
# findIndex(function(currentValue[, index][, arr])[, thisValue])
`findIndex` 跟 `find` 基本一樣，只不過返回的是不是元素本身，而是元素下標。
![arr-1.png](https://ae03.alicdn.com/kf/Uaddc09c1483d419db39561bcbf5e8a794.jpg)

```js
const ages = [7, 8, 2, 18, 20]
let firstAdult = ages.findIndex((age) => { return age >= 18 })
console.log(firstAdult)
// 3
```
# forEach(function(currentValue[, index][, arr])[, thisValue])
`forEach` 方法用於調用數組中的每個元素，並將元素傳遞給回調函數。
![arr-1.png](https://ae03.alicdn.com/kf/Uaddc09c1483d419db39561bcbf5e8a794.jpg)
```js
const arr = [1,2,3,4,5]
const addone = []
arr.forEach((val) => {
    addone.push(val+1)
})
console.log(addone)
// [ 2, 3, 4, 5, 6 ]
```
用於對象元素的數組也有很漂亮的用法：
```js
const UserRoute = {
    path: '/user',
    name: 'User',
    children: [
        {
            path: '',
            name: 'Login'
        },
        {
            path: 'register',
            name: 'Register'
        }
    ]
}
const HomeRoute = {
    path: '/home',
    name: 'Home'
}
const router = []
router.push(UserRoute, HomeRoute)

const allRoute = []
router.forEach(({path, children}) => {
    if(children) {
        children.forEach(({path: cpath}) => {
            allRoute.push(`${path}${cpath ? '/' + cpath : ''}`)
        })
    } else {
        allRoute.push(path)
    }
})

console.log(allRoute)
// [ '/user', '/user/register', '/home' ]
```
# map(function(currentValue[, index][, arr])[, thisValue])
`map` 會依照給定的函數對每個元素調用一次，且不會改變原數組，返回經過處理後的新數組。
![arr-1.png](https://ae03.alicdn.com/kf/Uaddc09c1483d419db39561bcbf5e8a794.jpg)

```js
const arr = [1,2,3,4,5]
let twoTimesArr = arr.map((val) => { return val*2 })
console.log(twoTimesArr)
// [ 2, 4, 6, 8, 10 ]
```
# filter
`filter` 會依照給定的函數對每個元素調用一次，過濾掉測試條件返回為 false 的元素，且不會改變原數組，返回經過過濾後的新數組。
![arr-1.png](https://ae03.alicdn.com/kf/Uaddc09c1483d419db39561bcbf5e8a794.jpg)

```js
const arr = [1,2,3,4,5,6,7,8,9,10]
let even = arr.filter((val) => { return val%2===0 })
console.log(even)
// [ 2, 4, 6, 8, 10 ]
```
# reduce(function(total, currentValue[, currentIndex][, arr])[. initialValue])
`reduce` 接受一個函數作為累加器，數組中每個值開始縮減，最後計算為一個值。
![arr-2.png](https://ae04.alicdn.com/kf/Ubfea4ab1e1314cc28cb657cdab397e99w.jpg)

```js
// 累加數組之和
const arr = [1,2,3,4,5,6,7,8,9,10]
const total = arr.reduce((res, cur) => { return res + cur })
console.log(total)
// 5
```
```js
// 格式化金錢
const money = '1234567890'
function formatCash(str) {
    return str.split('').reverse().reduce((res, cur, index) => {
        return ((index % 3 === 2) ? ',' + cur : cur) + res
    })
}
console.log(formatCash(money))
// 1,234,567,890
```
# every(function(currentValue[, index][, arr])[, thisValue])
`every` 用於檢測數組中所有元素是否都滿足給定的測試函數條件，若其中一個不符合條件則返回 false，否則返回 true。
![arr-1.png](https://ae03.alicdn.com/kf/Uaddc09c1483d419db39561bcbf5e8a794.jpg)

```js
const arr = [1,2,3,4,5,6]
console.log(arr.every((val) => {
    return val % 2 === 0
}))
// false
```
# some(function(currentValue[, index][, arr])[, thisValue])
`some` 用於檢測數組中是否有滿足給定的測試函數條件的元素，有則返回 true，否則 false。
![arr-1.png](https://ae03.alicdn.com/kf/Uaddc09c1483d419db39561bcbf5e8a794.jpg)

```js
const arr = [1,2,3,4,5,6]
console.log(arr.some((val) => {
    return val > 7
}))
// false
```

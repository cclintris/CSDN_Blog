@[TOC](ES6 數據結構 Map)

# Map 結構的目的和基本用法
js 中的對象(Object)，本質上是鍵值對的集合，但是一般只能用字符串當作鍵，這給使用上帶來了一定的限制。
```js
let data = {}
let element = document.getElementById('myDiv')

data.element = 'node'
data['[object HTMLDivElement]'] // 'node'
```
上面代碼的原意其實是希望可以把 id 為 `myDiv` 的 DOM 節點作為 data 對象的屬性，而對應的值是 node。但是，由於對象只接受字符串作為鑑名，所以 `element` 被自動轉換為字符串 `'[object HTMLDivElement]'`，這顯然不是我們所希望的。

為了解決這樣的問題，ES6 提供了 Map 數據結構。它類似於對象，也是鍵值對的集合，不過鍵的約束不局限於字符串了，甚麼都可以作為 Map 的鍵。

簡單來說，Object 提供了一個 `string-value` 的一一對應結構，而 Map 則是提供了 `value-value` 的一一對應結構，更加完善方便。如果本質上需要的是鍵值對的數據結構，因為不一定能確定鍵的類型，所以其實 Map 會比 Object 更加合適。先看一點代碼基本用法，下面都還會一一再做整理。
```js
let map = new Map()

const obj1 = {
    name: 'object 1'
}

map.set(obj1, 'id1')
console.log(map.get(obj1))
// id1
console.log(map.has(obj1))
// true
console.log(map.delete(obj1))
// true
console.log(map.has(obj1))
// false
```
上述代碼中，我們用了 `set` 將對象 `obj1` 作為 map 的鍵，並用 `get` 根據鍵獲得值，用 `has` 檢查 map 中是否存在該鍵，用 `delete` 刪除鍵。

我們也可以發現，創建出一個 Map 實例其實是通過 Map 構造函數，而作為構造參數，Map 也可以接受一個數組作為參數，該數組的成員會被一個個解析成為 Map 的鍵值對。
```js
let map = new Map([
    ['name', 'csd'],
    ['description', 'csdnb']
])

console.log(map)

// Map { 'name' => 'csd', 'description' => 'csdnb' }
```
上面代碼在新建 Map 實例時，就指定了兩個鍵 `name`, `description`。其實呢，將數組作為 Map 構造函數的參數，實際上是做了這件事:
```js
const obj1 = [
    ['name', 'csd'],
    ['description', 'csdnb']
]

let map = new Map()

obj1.forEach(([key, value]) => {
    map.set(key, value)
})
```
如果對同一個鍵多次覆值，就會覆蓋先前的值。
```js
let map = new Map();

map.set(1, 'aaa')
map.set(1, 'bbb')

console.log(map.get(1))

// bbb
```
讀取一個不存在的鍵則會返回 `undefined`。
```js
new Map().get('unknown')
// undefined
```
不過這邊要很注意一點，就是當我們使用對象作為 Map 的鍵時，是對同一對象的引用才會指向同一個值。甚麼意思，看下面代碼:
```js
let map = new Map()

map.set(['a'], 1)
map.set(['a'], 2)
console.log(map)

// Map { [ 'a' ] => 1, [ 'a' ] => 2 }
```
上面我們將對象 `['a']` 兩次用於設置 Map 的鍵，理論上好像應該要覆蓋前一次的 `set` 操作，但是打印出來的結果顯然不是。這就是因為對於 js，對象都是不一樣的，上面兩次的 `['a']` 雖看起來完全一樣，但其實在堆內存中都有各自的內存地址的，因此對 Map 來說會被視為兩個不同的鍵。
```js
let map = new Map()

map.set(['a'], 1)
console.log(map.get(['a']))

// undefined
```
上面例子中，表面上看起來是在訪問一個已經存在的 `['a']` 鍵，但是就是因為實際上不是一個對象，所以返回的是 undefined。

同理，所以即使是兩個同樣的值，只要是兩個實例，就會被 Map 視為不同的鍵。所以其實可以知道，Map 的鍵實際上是跟內存地址綁定的，只要內存地址不一樣，就會被 Map 視為兩個不同的鍵。關於 js 的內存分配這邊簡單總結下：在 js 中，對象的生成都會放在堆上，其他則在棧上，而對於一個對象，無論其值或是說內容是否一樣，系統都會為其開闢另一塊堆空間，但是對於存在棧上的東西來說，如果值一樣，即使名不一樣，實際上也都只是將指針(引用)指向原本就存在的那個東西的地址，換言之，本質上就是一個東西。至於字符串(String)，則相對特殊一點，它是在棧上的東西，不過行為卻又跟在堆上的東西一樣。

# Map 屬性和操作方法
接下來介紹下 Map 的屬性以及一些操作，也算是一個整理。

## size 屬性
size 屬性返回 Map 實例成員個數。
```js
let map = new Map()

map.set('name', 'kerr')
map.set('age', 50)
console.log(map.size)

// 2
```
## set(key, value)
set 方法可以設置 Map 實力的鍵值對，然後返回新的 Map 實例。若已存在，則覆蓋先前的值，否則新建鍵值對。
```js
let map = new Map()

map.set("edition", 6)        // 键是字符串
map.set(262, "standard")     // 键是数值
map.set(undefined, "nah")    // 键是undefined
console.log(map)

// Map { 'edition' => 6, 262 => 'standard', undefined => 'nah' }
```
set 方法返回值就是 Map 結構，因此支持鏈式寫法。
```js
let map = new Map()

map.set(1, 'a').set(2, 'b').set(3, 'c')
```
## get(key)
get 方法獲取對應 `key` 的值，若不存在則返回 undefined。
```js
let map = new Map()

map.set("edition", 6)
console.log(map.get('edition'))
console.log(map.get('hello'))

// 6
// undefined
```
## has(key)
has 方法返回一個 bool 值，檢查 `key` 是否存在於 Map 結構中。
```js
let map = new Map()

map.set("edition", 6);
map.set(262, "standard");
map.set(undefined, "nah");

map.has("edition")     // true
map.has("years")       // false
map.has(262)           // true
map.has(undefined)     // true
```
## delete(key)
delete 方法刪除指定的鍵，當然其值也連帶被刪除，相當於刪除整個鍵值對。若刪除成功則返回 true，否則 false。
```js
let map = new Map()

map.set('edition', 6)
map.set(262, 'standard')
console.log(map)
map.delete('edition')
console.log(map)

// Map { 'edition' => 6, 262 => 'standard' }
// Map { 262 => 'standard' }
```
## clear()
clear 方法刪除所有 Map 結構成員，沒有返回值。
```js
let map = new Map()

map.set('edition', 6)
map.set(262, 'standard')
console.log(map)
map.clear()
console.log(map)

// Map { 'edition' => 6, 262 => 'standard' }
// Map {}
```
## 遍歷方法
Map 本身提供了三個遍歷器生成函數，以及一個遍歷方法。
- `keys():` 返回鍵名的遍歷器
- `values():` 返回鍵值的遍歷器
- `entries():` 返回鍵值對的遍歷器
- `forEach():` 遍歷 Map 的每個成員

來看看怎麼使用吧:
```js
let map = new Map([
    ['js', 'hard'],
    ['java', 'medium'],
    ['python', 'easy']
])

for(let key of map.keys()) {
    console.log(key)
}
// js
// java
// python

for(let value of map.values()) {
    console.log(value)
}
// hard
// medium
// easy

for(let [k ,v] of map.entries()) {
    console.log(k, v)
}
// js hard
// java medium
// python easy

map.forEach((v, k) => {
    console.log(v, k)
})
// hard js
// medium java
// easy python
```
這邊注意使用 `forEach` 的話，參數是先值再鍵。(我覺得很怪，但也不知道幹嘛這樣設計)

有時候我們也需要將 Map 轉換為數組結構:
```js
let map = new Map([
    [1, 'hard'],
    [2, 'medium'],
    [3, 'easy']
])

const keys = [...map.keys()]
const values = [...map.values()]
const entries = [...map.entries()]

console.log(keys)
console.log(values)
console.log(entries)

// [ 1, 2, 3 ]
// [ 'hard', 'medium', 'easy' ]
// [ [ 1, 'hard' ], [ 2, 'medium' ], [ 3, 'easy' ] ]
```
而這麼一來 Map 也可以結合數組的方法，像是 `map`, `filter` 等都很常用，Map 自己本身是沒有這些方法的。
```js
let map = new Map([
    [1, 'hard'],
    [2, 'medium'],
    [3, 'easy']
])

let map1 = new Map(
    [...map].filter(([k, v]) =>  k % 2 === 0 )
)
console.log(map1)
// Map { 2 => 'medium' }

let map2 = new Map(
    [...map].map(([k, v]) => [k*2, v+' learn'])
)
console.log(map2)
// Map { 2 => 'hard learn', 4 => 'medium learn', 6 => 'easy learn' }
```
## 與其他數據結構相互轉換

### Map => 數組
前面應該也都用到了，用 `...` 很方便。
```js
let map = new Map([
    [1, 'one'],
    [2, 'two'],
    [3, 'three']
])

let arr = [...map]
console.log(arr)

// [ [ 1, 'one' ], [ 2, 'two' ], [ 3, 'three' ] ]
```
### 數組 => Map
數組想要變為 Map 就直接當作參數丟進 Map 構造函數就好了。
```js
let arr = [ [ 1, 'one' ], [ 2, 'two' ], [ 3, 'three' ] ]
let map = new Map(arr)
console.log(map)

// Map { 1 => 'one', 2 => 'two', 3 => 'three' }
```

### Map => Object
如果所有 Map 的鍵都是字符串，就可以轉換成對象。
```js
let map = new Map([
    ['one', 1],
    ['two', 2],
    ['three', 3]
])

function map2Obj(map) {
    let obj = {}
    for(let [k, v] of map.entries()) {
        obj[k] = v
    }
    return obj
}

let obj = map2Obj(map)
console.log(obj)

// { one: 1, two: 2, three: 3 }
```

### Object => Map
```js
let obj = {
    one: 1,
    two: 2,
    three: 3
}

function obj2Map(obj) {
    let map = new Map()
    for(let key of Object.keys(obj)) {
        map.set(key, obj[key])
    }
    return map
}

let map = obj2Map(obj)
console.log(map)

// Map { 'one' => 1, 'two' => 2, 'three' => 3 }
```

### Map => JSON
Map 轉為 json 需要區分兩種情況。第一種，如果 Map 中的鍵都是字符串，那可以直接轉為 json object。
```js
let map = new Map([
    ['one', 1],
    ['two', 2],
    ['three', 3]
])

function map2JsonObj(map) {
    function map2Obj() {
        let obj = {}
        for(let [k, v] of map.entries()) {
            obj[k] = v
        }
        return obj
    }
    return JSON.stringify(map2Obj())
}

let jsonObj = map2JsonObj(map)
console.log(jsonObj)

// {"one":1,"two":2,"three":3}
```
但如果 Map 中的鍵不完全是字符串的話，就應該要轉換為 json 數組。
```js
let map = new Map([
    ['one', 1],
    [{ name: 'foo'}, 'string']
])

function map2JsonArray(map) {
    return JSON.stringify([...map])
}

let jsonArr = map2JsonArray(map)
console.log(jsonArr)

// [["one",1],[{"name":"foo"},"string"]]
```
### JSON => Map
json 轉 Map 通常情況下都是 json Object，所以鍵也都是字符串。
```js
let jsonObj = '{ "one": 1, "two": 2, "three": 3 }'

function jsonObj2Map(jsonObj) {
    function obj2Map(obj) {
        let map = new Map()
        for(let key of Object.keys(obj)) {
            map.set(key, obj[key])
        }
        return map
    }
    return obj2Map(JSON.parse(jsonObj))
}

let map = jsonObj2Map(jsonObj)
console.log(map)

// Map { 'one' => 1, 'two' => 2, 'three' => 3 }
```
# WeakMap
WeakMap 跟 Map 基本一樣，唯一區別就在於 WeakMap 只能接受對象作為鍵，而且鍵名所指向的對象不計入 js 的垃圾回收機制。

WeakMap 的設計初衷在於，鍵名是對像的弱引用的，所以 js 的垃圾回收機制不會考慮這些對象，所以其所指向的對象有可能會被自動回收，而當對象被回收後，WeakMap 也會自動移除該鍵值對。

一個典型應用是，DOM 元素的的 WeakMap，當某 DOM 節點被移除，WeakMap 也會相應的自動移除其鍵值對。WeakMap 的使用場合其實就是，當其中的鍵在未來有可能會自動消失，而使用 WeakMap 可以更好的預防一些內存上的洩漏，或是程序邏輯上的錯誤。

WeakMap 的 API 跟 Map 的區別主要在於，WeakMap 是不能遍歷的，因為大小隨時有可能動態變化，當然也就沒有 `size` 屬性，且也不支持 `clear` 操作。因此，WeakMap 擁有的操作就是，`set`, `get`, `has`, `delete`。
```js
let wm = new WeakMap()

let element = document.getElementById('button')
wm.set(element, { clicked: 0 })

element.addEventListener('click', function() {
    wm.get(element).clicked++
})
```
上面代碼做的就是，將一個 DOM 節點對象作為 WeakMap 的一個鍵，對應的值也是一個對象，其中有一個屬性 `clicked` 表示該 DOM 節點被點擊的次數，初值為 0。每當發生點擊事件，就要更新一下狀態。而一旦該 DOM 節點消失，此鍵值隊也會從 WeakMap 中自動消失，不存在內存洩漏的風險。

最後再來看一個 WeakMap 的使用場景。WeakMap 還可以用於部屬私有屬性。
```js
let _counter = new WeakMap()

class Countdown {
    constructor(count) {
        _counter.set(this, count)
    }

    minus() {
        let count = _counter.get(this)
        if(count === 0) {
            console.log('Time is up')
            return
        }else {
            console.log(`${count} left`)
        }
        count--
        _counter.set(this, count)
    }
}

let cd = new Countdown(3)
cd.minus()
cd.minus()
cd.minus()
cd.minus()

// 3 left
// 2 left
// 1 left
// Time is up
```
上面代碼的 Countdown 類中，設置了一個私有變量 `count`，因為利用了 WeakMap 使之與 this 綁定，因此可以說 `count` 其實是一個 Countdown 的弱引用。一旦實例 cs 消亡，這個 `count` 也就隨之消失，不會洩漏內存。

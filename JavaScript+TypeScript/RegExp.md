@[TOC](JavaScript 中的正則表達式 Regular Expression)

# 前言
正則表達式這個東西一直都有聽過的，不管在學習什麼語言都有正則表達式的學習，無奈本人之前學習都有點不求甚解，所以對於正則表達式這個東西一直沒有很

# 正文

## 什麼是正則表達式？
正則表達式(regular expression)描述了一種字符串匹配的模式，可以用來檢查一個字符串是否含有某種子串、將匹配的子串做替換或者從某個字符串中取出符合某個條件的子串等，整體來說是超級強大的。

說白了正則表達式就是處理字符串的,我們可以用它來處理一些複雜的字符串。

不過可能會問說為什麼要學習正則？直接看一個例子你就知為啥了。假設給一個字符串，要找出字符串中所有的數字子串，來看看用與不用正則的差別有多大：

```js
const str = "abc123de45fgh6789qqq111";

// 不使用正則
function findNumSubstr(str) {
  const res = [];
  let tmp = "";
  for (let i = 0; i < str.length; i++) {
    let cur = str[i];
    if (!isNaN(cur)) {
      tmp += cur;
    } else {
      if (tmp) {
        res.push(tmp);
        tmp = "";
      }
    }
  }

  if (tmp) {
    res.push(tmp);
    tmp = "";
  }
  return res;
}

console.log(findNumSubstr(str));
// [ '123', '45', '6789', '111' ]

// 正則
let reg = /\d+/g
console.log(str.match(reg))
// [ '123', '45', '6789', '111' ]
```
可以看到正則超級強大，短短一行就處理完了我們自己寫的函數算法，且效率也高很多，又少了一堆多餘代碼。所以雖然正則看起來像火星文一樣都是亂碼，但還是很有必要去學。

## 創建正則表達式
首先我們要先創建正則表達式，而創建正則表達式有兩種方式：

|字面量創建|實例創建|
|:-----|:-----|
|`const reg = /pattern/flags`|`const reg = new RegExp(pattern, flags)`|

|參數|描述|
|:-----|:-----|
|pattern|正則表達式|
|flags|標示|

標示主要包括：

1. i 忽略大小写匹配
2. m 多行匹配，即在到達一行文本末尾時還會繼續巡尋找下一行中是否與正則匹配的項
3. g 全局匹配，模式應用於所有字符串，而非在找到第一個匹配項時停止

## 元字符
這邊來整理下正則的元字符，算是正則中的一些基本規則，寫法吧～

### 代表特殊含義的元字符

|元字符|描述|
|:-----|:-----|
|`\d`|0～9的任意數字，\d只占一個位置|
|`\w`|數字、字母、下滑線 (0~9, a~z, A~Z, _ )|
|`\s`|空格或是空白|
|`\D`|除了 \d|
|`\W`|除了 \w|
|`\S`|除了 \s|
|`.`|除了 \n 之外的任意字符|
|`\`|轉譯字符|
|`()`|分組|
|`\n`|匹配換行符|
|`\b`|匹配邊界，字符串的開頭和結尾，空格的兩邊也算邊界|
|`^`|限定開始位置|
|`$`|限定結束位置|
|`[a-z]`|任意字母，`[]` 中表示任意一個都可以|
|`[^a-z]`|任意非字母，`[]` 中的 `^` 表示除了|
|`[abc]`|abc 任意字母其中一個|
|`[^abc]`|除了 abc 的任意字符|

### 代表次數 - 量詞元字符

|元字符|描述|
|:-----|:-----|
|`*`|0到多個|
|`+`|1到多個|
|`?`|0次或1次，可有可無|
|`{n}`|正好 n 次|
|`{n,}`|n 到多次|
|`{n,m}`|n 到 m 次|

量詞元字符會出現在特殊含義元字符之後，比如說 `\d+` 表示出現多個數字，效果就是實現前面的特殊含義元字符出現的次數。來看看下面例子感受一下：

```js
const str = '1223334444';
const res = str.match(/\d{2}/g)
console.log(res)
// [ '12', '23', '33', '44', '44' ]
```
```js
const str = '   cclin  hello '
const res = str.replace(/^\s+|$\s+/g, '')
console.log(res)
// cclin  hello 
```
### 特殊情況

下面來說說一些比較特殊的情況：

- `[]`

一般來說，`[]` 中的字符沒有特殊意義，比如說 `+` 就是 `+`，但是 `\w` 這種 `\` 開頭的還是算是有特殊含義的。

```js
const str1 = 'abc';
const str2 = 'dbc';
const str3 = '.bc';

const reg = new RegExp(/[ab.]bc/g)
console.log(reg.test(str1))
console.log(reg.test(str2))
console.log(reg.test(str3))

// true
// false
// true
```
還有一個點就是 `[]` 中不能出現兩位數。什麼意思呢？我們都知道 [0-9] 的意思就是 0 到 9 任意一個，那如果今天我有一個需求是要對一個字符傳測試是不是介於 18 到 65(青壯年)，可能會想這樣寫：

```js
const age = '50'
const ageReg = new RegExp(/[18-65]/g)
console.log(ageReg.test(age))
// SyntaxError: Invalid regular expression: /[18-65]/: Range out of order in character class
```
這就是 `[]` 中不可以有兩位數的意思。對於上述這個需求，我們就必須拆成三部分來寫，分別是 (18, 19), (20, 59), (60, 65)。

```js
const age1 = '50'
const age2 = '70'
const ageReg = new RegExp(/(18,19)|([2-5]\d)|(6[0-5])/g)
console.log(ageReg.test(age1))
// true
console.log(ageReg.test(age2))
// false
```
## 正則的特性
 
- 貪婪性

所謂的貪婪性就是正則在捕獲時，每一次會盡可能多的去捕獲符合條件的內容。如果我們想盡可能的少的去捕獲符合條件的字符串的話，可以在量詞元字符後加 `?`。

- 懶惰性

懶惰性則是正則在成功捕獲 `一次` 後不管後邊的字符串有沒有符合條件的都不再捕獲。如果想捕獲目標中所有符合條件的字符串的話，我們可以用標識符 `g `來標明是全局捕獲。

## 正則方法
上面介紹了關於正則的一些基礎和概念，這邊我們就來看看一些使用正則的方法，其實有些在上面的代碼都看過啦～

### str.search(reg)
`search` 方法用於搜索符合正則規則的字符串，找到了就會返回檢索到的字符串的 index，找不到就會返回 -1。

```js
const str = 'aaa124BBB789'
console.log(str.search(/\d+/))
// 3
```
你可能會想如果加上 `g` 那是不是 `search()` 就會返回所有符合正則規則的匹配字符串各自開頭的 index？其實不會。`search()` 方法不執行全局匹配，它將忽略標誌 `g`，並且總是從字符串的開始進行檢索，這意味著它總是返回 str 的第一個匹配的位置。

### reg.test(str)

`reg.test(str)` 用於驗證字符串是否符合正則的規則，是就返回 true，否則返回 false。

```js
const str = 'abc'
const reg = new RegExp(/\w+/g)
console.log(reg.test(str))
// true
```
### reg.exec(str)

`reg.exec(str)` 用於捕獲符合正則規則的字符串，會返回一個詳細信息的數組。

`reg.exec()` 這個方法還有一個特別的屬性，就是 `lastIndex`。這個屬性紀錄了最後一次查找的下標，也就是下次開始查找的下標位置。也就是說，`reg.exec()` 一次只會捕獲到一個目標字符串，然後當再次調用 `reg.exec()` 時就會從 `lastIndex` 開始繼續找直到沒有。當沒有捕獲到任何結果時就會返回 null。

至於這個 `lastIndex` 如何變化則是根據我們有沒有設置標示符，如果我們把標示符邵設為 `g`，那每次 `lastIndex` 就會停在上次捕獲完的位置，再次執行 `reg.exec()` 則從 `lastIndex` 繼續找，直到返回 null。但如果沒設置每次就都會從頭開始找，也就表示每次執行的結果將會都一樣。這邊也體現出了正則的懶惰性。

- 局部 `exec`
```js
const str = 'abc123cba456aaa789'
const reg = new RegExp(/\d+/)
console.log(reg.exec(str))
console.log(reg.lastIndex)

// [ '123', index: 3, input: 'abc123cba456aaa789', groups: undefined ]
// 0
```
- 全局 `exec`
```js
const str = 'abc123cba456aaa789'
const reg = new RegExp(/\d+/g)

console.log(reg.exec(str))
console.log(reg.lastIndex)

console.log(reg.exec(str))
console.log(reg.lastIndex)

console.log(reg.exec(str))
console.log(reg.lastIndex)

console.log(reg.exec(str))
console.log(reg.lastIndex)

// [ '123', index: 3, input: 'abc123cba456aaa789', groups: undefined ]
// 6
// [ '456', index: 9, input: 'abc123cba456aaa789', groups: undefined ]
// 12
// [ '789', index: 15, input: 'abc123cba456aaa789', groups: undefined ]
// 18
// null
// 0
```
可以看到 `exec()` 全局與不全局的區別，所以也可以通過如下的形式遍歷整個字符串：

```js
const str = 'abc123cba456aaa789'
const reg = new RegExp(/\d+/g)

while(let tmp = reg.exec(str) !== null) {
  // ,,,
}

// or

while(let lastIndex = reg.exec(str).lastIndex !== 0) {
  // ,,,
}
```

### str.match(reg)

`str.match(reg)` 如果匹配成功就會返回匹配成功的數組，其中就會有第一個被正則匹配到的字符串，否則就返回 null。

```js
const str = 'abc123cba456aaa789'
const reg = new RegExp(/\d+/)

console.log(str.match(reg))
// [ '123', index: 3, input: 'abc123cba456aaa789', groups: undefined ]
```
`match` 方法跟 `exec` 在上面的例子看起來是一樣沒錯，不過如果加上標示符 `g` 則不太一樣，來看看：

```js
const str = 'abc123cba456aaa789'
const reg = new RegExp(/\d+/g)

console.log(str.match(reg))
// [ '123', '456', '789' ]
```
當全局匹配時，`match` 方法會一次性把符合匹配條件的字符串全部捕獲到數組中，如果想用 `exec` 來達到同樣的效果需要執行多次 `exec` 方法。

### str.replace(reg, [newStr, function])
`replace` 這個方法超好用，也很直白，就是根據你給定的正則匹配，並將匹配的字串用 `newStr` 替換掉。直接來看看一些用法：

```js
const str = 'a111bc222de'
const newStr = str.replace(/\d/g, 'G')
console.log(newStr)
// aGGGbcGGGde

const newStr = str.replace(/\d/, 'G')
console.log(newStr)
// aG11bc222de
```
第二個參數也可以放一個函數，其中函數的參數就是被匹配到的每個字符串，return 的值就會被當作替換的值使用：
```js
const str = 'aaaBBBcccDDD'
const newStr = str.replace(/[a-z]*/g, function(lower) {
    return lower.toUpperCase()
})
console.log(newStr)
// AAABBBCCCDDD
```
來看看一個常見的需求，用來處理 url:
```js
function queryString(url) {
  let obj = {},
    reg = /([^?&#+]+)=([^?&#+]+)/g;
  url.replace(reg, function ($0, $1, $2) {
    obj[$1] = $2;
  });
  return obj;
}

const url = "https://www.baidu.com?a=1&b=2";
console.log(queryString(url));

// a=1
// a
// 1
// b=2
// b
// 2
// { a: '1', b: '2' }
```
可以看到，我們在第二個參數中使用了很像 linux 語法的參數 `$0`。`$0` 代表的就是匹配到的字符串，而後續的 `$1`, `$2` ... 就是根據你的分組所對應的元素(如果有分組`()`)。

# 結語
這篇算是入門了 js 中的正則表達式，介紹了個人感覺可能最常用的幾個方法。確實在整理學習的過程中可以感受到這個東西有多強大，不過感覺要玩得很 6 還是需要很多的經驗。以後在項目中應該要提醒自己不要每次處理字符串就馬上開始無腦寫函數硬處理，而應該想想是否可以使用正則處理，以後還多多加油吧～

# 參考
|參考|鏈接|
|:-----|:-----|
|w3school|https://www.w3school.com.cn/jsref/jsref_exec_regexp.asp|
|JavaScript 正则表达式|https://www.runoob.com/js/js-regexp.html|


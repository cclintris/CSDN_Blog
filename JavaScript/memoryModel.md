@[TOC](理解 JavaScript 的內存模型)

# 前言
作為一名程序員，無論用什麼語言，每天我們都在聲明變量，初始化變量，給變量重新賦值，這都是我們的日常工作。但是，我們真的知道 js 如何在內部，在底層處理這些事情嗎？還是用完就無所謂了。要成為一名好的程序員，這些還是都要知道的，這篇就來談談 js 的內存模型以及其工作方式。

# 正文
以下分四個部分來探討這個問題:

- js 原始類型變量聲明與賦值
- js 內存模型
- js 非原始類型變量聲明與賦值
- `let` vs `const`

## js 原始類型變量聲明與賦值
先從一個簡單的例子開始。
```js
let myNumber = 123
```
執行這段代碼，js 其實會依序為我們做好以下事情:

1. 為變量創建一個唯一的標識符 `myNumber`
2. 在內存中分配一個地址(運行時動態分配)，`myNumber` 指向該地址
3. 在分配好的地址上存儲它的值 123

![](https://wtfhhh.oss-cn-beijing.aliyuncs.com/memmodel-1.png)

這邊就要澄清一下了。我們常常就會說，`myNumber` 等於 23，其實更精確是說，`myNumber` 等於存儲著 23 這個值得地址。

接著，我們如果創建一個 `newVar`，並把 `myNumber` 賦值給它，會怎麼樣呢?
```js
let newVar = myNumber
```
由於 `myNumber` 實際上是指向 `0012CCGWH80`，而不是直接就等於 23，所以 `newVar` 當然也就是指向 `0012CCGWH80`，也就意味著，`newVar` 跟 `myNumber` 是相等的，且都指向同一個內存地址，其存儲著 23。
![](https://wtfhhh.oss-cn-beijing.aliyuncs.com/memmodel-2.png)

那如果今天我們改變一下 `myNumber`，會發生什麼事呢?
```js
myNumber += 1
```
當然 `myNumber` 會變成 24(廢話)，但是既然 `myNumber` 和 `newVar` 都指向同一個內存地址，`newVar` 也會變嗎?

<strong>答案是不會的 !</strong> 原因是因為 js 的原始數據類型是不可變的，也就是說，一旦一個原始數據類型在內存中分配好地址，這個地址所存儲的那個值將不可以再改變。所以，`myNumber++` 時，其實 js 會在內存中開闢一個新的地址，把 24 給存下來，同時把 `myNumber` 的指向重定向到新的這個地址上。
![](https://wtfhhh.oss-cn-beijing.aliyuncs.com/memmodel-3.png)

再來看看一個例子:
```js
let myString = 'abc'
myString += 'd'
```
String 類型一直是一個很特別的類型，所以總是多留意一下。在 js 中，String 類型也被認為是原始數據類型，因此規則也跟上面一樣。
![](https://wtfhhh.oss-cn-beijing.aliyuncs.com/memmodel4.png)

下面我們要來更深入理解原始數據類型的內存分配是如何發生的。

## js 內存模型
簡單來說，我們可以將 js 的內存模型理解為兩大區域，堆和棧。
![](https://wtfhhh.oss-cn-beijing.aliyuncs.com/memmodel-5.png)

棧是原始數據類型存儲的地方，前面聲明的變量在棧中大概就像下面這樣:
![](https://wtfhhh.oss-cn-beijing.aliyuncs.com/memmodel-6.png)

這邊還是要注意，雖然上圖為了方便，抽象了變量在內存中的實際儲存，但是還是要記得，變量實際上是指向內存的地址，而內存地址存儲對應值，而非變量本身就是那個值，變量其實只是一個標識符，指向一個內存地址。這個一定要搞明白，因為這會成為理解 `let` 和 `const` 的關鍵。

接下來要說的就是堆。堆是存儲非原始數據類型的地方，堆最大的特色就是可以 <strong>無序的</strong> 存儲數據，且可以動態增長，也就特別適合數組以及對象。

## js 非原始類型變量聲明與賦值
跟原始數據類型比，非原始數據類型在行為上有很大的不同。我們用下面例子來說明:
```js
let myArray = []
```
當我們聲明 `myArray` 變量並賦值為非原始數據類型，如 `[]` 時，系統其實會幫我們做以下事情:

1. 為變量創建一個唯一的標識符 `myArray`
2. 在棧內存中分配一個地址(運行時動態分配)，`myArray` 指向該地址
3. 在堆內存中分配一個由棧內存所存儲的值指向的內存地址(運行時動態分配)
4. 在分配好的堆內存地址上存儲 `[]`

![](https://wtfhhh.oss-cn-beijing.aliyuncs.com/memmodel-7.png)
![](https://wtfhhh.oss-cn-beijing.aliyuncs.com/memmodel-8.png)

如上圖所示，其實對於非原始數據類型，js 在存儲時其實是一個二次尋址的過程，變量 `myArray` 仍舊指向一個棧地址，而該棧地址所存儲的數據又是堆內存中的另一個地址，也就是實際上數據 `[]` 所存儲的地方，也就在堆上。

## `let` vs `const`
好的習慣是，我們要盡可能的使用 `const`，只有在變量可能改變時我們才用 `let`。但是這邊我們要釐清的就是所謂「改變」指的是什麼。

如果把所謂「改變」理解為值或是內容的改變，將會發生問題。這樣理解的 js 程序員，可能會寫出以下代碼:
```js
let sum = 0
sum = 1+2+3
let numbers = new Array()
numbers.push(1)
numbers.push(2)
numbers.push(3)
```
用 `let` 聲明 sum 顯然沒問題，因為 sum 確實改變了。但是他錯誤的使用了 `let` 聲明 numbers，因為他覺得往 numbers 中 push 東西相當於改變了 numbers 的內容。但這是不對的理解。

其實真正意義上的「改變」應該被理解成「改變內存地址」。`let` 允許改變內存地址，而 `const` 不允許。
```js
const importantID = 489
importantID = 100
// TypeError: Assignment to constant variable.
```
當我們聲明 importantID 時，系統就幫我們分配好一個棧內存地址，存儲 489，並把 importantID 指向該地址，一切都跟上面說的一樣。
![](https://wtfhhh.oss-cn-beijing.aliyuncs.com/memmodel-9.png)
接下來，當我們要重新賦值，把 100 賦值給 importantID 時，因為 100 本身是一個原始數據類型，所以會再開闢另一個棧內存地址來存儲 100，並要把 importantID 重新指向這個新的地址。

然而，這時候 js 因為看到了我們聲明 importantID 是一個 `const` 變量，所以就會阻止這樣的重定向，相當於 `const` 阻止 importantID 指向別的地址。其實這也正合我們意，因為我們其實也不希望 importantID 被改變，所以限制它永遠只能指向初始地址。 
![](https://wtfhhh.oss-cn-beijing.aliyuncs.com/memmodel-10.png)
回到前面的疑問，說用 let 來聲明數組是不標準的理解，相反的應該要用 `const`。個人覺得，這有點違反我們的直覺，因為總是會認為，數組要能改變才有用阿，用了 `const` 不就禁止它改變了嗎 ? 這時候，我們就要再一次強調，所謂的「改變」是指改變內存地址。繼續看下去，更深入去理解為甚麼應該用 `const` 聲明數組，即使感覺很怪。
```js
const myArray = []
```
如果上面有理解的話，因為數組是非原始數據類型，所以如上聲明內存空間大概如下:
![](https://wtfhhh.oss-cn-beijing.aliyuncs.com/memmodel-11.png)
接下來，如果我們執行以下操作:
```js
myArray.push(1)
myArray.push(2)
myArray.push(3)
myArray.push(4)
myArray.push(5)
```
![](https://wtfhhh.oss-cn-beijing.aliyuncs.com/memmodel-12.png)
我們看到，其實這時候已經往堆上實際的數組添加了數據，但是 myArray 本身所指向的地址根本沒有發生變化。這就是為甚麼雖然聲明 myArray 為 `const` 也沒有報錯。myArray 始終都指向 `0458AFCZX91`，而實際數據是它的值 `22VVCX011` 所指向的堆內存空間。

如果這樣做就會報錯:
```js
myArray = 3
```
因為 3 是原始數據類型，當進行重新賦值時，系統會為 3 開闢一個新的空間，而當要把 myArray 重定向到這塊內存地址時，就會被 `const` 給阻止。
![](https://wtfhhh.oss-cn-beijing.aliyuncs.com/memmodel-13.png)
這樣也會報錯:
```js
myArray = ['a']
```
雖然 `['a']` 是非原始數據類型，但是系統同樣會為其開闢一塊新的棧內存地址，並指向新的堆內存空間，所以依然會要將 myArray 重定向到新的棧內存地址。
![](https://wtfhhh.oss-cn-beijing.aliyuncs.com/memmodel-14.png)
同樣，我們也應該用 `const` 聲明對象，也可以添加刪除屬性，只要我們不要重新賦值都沒問題，因為只要不重新賦值，都不會要改變原本指向的內存地址，也就不是真正意義上的「改變」。
```js
const obj = {}
obj.id = Symbol('obj')

console.log(obj)
// { id: Symbol(obj) }
```
# 結語
最近都再好好的踏實學習 js 的一些基礎，希望能夠紮實的把 js 學得稍微好一點，所以像這種底層內部的機制也不免需要了解了解，對於編程或是理解 js 上都應該會有多多少少的幫助的。


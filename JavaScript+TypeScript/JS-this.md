@[TOC](搞清楚JavaScript中的this)

# 前言
前幾天突然想起一直都沒真正搞明白 js 中的 this 到底是個什麼玩意，所幸開始看看文章，開始寫一篇自己的博客，還是希望可以一勞永逸吧，對於 this 這個東西。如果你和我一樣菜的話，那就一起看看這篇文章吧!

# 正文

## Global Object (Window物件)
馬上來看看我們在瀏覽器中以不同方式 console `this` 會出現甚麼吧!這裡我們在三種環境下來呼叫 `this`，分別是 1. 在代碼的最外部(outer environment) 2. 以 function statement 執行 3. 以 function expression 執行。

```js
// outer environment
console.log(this)

// function statement
function this_func_statement() {
    console.log(this)
}
this_func_statement()

// function expression
let this_func_expression = function() {
    console.log(this)
}
this_func_expression()
```
結果顯示如下圖:
![](https://wtfhhh.oss-cn-beijing.aliyuncs.com/this-1.png)

我們可以看到，三種呼叫方式都會指向同一個物件，就是所謂的全域物件，也就是全域環境（global environment）中的 global object（在瀏覽器的環境下，global object 就是 Window 物件）

也就是說，有了 this 以及 function，我們就可以在這個 window 物件中添加任意的屬性囉!

```js
function func_windowAddattribute() {
    console.log(this)
    this.newAttribute = 'I am a new attribute.'
}

func_windowAddattribute()
```
結果顯示如下圖:
![](https://wtfhhh.oss-cn-beijing.aliyuncs.com/this-2.png)

我們看到 `newAttribute` 這個屬性被添加到 this 所指向的物件，也就是 window 啦!

## Method in Object
我們都知道，在 js 的世界中，在物件中有「屬性」(property)，還有「方法」(method)。那如果我們在物件的方法中呼叫 this，這個 this 又會指向甚麼呢?

```js
let func_call_this_inObjectMethod = {
    name: 'I am JavaScript Object.',
    log: function() {
        console.log(this)
    }
}

func_call_this_inObjectMethod.log()
```
結果顯示如下圖:
![](https://wtfhhh.oss-cn-beijing.aliyuncs.com/this-3.png)

我們看到，透過我們建立的物件裏頭的方法來呼叫 this，this 將會指向這個物件本身。

> 當某個函式是放在某一個物件裡面時，那麼該函式裡面的 this 指稱的就是該物件本身。

##　關於 JavaScript this 的一個 bug
延伸上面那個例子，將代碼改成下面這樣:
```js
let func_call_this_inObjectMethod = {
    name: 'I am JavaScript Object.',
    log: function() {
        this.name = 'I am Update object name.'
        console.log(this)
    }
}

func_call_this_inObjectMethod.log()
```
結果顯示如下圖:
![](https://wtfhhh.oss-cn-beijing.aliyuncs.com/this-4.png)

這邊應該沒什麼問題吧!上面說函式裡面的 this 指稱的就是該物件本身，所以當執行 func_call_this_inObjectMethod 的 log 方法時，當然更改的就是 func_call_this_inObjectMethod 這個物件本身的 name 囉!

但是，如果今天我再 log 方法裡面，再多加一個 setName 方法，用來更改 name，會發生什麼呢?讓我們先把代碼改成下面這樣:

```js
let func_call_this_inObjectMethod = {
    name: 'I am JavaScript Object.',
    log: function() {
        this.name = 'I am Update object name.'
        console.log(this)

        const setName = function(newName) {
            this.name = newName
        }
        setName('I am newest object name.')
        console.log(this)
    }
}

func_call_this_inObjectMethod.log()
```
結果顯示如下圖:
![](https://wtfhhh.oss-cn-beijing.aliyuncs.com/this-5.png)

問題來了，明明改了 `this.name`啊?為什麼 name 仍然還是 I am Update object name呢?不是應該第一次 `this.name` 是 I am Update object name.，而第二次不是應該變成 I am newest object name.嗎???

為了一探究竟，首先我們先看看調用完 log 方法之後，window 物件變成了什麼樣子。透過 `console.log(this)`，我們看看 window 現在的狀態:

![](https://wtfhhh.oss-cn-beijing.aliyuncs.com/this-6.png)

神奇的事情發生了! Window 的 name 屬性竟然變成 I am newest object name.。因此，我們合理推測，在 setName 裡呼叫 this 會指向前面提到的 Window 物件。那我們就來確認一下:

```js
let func_call_this_inObjectMethod = {
    name: 'I am JavaScript Object.',
    log: function() {
        this.name = 'I am Update object name.'
        console.log(this)

        const setName = function(newName) {
            console.log(this)
            this.name = newName
        }
        setName('I am newest object name.')
        console.log(this)
    }
}

func_call_this_inObjectMethod.log()
```
log 方法中我們 `console.log(this)` 了三次，結果顯示如下圖:
![](https://wtfhhh.oss-cn-beijing.aliyuncs.com/this-7.png)

果然，第二次的 `console.log(this)`，指向了 Window 物件，也難怪 Window 物件最後的 name 屬性變成了 I am newest object name.，而第三次 `console.log(this)` 的 this 指向了本身這個物件，但這個物件的 name 依舊是 I am Update object name.，因為 setName 中的 this 指向的根本不是物件本身，而是 Window 物件，所以壓根沒改到物件本身的 name啦!

而許多人認為，這是 JavaScript 長久以來存在的一個問題......

### 要怎麼解決這個 bug 呢?
要解決這個問題，換句話來說，就是將問題簡化為 '我們可以怎麼做來避免指稱到不同的物件呢?'。看看下面的代碼吧!

```js
let func_call_this_inObjectMethod = {
    name: 'I am JavaScript Object.',
    log: function() {
        const self = this
        self.name = 'I am Update object name.'
        console.log(self)

        const setName = function(newName) {
            console.log(self)
            self.name = newName
        }
        setName('I am newest object name.')
        console.log(self)
    }
}

func_call_this_inObjectMethod.log()
```
結果顯示如下圖:
![](https://wtfhhh.oss-cn-beijing.aliyuncs.com/this-8.png)

解決方法就是加了一行 `const self = this`，目的就是將 this 的內存位址讓 self 先給保存起來，然後下面所有要址稱到 this 的地方就都改成 self，這樣就能確保都是指向物件本身，避免指稱到不同的物件的情況啦! 結果也確實如我們所預期，第二次 `console.log(self)` 指向的是物件本身，-而第三次 `console.log(self)` 的 name 變成了 I am newest object name.。

# 結語
對於 js 中 this 一知半解的人，如果你有耐心看完這篇的話，應該蠻清楚的啦! 雖然說感覺懂了這個沒有太大用處，但是覺得還是要真正理解才行。希望對跟我一樣菜的人們有些幫助，若有錯誤，也歡迎各位大神們指正。


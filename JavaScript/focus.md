@[TOC](關於 JavaScript 焦點事件處理)

# 何謂聚焦?
所謂聚焦(`focus`)通常是在說聚焦到一個元素上，而聚焦到一個元素上通常意味著 “準備要此處接受輸入”，所以，這正是我們可以用代碼來初始化一些我們所需的功能的時刻。

失去焦點(`blur`)可能還更重要一些，這個事件可能發生在當用戶點擊頁面其他地方，或是跑去做別的事情等等。所以失去焦點通常意味著 “數據輸入都已經完成了”，所以我們就可以用代碼來檢查這些輸入，甚至是將其發到服務器上存起來。

處理這些焦點事件是前端很常見，很 basic 的功能，也有一些重要的特性，接下來就來一一做了解。

# focus/blur 事件
當元素被聚焦到時，就會觸發 `focus` 事件，而當元素失去焦點時，就會觸發 `blur` 事件。

我們用一個 `input` 輸入框來感受一下。在下面示例中:

- `focus` 事件隱藏 error 信息，因為我們要在 `blur` 事件再檢查一遍
- `blur` 事件檢查這個字段是否輸入了郵箱，如果沒有則顯示一個 error

```html
<!DOCTYPE html>
<html lang="en">
<head>
    <meta charset="UTF-8">
    <meta http-equiv="X-UA-Compatible" content="IE=edge">
    <meta name="viewport" content="width=device-width, initial-scale=1.0">
    <title>focus test</title>
</head>
<body>
    Your email : <input type="email" id="email" required>
    <div id="info"></div>
</body>
<style>
    .invalid {
        border-color: red;
    }
    .error {
        color: red;
    }
    .pass {
        color: green;
    }
</style>
<script>
    let email = document.getElementById('email')
    let info = document.getElementById('info')

    email.onfocus = function() { // 對 email DOM 添加 focus 事件
        if(this.classList.contains('invalid')) {
            this.classList.remove('invalid')
            info.innerHTML = ''
        }
    }
    email.onblur = function() { // 對 email DOM 添加 blur 事件
        if(!this.value.includes('@')) { // not email
            this.classList.add('invalid')
            if(info.classList.contains('pass'))
                info.classList.remove('pass')
            info.classList.add('error')
            this.focus()
            info.innerHTML = 'Please enter a correct email!'
        }else {
            if(info.classList.contains('error'))
                info.classList.remove('error')
            info.classList.add('pass')
            info.innerHTML = 'Email passed!'
        }
    }
</script>
</html>
```
效果如下:
![](https://wtfhhh.oss-cn-beijing.aliyuncs.com/focus-1.gif)

# focus/blur 方法
除了通過 `onfocus` 和 `onblur` 添加事件以外，我們也可以通過 `ele.focus()` 和 `ele.blur()` 方法來設置和移除元素上的焦點。

繼續上面的例子，如果輸入的郵箱無效，我們可以讓焦點無法離開 `input` 元素，直到輸入格式沒問題。
```html
<!DOCTYPE html>
<html lang="en">
<head>
    <meta charset="UTF-8">
    <meta http-equiv="X-UA-Compatible" content="IE=edge">
    <meta name="viewport" content="width=device-width, initial-scale=1.0">
    <title>focus test</title>
</head>
<body>
    Your email : <input type="email" id="email" required>
    <div id="info"></div>
</body>
<style>
    .invalid {
        border-color: red;
    }
    .error {
        color: red;
    }
    .pass {
        color: green;
    }
</style>
<script>
    let email = document.getElementById('email')
    let info = document.getElementById('info')

    email.onblur = function() {
        if(!this.value.includes('@')) { // not email
            this.classList.add('invalid')
            if(info.classList.contains('pass'))
                info.classList.remove('pass')
            info.classList.add('error')
            info.innerHTML = 'Please enter a correct email!'
            this.focus()
        }else {
            if(info.classList.contains('error'))
                info.classList.remove('error')
            info.classList.add('pass')
            this.classList.remove('invalid')
            info.innerHTML = 'Email passed!'
        }
    }
</script>
</html>
```
效果如下:
![](https://wtfhhh.oss-cn-beijing.aliyuncs.com/focus-2.gif)

注意一下細微的區別，可以看到，當我們所輸入的郵箱格式不包含 `@` 時，除了同樣會提示用戶一些錯誤信息和效果以外，注意到焦點還是再輸入框上(浮標還在跳)，也就表示還在等待用戶輸入，直到正確。這就是因為我們用 `this.focus()` 讓焦點重新被拉回來，而不是直接 `blur` 掉。

這邊要注意的一點就是，焦點是有可能會丟失的!

其中之一有可能用戶就亂點了其他位置，或是 js 自身的其他事件也可能會導致焦點丟失，譬如:

1. `alert` 會把焦點移到自身，從而觸發其他元素的 `blur` 事件，而當 `alert` 對話框被取消後，焦點又會回到本來得元素上。
2. 如果一個 DOM 元素被刪除了，那麼焦點也會消失。即使之後又把 DOM 元素加回去，焦點仍然不會回到他身上。

所以就是，當我們要使用 `focus` 和 `blur` 事件時要更加小心，確認好焦點到底在誰身上，且要注意焦點轉移的邏輯是否正確。

# 允許在任何元素上聚焦 : tabindex
默認情況下，其實很多元素都是不支持聚焦的。

通常，`focus` 和 `blur` 事件都支持那些與用戶交互相關的元素，像是 `button`, `a`, `select`, `input` 等等。

另一方面，像是 `div`, `span` 等等其實默認是不能被聚焦的，也就表示 `focus`, `blur` 事件並不適用於他們，當然 `ele.focus()`, `ele.blur()` 也不用說。

不過，使用一個 Html 屬性 `tabindex` 可以解決這種情況。

任何被設置了 `tabindex` 的元素都會變得可聚焦，而 `tabindex` 的值是當切換元素時聚焦的順序號，這邊注意下，這個值要從 1 開始，等於 0 的話相當於沒有設置 `tabindex`，而如果小於 0 則不會被受理。

```html
<!DOCTYPE html>
<html lang="en">
<head>
    <meta charset="UTF-8">
    <meta http-equiv="X-UA-Compatible" content="IE=edge">
    <meta name="viewport" content="width=device-width, initial-scale=1.0">
    <title>focus test</title>
</head>
<body>
    <ul>
        <li tabindex="1">one</li>
        <li tabindex="3">three</li>
        <li tabindex="2">two</li>
        <li tabindex="0">zero</li>
        <li tabindex="-1">minus one</li>
    </ul>
</body>
<style>
    li {
        cursor: pointer;
    }
    :focus {
        outline: 2px dashed green;
    }
</style>
<script>
    const lists = document.querySelectorAll('li')
    for(let i=0; i<lists.length; i++) {
        lists[i].onfocus = function() {
            console.log(lists[i].innerHTML)
        }
    }
</script>
</html>
```
效果如下:
![](https://wtfhhh.oss-cn-beijing.aliyuncs.com/focus-4.gif)

可以看到，後面都是通過按 `Tab` 來切換焦點，順序就是 1-3-2-0。且設置了 `tabindex` 後也支持 `onfocus`。


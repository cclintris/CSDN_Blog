@[TOC](關於 React 組件生命週期)

# 前言
最近因為需要，主要是為暑假的實習做準備，所以開始學習 React，其實還只是很菜很菜的，所以覺得應該要好好扎實的學，不要再像之前一樣，又因為基礎不扎實導致後續很多開發上的問題。這篇要來介紹 React 組建的生命週期，之前如果有用過 Vue 的話就會知道 Vue 也是有組建的生命週期的，之前我也寫過一篇，可以參考 [關於 Vue 的生命週期與鉤子函數](https://blog.csdn.net/weixin_46803507/article/details/115107627)。話不多說，進入正題吧!

# 參考
|參考|鏈接|
|:-----|:-----|
|理解React组件生命周期|https://segmentfault.com/a/1190000013309058|
|||
|React函数组件和类组件的区别|https://blog.csdn.net/wu_xianqiang/article/details/91320529|

# 正文
React 的生命週期大致上可以分為三個階段：

- 初始化
- 更新
- 卸載

直接來看看 React 完整的生命週期圖：
![](https://ftp.bmp.ovh/imgs/2021/05/0735dff9c6db6ddc.webp)

看不懂也沒關係，接下來會做更詳細的解釋。

## 組件初始化(initial)
![](https://ftp.bmp.ovh/imgs/2021/05/bbcb4ea69877c434.png)

### constructor()
React 組件的構造函數會在渲染前被調用。當我們為一個 `React.Component` 子類定義構造函數時，好習慣是在任何其他表達是之前先調用 `super(props)`，否則 `this.props` 在組件內將會是 undefined。

構造函數是初始化狀態的合適位置，如果不初始化狀態或是不綁定方法，那其實也就不需要構造函數了。所以通常好習慣就是在創建一個組件時記得都要加上 `constructor`，並且記得要 `super(props)`，這樣才能調用父類的構造函數，然後如果有要定義、初始化的狀態也是在這邊定義。

看看下面例子：
```js
class Counter extends React.Component {
    constructor(props) {
        super(props)
        this.state = {
            count: 0
        }
    }
}
```
### componentWillMount()
```js
componentWillMount()
```
`componentWillMount()` 其實在實際項目中用得很少，其實所做的事情也可以由 `constructor()` 完成，所以通常都推薦用 `constructor()` 就好，因此這邊就不對這個鉤子做太多深入。

### render()
```js
render() {
    return (
        /* jsx */
    )
}
```
`render()` 是必須的。`render()` 方法要求要返回一個 jsx 元素，<strong>只能ㄧ個！</strong>必須要用一個外層的 jsx 包裹其他 jsx 元素，返回多個並列的 jsx 元素是不合法的。

`render()` 應該保持純淨，什麼意思呢？其實就是說 `render()` 不應該改變組件的狀態，每次調用都應該返回相同的結果，沒有副作用。同時也不應該直接與瀏覽器交互，應該要把交互的任務放在其他生命週期鉤子中。簡單來說可以把 `render()` 想成就只負責 return 一個渲染模板，其他什麼都盡量不要做。

> 注意，若 `shouldComponentUpdate()` 返回 false，則 `render()` 不會被調用。下面會再講到 `shouldComponentUpdate()`。
 
### componentDidMount()
```js
componentDidMount()
```
`componentDidMount()` 會在渲染之後被立即調用。這個鉤子也可以說是 React 組件初始化的最後一個階段，如果需要從後端請求一些數據供組件渲染使用，使用 `componentDidMount()` 很適合。在這個鉤子中更改狀態都會引發視圖的重繪，用過 Vue 的話，可以把這個鉤子想成 Vue 中的 `mounted()` 鉤子。

## 更新狀態(Updating State)
![](https://ftp.bmp.ovh/imgs/2021/05/fe4feb08ea2d42aa.png)

### shouldComponentUpdate()
```js
shouldComponentUpdate(newProps, newState)
```
使用 `shouldComponentUpdate()` 可以做到讓 React 知道當前組件的狀態(state)或是屬性(props)改變是否<strong>不</strong>影響組件的輸出。默認行為是會在每次狀態改變進行重新渲染，而其實大部分情況下我們依賴默認行為就好。

當接收到新狀態或是新屬性時，`shouldComponentUpdate()` 會在渲染前被調用，默認返回 true，而返回 false 則可以阻止組件以及子組件因為狀態或是屬性的更新的重新渲染。

如果只定義了方法，但什麼都不返回，會報一個 warning:
```
shouldComponentUpdate(): Returned undefined instead of a boolean value.
```
> 注意，該方法不會在初始化渲染或是使用 `forceUpdate()` 時被調用，下面也會介紹到 `forceUpdate()`

### componentWillUpdate()
```js
componentWillUpdate(newProps, newState)
```
當接收到新狀態或是新屬性時，`componentWillUpdate()` 會在渲染前被立即調用，也就是說，在更新發生之前，這個鉤子提供一個機會讓你對數據做一些操作，同樣，該方法不會在初始化渲染被調用。

> 注意，如果 `shouldComponentUpdate()` 返回 false 則該方法不會被調用

### componentDidUpdate()
```js
componentDidUpdate(newProps, newState)
```
當接收到新狀態或是新屬性時，`componentDidUpdate()` 會在渲染後被立即調用。同樣，該方法不會在初始化渲染被調用。


到這邊，應該大致上有一點概念了，來看看一個實際的代碼例子，會更清楚些。
```html
<!DOCTYPE html>
<html lang="en">
  <head>
    <meta charset="UTF-8" />
    <meta http-equiv="X-UA-Compatible" content="IE=edge" />
    <meta name="viewport" content="width=device-width, initial-scale=1.0" />
    <title>React lifecycle demo</title>
     
    <script src="https://unpkg.com/react@16/umd/react.development.js"></script>
    <script src="https://unpkg.com/react-dom@16/umd/react-dom.development.js"></script>
    <script src="https://unpkg.com/babel-standalone@6.15.0/babel.min.js"></script>
  </head>
  <body>
    <div id="app"></div>
    <script type="text/babel">
      // 定义一个子组件Person
      class Person extends React.Component {
        constructor(props) {
          super(props);
          this.state = { no: props.no + 100 };
        }

        componentWillMount() {
          console.log("trigger componentWillMount");
        }

        componentDidMount() {
          console.log("trigger componentDidMount");
        }

        componentWillUpdate(newProps, newState) {
          console.log(
            "trigger componentWillUpdate newProps=",
            newProps,
            " newState=",
            newState
          );
        }

        componentDidUpdate(newProps, newState) {
          console.log("trigger componentDidUpdate");
        }

        render() {
          return (
            <div>
              <button onClick={() => this.setState({ no: this.state.no + 1 })}>
                子组件按钮(用于修改state)
              </button>
              <p>props.no:{this.props.no}</p>
              <p>state.no:{this.state.no}</p>
            </div>
          );
        }
      }

      // 定义一个父组件App,它会引用子组件，并且修改子组件的props
      class App extends React.Component {
        constructor(props) {
          super(props);
          this.state = { no: 0 };
        }
        render() {
          return (
            <div>
              <button onClick={() => this.setState({ no: this.state.no + 1 })}>
                父组件按钮(用于修改props)
              </button>
              <Person no={this.state.no} />
            </div>
          );
        }
      }
      ReactDOM.render(<App />, document.getElementById("app"));
    </script>
  </body>
</html>
```
視圖渲染如下：
![](https://ftp.bmp.ovh/imgs/2021/05/a8907c3389060d12.png)

初始化的控制台如下：
![](https://ftp.bmp.ovh/imgs/2021/05/9fffbb15fe676b9f.png)
(warning 就先不管了，也無所謂hhh)

點擊事件後控制台打印如下：
![](https://ftp.bmp.ovh/imgs/2021/05/c8b39dfc1c8c3f5b.gif)

可以看到，每次無論是 props 或是 state 發生了改變，對於 `Person` 組建來說，都會觸發 `componentWillUpdate` 以及 `componentDidUpdate`，應該效果展示都還算清晰。

## 更新屬性(Updating Props)
![](https://ftp.bmp.ovh/imgs/2021/05/bf5f3d69dc02cb26.png)

### componentWillReceiveProps
```js
componentWillReceiveProps(newProps)
```
`componentWillReceiveProps()` 在 props 發生變化的時候會被調用，且同樣該方法不會在初始化渲染被調用。

在這個鉤子裡面，可以根據 props 的變化，使用 `this.setState({...})` 來更新組件的狀態。舊的 props 仍然可以通過 `this.props` 獲得，而且這邊對狀態的更新是相對安全的，並不會觸發 `render()` 進行重繪。

## 卸載(Unmounting)
![](https://ftp.bmp.ovh/imgs/2021/05/9ec13586456df4ae.png)

### componentWillUnmount()
```js
componentWillUnmount()
```
`componentWillUnmount()` 會在組件被卸載前被立刻調用，可以在這個鉤子中進行一些清理工作，例如定時器銷毀、取消網絡連接請求等等。

## forceUpdate
![](https://ftp.bmp.ovh/imgs/2021/05/67036c90a1383b59.png)

通常情況下，當組件所依賴的數據發生改變，React 就會幫我們重繪該組件，但如果什麼原因或是特殊的需求希望能夠強制進行重新渲染組件，就可以使用 `forceUpdate()`。

調用 `forceUpdate()` 可以強制觸發組件的 `render()` 方法，並且忽略 `shouldComponentUpdate()`。看看下面代碼，十分清楚：
```js
<!DOCTYPE html>
<html lang="en">
  <head>
    <meta charset="UTF-8" />
    <meta http-equiv="X-UA-Compatible" content="IE=edge" />
    <meta name="viewport" content="width=device-width, initial-scale=1.0" />
    <title>Document</title>

    <script src="https://unpkg.com/react@16/umd/react.development.js"></script>
    <script src="https://unpkg.com/react-dom@16/umd/react-dom.development.js"></script>
    <script src="https://unpkg.com/babel-standalone@6.15.0/babel.min.js"></script>
  </head>
  <body>
    <div id="app"></div>
    <script type="text/babel">
      class App extends React.Component {
        constructor(props) {
          super(props);
          this.forceUpdateHandler = this.forceUpdateHandler.bind(this);
        }

        componentWillUpdate() {
          console.info("componentWillUpdate called");
        }

        componentDidUpdate() {
          console.info("componentDidUpdate called");
        }

        forceUpdateHandler = () => {
          this.forceUpdate();
        };

        render() {
          return (
            <div>
              <button onClick={this.forceUpdateHandler}>FORCE UPDATE</button>
              <h4>Random number : {Math.random()}</h4>
            </div>
          );
        }
      }
      ReactDOM.render(<App />, document.getElementById("app"));
    </script>
  </body>
</html>
``` 
直接貼到 html 文件運行看看吧，相信一目瞭然的，這邊就不展示效果了。

## 函數組件 vs 類組件
看完這麼多關於組件的生命週期，最後來做一個重要的區別，也就是函數組件和類組件。有一點點基礎的應該就知道，React 中有兩種創建組建的方式。

- 函數組件
```js
function App() {
    return (
        <div>Function component</div>
    )
}
```

- 類組件
```js
class App extends React.Component {
    render() {
        return (
            <div>class component</div>
        )
    }
}
```
函數組件和類組件當然是有區別的，來看看下表的整理：
|區別|函數組件|類組件|
|:-----|:-----|:-----|
|性能|高|低(因為要實例化)|
|是否有 `this`|否|是|
|是否有生命週期|否|是|
|是否有 `state`|否|是|

所以可以看到，當我們在寫函數組件時，其實是直接 return，因為根本沒有生命週期也就不存在 `render()` 鉤子，也因此推出了 Hooks 來做到組件階段的管理。而也因為函數組件沒有 state，所以就有了 Redux 來進行狀態管理。

之前和已經在工作的學長聊過，貌似好像現在都越來越推薦使用函數組件，所以像 Redux, Hooks 也都是必須要學習的。

關於 Redux 和 Hooks 也都是 React 很重要的東西，之後會在寫寫關於這兩個知識點的博客。

# 結語
這篇算是覆蓋了關於 React 組件的生命週期的概念以及用法，具體還要之後在項目中好好實踐一下才能更好的體會。雖然好像越來越提倡函數組件，但我覺得這還是很基礎的東西，感覺想學好 React 還是要掌握的，所幸就寫了這篇。本人其實也還只是 React 小白，若有錯誤歡迎大老指出。


@[TOC](深入理解 React 的 setState)

# 前言

這篇來探討一個 react 中很基本的東西，`setState`，探討這個一方面其實也是打好 react 的基礎，另一方面是因為我自己拿來練手的 side-project 確實用到 `setState`，但是卻遇到了一個 bug，我思索應該就是 `setState` 的問題，沒有理解透徹。因此趕緊來學習這個東西，好掌握這個知識點。

# 參考

| 參考                                 | 鏈接                                                                                                       |
| :----------------------------------- | :--------------------------------------------------------------------------------------------------------- |
| React 官方文檔                       | https://reactjs.org/docs/react-component.html#setstate                                                     |
| StackOverflow Dan Abramov 大神的解答 | https://stackoverflow.com/questions/48563650/does-react-keep-the-order-for-state-updates/48610973#48610973 |

# 正文

## setState 到底是同步還是異步的？

對於已經熟悉 react 的人來說，應該已經都知道了，來看看一段原文，官方文檔對 `setState` 的說明如下：

> `setState()` enqueues changes to the component state and tells React that this component and its children need to be re-rendered with the updated state. This is the primary method you use to update the user interface in response to event handlers and server responses.

> Think of `setState()` as a request rather than an immediate command to update the component. For better perceived performance, React may delay it, and then update several components in a single pass. React does not guarantee that the state changes are applied immediately.

> `setState()` does not always immediately update the component. It may batch or defer the update until later. This makes reading `this.state` right after calling `setState()` a potential pitfall. Instead, use `componentDidUpdate` or a setState callback `(setState(updater, callback))`, either of which are guaranteed to fire after the update has been applied. If you need to set the state based on the previous state, read about the updater argument below.

如果英文不好的話也沒關係，上面這麼一大段話就是在說，`setState` 會依序更新 state，然後告訴 react 要重新渲染這個組件以及他的子組件，這個函數可以運用在事件觸發或是或是對服務端請求所帶來的數據變化。

不過重點其實在第二段話，、不過重點其實在第二段話，第二段話告訴我們要把 `setState` 想成一個『非同步』的操作，也就是說，執行了 `setState` 其實不代表 component 的 state 一定被立即更新了，這其實是 react 為了提高性能的一個設計，後面也會再提到。

也因此，所以如果在 `setState` 之後馬上去讀取 `this.state`，可能會發現 `state` 沒有更新。這時候該怎麼辦呢？其實上面的原文也有提到，我們可以用 `componentDidUpdate()` 鉤子，如果不清楚歡迎參考 [關於 React 組件生命週期](https://blog.csdn.net/weixin_46803507/article/details/116433318)，或是如果想要精準的基於先前的 state 進行後續操作的話，推薦使用 `updater` 函數。

`updater` 函數長這樣：

```js
this.setState((state, props) => {
  return { ...一個新的對象 };
});
```

> `state` is a reference to the component state at the time the change is being applied. It should not be directly mutated. Instead, changes should be represented by building a new object based on the input from `state` and `props`.

這麼一直看理論啥的其實很抽象，馬上帶入一個例子，假設我們今天有一個組件，如下：

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
      class Count extends React.Component {
        constructor(props) {
          super(props);
          this.state = {
            count: 0,
          };
          this.handleAddCount = this.handleAddCount.bind(this);
        }

        handleAddCount = () => {
          this.setState((state) => ({
            count: state.count + 1,
          }));
        // 寫成下面這樣當然也可以
        //   this.setState((state) => {
        //     return {
        //       count: state.count + 1,
        //     };
        //   });
        };

        render() {
          return (
            <div>
              <h2>{this.state.count}</h2>
              <button onClick={this.handleAddCount}>Add</button>
            </div>
          );
        }
      }
      ReactDOM.render(<Count />, document.getElementById("app"));
    </script>
  </body>
</html>
```

![](https://riyugo.com/o/210506/m5yr16.gif)

上面的 `handleAddCount` 使用了 `updater`，所以一切更新看起來都挺正常的。那如果我就執意不用 `updater` 呢？

```js
handleAddCount = () => {
  this.setState({
    count: this.state.count + 1,
  });
};
```

![](https://riyugo.com/o/210506/m5yr16.gif)

結果其實會發現，還是可以正常 add 啊？！那好像用不用 `updater` 沒差？上面說到的 `setState` 的同步非同步又是怎樣？別急，有耐心點繼續看下去。

如果我今天是一次想要加三上去，也我就執意不用 `updater`，來看看會發生什麼：

```js
handleAddCount = () => {
  this.setState({
    count: this.state.count + 1,
  });
  this.setState({
    count: this.state.count + 1,
  });
  this.setState({
    count: this.state.count + 1,
  });
};
```

![](https://riyugo.com/o/210506/m5yr16.gif)

好的問題出現了！我們明明就三次執行了 `setState`，但卻不是一次加 3 上去，仍然還是每次只加 1。這就是前面提到的，每次 `setState` 不一定會馬上更新，因此執行完 `setState` 後馬上讀取 `this.state` 不見得會如我們預期。

所以我們就改成用 `updater`，如下：

```js
handleAddCount = () => {
  this.setState((state) => ({
    count: state.count + 1,
  }));
  this.setState((state) => ({
    count: state.count + 1,
  }));
  this.setState((state) => ({
    count: state.count + 1,
  }));
};
```

![](https://riyugo.com/o/210506/mamhol.gif)
發現確實如我們所預期一次加 3 上去～當時學到這邊得出一個結論，反正以後基於先前的 state 操作，用 `updater` 就對了，而且大部分情況我們都是希望 state 要按我們的 `setState` 操作更新，所以應該都用 `updater` 就對了。

如果你也有一樣的想法，只能說，很遺憾你跟我一樣菜，沒關係我們繼續學。其實不是這樣那麼簡單的，其實並不是都會像上面一樣，多次執行 `setState` 後『一定不會』馬上更新。(有點拗口 hhh)

來看看下面這個例子：

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
      class Clock extends React.Component {
        constructor(props) {
          super(props);
          this.state = {
            date: new Date(),
            count: 0,
          };
          this.tick = this.tick.bind(this);
        }

        componentDidMount() {
          setInterval(this.tick, 1000);
        }

        tick = () => {
          this.setState({
            date: new Date(),
          });
          for (let i = 0; i < 3; i++) {
            this.setState((state) => {
              return {
                count: state.count + 1,
              };
            });
          }
        };

        render() {
          return (
            <div>
              <h1>Learn React</h1>
              <h2>{this.state.date.toLocaleDateString()}</h2>
              <p>{this.state.count}</p>
            </div>
          );
        }
      }
      ReactDOM.render(<Clock />, document.getElementById("app"));
    </script>
  </body>
</html>
```

![](https://riyugo.com/o/210506/nb377n.gif)

在這個例子中，一個簡單時間顯示的 clock 組件，同時 state 中還有一個 count，希望每秒的 count 都基於前面的 count 加上去。

`tick` 函數中，我三次調用了 `setState`，這邊我使用了 `updater`，但其實你自己改成不用 `updater` 試試看就會發現，也是一樣的效果～

這個例子告訴我們，很明顯先前的想法反正都用 `updater` 不一定正確，因為這邊不管用不用都一樣。所以這邊進入說明：

> 什麼時候 `setState` 才會變成非同步的？

其實是這樣的，在 react 中，調用 `setState` 確實會立刻更新 state 並重新 render，但是如果處理更新的 handler 綁定在『事件處理』上，像是 onClick、onChange、onKeyDown 等等，那這些 `setState` 的執行只會觸發最後一次 render，而不是每次 `setState` 後都會 re-render。這就是前面提到的 react 為了性能上的設計，設想如果每次事件發生，比如按鈕被點擊(onClick)，觸發 eventHandler，組件以及他的子組件都執行 `setState`，每次都進行重繪，顯然當項目越來越大、組件越來越多等等，就會嚴重消耗系統性能。

我們把上面的代碼重構一下，分成父組件以及子組件的結構，來看看下面代碼：

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
      class Clock extends React.Component {
        constructor(props) {
          super(props);
          this.state = {
            date: new Date(),
          };
        }

        componentDidMount() {
          setInterval(() => this.tick(), 1000);
        }

        tick() {
          this.setState({
            date: new Date(),
          });
        }

        render() {
          console.log("Parent render");
          return (
            <div>
              <ShowClock date={this.state.date} />
            </div>
          );
        }
      }

      class ShowClock extends React.Component {
        constructor(props) {
          super(props);
          this.state = {
            count: 0,
          };
        }

        componentDidMount() {
          setInterval(() => this.addCountPerSec(), 1000);
        }

        addCountPerSec() {
          this.setState({
            count: this.state.count + 1,
          });
        }

        render() {
          console.log("Child render");
          console.log("-------------");
          return (
            <div>
              <h1>Learn React</h1>
              <h2>{this.props.date.toLocaleTimeString()}</h2>
              <p>count : {this.state.count}</p>
            </div>
          );
        }
      }
      ReactDOM.render(<Clock />, document.getElementById("app"));
    </script>
  </body>
</html>
```

![](https://riyugo.com/o/210506/qxv7yg.gif)

可以看到控制台輸出，我們在 Clock component 以及 ShowClock component 的 render 中都方一個 console.log()，而我們發現，每過一秒，ShowClock 都 re-render 了兩次，因為第一次是 Clock 通過了 `setState` 更改了傳給 ShowClock 的 props，第二次則是 ShowClock 自己通過了 `setState` 更改了自己的 state。

這就是為什麼 react 要幫我們把觸發的 eventHandler 的 `setState` 包裝成非同步的，因為就是要避免這種大量重繪的問題。

來看看這個例子：

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
      class Father extends React.Component {
        constructor(props) {
          super(props);
          this.state = {
            a: false,
          };
        }

        render() {
          console.log("Father render");
          return (
            <Son
              parentState={this.state.a}
              setParentState={this.setState.bind(this)}
            />
          );
        }
      }

      class Son extends React.Component {
        constructor(props) {
          super(props);
          this.state = {
            b: false,
          };
        }

        handleClick = () => {
          const parentState = this.props.parentState;
          this.props.setParentState({
            a: parentState === true ? false : true,
          });
          this.setState({
            b: this.state.b === true ? false : true,
          });
        };

        render() {
          console.log("Son render");
          return (
            <div>
              <p>FatherState : {`${this.props.parentState}`}</p>
              <p>SonState : {`${this.state.b}`}</p>
              <button onClick={this.handleClick}>click</button>
            </div>
          );
        }
      }
      ReactDOM.render(<Father />, document.getElementById("app"));
    </script>
  </body>
</html>
```

控制台輸出如下：
![](https://riyugo.com/o/210506/re5e6t.gif)

邏輯也很單純，就是一個父組件、一個子組件，同樣也是由父組件通過 props 將參數(自身狀態)傳給子組件，不同的是，這邊我將兩個 `setState` 操作綁定在 onClick 事件上，所以可以看到，在 Son component 中，雖然也是做了兩次 `setState`，但是實際上觸發了一次 render(只打印出一次 Son render)，這就是 react 幫我們做的異步化，讓整個事件中只觸發一次 render。

關於這個概念，我們來看看大神 Dan Abramov 的總結：

> The key to understanding this is that <strong>no matter how many setState() calls in how many components you do inside a React event handler, they will produce only a single re-render at the end of the event.</strong> This is crucial for good performance in large applications because if Child and Parent each call `setState()` when handling a click event, you don't want to re-render the Child twice.

> `setState()` calls happen inside a React event handler. Therefore they are always flushed together at the end of the event (and you don't see the intermediate state).

不愧是大牛，說得真好～ ok 現在我們知道在事件處理的函數中，`setState` 的執行是異步的，那我們回到最一開始的例子，為什麼明明三次執行了 `setState`，但還是以 1 為單位加上去？

```js
// 沒有使用 updater

handleAddCount = () => {
  this.setState({
    count: this.state.count + 1,
  });
  this.setState({
    count: this.state.count + 1,
  });
  this.setState({
    count: this.state.count + 1,
  });
};
```

原因就是因為 react 其實把他們處理成這樣：

```js
Object.assign(
  previousState,
  { count: this.state.count + 1 },
  { count: this.state.count + 1 },
  { count: this.state.count + 1 }
);
```

我們都知道，`setState` 的參數必須是一個對象，扣回到剛剛講的，react 對事件處理函數中的 `setState` 所引發的 render 是異步的，也就是說 react 會把三次 `setState` 中的對象合併在一起後，才會實際執行更新 state, re-render 的動作。

我們都知道，如果我們這樣寫：

```js
const person = {
  name: "cyzsb",
  name: "csdnb",
  name: "cclin",
};
```

那其實 person 對象如下：

```js
{
  name: "cclin";
}
```

因為同一個對象不能有重名的屬性，若有，會以後面的覆蓋前面的(根本就在講廢話 hhh)。

看到這裡，其實應該對 `setState` 更加明白了，就是因為這個原因，所以其實你以為自己三次執行了 `setState`，但經過 react 背後的處理，實際上最後只剩下一次的效果。

這才是 `updater` 的用處。所以其實 `updater` 是用於如果在一個事件觸發的處理函數中要多次更新 state，且希望每次 `setState` 都基於先前的 state 操作，才要用 `updater`，如果 `setState` 不是通過事件綁定觸發，其實用不用 `updater` 就根本無所謂。

## setState 原理

看到這邊應該都懂了，應該也沒多少人看到這邊，畢竟這篇確實有點長。看到這邊的話，其實可以停了也知道怎麼使用 `setState` 了，不過還是建議再多堅持一下下，看看具體 react 中的 `setState` 的原理。

其實一句話簡單就可以概括 `setState` 的原理：

> 在 React 生命周期内，也可以理解主线程中 `setState` 就是异步的；子线程或者说异步任务中，例如 setInterval,setTimeout 里，`setState` 才是同步更新的。

![](https://riyugo.com/o/210506/soj6o7.png)

看一個例子，非常清楚：

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
          this.state = {
            num: 0,
          };
        }

        componentDidMount() {
          this.setState((state) => ({
            num: state.num + 1,
          }));
          console.log("第一次：", this.state.num);
          this.setState((state) => ({
            num: state.num + 1,
          }));
          console.log("第二次：", this.state.num);
          setTimeout(() => {
            this.setState((state) => ({
              num: state.num + 1,
            }));
            console.log("第三次：", this.state.num);
            this.setState((state) => ({
              num: state.num + 1,
            }));
            console.log("第四次：", this.state.num);
          }, 0);
        }

        componentDidUpdate() {
          console.log("componentDidUpdate");
        }

        render() {
          return <div>app</div>;
        }
      }
      ReactDOM.render(<App />, document.getElementById("app"));
    </script>
  </body>
</html>
```

![](https://riyugo.com/o/210506/szdv9m.png)

当第一次调用 `setState`，newState 被放入 pending 队列，此时 batch update(源码中 isBatchingUpdates) 是 true ,所以 newState 会被保存在 dirtyComponents 数组中，并不会调用 updateComponent,所以 state 并没有更新，但是 state 的值已经被合并了，第二次调用 `setState` 的情况也是一样的。

第三次调用 `setState` 时，newState 被放入 pending 队列，此时 batch update (源码中 isBatchingUpdates) 是 false，此时会马上遍历 dirtycomponents 数组，调用 updateComponent 方法，然后更新 state，第四次同第三次。

注意到這邊我們用了 `updater`，既然上面講到了那麼多，不仿再來看看如果不用 `updater` 會發生什麼？

```js
componentDidMount() {
    //   this.setState((state) => ({
    //     num: state.num + 1,
    //   }));
    //   console.log("第一次：", this.state.num);
    //   this.setState((state) => ({
    //     num: state.num + 1,
    //   }));
    //   console.log("第二次：", this.state.num);
    //   setTimeout(() => {
    //     this.setState((state) => ({
    //       num: state.num + 1,
    //     }));
    //     console.log("第三次：", this.state.num);
    //     this.setState((state) => ({
    //       num: state.num + 1,
    //     }));
    //     console.log("第四次：", this.state.num);
    //   }, 0);
    this.setState({
        num: this.state.num + 1,
    });
    console.log("第一次：", this.state.num);
    this.setState({
        num: this.state.num + 1,
    });
    console.log("第二次：", this.state.num);
    setTimeout(() => {
    this.setState({
        num: this.state.num + 1,
    });
    console.log("第三次：", this.state.num);
    this.setState({
        num: this.state.num + 1,
    });
    console.log("第四次：", this.state.num);
    }, 0);
}
```
![](https://riyugo.com/o/210506/tyjgaq.png)

可以看到跟使用了 `updater` 有一點點的不一樣。所以其實就可以知道，react 在背後幫我們做的 `setState` 的異步，其實就是合併的動作，而 `updater` 做到的就是精準的合併每一次 `setState` 後的操作，而不用 `updater` 的話，則會被 react 處理成上面 `Object.assign()` 的樣子。

也正是因為這樣，所以在這個例子中，如果不使用 `updater`，會發現第三次打印出來的 num 是 2，而使用 `updater` 打印出來的會是 3。

# 結語
這篇詳細了講解了關於 React 的 setState 方法，如果能看完我只能說，你真有耐心。整篇下來應該算是非常詳細的了，其實也只是本菜雞學習 react 的剛開始而已，希望以後也還能繼續堅持下去。對於本文如果有不完整或是錯誤，歡迎大老指出，也感謝那些認真讀完的人，應該會有所收穫的！


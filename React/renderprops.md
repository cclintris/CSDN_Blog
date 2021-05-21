@[TOC](React 中使用 render props)

# 前言
這篇也是紀錄了關於學習 react 的過程，起因是因為開始學習 hooks，但是發現好像有些坑比較重要但卻被我跳過了，像是 render props, HOC 等等。這篇就是來補一下 render props 這部分，本篇也相對簡單，所以不會太長就是紀錄一下而已，不過還是算是比較重要的。

# 正文
官方文檔是這樣給 render props 解釋的：
> 术语 “render prop” 是指一种在 React 组件之间使用一个值为函数的 prop 共享代码的简单技术

> 具有 render prop 的组件接受一个函数，该函数返回一个 React 元素并调用它而不是实现自己的渲染逻辑。

反正我自己理解能力差所以總之有點不是非常透徹，所以這篇就是講講 render props 以及我自己對這個東西個人白話的理解。

下面開始用一個例子來講。以下組件負責跟蹤 Web 應用中鼠標的位置：
```js
class MouseTracker extends React.Component {
  constructor(props) {
    super(props);
    this.handleMouseMove = this.handleMouseMove.bind(this);
    this.state = { x: 0, y: 0 };
  }

  handleMouseMove(event) {
    this.setState({
      x: event.clientX,
      y: event.clientY
    });
  }

  render() {
    return (
      <div style={{ height: '100vh' }} onMouseMove={this.handleMouseMove}>
        <p>Current mouse position ({this.state.x}, {this.state.y})</p>
      </div>
    );
  }
}
```
都很簡單，應該都看得懂，反正就是只要你鼠標動到哪就會顯示在 `<p>` 中。

那如果今天有一個新的需求：我希望有一個圖片，跟著我的鼠標動到哪圖片就也跟到哪，那該怎麼做呢？

其實也很簡單，首先應該會想把跟蹤鼠標這個行為單獨抽出來成一個組件：
```js
class Mouse extends React.Component {
  constructor(props) {
    super(props);
    this.handleMouseMove = this.handleMouseMove.bind(this);
    this.state = { x: 0, y: 0 };
  }

  handleMouseMove(event) {
    this.setState({
      x: event.clientX,
      y: event.clientY
    });
  }

  render() {
    return (
      <div style={{ height: '100vh' }} onMouseMove={this.handleMouseMove}>
        <p>TCurrent mouse position  ({this.state.x}, {this.state.y})</p>
      </div>
    );
  }
}

class MouseTracker extends React.Component {
  render() {
    return (
        <Mouse />
    );
  }
}
```
好的，現在 Mouse 組件擁有了所有跟蹤鼠標該有的行為，也存有 x,y 座標，而透過 MouseTracker 渲染出來。那假設今天圖片希望是麥克斯(不知道的話沒差，或是去玩荒野亂鬥hh)，那這麼寫就好啦：
```js
class BSMax extends React.Component {
  render() {
    const mouse = this.props.mouse;
    return (
      <img src="/Max.jpg" style={{ position: 'absolute', left: mouse.x, top: mouse.y }} />
    );
  }
}

class MouseWithMax extends React.Component {
  constructor(props) {
    super(props);
    this.handleMouseMove = this.handleMouseMove.bind(this);
    this.state = { x: 0, y: 0 };
  }

  handleMouseMove(event) {
    this.setState({
      x: event.clientX,
      y: event.clientY
    });
  }

  render() {
    return (
      <div style={{ height: '100vh' }} onMouseMove={this.handleMouseMove}>
        <BSMax mouse={this.state} />
      </div>
    );
  }
}

class MouseTracker extends React.Component {
  render() {
    return (
      <div>
        <h1>移动鼠标!</h1>
        <MouseWithMax />
      </div>
    );
  }
}
```
大概效果像這樣：
![](https://riyugo.com/o/210508/ph6xo4.gif)

確實完成效果了。但是其實這根本就是假的封裝，因為這只適用於特定的圖片。也就是說，這個封裝出來的跟蹤鼠標功能的組件和圖片 Max 強耦合在一起，這是非常糟糕的。如果今天我想要換個圖片，譬如說 bibi(也是荒野角色hh)，那是不是就要像上面一樣，再寫一堆新的組件，有夠麻煩。

歸根究底就是因為我們把渲染圖片的工作硬編碼在跟蹤鼠標功能的組件中。這時候 render props 就派上用場了。

其實實際上我們希望的是，將渲染的動作也獨立開來，不要跟跟蹤鼠標組件的狀態等等混再一起。

render props 就是提供一個 prop，但這個 prop 是個函數，雃這個函數來動態的決定渲染什麼圖片，這樣我們就可以不用把 `<Cat />` 硬編碼到 `Mouse` 中了。

下面就附上完整的代碼：
```js
import React from "react";
import "../style/MouseTracker.css";
import Mouse from "./Mouse";
import BS from "./BS";

export default class MouseTracker extends React.Component {
  render() {
    return (
      <div style={{ height: "100vh" }}>
        {/* render props 就這麼用 */}
        <Mouse render={(mouse) => <BS mouse={mouse} />} />
      </div>
    );
  }
}
```

```js
import React from "react";

export default class Mouse extends React.Component {
  constructor(props) {
    super(props);
    this.handleMouseMove = this.handleMouseMove.bind(this);
    this.state = {
      x: 0,
      y: 0,
    };
  }

  handleMouseMove(event) {
    this.setState({
      x: event.clientX,
      y: event.clientY,
    });
  }

  render() {
    return (
      <div style={{ height: "100vh" }} onMouseMove={this.handleMouseMove}>
        {this.props.render(this.state)}
      </div>
    );
  }
}
```

```js
import React from "react";
import BSMax from "../../assets/BSMax.png";
import BSbibi from '../../assets/BSbibi.jpeg';

export default class BS extends React.Component {
  render() {
    const mouse = this.props.mouse;
    return (
      <img
      {/* 我們要改圖片只要在這邊改就好了 */}
        src={BSbibi}
        alt=""
        style={{
          position: "absolute",
          left: mouse.x,
          top: mouse.y,
          width: "80px",
          height: "100px",
        }}
      />
    );
  }
}
```
- `src={BSMax}`

![](https://riyugo.com/o/210508/ph6xo4.gif)

- `src={BSbibi}`

![](https://riyugo.com/o/210508/ppuwah.gif)

最後給出我自己的理解，還有使用的心得。

其實簡單說，其實 render props 就是 props，只不只不過傳的是 function，而這個 function 就是用於做到動態的渲染，所以才叫 render props。最大的好處就是可以將實際渲染的動作與一些組件解耦，大大地增加了組件的可複用性。本身使用組件的目的就是提升可複用性，如果還要寫一堆一樣的組件就太智障了。

# 結語
這篇大致介紹了 render props 的概念，也附上一個簡單的使用方式，應該看完就懂了，實際上還是要在項目中應用會體會更深點。下篇再來談談 HOC 高階組件。若有錯誤，歡迎糾正！


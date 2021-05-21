@[TOC](React 搭配 TypeScript 新手入門)

# 前言
這篇的主要焦點會放在如何在 react 項目中使用 ts，以及一些 ts 搭配 react 的基礎技巧以及好處，比單純用 js 好在哪，當然中間也會附上一些例子。

寫這篇主要是因為暑假實習貌似要使用到的技術棧就是 react + ts，而且聽了 ts 也一陣子，也只知道什麼語法啥的，但也不知道具體有什麼用，所幸通過這篇來記錄一下 react + ts，建議看這篇前先稍微去看看 ts 的基礎，也不難。

# 正文

## 關於 TS

- TS 是什麼？

TypeScript = Type + Script（標準JS）。

TS的官方網站：TypeScript is a typed superset of JavaScript that compiles to plain JavaScript。TypeScript 是一個編譯到純 JS 的有類型定義的 JS 超集。

![](https://riyugo.com/o/210514/fte3v2.png)

- 為什麼要使用 TS？

1. TS 最大的優勢是它提供了強大的靜態分析能力，結合 TSLint 能對代碼做到更加嚴格的檢查約束。傳統的 EcmaScript 由於沒有靜態類型，即使有了 ESLint 也只能做到很基本的檢查，一些 type 問題可能線上出了 Bug 後才被發現。但在 TS 的幫助下，可以在編譯期就檢查出很多錯誤。

2. TS 讓代碼更加健壯，尤其是對於大型項目，編譯通過幾乎就代表運行正常，也給重構增加了很多信心。

3. TS 類型聲明讓提高代碼的可讀性、結構清晰。

4. IDE 的提示更加智能，這個感覺十分方便。

關於 ts 的學習如果對 js 有了解的話其實學習成本還是低一些的，具體可以參考 [一篇搞定TypeScript](https://blog.csdn.net/qq_25650505/article/details/116664304)。


## 創建 React + TS 項目
我們都知道創建一個 react 項目通過 `create-react-app`，不過這樣的項目只支持 js，要想讓 react 項目也支持 ts，我們可以使用以下命令：
```
create-react-app [項目名] --typescript
```
創好之後就會發現，之前的 jsx 都變成 tsx 了，而且最不一樣的就是多了一個文件 `tsconfig.json`，就是這個文件的配置讓我們的 react 項目也支持 ts，具體當然可以自行配置，後面學習到都還會陸續更新。

## TS 應用
其實 ts 跟 js 基本上最大的不同就在於，js 是沒有類型的，而 ts 是有的。這對於 react 項目有什麼好處呢？其實好處也很明顯，使用 ts 可以讓我們所有的變量全部都有類型，就都可以進行類型檢查，使我們的代碼更清楚，更好維護。

使用 ts，組件可以寫成這樣：
```ts
import React from "react";

const App: React.FC = () => {
  return (
    <div>
        App
    </div>
  )
};

export default App;
```
這就相當於聲明 App 是一個函數組件(Function Component)。

### Props + TS 
我們都知道 props 可以用於很方便的組件間傳參，但是使用 ts 可以讓我們對這些 props 進行更嚴格的檢查。直接上代碼看看怎麼做：

```ts
const App: React.FC = () => {
  return (
    <div>
      <TextField text="hello" num={10} />
    </div>
  );
};

const TextField: React.FC<{ text: string; num: number }> = ({ text, num }) => {
  return (
    <div>
      text : {text}
      num : {num}
    </div>
  );
};
```
這樣就可以指定 props 類型，更為規範。通過在 `React.FC()` 中傳入 ts 對象作為參數明定 props 以及各自類型，並且在箭頭函數的 `()` 中利用 es6 的解構賦值就可以獲取具體傳過來的 props。

不過其實這樣有點醜，當要傳的 props 過多就會很龐大很醜。其實比較常見的做法是通過 ts interface。來看看代碼：
```ts
const App: React.FC = () => {
  return (
    <div>
      <TextField text="hello" num={10} />
    </div>
  );
};
```
```ts
interface Props {
  text: string;
  num: number;
}

const TextField: React.FC<Props> = ({ text, num }) => {
  return (
    <div>
      <p>text : {text}</p>
      <p>num : {num}</p>
    </div>
  );
};
```
通過 ts interface 我們可以把 props 單獨抽出來，其他都一樣。ts interface 可以在屬性後面加上 `?`，表示這個屬性是可選的，這樣沒傳也就不會報錯了。
```ts
interface Person {
    firstName: string;
    middleName?: string;
    lastName: string;
}

interface Props {
  text: string;
  num: number;
  fn?: (str: string) => void; // optional
  ok? : boolean // optional
  person: Person;
}
```
### Hooks + TS 
這篇來講講如何將 hooks 配合 ts 一起使用。對於 hooks 如果還不是很熟悉，可以先去看看 [關於 React 中的 Hooks - 完全上手指南](https://blog.csdn.net/weixin_46803507/article/details/116727901)。下面介紹 `useState`、`useRef`、`useReducer` 三個最常用於跟 ts 配合在一起的 react hook。

#### useState
先從最簡單的 `useState` 說起。其實我們可以看到添加了 ts 支持其實繼自動幫我們加上類型檢查了。
```ts
import React, { useState } from "react";

interface Props {
  text: string;
  num: number;
}

const TextField: React.FC<Props> = ({ text, num }) => {
  const [count, setCount] = useState(0);

  return (
    <div>
      <p>text : {text}</p>
      <p>num : {num}</p>
    </div>
  );
};

export default TextField;
```
![](https://riyugo.com/o/210513/12sgomq.png)

我們也可以自行設定我們的允許的 state 的類型，如下：
```ts
const [count, setCount] = useState<number | null>(0);
```
其實就是很簡單的 ts 語法而已，具體就是放在 `useState` 後面的一個 `<>`。上面就允許 count 可以是 number 或是 null，傳什麼進去都可以。

#### useRef
這邊介紹 `useRef` 怎麼和 ts 配合起來，其實也是說說我自己在學習上『發現』在 react 中使用 ts 的一個好用法，其實也不是什麼用法，就是看 api 而已(hhh)。

一般 `useRef` 就這麼寫，很簡單：
```ts
import React, { useRef } from "react";

const TextField: React.FC = () => {
  const emailRef = useRef();

  return (
    <div>
      <input type="email" ref={emailRef} />
    </div>
  );
};

export default TextField;
```
但你會發現報錯，因為 ts 就是要求所有東西都要有類型，ref 也不例外，所以我們需要告訴 ref 我們到底傳進去的引用是什麼類型。那你會發現，那這樣類型豈不是太多了，哪記得完？？？其實也沒那麼難，就是將鼠標懸浮在報錯上就會給出預期類型的提示了。

![](https://riyugo.com/o/210514/8gqpi.png)

於是正確代碼如下：
```ts
import React, { useRef } from "react";

const TextField: React.FC = () => {
  const emailRef = useRef<HTMLInputElement>(null);

  return (
    <div>
      <input type="email" ref={emailRef} />
    </div>
  );
};

export default TextField;
```
再舉一個例子就可以感受到 ts 有多方便了，假設在上面代碼的基礎上我們想要由 App 傳入一個 prop 作為 input 的 onChange 回調函數。

當然我們可以直接定義一個 props 如下並使用：
```ts
interface Props {
    handleChange: () => void;
}
```
但是這樣的問題就在於，當 App 組件要傳參時根本很難知道所有 input.onChange 的 api，或者更準確一點說，因為我們的 Props.handleChange 根本沒有限制參數類型，所以 App 也不會知道說這個 handleChange 是對於 input 的。但是這樣就沒有體現出 ts 的強大，所以通過上面的方法我們可以將代碼改成如下：
```ts
import React, { useRef } from "react";

interface Props {
  handleChange: (event: React.ChangeEvent<HTMLInputElement>) => void;
}

const TextField: React.FC<Props> = ({ handleChange }) => {
  const emailRef = useRef<HTMLInputElement>(null);

  return (
    <div>
      <input type="email" ref={emailRef} onChange={handleChange} />
    </div>
  );
};

export default TextField;
```
這樣在 App 組件內傳參就變得極為方便：
```ts
import React from "react";
import TextField from "./TextField";

const App: React.FC = () => {
  return (
    <div>
      <TextField
        handleChange={(e) => {
          // e.xxx accessable
        }}
      />
    </div>
  );
};

export default App;
```
看看效果多麼方便：
![](https://riyugo.com/o/210514/nford.png)

#### useReducer
`useReducer` 搭配 ts 的使用會稍微複雜一點點，但也還好，所以我們來看看怎麼將 ts 跟 `useReducer` 用在一塊，下面大致上是演示一個簡單的 todo 應用，直接上代碼比較容易理解：
```ts
import React, { useReducer } from "react";

type Actions =
  | { type: "ADD_TODO"; text: string }
  | { type: "REMOVE_TODO"; idx: number };

interface Todo {
  text: string;
  completed: boolean;
}

type State = Array<Todo>;

const todoReducer = (state: State, action: Actions) => {
  switch (action.type) {
    case "ADD_TODO":
      return [...state, { text: action.text, completed: false }];
    case "REMOVE_TODO":
      return state.filter((_, i) => action.idx !== i);
    default:
      return state;
  }
};

const TodoList: React.FC = () => {
  const [todos, dispatch] = useReducer(todoReducer, []);

  return (
    <div>
      {JSON.stringify(todos)}
      <button
        onClick={() => {
          dispatch({ type: "ADD_TODO", text: "" });
        }}
      >
        +
      </button>
      <button
        onClick={() => {
          dispatch({ type: "REMOVE_TODO", idx: 2 });
        }}
      ></button>
    </div>
  );
};

export default Todo;
```
將 `useReducer` 跟 ts 配合起來的方式不會直接對 `useReducer` 動手腳，而是對 reducer 動手腳。

如上，通過 ts，我們為 todoReducer 所需要接受的兩個參數 state 和 action 各自定義了類型，類型都可以通過 ts 的 `type` 關鍵字自定義類型。這使得我們 dispatch 的 action 更加規範，而且都有很好的自動補全：

![](https://riyugo.com/o/210514/im7vj1.gif)

### Render Props + TS 
既然都講到 props 了，就順便也講講 ts 跟 render props 的配合。對於 render props 還不熟悉的可以先去看看 [React 中使用 render props](https://blog.csdn.net/weixin_46803507/article/details/116531795)。

直接給代碼吧，因為也沒什麼好說的，看看代碼：

- counter.tsx
```ts
import React, { useState } from "react";

interface Props {
  children: (param: {
    count: number;
    setCount: React.Dispatch<React.SetStateAction<number>>;
  }) => JSX.Element | null;
}

const Counter: React.FC<Props> = ({ children }) => {
  const [count, setCount] = useState<number>(0);

  return <div>{children({ count, setCount })}</div>;
};

export default Counter;
```
- App.tsx
```ts
 import React from "react";
import Counter from "./Counter";

const App: React.FC = () => {
  return (
    <div>
      <Counter
        children={({ count, setCount }) => (
          <div>
            {count}
            <button onClick={() => setCount(count + 1)}>+</button>
          </div>
        )}
      />
    </div>
  );
};

export default App;
```

# 結語
這篇作為 react + ts 的初學者整理了關於 ts 在 react 項目中的一些基本的使用方法還有一些代碼案例，整體上還是算是非常基本的，不過確實可以感覺到 ts 確實有他的好處，貌似對於大型項目真的挺重要的，希望之後可以在實際項目中實際應用，學習學習大老都怎麼用。若本篇有錯誤，歡迎多多指出！


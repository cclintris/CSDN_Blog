@[TOC](淺談 TypeScript 泛型 Generic)

# 前言

也是最近除了 react 之外之外因應實習的技術棧要求，所以也在學習 ts 相關東西。這篇來說說對於 ts 泛型的一些比較基本，常用的用法吧！常常在看一些源代碼像是 react 源碼時，都會看到這樣的用法，但都不明所以，趁此機會補補下這方面的東西。

# 正文

## 什麼是泛型？

在軟件開發中，我們不僅要創建一致的、定義良好的 API，同時也要考慮可重用性。我們希望組件不僅要能夠支持當前的數據類型，同時也能支持未來的數據類型，這在創建大型系統時為你提供了十分靈活的功能。

在像 C# 和 Java 這樣的語言中，可以使用泛型來創建可重用的組件，一個組件可以支持多種類型的數據。這樣用戶就可以以自己的數據類型來使用組件。那我們來看看 ts 怎麼支持泛型。

我們從簡單的例子直接講起，看看下面代碼：

```ts
type numArray = number[];
// or
type numArray = Array<number>;
```

其實就是很簡單的 ts 基礎而已，上面代碼的意思就是創建一個 `numArray` 類型，這個類型是一個 number 的數組。

那所謂的泛型就是想要做到可以創建任意類型的 type，這樣更加靈活方便。

當然在看這篇之前，還是要先對 ts 有一個基礎的了解，還不了解的可以先去看看 [TypeScript 教程](https://www.runoob.com/typescript/ts-tutorial.html)。

## 函數泛型

假設我們今天有一個方法 last，作用就是獲取數組最後一個元素，我們可以這樣寫：

```ts
const last = (arr: number[]) => {
  return arr[arr.length - 1];
};
```

這肯定是沒問題的，但是如果我們今天這個方法希望不只適用於 number，還可以適用於 string 呢？有一點 ts 基礎可能會想到那就用 `any` 就好了，於是代碼改成下面這樣：

```ts
const last = (arr: any[]) => {
  return arr[arr.length - 1];
};

const ln = last([1, 2, 3]);
const ls = last(["a", "b", "c"]);
```

這樣確實都沒問題，但是有一個缺點，看下面：
![](https://riyugo.com/i/2021/05/26/r9t6h2.gif)

問題就在於，我們會發現 ln 和 ls 的類型現在都被解釋成 `any`，但是這樣其實就根本失去了 ts 的能力了。因為我們在應用中使用 ts 無非就是為了 ts 的類型檢查的強大能力，但這樣都變成 `any` 雖然仍然是 ts 中的類型，但是某種程度上也就相當於沒有類型檢查了，這也告訴我們 `any` 不要亂用，也沒有那麼強大！

這時候泛型就派上用場了，通過泛型可以將代碼改成這樣：

```ts
const last = <T>(arr: T[]): T => {
  return arr[arr.length - 1];
};

const ln = last([1, 2, 3]);
const ls = last(["a", "b", "c"]);
```

`<T>` 的意思就是聲明一個 T 類型，這是一個特殊的變量叫做 `類型變量`，只用於表示類型。T 可以幫我們捕獲用戶傳入的參數的具體類型，之後我們就可以使用 T 了。最後 `: T` 則表示 return 的類型，可有可無，ts 其實自己就知道了。看看效果：
![](https://riyugo.com/i/2021/05/26/s69bw8.gif)

接下來再來看看關於函數泛型更多的用法～我們當然可以有不只一個泛型參數，如下：

```ts
const makeArr = <X, Y>(x: X, y: Y): [X, Y] => {
  return [x, y];
};
```

這樣我們就可以傳入兩種不同的參數，ts 也都會幫我們捕獲到正確的類型。

當然我們也可以在實際調用方法時自己覆寫泛型，常見的一個用法如下，如下：

```ts
const makeArr = <X, Y>(x: X, y: Y): [X, Y] => {
  return [x, y];
};

const arr = makeArr<string | null, number>(null, 5);
```

再來看一個例子，假設今天有一個方法 makeFullname 如下：

```ts
const makeFullname = (obj: { firstname: string; lastname: string }) => {
  return {
    ...obj,
    fullname: obj.firstname + " " + obj.lastname,
  };
};
const o = makeFullname({ firstname: "stephen", lastname: "curry" });
```

但是今天我們發現如果對我們的對象參數增加一個屬性就會報錯。如果今天需求改成，makeFullname 希望可以接受 `至少` 有 firstname 和 lastname 屬性的對象，有其他也沒關係，照樣保留下來，這時候我們就可以用 `extends generic` 的寫法，看看下面代碼：

```ts
const makeFullname = <T extends { firstname: string; lastname: string }>(
  obj: T
) => {
  return {
    ...obj,
    fullname: obj.firstname + " " + obj.lastname,
  };
};

const o = makeFullname({ firstname: "stephen", lastname: "curry", age: 15 });
```

這樣就可以完美地達成這樣的需求，只要對象參數有要求的屬性的就可以，其他多餘屬性也會被保留下來。

## 接口泛型

ts 接口常用於定義一些基本類型，在 ts 的應用中非常常用，添加上泛型也非常方便，不說太多直接看代碼吧：

```ts
interface Tab<T> {
  id: string;
  position: [number, number];
  data: T;
}

type numberTabArr = Tab<number>[];
type stringTabArr = Tab<string>[];
```

## React + TS 泛型
之前在 [React 搭配 TypeScript 新手入門](https://blog.csdn.net/weixin_46803507/article/details/116781038) 有講到了怎麼在 react 中配合 ts 使用，複習下，如果我們今天要創建一個組件，並且要接受一個 props 名稱為 name 類型為 string，我們會這麼寫：

```ts
interface Props {
  name: string;
}

const App: React.FC<Props> = ({ name }) => {
    return (
        <div>{name}</div>
    )
};
```
其實這就是 react 中使用泛型的一個例子。對 App 組件來說，`<Props>` 其實就只是一個類型，因為它並不會知道到底什麼類型要傳進來，而是我們自己定義了 `Props`，所以後面通過 `{ name }` 才能順利拿到參數。

## jsx 泛型組件
這邊最後說說配合 ts 的泛型可以怎麼實現泛型組件，以及這有什麼好處。直接從一個很實際的案例出發。

列表中的分頁加載，滾動刷新邏輯等，對於所有列表數據都是通用的，將這個列表組件書寫成泛型便可和任意類型列表數據結合，而無須通過其他方式來達到復用的目的，將列表元素聲明成 `any` 或 `Record<string, any>` 等類型。

先看不使用泛型情況下，如何實現這麽一個列表組件。此處只看列表元素的展示以闡述泛型的作用，其他邏輯比如數據加載等先忽略。

- List.tsx

```ts
interface Item {
  [id: string]: any;
  [name: string]: any;
}

interface Props {
  list: Item[];
  children: (item: Item) => JSX.Element;
}

function List({ list, children }: Props) {
  // 列表中其他逻辑...
  return <div>{list.map(children)}</div>;
}
```
上面，為了盡可能滿足大部分數據類型，將列表的元素類型定義成了 `[prop: string]: any;` 的形式，其實和 `Record<string,any>` 沒差。在這裏已經可以看到類型的丟失了，因為出現了 `any`，而我們使用 TypeScript 的首要準則是盡量避免 `any`。

然後是使用上面所定義的列表組件：
```ts
interface User {
  id: number;
  name: string;
}
const data: User[] = [
  {
    id: 1,
    name: "cc"
  },
  {
    id: 2,
    name: "lin"
  }
];

const App = () => {
  return (
    <div className="App">
      <List list={data}>
        // children render prop
        {item => {
          // ? 此處 `item.name` 类型为 `any`
          return <div key={item.id}>{item.name}</div>;
        }}
      </List>
    </div>
  );
};
```
這裏使用時，`item.name` 的類型已經成了 `any`。對於簡單數據來說，還可以接收這樣類型的丟失，但對於復雜類型，類型的丟失就完全享受不到 TypeScript 所帶來的類型便利了。

上面的實現還有個問題是它規定了列表元素必需是對象，因為使用了 ts 接口本身就是對 js 對象的一個抽象，理所應當地就不能處理元始類型數組了，比如無法渲染 ['cc','lin'] 這樣的輸入。

下面使用泛型改造上面的列表組件，讓它支持外部傳入類型。

```ts
interface Props<T> {
  list: T[];
  children: (item: T) => JSX.Element;
}

function List<T>({ list, children }: Props<T>) {
  // 列表中其他逻辑...
  return <div>{list.map(children)}</div>;
}
```
改造後，列表元素的類型完全由使用方決定，作為列表組件，內部它無須關心，同時對於外部傳遞的 children 回調中 item 入參，類型也沒有丟失。

使用改造後的泛型列表：
```ts
interface User {
  id: number;
  name: string;
}
const data: User[] = [
  {
    id: 1,
    name: "cc"
  },
  {
    id: 2,
    name: "lin"
  }
];

const App = () => {
  return (
    <div className="App">
      <List list={data}>
        {item => {
          // ? 此處 `item` 类型为 `User`
          return <div key={item.id}>{item.name}</div>;
        }}
      </List>
      <List list={["cc", "lin"]}>
        {item => {
          // ? 此處 `item` 类型为 `string`
          return <div key={item}>{item}</div>;
        }}
      </List>
    </div>
  );
};
```
可以看到，使用泛型後的列表組件更加靈活，而且完全體現出了 ts 的類型能力，複用性也得到提升。

# 結語
本篇介紹了 ts 的泛型概念，以及一些基本的用法，包含了在 react + ts 項目中的一些使用方式，個人到最後確實有點亂掉了。泛型是一個很重要的思想，但其實感覺還是使用的很不好，具體還是要再之後項目中有機會的話多想點，實際應用上才更能體會，還是太菜了多學學吧～

# 參考
| 參考            | 鏈接                                               |
| :-------------- | :------------------------------------------------- |
| TypeScript 教程 | https://www.runoob.com/typescript/ts-tutorial.html|
| TypeScript 泛型 | https://www.tslang.cn/docs/handbook/generics.html|
|React + TypeScript 实现泛型组件|https://blog.csdn.net/weixin_30894583/article/details/102232177|

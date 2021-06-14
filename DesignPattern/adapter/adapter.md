@[TOC](Java 設計模式 - 適配器模式)

# 適配器模式前言

在現實生活中，經常出現兩個對象因接口不兼容而不能在一起工作的實例，這時需要第三者進行適配。例如，講中文的人跟講英文的人對話時需要一個翻譯，用直流電的筆記本電腦接交流電源時需要一個電源適配器等。

在軟件設計中也可能出現：需要開發的具有某種業務功能的組件在現有的組件庫中已經存在，但它們與當前系統的接口規範不兼容，如果重新開發這些組件成本又很高，這時用適配器模式能很好地解決這些問題。

# 模式概述

適配器模式（Adapter）的定義如下：

> 將一個類的接口轉換成客戶希望的另外一個接口，使得原本由於接口不兼容而不能一起工作的那些類能一起工作。
>
> 適配器模式分為 `類適配器模式` 和 `對象適配器模式` 兩種，前者類之間的耦合度比後者高，且要求用戶了解現有組件庫中的相關組件的內部結構，所以應用相對較少些。

# 模式結構

適配器模式包含如下幾個角色，一起來看看：

| 角色             | 描述                                                                                                                                                                     |
| :--------------- | :----------------------------------------------------------------------------------------------------------------------------------------------------------------------- |
| 目標接口 Target  | 當前系統業務所期待的接口，它可以是抽象類或接口，定義客戶要用的特定領域的接口                                                                                             |
| 適配者類 Adaptee | 被適配的角色，定義了一個已經存在的接口，這個接口需要適配                                                                                                                 |
| 適配器類 Adapter | 可以調用另一個接口，作為一個轉換器，對適配者和抽象目標類進行適配，是模式的核心，通過繼承或引用適配者的對象，把適配者接口轉換成目標接口，讓客戶按目標接口的格式訪問適配者 |

- 類適配器模式示意圖：

![](https://ftp.bmp.ovh/imgs/2021/06/7afddbc8d6141558.png)

- 對象適配器模式示意圖：

![](https://ftp.bmp.ovh/imgs/2021/06/34c1b47333243219.png)

# 模式具體應用

自己在看適配器模式時是覺得稍微難懂一點(就是太菜了)，所幸還是來看看代碼實現，更容易理解些！

## 類適配器模式

- 適配者類

```java
package ClassAdapter;

// 適配者類
public class Adaptee {
    public void specificRequest() {
        System.out.println("适配者中的业务代码被调用！");
    }
}
```

- 目標接口

```java
package ClassAdapter;

// 目標接口
public interface Target {
    public void request();
}
```

- 類適配器類

```java
package ClassAdapter;

// 類適配器類
public class ClassAdapter extends Adaptee implements Target {
    public void request() {
        specificRequest();
    }
}
```

- Main

```java
package ClassAdapter;

// 客户端
public class Main {
    public static void main(String[] args) {
        System.out.println("类适配器模式测试：");
        Target target = new ClassAdapter();
        target.request();
    }
}

// 类适配器模式测试：
// 适配者中的业务代码被调用！
```
分析下上面的代碼：

我們從適配者類開始，`Adaptee` 中的 `specificRequest` 就是我們希望使用的方法，但是 `Adaptee` 的接口形式與當前外部不適配所以不可用(感受下hh)。

我們的系統希望的接口形式是 `Target` 這樣。因此 `Target` 中定義了一個 `request` 方法。然後通過 `ClassAdapter` 類適配器類繼承 `Adaptee` 並實現 `Target` 中的 `request` 方法，在 `request` 方法中實際調用了我們所需要的 `specificRequest`。

最後對外部來說，我們就可以通過可以接受的 `Target` 接口形式中的 `request` 方法去複用實際需要(`specificRequest`)但是存在於一個與外部不適配的類(`Adaptee`)的方法。

## 對象適配器模式

- 適配者類

```java
package ObjectAdapter;

// 適配者類
public class Adaptee {
    public void specificRequest() {
        System.out.println("适配者中的业务代码被调用！");
    }
}
```

- 目標接口

```java
package ObjectAdapter;

// 目標接口
public interface Target {
    public void request();
}
```

- 對象適配器類

```java
package ObjectAdapter;

// 對象適配器類
public class ObjectAdapter implements Target {
    private Adaptee adaptee = null;

    public ObjectAdapter(Adaptee adaptee) {
        this.adaptee = adaptee;
    }

    @Override
    public void request() {
        adaptee.specificRequest();
    }
}
```

- Main

```java
package ObjectAdapter;

// 客户端
public class Main {
    public static void main(String[] args) {
        System.out.println("对象适配器模式测试：");
        Adaptee adaptee = new Adaptee();
        Target target = new ObjectAdapter(adaptee);
        target.request();
    }
}

// 对象适配器模式测试：
// 适配者中的业务代码被调用！
```
看到在對象適配器模式中，跟類適配器模式不一樣的地方就在於，對象適配器模式中的適配器類直接引用了一個適配者對象，而在類適配器模式中是通過 `extends Adaptee`。因此我們就可以知道為什麼說類適配器模式耦合度相對高，因為本質上就使用到了繼承，自然也就增加了類之間的耦合，而且也因為只是通過繼承，所以就要求編程人員在編寫適配器類時必須知道適配者的內部細節。

# 優缺點
最後整理下對於適配器模式的優勢以及缺點：

該模式的主要優點如下：

1. 客戶端通過適配器可以透明地調用目標接口。
2. 將目標類和適配者類解耦，解決了目標類和適配者類接口不一致的問題。

其缺點是：

1. 適配器編寫過程需要結合業務場景全面考慮，可能會增加系統的復雜性。
2. 增加代碼閱讀難度，降低代碼可讀性，過多使用適配器會使系統代碼變得淩亂。

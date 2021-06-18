@[TOC](Java 設計模式 - 裝飾器模式)

# 裝飾器模式前言

上班族大多都有睡懶覺的習慣，每天早上上班時間都很緊張，於是很多人為了多睡一會，就會用方便的方式解決早餐問題。有些人早餐可能會吃煎餅，煎餅中可以加雞蛋，也可以加香腸，但是不管怎麽“加碼”，都還是一個煎餅。在現實生活中，常常需要對現有產品增加新的功能或美化其外觀，如房子裝修、相片加相框等，都是裝飾器模式。

在軟件開發過程中，有時想用一些現存的組件。這些組件可能只是完成了一些核心功能。但在不改變其結構的情況下，可以動態地擴展其功能。所有這些都可以釆用裝飾器模式來實現。

# 模式概述

裝飾器（Decorator）模式的定義：

> 指在不改變現有對象結構的情況下，動態地給該對象增加一些職責（即增加其額外功能）的模式，它屬於對象結構型模式。

通常情況下，擴展一個類的功能會使用繼承方式來實現。但繼承具有靜態特征，耦合度高，並且隨著擴展功能的增多，子類會很膨脹。如果使用組合關系來創建一個包裝對象（即裝飾對象）來包裹真實對象，並在保持真實對象的類結構不變的前提下，為其提供額外的功能，這就是裝飾器模式的目標。下面來分析其基本結構和實現方法。

# 模式結構

裝飾器模式主要包含以下角色：

| 模式角色                          | 描述                                                                 |
| :-------------------------------- | :------------------------------------------------------------------- |
| 抽象構件（Component）角色         | 定義一個抽象接口以規範準備接收附加責任的對象                         |
| 具體構件（ConcreteComponent）角色 | 實現抽象構件，通過裝飾角色為其添加一些職責                           |
| 抽象裝飾（Decorator）角色         | 繼承抽象構件，並包含具體構件的實例，可以通過其子類擴展具體構件的功能 |
| 具體裝飾（ConcreteDecorator）角色 | 實現抽象裝飾的相關方法，並給具體構件對象添加附加的責任               |

![](https://seikim.com/i/2021/06/18/gyux3c.png)

# 模式具體應用

下面就來演示下怎麼實現裝飾器模式：

- 抽象构件角色

```java
// 抽象构件角色
public interface Component {
    public void operation();
}
```

- 具体构件角色

```java
// 具体构件角色
public class ConcreteComponent implements Component {
    public ConcreteComponent() {
        System.out.println("创建具体构件角色");
    }
    public void operation() {
        System.out.println("调用具体构件角色的方法operation()");
    }
}
```

- 抽象装饰角色

```java
// 抽象装饰角色
public class Decorator implements Component {
    private Component component = null;
    public Decorator(Component component) {
        this.component = component;
    }
    public void operation() {
        component.operation();
    }
}
```

- 具体装饰角色

```java
// 具体装饰角色
public class ConcreteDecorator extends Decorator {
    public ConcreteDecorator(Component component) {
        super(component);
    }
    public void operation() {
        super.operation();
        addedFunction();
    }
    public void addedFunction() {
        System.out.println("为具体构件角色增加额外的功能addedFunction()");
    }

}
```

- Main

```java
public class Main {
    public static void main(String[] args) {
        Component p = new ConcreteComponent();
        p.operation();
        System.out.println("---------------------------------");
        Component d = new ConcreteDecorator(p);
        d.operation();
    }
}

// 创建具体构件角色
// 调用具体构件角色的方法operation()
// ---------------------------------
// 调用具体构件角色的方法operation()
// 为具体构件角色增加额外的功能addedFunction()
```

我們分析一下上面裝飾者模式的實現。首先我們看到 `Component` 接口，我們就把這個接口想像成一個現成的組件庫，然後 `ConcreteComponent` 就是實現這個接口的 `operation` 方法，就想成是這個現成的組件庫提供了一個現成的 `operation` 方法。

現在我們想做的就是，我們希望在 `operation` 方法的基礎上還要再加一些功能。所以通過 `Decorator` 類實現 `Component` 接口，這個類有一個 component 的引用，`operation` 被實現為單純的調用 `Component` 的 `operation` 方法。接著，通過 `ConcreteDecorator` 實際做出我們需要的新方法，`operation` 方法覆蓋了 `Decorator`，增加了一個 `addedFunction` 方法調用。

最後看 main 的代碼就可以看到效果啦～

# 優缺點

裝飾器模式的主要優點有：

1. 裝飾器是繼承的有力補充，比繼承靈活，在不改變原有對象的情況下，動態的給一個對象擴展功能，即插即用
2. 通過使用不用裝飾類及這些裝飾類的排列組合，可以實現不同效果
3. 裝飾器模式完全遵守開閉原則

其主要缺點是：

1. 裝飾器模式會增加許多子類，過度使用會增加程序得復雜性。

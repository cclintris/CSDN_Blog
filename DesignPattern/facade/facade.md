@[TOC](Java 設計模式 - 外觀模式)

# 外觀模式前言
在現實生活中，常常存在辦事較復雜的例子，如辦房產證或註冊一家公司，有時要與多個部門聯系，這時要是有一個綜合部門能解決一切手續問題就好了。

軟件設計也是這樣，當一個系統的功能越來越強，子系統會越來越多，客戶對系統的訪問也變得越來越復雜。這時如果系統內部發生改變，客戶端也要跟著改變，這違背了“開閉原則”，也違背了“迪米特法則”。所以有必要為多個子系統提供一個統一的接口，從而降低系統的耦合度，這就是外觀模式的目標。

# 模式概述
外觀（Facade）模式又叫作門面模式，是一種通過為多個復雜的子系統提供一個一致的接口，而使這些子系統更加容易被訪問的模式。該模式對外有一個統一接口，外部應用程序不用關心內部子系統的具體細節，這樣會大大降低應用程序的復雜度，提高了程序的可維護性。

在日常編碼工作中，我們都在有意無意的大量使用外觀模式。只要是高層模塊需要調度多個子系統（2個以上的類對象），我們都會下意識地創建一個新的類封裝這些子系統，提供精簡的接口，讓高層模塊可以更加容易地調用這些子系統的功能。

# 模式結構
外觀（Facade）模式的結構比較簡單，主要是定義了一個高層接口。它包含了對各個子系統的引用，客戶端可以通過它訪問各個子系統的功能。現在來分析其基本結構和實現方法。

外觀（Facade）模式包含以下主要角色：

|模式角色|描述|
|:------|:-----|
|外觀（Facade）角色|為多個子系統對外提供一個共同的接口|
|子系統（Sub System）角色|實現系統的部分功能，客戶可以通過外觀角色訪問它|
|客戶（Client）角色|通過一個外觀角色訪問各個子系統的功能|

外觀模式結構圖：

![](https://seikim.com/i/2021/06/19/ibp9dv.png)

# 模式具體應用
我們來看看具體怎麼代碼實現外觀模式～

- 子系统角色

```java
public class Subsystem1 {
    public void method1() {
        System.out.println("子系统1的method1()被调用！");
    }
}
```

- 子系统角色

```java
public class Subsystem2 {
    public void method2() {
        System.out.println("子系统2的method2()被调用！");
    }
}
```

- 子系统角色

```java
public class Subsystem3 {
    public void method3() {
        System.out.println("子系统3的method3()被调用！");
    }
}
```

- 外观角色

```java
// 外观角色
public class Facade {
    private Subsystem1 subsystem1 = new Subsystem1();
    private Subsystem2 subsystem2 = new Subsystem2();
    private Subsystem3 subsystem3 = new Subsystem3();

    public void method() {
        subsystem1.method1();
        subsystem2.method2();
        subsystem3.method3();
    }
}
```

- Main

```java
public class Main {
    public static void main(String[] args) {
        Facade facade = new Facade();
        facade.method();
    }
}

// 子系统1的method1()被调用！
// 子系统2的method2()被调用！
// 子系统3的method3()被调用！
```
外觀模式比較簡單，在這邊就不在做什麼分析了。

# 優缺點

外觀（Facade）模式是“迪米特法則”的典型應用，它有以下主要優點：

1. 降低了子系統與客戶端之間的耦合度，使得子系統的變化不會影響調用它的客戶類。

2. 對客戶屏蔽了子系統組件，減少了客戶處理的對象數目，並使得子系統使用起來更加容易。

外觀（Facade）模式的主要缺點如下：

1. 不能很好地限制客戶使用子系統類，很容易帶來未知風險。

2. 增加新的子系統可能需要修改外觀類或客戶端的源代碼，違背了“開閉原則”，即對擴展開放對修改關閉。


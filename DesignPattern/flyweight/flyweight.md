@[TOC](Java 設計模式 － 享元模式)

# 享元模式前言
在面向對象程序設計過程中，有時會面臨要創建大量相同或相似對象實例的問題。創建那麽多的對象將會耗費很多的系統資源，它是系統性能提高的一個瓶頸。

例如，圍棋和五子棋中的黑白棋子，教室裏的桌子和凳子等。這些對象有很多相似的地方，如果能把它們相同的部分提取出來共享，則能節省大量的系統資源，這就是享元模式的產生背景。

# 模式概述
享元（Flyweight）模式的定義：

> 運用共享技術來有效地支持大量細粒度對象的復用。它通過共享已經存在的對象來大幅度減少需要創建的對象數量、避免大量相似類的開銷，從而提高系統資源的利用率。

享元模式的定義提出了兩個要求，`細粒度` 和 `共享對象`。因為要求細粒度，所以不可避免地會使對象數量多且性質相近，此時我們就將這些對象的信息分為兩個部分：`內部狀態` 和 `外部狀態`。

- 內部狀態指對象共享出來的信息，存儲在享元信息內部，並且不回隨環境的改變而改變

- 外部狀態指對象得以依賴的一個標記，隨環境的改變而改變，不可共享

比如，連接池中的連接對象，保存在連接對象中的用戶名、密碼、連接URL等信息，在創建對象的時候就設置好了，不會隨環境的改變而改變，這些為內部狀態。而當每個連接要被回收利用時，我們需要將它標記為可用狀態，這些為外部狀態。

# 模式結構
來看看享元模式中包含哪些角色以及整體結構～

|模式角色|描述|
|:-----|:-----|
|抽象享元角色（Flyweight|是所有的具體享元類的基類，為具體享元規範需要實現的公共接口，非享元的外部狀態以參數的形式通過方法傳入|
|具體享元（Concrete Flyweight）角色|實現抽象享元角色中所規定的接口|
|非享元（Unsharable Flyweight)角色|是不可以共享的外部狀態，它以參數的形式註入具體享元的相關方法中|
|享元工廠（Flyweight Factory）角色|負責創建和管理享元角色。當客戶對象請求一個享元對象時，享元工廠檢査系統中是否存在符合要求的享元對象，如果存在則提供給客戶；如果不存在的話，則創建一個新的享元對象|

![](https://seikim.com/i/2021/06/20/3lx0pj.png)

# 模式具體應用
當然到了這邊，就是來看看關於享元模式的實現啦！

- 非享元角色

```java
// 非享元角色
public class UnsharedConcreteFlyweight {
    private String info;
    UnsharedConcreteFlyweight(String info) {
        this.info = info;
    }
    public String getInfo() {
        return info;
    }
    public void setInfo(String info) {
        this.info = info;
    }
}
```

- 抽象享元角色

```java
// 抽象享元角色
public interface Flyweight {
    public void operation(UnsharedConcreteFlyweight state);
}
```

- 具体享元角色

```java
// 具体享元角色
public class ConcreteFlyweight implements Flyweight {
    private String key;
    ConcreteFlyweight(String key) {
        this.key = key;
        System.out.println("具体享元" + key + "被创建！");
    }
    public void operation(UnsharedConcreteFlyweight outState) {
        System.out.print("具体享元" + key + "被调用，");
        System.out.println("非享元信息是:" + outState.getInfo());
    }
}
```

- 享元工厂角色

```java
import java.util.*;

// 享元工厂角色
public class FlyweightFactory {
    private HashMap<String, Flyweight> flyweights = new HashMap<String, Flyweight>();
    public Flyweight getFlyweight(String key) {
        Flyweight flyweight = (Flyweight) flyweights.get(key);
        if (flyweight != null) {
            System.out.println("具体享元" + key + "已经存在，被成功获取！");
        } else {
            flyweight = new ConcreteFlyweight(key);
            flyweights.put(key, flyweight);
        }
        return flyweight;
    }
}
```

- Main

```java
public class Main {
    public static void main(String[] args) {
        FlyweightFactory factory = new FlyweightFactory();
        Flyweight f01 = factory.getFlyweight("a");
        Flyweight f02 = factory.getFlyweight("a");
        Flyweight f03 = factory.getFlyweight("a");
        Flyweight f11 = factory.getFlyweight("b");
        Flyweight f12 = factory.getFlyweight("b");
        f01.operation(new UnsharedConcreteFlyweight("第1次调用a。"));
        f02.operation(new UnsharedConcreteFlyweight("第2次调用a。"));
        f03.operation(new UnsharedConcreteFlyweight("第3次调用a。"));
        f11.operation(new UnsharedConcreteFlyweight("第1次调用b。"));
        f12.operation(new UnsharedConcreteFlyweight("第2次调用b。"));
    }
}

// 具体享元a被创建！
// 具体享元a已经存在，被成功获取！
// 具体享元a已经存在，被成功获取！
// 具体享元b被创建！
// 具体享元b已经存在，被成功获取！
// 具体享元a被调用，非享元信息是:第1次调用a。
// 具体享元a被调用，非享元信息是:第2次调用a。
// 具体享元a被调用，非享元信息是:第3次调用a。
// 具体享元b被调用，非享元信息是:第1次调用b。
// 具体享元b被调用，非享元信息是:第2次调用b。
```
個人覺得享元模式還算好理解，把代碼看懂就是了，這邊就不再做過多的分析。

# 優缺點

享元模式的主要優點是：

1. 相同對象只要保存一份，這降低了系統中對象的數量，從而降低了系統中細粒度對象給內存帶來的壓力

其主要缺點是：

1. 為了使對象可以共享，需要將一些不能共享的狀態外部化，這將增加程序的複雜性

2. 讀取享元模式的外部狀態會使得運行時間稍微變長

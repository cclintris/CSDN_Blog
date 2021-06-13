@[TOC](Java 設計模式 - 中介者模式)

# 模式概述

在實際開發中，有可能建了很多個類，而這些類又需要互相引用，當項目變得龐大時，很容易造成引用關係過於複雜的情況。這時候我們就可以應用上中介者模式。中介者模式中，所有的相關對象都通過中介者對象來通信，而不是直接互相引用，所以當一個對象發生改變時，只需要通知中介者對象即可，不會造成太多的影響，因為沒有把直接的類引用直接硬編碼在其他類當中。

# 模式結構

中介者模式下通常會包含以下角色:

| 角色       | 描述                                                                                                                                                                                 |
| :--------- | :----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------- |
| 抽象中介者 | 定義一個接口，該接口用於與各同事對象之間的通信                                                                                                                                       |
| 具體中介者 | 實現抽象中介者，通過協調各個同事對象的關係來實現協作行為，了解並維護各個對象的引用以及互相的關係                                                                                     |
| 抽象同事類 | 定義各同事的公有方法                                                                                                                                                                 |
| 具體同事類 | 抽象同事類的子類，實現了抽象同事類中定義的方法，每一個同事對象都引用一個中介者對象，每一個同事對象在需要和其他對象通信時，都是先與中介者通信，通過中介者來間接完成與其他同事類的通信 |

模式結構圖：

![](https://seikim.com/i/2021/06/13/n85zz1.png)


# 模式適用場景

這邊總結一下中介者模式適用的場景：

1. 系統中對象之間存在複雜
的引用關系，產生的相互依賴關係使得結構混亂且難以理解

2. 一個對象由於引用了其他很多對象並且直接和這些對象通信，導致難以復用該對象

3. 想通過一個中間類來封裝多個類中的行為，而又不想生成太多的子類

# 模式具體應用
來看看一個實際簡單實現了中介者模式的代碼，會更加直觀：

![](https://seikim.com/i/2021/06/13/12acjvc.png)

- 抽象中介者

```java
// 抽象中介者

public abstract class Mediator {
    public abstract void register(Colleague colleague);
    public abstract void relay(Colleague colleague);
}
```

- 抽象同事类

```java
// 抽象同事类

public abstract class Colleague {
    protected Mediator mediator;

    public void setMediator(Mediator mediator) {
        this.mediator = mediator;
    }

    public abstract void receive();
    public abstract void send();
}
```
-  具体中介者

```java
import java.util.*;

// 具体中介者

public class ConcreteMediator extends Mediator {
    private List<Colleague> colleagues = new ArrayList<Colleague>();
    
    public void register(Colleague colleague) {
        if (!colleagues.contains(colleague)) {
            colleagues.add(colleague);
            colleague.setMediator(this);
        }
    }
    public void relay(Colleague cl) {
        for (Colleague ob : colleagues) {
            if (!ob.equals(cl)) {
                ((Colleague) ob).receive();
            }
        }
    }
}
```

- 具体同事類 1

```java
// 具體同事類

public class ConcreteColleague1 extends Colleague {
    public void receive() {
        System.out.println("具体同事类1收到请求。");
    }
    public void send() {
        System.out.println("具体同事类1发出请求。");
        mediator.relay(this); // 请中介者转发
    }
}
```

-  具體同事類 2

```java
// 具體同事類

public class ConcreteColleague2 extends Colleague {
    public void receive() {
        System.out.println("具体同事类2收到请求。");
    }
    public void send() {
        System.out.println("具体同事类2发出请求。");
        mediator.relay(this); // 请中介者转发
    }
}
```

- main

```java
public class main {
    public static void main(String[] args) {
        Mediator md = new ConcreteMediator();
        Colleague c1, c2;
        c1 = new ConcreteColleague1();
        c2 = new ConcreteColleague2();
        md.register(c1);
        md.register(c2);
        c1.send();
        System.out.println("-------------");
        c2.send();
    }
}

// 具体同事类1发出请求。
// 具体同事类2收到请求。
// -------------
// 具体同事类2发出请求。
// 具体同事类1收到请求。
```
分析下上述代碼，可以看到我們在 `ConcreteMediator` 類中有一個 `colleagues`，這個私有變量的用意就是用於註冊該中介者的服務對象類。然後還實現了 `Mediator` 中的兩個抽象方法 `register` 跟 `relay`，`register` 就用於註冊對象，`relay` 負責給對象請求中介者轉發。

然後再看到 `ConcreteColleague1` 以及 `ConcreteColleague2`，實現了 `receive` 和 `send` 方法。重點看到 `send` 中實際上發出請求會通過自身的中介者引用的 `relay` 方法將請求轉發給所有向中介者註冊過的對象。

所以可以看到在中介者模式中，對象之間的通信不再需要通過持有各自的引用，而是只要抽象同事類中持有一個中介者引用，並向希望協助轉發請求的中介者註冊，所有請求就都可以通過該中介者轉發給其他也在該中介者管理下的所有對象，避免了對象之間複雜的引用關係。

# 優缺點
最後就總結一下關於中介者模式的優缺點：

1. 優點：簡化了對象之間的交互，將各同事解耦，減少子類生成，對於複雜的對象之間的交互，通過引入中介者可以簡化各同事類的設計和實現。

2. 缺點：具體中介者類中包含了同事之間的交互細節，可能導致其非常復雜，使得系統難以維護，導致單一類的職責過於繁重，規模過大的話可能不好維護。


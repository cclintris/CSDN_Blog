@[TOC](Java 設計模式 - 組合模式)

# 組合模式前言
在現實生活中，存在很多“部分-整體”的關系，例如，大學中的部門與學院、總公司中的部門與分公司、學習用品中的書與書包、生活用品中的衣服與衣櫃、以及廚房中的鍋碗瓢盆等。在軟件開發中也是這樣，例如，文件系統中的文件與文件夾、窗體程序中的簡單控件與容器控件等。對這些簡單對象與復合對象的處理，如果用組合模式來實現會很方便。

# 模式概述
組合（Composite Pattern）模式的定義：

>有時又叫作整體-部分（Part-Whole）模式，它是一種將對象組合成 `樹狀` 的層次結構的模式，用來表示“整體-部分”的關系，使用戶對單個對象和組合對象具有一致的訪問性，屬於結構型設計模式。

組合模式一般用來描述整體與部分的關系，它將對象組織到樹形結構中，頂層的節點被稱為根節點，根節點下面可以包含樹枝節點和葉子節點，樹枝節點下面又可以包含樹枝節點和葉子節點，樹形結構圖如下。其實就是一棵樹啦hhh。～

![](https://seikim.com/i/2021/06/16/kfd5lf.png)

在組合模式中，整個樹形結構中的對象都屬於同一種類型，帶來的好處就是用戶不需要辨別是樹枝節點還是葉子節點，可以直接進行操作，給用戶的使用帶來極大的便利。

# 模式結構

組合模式分為 `透明式` 的組合模式和 `安全式` 的組合模式。

組合模式包含以下主要角色：

1. `抽象構件（Component）角色`：

它的主要作用是為樹葉構件和樹枝構件聲明公共接口，並實現它們的默認行為。

在透明式的組合模式中抽象構件還聲明訪問和管理子類的接口；在安全式的組合模式中不聲明訪問和管理子類的接口，管理工作由樹枝構件完成。（總的抽象類或接口，定義一些通用的方法，比如新增、刪除）

2. `樹葉構件（Leaf）角色`：

是組合中的葉節點對象，它沒有子節點，用於繼承或實現抽象構件。

3. `樹枝構件（Composite）角色` / `中間構件`：

是組合中的分支節點對象，它有子節點，用於繼承和實現抽象構件。它的主要作用是存儲和管理子部件，通常包含 `Add()`、`Remove()`、`GetChild()` 等方法。

## 透明式
在該方式中，由於抽象構件聲明了所有子類中的全部方法，所以客戶端無須區別樹葉對象和樹枝對象，對客戶端來說是透明的。

但其缺點是：樹葉構件本來沒有 Add()、Remove() 及 GetChild() 方法，卻要實現它們（空實現)，這樣會帶來一些安全性問題。

![](https://seikim.com/i/2021/06/16/ozp40i.png)


## 安全式
在該方式中，將管理子構件的方法移到樹枝構件中，抽象構件和樹葉構件沒有對子對象的管理方法，這樣就避免了上一種方式的安全性問題，但由於葉子和分支有不同的接口，客戶端在調用時要知道樹葉對象和樹枝對象的存在，所以失去了透明性。

![](https://seikim.com/i/2021/06/16/ozp8md.png)

# 模式具體應用
其實組合模式沒有那麼複雜，但個人感覺光看敘述還是蠻複雜的，還是從代碼入手吧～

假如要訪問集合 `c0={leaf1,{leaf2,leaf3}}` 中的元素，其對應的樹狀圖如圖:

![](https://seikim.com/i/2021/06/16/qsmodc.png)

## 透明式

- 抽象构件

```java
// 抽象构件
public interface Component {
    public void add(Component c);
    public void remove(Component c);
    public Component getChild(int i);
    public void operation();
}
```
- 树叶构件

```java
// 树叶构件
public class Leaf implements Component {
    private String name = "";

    public Leaf(String name) {
        this.name = name;
    }

    public void add(Component c) {}

    public void remove(Component c) {}

    public Component getChild(int i) {
        return null;
    }

    public void operation() {
        System.out.println("树叶" + name + "：被访问！");
    }
}
```
- 树枝构件

```java
import java.util.*;

// 树枝构件
public class Composite implements Component {
    private final ArrayList<Component> children = new ArrayList<>();

    public void add(Component c) {
        children.add(c);
    }

    public void remove(Component c) {
        children.remove(c);
    }

    public Component getChild(int i) {
        return children.get(i);
    }

    public void operation() {
        for (Object obj : children) {
            ((Component) obj).operation();
        }
    }
}
```
- Main

```java
public class Main {
    public static void main(String[] args) {
        Component c0 = new Composite();
        Component c1 = new Composite();
        Component leaf1 = new Leaf("1");
        Component leaf2 = new Leaf("2");
        Component leaf3 = new Leaf("3");
        c0.add(leaf1);
        c0.add(c1);
        c1.add(leaf2);
        c1.add(leaf3);
        c0.operation();
    }
}

// 树叶1：被访问！
// 树叶2：被访问！
// 树叶3：被访问！
```

## 安全式

- 抽象构件

首先修改 Component 代碼，只保留層次的公共行為。

```java
// 抽象构件
public interface Component {
    public void operation();
}
```
- 树叶构件

```java
// 树叶构件
public class Leaf implements Component {
    private String name = "";

    public Leaf(String name) {
        this.name = name;
    }

    public void operation() {
        System.out.println("树叶" + name + "：被访问！");
    }
}
```
- 树枝构件

```java
import java.util.*;

// 树枝构件
public class Composite implements Component {
    private final ArrayList<Component> children = new ArrayList<>();

    public void add(Component c) {
        children.add(c);
    }

    public void remove(Component c) {
        children.remove(c);
    }

    public Component getChild(int i) {
        return children.get(i);
    }

    public void operation() {
        for (Object obj : children) {
            ((Component) obj).operation();
        }
    }
}
```
- Main

然後修改客戶端代碼，將樹枝構件類型更改為 Composite 類型，以便獲取管理子類操作的方法。

```java
public class Main {
    public static void main(String[] args) {
        Composite c0 = new Composite();
        Composite c1 = new Composite();
        Component leaf1 = new Leaf("1");
        Component leaf2 = new Leaf("2");
        Component leaf3 = new Leaf("3");
        c0.add(leaf1);
        c0.add(c1);
        c1.add(leaf2);
        c1.add(leaf3);
        c0.operation();
    }
}

// 树叶1：被访问！
// 树叶2：被访问！
// 树叶3：被访问！
```

# 優缺點

關於組合模式的優缺點整理如下：

組合模式的主要 `優點` 有：

1. 組合模式使得客戶端代碼可以一致地處理單個對象和組合對象，無須關心自己處理的是單個對象，還是組合對象，這簡化了客戶端代碼

2. 更容易在組合體內加入新的對象，客戶端不會因為加入了新的對象而更改源代碼，滿足“開閉原則”

其主要 `缺點` 是：

1. 設計較復雜，客戶端需要花更多時間理清類之間的層次關系

2. 不容易限製容器中的構件

3. 不容易用繼承的方法來增加構件的新功能


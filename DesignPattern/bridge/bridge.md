@[TOC](Java 設計模式 - 橋接模式)

# 橋接模式前言
在現實生活中，某些類具有兩個或多個維度的變化，如圖形既可按形狀分，又可按顏色分。如何設計類似於 Photoshop 這樣的軟件，能畫不同形狀和不同顏色的圖形呢？如果用繼承方式，m 種形狀和 n 種顏色的圖形就有 m*n 種，不但對應的子類很多，而且擴展困難。

當然，這樣的例子還有很多，如不同顏色和字體的文字、不同品牌和功率的汽車、不同性別和職業的男女、支持不同平臺和不同文件格式的媒體播放器等。如果用橋接模式就能很好地解決這些問題。

# 模式概述
橋接（Bridge）模式的定義如下：

> 將抽象與實現分離，使它們可以獨立變化。它是用組合關系代替繼承關系來實現，從而降低了抽象和實現這兩個可變維度的耦合度。

# 模式結構
橋接模式包含以下結構：

|模式角色|描述|
|:-----|:-----|
|抽象化（Abstraction）角色|定義抽象類，並包含一個對實現化對象的引用|
|擴展抽象化（Refined Abstraction）角色|是抽象化角色的子類，實現父類中的業務方法，並通過組合關系調用實現化角色中的業務方法|
|實現化（Implementor）角色|定義實現化角色的接口，供擴展抽象化角色調用|
|具體實現化（Concrete Implementor）角色|給出實現化角色接口的具體實現|

![](https://seikim.com/i/2021/06/17/pk14pu.png)

# 模式具體應用

- 实现化角色

```java
// 实现化角色
public interface Implementor {
    public void OperationImpl();
}
```

- 具体实现化角色

```java
// 具体实现化角色
public class ConcreteImplementor implements Implementor {
    public void OperationImpl() {
        System.out.println("具体实现化(Concrete Implementor)角色被访问");
    }
}
```

- 抽象化角色

```java
// 抽象化角色
public abstract class Abstraction {
    protected Implementor imple;
    protected Abstraction(Implementor imple) {
        this.imple = imple;
    }
    public abstract void Operation();
}
```

- 扩展抽象化角色

```java
// 扩展抽象化角色
public class RefinedAbstraction extends Abstraction {
    public RefinedAbstraction(Implementor imple) {
        super(imple);
    }
    public void Operation() {
        System.out.println("扩展抽象化(Refined Abstraction)角色被访问");
        imple.OperationImpl();
    }
}
```

- Main

```java
public class Main {
    public static void main(String[] args) {
        Implementor imple = new ConcreteImplementor();
        Abstraction abs = new RefinedAbstraction(imple);
        abs.Operation();
    }
}

// 扩展抽象化(Refined Abstraction)角色被访问
// 具体实现化(Concrete Implementor)角色被访问
```

# 優缺點

橋接（Bridge）模式的優點是：

1. 抽象與實現分離，擴展能力強
2. 符合開閉原則
3. 符合合成復用原則
4. 其實現細節對客戶透明

缺點是：

由於聚合關系建立在抽象層，要求開發者針對抽象化進行設計與編程，能正確地識別出系統中兩個獨立變化的維度，這增加了系統的理解與設計難度。


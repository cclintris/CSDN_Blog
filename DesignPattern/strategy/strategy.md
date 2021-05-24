@[TOC](Java設計模式-策略模式)

# 什麼是策略模式?
在阎宏博士的《JAVA与模式》中，對於策略模式是這麼描述的:
> 策略模式属于对象的行为模式。其用意是针对一组算法，将每一个算法封装到具有共同接口的独立的类中，从而使得它们可以相互替换。策略模式使得算法可以在不影响到客户端的情况下发生变化。

# 策略模式的結構
策略模式其實是對「算法」進行包裝的，是把使用算法的工作和算法本身分離開的，委託給不同對象管理。

策略模式的做法通常是把一系列的算法都包裝到一系列的策略類裡面，而這一系列策略類是一個抽象策略類的子類。用一句話說，其實就是「準備一組算法，並將每一個算法封裝起來，使他們可以相互替換」。

下圖就是策略模式的結構圖:
![](https://wtfhhh.oss-cn-beijing.aliyuncs.com/strategy.png)

策略模式涉及到三個角色:

- 環境角色(Context): 持有一個對抽象策略類 Strategy 的引用。

- 抽象策略角色(Abstract Strategy): 這是一個抽象角色，通常由接口或是抽象類實現。此角色要給出所有具體策略類所需要接口。

- 具體策略角色(Concrete Strategy): 實現所有抽象策略角色提供的接口，包裝算法核心行為。

# 代碼實現

- Context 環境角色
```java
package strategy;

public class Context {

    private Strategy strategy;

    public Context(Strategy strategy) {
        this.strategy = strategy;
    }

    public void contextInterface() {
        strategy.strategyInterface();
    }
}
```

- Abstract Strategy 抽象策略角色
```java
package strategy;

public interface Strategy {
    public void strategyInterface();
}
```

- Concrete Strategy 具體策略角色
```java
package strategy;

public class ConcreteStrategy implements Strategy {

    @Override
    public void strategyInterface() {
        // 相關算法業務實現
    }
}
```

# 實際案例
假設今天要設計一個販賣各類書籍的電商的購物車系統。你可能會覺得很簡單，不就是把購物車中所有品項的單價乘上各自的單價最後加總就好了嗎?沒錯，但是現實情況通常不會這麼簡單的。比如，電商可能會對高級會員提供每本書 20% 的促銷，對中級會員提供 10% 的折扣，對初級會員則提供 5% 的折扣。這個場景就很適合使用策略模式:

總結一下需求，折扣是根據以下算法其一進行計算的:

- 算法一: 對初級會員提供 5% 折扣
- 算法二: 對中級會員提供 10% 折扣
- 算法三: 對高級會員提供 20% 折扣

則使用策略模式，需求結構可以整理成如下:
![](https://wtfhhh.oss-cn-beijing.aliyuncs.com/strategy-1.png)

來看看具體的代碼實現吧!

- 抽象折扣類
```java
package OnlineRetail;

public interface DiscountStrategy {

    // 計算價格
    public double calcPrice(double bookPrice);
}
```

- 具體初級會員折扣類
```java
package OnlineRetail;

public class PrimaryMember implements DiscountStrategy {
    @Override
    public double calcPrice(double bookPrice) {
        System.out.println("初級會員打 95 折");
        return bookPrice * 0.95;
    }
}
```

- 具體中級會員折扣類
```java
package OnlineRetail;

public class MediateMember implements DiscountStrategy {
    @Override
    public double calcPrice(double bookPrice) {
        System.out.println("中級會員打 9 折");
        return bookPrice * 0.9;
    }
}
```

- 具體高級會員折扣類
```java
package OnlineRetail;

public class AdvancedMember implements DiscountStrategy {
    @Override
    public double calcPrice(double bookPrice) {
        System.out.println("高級會員打 8 折");
        return bookPrice * 0.8;
    }
}
```

- 價格類
```java
package OnlineRetail;

public class Price {

    private DiscountStrategy strategy;

    public Price(DiscountStrategy strategy) {
        this.strategy = strategy;
    }

    public double quote(double bookPrice) {
        return this.strategy.calcPrice(bookPrice);
    }
}
```

- 客戶類
```java
package OnlineRetail;

public class Customer {
    public static void main(String[] args) {
        DiscountStrategy strategy = new MediateMember();
        Price price = new Price(strategy);
        double bookValue = 500;
        System.out.println("打折前的價格: " + bookValue);
        double discountPrice = price.quote(bookValue);
        System.out.println("打完折後價格: " + discountPrice);
    }
}
```
運行結果如下:
![](https://wtfhhh.oss-cn-beijing.aliyuncs.com/strategy-2.png)
以上代碼不難看出，策略模式僅僅是封裝了算法，可以很好的將新的算法添加到現有的系統中，也可以很好的將棄用的算法從系統中「退休」。策略模式並不決定什麼時候使用甚麼算法，使用算法是由客戶自己決定的。

讀到這邊的相信都已經了解什麼是策略模式了，最後總結一下策略模式的特點以及適用場景。

# 特點

- 策略模式的重心

其實策略模式的重心不在於如何實現算法，而是在於如何更好的組織，調度這些算法。

- 算法的平等性

策略模式很大的一個特色就是算法的平等性。對於一系列的算法，大家的地位都是一樣的，而正因為地位一樣，才可以做到替換。所有算法都是獨立的，沒有依賴關係。

簡單來說，策略模式中的一系列算法: 相同行為，不同實現。

- 運行時策略的唯一性
雖然說策略模式中的算法可以相互替換，但是同一時刻仍然只會有一個具體算法被實際用到，實際運行。

# 優缺點
策略模式的優點:

1. 策略模式給出了一個公共的算法族，提供了一個公共的接口統一實現，減少了代碼的重複性，也提高了可擴展性。

2. 如很多設計模式一樣，策略模式可以避免很多的 `if/else` 語句，提升了代碼的可維護性。

策略模式的缺點:

1. 客戶端需要知道所有具體的算法，才能決定自己要用哪一個。這就表示，客戶端還要去理解這些算法並在恰當的時機使用，且萬一有很多很多個算法，那將會大大提高對客戶端的要求。

2. 由於策略模式把每個具體的算法都封裝成一個單獨的類，所以如果策略很多的話，那麼系統的對象數將會很可觀，不易維護。


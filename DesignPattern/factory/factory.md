@[TOC](Java設計模式-工廠模式)

# 工廠模式
設計模式可以說是非常重要的一個部分，許多大牛都說，設計模式是一個進階程序員必備的。總共有 23 種設計模式，這篇就先來介紹工廠模式。

## 簡單工廠模式
![](https://wtfhhh.oss-cn-beijing.aliyuncs.com/factory-1.png)

簡單工廠模式，也叫工廠模式，是最簡單的一種工廠模式，簡單來說，就是當你需要一個對象的時候，通常我們就是用 `new` 關鍵字 new 出來一個實例，但是這樣我們的代碼耦合度太高了，且不易於擴展，因此，我們要利用一個工廠來為我們生產出對象。

我們直接來看一個例子，在場景中理解簡單工廠模式。假設今天提供 3 種支付方法，Alipay, WeixinPay, UnionPay，所以首先就先想到設計一個 Pay 接口，而對於這個接口，我們提供兩個方法，支付指定價格 `pay` 以及一個向用戶提供反饋的通知方法 `inform`。接著就是寫 3 個類，AliPay, WeixinPay, UnionPay 分別都實現 Pay 接口的兩個方法就完成了。如此一來，當需要哪一種支付方式，就 new 相應的類就好了。但是，這樣的設計方式顯然免不了許多 if/else 來判斷要 new 哪一個，於是這時簡單工廠模式就派上用場了。

我們新建一個 PayFactory 類，並在裡面添加一個 getPayInstance 方法用於獲得相應的實例。這樣一來，我們只要 new 一個 PayFactory，就能獲得要求的類。來看看具體的代碼實現:
```java
package Factory;

public interface Pay {
    void pay(int value); // 支付接口
    void inform(); // 反饋接口
}
```
```java
package Factory;

public class WeixinPay implements Pay {
    private int WXbank = 0;
    private boolean success = true;

    @Override
    public void pay(int value) {
        if(value <= 0) {
            success = !success;
        }else {
            WXbank += value;
        }
    }

    @Override
    public void inform() {
        if(success)
            System.out.println("Weixin pay success!");
        else
            System.out.println("Weixin pay fail!");
    }
}
```
```java
package Factory;

public class UnionPay implements Pay {
    private int Unionbank = 0;
    private boolean success = true;

    @Override
    public void pay(int value) {
        if(value <= 0) {
            success = !success;
        }
        Unionbank += value;
    }

    @Override
    public void inform() {
        if(success)
            System.out.println("Alibaba pay success!");
        else
            System.out.println("Alibaba pay fail!");
    }
}
```
```java
package Factory;

public class AliPay implements Pay {
    private int Alibank = 0;
    private boolean success = true;

    @Override
    public void pay(int value) {
        if(value <= 0) {
            success = !success;
        }else {
            Alibank += value;
        }
    }

    @Override
    public void inform() {
        if(success)
            System.out.println("Alibaba pay success!");
        else
            System.out.println("Alibaba pay fail!");
    }
}
```
```java
package Factory;

// 工廠類

public class PayFactory {
    private static final String ALI_PAY = "Alibaba";
    private static final String WX_PAY = "Weixin";
    private static final String UNION_PAY = "Union";

    public Pay getPayInstance(String contributor) {
        Pay pay = null;
        if(contributor == ALI_PAY)
            pay = new AliPay();
        else if(contributor == WX_PAY)
            pay = new WeixinPay();
        else if(contributor == UNION_PAY)
            pay = new UnionPay();
        else
            System.out.println("Unsupport contributor!");

        return pay;
    }
}
```
```java
package Factory;

// 測試類

public class Main {
    public static void main(String[] args) {
        PayFactory payFactory = new PayFactory();
        Pay contributor = payFactory.getPayInstance("Alibaba");
        contributor.pay(100);
        contributor.inform();
    }
}

// 測試結果: Alibaba pay success!
```

## 工廠方法模式
![](https://wtfhhh.oss-cn-beijing.aliyuncs.com/factory-2.webp)
工廠方法模式簡單來說，就是定義一個創建產品對象的工廠接口，而將實際的創見工作推遲到子類中。核心工廠類不再負責產品對象的具體創建，而是將核心工廠類抽象成一個工廠角色，僅僅提供具體工廠子類的必須實現的接口。這樣進一步抽象的好處在於，使得工廠方法模式可以使系統在不修改具體工廠角色的情況下引進新的產品。接下來介紹工廠方法模式下的各個角色:

- 抽象工廠角色(Abstract Creator):是工廠方法模式的核心，任何在模式下創建的產品類的具體工廠類都必須實現該接口。

- 具體工廠角色(Concrete Creator):這是實現抽象工廠接口的具體工廠類，包含與具體應用密切相關的業務邏輯，並且受到應用程序調用進而提供創建好的對象實例給應用端。

- 抽象產品角色(Abstract Product):這是工廠方法模式中所創建對象的父類，也就是產品對象公共的接口。

- 具體產品角色(Concrete Product):這個角色負責實現抽象產品角色的接口，而某具體產品則由對應的具體工廠創建，往往是一一對應的。

來看一個例子。假設今天 Google, Apple, Microsoft 都推出了各自的打掃機器人，且每個機器人都會 `sweep`, `cook` 以及 `washDish`。運用工廠方法模式實現如下:
```java
// 抽象工廠角色

package FactoryMethod;

public interface AbstractFactory {
    CleanRobot createCleanRobot();
}
```

```java
// 具體工廠角色

package FactoryMethod;

public class ConcreteMicrosoftBotFactory implements AbstractFactory {
    public CleanRobot createCleanRobot() {
        return new MicrosoftBot();
    }
}

public class ConcreteGoogleBotFactory implements AbstractFactory {
    public CleanRobot createCleanRobot() {
        return new GoogleBot();
    }
}

public class ConcreteAppleBotFactory implements AbstractFactory {
    public CleanRobot createCleanRobot() {
        return new AppleBot();
    }
}
```

```java
// 抽象產品角色

package FactoryMethod;

public interface CleanRobot {
    void sweep(); // 掃地
    void washDish(); // 洗碗
    void cook(); // 煮飯
}
```

```java
// 具體產品角色

package FactoryMethod;

public class MicrosoftBot implements CleanRobot {
    @Override
    public void sweep() {
        System.out.println("Microsoft robot sweeping!");
    }

    @Override
    public void washDish() {
        System.out.println("Microsoft robot washing dishes!");
    }

    @Override
    public void cook() {
        System.out.println("Microsoft robot cooking!");
    }
}

public class GoogleBot implements CleanRobot {
    @Override
    public void sweep() {
        System.out.println("Google robot sweeping!");
    }

    @Override
    public void washDish() {
        System.out.println("Google robot washing dishes!");
    }

    @Override
    public void cook() {
        System.out.println("Google robot cooking!");
    }
}

public class AppleBot implements CleanRobot {
    @Override
    public void sweep() {
        System.out.println("Apple robot sweeping!");
    }

    @Override
    public void washDish() {
        System.out.println("Apple robot washing dishes!");
    }

    @Override
    public void cook() {
        System.out.println("Apple robot cooking!");
    }
}
```

```java
// 測試類

package FactoryMethod;

public class Main {
    public static void main(String[] args) {
        AbstractFactory factory = new ConcreteGoogleBotFactory();
        CleanRobot cleanRobot = factory.createCleanRobot();
        cleanRobot.sweep();
        cleanRobot.washDish();
        cleanRobot.cook();
    }
}

// 測試結果: 
// Google robot sweeping!
// Google robot washing dishes!
// Google robot cooking!
```

## 抽象工廠模式
![](https://wtfhhh.oss-cn-beijing.aliyuncs.com/factory-3.webp)
抽象工廠模式可以說是工廠模式的終極版，可以說是工廠方法模式的抽象。為了好理解抽象工廠模式，我們直接帶入例子理解。

世界各地都有自己的水果園，我們要把這些水果園抽象成一個水果接口。在中國，美國，英國都有自己的水果園，也都種植各類水果，也就是說，以香蕉來說，有美國香蕉，也有英國香蕉等等。運用抽象工廠模式，我們要在抽象工廠中聲明蘋果，香蕉等水果，而具體的工廠則是中國，英國，美國的水果園，各水果園負責生產蘋果，香蕉等。

抽象工廠模式中的相關概念如下:

- 產品等級結構:其實就是產品產品繼承結構，跟 java 中的類很像。例如，蘋果是一個抽象類，那中國，美國，英國的蘋果都是它的子類。

- 產品族:指的是同一個工廠的，位於不同產品等級結構的一組產品。例如蘋果，香蕉都產自美國，但它們不是同一個產品等級結構，所以共同構成一個產品族。

- 抽象工廠:是抽象工廠模式的核心，用接口實現。包含對多個產品等級結構的聲明，任何具體工廠類都必須實現這些接口。

- 具體工廠:是抽象工廠的實現，負責實例化某個產品族中的產品對象，例如中國的蘋果，香蕉等。
![](https://wtfhhh.oss-cn-beijing.aliyuncs.com/factory-2.png)

具體代碼實現的示意圖如下:
![](https://wtfhhh.oss-cn-beijing.aliyuncs.com/factory-3.png)

```java
package AbstractFactory;

// Fruit 接口
public interface Fruit {
    public void get();
}
```

```java
package AbstractFactory;

// Apple 抽象類
public abstract class Apple implements Fruit {
    public abstract void get();
}

// Banana 抽象類
package AbstractFactory;

public abstract class Banana implements Fruit {
    public abstract void get();
}
```

```java
package AbstractFactory;

// ChinaApple 具體類
public class ChinaApple extends Apple {
    @Override
    public void get() {
        System.out.println("China Made Apple!");
    }
}

// ChinaBanana 具體類
public class ChinaBanana extends Banana {
    @Override
    public void get() {
        System.out.println("China Made banana!");
    }
}

// UsaApple 具體類
public class UsaApple extends Apple {
    @Override
    public void get() {
        System.out.println("USA Made apple!");
    }
}

// UsaBanana 具體類
public class UsaBanana extends Banana {
    @Override
    public void get() {
        System.out.println("USA Made banana!");
    }
}
```

```java
package AbstractFactory;

// 抽象工廠接口
public interface FruitFactory {
    public Fruit getApple(); // 實例化蘋果
    public Fruit getBanana(); // 實例化香蕉
}
```

```java
// 具體工廠接口

package AbstractFactory;

// 中國工廠
public class ChinaFactory implements FruitFactory {
    @Override
    public Fruit getApple() {
        return new ChinaApple();
    }

    @Override
    public Fruit getBanana() {
        return new ChinaBanana();
    }
}

// 美國工廠
package AbstractFactory;

public class UsaFactory implements FruitFactory {
    @Override
    public Fruit getApple() {
        return new UsaApple();
    }

    @Override
    public Fruit getBanana() {
        return new UsaBanana();
    }
}
```

```java
// 測試類

package AbstractFactory;

public class Main {
    public static void main(String[] args) {
        FruitFactory chinaFactory = new ChinaFactory();
        Fruit chinaApple = chinaFactory.getApple();
        chinaApple.get();

        FruitFactory usaFactory = new UsaFactory();
        Fruit usaBanana = usaFactory.getBanana();
        usaBanana.get();
    }
}

// 測試結果:
// China Made apple!
// USA Made banana!
```



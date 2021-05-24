@[TOC](Java設計模式-單例模式)

# 什麼是單例模式?
單例模式就是說，確保某一個類只有一個實例，並且提供一個全局訪問點。單例模式總體來說就是 3 個特點:

- 只有一個實例 
- 自我實例化
- 提供全局訪問點

UML 圖如下，應該很清楚的:
![](https://wtfhhh.oss-cn-beijing.aliyuncs.com/singleton-1.webp)

- 優點
> 由於只有一個實例，所以能夠節省系統資源，減少性能開銷，提供效率等等，同時也能夠嚴格限制用戶對它的訪問。
- 缺點
> 也正因為只有一個實例，也就可能導致單例的職責過重，同時也因為沒有抽象的概念，在擴展上可能也會更困難。

# 四種實現
關於單例模式有 4 種實現方式，接下來一一來看!

## 餓漢式
```java
package Hungry;

public class Singleton {
    private static Singleton singleton = new Singleton();

    private Singleton() {}

    public static Singleton getInstance() {
        return singleton;
    }
}
```
餓漢式的缺點: 線程安全，調用效率高，但是無法延時加載。

餓漢式一開始就會先把實例 new 好，要用的時候直接返回即可。但是這樣問題也十分明顯，單例還沒使用到時就已經被初始化了，也就是說，即使程序從頭到尾都沒有使用到這個單例，單例還是會被創建，造成資源浪費，比較不推薦這種方式。

## 懶漢式
```java
package Lazy;

public class Singleton {
    private static Singleton singleton = null;

    private Singleton() {};

    public static Singleton getInstance() {
        if(singleton == null)
            singleton = new Singleton();

        return singleton;
    }

}
```
上面其實就是單例大致的概念以及實現，但是，這樣的代碼是有瑕疵的，這樣的單例是線程不安全的。來看看測試:
```java
package Lazy;

public class TestLazy extends Thread {
    @Override
    public void run() {
        Singleton lazySingleton = Singleton.getInstance();
    }
}

// 添加於 Singleton class 中
public static void main(String[] args) {
    for(int i = 1; i < 2; i++) {
        Thread thread = new TestLazy();
        thread.start();
    }
}
```
來看看 debug 的過程以及結果:
![](https://wtfhhh.oss-cn-beijing.aliyuncs.com/debug.gif)

可以看到，當第一個線程執行驗證 singleton 是否為 null，準備 new 一個新對象時，第二個線程也驗證完，這時兩個線程都會創建單例，也就表示線程是不安全的。

解決線程不安全的方法有兩種，但大體是差不多的，如下:

### Synchronized 實例化加鎖
```java
package Lazy;

public class SynchronizedSingleton {
    private static SynchronizedSingleton singleton = null;

    private SynchronizedSingleton() {};

    public static synchronized SynchronizedSingleton getInstance() {
        if(singleton == null)
            singleton = new SynchronizedSingleton();

        return singleton;
    }
}
```
加鎖的方法可以達到線程安全，不過對 getInstance 方法加了 `static` 和 `synchronized` 修飾關鍵詞，可能會對性能有影響。

### Double Check + Volatile 雙重檢測
```java
package Lazy;

public class DoubleCheckSingleton {
    private volatile static DoubleCheckSingleton singleton = null;

    private DoubleCheckSingleton() {};

    public DoubleCheckSingleton getInstance() {
        if(singleton == null) {
            synchronized (DoubleCheckSingleton.class) {
                if(singleton == null)
                    singleton = new DoubleCheckSingleton();
            }
        }

        return singleton;
    }
}
```
這個方法可以說是很完整的，不僅克服了 JVM 相關問題(這部分個人還不是很懂)，也完全的保證的線程安全了。關於 Java 的 `volatile` 關鍵字可以參考 [Java中Volatile关键字详解](https://www.cnblogs.com/aimei/p/12192943.html)。

## 靜態內部類式
```java
package StaticInner;

public class Singleton {
    private Singleton() {};

    private static class StaticInnerSingleton {
        private static Singleton singleton = new Singleton();
    }

    public static Singleton getInstance() {
        return StaticInnerSingleton.singleton;
    }
}
```
使用靜態內部類的優點是，外部類加載時並不需要立即加載內部類，而內部類不加載就不創建實例，也就不占內存。也就是說，當 Singleton 第一次被創建時並不需要加載 StaticInnerSingleton，而是等到調用了 getInstance 方法時才會創建單例。

這樣做不僅可以保證單例的唯一性，又可以提高，還盡可能的推遲了創建實例。

## 枚舉單例
```java
package Enum;

public enum Singleton {
    singleton;

    public void SingletonOperations() {
        /* 添加操作 */
    }
}
```
因為枚舉本身就是單例，所以可以這麼實現。關於 Java 的枚舉可以參考 [Java枚举](https://www.cnblogs.com/ldq2016/p/6627542.html)，非常清楚。

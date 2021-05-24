@[TOC](Java設計模式-狀態模式)

# 模式動機
在很多情況下，一個對象的行為取決於一個或多個動態變化的屬性，這樣的屬性稱為 “狀態”，這樣的對象叫做有狀態(stateful)的對象。這樣的對象狀態是從一系列預先定義好的值中取出來的，而當這樣的一個對象跟外部產生互動時，其內部狀態就會改變，從而使系統的行為也發生變化。

# 模式定義
不管怎樣，我們還是都要來看看狀態模式的定義:
> 狀態模式允許其內部狀態改變時改變對象本身的行為，對象看起來好像是修改了他的類。

# 模式結構與分析
狀態模式包含如下角色:

- Context: 環境類，上下文
- State: 抽象狀態類
- ConcreteState: 具體狀態類

![](https://wtfhhh.oss-cn-beijing.aliyuncs.com/state-3.png)

分析一下，狀態模式描述了對象的變化以及對象如何在每一種狀態下表現出不同的行為。

狀態模式的關鍵在於引入了一個抽象類來表示對象的狀態，叫做抽象狀態類，而對象的每一種狀態都繼承這個抽象狀態類，並實現不同狀態下的行為，包括狀態之間的轉換。

一個要重點理解的點是環境類與抽象狀態類的作用。

- 環境類實際上是擁有狀態的對象，有時候甚至可以充當狀態管理者的角色(State Manager)，在環境類中對狀態進行切換。
- 抽象狀態類可以是抽象類，也可以是接口，不同的具體狀態類就是繼承這個父類的不同子類，狀態類的產生是由於環境類存在多個狀態，同時還滿足兩個條件：這些狀態經常需要切換，在不同的狀態下對象的行為不同。因此可以將不同狀態下的行為單獨提取出來封裝在具體的狀態類中，使得環境對象在其內部狀態改變時可以改變它的行為。對象看起來似乎修改了它的類，而實際上是由於切換到不同的具體狀態類實現的。由於環境類可以設置為任一具體狀態類，因此它針對抽象狀態類進行設計，在運行時可以將任一具體狀態類的對象設置到環境類中，從而使得環境類可以通過改變內部狀態，來改變行為。

# 場景代碼實現
用一個簡單的場景來模擬，假設有一個酒店管理系統，對於房間有三個狀態，空閒，已預訂和已入住，這些狀態之間會常常需要切換，這時候就可以考慮使用狀態模式。

```java
package state;

/* 環境類 */

public class Context {
    private State state;

    public Context() {
        this.state = new EmptyState();
        System.out.println("初始房間狀態為空閒");
    }

    public void changeState(State state) {
        System.out.println("房間狀態被更改");
        this.state = state;
    }

    public void request() {
        System.out.print("當前房間狀態下行為 : ");
        this.state.handle();
    }
}
```
```java
package state;

/* 抽象狀態類 */

public abstract class State {
    protected Context context;

    public void setContext(Context context) {
        this.context = context;
    }

    public abstract void handle();
}
```
```java
package state;

/* 具體狀態類 */

public class EmptyState extends State {
    @Override
    public void handle() {
        System.out.println("房間空閒");
    }
}
```
```java
package state;

/* 具體狀態類 */

public class OrderedState extends State {
    @Override
    public void handle() {
        System.out.println("房間已被預訂");
    }
}
```
```java
package state;

/* 具體狀態類 */

public class LivedState extends State {
    @Override
    public void handle() {
        System.out.println("房間已入住");
    }
}
```
```java
package state;

/* 測試類 */

public class Main {
    public static void main(String[] args) {
        Context ctx = new Context();
        ctx.request();
        ctx.changeState(new OrderedState());
        ctx.request();
        ctx.changeState(new LivedState());
        ctx.request();
    }
}

// 初始房間狀態為空閒
// 當前房間狀態下行為 : 房間空閒
// 房間狀態被更改
// 當前房間狀態下行為 : 房間已被預訂
// 房間狀態被更改
// 當前房間狀態下行為 : 房間已入住
```
可以看到，我們通過切換狀態，來做到不同狀態下不一樣行為的展現。

# 優缺點
最後來分析一下狀態模式的優缺點。

- 優點

1. 封裝了狀態轉換規則
2. 將所有與特定狀態有關的行為單獨封裝於一個類中，並且可以很方便的增加狀態
3. 只需改變對象狀態即可改變對象行為
4. 向客戶端隱藏了狀態轉換邏輯，避免過多 `if/else`

- 缺點
1. 狀態模式必然會增加系統中類的數量
2. 狀態模式如果不當使用，很容易使系統變得難以維護
3. 狀態模式對於 “開閉原則” 不太友善，增加新的狀態需要修改狀態轉換部分的原代碼(除非本來就沒有隱藏的狀態轉換邏輯)


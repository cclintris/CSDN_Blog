@[TOC](Java設計模式-建造者模式)

# 動機
既然本篇的主角的設計模式叫做建造者模式，想必大家都會想到建築工人。比如，在建造一棟茅草屋，或是建造一棟水泥房時，其實過程都是一樣的，只是也許用到的材料不同。比如第一步都是要先打好地基(茅草屋用工具，水泥屋用機器)，第二步可能是要建立好框架(茅草屋用樹木，水泥屋用混凝土)，第三步才開始建造房屋(茅草屋用茅草，水泥屋用水泥)。

可以看到，其實過程都是一樣的，只是材料不同罷了。軟件系統常常也是這樣，因此我們就希望將過程跟材料解耦，將這些複雜材料的生產分離出來(`建造者`)，而將流程獨立出來為`指揮者`。

# 什麼是建造者模式?

- 定義

將一個複雜對象的 <strong>構建</strong> 與它的 <strong>表示</strong> 分離開來，使得同樣的過程可以產生不同的產品。

- 主要作用

用戶可以在不了解對象的構建過程和細節的情況下就直接創建出複雜對象。

- 使用方式

用戶只需給出複雜對象的 <strong>類型</strong> 和 <strong>內容</strong> 就好，建造者模式就會負責創建好對象。

- 比較

可能你會覺得這與工廠模式很像，其實差別在於，建造者模式更加關注於零件裝配的順序，一般來說用於創建更複雜的對象。

# 具體實現
建造者模式通常是通過 Director, Builder, Product 形成的。

接下來就來看看怎麼實現吧!

## 實現 (Director + Builder +  Product)

一般來說，這種實現方式會有以下角色:

- 抽象建造者(AbstractBuilder) : 描述具體建造者的公共接口，一般用來定義建造細節的方法，並不涉及具體對象部件的創建。

- 具體建造者(ConcreteBuilder) : 具體建造者，需要實現 AbstractBuilder 所提供的所有接口。

- 指揮者(Director) : 負責調用具體建造者來創建複雜對象(產品)的各個部件，並按照 <strong>一定順序</strong> 來建造該複雜對象。

- 產品(Product) : 由一系列部件組成的複雜對象。

在代碼實現之前，我們先帶入一個場景，也好讓代碼實現該場景需求。

既然講的是建造者模式，就來造房子吧!我們把造房子簡單分為以下幾步:

1. 打地基 
2. 鋼筋工程
3. 舖設電線
4. 粉刷

而運用建造者模式的思路就是，每當我們想造一棟房子，就需要找來一個建築公司或是工程承包商(Director)，指揮工人(ConcreteBuilder)來建造房子(Product)，最後要交付完成的房子。

說了這麼多，直接看看代碼實現吧!

```java
package classic;

/*
 * 抽象建造者
 * */

public abstract class Builder {

    // 打地基
    public abstract void foundation();

    // 鋼筋工程
    public abstract void concreteIron();

    // 鋪設電線
    public abstract void wire();

    // 粉刷
    public abstract void paint();

    // 完成 - 交付產品
    public abstract Product getProduct();
}
```
```java
package classic;

/*
 * 產品
 * */

public class Product {
    private String foundation;
    private String concreteIron;
    private String wire;
    private String paint;

    public String getFoundation() {
        return foundation;
    }

    public void setFoundation(String foundation) {
        this.foundation = foundation;
    }

    public String getConcreteIron() {
        return concreteIron;
    }

    public void setConcreteIron(String concreteIron) {
        this.concreteIron = concreteIron;
    }

    public String getWire() {
        return wire;
    }

    public void setWire(String wire) {
        this.wire = wire;
    }

    public String getPaint() {
        return paint;
    }

    public void setPaint(String paint) {
        this.paint = paint;
    }

    @Override
    public String toString() {
        return this.foundation + "\n" + this.concreteIron + "\n" + this.wire + "\n" + this.paint + "\n" + "房屋建造完成\n";
    }
}
```
```java
package classic;

/*
* 具體建造者: 茅草屋工人
* */

public class GrasshouseBuilder extends Builder {
    private Product grassHouse;
    public GrasshouseBuilder() {
        grassHouse = new Product();
    }

    @Override
    public void foundation() {
        grassHouse.setFoundation("用茅草打地基");
    }

    @Override
    public void concreteIron() {
        grassHouse.setConcreteIron("用樹木搭建鋼筋工程");
    }

    @Override
    public void wire() {
        grassHouse.setWire("用火搭建電力系統");
    }

    @Override
    public void paint() {
        grassHouse.setPaint("用草藥水粉刷");
    }

    @Override
    public Product getProduct() {
        return grassHouse;
    }
}
```
```java
package classic;

/*
 * 具體建造者: 水泥屋工人
 * */

public class ConcretehouseBuilder extends Builder {
    private Product concreteHouse;
    public ConcretehouseBuilder() {
        concreteHouse = new Product();
    }

    @Override
    public void foundation() {
        concreteHouse.setFoundation("用機器打地基");
    }

    @Override
    public void concreteIron() {
        concreteHouse.setConcreteIron("用鋼鐵搭建鋼筋工程");
    }

    @Override
    public void wire() {
        concreteHouse.setWire("用電建電力系統");
    }

    @Override
    public void paint() {
        concreteHouse.setPaint("用油漆粉刷");
    }

    @Override
    public Product getProduct() {
        return concreteHouse;
    }
}
```
```java
package classic;

/*
 * 指揮者
 * */

public class Director {
    // 指揮工人按順序造房
    public Product command(Builder builder) {
        builder.foundation();
        builder.concreteIron();
        builder.wire();
        builder.paint();
        return builder.getProduct();
    }
}
```
```java
package classic;

/*
 * 測試類
 * */

public class Main {
    public static void main(String[] args) {
        Director director1 = new Director();
        Director director2 = new Director();

        Builder grassHouseBuilder = new GrasshouseBuilder();
        Builder concreteHouseBuilder = new ConcretehouseBuilder();

        Product grassHouse = director1.command(grassHouseBuilder);
        Product concreteHouse = director2.command(concreteHouseBuilder);

        System.out.println(grassHouse.toString());
        System.out.println(concreteHouse.toString());
    }
}
```
測試類運行結果如下:
![](https://wtfhhh.oss-cn-beijing.aliyuncs.com/builder.png)

分析一下就可以發現，即便茅草屋和水泥屋的過程使用的都是不一樣的材料，但是只要他們都擁有一樣的構建過程，就都可以套用在這樣的建造者模式上。(輸出可能有些莫名其妙，不過不是重點啦!hhh)

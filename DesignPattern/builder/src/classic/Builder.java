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

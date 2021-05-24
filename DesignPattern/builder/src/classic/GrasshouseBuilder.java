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

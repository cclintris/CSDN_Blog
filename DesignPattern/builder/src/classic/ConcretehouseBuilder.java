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

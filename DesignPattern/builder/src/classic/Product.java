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


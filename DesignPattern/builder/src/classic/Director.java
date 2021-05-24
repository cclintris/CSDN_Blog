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

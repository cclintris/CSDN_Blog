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

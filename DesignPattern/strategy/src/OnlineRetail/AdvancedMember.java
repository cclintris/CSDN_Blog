package OnlineRetail;

public class AdvancedMember implements DiscountStrategy {
    @Override
    public double calcPrice(double bookPrice) {
        System.out.println("高級會員打 8 折");
        return bookPrice * 0.8;
    }
}

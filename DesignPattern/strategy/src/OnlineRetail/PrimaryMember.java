package OnlineRetail;

public class PrimaryMember implements DiscountStrategy {
    @Override
    public double calcPrice(double bookPrice) {
        System.out.println("初級會員打 95 折");
        return bookPrice * 0.95;
    }
}

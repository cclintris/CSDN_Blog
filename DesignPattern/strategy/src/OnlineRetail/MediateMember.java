package OnlineRetail;

public class MediateMember implements DiscountStrategy {
    @Override
    public double calcPrice(double bookPrice) {
        System.out.println("中級會員打 9 折");
        return bookPrice * 0.9;
    }
}

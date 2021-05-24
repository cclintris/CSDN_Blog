package OnlineRetail;

public class Customer {
    public static void main(String[] args) {
        DiscountStrategy strategy = new MediateMember();
        Price price = new Price(strategy);
        double bookValue = 500;
        System.out.println("打折前的價格: " + bookValue);
        double discountPrice = price.quote(bookValue);
        System.out.println("打完折後價格: " + discountPrice);
    }
}

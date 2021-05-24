package Factory;

public class Main {
    public static void main(String[] args) {
        PayFactory payFactory = new PayFactory();
        Pay contributor = payFactory.getPayInstance("Alibaba");
        contributor.pay(100);
        contributor.inform();
    }
}

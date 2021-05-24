package Factory;

public class UnionPay implements Pay {
    private int Unionbank = 0;
    private boolean success = true;

    @Override
    public void pay(int value) {
        if(value <= 0) {
            success = !success;
        }
        Unionbank += value;
    }

    @Override
    public void inform() {
        if(success)
            System.out.println("Alibaba pay success!");
        else
            System.out.println("Alibaba pay fail!");
    }
}

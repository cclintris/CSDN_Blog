package Factory;

public class AliPay implements Pay {
    private int Alibank = 0;
    private boolean success = true;

    @Override
    public void pay(int value) {
        if(value <= 0) {
            success = !success;
        }else {
            Alibank += value;
        }
    }

    @Override
    public void inform() {
        if(success)
            System.out.println("Alibaba pay success!");
        else
            System.out.println("Alibaba pay fail!");
    }
}

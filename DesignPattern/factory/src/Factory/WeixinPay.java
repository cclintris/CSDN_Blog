package Factory;

public class WeixinPay implements Pay {
    private int WXbank = 0;
    private boolean success = true;

    @Override
    public void pay(int value) {
        if(value <= 0) {
            success = !success;
        }else {
            WXbank += value;
        }
    }

    @Override
    public void inform() {
        if(success)
            System.out.println("Weixin pay success!");
        else
            System.out.println("Weixin pay fail!");
    }
}

package Factory;

public class PayFactory {
    private static final String ALI_PAY = "Alibaba";
    private static final String WX_PAY = "Weixin";
    private static final String UNION_PAY = "Union";

    public Pay getPayInstance(String contributor) {
        Pay pay = null;
        if(contributor == ALI_PAY)
            pay = new AliPay();
        else if(contributor == WX_PAY)
            pay = new WeixinPay();
        else if(contributor == UNION_PAY)
            pay = new UnionPay();
        else
            System.out.println("Unsupport contributor!");

        return pay;
    }
}


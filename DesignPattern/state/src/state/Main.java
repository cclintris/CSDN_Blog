package state;

public class Main {
    public static void main(String[] args) {
        Context ctx = new Context();
        ctx.request();
        ctx.changeState(new OrderedState());
        ctx.request();
        ctx.changeState(new LivedState());
        ctx.request();
    }
}

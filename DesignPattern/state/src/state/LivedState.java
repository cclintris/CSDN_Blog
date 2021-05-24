package state;

public class LivedState extends State {
    @Override
    public void handle() {
        System.out.println("房間已入住");
    }
}

package state;

public class OrderedState extends State {
    @Override
    public void handle() {
        System.out.println("房間已被預訂");
    }
}

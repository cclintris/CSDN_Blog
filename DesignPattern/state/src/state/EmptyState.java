package state;

public class EmptyState extends State {
    @Override
    public void handle() {
        System.out.println("房間空閒");
    }
}

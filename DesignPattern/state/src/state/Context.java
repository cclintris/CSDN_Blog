package state;

public class Context {
    private State state;

    public Context() {
        this.state = new EmptyState();
        System.out.println("初始房間狀態為空閒");
    }

    public void changeState(State state) {
        System.out.println("房間狀態被更改");
        this.state = state;
    }

    public void request() {
        System.out.print("當前房間狀態下行為 : ");
        this.state.handle();
    }
}

package observer;

/*
 * 具體主題類
 * */

public class ConcreteSubject extends AbstractSubject {

    private String state;

    public ConcreteSubject(String state) {
        this.state = state;
    }

    public String getState() {
        return state;
    }

    // 主題狀態發生變化
    public void change(String newState) {
        this.state = newState;
        this.notify(newState);
    }
}

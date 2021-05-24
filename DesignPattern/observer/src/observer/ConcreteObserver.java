package observer;

/*
 * 具體觀察者類
 * */

public class ConcreteObserver extends AbstractObserver {

    // 觀察者名稱
    private String name;

    public ConcreteObserver(String name) {
        this.name = name;
    }

    @Override
    public void update(String newState) {
        System.out.println("觀察者 " + this.name + " 狀態被更新為 " + newState);
    }
}

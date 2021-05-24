package observer;

public class Main {
    public static void main(String[] args) {
        ConcreteSubject subject = new ConcreteSubject("initial state");

        ConcreteObserver observer1 = new ConcreteObserver("observer1");
        ConcreteObserver observer2 = new ConcreteObserver("observer2");
        ConcreteObserver observer3 = new ConcreteObserver("observer3");

        subject.add(observer1);
        subject.add(observer2);
        subject.add(observer3);

        System.out.println("主題角色狀態: " + subject.getState());

        subject.change("change state");

        System.out.println("主題角色狀態: " + subject.getState());
    }
}

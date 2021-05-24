package observer;

/*
* 抽象主題類
* */

import java.util.ArrayList;
import java.util.List;

public abstract class AbstractSubject {

    // 觀察者集合
    List<AbstractObserver> observers = new ArrayList<AbstractObserver>();

    // 添加觀察者
    public void add(AbstractObserver observer) {
        observers.add(observer);

    }

    // 移除觀察者
    public void remove(AbstractObserver observer) {
        observers.remove(observer);
    }

    // 通知所有註冊的觀察者
    public void notify(String newState) {
        for(AbstractObserver observer : observers) {
            observer.update(newState);
        }
    }
}

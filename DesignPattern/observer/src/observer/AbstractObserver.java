package observer;

/*
* 抽象觀察者類
* */

public abstract class AbstractObserver {

    // 更新觀察者的狀態
    public abstract void update(String newState);
}

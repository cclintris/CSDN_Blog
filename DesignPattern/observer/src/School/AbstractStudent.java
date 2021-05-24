package School;

/*
 * 抽象觀察者類
 * */

public abstract class AbstractStudent {

    // 學生完成作業，相當於更新作業狀態
    public abstract void finish(String homework);
}

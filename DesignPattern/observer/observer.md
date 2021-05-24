@[TOC](Java設計模式-觀察者模式)

# 什麼是觀察者模式?
首先先來看看一個對於觀察者模式一個經典的描述，出自於阎宏博士的 《JAVA與模式》:
> 观察者模式是对象的行为模式，又叫发布-订阅(Publish/Subscribe)模式、模型-视图(Model/View)模式、源-监听器(Source/Listener)模式或从属者(Dependents)模式。
观察者模式定义了一种一对多的依赖关系，让多个观察者对象同时监听某一个主题对象。这个主题对象在状态上发生变化时，会通知所有观察者对象，使它们能够自动更新自己。

可能單看上面一段話有點抽象，接下來再用更實際的描述理解觀察者模式。

# 觀察者模式的結構
一個軟件系統中存在各種對象，且常常要求在某一個對象狀態發生變化時，某些其他可能與之關聯的對象要做出相應的改變。觀察者模式就正是為了這樣需求而誕生的設計模式，來看看觀察者模式的架構圖:
![](https://wtfhhh.oss-cn-beijing.aliyuncs.com/observe-1.png)

觀察者模式涉及的角色如下:
- 抽象主題角色(Abstract Subject): 抽象主題角色把所有對觀察者對象的引用都保存到 `observers:ArrayList` 中，每個主題都可以有任意個觀察者。抽象主題提供一個接口，可以增加 `attach` 或刪除 `detach` 觀察者對象，因此抽象主題角色又叫做抽象被觀察者(Abstract Observable)角色。

- 具體主題角色(Concrete Subject): 將有關狀態存入具體觀察者對象，在具體主題的內部狀態發生改變時，給所有登記過觀察者發出通知，因此具體主題角色又叫做具體被觀察者(Concrete Observable)角色。

- 抽象觀察者角色(Abstract Observer): 為所有具體觀察者提供一個接口，在得到主題的通知時更新自己 `update`。

- 具體觀察者角色(Concrete Observer): 存儲與主題的狀態自恰的狀態。具體觀察者角色要實現所有抽象觀察者提供的所有接口，以便使自己的狀態跟主題的狀態互相協調。

接下來來看看觀察者模式的原代碼!

# 代碼實現
```java
package observer;

/*
* 抽象觀察者類
* */

public abstract class AbstractObserver {

    // 更新觀察者的狀態
    public abstract void update(String newState);
}
```
```java
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
```
```java
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
```
```java
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
```
```java
package observer;

/*
 * 測試類
 * */

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
```
測試類運行結果如下:
![](https://wtfhhh.oss-cn-beijing.aliyuncs.com/observe-2.png)

# 實際案例
最後通過一個實際案例場景，運用觀察者模式滿足需求。

假設今天要模擬一個學校班級，首先會有老師跟學生。老師會自己知道作業狀態，且會布置作業給學生。而一旦收到老師布置的作業，學生便會開始完成作業。來看看用觀察者模式的代碼實現:

```java
package School;

/*
 * 抽象觀察者類
 * */

public abstract class AbstractStudent {

    // 學生完成作業，相當於更新作業狀態
    public abstract void finish(String homework);
}
```

```java
package School;

/*
 * 抽象主題類
 * */

import java.util.ArrayList;
import java.util.List;

public abstract class AbstractTeacher {

    // 老師管理的學生集合
    private List<AbstractStudent> students = new ArrayList<AbstractStudent>();

    // 增加學生
    public void add(AbstractStudent student) {
        students.add(student);
    }

    // 移除學生
    public void remove(AbstractStudent student) {
        students.remove(student);
    }

    // 老師布置作業
    public void assignHomework(String homework) {
        for(AbstractStudent student : students) {
            student.finish(homework);
        }
    }
}
```

```java
package School;

/*
 * 具體觀察者類
 * */

public class ConcreteStudent extends AbstractStudent {

    String name;
    int number;

    public ConcreteStudent(String name, int number) {
        this.name = name;
        this.number = number;
    }

    @Override
    public void finish(String homework) {
        System.out.println("學號 " + this.number + " " + this.name + " 同學正在完成作業 " + homework);
    }
}
```

```java
package School;

/*
 * 具體主題類
 * */

public class ConcreteTeacher extends AbstractTeacher {

    private String homework = "目前沒有作業";

    public String getHomework() {
        return this.homework;
    }

    public void notifyStudents(String homework) { // 老師通知學生
        this.homework = homework;
        this.assignHomework(homework);
    }
}
```

```java
package School;

/*
 * 測試類
 * */

public class Main {
    public static void main(String[] args) {
        ConcreteTeacher teacher = new ConcreteTeacher();

        ConcreteStudent student1 = new ConcreteStudent("Tristan", 181250083);
        ConcreteStudent student2 = new ConcreteStudent("William", 181250168);
        ConcreteStudent student3 = new ConcreteStudent("cyzPaul", 181250015);
        ConcreteStudent student4 = new ConcreteStudent("Donovan", 181250003);

        teacher.add(student1);
        teacher.add(student2);
        teacher.add(student3);
        teacher.add(student4);

        System.out.println("目前作業狀態 : " + teacher.getHomework());

        teacher.notifyStudents("homework1");

        System.out.println("目前作業狀態 : " + teacher.getHomework());
    }
}
```
測試類運行結果如下:
![](https://wtfhhh.oss-cn-beijing.aliyuncs.com/observe-3.png)

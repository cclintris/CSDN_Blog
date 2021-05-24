@[TOC](Java設計模式-命令模式)

# 模式概述
在軟件系統中，系統中，“行為請求者” 與 “行為實現者” 往往呈現一種緊耦合的關係，但是在某些場合，比如希望對事務做撤銷，或是重做等操作，那麼這種緊耦合的結構是不合適的。

命令模式就是為了因應這種需求，將 “行為請求者” 與 “行為實現者” 解耦，將一組行為抽象成對象，實現二者之間的松耦合。

# 模式結構
命令模式下通常會包含以下角色:

|角色|職責|
|:-----|:-----|
|抽象命令類(AbstractCommand)|定義命令的接口，執行的方法|
|具體命令類(ConcreteCommand)|命令接口實現對象，通常會持有一個接收者的引用，並調用接收者的功能來完成要執行的操作|
|接收者(Receiver)|真正執行命令的對象，只要能夠實現命令要求實現的相應功能，任何類都可能成為接收者|
|調用者(Invoker)|要求命令對象執行請求，通常會持有一個或多個命令對象。Invoker 是客戶端真正觸發命令並要求命令執行相應操作的地方，也就是說相當於使用命令對象的入口|
|客戶端(Client)|創建具體的命令對象，並且設置命令對象的接收者。這邊要釐清一下，這邊說的客戶端並不是我們日常開發中所指的客戶端，而是在組裝命令對象和接收者。其實應該要把這個『客戶端』理解為裝配者，因為使用命令的客戶端是從 Invoker 觸發執行的。|

- UML示意圖

![](https://ftp.bmp.ovh/imgs/2021/04/c1203790b6ba831a.png)

# 模式適用場景
這邊大致整理了命令模式適用的場景：

1. 系統希望將請求調用者和請求接收者解耦，使調用者和接收者不直接交互
2. 系統需要將一組操作封裝，組合在一起，相當於提供宏命令
3. 系統需要支持命令的 undo(撤銷) redo(重做)

# 模式具體應用
> 現在假設這麼一個場景，我們要設計一個電燈遙控器，該遙控器可以遠程 打開/關閉 電燈。

應用命令模式，我們可以畫出如下 UML 圖：

![](https://ftp.bmp.ovh/imgs/2021/04/2d570e29dfb131db.png)

接下來當然就來看看具體代碼實現!

- 抽象命令類
```java
package command;

/*
 * Abstract Command
 * */

public interface Command {
    public void execute();
}
```
- 接收者
```java
package command;

/*
* Receiver
* */

public class Light {
    public void on() {
        System.out.println("Light is on!");
    }

    public void off() {
        System.out.println("Light is off!");
    }
}
```

- 具體命令類 1
```java
package command;

/*
 * Concrete Command
 * */

public class LightOnCommand implements Command {
    Light light;

    public LightOnCommand(Light light) {
        this.light = light;
    }

    @Override
    public void execute() {
        this.light.on();
    }
}
```

- 具體命令類 2
```java
package command;

/*
 * Concrete Command
 * */

public class LightOffCommand implements Command {
    Light light;

    public LightOffCommand(Light light) {
        this.light = light;
    }

    @Override
    public void execute() {
        this.light.off();
    }
}
```

- 調用者
```java
package command;

/*
 * Invoker
 * */

public class RemoteControl {
    Command onCommand;
    Command offCommand;

    public void setOnCommand(Command onCommand) {
        this.onCommand = onCommand;
    }

    public void setOffCommand(Command offCommand) {
        this.offCommand = offCommand;
    }

    public void onButtonPushed() {
        onCommand.execute();
    }

    public void offButtonPushed() {
        offCommand.execute();
    }
}
```

- 客戶端
```java
package command;

/*
* Client
* */

public class RemoteLoader {
    public static void main(String[] args) {
        // 定義遠程遙控器
        RemoteControl remoteControl = new RemoteControl();
        // 定義電燈
        Light light = new Light();

        // 定義操作電燈的命令對象
        Command onCommand = new LightOnCommand(light);
        Command offCommand = new LightOffCommand(light);

        remoteControl.setOnCommand(onCommand);
        remoteControl.setOffCommand(offCommand);

        remoteControl.onButtonPushed();
        remoteControl.offButtonPushed();
    }
}

// Light is on!
// Light is off!
```


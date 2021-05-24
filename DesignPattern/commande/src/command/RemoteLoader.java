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

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

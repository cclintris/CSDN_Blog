package FactoryMethod;

public class ConcreteAppleBotFactory implements AbstractFactory {
    public CleanRobot createCleanRobot() {
        return new AppleBot();
    }
}

package FactoryMethod;

public class ConcreteGoogleBotFactory implements AbstractFactory {
    public CleanRobot createCleanRobot() {
        return new GoogleBot();
    }
}

package FactoryMethod;

public class ConcreteMicrosoftBotFactory implements AbstractFactory {
    public CleanRobot createCleanRobot() {
        return new MicrosoftBot();
    }
}

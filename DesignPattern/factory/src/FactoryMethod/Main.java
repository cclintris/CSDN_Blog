package FactoryMethod;

public class Main {
    public static void main(String[] args) {
        AbstractFactory factory = new ConcreteGoogleBotFactory();
        CleanRobot cleanRobot = factory.createCleanRobot();
        cleanRobot.sweep();
        cleanRobot.washDish();
        cleanRobot.cook();
    }
}

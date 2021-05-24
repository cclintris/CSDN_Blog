package FactoryMethod;

public class MicrosoftBot implements CleanRobot {
    @Override
    public void sweep() {
        System.out.println("Microsoft robot sweeping!");
    }

    @Override
    public void washDish() {
        System.out.println("Microsoft robot washing dishes!");
    }

    @Override
    public void cook() {
        System.out.println("Microsoft robot cooking!");
    }
}

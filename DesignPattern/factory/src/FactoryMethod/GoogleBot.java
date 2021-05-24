package FactoryMethod;

public class GoogleBot implements CleanRobot {
    @Override
    public void sweep() {
        System.out.println("Google robot sweeping!");
    }

    @Override
    public void washDish() {
        System.out.println("Google robot washing dishes!");
    }

    @Override
    public void cook() {
        System.out.println("Google robot cooking!");
    }
}

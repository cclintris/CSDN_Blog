package FactoryMethod;

public class AppleBot implements CleanRobot {
    @Override
    public void sweep() {
        System.out.println("Apple robot sweeping!");
    }

    @Override
    public void washDish() {
        System.out.println("Apple robot washing dishes!");
    }

    @Override
    public void cook() {
        System.out.println("Apple robot cooking!");
    }
}

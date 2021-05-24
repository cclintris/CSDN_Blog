package Lazy;

public class Singleton {
    private static Singleton singleton = null;

    private Singleton() {};

    public static Singleton getInstance() {
        if(singleton == null)
            singleton = new Singleton();

        return singleton;
    }

    public static void main(String[] args) {
        for(int i = 1; i < 2; i++) {
            Thread thread = new TestLazy();
            thread.start();
        }
    }
}

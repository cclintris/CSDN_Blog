package StaticInner;

public class Singleton {
    private Singleton() {};

    private static class StaticInnerSingleton {
        private static Singleton singleton = new Singleton();
    }

    public static Singleton getInstance() {
        return StaticInnerSingleton.singleton;
    }
}

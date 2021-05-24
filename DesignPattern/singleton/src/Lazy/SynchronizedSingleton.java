package Lazy;

public class SynchronizedSingleton {
    private static SynchronizedSingleton singleton = null;

    private SynchronizedSingleton() {};

    public static synchronized SynchronizedSingleton getInstance() {
        if(singleton == null)
            singleton = new SynchronizedSingleton();

        return singleton;
    }
}

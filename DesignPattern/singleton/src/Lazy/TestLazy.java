package Lazy;

public class TestLazy extends Thread {
    @Override
    public void run() {
        Singleton lazySingleton = Singleton.getInstance();
    }
}

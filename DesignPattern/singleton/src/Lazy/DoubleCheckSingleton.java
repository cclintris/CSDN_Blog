package Lazy;

public class DoubleCheckSingleton {
    private volatile static DoubleCheckSingleton singleton = null;

    private DoubleCheckSingleton() {};

    public DoubleCheckSingleton getInstance() {
        if(singleton == null) {
            synchronized (DoubleCheckSingleton.class) {
                if(singleton == null)
                    singleton = new DoubleCheckSingleton();
            }
        }

        return singleton;
    }
}

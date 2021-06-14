package ClassAdapter;

// 類適配器類
public class ClassAdapter extends Adaptee implements Target {
    public void request() {
        specificRequest();
    }
}

// 外观角色
public class Facade {
    private Subsystem1 subsystem1 = new Subsystem1();
    private Subsystem2 subsystem2 = new Subsystem2();
    private Subsystem3 subsystem3 = new Subsystem3();

    public void method() {
        subsystem1.method1();
        subsystem2.method2();
        subsystem3.method3();
    }
}

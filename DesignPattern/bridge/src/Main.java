public class Main {
    public static void main(String[] args) {
        Implementor imple = new ConcreteImplementor();
        Abstraction abs = new RefinedAbstraction(imple);
        abs.Operation();
    }
}

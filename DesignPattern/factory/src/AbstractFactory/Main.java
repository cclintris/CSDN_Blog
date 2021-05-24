package AbstractFactory;

public class Main {
    public static void main(String[] args) {
        FruitFactory chinaFactory = new ChinaFactory();
        Fruit chinaApple = chinaFactory.getApple();
        chinaApple.get();

        FruitFactory usaFactory = new UsaFactory();
        Fruit usaBanana = usaFactory.getBanana();
        usaBanana.get();
    }
}

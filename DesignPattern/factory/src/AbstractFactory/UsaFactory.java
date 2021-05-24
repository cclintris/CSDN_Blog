package AbstractFactory;

public class UsaFactory implements FruitFactory {
    @Override
    public Fruit getApple() {
        return new UsaApple();
    }

    @Override
    public Fruit getBanana() {
        return new UsaBanana();
    }
}

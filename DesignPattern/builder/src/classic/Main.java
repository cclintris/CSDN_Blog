package classic;

/*
 * 測試類
 * */

public class Main {
    public static void main(String[] args) {
        Director director1 = new Director();
        Director director2 = new Director();

        Builder grassHouseBuilder = new GrasshouseBuilder();
        Builder concreteHouseBuilder = new ConcretehouseBuilder();

        Product grassHouse = director1.command(grassHouseBuilder);
        Product concreteHouse = director2.command(concreteHouseBuilder);

        System.out.println(grassHouse.toString());
        System.out.println(concreteHouse.toString());
    }
}

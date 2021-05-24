package School;

/*
 * 具體觀察者類
 * */

public class ConcreteStudent extends AbstractStudent {

    String name;
    int number;

    public ConcreteStudent(String name, int number) {
        this.name = name;
        this.number = number;
    }

    @Override
    public void finish(String homework) {
        System.out.println("學號 " + this.number + " " + this.name + " 同學正在完成作業 " + homework);
    }
}

package School;

public class Main {
    public static void main(String[] args) {
        ConcreteTeacher teacher = new ConcreteTeacher();

        ConcreteStudent student1 = new ConcreteStudent("Tristan", 181250083);
        ConcreteStudent student2 = new ConcreteStudent("William", 181250168);
        ConcreteStudent student3 = new ConcreteStudent("cyzPaul", 181250015);
        ConcreteStudent student4 = new ConcreteStudent("Donovan", 181250003);

        teacher.add(student1);
        teacher.add(student2);
        teacher.add(student3);
        teacher.add(student4);

        System.out.println("目前作業狀態 : " + teacher.getHomework());

        teacher.notifyStudents("homework1");

        System.out.println("目前作業狀態 : " + teacher.getHomework());
    }
}

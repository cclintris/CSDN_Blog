package School;

/*
 * 具體主題類
 * */

public class ConcreteTeacher extends AbstractTeacher {

    private String homework = "目前沒有作業";

    public String getHomework() {
        return this.homework;
    }

    public void notifyStudents(String homework) { // 老師通知學生
        this.homework = homework;
        this.assignHomework(homework);
    }
}

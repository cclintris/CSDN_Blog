package School;

/*
 * 抽象主題類
 * */

import java.util.ArrayList;
import java.util.List;

public abstract class AbstractTeacher {

    // 老師管理的學生集合
    private List<AbstractStudent> students = new ArrayList<AbstractStudent>();

    // 增加學生
    public void add(AbstractStudent student) {
        students.add(student);
    }

    // 移除學生
    public void remove(AbstractStudent student) {
        students.remove(student);
    }

    // 老師布置作業
    public void assignHomework(String homework) {
        for(AbstractStudent student : students) {
            student.finish(homework);
        }
    }
}

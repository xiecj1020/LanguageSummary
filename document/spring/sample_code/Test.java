import org.springframework.context.ApplicationContext;
import org.springframework.context.support.ClassPathXmlApplicationContext;

public class Test {
    public static void main(String[] args){
        Chinese chinese = new Chinese();
        //Chinese类和StoneAxe类耦合在了java代码中
        //如果现在需求有变，我们在这里要使用一种新的斧头，即Axe有一个新的实现类SteelAxe，
        // 那么就必须修改这里的代码Axe axe = new SteelAxe();这对于代码维护其实并不方便
        Axe axe = new StoneAxe();
        chinese.setAxe(axe);
        chinese.useAxe();
        //Spring的解决方案是将上面的步骤放在配置文件中,可以看到，在测试代码中，
        //完全没有了Chinese和StoneAxe两个类的耦合依赖关系，转而将依赖关系放在了上面的XML文件中，底层通过java反射的方式进行初始化
        //spring自动生成对象（可禁用）和设置依赖关系是最基本的功能。
        //Spring的IOC(或DI）机制，极大地降低了组件间的耦合，例如对于上面的需求变更，
        // 我们只需要在XML文件中新增一个bean节点，并设置好依赖关系即可，而测试代码不需要任何改变！
        //对于依赖注入，有两种方式，一种是设置注入，即上面XML配置文件中设置<property />标签的方式，
        // 这种方法本质上是利用反射调用对象的setter方式进行初始化。
        //另外，还可以使用构造注入的方式，即在XML配置文件中使用<constructor-arg />标签，不过这种方式要求有对应的构造方法才行。
        //对于上面的例子，首先我们要让Chinese类有一个构造方法，
        ApplicationContext ctx = new ClassPathXmlApplicationContext("beans.xml");
        Person p = ctx.getBean("chinese", Person.class);
        p.useAxe();
    }
}

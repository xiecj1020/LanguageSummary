import java.lang.reflect.InvocationHandler;
import java.lang.reflect.Proxy;

/**
 * 客户端测试类
 */
public class TestMyService {
    public static void main(String[] args) {
        //保存动态生成的字节码文件com.sun.proxy.$Proxy0.class
        System.setProperty("sun.misc.ProxyGenerator.saveGeneratedFiles", "true");
        InvocationHandler h = new TimeConsumeHandler(new MyServiceImpl());
        //myService指客户端的代理类$Proxy0，动态生成的
        IMyService myService = (IMyService) Proxy.newProxyInstance(TestMyService.class.getClassLoader(),
                MyServiceImpl.class.getInterfaces(), h);
        //客户端代理类调用服务具体接口
        myService.sayHello();
        System.out.println(myService.toString());
    }
}

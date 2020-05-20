/**
 * 服务端服务的具体实现类
 */
public class MyServiceImpl implements IMyService {
    @Override
    public void sayHello() {
        //休眠50毫秒，便于观察方法执行时间
        try {
            Thread.sleep(50);
        } catch (InterruptedException e) {
            e.printStackTrace();
        }
        System.out.println("Hello world");
    }
}

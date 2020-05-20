import java.lang.reflect.InvocationHandler;
import java.lang.reflect.Method;
import java.lang.reflect.Proxy;

/**
 * 计算某个方法执行消耗的时间
 * 这个类是提供给客户端代理类使用的对象。
 * 相当于给{@link Proxy#h}这个字段赋值
 */
public class TimeConsumeHandler implements InvocationHandler {
    /**服务端的实例对象 */
    private Object mService;
    public TimeConsumeHandler(Object service){
        mService = service;
    }
    @Override
    public Object invoke(Object proxy, Method method, Object[] args) throws Throwable {
        //打印服务方法执行的时间
        long startTime = System.currentTimeMillis();
        Object value=method.invoke(mService,args);
        long consumedTime = System.currentTimeMillis()-startTime;
        System.out.println("the method of "+method.getName()+" consumed="+consumedTime);
        return value;
    }
}

public class LambdaTest {
    public static void main(String[] args) throws Exception{
         new Thread(()->{
            System.out.println("java8 Thread lambada");     
         }).start();
    }
}

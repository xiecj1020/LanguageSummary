public class AnonymousTest {
    public static void main(String[] args) throws Exception{
         new Thread(new Runnable() {
            @Override
            public void run() {
                System.out.println("anonymous test");
            }
         }).start();
    }
}

public class Test {
    public static void main(String[] args){
         MyObject o1=new MyObject();
         System.out.println(o1);
         String s1 = "通话";
         String s2 = "重地";
         printStringHashCode(s1);
         printStringHashCode(s2);
         System.out.println(s1.equals(s2));
    }
    static class MyObject{

    }
    /**
     *打印String类的计算方法
    */
    public static int printStringHashCode(String s) {
        int h = 0;
        final int len = s.length();
        System.out.println(s);
        System.out.println("-------------");
        System.out.println("len="+len);
        if (h == 0 && len > 0) {
            for (int i = 0; i < len; i++) {
                int c = s.charAt(i);
                h = 31 * h + c;
                System.out.println("i"+i+" is "+c+" h="+h);
            }
        }
        System.out.println("-------------");
        return h;
    }
}

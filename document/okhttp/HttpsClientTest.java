import java.io.FileInputStream;
import java.io.IOException;
import java.io.InputStream;
import java.io.OutputStream;
import java.net.InetSocketAddress;
import java.net.Socket;
import java.security.KeyManagementException;
import java.security.KeyStore;
import java.security.KeyStoreException;
import java.security.NoSuchAlgorithmException;
import java.security.Principal;
import java.security.PrivateKey;
import java.security.SecureRandom;
import java.security.UnrecoverableKeyException;
import java.security.cert.CertificateException;
import java.security.cert.X509Certificate;
import java.text.SimpleDateFormat;
import java.util.Arrays;
import java.util.Date;

import javax.net.ssl.KeyManager;
import javax.net.ssl.KeyManagerFactory;
import javax.net.ssl.SSLContext;
import javax.net.ssl.SSLSocket;
import javax.net.ssl.SSLSocketFactory;
import javax.net.ssl.TrustManager;
import javax.net.ssl.TrustManagerFactory;
import javax.net.ssl.X509KeyManager;
import javax.net.ssl.X509TrustManager;

/**
 * https客户端测试类
 * 生成Jks文件的命令:
 * keytool -genkey -keyalg RSA -alias clientkeystore -keystore clientkeystore.jks -storepass ts123456 -validity 360 -keysize 2048
 */
public class HttpsClientTest {
    public static final String KEYSTORE_PATH = "/home/user/test/java/keystore.jks";
    public static final String KEYSTORE_PATH_CLIENT = "/home/user/test/java/clientkeystore.jks";
    public static final char[] password = "ts123456".toCharArray();
    public static void main(String[] args){
        connectForSocket();
    }

    /**
     * 当前时间精确到毫秒以字符串形式输出
     * @return 当前时间的字符串
     */
    public static String getCurrentTimeForMillis(){
        SimpleDateFormat formatter = new SimpleDateFormat("dd-MMM-yyyy HH:mm:ss:SSS");
        return formatter.format(new Date(System.currentTimeMillis()));
    }
    public static void connectForSocket(){
        SSLContext sslContext = null;
        //标记SSLContext是否初始化成功
        boolean sslContextSuccess = false;
        try {
            KeyStore ks = KeyStore.getInstance("JKS");
            ks.load(new FileInputStream(KEYSTORE_PATH_CLIENT), password);
            KeyManagerFactory kmf = KeyManagerFactory.getInstance("SunX509");
            kmf.init(ks, password);
            KeyManager[] km = kmf.getKeyManagers();

            ks = KeyStore.getInstance("JKS");
            ks.load(new FileInputStream(KEYSTORE_PATH), password);
            TrustManagerFactory tmf = TrustManagerFactory.getInstance("SunX509");
            tmf.init(ks);
            TrustManager [] tm = tmf.getTrustManagers();

            sslContext = SSLContext.getInstance("TLS");
            sslContext.init(new KeyManager[]{new MyKeyManger(km)}, new TrustManager[]{new MyTrustManger(tm)}, new SecureRandom());
            sslContextSuccess = true;
        } catch (NoSuchAlgorithmException e) {
            e.printStackTrace();
            sslContextSuccess = false;
        } catch (CertificateException e) {
            e.printStackTrace();
            sslContextSuccess = false;
        } catch (KeyStoreException e) {
            e.printStackTrace();
            sslContextSuccess = false;
        } catch (IOException e) {
            e.printStackTrace();
            sslContextSuccess = false;
        } catch (UnrecoverableKeyException e) {
            e.printStackTrace();
            sslContextSuccess = false;
        } catch (KeyManagementException e) {
            e.printStackTrace();
            sslContextSuccess = false;
        }
        System.out.println("sslContextSuccess=" + sslContextSuccess);
        if(sslContextSuccess){
            Socket socket = null;
            try {
                SSLSocketFactory factory = sslContext.getSocketFactory();
                //客户端socket对象
                socket = new Socket();
                InetSocketAddress address = new InetSocketAddress("127.0.0.1",8000);
                socket.connect(address,10000);
                SSLSocket ss = (SSLSocket) factory.createSocket(socket,"127.0.0.1", 8000,true);
                System.out.println("client hand shake start"+" "+getCurrentTimeForMillis());
                ss.startHandshake();
                System.out.println("client hand shake End"+" "+getCurrentTimeForMillis());
                OutputStream os = ss.getOutputStream();
                //write the char of 'B' to server
                os.write(66);
                os.flush();
                InputStream is = ss.getInputStream();
                int serverMsg = is.read();
                System.out.println("client receive the server message:" + (char)serverMsg + " " + getCurrentTimeForMillis());
                System.out.println("client ok");
            } catch (IOException e1) {
                e1.printStackTrace();
            }finally {
                if(socket!=null){
                    try {
                        socket.close();
                    } catch (IOException e) {
                        e.printStackTrace();
                    }
                }
            }
        }
    }
    public static class MyTrustManger implements X509TrustManager{
        private X509TrustManager mManger;
        public MyTrustManger(TrustManager[] trustManagers){
            if(trustManagers[0] instanceof X509TrustManager){
                mManger = (X509TrustManager) trustManagers[0];
            }
        }
        @Override
        public void checkClientTrusted(X509Certificate[] chain, String authType) throws CertificateException {
            System.out.println("client checkClientTrusted "+ getCurrentTimeForMillis());
            mManger.checkClientTrusted(chain,authType);
        }

        @Override
        public void checkServerTrusted(X509Certificate[] chain, String authType) throws CertificateException {
            System.out.println("client checkServerTrusted "+ getCurrentTimeForMillis());
            mManger.checkServerTrusted(chain,authType);
        }

        @Override
        public X509Certificate[] getAcceptedIssuers() {
            System.out.println("client getAcceptedIssuers "+ getCurrentTimeForMillis());
            X509Certificate[] certificates = mManger.getAcceptedIssuers();
//            if(certificates!=null){
//                for(X509Certificate certificate:certificates){
//                    System.out.println("client getAcceptedIssuers certificate="+certificate+" "+getCurrentTimeForMillis());
//                }
//            }
            return certificates;
        }
    }
    public static class MyKeyManger implements X509KeyManager {
        private X509KeyManager mKeyManager;

        public MyKeyManger(KeyManager[] keyManagers) {
            if (keyManagers[0] instanceof X509KeyManager) {
                mKeyManager = (X509KeyManager) keyManagers[0];
            }
        }

        @Override
        public String[] getClientAliases(String keyType, Principal[] issuers) {
            System.out.println("client getClientAliases keyType=" + keyType+" "+ getCurrentTimeForMillis());
            return mKeyManager.getClientAliases(keyType, issuers);
        }

        @Override
        public String chooseClientAlias(String[] keyType, Principal[] issuers, Socket socket) {
            System.out.println("client chooseClientAlias keyType=" + Arrays.toString(keyType)+" "+ getCurrentTimeForMillis());
            return mKeyManager.chooseClientAlias(keyType, issuers, socket);
        }

        @Override
        public String[] getServerAliases(String keyType, Principal[] issuers) {
            System.out.println("client getServerAliases keyType=" + keyType+" "+ getCurrentTimeForMillis());
            return mKeyManager.getServerAliases(keyType, issuers);
        }

        @Override
        public String chooseServerAlias(String keyType, Principal[] issuers, Socket socket) {
            System.out.println("client chooseServerAlias keyType=" + keyType+" "+ getCurrentTimeForMillis());
            return mKeyManager.chooseServerAlias(keyType, issuers, socket);
        }

        @Override
        public X509Certificate[] getCertificateChain(String alias) {
            System.out.println("client getCertificateChain alias=" + alias+" "+ getCurrentTimeForMillis());
            return mKeyManager.getCertificateChain(alias);
        }

        @Override
        public PrivateKey getPrivateKey(String alias) {
            System.out.println("client getPrivateKey alias=" + alias+" "+ getCurrentTimeForMillis());
            return mKeyManager.getPrivateKey(alias);
        }
    }
}

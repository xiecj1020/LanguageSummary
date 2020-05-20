import java.io.FileInputStream;
import java.io.IOException;
import java.io.InputStream;
import java.io.OutputStream;
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
import javax.net.ssl.SSLServerSocket;
import javax.net.ssl.SSLServerSocketFactory;
import javax.net.ssl.TrustManager;
import javax.net.ssl.TrustManagerFactory;
import javax.net.ssl.X509KeyManager;
import javax.net.ssl.X509TrustManager;

/**
 * https服务端测试类
 * 生成Jks文件的命令:
 * keytool -genkey -keyalg RSA -alias serverkeystore -keystore keystore.jks -storepass ts123456 -validity 360 -keysize 2048
 */
public class HttpsServerTest {
    //服务端的jks文件
    private static final String KEYSTORE_PATH = "/home/user/test/java/keystore.jks";
    //客户端的jks文件
    private static final String KEYSTORE_PATH_CLIENT = "/home/user/test/java/clientkeystore.jks";
    //jks文件密码
    private static final char[] password = "ts123456".toCharArray();

    /**
     * 当前时间精确到毫秒以字符串形式输出
     *
     * @return 当前时间的字符串
     */
    public static String getCurrentTimeForMillis() {
        SimpleDateFormat formatter = new SimpleDateFormat("dd-MMM-yyyy HH:mm:ss:SSS");
        return formatter.format(new Date(System.currentTimeMillis()));
    }

    public static void main(String[] args) {
        SSLContext sslContext = null;
        //标记SSLContext是否初始化成功
        boolean sslContextSuccess = false;
        try {
            KeyStore ks = KeyStore.getInstance("JKS");
            ks.load(new FileInputStream(KEYSTORE_PATH), password);
            KeyManagerFactory kmf = KeyManagerFactory.getInstance("SunX509");
            kmf.init(ks, password);
            KeyManager[] km = kmf.getKeyManagers();

            ks = KeyStore.getInstance("JKS");
            ks.load(new FileInputStream(KEYSTORE_PATH_CLIENT), password);
            TrustManagerFactory tmf = TrustManagerFactory.getInstance("SunX509");
            tmf.init(ks);
            TrustManager[] tm = tmf.getTrustManagers();
            sslContext = SSLContext.getInstance("TLS");
            sslContext.init(new KeyManager[]{new HttpsServerTest.MyKeyManger(km)}, new TrustManager[]{new MyTrustManger(tm)}, new SecureRandom());
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
        if (sslContextSuccess) {
            SSLServerSocketFactory serverSocketFactory = sslContext.getServerSocketFactory();
            SSLServerSocket ss = null;
            try {
                ss = (SSLServerSocket) serverSocketFactory.createServerSocket(8000);
                //是否需要验证客户端,true开启客户端验证，即https双向验证
                ss.setNeedClientAuth(true);
                Socket s = ss.accept();
                System.out.println("server receive a client connect " + getCurrentTimeForMillis());
                InputStream is = s.getInputStream();
                int clientMsg = is.read();
                System.out.println("serve receive the client message:" + (char)clientMsg + " " + getCurrentTimeForMillis());
                OutputStream os = s.getOutputStream();
                //write the char of 'C" to client
                os.write(67);
                os.flush();
                System.out.println("server send message to client " + getCurrentTimeForMillis());
                System.out.println("server ok");
            } catch (IOException e) {
                e.printStackTrace();
            } finally {
                if (ss != null) {
                    try {
                        ss.close();
                    } catch (IOException e) {
                        e.printStackTrace();
                    }
                }
            }
        }
    }

    public static class MyTrustManger implements X509TrustManager {
        private X509TrustManager mManger;

        public MyTrustManger(TrustManager[] trustManagers) {
            if (trustManagers[0] instanceof X509TrustManager) {
                mManger = (X509TrustManager) trustManagers[0];
            }
        }

        @Override
        public void checkClientTrusted(X509Certificate[] chain, String authType) throws CertificateException {
            System.out.println("server checkClientTrusted " + getCurrentTimeForMillis());
            mManger.checkClientTrusted(chain, authType);
        }

        @Override
        public void checkServerTrusted(X509Certificate[] chain, String authType) throws CertificateException {
            System.out.println("server checkServerTrusted " + getCurrentTimeForMillis());
            mManger.checkServerTrusted(chain, authType);
        }

        @Override
        public X509Certificate[] getAcceptedIssuers() {
            System.out.println("server getAcceptedIssuers " + getCurrentTimeForMillis());
            X509Certificate[] certificates = mManger.getAcceptedIssuers();
//            if(certificates!=null){
//                for(X509Certificate certificate:certificates){
//                    System.out.println("server getAcceptedIssuers certificate="+certificate+" "+getCurrentTimeForMillis());
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
            System.out.println("server getClientAliases keyType=" + keyType + " " + getCurrentTimeForMillis());
            return mKeyManager.getClientAliases(keyType, issuers);
        }

        @Override
        public String chooseClientAlias(String[] keyType, Principal[] issuers, Socket socket) {
            System.out.println("server chooseClientAlias keyType=" + Arrays.toString(keyType) + " " + getCurrentTimeForMillis());
            return mKeyManager.chooseClientAlias(keyType, issuers, socket);
        }

        @Override
        public String[] getServerAliases(String keyType, Principal[] issuers) {
            System.out.println("server getServerAliases keyType=" + keyType + " " + getCurrentTimeForMillis());
            return mKeyManager.getServerAliases(keyType, issuers);
        }

        @Override
        public String chooseServerAlias(String keyType, Principal[] issuers, Socket socket) {
            System.out.println("server chooseServerAlias keyType=" + keyType + " " + getCurrentTimeForMillis());
            return mKeyManager.chooseServerAlias(keyType, issuers, socket);
        }

        @Override
        public X509Certificate[] getCertificateChain(String alias) {
            System.out.println("server getCertificateChain alias=" + alias + " " + getCurrentTimeForMillis());
            return mKeyManager.getCertificateChain(alias);
        }

        @Override
        public PrivateKey getPrivateKey(String alias) {
            System.out.println("server getPrivateKey alias=" + alias + " " + getCurrentTimeForMillis());
            return mKeyManager.getPrivateKey(alias);
        }
    }
}

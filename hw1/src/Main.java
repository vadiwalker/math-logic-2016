import java.io.*;
/**
 * Created by vadim on 24.10.16.
 */
public class Main {
    private static String fileName = "good3";
    public static void main(String[] args) {
        try {
            InputStream in = new FileInputStream("input/" + fileName + ".in");
            OutputStream out = new FileOutputStream("output/" + fileName + ".out");
            ProofChecker.checkProof(in, out);
            out.close();
        } catch(Exception e) {
            throw new RuntimeException(e);
        }
    }
}

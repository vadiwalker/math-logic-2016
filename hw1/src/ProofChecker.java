import java.io.*;
import java.util.ArrayList;
import java.util.HashMap;


public class ProofChecker {
    private static BufferedReader br;
    private static PrintWriter pw;
    private static Parser parser = new Parser();
    private static Node[] ax;
    private static int AXIOM_CNT = 10;
    private static HashMap<Character, String> mp = new HashMap<>();

    static {
        ax = new Node[AXIOM_CNT];
        ax[0] = parser.get("A->B->A");
        ax[1] = parser.get("(A->B)->(A->B->C)->(A->C)");
        ax[2] = parser.get("A&B->A");
        ax[3] = parser.get("A&B->B");
        ax[4] = parser.get("A->B->A&B");
        ax[5] = parser.get("A->A|B");
        ax[6] = parser.get("B->A|B");
        ax[7] = parser.get("(A->C)->(B->C)->(A|B->C)");
        ax[8] = parser.get("(A->B)->(A->!B)->!A");
        ax[9] = parser.get("!!A->A");
    }

    private static boolean check(Node ax, Node b) {
        if (ax == null && b == null) {
            return true;
        }
        if (ax == null || b == null) {
            return false;
        }
        if (Character.isLetter(ax.var.charAt(0))) {
            if (mp.containsKey(ax.var.charAt(0))) {
                return mp.get(ax.var.charAt(0)).equals(b.toString());
            } else {
                mp.put(ax.var.charAt(0), b.toString());
                return true;
            }
        } else {
            return ax.var.equals(b.var) && check(ax.lson, b.lson) && check(ax.rson, b.rson);
        }
    }



    public static void checkProof(InputStream in, OutputStream out) {
        br = new BufferedReader(new InputStreamReader(in));
        pw = new PrintWriter(out);
        HashMap<String, Integer> hypo = new HashMap<>();
        HashMap<String, Pair<Integer, Integer>> modus = new HashMap<>();
        HashMap<String, Integer> proved = new HashMap<>();
        HashMap<String, ArrayList<Pair<String, Integer>>> cursor = new HashMap<>();
        String headLine = nextLine().replaceAll("\\s+", "");
        pw.println(headLine);
        String[] suppose = headLine.split("\\|-");


        assert suppose.length == 2;

        Node needToProve = parser.get(suppose[1]);

        String hypothems[] = suppose[0].split(",");

        for (int i = 0; i < hypothems.length; ++i) {
            String x = hypothems[i];
            if (x.length() == 0) {
                continue;
            }
            Node foo = parser.get(x);
            hypo.put(foo.toString(), i+1);
        }

        outer:
        for (int i = 1; hasNextLine(); i++) {
            Node foo = parser.get(nextLine().replaceAll("\\s+", ""));
            String curExpr = foo.toString();

            if (cursor.containsKey(curExpr)) {
                for (Pair<String, Integer> pair : cursor.get(curExpr)) {
                    String str = pair.first;
                    modus.put(str, new Pair<>(pair.second, i));
                }
                cursor.remove(curExpr);
            }
            if (foo.var.equals("->")) {
                String left = foo.lson.toString();
                String right = foo.rson.toString();
                if (proved.containsKey(left)) {
                    modus.put(right, new Pair<>(i, proved.get(left)));
                }

                ArrayList<Pair<String, Integer>> array = cursor.get(left);
                if (array == null) {
                    array = new ArrayList<>();
                }
                array.add(new Pair<>(right, i));
                cursor.put(left, array);
            }

            pw.print("(" + i + ")" + curExpr);
            if (hypo.containsKey(curExpr)) {
                pw.println("(Предп. " + hypo.get(curExpr) + ")");
                proved.put(curExpr, i);
                continue;
            }

            for (int j = 0; j < AXIOM_CNT; ++j) {
                mp.remove('A');
                mp.remove('B');
                mp.remove('C');
                if (check(ax[j], foo)) {
                    pw.println("(Сх. акс. " + (j+1) + ")");
                    proved.put(curExpr, i);
                    continue outer;
                }
            }

            if (modus.containsKey(curExpr)) {
                Pair<Integer, Integer> x = modus.get(curExpr);
                pw.println("(M.P. " + x.first + ", " + x.second + ")");
                proved.put(curExpr, i);
                continue;
            }
            pw.println("(Не доказано)");
        }
    }

    public static class Pair<V, U> {
        V first;
        U second;
        Pair(V first, U second) {
           this.first = first;
           this.second = second;
        }
    }

    private static String nextLine() {
        try {
            return br.readLine();
        } catch(IOException e) {
            throw new RuntimeException(e);
        }
    }

    private static boolean hasNextLine() {
        try {
            return br.ready();
        } catch(IOException e) {
            throw new RuntimeException(e);
        }
    }
}

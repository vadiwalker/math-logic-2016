/**
 * Created by vadim on 24.10.16.
 */
public class Node {
    String var;
    Node lson;
    Node rson;
    Node(String var, Node lson, Node rson) {
        this.var = var;
        this.lson = lson;
        this.rson = rson;
    }

    @Override
    public String toString() {
        StringBuilder ret = new StringBuilder();
        if (lson == null) {
            return var;
        } else if (rson == null) {
            return var + lson.toString();
        } else {
            return "(" + lson.toString() + var + rson.toString() + ")";
        }
    }
}

/**
 * Created by vadim on 24.10.16.
 */
public class Parser {
    enum L {
        OR, AND, VAR, CURS, OP, CLO, END, NOT
    }
    private L lex;
    private int ptr;
    private String formula;
    private String val;

    private void takeLex() {
        if (ptr >= formula.length()) {
            lex = L.END;
            return;
        }
        switch(formula.charAt(ptr)) {
            case '(':
                lex = L.OP;
                ptr++;
                break;
            case ')':
                lex = L.CLO;
                ptr++;
                break;
            case '|':
                lex = L.OR;
                ptr++;
                break;
            case '&':
                lex = L.AND;
                ptr++;
                break;
            case '-':
                lex = L.CURS;
                ptr += 2;
                break;
            case '!':
                lex = L.NOT;
                ptr++;
                break;
            default:
                lex = L.VAR;
                StringBuilder str = new StringBuilder();
                while (ptr < formula.length() && Character.isLetterOrDigit(formula.charAt(ptr))) {
                    str.append(formula.charAt(ptr++));
                }
                val = str.toString();
        }
    }

    private Node expression() {
        Node foo = or();
        if (lex.equals(L.CURS)) {
            takeLex();
            return new Node("->", foo, expression());
        } else {
            return foo;
        }
    }

    private Node or() {
        Node foo = and();
        if (lex.equals(L.OR)) {
            takeLex();
            return new Node("|", foo, or());
        } else {
            return foo;
        }
    }

    private Node and() {
        Node foo = no();
        if (lex.equals(L.AND)) {
            takeLex();
            return new Node("&", foo, and());
        } else {
            return foo;
        }
    }

    private Node no() {
        if (lex.equals(L.VAR)) {
            takeLex();
            return new Node(val, null, null);
        } else if (lex.equals(L.OP)) {
            takeLex();
            Node ret = expression();
            assert lex.equals(L.CLO);
            takeLex();
            return ret;
        } else {
            assert lex.equals(L.NOT);
            takeLex();
            return new Node("!", no(), null);
        }
    }

    public Node get(String expr) {
        formula = expr;
        ptr = 0;
        takeLex();
        return expression();
    }
}

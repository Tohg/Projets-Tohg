import java.util.ArrayList;

public class Set {
    private ArrayList<Integer> elements;

    public Set() {
        this.elements = new ArrayList<>();
    }

    public static Set singleton(Integer x) {
        Set s = new Set();
        s.add(x);
        return s;
    }

    public Integer cardinal() {
        return elements.size();
    }

    public void add(Integer x) {
        if (!elements.contains(x)) {
            elements.add(x);
        }
    }

    public void print() {
        System.out.print("{");
        for (int i = 0; i < elements.size(); i++) {
            System.out.print(elements.get(i) + (i < elements.size() - 1 ? ", " : ""));
        }
        System.out.println("}");
    }

    public Set intersect(Set s) {
        Set result = new Set();
        for (Integer val : this.elements) {
            if (s.elements.contains(val)) {
                result.add(val);
            }
        }
        return result;
    }

    public Set union(Set s) {
        Set result = new Set();
        for (Integer val : this.elements) result.add(val);
        for (Integer val : s.elements) result.add(val);
        return result;
    }
}
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

    public static void main(String[] args) {
        System.out.println("=== Test de la classe Set ===\n");
        
        // Test singleton
        System.out.println("Test singleton:");
        Set s1 = Set.singleton(5);
        System.out.print("Singleton(5): ");
        s1.print();
        System.out.println("Cardinal: " + s1.cardinal());
        
        // Test création manuelle
        System.out.println("\nTest création manuelle:");
        Set s2 = new Set();
        s2.add(1);
        s2.add(2);
        s2.add(3);
        s2.add(2); // doublon, ne doit pas être ajouté
        System.out.print("Set s2 {1, 2, 3, 2}: ");
        s2.print();
        System.out.println("Cardinal: " + s2.cardinal());
        
        Set s3 = new Set();
        s3.add(2);
        s3.add(3);
        s3.add(4);
        System.out.print("Set s3 {2, 3, 4}: ");
        s3.print();
        System.out.println("Cardinal: " + s3.cardinal());
        
        // Test intersection
        System.out.println("\nTest intersection (s2 ∩ s3):");
        Set inter = s2.intersect(s3);
        System.out.print("Résultat: ");
        inter.print();
        System.out.println("Cardinal: " + inter.cardinal());
        
        // Test union
        System.out.println("\nTest union (s2 ∪ s3):");
        Set uni = s2.union(s3);
        System.out.print("Résultat: ");
        uni.print();
        System.out.println("Cardinal: " + uni.cardinal());
    }
}
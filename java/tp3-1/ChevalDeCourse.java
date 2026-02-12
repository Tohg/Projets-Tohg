public class ChevalDeCourse extends Cheval {
    private double montantGains; // [cite: 19]
    private Entraineur entraineur; // [cite: 31]
    private Jockey jockey; // [cite: 32]

    public ChevalDeCourse(String n, String s, String r, double g, Entraineur e) {
        super(n, s, r); // [cite: 31]
        this.montantGains = g;
        this.entraineur = e;
        this.jockey = null; // Jockey inconnu à la création [cite: 32]
    }

    public double getGains() { return montantGains; }
    public void attribueJockey(Jockey j) { this.jockey = j; } // [cite: 34]
    
    public void affiche() { // [cite: 34]
        System.out.println(this.toString() + " | Gains: " + montantGains + "€");
    }
}
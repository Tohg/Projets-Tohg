public class Cheval {
    private static int compteur = 1; // [cite: 28]
    private int numIdentification; // [cite: 28]
    protected String nom, sexe, race; // [cite: 28]

    public Cheval(String nom, String sexe, String race) { // [cite: 29]
        this.numIdentification = compteur++; // Gestion automatique [cite: 28]
        this.nom = nom;
        this.sexe = sexe;
        this.race = race;
    }
    public String getNom() { return nom; }
    public int getNumIdentification() { return numIdentification; }
    public String toString() { return "#" + numIdentification + " - " + nom; }
}
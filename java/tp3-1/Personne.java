public class Personne {
    protected String nom, prenom, adresse; // [cite: 24]

    public Personne(String nom, String prenom, String adresse) { // [cite: 25]
        this.nom = nom;
        this.prenom = prenom;
        this.adresse = adresse;
    }
    public String getNom() { return nom; }
    public String toString() { return prenom + " " + nom + " (" + adresse + ")"; }
}
import java.util.*;

public class Voiture {
    private String immatriculation;
    private String modele;
    private int nbKilometres;
    private float tarifKm;

    // Constructeur avec saisie clavier (Question 1)
    public Voiture() {
        Scanner sc = new Scanner(System.in);
        System.out.print("Immatriculation : ");
        this.immatriculation = sc.nextLine();
        System.out.print("Modèle : ");
        this.modele = sc.nextLine();
        System.out.print("Kilométrage : ");
        this.nbKilometres = sc.nextInt();
        System.out.print("Tarif au km : ");
        this.tarifKm = sc.nextFloat();
    }

    // Constructeur sans saisie (nécessaire pour la Question 6)
    public Voiture(String immat, String mod, int km, float tarif) {
        this.immatriculation = immat;
        this.modele = mod;
        this.nbKilometres = km;
        this.tarifKm = tarif;
    }

    // Accesseurs (Getters/Setters)
    public String getImmatriculation() { return immatriculation; }
    public int getNbKilometres() { return nbKilometres; }
    public void setNbKilometres(int km) { this.nbKilometres = km; }
    public float getTarifKm() { return tarifKm; }

    public void afficher() {
        System.out.println("Voiture " + modele + " (" + immatriculation + ") - " + nbKilometres + " km - Tarif: " + tarifKm + "€/km");
    }

    // Main de test (Question 2)
    public static void main(String[] args) {
        ArrayList<Voiture> flotte = new ArrayList<>(10);
        // Ajout, affichage et suppression à tester ici
    }
}
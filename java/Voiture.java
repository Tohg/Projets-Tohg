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
        System.out.println("=== Test de la classe Voiture ===\n");
        
        // Test avec constructeur sans saisie (données pré-définies)
        ArrayList<Voiture> flotte = new ArrayList<>(10);
        
        Voiture v1 = new Voiture("AB-123-CD", "Clio", 50000, 0.5f);
        Voiture v2 = new Voiture("EF-456-GH", "Tesla S", 10000, 1.2f);
        Voiture v3 = new Voiture("IJ-789-KL", "Peugeot 308", 75000, 0.6f);
        
        flotte.add(v1);
        flotte.add(v2);
        flotte.add(v3);
        
        System.out.println("Flotte de véhicules:");
        for (Voiture v : flotte) {
            v.afficher();
        }
        
        System.out.println("\nTest des getters:");
        System.out.println("Immatriculation v1: " + v1.getImmatriculation());
        System.out.println("Kilométrage v1: " + v1.getNbKilometres() + " km");
        System.out.println("Tarif v1: " + v1.getTarifKm() + "€/km");
        
        System.out.println("\nTest de modification du kilométrage:");
        System.out.println("Avant: " + v1.getNbKilometres() + " km");
        v1.setNbKilometres(51000);
        System.out.println("Après: " + v1.getNbKilometres() + " km");
        v1.afficher();
    }
}
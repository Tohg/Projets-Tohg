public class Location {
    private static int cptLoc = 0;
    private int numero;
    private Voiture voiture;
    private Client client;
    private Date dateLocation;
    private Date dateRetour;
    private int kmDepart;
    private int kmRetour;

    public Location(Voiture v, Client c, Date d) {
        this.numero = ++cptLoc;
        this.voiture = v;
        this.client = c;
        this.dateLocation = d;
        this.dateRetour = null; // En cours
        this.kmDepart = v.getNbKilometres();
    }

    public void terminerLocation(Date dRetour, int kmR) {
        this.dateRetour = dRetour;
        this.kmRetour = kmR;
    }

    public void afficher() {
        System.out.print("Location n°" + numero + " | Client: " + client.getNumero() + " | Voiture: " + voiture.getImmatriculation());
        if (dateRetour == null) {
            System.out.println(" [EN COURS]");
        } else {
            System.out.println(" [TERMINEE]");
        }
    }
    
    // Getters nécessaires pour les calculs de l'Agence
    public Voiture getVoiture() { return voiture; }
    public Date getDateLocation() { return dateLocation; }
    
    public boolean isEnCours() {
        return dateRetour == null;
    }

    public static void main(String[] args) {
        // Test de création de locations
        Voiture v1 = new Voiture("AB-123-CD", "Clio", 50000, 0.5f);
        Voiture v2 = new Voiture("EF-456-GH", "Tesla S", 10000, 1.2f);
        Client c1 = new Client("Durand", "Lannion");
        Client c2 = new Client("Le Gall", "Brest");
        
        Date d1 = new Date(15, 2, 2026);
        Date d2 = new Date(20, 2, 2026);
        
        Location loc1 = new Location(v1, c1, d1);
        Location loc2 = new Location(v2, c2, d1);
        
        System.out.println("Test affichage locations:");
        loc1.afficher();
        loc2.afficher();
        
        System.out.println("\nTest isEnCours (avant retour):");
        System.out.println("Location 1 en cours: " + loc1.isEnCours());
        
        System.out.println("\nTerminer location 1:");
        loc1.terminerLocation(d2, 51000);
        loc1.afficher();
        System.out.println("Location 1 en cours: " + loc1.isEnCours());
    }
}
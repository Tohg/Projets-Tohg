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
}
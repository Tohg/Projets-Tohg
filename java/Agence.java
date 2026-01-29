import java.util.*;

public class Agence {
    private String nom;
    private ArrayList<Voiture> lesVoitures;
    private ArrayList<Client> lesClients;
    private ArrayList<Location> lesContrats;

    public Agence(String nom) {
        this.nom = nom;
        // Initialisation avec capacité de 100 [cite: 32]
        this.lesVoitures = new ArrayList<>(100);
        this.lesClients = new ArrayList<>(100);
        this.lesContrats = new ArrayList<>(100);

        // Données de test (Question 6) sans saisie clavier [cite: 33, 34]
        lesVoitures.add(new Voiture("AB-123-CD", "Clio", 50000, 0.5f));
        lesVoitures.add(new Voiture("EF-456-GH", "Tesla S", 10000, 1.2f));
        lesClients.add(new Client("Durand", "Lannion"));
        lesClients.add(new Client("Le Gall", "Brest"));
    }

    // Question 7 : Enregistrer une location [cite: 37, 39, 40]
    public void enregistrerLocation() {
        Scanner sc = new Scanner(System.in);
        
        // 1. Saisie et vérification du client
        System.out.print("Saisir n° client : ");
        int numC = sc.nextInt();
        Client c = chercherClient(numC);
        while (c == null) {
            System.out.println("Erreur : client inexistant !");
            System.out.print("Saisir n° client : ");
            numC = sc.nextInt();
            c = chercherClient(numC);
        }

        // 2. Affichage voitures disponibles [cite: 40, 42]
        ArrayList<Voiture> dispos = getVoituresDisponibles();
        if (dispos.isEmpty()) {
            System.out.println("Aucune voiture disponible.");
            return;
        }
        for (Voiture v : dispos) v.afficher();

        // 3. Choix de la voiture
        System.out.print("Saisir immatriculation : ");
        sc.nextLine(); // buffer
        String immat = sc.nextLine();
        Voiture v = chercherVoitureDispo(immat);
        while (v == null) {
            System.out.println("Erreur : voiture non trouvée ou déjà louée !");
            immat = sc.nextLine();
            v = chercherVoitureDispo(immat);
        }

        // 4. Création de la location
        System.out.println("Saisie date départ (j m a) : ");
        Date dDep = new Date(sc.nextInt(), sc.nextInt(), sc.nextInt());
        Location loc = new Location(v, c, dDep);
        lesContrats.add(loc);
        System.out.println("Location enregistrée.");
    }

    // Question 8 : Enregistrer le retour [cite: 45, 47, 48]
    public void enregistrerRetour() {
        Scanner sc = new Scanner(System.in);
        System.out.print("Saisir immatriculation voiture au retour : ");
        String immat = sc.nextLine();
        
        Location loc = chercherLocationEnCours(immat);
        while (loc == null) {
            System.out.println("Erreur : pas de location en cours pour ce véhicule.");
            immat = sc.nextLine();
            loc = chercherLocationEnCours(immat);
        }

        loc.afficher();
        
        System.out.println("Saisir date du jour (j m a) : ");
        Date dRetour = new Date(sc.nextInt(), sc.nextInt(), sc.nextInt());
        // Vérification chronologique (Optionnel Question 8 point 1) [cite: 49, 50]
        while (!dRetour.estApres(loc.getDateLocation())) {
            System.out.println("Erreur : la date de retour est antérieure au départ.");
            dRetour = new Date(sc.nextInt(), sc.nextInt(), sc.nextInt());
        }

        System.out.print("Saisir kilométrage retour : ");
        int kmR = sc.nextInt();
        while (kmR <= loc.getVoiture().getNbKilometres()) {
            System.out.println("Erreur : kilométrage incohérent !");
            kmR = sc.nextInt();
        }

        // Calculs finaux [cite: 48]
        int distance = kmR - loc.getVoiture().getNbKilometres();
        float prix = distance * loc.getVoiture().getTarifKm();
        System.out.println("Distance parcourue : " + distance + " km");
        System.out.println("Prix à payer : " + prix + " €");

        // Mise à jour [cite: 48]
        loc.terminerLocation(dRetour, kmR);
        loc.getVoiture().setNbKilometres(kmR);
    }

    // --- Méthodes utilitaires (Aides) ---

    private Client chercherClient(int num) {
        for (Client c : lesClients) if (c.getNumero() == num) return c;
        return null;
    }

    private ArrayList<Voiture> getVoituresDisponibles() {
        ArrayList<Voiture> dispos = new ArrayList<>(lesVoitures);
        for (Location loc : lesContrats) {
            if (loc.isEnCours()) dispos.remove(loc.getVoiture());
        }
        return dispos;
    }

    private Voiture chercherVoitureDispo(String immat) {
        for (Voiture v : getVoituresDisponibles()) {
            if (v.getImmatriculation().equals(immat)) return v;
        }
        return null;
    }

    private Location chercherLocationEnCours(String immat) {
        for (Location loc : lesContrats) {
            if (loc.isEnCours() && loc.getVoiture().getImmatriculation().equals(immat)) return loc;
        }
        return null;
    }
}
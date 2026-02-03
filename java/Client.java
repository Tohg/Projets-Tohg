public class Client {
    private static int compteur = 0;
    private int numero;
    private String nom;
    private String domicile;

    public Client(String nom, String domicile) {
        this.numero = ++compteur;
        this.nom = nom;
        this.domicile = domicile;
    }

    public int getNumero() { return numero; }

    public void afficher() {
        System.out.println("Client n°" + numero + " : " + nom + " demeurant à " + domicile);
    }

    public static void main(String[] args) {
        System.out.println("=== Test de la classe Client ===\n");
        
        // Test création de clients
        Client c1 = new Client("Durand", "Lannion");
        Client c2 = new Client("Le Gall", "Brest");
        Client c3 = new Client("Martin", "Quimper");
        
        System.out.println("Affichage des clients créés:");
        c1.afficher();
        c2.afficher();
        c3.afficher();
        
        System.out.println("\nNuméros générés automatiquement:");
        System.out.println("Client 1 numéro: " + c1.getNumero());
        System.out.println("Client 2 numéro: " + c2.getNumero());
        System.out.println("Client 3 numéro: " + c3.getNumero());
    }
}
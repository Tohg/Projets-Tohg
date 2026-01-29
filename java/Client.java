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
}
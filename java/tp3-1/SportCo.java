public class SportCo extends Sport { // [cite: 10]
    private int nbJoueurs; // [cite: 10]

    public SportCo(String code, String libelle, int nbJoueurs) {
        super(code, libelle); // Initialise la classe parente
        this.nbJoueurs = nbJoueurs;
    }

    @Override
    public String toString() { // [cite: 11]
        return super.toString() + ", Nombre de joueurs: " + nbJoueurs;
    }
}
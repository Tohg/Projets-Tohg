public class Gerant extends Employe {
    private final int nb_h_g = 30;

    public Gerant(String nom, int age, int nb_A) {
        super(nom, age, nb_A); 
    }

    public int getNbHG() {
        return nb_h_g;
    }


    public double calculerSalaire() {
        double salaireGerant;
        salaireGerant = 5.0 * this.getNbHG() * ( 20.0 + getNbAnnEx()* 0.5);
        return salaireGerant;
    }

    public String toString() {
        return "Catégorie :  Gerant " + super.toString();
    }
}
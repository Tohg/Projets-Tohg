public class Ouvrier extends Employe {
     private final int nb_h_o = 35;

    public Ouvrier(String nom, int age, int nb_A) {
        super(nom, age, nb_A); 

    }

    public int getNbHO() {
        return nb_h_o;
    }

    public double calculerSalaire() {
        double salaireOuvrier;
        salaireOuvrier = 4.0* getNbHO() * ( 10.0 + getNbAnnEx() * 0.5);
        return salaireOuvrier;
    }


    public String toString() {
        return "Catégorie :  Ouvrier " + super.toString();
    }
}
public class Gerant extends Emplye {
    private String nb_h_g; // nombre d'heures fixes gérant

    public Gerant(String nom, int age, String nb_A, String nb_h_g) {
        super(nom, age, nb_A); 
        this.nb_h_g = nb_h_g;
    }

    public String getNbHG() {
        return nb_h_g;
    }


    public double calculerSalaire() {
        double salaireGerant;
        salaireGerant = 5* Double.parseDouble(getnbHO) * ( 20 + (Double.parseDouble(getNbAnnEx)* 0.5));
        return salaireGerant;
    }
}
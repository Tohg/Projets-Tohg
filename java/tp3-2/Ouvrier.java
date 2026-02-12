public class Ouvrier extends Employe {
    private String nb_h_o; // nombre d'heures fixes ouvrier

    public Ouvrier(String nom, int age, String nb_A, String nb_h_o) {
        super(nom, age, nb_A); 
        this.nb_h_o = nb_h_o;
    }

    public String getNbHO() {
        return nb_h_o;
    }

    public double calculerSalaire() {
        double salaireOuvrier;
        salaireOuvrier = 4* Double.parseDouble(getnbHO) * ( 10 + (Double.parseDouble(getNbAnnEx)* 0.5));
        return salaireOuvrier;
    }
}
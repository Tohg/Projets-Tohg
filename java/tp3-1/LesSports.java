import java.util.ArrayList;

public class LesSports {
    private ArrayList<Sport> listeSports; // 

    public LesSports() {
        this.listeSports = new ArrayList<>();
    }

    public void ajouterSport(Sport s) { // [cite: 13]
        listeSports.add(s);
    }

    public void afficherLesSports() { // [cite: 14]
        for (Sport s : listeSports) {
            s.affiche();
        }
    }
}
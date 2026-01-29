import java.util.Scanner;

public class Date {
    private int jour, mois, annee;

    public Date(int j, int m, int a) {
        this.jour = j; this.mois = m; this.annee = a;
    }

    public void afficher() {
        System.out.print(jour + "/" + mois + "/" + annee);
    }

    // Retourne vrai si la date courante est après ou égale à d2
    public boolean estApres(Date d2) {
        if (this.annee != d2.annee) return this.annee > d2.annee;
        if (this.mois != d2.mois) return this.mois > d2.mois;
        return this.jour >= d2.jour;
    }
}
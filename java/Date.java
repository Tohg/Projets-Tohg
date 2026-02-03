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

    public static void main(String[] args) {
        System.out.println("=== Test de la classe Date ===\n");
        
        // Test création et affichage
        Date d1 = new Date(15, 2, 2026);
        Date d2 = new Date(20, 2, 2026);
        Date d3 = new Date(15, 3, 2026);
        Date d4 = new Date(15, 2, 2025);
        
        System.out.println("Affichage des dates:");
        System.out.print("Date 1: ");
        d1.afficher();
        System.out.println();
        
        System.out.print("Date 2: ");
        d2.afficher();
        System.out.println();
        
        System.out.print("Date 3 (mois suivant): ");
        d3.afficher();
        System.out.println();
        
        System.out.print("Date 4 (année précédente): ");
        d4.afficher();
        System.out.println();
        
        System.out.println("\nTest de la méthode estApres():");
        System.out.println("d1.estApres(d2): " + d1.estApres(d2) + " (15/02 après 20/02? FALSE)");
        System.out.println("d2.estApres(d1): " + d2.estApres(d1) + " (20/02 après 15/02? TRUE)");
        System.out.println("d1.estApres(d1): " + d1.estApres(d1) + " (même date? TRUE)");
        System.out.println("d3.estApres(d1): " + d3.estApres(d1) + " (mois suivant? TRUE)");
        System.out.println("d1.estApres(d4): " + d1.estApres(d4) + " (année suivante? TRUE)");
    }
}
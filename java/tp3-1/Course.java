import java.util.ArrayList;

public class Course {
    private String nomEpreuve;
    private double dotation;
    private ArrayList<ChevalDeCourse> partants; // 

    public Course(String nom, double dot) {
        this.nomEpreuve = nom;
        this.dotation = dot;
        this.partants = new ArrayList<>(); // [cite: 37]
    }

    public boolean chevalPresent(String nom) { // [cite: 38]
        for (ChevalDeCourse c : partants) {
            if (c.getNom().equals(nom)) return true;
        }
        return false;
    }

    public void enregistre(ChevalDeCourse c) { // [cite: 40]
        if (!chevalPresent(c.getNom())) {
            partants.add(c);
        }
    }

    public void rechercheCheval(String nom) { // [cite: 42]
        boolean trouve = false;
        for (ChevalDeCourse c : partants) {
            if (c.getNom().equals(nom)) {
                c.affiche();
                trouve = true;
            }
        }
        if (!trouve) System.out.println("Absent");
    }

    public ChevalDeCourse meilleureCote() { // [cite: 44]
        if (partants.isEmpty()) return null;
        ChevalDeCourse meilleur = partants.get(0);
        for (ChevalDeCourse c : partants) {
            if (c.getGains() > meilleur.getGains()) meilleur = c;
        }
        return meilleur;
    }
}
public class Employe {
    private static int numeroEmploye = 1;
    private String nom;
    private int age;
    private String nb_A;
    
    public Employe(String nom, int age, String nb_A) {
        this.nom = nom;
        this.age = age;
        this.nb_A = nb_A;
        numeroEmploye++;
    }

    public String getNom() {
        return nom;
    }
    public int getAge() {
        return age;
    }
    public String getNbAnnEx() {
        return nb_A;
    }
    public String toString() {
        return "Employé #" + numeroEmploye + ": " + nom + ", Age: " + age + ", Années d'expérience: " + nbAnnEx;
    }

    public abstract double calculerSalaire(); 

}
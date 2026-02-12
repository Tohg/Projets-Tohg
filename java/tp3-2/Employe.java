public abstract class Employe {
    private static int numeroEmploye = 1;
    private String nom;
    private int age;
    private int nb_A;
    
    public Employe(String nom, int age, int nb_A) {
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
    public int getNbAnnEx() {
        return nb_A;
    }
    public String toString() {
        return "Employé #" + numeroEmploye + ": " + nom + ", Age: " + age + ", Années d'expérience: " + nb_A + ", Salaire : " + this.calculerSalaire();
    }

    public static int getnum() {
        return numeroEmploye;
    }

    public void afficher() {
        System.out.println(this);
    }

    public abstract double calculerSalaire(); 

}
public class Entraineur extends Personne {
    private String numLicence; // [cite: 20]
    public Entraineur(String n, String p, String a) { super(n, p, a); }
    public void setNumLicence(String s) { this.numLicence = s; }
}
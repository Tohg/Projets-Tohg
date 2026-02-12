public class Sport {
    protected String code; // 
    protected String libelle; // 

    public Sport(String code, String libelle) { // [cite: 7]
        this.code = code;
        this.libelle = libelle;
    }

    public String getCode() { return code; } // [cite: 8]
    public String getLibelle() { return libelle; } // [cite: 8]

    @Override
    public String toString() { // [cite: 8]
        return "Code: " + code + ", Libellé: " + libelle;
    }

    public void affiche() { // [cite: 8]
        System.out.println(this.toString());
    }
}
public class Exercice1 {
    public static void main(String[] args) {
        Ouvrier o1 = new Ouvrier("Bernard",29,6);
        Ouvrier o2 = new Ouvrier("Jean",19,1);
        Ouvrier o3 = new Ouvrier("Paul",23,3);
        Gerant g1 = new Gerant("Michelle", 38, 8);

        String nbremploye = "Nombre d'employés : " + getnum();
        System.out.println(nbremploye);
    }
}

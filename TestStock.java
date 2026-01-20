package tp2;

import java.util.*;

public class TestStock {
    public static void main(String[] args) {

    }

    public class Produit {
        private String reference;
        private int date_entree;

        public String toString() {
            return "Référence : " + reference + "\nEntrée dans le stock le " + date_entree + " jour";
        }

        void afficher() {
            System.out.print(this.toString());
        }

        public Produit(int date) {
            date_entree = date;
            System.out.println("Quelle est la référence du produit");
            Scanner entree = new Scanner(System.in);
            reference = entree.next();
            entree.close();
        }
    }

    public class Pile {
        private Produit[] pileproduit;
        private int indice = -1;

        public Pile(int max) {
            indice = max;
            pileproduit = new Produit[max];
        }

        boolean pileVide(){
            /*delivre vrai si la pile est vide, faux sinon */
            if (pileproduit[0] == null){
                return true;
            }
            else{
                return false;
            }
        }

        boolean pilePleine(){
            /*delivre vrai si la pile est pleine, faux sinon */
            if (pileproduit[indice] != null){
                return true;
            }
            else{
                return false;
            }
        }

        void empiler(Produit p) {
            /* permet de stocker un nouveau produit au sommet de la pile */
            if (pilePleine() != true){
                for (int i = indice - 1 ; i > 0; i--){
                    pileproduit[i+1] = pileproduit[i];
                }
                pileproduit[0] = p;
            }
        }

        void depiler() {
            /* permet de sortir le produit situé au sommet de la pile */
            int fin = 0;
            for (int i = 0; i < indice && pileproduit[i] != null; i++){
                    pileproduit[i] = pileproduit[i + 1];
                    fin = i;
            }
            if (fin < indice){
                pileproduit[fin+1] = null;
            }
        }

        Produit sommet() {
            /* délivre le produit au sommet de la pile */
            return pileproduit[0];
        }

        void afficherStock() {
            for (int i=0; i < indice; i++){
               pileproduit[i].afficher();
            }
        }
    }
    public class Stock {
        private Pile stock;
        private int dateJ = 1;
        public Stock(int taille){
            
        }
        void entrer(Produit p){
            /*permet d'entrer un produit p dans le stock */
            
            
        }
        void sortir(int dateJ){
            /*permet de sortir le produit le plus frais du stock
            À la sortie d'un produit du stock, il ne peut etre 
            vendu si sa date d'entree dans le stock est supérieur
            de 5 jours à celle de la date du jour dateJ. dans un tel cas
            On sortira successivement de la pile tous les produits ne 
            pouvant pas être vendus*/
            
        }
        void afficheMenu(){
            /*propose à l'utilisateur ce menu 
            
            - e : entrée d'un produit dans le stock
            - s : sortie d'un produit du stock
            - i : incrémenter la date du jour
            - q : quitter*/
            System.out.println("- e : entrée d'un produit dans le Stock");
            System.out.println("- s : sortie d'un produit du Stock");
            System.out.println("- i : incrémenter la date du jour");
            System.out.println("- q : quitter");
        }
    }

}

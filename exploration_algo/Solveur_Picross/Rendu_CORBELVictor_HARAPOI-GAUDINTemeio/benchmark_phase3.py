import random
import time

import picross_phase2
import picross_phase3

genere_croix = picross_phase2.genere_croix
backtracking = picross_phase2.backtracking
indices_depuis_solution = picross_phase2.indices_depuis_solution
solveur_hybride = picross_phase3.solveur_hybride

# Solveur de Picross
# Rapport d'Analyse :
# - Analyse de complexité
# - Benchmark de temps en fonction de la taille
# - Benchmark de temps en fonction de la densité
# - Discussion : Pourquoi l'approche "Propagation" est-elle plus rapide ? Dans quels cas échoue-t-elle à résoudre la grille entièrement sans aide du backtracking ?
#
# Victor CORBEL - Temeio HARAPOI-GAUDIN / 1B1

def mesurer_temps(solveur, indices_lignes, indices_colonnes):
    """
    Mesure le temps d'exécution d'un solveur sur un Picross donné.
    solveur : fonction de résolution (ex: backtracking ou solveur_hybride)
    indices_lignes : indices des lignes du Picross
    indices_colonnes : indices des colonnes du Picross
    Retourne le temps d'exécution en secondes et un booléen indiquant si une solution a été trouvée.
    """
    debut = time.perf_counter()
    solution = solveur(indices_lignes, indices_colonnes)
    fin = time.perf_counter()
    if solution is not None:
        return fin - debut, True


def benchmark_taille():
    """
    Benchmark de temps en fonction de la taille du Picross.
    Teste les tailles 5x5, 10x10 et 15x15 avec une grille en forme de croix.
    Retourne une liste de résultats contenant la taille, les temps pour chaque solveur et si la solution est correcte.
    """
    tailles = [5, 10, 15]
    resultats = []

    for taille in tailles:
        indices_lignes, indices_colonnes = genere_croix(taille)

        temps_backtracking, ok_backtracking = mesurer_temps(backtracking, indices_lignes, indices_colonnes)
        temps_hybride, ok_hybride = mesurer_temps(solveur_hybride, indices_lignes, indices_colonnes)

        resultats.append(
            {
                "taille": taille,
                "temps_backtracking": temps_backtracking,
                "temps_hybride": temps_hybride,
                "ok_backtracking": ok_backtracking,
                "ok_hybride": ok_hybride,
            }
        )

    return resultats


def generer_grille_aleatoire(taille, densite, generateur):
    """
    Génère une grille aléatoire de Picross.
    taille : dimension de la grille (taille x taille)
    densite : proportion de cases noires (entre 0 et 1)
    generateur : instance de random.Random pour la reproductibilité
    Retourne une grille de 0 (blanc) et 1 (noir).
    """
    grille = []
    for _ in range(taille):
        ligne = []
        for _ in range(taille):
            case = 1 if generateur.random() < densite else 0
            ligne.append(case)
        grille.append(ligne)
    return grille


def benchmark_densite():
    """
    Benchmark de temps en fonction de la densité de cases noires.
    Teste une grille aléatoire de taille 10x10 avec des densités de 0.1, 0.3, 0.5, 0.7 et 0.9.
    Retourne une liste de résultats contenant la densité, les temps pour chaque solveur et si la solution est correcte.
    """
    taille = 10
    densites = [0.1, 0.3, 0.5, 0.7, 0.9]
    nombre_essais = 3
    generateur = random.Random(2026)

    resultats = []

    for densite in densites:
        liste_backtracking = []
        liste_hybride = []

        for _ in range(nombre_essais):
            grille = generer_grille_aleatoire(taille, densite, generateur)
            indices_lignes, indices_colonnes = indices_depuis_solution(grille)

            temps_backtracking, _ = mesurer_temps(backtracking, indices_lignes, indices_colonnes)
            temps_hybride, _ = mesurer_temps(solveur_hybride, indices_lignes, indices_colonnes)

            liste_backtracking.append(temps_backtracking)
            liste_hybride.append(temps_hybride)

        moyenne_backtracking = sum(liste_backtracking) / len(liste_backtracking)
        moyenne_hybride = sum(liste_hybride) / len(liste_hybride)

        resultats.append(
            {
                "densite": densite,
                "temps_backtracking": moyenne_backtracking,
                "temps_hybride": moyenne_hybride,
            }
        )

    return resultats

#===========================
#|        AFFICHAGE        |
#===========================

def afficher_resultats_taille(resultats):
    print("")
    print(" ============================================")
    print("| Benchmark : temps en fonction de la taille |")
    print(" ============================================")
    print("")
    for resultat in resultats:
        print(
            f"N = {resultat['taille']} | "
            f"Backtracking = {resultat['temps_backtracking']:.6f}s | "
            f"Hybride = {resultat['temps_hybride']:.6f}s"
        )


def afficher_resultats_densite(resultats):
    print("")
    print(" =============================================")
    print("| Benchmark : temps en fonction de la densité |")
    print(" =============================================")
    print("")
    for resultat in resultats:
        print(
            f"Densité = {resultat['densite']:.1f} | "
            f"Backtracking = {resultat['temps_backtracking']:.6f}s | "
            f"Hybride = {resultat['temps_hybride']:.6f}s"
        )


def afficher_analyse_complexite():
    print("")

    print(" ========================")
    print("| Analyse de complexité  |")
    print(" ========================")

    print("")

    print("Backtracking pur :")
    print("1. On teste 2 choix par case (noir ou blanc).")
    print("2. En pire cas, on explore presque tout l'arbre des possibilités.")
    print("3. Complexite temporelle théorique : O(2^(N x M)).")
    print("4. Complexite mémoire : O(N x M) pour la grille et la récursion.")

    print("")

    print("Approche propagation + backtracking :")
    print("1. La propagation déduit d'abord des cases certaines.")
    print("2. Elle réduit souvent le nombre de choix à explorer ensuite.")
    print("3. Le coût de propagation existe, mais elle coupe des branches inutiles.")


def afficher_discussion():
    print("")

    print(" ============")
    print("| Discussion |")
    print(" ============")

    print("Pourquoi la propagation peut être plus rapide :")
    print("1. Elle élimine tôt des configurations impossibles.")
    print("2. Elle diminue la profondeur utile du backtracking.")

    print("")

    print("Pourquoi elle peut parfois sembler moins rapide :")
    print("1. Sur des grilles très simples, le backtracking trouve vite une solution.")
    print("2. Le calcul des possibilités de lignes ajoute un surcoût fixe.")

    print("")

    print("Quand la propagation seule ne suffit pas :")
    print("1. Quand plusieurs placements restent compatibles localement.")
    print("2. Quand la grille est ambiguë ou très symétrique.")
    print("-> Dans ce cas, il faut faire une hypothèse puis faire le backtracking.")


if __name__ == "__main__":
    resultats_taille = benchmark_taille()
    resultats_densite = benchmark_densite()

    afficher_resultats_taille(resultats_taille)
    afficher_resultats_densite(resultats_densite)
    afficher_analyse_complexite()
    afficher_discussion()

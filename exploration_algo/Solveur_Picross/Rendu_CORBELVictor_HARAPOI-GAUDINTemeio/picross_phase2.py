import picross_phase1

est_valide = picross_phase1.est_valide

# Solveur de Picross
# Phase 2 : Approche "Force Brute" (Backtracking Naïf)
#
# Victor CORBEL - Temeio HARAPOI-GAUDIN / 1B1

def peut_etre_valide(partiel, indice, long_totale)-> bool:
    """
    Vérifie si une ligne partielle peut encore être complétée pour respecter les indices.
    partiel : partie déjà remplie de la ligne.
    indice : liste des blocs attendus.
    long_totale : longueur totale de la ligne.
    Retourne True si la ligne partielle peut encore être complétée pour respecter les indices, False sinon.
    """
    ligne = partiel + [-1] * (long_totale - len(partiel))
    memoire = {}

    def peut_commencer_a_calculer(position, indice_courant, bloc_courant):
        cle = (position, indice_courant, bloc_courant)
        if cle in memoire:
            return memoire[cle]

        if position == long_totale:
            if bloc_courant > 0:
                resultat = (
                    indice_courant < len(indice)
                    and bloc_courant == indice[indice_courant]
                    and indice_courant + 1 == len(indice)
                )
            else:
                resultat = indice_courant == len(indice)
            memoire[cle] = resultat
            return resultat

        case = ligne[position]

        if case in (-1, 0):
            if bloc_courant == 0:
                resultat = peut_commencer_a_calculer(position + 1, indice_courant, 0)
            elif indice_courant < len(indice) and bloc_courant == indice[indice_courant]:
                resultat = peut_commencer_a_calculer(position + 1, indice_courant + 1, 0)
            else:
                resultat = False
            if resultat:
                memoire[cle] = True
                return True

        if case in (-1, 1):
            if bloc_courant == 0:
                resultat = indice_courant < len(indice) and peut_commencer_a_calculer(
                    position + 1, indice_courant, 1
                )
            else:
                resultat = indice_courant < len(indice) and bloc_courant < indice[indice_courant] and peut_commencer_a_calculer(
                    position + 1, indice_courant, bloc_courant + 1
                )
            memoire[cle] = resultat
            return resultat

        memoire[cle] = False
        return False

    return peut_commencer_a_calculer(0, 0, 0)

def backtracking(indice_l, indice_c, ligne=0, colonne=0) -> list[list[int]]:
    """
    Résout le Picross en appliquant un algorithme de backtracking.
    Retourne une solution complète (grille de 0 et 1) ou None si aucune solution n'existe.
    """
    nombre_lignes = len(indice_l)
    nombre_colonnes = len(indice_c)
    nombre_total_de_cases = nombre_lignes * nombre_colonnes
    solution = [[0 for _ in range(nombre_colonnes)] for _ in range(nombre_lignes)]

    def resoudre(position):
        if position == nombre_total_de_cases:
            for numero_ligne in range(nombre_lignes):
                if not est_valide(solution[numero_ligne], indice_l[numero_ligne]):
                    return False
            for numero_colonne in range(nombre_colonnes):
                colonne_courante = [solution[numero_ligne][numero_colonne] for numero_ligne in range(nombre_lignes)]
                if not est_valide(colonne_courante, indice_c[numero_colonne]):
                    return False
            return True

        numero_ligne, numero_colonne = divmod(position, nombre_colonnes)

        for valeur in (0, 1):
            solution[numero_ligne][numero_colonne] = valeur

            ligne_partielle = solution[numero_ligne][: numero_colonne + 1]
            colonne_partielle = [solution[ligne_index][numero_colonne] for ligne_index in range(numero_ligne + 1)]

            if peut_etre_valide(ligne_partielle, indice_l[numero_ligne], nombre_colonnes) and peut_etre_valide(
                colonne_partielle, indice_c[numero_colonne], nombre_lignes
            ):
                if resoudre(position + 1):
                    return True

        solution[numero_ligne][numero_colonne] = 0
        return False
    
    if resoudre(0):
        return solution
    else:
        return None

def indices_depuis_solution(solution):
    """
    Calcule les indices d'une grille solution.
    """
    nombre_lignes = len(solution)
    nombre_colonnes = len(solution[0])

    def extraire_indices(ligne):
        indices = []
        longueur_bloc = 0
        for valeur in ligne:
            if valeur == 1:
                longueur_bloc += 1
            elif longueur_bloc > 0:
                indices.append(longueur_bloc)
                longueur_bloc = 0
        if longueur_bloc > 0:
            indices.append(longueur_bloc)
        return indices

    indices_lignes = [extraire_indices(solution[numero_ligne]) for numero_ligne in range(nombre_lignes)]
    indices_colonnes = [extraire_indices([solution[numero_ligne][numero_colonne] for numero_ligne in range(nombre_lignes)]) for numero_colonne in range(nombre_colonnes)]
    return indices_lignes, indices_colonnes


def genere_croix(taille):
    """
    Génère une grille de test en forme de croix.
    """
    solution = [[0 for _ in range(taille)] for _ in range(taille)]
    for index in range(taille):
        solution[index][index] = 1
        solution[index][taille - 1 - index] = 1
    return indices_depuis_solution(solution)

#===========================
#|        AFFICHAGE        |
#===========================

def test_phase2():
    """
    Teste le backtracking sur une grille de 5x5, 10x10 et 15x15.
    """
    import time

    print("")
    print("  =============================")
    print(" | Phase 2 : Backtracking naif |")
    print("  =============================")
    print("")
    for taille in [5, 10, 15]:
        indices_lignes, indices_colonnes = genere_croix(taille)
        debut = time.time()
        solution = backtracking(indices_lignes, indices_colonnes)
        temps = time.time() - debut
        if solution:
            etat = "OK"
        else:
            etat = "ECHEC"
        print(f"{taille}x{taille} : {etat}, {temps:.3f}s")

    print("Complexité théorique : O(2^(N x M)) dans le pire des cas")


if __name__ == "__main__":
    indices_lignes = [[1, 1, 1, 3], [3, 1, 1], [1, 2, 2], [1, 2, 1, 2], [3, 2], [1, 2], [1, 2, 1], [2, 3, 1], [10], [1, 3]]
    indices_colonnes = [[1, 2, 2], [1, 1, 1, 2], [1, 2, 3], [2, 2, 1, 1], [2, 2], [1, 2, 1, 3], [1, 5], [1, 2], [1, 3, 1], [5, 2]]

    solution = backtracking(indices_lignes, indices_colonnes)
    if solution:
        print("")
        print("Solution trouvée :")
        print("")
        for ligne in solution:
            print(ligne)
    else:
        print("Aucune solution trouvée")

    test_phase2()
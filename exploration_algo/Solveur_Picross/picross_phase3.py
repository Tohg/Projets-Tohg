import picross_phase2

# Fonctions conservées des phases précédentes
est_valide = picross_phase2.est_valide
peut_etre_valide = picross_phase2.peut_etre_valide
backtracking = picross_phase2.backtracking

INCONNU = -1
BLANC = 0
NOIR = 1

# Solveur de Picross
# Phase 3 : Approche "Intelligente" (Propagation de Contraintes)
#
# Victor CORBEL - Temeio HARAPOI-GAUDIN / 1B1

def copier_grille(grille)-> list[list[int]]:
	"""
	Retourne une copie de la grille.
	"""
	return [ligne[:] for ligne in grille]


def extraire_colonne(grille, numero_colonne)-> list[int]:
	"""
	Extrait une colonne de la grille.
	Retourne une liste de 0 et 1 représentant la colonne.
	"""
	return [grille[numero_ligne][numero_colonne] for numero_ligne in range(len(grille))]


def generer_lignes_possibles(longueur_ligne, indices)-> list[list[int]]:
	"""
	Génère toutes les lignes complètes valides pour une longueur et des indices.
	Retourne une liste de lignes possibles, où chaque ligne est une liste de 0 et 1.
	"""
	if not indices:
		return [[BLANC] * longueur_ligne]

	lignes_possibles = []

	def construire(index_indice, position_minimale, ligne_courante):
		if index_indice == len(indices):
			ligne_finale = ligne_courante + [BLANC] * (longueur_ligne - len(ligne_courante))
			lignes_possibles.append(ligne_finale)
			return

		blocs_restants = indices[index_indice:]
		taille_min_restante = sum(blocs_restants) + (len(blocs_restants) - 1)
		derniere_position_depart = longueur_ligne - taille_min_restante

		for position_depart in range(position_minimale, derniere_position_depart + 1):
			nouvelle_ligne = ligne_courante + [BLANC] * (position_depart - len(ligne_courante))
			nouvelle_ligne += [NOIR] * indices[index_indice]

			prochaine_position = len(nouvelle_ligne)
			if index_indice < len(indices) - 1:
				nouvelle_ligne.append(BLANC)
				prochaine_position += 1

			construire(index_indice + 1, prochaine_position, nouvelle_ligne)

	construire(0, 0, [])
	return lignes_possibles


def ligne_compatible_avec_modele(ligne_partielle, ligne_complete)-> bool:
	"""
	Vérifie qu'une ligne partielle (-1, 0, 1) est compatible avec une ligne complète (0, 1).
	Retourne True si la ligne partielle est compatible avec la ligne complète, False sinon.
    """
	for valeur_partielle, valeur_complete in zip(ligne_partielle, ligne_complete):
		if valeur_partielle != INCONNU and valeur_partielle != valeur_complete:
			return False
	return True


def lignes_compatibles(ligne_partielle, indices)-> list[list[int]]:
	"""
	Renvoie toutes les lignes complètes compatibles avec la ligne partielle donnée.
	ligne_partielle : liste de -1 (inconnu), 0 (blanc) et 1 (noir)
    indices : liste d'entiers représentant les blocs attendus
	Retourne une liste de lignes complètes compatibles, où chaque ligne est une liste de 0 et 1.
	"""
	toutes_les_lignes = generer_lignes_possibles(len(ligne_partielle), indices)
	return [ligne for ligne in toutes_les_lignes if ligne_compatible_avec_modele(ligne_partielle, ligne)]


def deduire_cases_certaines_ligne(ligne_partielle, indices):
	"""
	Déduit les cases certaines d'une ligne à partir des possibilités restantes.
	Retourne (nouvelle_ligne, a_change) ou (None, False) en cas de contradiction.
	"""
	possibilites = lignes_compatibles(ligne_partielle, indices)
	if not possibilites:
		return None, False

	nouvelle_ligne = ligne_partielle[:]
	a_change = False

	for position in range(len(ligne_partielle)):
		valeur_reference = possibilites[0][position]
		if all(modele[position] == valeur_reference for modele in possibilites):
			if nouvelle_ligne[position] == INCONNU:
				nouvelle_ligne[position] = valeur_reference
				a_change = True

	return nouvelle_ligne, a_change


def grille_complete(grille)-> bool:
	"""
	Vérifie qu'il n'y a plus de case inconnue.
	Retourne True si la grille est complète, False sinon.
	"""
	for ligne in grille:
		if INCONNU in ligne:
			return False
	return True


def grille_valide(grille, indices_lignes, indices_colonnes)-> bool:
	"""
	Vérifie toutes les contraintes en fin de résolution.
	Retourne True si la grille est valide, False sinon.
	"""
	for numero_ligne in range(len(indices_lignes)):
		if not est_valide(grille[numero_ligne], indices_lignes[numero_ligne]):
			return False

	for numero_colonne in range(len(indices_colonnes)):
		colonne = extraire_colonne(grille, numero_colonne)
		if not est_valide(colonne, indices_colonnes[numero_colonne]):
			return False

	return True


def propagation_contraintes(grille, indices_lignes, indices_colonnes)-> list[list[int]] | None:
	"""
	Fait les déductions sûres sur lignes et colonnes.
	Retourne la grille propagée, ou None si contradiction.
	"""
	nombre_lignes = len(indices_lignes)
	nombre_colonnes = len(indices_colonnes)

	changement = True
	while changement:
		changement = False

		for numero_ligne in range(nombre_lignes):
			ligne_deduite, a_change = deduire_cases_certaines_ligne(grille[numero_ligne], indices_lignes[numero_ligne])
			if ligne_deduite is None:
				return None
			if a_change:
				grille[numero_ligne] = ligne_deduite
				changement = True

		for numero_colonne in range(nombre_colonnes):
			colonne_actuelle = extraire_colonne(grille, numero_colonne)
			colonne_deduite, a_change = deduire_cases_certaines_ligne(colonne_actuelle, indices_colonnes[numero_colonne])
			if colonne_deduite is None:
				return None
			if a_change:
				for numero_ligne in range(nombre_lignes):
					grille[numero_ligne][numero_colonne] = colonne_deduite[numero_ligne]
				changement = True

	return grille


def solveur_hybride(indices_lignes, indices_colonnes)-> list[list[int]] | None:
	"""
	Résolution hybride:
	1. Propagation de contraintes autant que possible.
	2. Si blocage, backtracking sur une case inconnue.
	Retourne une solution complète (grille de 0 et 1) ou None si aucune solution n'existe.
	"""
	nombre_lignes = len(indices_lignes)
	nombre_colonnes = len(indices_colonnes)
	grille_initiale = [[INCONNU for _ in range(nombre_colonnes)] for _ in range(nombre_lignes)]

	def resoudre(grille):
		grille = propagation_contraintes(copier_grille(grille), indices_lignes, indices_colonnes)
		if grille is None:
			return None

		if grille_complete(grille):
			return grille if grille_valide(grille, indices_lignes, indices_colonnes) else None

		for numero_ligne in range(nombre_lignes):
			for numero_colonne in range(nombre_colonnes):
				if grille[numero_ligne][numero_colonne] == INCONNU:
					for valeur_test in (NOIR, BLANC):
						nouvelle_grille = copier_grille(grille)
						nouvelle_grille[numero_ligne][numero_colonne] = valeur_test

						ligne_test = nouvelle_grille[numero_ligne]
						if not lignes_compatibles(ligne_test, indices_lignes[numero_ligne]):
							continue

						colonne_test = extraire_colonne(nouvelle_grille, numero_colonne)
						if not lignes_compatibles(colonne_test, indices_colonnes[numero_colonne]):
							continue

						solution = resoudre(nouvelle_grille)
						if solution is not None:
							return solution
					return None
		return None

	return resoudre(grille_initiale)


def afficher_ligne(ligne)-> str:
	"""
	Affichage lisible d'une ligne: # pour noir, . pour blanc, ? pour inconnu.
	Retourne une chaîne de caractères représentant la ligne.
	"""
	conversion = {NOIR: "#", BLANC: ".", INCONNU: "?"}
	resultat = ""
	for valeur in ligne:
		resultat += conversion[valeur]
	return resultat

#===========================
#|        AFFICHAGE        |
#===========================

def test_phase3():
	"""
	Test de la phase 3.
	"""
	print("")
	print("  ======================================")
	print(" | Phase 3 : Propagation de Contraintes |")
	print("  ======================================")
	print("")

	# Exemple demandé : ligne de 10 cases, indice [8]
	ligne_test = [INCONNU] * 10
	ligne_deduite, _ = deduire_cases_certaines_ligne(ligne_test, [8])
	print("Exemple ligne de 10 cases, indice = 8") # les # sont les 6 cases sûres
	print("Avant :", afficher_ligne(ligne_test))
	print("Après :", afficher_ligne(ligne_deduite))

	# Test solveur hybride sur un exemple 10x10 (reprend les indices de la phase 2)
	indices_lignes = [[1, 1, 1, 3], [3, 1, 1], [1, 2, 2], [1, 2, 1, 2], [3, 2], [1, 2], [1, 2, 1], [2, 3, 1], [10], [1, 3]]
	indices_colonnes = [[1, 2, 2], [1, 1, 1, 2], [1, 2, 3], [2, 2, 1, 1], [2, 2], [1, 2, 1, 3], [1, 5], [1, 2], [1, 3, 1], [5, 2]]

	solution = solveur_hybride(indices_lignes, indices_colonnes)
	if solution:
		print("")
		print("Solution hybride trouvée :")
		for ligne in solution:
			print(ligne)
	else:
		print("")
		print("Aucune solution trouvée avec l'approche hybride.")


if __name__ == "__main__":
	test_phase3()

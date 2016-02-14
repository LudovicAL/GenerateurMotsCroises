#include "Bd.h"
#include "Grille.h"
#include "Utils.h"
#include <algorithm>
#include <iostream>

void recupereDefinitions(Grille &grille, Bd &bd) {
	for (unsigned int i = 0; i < grille.nbChamps; i++) {
		grille.listeDesChamps[i]->listeDesDefinitions = bd.donneVecteurDefinitions(grille.listeDesChamps[i]->getMot());
	}
}

bool remplirGrilleLoop(Grille &grille, Bd &bd) {
	Champ* champARemplir = grille.listeDesChamps[0];
	unsigned int noDuChampARemplir = 0;
	bool impossible = false;
	while (champARemplir != NULL && !impossible) {
		if (champARemplir->nbMotsPossibles == 0 && !champARemplir->listeInitialisee) {
			champARemplir->listeInitialisee = true;
			champARemplir->listeDesMotsPossibles = bd.donneVecteurMotsPossibles(champARemplir);
			champARemplir->nbMotsPossibles = champARemplir->listeDesMotsPossibles.size();
			if (champARemplir->nbMotsPossibles > 0) {	//Si la liste des mots possible n'est plus vide
				std::random_shuffle(champARemplir->listeDesMotsPossibles.begin(), champARemplir->listeDesMotsPossibles.end());
				if (champARemplir->nbMotsPossibles > 100) {
					champARemplir->listeDesMotsPossibles.erase(champARemplir->listeDesMotsPossibles.begin() + 100, champARemplir->listeDesMotsPossibles.end());
					champARemplir->nbMotsPossibles = champARemplir->listeDesMotsPossibles.size();
				}
				for (int i = 0; i < champARemplir->nbMotsPossibles; i++) {
					champARemplir->ecrireMot(champARemplir->listeDesMotsPossibles[i]);
					champARemplir->listeDesMotsPossibles[i].score = grille.existentMotsPerpendiculaires(champARemplir, bd);
					grille.effacerMot(champARemplir);
				}
				std::sort(champARemplir->listeDesMotsPossibles.begin(), champARemplir->listeDesMotsPossibles.end());
			}
		}
		if (champARemplir->nbMotsPossibles > 0 && champARemplir->listeDesMotsPossibles[0].score > 0 && champARemplir->nbEssais < 7) {
			champARemplir->nbEssais++;
			champARemplir->ecrireMot(champARemplir->listeDesMotsPossibles[0]);
			champARemplir->listeDesMotsPossibles.erase(champARemplir->listeDesMotsPossibles.begin());
			champARemplir->nbMotsPossibles = champARemplir->listeDesMotsPossibles.size();
			noDuChampARemplir++;
			grille.calculeContrainteDesChampsLoop(champARemplir, noDuChampARemplir);
		} else {
			if (noDuChampARemplir > 0) {
				grille.effacerMot(champARemplir);
				champARemplir->nbEssais = 0;
				champARemplir->listeDesMotsPossibles.clear();
				champARemplir->nbMotsPossibles = 0;
				champARemplir->listeInitialisee = false;
				grille.calculeContrainteDesChampsLoop(champARemplir, noDuChampARemplir);
				noDuChampARemplir--;
				if (noDuChampARemplir < 4) {
					std::cout << "Warning" << std::endl;
				}
			} else {
				impossible = true;
			}
		}
		champARemplir = (noDuChampARemplir < grille.nbChamps) ? grille.listeDesChamps[noDuChampARemplir] : NULL;
		std::cout << grille.lireGrille() << "---------------------------" << std::endl;
	}
	return !impossible;
}

bool remplirGrille(Grille &grille, Bd &bd, int noDuChampARemplir) {
	Champ* champARemplir = grille.listeDesChamps[noDuChampARemplir];
	if (champARemplir != NULL) {
		std::vector<Mot> vecteurMots = bd.donneVecteurMotsPossibles(champARemplir);
		if (vecteurMots.size() > 0) {
			std::random_shuffle(vecteurMots.begin(), vecteurMots.end());
			if (vecteurMots.size() > 100) {
				vecteurMots.erase(vecteurMots.begin() + 100, vecteurMots.end());
			}
			int essais = 0;
			int tailleVecteur = vecteurMots.size();
			for (int i = 0; i < tailleVecteur; i++) {
				champARemplir->ecrireMot(vecteurMots[i]);
				vecteurMots[i].score = grille.existentMotsPerpendiculaires(champARemplir, bd);
				if (vecteurMots[i].score / vecteurMots[i].nbLettres > 200 && essais < 3) {
					essais++;
					std::cout << grille.lireGrille() << "---------------------------" << std::endl;
					grille.calculeContrainteDesChamps(champARemplir, noDuChampARemplir);
					if (remplirGrille(grille, bd, noDuChampARemplir + 1)) {
						return true;
					}
					vecteurMots[i].score = 0;
				}
				grille.effacerMot(champARemplir);
			}
			std::sort(vecteurMots.begin(), vecteurMots.end());
			for (int i = 0; i < tailleVecteur && i < 10 && vecteurMots[i].score > 0; i++) {
				champARemplir->ecrireMot(vecteurMots[i]);
				std::cout << grille.lireGrille() << "---------------------------" << std::endl;
				grille.calculeContrainteDesChamps(champARemplir, noDuChampARemplir);
				if (remplirGrille(grille, bd, noDuChampARemplir + 1)) {
					return true;
				} else {
					grille.effacerMot(champARemplir);
				}
			}
		} else {
			grille.effacerMot(grille.trouveDernierChampRempli());
			return false;
		}
	} else {
		return true;
	}
	return false;
}

void creerFichier(Grille &grille, std::string fileName) {
	std::ofstream myFile;
	const char* f = fileName.c_str();
	myFile.open (f);
	std::string resultat = "";
	resultat += grille.lireGrille();
	resultat += grille.lireDefinitions();
	myFile << resultat;
	myFile.close();
}

//PROGRAMME PRINCIPAL
int main(int argc, char *argv[]) {
	//Initialisation du générateur de nombres aléatoires
	initialisationDUtils();

	//Génération d'une grille vide (avec ses cases noires)
	Grille grille("grillesVides\\12;12\\grille1.txt");
	unsigned int tailleMotMax = grille.nbChampsParTaille.size();
	std::cout << grille.lireGrille() << "---------------------------" << std::endl;

	//Ouverture des banques de mots
	Bd bd(2, tailleMotMax);

	//Remplissage de la grille
	if (remplirGrilleLoop(grille, bd)) {
		std::cout << grille.lireGrille() << "---------------------------" << std::endl;
	} else {
		std::cout << grille.lireGrille() << "---------------------------" << std::endl;
		std::cout << "Impossible de remplir la grille" << std::endl;
	}

	//Fermeture des banques de mots
	bd.fermeLesBd(2, tailleMotMax);

	//Ouverture de la banque de mot avec défintions
	bd.ouvreLesBd(0, 0);

	//Récupération des définitions
	recupereDefinitions(grille, bd);
	std::cout << grille.lireDefinitions() << "---------------------------" << std::endl;

	//Créer le fichier
	creerFichier(grille, "grillesPleines/grille1.txt");

	//Fermeture de la banque de mot avec définition
	bd.fermeLesBd(0, 0);

	//Fermeture du programme
	system("pause");
	return 0;
}

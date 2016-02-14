#include "Grille.h"
#include <sstream>
#include <fstream>
#include <iostream>
#include <algorithm>
#include "Utils.h"

bool trieuse(Champ *lhs, Champ *rhs) {
	return lhs->contrainteLexicale > rhs->contrainteLexicale;
}

Grille::Grille(std::string nomFichier) {
	genereGrilleVideDepuisFichier(nomFichier);
	calculeContraintesDesCases();
	recupereChampsHorizontauxDeGrille();
	recupereChampsVerticauxDeGrille();
	calculeNbChamps();
	calculeNbCasesNoires();
	recupereChampsPerpendiculaires();
	calculeContrainteDesChamps();
}

void Grille::calculeNbCasesNoires() {
	nbCasesNoires = 0;
	for (unsigned int i = 0; i < largeur; i++) {
		for (unsigned int j = 0; j < hauteur; j++) {
			if (donnees[i + j * largeur]->caseNoire) {
				nbCasesNoires++;
			}
		}
	}
}

//MÉTHODE CALCULANT LE NOMBRE DE CHAMPS
void Grille::calculeNbChamps() {
	nbChamps = listeDesChamps.size();
	for (unsigned int i = 0; i < nbChamps; i++) {	//Pour calculer le nombre de champs par taille
		unsigned int tailleMot = listeDesChamps[i]->nbLettres;
		while (nbChampsParTaille.size() < tailleMot) {
			nbChampsParTaille.push_back(0);
		}
		nbChampsParTaille[tailleMot]++;
	}
	for (unsigned int i = 0; i < nbChamps; i++) {	//Pour calculer le nombre de champs par sens
		if (listeDesChamps[i]->horizontal) {
			nbChampsHorizontaux++;
		} else {
			nbChampsVerticaux++;
		}
	}
}

//MÉTHODE GÉNÉRANT UNE GRILLE VIDE, AVEC SES CASES NOIRES
void Grille::genereGrilleVideDepuisFichier(std::string nomFichier) {
	hauteur = 0;
	largeur = 0;
	std::ifstream docGrille;
	std::ostringstream fileName;
	fileName << nomFichier;
	docGrille.open(fileName.str().c_str(), std::ios::in|std::ios::binary);
	if (docGrille.is_open()) {
		std::string contenuLigne;
		docGrille.seekg(0, docGrille.beg);	//Pour placer le chariot au début du document
		while (std::getline(docGrille, contenuLigne), contenuLigne.length() != 0) {
			largeur = contenuLigne.length() - 1;
			for (unsigned int i = 0; i < largeur; i++) {
				donnees.push_back(new Case(i, hauteur, contenuLigne[i]));
			}
			hauteur++;
		}
		docGrille.clear();	//Pour nettoyer les drapeaux levés lors de la lecture du fichier
	}
	docGrille.close();
}

//MÉTHODE RÉCUPÉRANT LES CHAMPS HORIZONTAUX DE LA GRILLE
void Grille::recupereChampsHorizontauxDeGrille() {
	for (unsigned int i = 0; i < hauteur; i++) {
		for (unsigned int j = 0; j < largeur; j++) {
			std::vector<Case*> mot;
			while (j < largeur && !donnees[j + i * largeur]->caseNoire) {
				mot.push_back(donnees[j + i * largeur]);
				j++;
			}
			if (mot.size() > 1) {
				listeDesChamps.push_back(new Champ(mot, true, listeDesChamps.size()));
				listeDesChampsHorizontaux.push_back(listeDesChamps.back());
			}
		}
	}
}

//MÉTHODE RÉCUPÉRANT LES CHAMPS VERTICAUX DE LA GRILLE
void Grille::recupereChampsVerticauxDeGrille() {
	for (unsigned int i = 0; i < largeur; i++) {
		for (unsigned int j = 0; j < hauteur; j++) {
			std::vector<Case*> mot;
			while (j < hauteur && !donnees[i + j * largeur]->caseNoire) {
				mot.push_back(donnees[i + j * largeur]);
				j++;
			}
			if (mot.size() > 1) {
				listeDesChamps.push_back(new Champ(mot, false, listeDesChamps.size()));
				listeDesChampsVerticaux.push_back(listeDesChamps.back());
			}
		}
	}
}

//MÉTHODE CALCULANT LA CONTRAINTE D'UNE CASE
void Grille::calculeContraintesDesCases() {
	for (unsigned int i = 0; i < largeur; i++) {
		for (unsigned int j = 0; j < hauteur; j++) {
			if (!donnees[i + j * largeur]->caseNoire) {
				unsigned int compteur = 0;
				//Vers le haut
				compteur += calculeContrainteVerticale(i, j, -1);
				//Vers le bas
				compteur += calculeContrainteVerticale(i, j, 1);
				//Vers la gauche
				compteur += calculeContrainteHorizontale(i, j, -1);
				//Vers la droite
				compteur += calculeContrainteHorizontale(i, j, 1);
				donnees[i + j * largeur]->contrainte = compteur;
			}
		}
	}
}

//MÉTHODE CALCULANT LA CONTRAINTE HORIZONTALE D'UNE CASE
int Grille::calculeContrainteHorizontale(unsigned int i, unsigned int j, int signe) {
	int compteur = 0;
	i += signe;
	if (i >= 0 && i < largeur && !donnees[i + j * largeur]->caseNoire) {
		compteur++;
		while (i >= 0 && i < largeur && !donnees[i + j * largeur]->caseNoire) {
			compteur++;
			i += signe;
		}
	}
	return compteur;
}

//MÉTHODE CALCULANT LA CONTRAINTE VERTICALE D'UNE CASE
int Grille::calculeContrainteVerticale(unsigned int i, unsigned int j, int signe) {
	int compteur = 0;
	j += signe;
	if (j >= 0 && j < hauteur && !donnees[i + j * largeur]->caseNoire) {
		compteur++;
		while (j >= 0 && j < hauteur && !donnees[i + j * largeur]->caseNoire) {
			compteur++;
			j += signe;
		}
	}
	return compteur;
}

//MÉTHODE TROUVANT LE PROCHAIN CHAMP À REMPLIR (LE PROCHAIN CHAMP LE PLUS CONTRAINT)
Champ* Grille::trouveProchainChampARemplir() {
	for (unsigned int i = 0; i < nbChamps; i++) {
		if (!listeDesChamps[i]->rempli) {
			return listeDesChamps[i];
		}
	}
	return NULL;
}

//MÉTHODE TROUVANT LE DERNIER CHAMP À REMPLI
Champ* Grille::trouveDernierChampRempli() {
	Champ* champTempo = NULL;
	for (unsigned int i = 0; i < nbChamps; i++) {
		if (listeDesChamps[i]->rempli) {
			champTempo = listeDesChamps[i];
		} else {
			break;
		}
	}
	return champTempo;
}

//MÉTHODE AFFICHANT LA GRILLE DANS LA CONSOLE
std::string Grille::lireGrille() {
	std::string resultat = "";
	unsigned int fin = this->largeur * this->hauteur;
	for (unsigned int i = 0; i < fin; ++i) {
		//resultat += '|';
		resultat += donnees[i]->contenu;
		if (i % this->largeur == (this->largeur - 1)) //À chaque fois qu'on arrive au bout d'une ligne, on ajoute un retour de chariot
			resultat += '\n';
	}
	return resultat;
}

//MÉTHODE AFFICHANT LES DÉFINITIONS DANS LA CONSOLE
std::string Grille::lireDefinitions() {
	std::string resultat;
	resultat = "HORIZONTAUX\n--------------------------\n";
	for (unsigned int i = 0, nbChampsHorizontaux = listeDesChampsHorizontaux.size(); i < nbChampsHorizontaux; ++i) {
		resultat += intToString(listeDesChampsHorizontaux[i]->xDebut);
		resultat += ",";
		resultat += intToString(listeDesChampsHorizontaux[i]->yDebut);
		resultat += " ";
		resultat += listeDesChampsHorizontaux[i]->getMot();
		resultat += " :: ";
		for (unsigned int j = 0, nbDefinitions = listeDesChampsHorizontaux[i]->listeDesDefinitions.size(); j < nbDefinitions; j++) {
			resultat += listeDesChampsHorizontaux[i]->listeDesDefinitions[j].contenu;
			resultat += ";";
		}
		resultat += "\n";
	}
	resultat += "VERTICAUX\n--------------------------\n";
	for (unsigned int i = 0, nbChampsVerticaux = listeDesChampsVerticaux.size(); i < nbChampsVerticaux; ++i) {
		resultat += intToString(listeDesChampsVerticaux[i]->xDebut);
		resultat += ",";
		resultat += intToString(listeDesChampsVerticaux[i]->yDebut);
		resultat += " ";
		resultat += listeDesChampsVerticaux[i]->getMot();
		resultat += " :: ";
		for (unsigned int j = 0, nbDefinitions = listeDesChampsVerticaux[i]->listeDesDefinitions.size(); j < nbDefinitions; j++) {
			resultat += listeDesChampsVerticaux[i]->listeDesDefinitions[j].contenu;
			resultat += ";";
		}
		resultat += "\n";
	}
	return resultat;
}

void Grille::effacerMot(Champ* champ) {
	for (unsigned int i = 0; i < champ->nbChampsPerpendiculaires; i++) {
		if (!champ->listeDesChampsPerpendiculaires[i]->rempli) {
			champ->listeDesCases[i]->contenu = '1';
		}
	}
	for (unsigned int i = 0; i < champ->nbCasesSansPerpendiculaires; i++) {
		champ->listeDesCasesSansChampPerpendiculaire[i]->contenu = '1';
	}
	champ->rempli = false;
}

float Grille::existentMotsPerpendiculaires(Champ* champ, Bd &bd) {
	float indice = 1.1;
	for (unsigned int i = 0, nbPerpendiculaires = champ->listeDesChampsPerpendiculaires.size(); i < nbPerpendiculaires; i++) {
		if (!champ->listeDesChampsPerpendiculaires[i]->rempli) {
			std::vector<Mot> vecTempo = bd.donneVecteurMotsPossibles(champ->listeDesChampsPerpendiculaires[i]);
			if (vecTempo.size() > 0) {
				indice += vecTempo.size();
			} else {
				indice = 0.0;
				break;
			}
		}
	}
	return indice;
}

void Grille::recupereChampsPerpendiculaires() {
	for (unsigned int i = 0; i < nbChamps; i++) { //Pour tous les champs
		for (unsigned int j = 0; j < listeDesChamps[i]->nbLettres; j++) { //Pour toutes les cases d'un champs
			bool trouve = false;
			for (unsigned int k = 0; k < nbChamps && trouve == false; k++) {	//On compare à tous les autres champs qui ne sont pas du même sens
				if (listeDesChamps[i]->horizontal != listeDesChamps[k]->horizontal) {
					for (unsigned int m = 0; trouve == false && m < listeDesChamps[k]->nbLettres; m++) {	//Toutes les cases de ces champs
						if (listeDesChamps[i]->listeDesCases[j] == listeDesChamps[k]->listeDesCases[m]) {
							listeDesChamps[i]->listeDesChampsPerpendiculaires.push_back(listeDesChamps[k]);
							trouve = true;
						}
					}
				}
			}
			if (trouve == false) {
				listeDesChamps[i]->listeDesCasesSansChampPerpendiculaire.push_back(listeDesChamps[i]->listeDesCases[j]);
			}
		}
		listeDesChamps[i]->nbChampsPerpendiculaires = listeDesChamps[i]->listeDesChampsPerpendiculaires.size();
		listeDesChamps[i]->nbCasesSansPerpendiculaires = listeDesChamps[i]->listeDesCasesSansChampPerpendiculaire.size();
	}
}

void Grille::calculeContrainteDesChamps() {
	for (unsigned int i = 0; i < nbChamps; i++) {
		if (!listeDesChamps[i]->rempli) {
			listeDesChamps[i]->contrainteLexicale = listeDesChamps[i]->contrainteGeographique;
			for(unsigned int j = 0; j < listeDesChamps[i]->nbLettres; j++) {
				if (listeDesChamps[i]->listeDesCases[j]->contenu != '1') {
					listeDesChamps[i]->contrainteLexicale += 1000;
				}
			}
		}
	}
	std::sort(listeDesChamps.begin(), listeDesChamps.end(), trieuse);
}

void Grille::calculeContrainteDesChampsLoop(Champ* dernierChampRempli, unsigned int noDuChampARemplir) {
	for (unsigned int i = 0; i < dernierChampRempli->nbChampsPerpendiculaires; i++) {
		if (!dernierChampRempli->listeDesChampsPerpendiculaires[i]->rempli) {
			dernierChampRempli->listeDesChampsPerpendiculaires[i]->contrainteLexicale = dernierChampRempli->listeDesChampsPerpendiculaires[i]->contrainteGeographique;
			for(unsigned int j = 0; j < dernierChampRempli->listeDesChampsPerpendiculaires[i]->nbLettres; j++) {
				if (dernierChampRempli->listeDesChampsPerpendiculaires[i]->listeDesCases[j]->contenu != '1') {
					dernierChampRempli->listeDesChampsPerpendiculaires[i]->contrainteLexicale += 1000;
				}
			}
		}
	}
	std::sort(listeDesChamps.begin() + noDuChampARemplir, listeDesChamps.end(), trieuse);
}

void Grille::calculeContrainteDesChamps(Champ* dernierChampRempli, unsigned int noDuChampARemplir) {
	for (unsigned int i = 0; i < dernierChampRempli->nbChampsPerpendiculaires; i++) {
		if (!dernierChampRempli->listeDesChampsPerpendiculaires[i]->rempli) {
			dernierChampRempli->listeDesChampsPerpendiculaires[i]->contrainteLexicale = dernierChampRempli->listeDesChampsPerpendiculaires[i]->contrainteGeographique;
			for(unsigned int j = 0; j < dernierChampRempli->listeDesChampsPerpendiculaires[i]->nbLettres; j++) {
				if (dernierChampRempli->listeDesChampsPerpendiculaires[i]->listeDesCases[j]->contenu != '1') {
					dernierChampRempli->listeDesChampsPerpendiculaires[i]->contrainteLexicale += 1000;
				}
			}
		}
	}
	std::sort(listeDesChamps.begin() + noDuChampARemplir + 1, listeDesChamps.end(), trieuse);
}

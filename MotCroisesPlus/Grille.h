#ifndef GRILLE_H_
#define GRILLE_H_

#include <vector>
#include <string>
#include "Case.h"
#include "Bd.h"

class Champ;

class Grille {
	public:
		unsigned int largeur;
		unsigned int hauteur;
		std::vector<Case*> donnees;
		std::vector<Champ*> listeDesChamps;
		std::vector<Champ*> listeDesChampsHorizontaux;
		std::vector<Champ*> listeDesChampsVerticaux;
		std::vector<int> nbChampsParTaille;
		unsigned int nbChamps;
		unsigned int nbCasesNoires;
		unsigned int nbChampsHorizontaux;
		unsigned int nbChampsVerticaux;
	public:
		Grille(std::string nomFichier); //Constructeur
		void genereGrilleVideDepuisFichier(std::string nomFichier);
		void recupereChampsHorizontauxDeGrille();
		void recupereChampsVerticauxDeGrille();
		void calculeNbCasesNoires();
		void calculeNbChamps();
		void calculeContraintesDesCases();
		int calculeContrainteHorizontale(unsigned int i, unsigned int j, int signe);
		int calculeContrainteVerticale(unsigned int i, unsigned int j, int signe);
		Champ* trouveProchainChampARemplir();
		Champ* trouveDernierChampRempli();
		std::string lireGrille();
		std::string lireDefinitions();
		void effacerMot(Champ* champ);
		float existentMotsPerpendiculaires(Champ* champ, Bd &bd);
		void recupereChampsPerpendiculaires();
		void calculeContrainteDesChamps();
		void calculeContrainteDesChampsLoop(Champ* dernierChampRempli, unsigned int noDuChampARemplir);
		void calculeContrainteDesChamps(Champ* dernierChampRempli, unsigned int noDuChampARemplir);
		void recupereDefinitions();
};

#endif

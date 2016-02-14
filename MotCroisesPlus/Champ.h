#ifndef CHAMP_H_
#define CHAMP_H_

#include "Mot.h"
#include "Definition.h"
#include <vector>

class Case;

class Champ {
	public:
		std::vector<Case*> listeDesCases;
		std::vector<Champ*> listeDesChampsPerpendiculaires;
		std::vector<Case*> listeDesCasesSansChampPerpendiculaire;
		std::vector<Definition> listeDesDefinitions;
		std::vector<Mot> listeDesMotsPossibles;
		int nbMotsPossibles;
		bool listeInitialisee;
		unsigned int nbEssais;
		unsigned int nbLettres;
		unsigned int nbChampsPerpendiculaires;
		unsigned int nbCasesSansPerpendiculaires;
		int contrainteGeographique;
		int contrainteLexicale;
		bool horizontal;
		bool rempli;
		unsigned int xDebut;
		unsigned int yDebut;
		unsigned int xFin;
		unsigned int yFin;
	public:
		Champ(std::vector<Case*> mot, bool horizontal, int id);
		virtual ~Champ();
		bool operator < (const Champ & autre) const;	//Surcharge de l'opérateur de comparaison, utile pour faire un "sort"
		std::string getMot();
		void ecrireMot(Mot mot);
};

#endif

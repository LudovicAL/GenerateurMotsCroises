#include "Champ.h"
#include "Case.h"

Champ::Champ(std::vector<Case*> listeDesCases, bool horizontal, int id) {
	this->listeDesCases = listeDesCases;
	this->nbLettres = listeDesCases.size();
	for(unsigned int i = 0; i < nbLettres; i++) {
		listeDesCases[i]->dansUnChamp = true;
	}
	this->nbEssais = 0;
	this->contrainteLexicale = 0;
	this->contrainteGeographique = 0;
	for(unsigned int i = 0; i < nbLettres; i++) {
		contrainteGeographique += listeDesCases[i]->contrainte;
	}
	this->nbChampsPerpendiculaires = 0;
	this->nbCasesSansPerpendiculaires = 0;
	this->rempli = false;
	this->horizontal = horizontal;
	this->xDebut = listeDesCases.front()->x;
	this->yDebut = listeDesCases.front()->y;
	this->xFin = listeDesCases.back()->x;
	this->yFin = listeDesCases.back()->y;
	this->nbMotsPossibles = 0;
	this->listeInitialisee = false;
}

Champ::~Champ() {
	// TODO Auto-generated destructor stub
}

bool Champ::operator < (const Champ& autre) const {	//Surcharge de l'opérateur de comparaison, utile pour faire un "sort"
	return this->contrainteLexicale < autre.contrainteLexicale;
}

std::string Champ::getMot() {
	std::string terme = "";
	for (unsigned int i = 0; i < this->nbLettres; i++) {
		terme += this->listeDesCases[i]->contenu;
	}
	return terme;
}

void Champ::ecrireMot(Mot mot) {
	for (unsigned int i = 0; i < this->nbLettres; i++) {
		this->listeDesCases[i]->contenu = mot.strMot[i];
	}
	this->rempli = true;
}

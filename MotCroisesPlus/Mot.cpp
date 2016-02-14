#include "Mot.h"

Mot::Mot(std::string strMot) {
	this->strMot = strMot;
	this->nbLettres = this->strMot.length();
	this->score = 0.0;
}

Mot::~Mot() {
	// TODO Auto-generated destructor stub
}

int Mot::comparerScores(Mot mot1, Mot mot2) {
	if (mot1.score > mot2.score) {
		return 1;
	} else if (mot1.score < mot2.score) {
		return -1;
	} else {
		return 0;
	}
}

bool Mot::operator < (const Mot& autre) const {	//Surcharge de l'opérateur de comparaison, utile pour faire un "sort"
	return this->score > autre.score;
}

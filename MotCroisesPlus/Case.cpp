#include "Case.h"

Case::Case(int x, int y, char contenu) {
	this->x = x;
	this->y = y;
	this->contenu = contenu;
	this->contrainte = 0;
	this->caseNoire = (contenu == '0') ? true : false;
	this->dansUnChamp = false;
}

Case::~Case() {
	// TODO Auto-generated destructor stub
}

int Case::comparerContraintes(Case case1, Case case2) {
	if (case1.contrainte > case2.contrainte) {
		return 1;
	} else if (case1.contrainte < case2.contrainte) {
		return -1;
	} else {
		return 0;
	}
}

/*
 * Definition.cpp
 *
 *  Created on: 2015-11-16
 *      Author: Ludovic
 */

#include "Definition.h"

Definition::Definition(std::string contenu) {
	this->contenu = contenu;
	this->nbLettres = this->contenu.length();
}

Definition::~Definition() {
	// TODO Auto-generated destructor stub
}


#include "Utils.h"
#include <time.h>
#include <iostream>
#include <sstream>
#include <algorithm>

void initialisationDUtils() {
	time_t seed = time(NULL);
	std::cout << "Seed = " << seed << std::endl << std::endl;
	srand(seed);
	rand(); //Gaspillage du premier r�sultat g�n�r� al�atoirement, car il est trop pr�visible
}

int genererNbAleatoireEntre(int min, int max) {
	return rand() % max + min;
}

std::string intToString(int chiffre) {
	std::ostringstream convert;
	convert << chiffre;
	return convert.str();
}

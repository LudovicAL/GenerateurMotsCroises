#ifndef BD_H_
#define BD_H_

#include "Definition.h"
#include "Mot.h"
#include "Champ.h"
#include <fstream>
#include <vector>

#define ARRAY_SIZE 25

class Bd {
	public:
		std::ifstream docMots[ARRAY_SIZE];
		int nbMots[ARRAY_SIZE];
		std::vector<std::vector<long> > offsets;
		float frequencesDesLettresDansBd[26];
	public:
		Bd(unsigned int tailleMotMin, unsigned int tailleMotMax);
		virtual ~Bd();
		void ouvreLesBd(unsigned int tailleMotMin, unsigned int tailleMotMax);
		void fermeLesBd(unsigned int tailleMotMin, unsigned int tailleMotMax);
		void analyseLesFrequences(unsigned int tailleMotMin, unsigned int tailleMotMax);
		int donneNbMots(unsigned int nbLettres);
		std::vector<long> donneOffsetsMots(unsigned int nbLettres);
		float calculeScore(std::string mot);
		Mot donneMotAvecIndex(unsigned int nbLettres, int index);
		std::string donneMotAvecIndexStr(unsigned int nbLettres, int index);
		int donneIndexMot(std::string mot);
		int donneIndexMotDansDefinitions(std::string terme);
		int donnePremierIndexDeChaine(std::string chaine, unsigned int tailleChamp);
		bool comparaisonWildCard(Mot mot, Mot motTempo);
		std::vector<Mot> donneVecteurMotsPossibles(Champ* champ);
		std::vector<Definition> donneVecteurDefinitions(std::string terme);
};

#endif

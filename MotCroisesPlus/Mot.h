#ifndef MOT_H_
#define MOT_H_

#include <string>

class Mot {
public:
	std::string strMot;
	unsigned int nbLettres;
	float score;
public:
	Mot(std::string strMot);
	virtual ~Mot();
	int comparerScores(Mot mot1, Mot mot2);
	bool operator < (const Mot & autre) const;	//Surcharge de l'opérateur de comparaison, utile pour faire un "sort"
};

#endif

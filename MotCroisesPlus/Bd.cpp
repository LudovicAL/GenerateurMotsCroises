#include "Bd.h"
#include "Utils.h"
#include <sstream>
#include <time.h>
#include <algorithm>

Bd::Bd(unsigned int tailleMotMin, unsigned int tailleMotMax) {
	ouvreLesBd(tailleMotMin, tailleMotMax);
	analyseLesFrequences(tailleMotMin, tailleMotMax);
}

Bd::~Bd() {
	// TODO Auto-generated destructor stub
}

//MÉTHODE OUVRANT LES BD
void Bd::ouvreLesBd(unsigned int tailleMotMin, unsigned int tailleMotMax) {
	for(unsigned int i = 0; i < tailleMotMin; i++) {	//Insertion d'un vecteur vide à l'index 0
		std::vector<long> tempo;
		offsets.push_back(tempo);
	}
	for(unsigned int i = tailleMotMin; i <= ARRAY_SIZE && i <= tailleMotMax; i++){
		std::ostringstream fileName;
		fileName << "bd\\mots" << i << ".txt";
		docMots[i].open(fileName.str().c_str(), std::ios::in|std::ios::binary);
		//Mesure de la taille des fichiers
		if (docMots[i].is_open()) {
			nbMots[i] = donneNbMots(i);
			docMots[i].clear(); //Lorsque le chariot est arrivé à  la fin d'un fichier, des drapeau d'erreur sont levés qu'il faut maintenant abaisser avant de continuer.
			offsets.push_back(donneOffsetsMots(i));	//Initialisation du tableau des offsets
		}
	}
}

//MÉTHODE FERMANT LES BD
void Bd::fermeLesBd(unsigned int tailleMotMin, unsigned int tailleMotMax) {
	for (unsigned int i = tailleMotMin; i < ARRAY_SIZE - 1 && i < tailleMotMax; i++) {
		docMots[i].close();
	}
	offsets.clear();
}

//MÉTHODE RENVOYANT UN TABLEAU CONTENANT LES INDICES DE FRÉQUENCES D'APPARITION DE CHAQUE LETTRE DE LA BD
void Bd::analyseLesFrequences(unsigned int tailleMotMin, unsigned int tailleMotMax) {
	int lettres[26];
	int nbLettres = 0;
	//Initialisation des variables
	for(int i = 0; i < 26; i++) {
		lettres[i] = 0;
		frequencesDesLettresDansBd[i] = 0.0;
	}
	//Lecture du fichier et récupération des quantités de chaque lettres
	for(unsigned int i = tailleMotMin; i <= ARRAY_SIZE && i <= tailleMotMax; i++){
		if (docMots[i].is_open()) {
			std::string contenuLigne;
			docMots[i].seekg(0, docMots[i].beg);	//Placement du chariot au début du document
			while (std::getline(docMots[i], contenuLigne), contenuLigne.length() != 0) {
				int numero;
				for (int i = 0, max = contenuLigne.length() - 1; i < max; i++) {
					numero = ((int)(contenuLigne[i])) - 65;
					if (numero != -181) {
						lettres[numero]++;
					} else {
						lettres[4]++;
						lettres[14]++;
					}
				}
				nbLettres += (contenuLigne.length() - 1);
			}
			docMots[i].clear();
		}
	}
	//Calcul de la fréquence ajustée d'apparition de chaque lettre
	float max = 0.0;
	for(int i = 0; i < 26; i++) {
		if (((float)lettres[i] / (float)nbLettres) > max) {
			max = ((float)lettres[i] / (float)nbLettres);
		}
	}
	for(int i = 0; i < 26; i++) {
		frequencesDesLettresDansBd[i] = ((float)lettres[i] / (float)nbLettres) / max;
	}
}

//MÉTHODE COMPTANT LE NOMBRE DE MOTS DANS LA BANQUE DE MOTS
int Bd::donneNbMots(unsigned int nbLettres) {
	int compteur = 0;
	std::string contenuLigne;
	docMots[nbLettres].seekg(0, docMots[nbLettres].beg);	//Pour placer le chariot au début du document
	while (std::getline(docMots[nbLettres], contenuLigne), contenuLigne.length() != 0) {	//Incrémentation du	compteur à chaque ligne trouvée
		compteur++;
	}
	return compteur;
}

//MÉTHODE INDEXANT LES OFFSET DE TOUS LES MOTS DE LA BANQUE DE MOTS
std::vector<long> Bd::donneOffsetsMots(unsigned int nbLettres) {
	std::string contenuLigne;
	docMots[nbLettres].seekg(0, docMots[nbLettres].beg);	//Pour placer le chariot au début du document
	std::vector<long> vecTempo;
	for (int j = 0, max = nbMots[nbLettres]; j < max; j++) {	//On parcour la banque de mots, et à chaque nouvelle ligne, on enregistre la valeur du offsets du premier caractère dans le tableau de offsets à la ligne correspondante.
		vecTempo.push_back(docMots[nbLettres].tellg());
		std::getline(docMots[nbLettres], contenuLigne);
	}
	return vecTempo;
}

//MÉTHODE CALCULANT LE SCORE D'UN MOT
float Bd::calculeScore(std::string mot) {
	float scoreTempo = 0.0;
	for (int i = 0, nbLettres = mot.length(); i < nbLettres; i++) {
		int numero = ((int)(mot[i])) - 65;
		if (numero != -181) {
			scoreTempo += frequencesDesLettresDansBd[numero];
		} else {
			scoreTempo += frequencesDesLettresDansBd[4];
			scoreTempo += frequencesDesLettresDansBd[14];
		}
	}
	return (mot.length() > 0) ? scoreTempo /= mot.length() : 0.0;
}

//MÉTHODE RENVOYANT UN OBJET MOT À PARTIR DE SON INDEX
Mot Bd::donneMotAvecIndex(unsigned int nbLettres, int index) {
	if (index >= 0) {
		std::string terme;
		docMots[nbLettres].seekg(offsets[nbLettres][index], docMots[nbLettres].beg);
		std::getline(docMots[nbLettres], terme);
		docMots[nbLettres].clear();
		terme = terme.substr(0, terme.length() - 1);	//Pour se débarasser du caractère de changement de ligne
		return Mot(terme);
	} else {
		return Mot("0");	//Pour les erreurs
	}
}

//MÉTHODE RENVOYANT UN MOT À PARIR DE SON INDEX
std::string Bd::donneMotAvecIndexStr(unsigned int nbLettres, int index) {
	if (index >= 0) {
		std::string terme;
		docMots[nbLettres].seekg(offsets[nbLettres][index], docMots[nbLettres].beg);
		std::getline(docMots[nbLettres], terme);
		docMots[nbLettres].clear();
		return terme.substr(0, terme.length() - 1);	//Pour se débarasser du caractère de changement de ligne
	} else {
		return "0"; 	//Pour les erreurs
	}
}

//FONCTION SERVANT À TROUVER L'INDEX D'UN MOT DANS LA BANQUE DE MOTS
int Bd::donneIndexMot(std::string mot) {
	int nbLettres = mot.length();
	int min = 0;
	int max = nbMots[nbLettres] - 1;
	int coupe;
	std::string motDeLaCoupe;
	while(min <= max) {
		coupe = (min + max) / 2;
		motDeLaCoupe = donneMotAvecIndexStr(nbLettres, coupe);
		if(motDeLaCoupe.compare(mot) > 0)
			max = coupe - 1;
		else
			if(motDeLaCoupe.compare(mot) < 0) {
				min = coupe + 1;
			} else {
				return coupe;
				break;
			}
	}
	return -1;
}

//MÉTHODE RENVOYANT L'INDEX D'UN MOT DANS LA BANQUE DES DÉFINITIONS
int Bd::donneIndexMotDansDefinitions(std::string terme) {
	int min = 0;
	int max = nbMots[0] - 1;
	int coupe;
	while(min <= max) {
		coupe = (min + max) / 2;
		std::string strTempo = donneMotAvecIndexStr(0, coupe);
		std::size_t found = strTempo.find(";");
		if(strTempo.substr(0, found).compare(terme) > 0)
			max = coupe - 1;
		else
			if(strTempo.substr(0, found).compare(terme) < 0) {
				min = coupe + 1;
			} else {
				return coupe;
				break;
			}
	}
	return -1;
}

//FONCTION SERVANT À TROUVER LE PREMIER INDEX DES MOTS COMMENÇANT PAR UNE SÉRIE DE LETTRES DÉFINIES PAR L'UTILISATEUR
//Renvoi -1 si aucun résultat trouvé
int Bd::donnePremierIndexDeChaine(std::string chaine, unsigned int tailleChamp) {
	//Déclaration des variables
	int min = 0;
	int max = nbMots[tailleChamp] - 1;
	int coupe;
	int comparaison;

	//On utilise la méthode "recherche dichotomique" pour trouver rapidement
	while(min <= max) {
		coupe = (min + max) / 2;
		comparaison = donneMotAvecIndexStr(tailleChamp, coupe).compare(0, tailleChamp, chaine);
		if(comparaison > 0) {
			max = coupe - 1;
		} else if(comparaison < 0) {
				min = coupe + 1;
		} else if (min != coupe) {
				max = coupe;
		} else {
			return coupe;
		}
	}
	return -1;
}

//MÉTHODE SERVANT À FAIRE LA COMPARAISON DE MOTS ENCORE INCOMPLETS
bool Bd::comparaisonWildCard(Mot mot, Mot motTempo) {
	for (unsigned int i = 0; i < mot.nbLettres && i < motTempo.nbLettres; i++) {
		if (mot.strMot[i] != '1' && mot.strMot[i] != motTempo.strMot[i]) {
			return false;
		}
	}
	return true;
}

//MÉTHODE RENVOYANT UN VECTEUR CONTENANT DES OBJETS MOTS COMMENÇANT PAR UNE CHAINE SPÉCIFIÉE DANS UNE BANQUE SPÉCIFIÉE
std::vector<Mot> Bd::donneVecteurMotsPossibles(Champ* champ) {
	std::string chaine = champ->getMot();
	std::size_t found = chaine.find("1");
	std::vector<Mot> vecteurDeMots;
	if (found == std::string::npos) {
		Mot chaineTempo(donneMotAvecIndex(champ->nbLettres, donneIndexMot(chaine)));
		if (chaineTempo.strMot.compare("0") != 0) {
			vecteurDeMots.push_back(chaineTempo);
		}
	} else {
		Mot chaineTempo(chaine);
		for (int indexMot = 0; indexMot < nbMots[champ->nbLettres]; indexMot++) {
			Mot motTempo(donneMotAvecIndex(champ->nbLettres, indexMot));
			if (comparaisonWildCard(chaineTempo, motTempo) == true) {
				vecteurDeMots.push_back(motTempo);
			}
		}
	}
	return vecteurDeMots;
}

//MÉTHODE RENVOYANT LES DÉFINITIONS D'UN MOT
std::vector<Definition> Bd::donneVecteurDefinitions(std::string terme) {
	std::vector<Definition> vecteurDeDefinitions;
	std::string strTempo = donneMotAvecIndexStr(0, donneIndexMotDansDefinitions(terme));
	if (strTempo.compare("0") != 0) {
		std::size_t pos = strTempo.find(";") + 1;
		for (int chariot = pos, len = strTempo.length(); chariot < len; chariot++) {
			std::string defTempo = "";
			while(chariot < len && strTempo[chariot] != ';') {
				defTempo += strTempo[chariot];
				chariot++;
			}
			if (defTempo.length() > 0) {
				Definition def(defTempo);
				vecteurDeDefinitions.push_back(def);
			} else {
				break;
			}
		}
	}
	return vecteurDeDefinitions;
}

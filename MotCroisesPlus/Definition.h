#ifndef DEFINITION_H_
#define DEFINITION_H_

#include <string>

class Definition {
	public:
		std::string contenu;
		int nbLettres;
	public:
		Definition(std::string contenu);
		virtual ~Definition();
};

#endif

#ifndef CASE_H_
#define CASE_H_

class Case {
	public:
		unsigned int x;
		unsigned int y;
		char contenu;
		unsigned int contrainte;
		bool caseNoire;
		bool dansUnChamp;
	public:
		Case(int x, int y, char contenu);
		virtual ~Case();
		int comparerContraintes(Case case1, Case case2);
};

#endif

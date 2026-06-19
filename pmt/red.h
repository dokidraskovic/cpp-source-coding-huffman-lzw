#ifndef _red_h_
#define _red_h_
#include<cstdlib>
#include<iostream>
#include "stablo.h"

using namespace std;

class Red {
	Cvor* prvi, *posl;
	int n;
public:
	explicit Red(Cvor* elem)
	{
		prvi = elem;
		posl = elem;
		n = 1;
	}
	void dodaj(Cvor* nesto);
	Cvor* skini();
	void ispis();
	Cvor* prviAdr();
	bool prazan();
	int uzmiN();
	void isprazni();
	~Red();
};

#endif // !_red_h_


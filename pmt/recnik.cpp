#include "recnik.h"

Recnik::Recnik(const Recnik & recnik)
{
	p = new Polje[recnik.br];
	for (int i = 0; i < recnik.br; i++)
	{
		p[i].simbol.append(recnik.p[i].simbol);
		p[i].kod.append(recnik.p[i].kod);
	}
	br = recnik.br;
}

Recnik::Recnik(Recnik && recnik)
{
	p = recnik.p;
	br = recnik.br;
	recnik.p = nullptr;
}

Recnik::~Recnik()
{
	delete[]p;
}

void Recnik::dodaj(string rec)
{
	p[br].simbol = rec;
	p[br].kod = to_string(br);
	br++;
}

string Recnik::trazi(string s)
{
	for (int i = 0; i < br; i++)
		if (p[i].simbol.compare(s) == 0)
			return p[i].kod;
	return nullptr;
}

bool Recnik::sadrzi(string s)
{
	for (int i = 0; i < br; i++)
		if (p[i].simbol.compare(s) == 0)
			return true;
	return false;
}

void Recnik::ispis()
{
	for (int i = 0; i < br; i++)
		cout << p[i].kod << "         " << p[i].simbol << endl;
}




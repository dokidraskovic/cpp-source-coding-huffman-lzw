#ifndef _recnik_h_
#define _recnik_h_
#include<cstdlib>
#include<iostream>
#include<string>

using namespace std;

struct Polje{
	string kod, simbol;
	Polje() { kod = ""; simbol = ""; }
};

class Recnik {
	Polje* p;
	int br;
public:
	Recnik(string s, string k, int n)
	{
		p = new Polje[n];
		p[0].kod = k;
		p[0].simbol = s;
		br = 1;
	}
	Recnik(const Recnik& recnik);
	Recnik(Recnik&& recnik);
	~Recnik();
	void dodaj(string rec);
	string trazi(string s);
	bool sadrzi(string s);
	int getbr() { return br; }
	void ispis();
};

#endif // !_recnik_h


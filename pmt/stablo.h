#ifndef _stablo_h_
#define _stablo_h_
#include<iostream>
#include<string>


using namespace std;

struct Cvor {
	float p;
	string s,sim;
	Cvor* levi;
	Cvor* desni;
	Cvor* otac;
	Cvor* sled;
	Cvor(float x,string y)
	{
		p = x;
		s = "";
		sim = y;
		levi = nullptr;
		desni = nullptr;
		otac = nullptr;
		sled = nullptr;
	}
	void ispis()
	{
		cout << "Simbol " << sim << " ima verovatnocu pojavljivanja " << p << " i kodnu rec " << s << endl;
	}
	float getP() { return p; }
	void ispisCeo()
	{
		Cvor* tren = this;
		while (tren != nullptr)
		{
			tren->ispis();
			tren = tren->sled;
		}
	}
};

class Stablo {
	Cvor* koren;
public:
	Stablo(Cvor* vr)
	{
		koren = vr;
	}
	Stablo* dodaj(Cvor* a, Cvor* b);
	Cvor* getCvor();
};

#endif // !_stablo_h_


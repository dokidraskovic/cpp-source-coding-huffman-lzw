#include "red.h"
#include<iostream>
#include<cstdlib>

using namespace std;

void Red::dodaj(Cvor * nesto)
{
	if (this->prvi == nullptr)
	{
		this->prvi = nesto;
		this->posl = nesto;
	}
	else
	{
		if (this->prvi->p > nesto->p)
		{
			nesto->sled = this->prvi;
			this->prvi = nesto;
			
		}
		else
		{
			Cvor* tren = this->prvi;
			if (tren->sled == nullptr)
				tren->sled = nesto;
			else
			{
				while (tren->sled->p <= nesto->p)
				{
					tren = tren->sled;
					if (tren->sled == nullptr) break;
				}
				nesto->sled = tren->sled;
				tren->sled = nesto;
			}
		}
	}
	n++;
}

Cvor * Red::skini()
{
	Cvor* tren = this->prvi;
	this->prvi = this->prvi->sled;
	tren->sled = nullptr;
	n--;
	return tren;
}

void Red::ispis()
{
	Cvor* tren = prvi;
	while (tren != nullptr)
	{
		cout << tren->p << ' ';
		tren = tren->sled;
	}
}

Cvor * Red::prviAdr()
{
	return this->prvi;
}

bool Red::prazan()
{
	return !prvi;
}

int Red::uzmiN()
{
	return n;
}

void Red::isprazni()
{
	if (prvi != nullptr)
	prvi->sled = nullptr;
	prvi = posl = nullptr;
	n = 0;
}


Red::~Red()
{
	while (prvi != nullptr)
	{
		Cvor* stari = prvi;
		prvi = prvi->sled;
		delete stari;
	}
}

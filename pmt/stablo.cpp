#include "stablo.h"
#include<iostream>
#include<cstdlib>

using namespace std;

Stablo* Stablo::dodaj(Cvor * a, Cvor * b)
{
	Cvor* novi = new Cvor(0.0,"");
	this->koren = novi;
	if (a->p > b->p)
	{
		this->koren->desni = a;
		this->koren->levi = b;
	}
	else
	{
		this->koren->levi = a;
		this->koren->desni = b;
	}
	a->otac = this->koren;
	b->otac = this->koren;
	this->koren->p = a->p + b->p;
	return this;
}

Cvor * Stablo::getCvor()
{
	return this->koren;
}


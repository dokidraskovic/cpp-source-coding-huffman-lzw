#include "stablo.h"
#include "red.h"
#include "recnik.h"
#include<iostream>
#include<cstdlib>
#include<ctime>
#include<iomanip>
#include<cmath>

using namespace std;

struct ObicanRed {     //Pomocna struktura za rad sa stablom
	Cvor** red;
	int kap, prvi=0, poslednji=0;
	ObicanRed(int k)
	{
		red = new Cvor*[k];
	}
	void dodaj(Cvor* neki)
	{
		if (prvi == poslednji)
		{
			red[prvi] = neki;
		}
		else
		{
			poslednji++;
			red[poslednji] = neki;
		}
	}
	bool prazan() { return prvi == poslednji; }
	Cvor* uzmi()
	{
		Cvor* pom = red[prvi];
		prvi++;
		return pom;
	}
	void isprazni() { prvi = poslednji = 0; }
	~ObicanRed()
	{
		delete[] red;
	}
};

void obrisiStablo(Stablo* st,ObicanRed& red)    //Pomocna fukncija da bi koriscenje memorije bilo racionalnije
{
	red.dodaj(st->getCvor());
	while (!red.prazan())
	{
		Cvor* pom = red.uzmi();
		if (pom->levi != nullptr)
			red.dodaj(pom->levi);
		if (pom->desni != nullptr)
			red.dodaj(pom->desni);
		delete pom;
		pom = nullptr;
	}
	st->~Stablo();
}

Cvor* prosirenje(Cvor* lista,int n)        //Ova funkcija na osnovu zadate liste simbola pravi prosirenje reda n i vraca novu listu
{
	Cvor* glava = nullptr;
	Cvor* posl = nullptr;
	Cvor* glava1=nullptr;
	Cvor* pom = lista;
	for (int i = 0; i < n-1; i++)
	{
		Cvor* tren = lista;
		while (tren != nullptr)  //Nalazenje svih kombinacija mnozenja elementa tren sa svim ostalim elementima liste
		{
			Cvor* sled = pom;
			while (sled != nullptr) //Prolazak kroz sve elemente liste
			{
				string s = tren->sim;
				Cvor* novi = new Cvor(tren->p*sled->p,s.append(sled->sim));//pravljenje novog elementa za novu listu simbola
				if (glava == nullptr)  //PRIMER: lista: 1->0
					glava = novi;		// glava->11->10->01->00
				else                    //Ukoliko trazimo drugo prosirenje gore navedena lista ce nam se kombinovati sa prvobitnom listom
					posl->sled = novi;
				posl = novi;
				sled = sled->sled;
			}
			tren = tren->sled;
		}
		pom = glava;//U ovom trenutku smo prosli i napravili sve kombinacije simbola koje postoje u pocetnoj listi
		glava1 = glava; //U zavisnosti koje prosirenje zelimo uz pomoc for petlje mi cemo ponoviti ovaj algoritam samo ovaj put
		glava = nullptr; //cemo u algoritam da uvrstimo novoformiranu listu
	}
	return glava1;
}

Stablo* napraviStablo(Red& red)
{

	Cvor* koren = new Cvor(0.0,"");
	Stablo* st = new Stablo(koren);
	while (!red.prazan()&& red.uzmiN()>1) //formiranje kodnog stabla 
	{									//Prvo sve simbole stavimo u prioritetni red i onda svaku iteraciju vadimo 2 simbola iz reda
		Cvor* a = red.skini();			//Od njih pravimo stablo u ciji su listovi upravo ta 2 simbola i onda taj koren vracamo
		Cvor* b = red.skini();			//u prioritetni red
		st=st->dodaj(a, b);				//Postupak se ponavlja dok god red nije prazan tacnije dok u njemu ne ostane samo koren 
		red.dodaj(st->getCvor());		//formiranog stabla
	}
	red.isprazni();
	return st;
}

void obilazak(Red& red,Stablo* st)
{
	Cvor* sled = st->getCvor();
	red.dodaj(sled);
	while (!red.prazan()) //Kada smo oformili stablo prolazimo kroz njega pocev od korena i ako idemo u levo pisemo 0 a ako idemo u desno 1
	{
		sled = red.skini();
		if (sled->levi != nullptr)
		{
			sled->levi->s = sled->s + "0";
			red.dodaj(sled->levi);
		}
		if (sled->desni != nullptr)
		{
			sled->desni->s = sled->s + "1";
			red.dodaj(sled->desni);
		}
	}
}

string kompresijaHafmen(char* sekvenca,Cvor** reci)
{
	int i = 0;
	string rez = "";
	string z =(const char*)sekvenca;
	int poc = 0;
	int kraj = 1;
	while (z[i] != '\0') //Prolazimo kroz originalnu sekvencu koju smo generisali i zamenjujemo simbol(kombinaciju simbola) sa odgovarajucom kodnom reci
	{
		int j = 0;
		while (reci[j] != nullptr) //Prolazak kroz niz kodnih reci
			if ((reci[j]->sim.compare(z.substr(poc, kraj))) == 0)//Ako se neki simbol(kombinacija simbola) poklopi sa delom sekvence, onda u rezultujucu sekvencu
			{												//ubacujemo odgovarajucu kodnu rec za taj simbol(kombinaciju simbola)
				rez.append(reci[j]->s);
				poc = i+1;
				kraj = 0;
				break;
			}
			else
			{
				j++; //Ako nije doslo do podudaranja prelazimo na sledeci simbol(kombinaciju simbola)
			}
		kraj++; //Ova promenljiva oznacava koliko karaktera u pocetnoj sekvenci ce biti uporedjeno sa elementom i nizu simbola
		i++; //Prelazak na sledeci karakter u sekvenci
	}
	return rez;
}

string kompresijaLZV(const char* sekvenca, Recnik& recnik,string& pomocni)
{
	pomocni = sekvenca;
	string konacni = "";
	string rec = pomocni.substr(0, 1); //Pocetni string je prvi karakter sekvence
	pomocni = konacni;
	int i = 1;
	while (sekvenca[i] != '\0')
	{
		char c = sekvenca[i]; //Uzimamo sledeci karakter u sekvenci
		i++;
		if (recnik.sadrzi(rec + c)) //Proveravamo da li kombinacija prethodnog stringa sa novim karakterom postoji u recniku
			rec += c;  //Ako postoji dodajemo novi karakter na stari string i vracamo se na pocetak petlje
		else
		{
			konacni.append(recnik.trazi(rec)); //Ako ne postoji onda u izlaznu sekvencu upisujem kod za dati string iz recnika
			pomocni.append(recnik.trazi(rec) + 'X'); //Ovo mi je pomocni string koji ce mi pomoci da jednoznacno odredim gde mi je kraj jednog koda a pocetak drugog
			recnik.dodaj(rec + c);						//na primer ako imam preko 10 kodova u recniku nisam siguran da li ako naidjem na 1 to treba da menjam
			rec = c;									//sa binarnom predstavom 1 ili da li treba da procitam jos jedan karakter (npr 9) i da zamenim 19
		}												// sa odgovarajucom binarnom predstavom
	}
	konacni.append(recnik.trazi(rec));          //U poslednje 2 linije u while petlji dodajemo novi niz karaktera u recnik i stavljamo da
	pomocni.append(recnik.trazi(rec) + 'X');	// pocetak novog stringa bude karakter koji je oznacio kraj prethodnog niza karaktera
	return konacni;
}

string konverzija(string s, int t) //Ovo je pomocna funkcija koja transformise string koji predstavlja neki prirodan broj u string
{									//koji predstavlja taj broj binarno sa t bita
	int br = stoi(s);
	string konv;
	while (br > 0)
	{
		int c = br % 2;
		konv.insert(0, to_string(c));
		br = br / 2;
	}
	for (int i = konv.size(); i < t; i++)
		konv.insert(0, "0");
	return konv;
}

string obradaLZV(Recnik rec,string sek)
{
	int st = log2(rec.getbr()) + 1; //Adrese u recniku (kodovi) ce biti predstavljeni binarno sa onoliko bita koliko imamo adresa
	string kom;
	int kraj = 0;
	int poc = 0;
	string::size_type n;
	n = sek.rfind('X'); //Indeks na kome se nalazi poslednje X koje ujedno predstavlja i zavrsni karakter pomocnog stringa
	int i = 0; //Tek sam kasnije shvatio da sam mogao da iskoristim metodu sek.size()-1 
	while (i <= n)
	{
		if (sek[i] == 'X') //Trazimo X jer znamo da je sve pre X odredjen kod iz recnika
		{
			string n=konverzija(sek.substr(poc,kraj),st);//Dobijanje binarne predstave datog koda (adrese) u recniku
			poc = i + 1;
			kraj = 0;
			kom += n; //Sada binarnu predstavu kopiramo u sekvencu koja ce nam predstavljati rezultat kompresije
		}
		else kraj++;
		i++;
	}
	return kom;
}

Cvor** kodneReciRed(Cvor* rec, Red& red,const int n) //Ovo je pomocna funkcija koja pravi niz  simbola i smesta ih u
{														//prioritetni red koji ce kasnije da se koristi za pravljenje stabla za
	Cvor** reci = new Cvor*[n + 1];						// odgovarajuc red prosirenja
	Cvor* tren = rec;
	int i = 0;
	while (tren != nullptr)
	{
		reci[i] = tren;
		Cvor* pom = tren->sled;
		tren->sled = nullptr;
		red.dodaj(tren);
		tren = pom;
		i++;
	}
	reci[i] = nullptr;
	return reci;
}

void obrada(Cvor* poc, Red& red, Stablo* st, Cvor**& kljuc, ObicanRed& ored, int n)//Ovo je pomocna funkcija koja vrsi odredjene korake obrade
{																				//prilikom izrade prosirenja
	delete[]kljuc;	//Racionalno koriscenje memorije
	Cvor* prosirenjen = prosirenje(poc, n); //Dobijemo listu simbola odredjenog prosirenja (npr. za 2. je to 11->10->01->00)
	red.isprazni();
	kljuc = kodneReciRed(prosirenjen, red, pow(2,n));//Stavljamo simbole iz liste u niz i takodje u prioritetni red
	Stablo* staro = st;
	st = napraviStablo(red); //Formiramo stablo za dati prioritetni red
	obrisiStablo(staro, ored); //Brisemo staro stablo
	ored.isprazni();
	red.ispis();
	obilazak(red, st); //Ovde svakom simbolu(kombinaciji simbola) dodeljujemo odgovarajucu kodnu rec
	for (int i = 0; i < pow(2,n); i++)
		kljuc[i]->ispis();
}

int main()
{
	float p1, q1, p0, q0, br1, br0;
	int n = 10000;
	char* sekvenca = new char[n+1];
	cout << "Unesite verovatnoce prelaska iz stanja 1 u 0 i iz stanja 0 u 1:" << endl;
	cin >> p1 >> p0;
	cout << "Prva vrednost sekvence ce se podrazumevati da je 0" << endl;
	srand((unsigned)time(nullptr));
	q1 = 1 - p1;
	q0 = 1 - p0;
	sekvenca[0] = '0';
	for (int i = 1; i < n; i++) //Generisemo n slucajnih brojeva prateci odgovarajuce prednosti p i q u grafu prelaza
	{
		float r = (float)rand() / (float)((unsigned)RAND_MAX + 1);
		if (sekvenca[i - 1] == '0')
			if (r < p0) sekvenca[i] = '1';
			else sekvenca[i] = '0';
		else
			if (r < p1) sekvenca[i] = '0';
			else sekvenca[i] = '1';
	}
	sekvenca[n] = '\0';
	cout << sekvenca << endl;
	br1 = br0 = 0;
	for (int i = 0; i < n; i++) //Racunamo koliko ukupno ima jedinica i nula u generisanoj sekvenci da bismo nasli verovatnoce koje cemo
		if (sekvenca[i] == '1') br1++; //koristiti u Hafmenovom algoritmu
		else br0++;
	Cvor* c1 = new Cvor(br1 / n,"1");
	Cvor* c0 = new Cvor(br0 / n,"0");
	cout << "Sada cemo u odnosu na generisanu sekvencu naci verovatnoce pojavljivanja 1 i 0 da bismo iskoristili Hafmenov algoritam" << endl;
	cout << "Verovatnoca pojavljivanja 1 je: ";
	cout<< c1->getP();
	cout <<endl<< "Verovatnoca pojavljivanja 0 je: ";
	cout<< c0->getP();
	cout << endl<< "Na osnovu ovih verovatnoca formiracemo stablo i pomocu njega kodne reci sledece kodne reci:" << endl;
	/*c0 = new Cvor(0.12, "A");
	c1 = new Cvor(0.22, "B");
	Cvor* c2 = new Cvor(0.15, "C");
	Cvor* c3 = new Cvor(0.07, "D");
	Cvor* c4 = new Cvor(0.07, "E");
	Cvor* c5 = new Cvor(0.17, "F");
	Cvor* c6 = new Cvor(0.09, "G");
	Cvor* c7 = new Cvor(0.11, "H");
	Red red(c0);
	red.dodaj(c1);
	red.dodaj(c2);
	red.dodaj(c3);
	red.dodaj(c4);
	red.dodaj(c5);
	red.dodaj(c6);
	red.dodaj(c7);
	Stablo *st = napraviStablo(red);
	obilazak(red, st);
	c0->ispis();
	c1->ispis();
	c2->ispis();
	c3->ispis();
	c4->ispis();
	c5->ispis();
	c6->ispis();
	c7->ispis();*/
	Red red(c1);
	red.dodaj(c0);
	Stablo* st = napraviStablo(red); //Pravi se kodno stablo na osnovu prioritetnog reda
	obilazak(red, st);	//Formiraju se kodne reci (u ovom slucaju imacemo samo 2 jer imamo samo 2 simbola)
	c1->ispis();
	c0->ispis();
	cout << "Sada cemo da na osnovu datih kodnih reci izvrsimo kompresiju" << endl;
	Cvor** kljuc= new Cvor*[3];
	kljuc[0] = c0;
	kljuc[1] = c1;
	kljuc[2] = nullptr;
	string komHaf = kompresijaHafmen(sekvenca, kljuc);
	cout << komHaf;
	cout << endl << "Sekvenca nakon kompresije ce biti iste duzine kao pre, eventualno ce 0 i 1 zameniti vrednosti u zavisnosti koji simbol" << endl;
	cout << "ima vecu verovatnocu" << endl;
	cout << "Sada cemo da izvrsimo kompresiju koristeci LZV algoritam" << endl;
	Recnik lzv("0", "0", 5000);
	lzv.dodaj("1");
	cout << "Adresa    Sadrzaj" << endl;
	string pom;
	string komLZV = kompresijaLZV(sekvenca, lzv,pom);
	lzv.ispis();
	cout << komLZV << endl;
	komLZV = obradaLZV(lzv, pom);
	cout << komLZV << endl;
	cout << "Odnos velicine sekvence pre i posle kompresije je " << endl;
	cout << (float)10000 / komLZV.size() << endl << endl;
	cout << "Dodatno cemo demonstrirati Hafmenov algoritam sa prosirenjima na pocetnoj sekvenci" << endl;
	(c1->p > c0->p) ? c1->sled = c0 : c0->sled = c1;
	Cvor* poc = c1->sled==nullptr ? c0 : c1;
	ObicanRed ored(1000);
	cout << "__________________________________________________________________________________" << endl;
	cout << "2.PROSIRENJE" << endl;
	obrada(poc, red, st, kljuc, ored, 2);
	komHaf = kompresijaHafmen(sekvenca, kljuc);
	cout << "Izvrsavamo kompresiju sekvence" << endl;
	cout << komHaf<<endl;
	cout << "Odnos velicine sekvence pre i posle kompresije je " << endl;
	cout << (float)10000 / komHaf.size() << endl;
	cout << "__________________________________________________________________________________" << endl;
	cout << "3.PROSIRENJE" << endl;
	obrada(poc, red, st, kljuc, ored, 3);
	komHaf = kompresijaHafmen(sekvenca, kljuc);
	cout << "Izvrsavamo kompresiju sekvence" << endl;
	cout << komHaf << endl;
	cout << "Odnos velicine sekvence pre i posle kompresije je " << endl;
	cout << "Ovde moze da dodje do neodgovarajuce vrednosti ukoliko ukupna velicina nije deljiva sa 3 jer mi svaku kombinaciju 3 simbola" << endl;
	cout << "hocemo da zamenimo odgovarajucom vrednoscu kodne reci te kombinacije simbola" << endl;
	cout << "Tako za slucaj kada imamo sekvencu od 10000 simbola gde se smenjuju 1 i 0 nece uspeti da zameni poslednji simbol" << endl;
	cout << (float)10000 / komHaf.size() << endl;
	cout << "__________________________________________________________________________________" << endl;
	cout << "4.PROSIRENJE" << endl;
	obrada(poc, red, st, kljuc, ored, 4);
	komHaf = kompresijaHafmen(sekvenca, kljuc);
	cout << "Izvrsavamo kompresiju sekvence" << endl;
	cout << komHaf << endl;
	cout << "Odnos velicine sekvence pre i posle kompresije je " << endl;
	cout << (float)10000 / komHaf.size() << endl;
	cout << "__________________________________________________________________________________" << endl;
	cout << "5.PROSIRENJE" << endl;
	obrada(poc, red, st, kljuc, ored, 5);
	komHaf = kompresijaHafmen(sekvenca, kljuc);
	cout << "Izvrsavamo kompresiju sekvence" << endl;
	cout << komHaf << endl;
	cout << "Odnos velicine sekvence pre i posle kompresije je " << endl;
	cout << (float)10000 / komHaf.size() << endl;
}
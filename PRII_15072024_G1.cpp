#include <iostream> 
#include<string>
#include<regex>
#include<mutex>
#include<thread>
#include<iomanip>
#include<sstream>
#include<chrono>
#include<set>

using namespace std;

/*
Kalup za sve kalse je isti
1. konstruktor
2. konstruktor kopije
3. metode 
4. operatori 
5. getteri ako ih ima
6. destruktor

globalne funkcije koje su povezane nisu rastavljene enterom,
postavljeno je objasnjenje u komentaru svake funkcije neke moje logike rada
*/
const char* crt = "\n-------------------------------------------\n";

enum Drzava { ENGLESKA, SPANIJA, HOLANDIJA, FRANCUSKA, BOSNA_I_HERCEGOVINA };
const char* ispis_drzava[]{"Engleska", "Spanija","Holandija","Francuska","Bosna i Hercegovina"};//Samo ispisuje nazive iz enuma.. duuuuh...

bool ValidirajID(const char* lozinka) {
	return regex_match(lozinka, regex("ID#([1-9]{4}|0[-][1-9]{3}|00[-][1-9]{2}|000[-][1-9]{1})"));
	/*
	nema ^$ da obiljezavaju pocetka i kraja, iz nekog razloga meni nepoznatog, su te oznake rusile kod
	ID# se gleda kao ta 3 karaktera, unutar () se nalaze 4 uslova za svaki moguci scenario: [1-9] {4} mora biti  4 broja od 1 do 9, 
	ili ako je manje dodaje se - pod moranje poslije nule
	*/
}

char* GetNizKaraktera(const char* sadrzaj, bool dealociraj = false) {
	if (sadrzaj == nullptr)return nullptr;
	int vel = strlen(sadrzaj) + 1;
	char* temp = new char[vel];
	strcpy_s(temp, vel, sadrzaj);
	if (dealociraj)
		delete[]sadrzaj;
	return temp;
	//Nisam je ja pisao :P
}

char* GenerisiID(int broj) {
	ostringstream baza;//kreiram ostringstream za lakse manipulisanje stringa
	baza << "ID#";//postavljam baznu vrijednost na ID#
	baza << setw(4) << setfill('0') << broj;//setw (4) kaze da je max broj cifara 4, setfill ce dodati nula onoliko koliko bude falilo nakon sto se broj unese

	string OvajCeUChar = baza.str();//pretvaramo ostringstream u string

	int NadjiNulu = OvajCeUChar.find_first_not_of("0", 3); //pronalazim prvu nulu koja se pojavljuje, bazna vrijednost je 3 i ako se ona vrati nema nula
	if (NadjiNulu > 3) {//stoga ova linija koda ako nema nula preskace se
		OvajCeUChar.insert(NadjiNulu, 1, '-');//ako ima nula dodaje se - tamo gdje je potreban (poslije zadnje nule)
	}
	const char* temp = OvajCeUChar.c_str();//pretvaram string u char* ali moram prvo u const char*
	char* napokon = GetNizKaraktera(temp);//pa onda u klasicni char*
	return napokon;//vracam char*
}

template<class T1, class T2, int max>
class Kolekcija {
	T1* _elementi1[max]{nullptr};
	T2* _elementi2[max]{nullptr};
	int* _trenutno;
public:
	Kolekcija() {
		_trenutno = new int(0);
	}

	Kolekcija(const Kolekcija& obj){
		_trenutno = new int(*obj._trenutno);
		for (size_t i = 0; i < *_trenutno ; i++)
		{
			_elementi1[i] = new T1(*obj._elementi1[i]);
			_elementi2[i] = new T2(*obj._elementi2[i]);
		}
	}

	void AddElement(T1 el1, T2 el2) {
		if (*_trenutno == max) { throw exception("Kolekcija popunjena!"); }
		_elementi1[*_trenutno] = new T1(el1);
		_elementi2[*_trenutno] = new T2(el2);
		(*_trenutno)++;
	}

	Kolekcija& InsertAt(int lokacija, T1 el1, T2 el2) {
		for (int i = *_trenutno; i > lokacija; i--)
		{
			_elementi1[i] = _elementi1[i - 1];
			_elementi2[i] = _elementi2[i - 1];
		}
		_elementi1[lokacija] = new T1(el1);
		_elementi2[lokacija] = new T2(el2);
		(*_trenutno)++;

		return *this;
	}

	Kolekcija<T1,T2,max>* RemoveRange(int pocetak, int kraj) {

		int size = kraj - pocetak;

		Kolekcija<T1,T2,max>* nova = new Kolekcija<T1,T2,max>();

		for (size_t i = pocetak; i <= kraj; i++)
		{
			nova->AddElement(*_elementi1[i], *_elementi2[i]);
		}

		int k = 0;
		for (size_t i = pocetak; i < (*_trenutno)-size; i++)
		{
			_elementi1[i] = _elementi1[kraj + k];
			_elementi2[i] = _elementi2[kraj + k];
			k++;
		}

		(*_trenutno) = *_trenutno - size;

		return nova;
	}
	
	Kolekcija& operator=(const Kolekcija& obj) {
		if (this != &obj) {
			for (size_t i = 0; i < *_trenutno; i++)
			{
				delete _elementi1[i]; _elementi1[i] = nullptr;
				delete _elementi2[i]; _elementi2[i] = nullptr;
			}
			delete _trenutno; _trenutno = nullptr;

			_trenutno = new int(*obj._trenutno);
			for (size_t i = 0; i < *_trenutno; i++)
			{
				_elementi1[i] = new T1(*obj._elementi1[i]);
				_elementi2[i] = new T2(*obj._elementi2[i]);
			}
		}
		return *this;
	}

	friend ostream& operator<< (ostream& COUT, Kolekcija& obj) {
		for (size_t i = 0; i < *obj._trenutno; i++)
			COUT << obj.getElement1(i) << " " << obj.getElement2(i) << endl;
		return COUT;
	}

	T1& getElement1(int lokacija)const { return *_elementi1[lokacija]; }
	T2& getElement2(int lokacija)const { return *_elementi2[lokacija]; }
	int getTrenutno() const { return *_trenutno; }

	~Kolekcija() {
		for (size_t i = 0; i < *_trenutno; i++)
		{
			delete _elementi1[i]; _elementi1[i] = nullptr;
			delete _elementi2[i]; _elementi2[i] = nullptr;
		}
		delete _trenutno; _trenutno = nullptr;
	}
};

class Vrijeme {
	int* _sat, * _minuta, * _sekunda;
public:
	Vrijeme(int sat = 10, int minuta = 0, int sekunda = 0) {
		_sat = new int(sat);
		_minuta = new int(minuta);
		_sekunda = new int(sekunda);
	}
	
	Vrijeme(const Vrijeme& obj) {
		_sat = new int(*obj._sat);
		_minuta = new int(*obj._minuta);
		_sekunda = new int(*obj._sekunda);
	}

	void addSekunda() {
		(*_sekunda)++;
		if ((*_sekunda)>=60)
		{
			(*_sekunda) = 0;
			(*_minuta)++;
		}
		if ((*_minuta) >= 60) {
			(*_minuta) = 0;
			(*_sat)++;
		}
	}

	int ToInt() {
		return ((*_minuta)*6)+((*_sat)*360)+((*_sekunda)*1);
	}

	friend bool operator ==(Vrijeme v1, Vrijeme v2) {
		if (*v1._sekunda == *v2._sekunda && *v1._minuta == *v2._minuta && *v1._sat == *v2._sat) { return true; }
		return false;
	}

	friend bool operator !=(Vrijeme v1, Vrijeme v2) {
		return!(v1 == v2);
	}

	friend bool operator < (Vrijeme v1, Vrijeme v2) {
		return (v1.ToInt() < v2.ToInt());
	}

	friend bool operator > (Vrijeme v1, Vrijeme v2) {
		return (v1.ToInt()> v2.ToInt());
	}

	Vrijeme& operator=(const Vrijeme& obj) {
		if (this != &obj) {
			delete _sat; _sat = nullptr;
			delete _minuta; _minuta = nullptr;
			delete _sekunda; _sekunda = nullptr;

			_sat = new int(*obj._sat);
			_minuta = new int(*obj._minuta);
			_sekunda = new int(*obj._sekunda);
		}
		return *this;
	}

	friend ostream& operator<< (ostream& COUT, const Vrijeme& obj) {
		COUT << *obj._sat << ":" << *obj._minuta << ":" <<*obj._sekunda;
		return COUT;
	}

	~Vrijeme() {
		delete _sat; _sat = nullptr;
		delete _minuta; _minuta = nullptr;
		delete _sekunda; _sekunda = nullptr;
	}
};

class Pogodak {
	Vrijeme _vrijemePogotka;
	char* _napomena;
public:
	Pogodak(Vrijeme vrijeme, const char* napomena) :_vrijemePogotka(vrijeme) {
		
		_napomena = GetNizKaraktera(napomena);}
	
	Pogodak(const Pogodak& obj):_vrijemePogotka(obj._vrijemePogotka){
		_napomena = GetNizKaraktera(obj._napomena);

	}

	Pogodak& operator=(const Pogodak& obj) {
		if (this != &obj) {
			delete[] _napomena; _napomena = nullptr;
			
			_vrijemePogotka = obj._vrijemePogotka;
			_napomena = GetNizKaraktera(obj._napomena);
		}
		return *this;
	}

	friend ostream& operator<< (ostream& COUT, const Pogodak& obj) {
		COUT << obj._vrijemePogotka << " -> " << obj._napomena;
		return COUT;
	}

	Vrijeme GetVrijemePogotka() { return _vrijemePogotka; }
	char* GetNapomena() { return _napomena; }

	~Pogodak() {delete[] _napomena; _napomena = nullptr;}
};

class Igrac {
	static int _id;
	char* _ID; // za inicijalizaciju _ID-a iskoristiti funkciju GenerisiID i vrijednost statickog clana _id
	char* _imePrezime;
	vector <Pogodak> _pogoci;
public:
	Igrac(const char* imePrezime){
		++_id;
		_imePrezime = GetNizKaraktera(imePrezime);
		_ID = GenerisiID(_id);
	}
	
	Igrac(const Igrac& obj){
		_imePrezime = GetNizKaraktera(obj._imePrezime);
		_ID = GetNizKaraktera(obj._ID);
		for (size_t i = 0; i < obj._pogoci.size(); i++)
			_pogoci.push_back(obj._pogoci[i]);
		
	}

	bool AddPogodak1(Pogodak& pogodak) {

		for (size_t i = 0; i <_pogoci.size() ; i++)
		{
			if (strcmp(_pogoci[i].GetNapomena(),pogodak.GetNapomena())==0) { return false; }
		}
		_pogoci.push_back(pogodak);
	
		return true;
	}
	
	Igrac& operator=(const Igrac& obj) {
		if (this != &obj) {
			delete[]_ID; _ID = nullptr;
			delete[]_imePrezime; _imePrezime = nullptr;
			_pogoci.clear();

			_imePrezime = GetNizKaraktera(obj._imePrezime);
			_ID = GetNizKaraktera(obj._ID);
			for (size_t i = 0; i < obj._pogoci.size(); i++)
				_pogoci.push_back(obj._pogoci[i]);
			
		}
		return *this;
	}

	friend bool operator == (const Igrac & obj, const Igrac& obj2) {
		return strcmp(obj._ID, obj2._ID) == 0;
	}

	friend ostream& operator<< (ostream& COUT, Igrac& obj) {
		COUT << obj._ID << " -> " << obj._imePrezime;
		for (size_t i = 0; i < obj._pogoci.size(); i++)
			cout << obj._pogoci[i] << endl;
		return COUT;
	}

	char* GetImePrezime() { return _imePrezime; }
	char* GetID() { return _ID; }
	vector<Pogodak>& GetPogoci() { return _pogoci; }

	~Igrac()
	{
		delete[]_ID; _ID = nullptr;
		delete[]_imePrezime; _imePrezime = nullptr;
	}
};

class Reprezentacija {
	Drzava _drzava;
	vector<Igrac> _igraci;
public:
	Reprezentacija(Drzava drzava) { _drzava = drzava;}

	Reprezentacija(const Reprezentacija& obj) {
		_drzava = (obj._drzava);
		for (size_t i = 0; i < obj._igraci.size(); i++)
		{
			_igraci.push_back(obj._igraci[i]);
		}
	}

	void AddIgrac(Igrac& ime) {
		for (size_t i = 0; i < _igraci.size(); i++)
		{
			if (strcmp(_igraci[i].GetID(), ime.GetID()) == true) { throw exception("Ne mozes istog igraca opet dodati... BOZE!"); }
		}
		_igraci.push_back(ime);
	}

	Reprezentacija& operator=(const Reprezentacija& obj) {
		if (this != &obj) {
			_drzava = (obj._drzava);
			for (size_t i = 0; i < obj._igraci.size(); i++)
			{
				_igraci.push_back(obj._igraci[i]);
			}
		}
		return *this;
	}

	friend bool operator==(Reprezentacija r1, Reprezentacija r2) {
		return(r1.GetDrzava() == r2.GetDrzava());
	}

	friend ostream& operator<<(ostream& COUT, Reprezentacija& obj) {
		cout<< "Drzava: " << ispis_drzava[obj._drzava]<<'\n';
		cout << "Igraci: \n";
		for (size_t i = 0; i < obj._igraci.size(); i++)
		{
			cout << obj._igraci[i];
			cout << endl;
		}
		
		return COUT;
	}

	Drzava  GetDrzava() { return _drzava; }

	vector<Igrac>& GetIgraci() { return _igraci; }

	~Reprezentacija() { }
};

mutex brava;
class Prventstvo {
	Kolekcija<Reprezentacija, Reprezentacija, 20> _utakmice;
	int pogoci1 = 0;
	int pogoci2 = 0;
public:
	Prventstvo(){}

	Prventstvo(const Prventstvo& obj){
		for (size_t i = 0; i < obj._utakmice.getTrenutno(); i++)
		{
			_utakmice.AddElement(obj._utakmice.getElement1(i), obj._utakmice.getElement2(i));
		}
	}

	void AddUtakmicu(Reprezentacija& r1, Reprezentacija& r2) {
		for (size_t i = 0; i < _utakmice.getTrenutno(); i++)
		{
			if (_utakmice.getElement1(i)==r1||_utakmice.getElement2(i)==r2)
			{
				throw exception("NE MOZE REVANS...BOZE..!");
			}
		}

		_utakmice.AddElement(r1, r2);
	}

	void posaljiMail(Pogodak pogodak, const char* Ime_ID) {
		brava.lock();

		cout << "To: ";
		for (size_t i = 0; i < _utakmice.getElement1(0).GetIgraci().size() ;i++)
		{
			cout << _utakmice.getElement1(0).GetIgraci()[i].GetID() << "@euro2024.com; ";
		}
		for (size_t i = 0; i < _utakmice.getElement2(0).GetIgraci().size(); i++)
		{
			cout << _utakmice.getElement2(0).GetIgraci()[i].GetID() << "@euro2024.com; ";
		}
		cout << "\nFrom: info@euro2024.com\nSubject: Informacija\n";
		cout << "\nPostovani, \n";
		cout << "\nU ";
		cout << pogodak.GetVrijemePogotka();
		cout << " igrac ";
		cout << Ime_ID;
		cout << " je zabiljezio svoj 1. pogodak na ovoj utakmici.";
		cout << "\n" << endl << "Trenutni rezultat je: \n";
		cout << ispis_drzava[_utakmice.getElement1(0).GetDrzava()] <<" "<<pogoci1<<" : "<<pogoci2<<'\n';
		cout << "\nPuno srece u nastavku susreta.\nNeka bolji tim pobijedi.";
		cout << crt;

		brava.unlock();
	}
	
	bool AddPogodak(Drzava d1, Drzava d2, const char* Ime_ID, Pogodak& pogodak) {

		for (size_t i = 0; i < _utakmice.getElement1(0).GetIgraci().size(); i++)
		{
			if (strcmp(_utakmice.getElement1(0).GetIgraci()[i].GetID(), Ime_ID) == 0 || strcmp(_utakmice.getElement1(0).GetIgraci()[i].GetImePrezime(), Ime_ID) == 0) {
				if (_utakmice.getElement1(0).GetIgraci()[i].AddPogodak1(pogodak)) {
					pogoci1++;

					//this_thread::sleep_for(chrono::seconds(2));
					thread mejlic(&Prventstvo::posaljiMail, this, pogodak, Ime_ID);
					mejlic.join();

					return true; };
			}
		}

		for (size_t i = 0; i < _utakmice.getElement2(0).GetIgraci().size(); i++)
		{
			if (strcmp(_utakmice.getElement2(0).GetIgraci()[i].GetID(), Ime_ID) == 0 || strcmp(_utakmice.getElement2(0).GetIgraci()[i].GetImePrezime(), Ime_ID) == 0) {
				if (_utakmice.getElement2(0).GetIgraci()[i].AddPogodak1(pogodak)) { 
					pogoci2++;

					//this_thread::sleep_for(chrono::seconds(2));
					thread mejlic(&Prventstvo::posaljiMail, this, pogodak, Ime_ID);
					mejlic.join();

					return true; }
			}
		}
		return false;
	}

	vector<Igrac*> operator()(Vrijeme v1, Vrijeme v2) {
		set<Igrac*> privremeni;//Koristim set jer ne dodaje duplikate ova funkcije nikako nije optimizovana ali radi :)

		do
		{
			for (size_t i = 0; i < _utakmice.getElement1(0).GetIgraci().size(); i++)
			{
				for (size_t j = 0; j < _utakmice.getElement1(0).GetIgraci()[i].GetPogoci().size(); j++)
				{
					if(_utakmice.getElement1(0).GetIgraci()[i].GetPogoci()[j].GetVrijemePogotka()==v1);
					privremeni.insert(&_utakmice.getElement1(0).GetIgraci()[i]);
				}
			}
			for (size_t i = 0; i < _utakmice.getElement2(0).GetIgraci().size(); i++)
			{
				for (size_t j = 0; j < _utakmice.getElement2(0).GetIgraci()[i].GetPogoci().size(); j++)
				{
					if (_utakmice.getElement2(0).GetIgraci()[i].GetPogoci()[j].GetVrijemePogotka() == v1);
					privremeni.insert(&_utakmice.getElement2(0).GetIgraci()[i]);
				}
			}
			v1.addSekunda();
		} while (v1 != v2);

		vector<Igrac*> novi(privremeni.begin(), privremeni.end());

		return novi;
	}

	friend ostream& operator<<(ostream& COUT, Prventstvo& obj) {
		COUT << crt;
		COUT << ispis_drzava[obj._utakmice.getElement1(0).GetDrzava()]<<" "<<obj.pogoci1<<" : "<< ispis_drzava[obj._utakmice.getElement2(0).GetDrzava()]<<" "<< obj.pogoci2;
		COUT << crt;

		COUT << "Golovi za: " << ispis_drzava[obj._utakmice.getElement1(0).GetDrzava()]<<endl;
		for (size_t i = 0; i < obj.GetUtakmice().getElement1(0).GetIgraci().size(); i++)
		{
			for (size_t j = 0; j < obj.GetUtakmice().getElement1(0).GetIgraci()[i].GetPogoci().size(); j++)
			{
				COUT<<obj.GetUtakmice().getElement1(0).GetIgraci()[j].GetImePrezime();
				COUT << endl;
			}
		}

		COUT << "Golovi za: " << ispis_drzava[obj._utakmice.getElement2(0).GetDrzava()] << endl;
		for (size_t i = 0; i < obj.GetUtakmice().getElement2(0).GetIgraci().size(); i++)
		{
			for (size_t j = 0; j < obj.GetUtakmice().getElement2(0).GetIgraci()[i].GetPogoci().size(); j++)
			{
				COUT << obj.GetUtakmice().getElement2(0).GetIgraci()[j].GetImePrezime();
			}
		}

		COUT << crt;

		return COUT;
	}

	Prventstvo& operator=(const Prventstvo& obj) {
		if (this != &obj) {
			for (size_t i = 0; i < obj._utakmice.getTrenutno(); i++)
			{
				_utakmice.AddElement(obj._utakmice.getElement1(i), obj._utakmice.getElement2(i));
			}
		}
		return *this;
	}

	Kolekcija<Reprezentacija, Reprezentacija, 20>& GetUtakmice() { return _utakmice; }

	~Prventstvo() { 
		cout << "Destruktor prventstva :)"; }
};

int Igrac::_id = 0;
void main() {

		cout<<crt << "Testiranje funkcije generisi id: \n";
		cout << GenerisiID(3) << endl;//treba vratiti ID#000-3 
		cout << GenerisiID(14) << endl;//treba vratiti ID#00-14 
		cout << GenerisiID(156) << endl;//treba vratiti ID#0-156 
		cout << GenerisiID(1798) << endl;//treba vratiti ID#1798 
		cout << crt;

		cout << "Testiranje funkcije ValidirajID: \n";
		if (ValidirajID("ID#000-3"))
			cout << "ID VALIDAN" << endl;
		if (ValidirajID("ID#0-156"))
			cout << "ID VALIDAN" << endl;
		if (!ValidirajID("ID#120-3"))
			cout << "ID NIJE VALIDAN" << endl;
		if (!ValidirajID("ID#00-02"))
			cout << "ID NIJE VALIDAN" << endl;
		cout << "Kraj testiranja funkcije ValidirajID: \n";
		cout << crt;

	cout << "Testiranje Kolekcije: \n";
		int kolekcijaTestSize = 9;
		Kolekcija<int, int, 10> kolekcija1;
		for (int i = 0; i < kolekcijaTestSize; i++)
			kolekcija1.AddElement(i, i); //dodaje vrijednosti u kolekciju
	
		cout<<"Ispis kolekcija1 nakon dodavanja 9 elementa metodom AddElement: \n"<<'\n' << "KOLEKCIJA 1: \n" << kolekcija1;

		cout << crt << "Testiranje operatora -> = <- i metode InsertAt: \n";
		Kolekcija<int, int, 10> kolekcija2 = kolekcija1.InsertAt(0, 10, 10);
		cout<<"Ispis kolekcija2 nakon -> kolekcija2 = kolekcija1.InsertAt<- :\n" << kolekcija2<<"Ispis kolekcija1 nakon istog: \n"<<kolekcija1 << crt;

	
		Kolekcija<int, int, 10>* kolekcija3 = kolekcija1.RemoveRange(1, 3);
		cout << "Testiranje metode ->RemoveRange<- :\n";
		cout << "Ispis kolekcija3 nakon -> kolekcija3 = kolekcija1.RemoveRange <-\n";
		cout << *kolekcija3 << endl;
		cout << "Ispis kolekcija1 nakon iste metode: \n";
		cout << kolekcija1 << crt;
	
		
		kolekcija1 = *kolekcija3;
		cout << "Ispis kolekcija 1 nakon ->kolekcija1 = *kolekcija3<-\n";
		cout << kolekcija1<<crt;
	
		cout << "Testiranje klase vrijeme: \n";
		Vrijeme
			prviPogodak201633(20, 16, 33),
			drugiPogodak202319(20, 23, 19),
			treciPogodak205108(20, 51, 8),
			cetvrtiPogodak210654(21, 6, 54);
		cout << prviPogodak201633<<'\n' << drugiPogodak202319 << '\n' << treciPogodak205108 << '\n' << cetvrtiPogodak210654 << '\n'<<crt;
	
		cout << "Testiranje klase Igrac: \n"<<endl;
		Igrac denis("Denis Music"), jasmin("Jasmin Azemovic"), goran("Goran Skondric"), adil("Adil Joldic");
		cout << "Igraci: \n" << denis << '\n' << jasmin << '\n' << goran << '\n' << adil << '\n' << endl;
		
		if (strcmp(denis.GetID(), "ID#000-1") == 0 && strcmp(jasmin.GetID(), "ID#000-2") == 0)
			cout << "ID se uspjesno generise!" << endl;
		cout << crt;

		cout << "Testiranje klase Pogodak: \n";
		Pogodak prviPogodak(prviPogodak201633, "podaci o prvom pogotku"),
			drugiPogodak(drugiPogodak202319, "podaci o drugom pogotku"),
			treciPogodak(treciPogodak205108, "podaci o trecem pogotku"),
			cetvrtiPogodak(cetvrtiPogodak210654, "podaci o pogotku");
		cout << '\n' << prviPogodak << '\n' << drugiPogodak << '\n' << treciPogodak << '\n' << cetvrtiPogodak << crt;


		cout << "Testiranje kalse Reprezentacija: \n";
		Reprezentacija BIH(BOSNA_I_HERCEGOVINA), ENG(ENGLESKA);
		BIH.AddIgrac(denis);
		BIH.AddIgrac(jasmin);

		ENG.AddIgrac(goran);
		ENG.AddIgrac(adil);
		cout << "Pokusavam dodati denisa 2. put: \n";
		try
		{
			BIH.AddIgrac(denis);
		}
		catch (exception& obj)
		{
			cout << obj.what();
		}
		cout << "\nAko vidis poruku iznad ovoga znaci da exception radi :)\n";
		cout << crt;

		cout << "Probajmo ispis BIH reprezentacije: \n";
		cout << BIH;

		cout<<crt << "Probajmo ispis ENG reprezentacije: \n";
		cout << ENG;
		cout << crt;


		Prventstvo euro2024;
		euro2024.AddUtakmicu(BIH, ENG);
		try
		{
			euro2024.AddUtakmicu(BIH, ENG);
		}
		catch (exception& obj)
		{
			cout << obj.what();
		}
		cout << endl;
		cout << crt;

		if (euro2024.AddPogodak(BOSNA_I_HERCEGOVINA, ENGLESKA, "ID#000-1", prviPogodak))
			cout << "Pogodak uspjesno dodat" << endl<<endl;
	
		if (!euro2024.AddPogodak(BOSNA_I_HERCEGOVINA, ENGLESKA, "Denis Music", prviPogodak))
			cout << "Pogodak NIJE uspjesno dodat" << endl << endl;
	
		if (euro2024.AddPogodak(BOSNA_I_HERCEGOVINA, ENGLESKA, "ID#000-2", drugiPogodak))
			cout << "Pogodak uspjesno dodat" << endl << endl;
	
		if (euro2024.AddPogodak(BOSNA_I_HERCEGOVINA, ENGLESKA, "Jasmin Azemovic", treciPogodak))
			cout << "Pogodak uspjesno dodat" << endl << endl;
	
		if (euro2024.AddPogodak(BOSNA_I_HERCEGOVINA, ENGLESKA, "Goran Skondric", cetvrtiPogodak))
			cout << "Pogodak uspjesno dodat" << endl << endl;
		
	
	
		cout << "Ispis klase Prventstvo: \n";
		cout << euro2024;
		cout << crt;
		
		cout << "Testiranje operatora() za klasu Prventstvo: \n";

		vector<Igrac*> igraci = euro2024(Vrijeme(20, 15, 15), Vrijeme(20, 24, 15));
	
		for (size_t i = 0; i < igraci.size(); i++)
			cout << igraci[i]->GetImePrezime() << endl;
	
		cin.get();
	system("pause>0");
};
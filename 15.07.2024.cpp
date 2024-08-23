#include <iostream> 
#include<fstream>
#include<string>
#include<sstream>
#include<thread>
#include<mutex>
#include<regex>
#include<iomanip>

const char* Poruka{ "Ovo je samo moj nacin rada ovog zadatka.\nNe znaci da je najbolji, niti najoptimiziraniji.\nNadam se da ce vam pomoci pri ucenju, SRETNO :)\n" };
const char* crt = "\n-------------------------------------------\n";

enum Drzava { BOSNA_I_HERCEGOVINA, ENGLESKA, SPANIJA, HOLANDIJA, FRANCUSKA};
std::ostream& operator<<(std::ostream& COUT, Drzava drzava) {
	const char* ispisEnuma[]{ "BOSNA_I_HERCEGOVINA","ENGLESKA","SPANIJA","HOLANDIJA","FRANCUSKA" };
	COUT << ispisEnuma[drzava];
	return COUT;
}

char* GetNizKaraktera(const char* sadrzaj, bool dealociraj = false) {
	if (sadrzaj == nullptr)return nullptr;
	size_t vel = strlen(sadrzaj) + 1;
	char* temp = new char[vel];
	strcpy_s(temp, vel, sadrzaj);
	if (dealociraj)
		delete[]sadrzaj;
	return temp;
}

bool ValidirajID(const char* ID) { return std::regex_match(ID, std::regex("ID#(000-[1-9]{1}|00-[1-9]{1}[0-9]{1}|0-[1-9]{1}[0-9]{2}|[1-9]{1}[0-9]{3})")); }

char* GenerisiID(int broj) {
	std::ostringstream manipulator;
	manipulator << "ID#";
	manipulator << std::setw(4) << std::setfill('0') << broj;

	std::string hajDodajMinus = manipulator.str();
	size_t place = hajDodajMinus.find_first_not_of('0', 3);
	if (place > 3) {hajDodajMinus.insert(place,1,'-');}

	const char* middleMan = hajDodajMinus.c_str();
	return GetNizKaraktera(middleMan);
}

template<class T1, class T2, int max>
class Kolekcija {
	T1* _elementi1[max];
	T2* _elementi2[max];
	int* _trenutno;
public:
	Kolekcija() {
		_trenutno = new int(0);

		for (int i = 0; i < max; i++)
		{
			_elementi1[i] = nullptr;
			_elementi2[i] = nullptr;
		}
	}

	Kolekcija(const Kolekcija& obj) {
		_trenutno = new int(*obj._trenutno);
		for (int i = 0; i < *_trenutno; i++)
		{
			_elementi1[i] = new T1(obj.getElement1(i));
			_elementi2[i] = new T2(obj.getElement2(i));

		}
	}

	void AddElement(T1 el1, T2 el2) {
		if (*_trenutno == max) { throw std::exception("Popunjena kolekcija"); }
		_elementi1[*_trenutno] = new T1(el1);
		_elementi2[*_trenutno] = new T2(el2);
		(*_trenutno)++;
	}

	Kolekcija& InsertAt(int lokacija, T1 el1, T2 el2) {
		for (int i = *_trenutno; i> lokacija; i--)
		{
			_elementi1[i] = _elementi1[i - 1];
			_elementi2[i] = _elementi2[i - 1];
		}
		_elementi1[lokacija] = new T1(el1);
		_elementi2[lokacija] = new T2(el2);
		(*_trenutno)++;

		return *this;
	}

	Kolekcija* RemoveRange(int start, int kraj) {
		if (start<0 || kraj>*_trenutno) { throw std::exception("Unos nije validan"); }
		
		Kolekcija* temp = new Kolekcija;
		int k = 1;

		for (int i = start; i <= kraj; i++)
		{
			temp->AddElement(getElement1(i), getElement2(i));
			delete _elementi1[i]; _elementi1[i] = nullptr;
			delete _elementi2[i]; _elementi2[i] = nullptr;
		}

		
		(*_trenutno) -= (kraj - start + 1);
		for (int i = start; i < *_trenutno; i++)
		{
			_elementi1[i] = _elementi1[kraj + k];
			_elementi2[i] = _elementi2[kraj + k];
			k++;
		}

		for (int i = *_trenutno; i < max; i++)
		{
			_elementi1[i] = nullptr;
			_elementi2[i] = nullptr;
		}

		return temp;
	}

	T1& getElement1(int lokacija)const {return *_elementi1[lokacija];}
	T2& getElement2(int lokacija)const {return *_elementi2[lokacija];}
	int getTrenutno() const { return *_trenutno; }

	Kolekcija& operator=(const Kolekcija& obj) {
		if (this != &obj) {
			*_trenutno = *obj._trenutno;
			for (int i = 0; i < *_trenutno; i++)
			{
				delete _elementi1[i]; _elementi1[i] = nullptr;
				delete _elementi2[i]; _elementi2[i] = nullptr;
				_elementi1[i] = new T1(obj.getElement1(i));
				_elementi2[i] = new T2(obj.getElement2(i));
			}
		}
		return *this;
	}

	friend std::ostream& operator<< (std::ostream& COUT, Kolekcija& obj) {
		for (int i = 0; i < *obj._trenutno; i++)
			COUT << obj.getElement1(i) << " " << obj.getElement2(i) <<std::endl;
		return COUT;
	}

	~Kolekcija() {
		for (int i = 0; i < *_trenutno; i++)
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

	int toInt() const {return *_sekunda + (*_minuta) * 6 + (*_sat) * 360;}
	bool operator<(const Vrijeme& obj) const { return toInt() < obj.toInt(); }
	bool operator>(const Vrijeme& obj) const { return toInt() > obj.toInt(); }
	bool operator==(const Vrijeme& obj) const { return toInt() == obj.toInt(); }

	Vrijeme& operator=(const Vrijeme& obj) {
		if (this != &obj) {
			*_sat = *obj._sat;
			*_minuta = *obj._minuta;
			*_sekunda = *obj._sekunda;
		}
		return *this;
	}

	friend std::ostream& operator<< (std::ostream& COUT, const Vrijeme& obj) {
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
	Pogodak(Vrijeme vrijeme, const char* napomena) :_vrijemePogotka(vrijeme) {_napomena = GetNizKaraktera(napomena);}
	
	Pogodak(const Pogodak& obj) {
		_napomena = GetNizKaraktera(obj._napomena);
		_vrijemePogotka = obj._vrijemePogotka;}
	
	Vrijeme GetVrijemePogotka() const { return _vrijemePogotka; }
	char* GetNapomena() { return _napomena; }

	bool operator==(const Pogodak& obj) const {
		if (strcmp(_napomena, obj._napomena) == 0
			&& _vrijemePogotka == obj._vrijemePogotka){return true;}

		return false;
	}

	Pogodak& operator=(const Pogodak& obj) {
		if (this != &obj) {
			delete[] _napomena; _napomena = nullptr;

			_napomena = GetNizKaraktera(obj._napomena);
			_vrijemePogotka = obj._vrijemePogotka;
		}
		return *this;
	}

	friend std::ostream& operator<< (std::ostream& COUT, const Pogodak& obj) {
		COUT << obj._vrijemePogotka << " -> " << obj._napomena;
		return COUT;
	}

	~Pogodak() { delete[] _napomena; _napomena = nullptr; }
};

class Igrac {
	static int _id;
	char* _ID; 
	char* _imePrezime;
	std::vector <Pogodak> _pogoci;

public:
	Igrac(const char* imePrezime){
		++_id;
		_imePrezime = GetNizKaraktera(imePrezime);
		_ID = GenerisiID(_id);}
	
	Igrac(const Igrac& obj) {
		_imePrezime = GetNizKaraktera(obj._imePrezime);
		_ID = GetNizKaraktera(obj._ID);
		_pogoci = obj._pogoci;}

	char* GetImePrezime() { return _imePrezime; }
	char* GetID() { return _ID; }
	const std::vector<Pogodak>& GetPogoci() const { return _pogoci; }
	std::vector<Pogodak>& GetPogoci()  { return _pogoci; }

	bool operator==(const Igrac& obj) const { return(strcmp(_ID, obj._ID) == 0); }//Ime i prezime teoretski moze biti isto ali ID ne moze? :) 

	Igrac& operator=(const Igrac& obj) {
		if (this != &obj) {
			delete[]_ID; _ID = nullptr;
			delete[]_imePrezime; _imePrezime = nullptr;

			_imePrezime = GetNizKaraktera(obj._imePrezime);
			_ID = GetNizKaraktera(obj._ID);
			_pogoci = obj._pogoci;}
		return *this;}

	friend std::ostream& operator<< (std::ostream& COUT, Igrac& obj) {
		COUT << obj._ID << " -> " << obj._imePrezime;
		for (size_t i = 0; i < obj._pogoci.size(); i++)
			std::cout << obj._pogoci[i] << std::endl;
		return COUT;
	}

	~Igrac(){
		delete[]_ID; _ID = nullptr;
		delete[]_imePrezime; _imePrezime = nullptr;}
};

class Reprezentacija {
	Drzava _drzava;
	std::vector<Igrac> _igraci;
public:

	Reprezentacija(Drzava drzava) {_drzava = drzava;}

	Reprezentacija(const Reprezentacija& obj) {
		_drzava = obj._drzava;
		for (int i = 0; i < obj._igraci.size() ;i++)
			_igraci.push_back(obj._igraci[i]);}

	int UkupnoGolova() const {
		size_t rez = 0;

		for (int i = 0; i < _igraci.size(); i++)
			rez+=_igraci[i].GetPogoci().size();
		
		return int(rez);}

	void AddIgrac(Igrac igrac) {
		for (int i = 0; i < _igraci.size(); i++)
			if (_igraci[i] == igrac) { throw std::exception("Dupliranje nije moguce"); }
		
		_igraci.push_back(igrac);}

	Drzava  GetDrzava() const { return _drzava; }
	std::vector<Igrac>& GetIgraci() { return _igraci; }

	friend std::ostream& operator<<(std::ostream& COUT, const Reprezentacija& obj) {
		COUT << "Drzava: " << obj._drzava<<'\n';
		COUT << "Igraci: \n";
		for (auto igrac : obj._igraci) {COUT << igrac<<'\n'; }
		return COUT;
	}

	Reprezentacija& operator=(const Reprezentacija& obj) {
		if (this != &obj) {
			_drzava = obj._drzava;
			_igraci.clear();
			for (int i = 0; i < obj._igraci.size() ;i++)
				_igraci.push_back(obj._igraci[i]);}

		return *this;}

	~Reprezentacija() {}
};

std::mutex mjutex;
class Prventstvo {
	Kolekcija<Reprezentacija, Reprezentacija, 20> _utakmice;

public:
	Prventstvo() = default;

	Prventstvo(const Prventstvo& obj) = default;

	void AddUtakmicu(Reprezentacija R1, Reprezentacija R2) {
		if (R1.GetDrzava() == R2.GetDrzava()) { throw std::exception("Greska-> Drzava ne moze igrati sama protiv sebe :/\n"); }

		for (int i = 0; i < _utakmice.getTrenutno(); i++)
			if (_utakmice.getElement1(i).GetDrzava() == R1.GetDrzava() 
				&& _utakmice.getElement2(i).GetDrzava() == R2.GetDrzava()) 
			{throw std::exception("Greska-> Utakmica vec postoji.\n");}
		
		_utakmice.AddElement(R1, R2);}

	int BrojUtakmice(Drzava d1, Drzava d2)const {
		for (int i = 0; i < _utakmice.getTrenutno(); i++)
			if (_utakmice.getElement1(i).GetDrzava() == d1 
				&& _utakmice.getElement2(i).GetDrzava() == d2) {return i;}		
		return 0;}

	void SendMail(Igrac igrac, Drzava d1, Drzava d2) {
		mjutex.lock();
		std::cout << "To: " << igrac.GetID() << "@euro2024.com\n";
		std::cout << "From: info@euro2024.com\n";
		std::cout << "Subject: Informacija\n" << std::endl;
		std::cout << "Postovani,\n" << std::endl;

		std::cout << "U " << igrac.GetPogoci()[igrac.GetPogoci().size() - 1].GetVrijemePogotka() << " sati Igrac " <<
			igrac.GetImePrezime() << " je zabio svoj " << igrac.GetPogoci().size() << " pogodak na ovoj utakmici\n";

		std::cout << "Trenutni rezultat je: " << 
			_utakmice.getElement1(BrojUtakmice(d1, d2)).GetDrzava() << " " << 
			_utakmice.getElement1(BrojUtakmice(d1, d2)).UkupnoGolova() << " : " <<

			_utakmice.getElement2(BrojUtakmice(d1, d2)).UkupnoGolova() << " " <<
			_utakmice.getElement2(BrojUtakmice(d1, d2)).GetDrzava();

		std::cout << "\n\nPuno srece u nastavku susreta\nNeka bolji tim pobjedi.\n\n";
		std::cout << "=======KRAJ MAILA=======\n";
		mjutex.unlock();
	}

	Igrac& VratiIgracaKojiJeDaoGol(Drzava d1, Drzava d2, const char* ImeP_ID) {
		int Broj_Utakmice = BrojUtakmice(d1, d2);

		for (auto& Igrac : _utakmice.getElement1(Broj_Utakmice).GetIgraci()) {
			if (strcmp(Igrac.GetID(), ImeP_ID) == 0 || 
				strcmp(Igrac.GetImePrezime(), ImeP_ID) == 0) 
			{ return Igrac; }}

		for (auto& Igrac : _utakmice.getElement2(Broj_Utakmice).GetIgraci()) {
			if (strcmp(Igrac.GetID(), ImeP_ID) == 0 || 
				strcmp(Igrac.GetImePrezime(), ImeP_ID) == 0) 
			{ return Igrac; }}

		throw std::exception("Greska-> Igrac sa tim imenom ili ID-om ne postoji.\n");}

	bool AddPogodak(Drzava d1, Drzava d2, const char* ImeP_ID, Pogodak Pogodak) {
		for (int i = 0; i < VratiIgracaKojiJeDaoGol(d1, d2, ImeP_ID).GetPogoci().size(); i++)
			if (VratiIgracaKojiJeDaoGol(d1, d2, ImeP_ID).GetPogoci()[i] == Pogodak) { return false; }
		
		VratiIgracaKojiJeDaoGol(d1, d2, ImeP_ID).GetPogoci().push_back(Pogodak);

		std::cout << "========SLANJE MAILA========\n";
		std::thread mejlic(&Prventstvo::SendMail, this, VratiIgracaKojiJeDaoGol(d1, d2, ImeP_ID), d1, d2);
		mejlic.join(); return true;}

	Kolekcija<Reprezentacija, Reprezentacija, 20>& GetUtakmice() { return _utakmice; }

	friend std::ostream& operator<<(std::ostream& COUT, const Prventstvo& p) {
		size_t brojGolova = p._utakmice.getElement1(0).GetIgraci()[0].GetPogoci().size();

		for (int i = 0; i < p._utakmice.getTrenutno(); i++){
			COUT << crt;

			COUT << p._utakmice.getElement1(i).GetDrzava() << " " <<
					p._utakmice.getElement1(i).UkupnoGolova() << " : " << 
					p._utakmice.getElement2(i).GetDrzava() << " " <<
					p._utakmice.getElement2(i).UkupnoGolova();

			COUT << crt;
			COUT << "Pogoci za " << p._utakmice.getElement1(i).GetDrzava() << " :\n";

			for (auto& igrac : p._utakmice.getElement1(i).GetIgraci()){
				for (int i = 0; i < igrac.GetPogoci().size(); i++){
					COUT << igrac.GetImePrezime() << '\n';}}

			COUT << std::endl;
			COUT << "Pogoci za " << p._utakmice.getElement2(i).GetDrzava() << " :\n";

			for (auto& igrac : p._utakmice.getElement2(i).GetIgraci()){
				for (int i = 0; i < igrac.GetPogoci().size(); i++){
					COUT << igrac.GetImePrezime() << '\n';}}}
		return COUT;
	}

	Prventstvo& operator=(const Prventstvo& obj) = default;

	std::vector<Igrac*> operator()(Vrijeme start, Vrijeme kraj) {
		std::vector<Igrac*>temp;
		for (int i = 0; i < _utakmice.getTrenutno(); i++){

				for (auto& igrac : _utakmice.getElement1(i).GetIgraci()) {

					for (auto& pogodak : igrac.GetPogoci()) {

						if (pogodak.GetVrijemePogotka() > start 
							&& pogodak.GetVrijemePogotka() < kraj) 

							{temp.push_back(new Igrac(igrac));}}}}
		return temp;}

	~Prventstvo() = default;
};

int Igrac::_id = 0;
int main() {
	std::cout << Poruka<<std::endl;

	std::cout<<crt << "Testiranje metode GenerisiID: \n";

	std::cout << GenerisiID(3) << std::endl;//treba vratiti ID#000-3 
	std::cout << GenerisiID(14) << std::endl;//treba vratiti ID#00-14 
	std::cout << GenerisiID(156) << std::endl;//treba vratiti ID#0-156 
	std::cout << GenerisiID(1798) << std::endl;//treba vratiti ID#1798 

	std::cout<< "Kraj testiranja metode GenerisiID" << crt;

	std::cout<<crt << "Testiranje metode ValidirajID:\n";

	if (ValidirajID("ID#000-3"))
		std::cout << "ID VALIDAN" << std::endl;
	if (ValidirajID("ID#0-156"))
		std::cout << "ID VALIDAN" << std::endl;
	if (!ValidirajID("ID#120-3"))
		std::cout << "ID NIJE VALIDAN" << std::endl;
	if (!ValidirajID("ID#00-02"))
		std::cout << "ID NIJE VALIDAN" << std::endl;

	std::cout << "Kraj testiranja metode ValidirajID" << crt;


	std::cout << "Testiranje klase Kolekcija:\n";

	int kolekcijaTestSize = 9;
	Kolekcija<int, int, 10> kolekcija1;
	for (int i = 0; i < kolekcijaTestSize; i++)
		kolekcija1.AddElement(i, i); 

	std::cout<<"Ispis kolekcija1 nakon AddElement:\n" << kolekcija1;

	Kolekcija<int, int, 10> kolekcija2 = kolekcija1.InsertAt(0, 10, 10);
	std::cout<<"\nIspis kolekcija2 nakon kolekcija2=kolekcija1.InsertAt: \n" << kolekcija2<<'\n';

	Kolekcija<int, int, 10>* kolekcija3 = kolekcija1.RemoveRange(1, 3);//Trebalo bi izbrisati kolekciju3 unutar main-a
	std::cout<<"\nIspis kolekcija3 nakon kolekcija3=kolekcija1.RemoveRange: \n" << *kolekcija3 << std::endl;
	std::cout<<"Ispis kolekcija1 nakon kolekcija3=kolekcija1.RemoveRange: \n" << kolekcija1 << '\n';

	kolekcija1 = *kolekcija3;
	std::cout << "Ispis kolekcija1 nakon kolekcija1=*kolekcija3:\n" << kolekcija1<<"\nKraj testiranja klase Kolekcija.\n" << crt;


	std::cout << "Testiranje klase Vrijeme:\n";
	Vrijeme
		prviPogodak201633(20, 16, 33),
		drugiPogodak202319(20, 23, 19),
		treciPogodak205108(20, 51, 8),
		cetvrtiPogodak210654(21, 6, 54);
	std::cout << "Kreiranje klasa...\n";
	std::cout << "Ispis kreiranih vremena: \n";
	std::cout<<'\n' << prviPogodak201633 << "\n" << drugiPogodak202319 << "\n" << treciPogodak205108 << "\n" << cetvrtiPogodak210654 << "\n";
	std::cout << "\nKraj testiranja klase Vrijeme." << crt;


	std::cout << "Testiranje klase Igrac: \n";
	std::cout << "Kreiranje igraca...\n";
	Igrac denis("Denis Music"), jasmin("Jasmin Azemovic"), goran("Goran Skondric"), adil("Adil Joldic");
	std::cout << "\nIspis kreiranih igraca:\n";
	std::cout << '\n' << denis << '\n' << jasmin << '\n' << goran << '\n' << adil << '\n';
	std::cout << "\nProvjeravamo da li se ID generise pravilno: ";
	if (strcmp(denis.GetID(), "ID#000-1") == 0 && strcmp(jasmin.GetID(), "ID#000-2") == 0)
		std::cout << "ID se uspjesno generise!\n" << std::endl;
	std::cout << "\nKraj testiranja klase Igrac." << crt;


	std::cout << "Testiranje klase Pogodak:\n";
	Pogodak prviPogodak(prviPogodak201633, "podaci o prvom pogotku"),
		drugiPogodak(drugiPogodak202319, "podaci o drugom pogotku"),
		treciPogodak(treciPogodak205108, "podaci o trecem pogotku"),
		cetvrtiPogodak(cetvrtiPogodak210654, "podaci o pogotku");
	std::cout << "Kreiranje pogodaka...\n";
	std::cout << "\nIspis kreiranih pogodaka: \n";
	std::cout<<'\n' << prviPogodak << '\n' << drugiPogodak << '\n' << treciPogodak << '\n' << cetvrtiPogodak << '\n';
	std::cout << "\nKraj testiranja klase pogodak" << crt;


	std::cout << "Testiranje klase Reprezentacija:\n";
	Reprezentacija BIH(BOSNA_I_HERCEGOVINA), ENG(ENGLESKA);
	std::cout << "Kreiranje reprezentacije...\n";
	std::cout << "\nTestiranje metode AddIgrac: \n";

	BIH.AddIgrac(denis);
	BIH.AddIgrac(jasmin);
	ENG.AddIgrac(goran);
	ENG.AddIgrac(adil);
	std::cout << "Ispis Reprezentacije BIH nakon AddIgrac:\n";
	std::cout << BIH<<'\n';
	std::cout << "Ispis Reprezentacije ENG nakon AddIgrac:\n";
	std::cout << ENG<<'\n';

	std::cout << "\nTestiranje exception-a (igrac sa istim ID): ";
	try{BIH.AddIgrac(denis);}
	catch (std::exception& obj){std::cout << obj.what();}
	std::cout << "\nKraj testiranja klase Reprezentacija" << crt;


	std::cout << "Testiranje klase Prventstvo:\n";
	Prventstvo euro2024;
	std::cout << "Testiranje metode AddUtakmicu(AddUtakmicu(BIH, ENG)): \n\n";
	euro2024.AddUtakmicu(BIH, ENG);
	
	std::cout << "Pokusavam dodati istu utakmicu opet: ";
	try{euro2024.AddUtakmicu(BIH, ENG);}
	catch (std::exception& obj){std::cout << obj.what();}

	std::cout << "Pokusavam dodati susret BIH, BIH: ";
	try { euro2024.AddUtakmicu(BIH, BIH); }
	catch (std::exception& obj) { std::cout << obj.what(); }

	std::cout << "\nTestiranje metode AddPogodak(dodavanje Pogodak klase koristeci ID ili ImePrezime):\n";
	if (euro2024.AddPogodak(BOSNA_I_HERCEGOVINA, ENGLESKA, "ID#000-1", prviPogodak))
		std::cout<<"AddPogodak BIH, ENG ID1, PrviPogodak -> " << "Pogodak uspjesno dodat" << std::endl;
	 
	if (!euro2024.AddPogodak(BOSNA_I_HERCEGOVINA, ENGLESKA, "Denis Music", prviPogodak))
		std::cout<<"AddPogodak BIH, ENG, Denis, PrviPogodak -> " << "Pogodak NIJE uspjesno dodat" << std::endl;

	if (euro2024.AddPogodak(BOSNA_I_HERCEGOVINA, ENGLESKA, "ID#000-2", drugiPogodak))
		std::cout<<"AddPogodak BIH, ENG, ID2, DrugiPogodak -> " << "Pogodak uspjesno dodat" << std::endl;

	if (euro2024.AddPogodak(BOSNA_I_HERCEGOVINA, ENGLESKA, "Jasmin Azemovic", treciPogodak))
		std::cout<<"AddPogodak BIH, ENG, Jasmin, TreciPogodak -> " << "Pogodak uspjesno dodat" << std::endl;

	if (euro2024.AddPogodak(BOSNA_I_HERCEGOVINA, ENGLESKA, "Goran Skondric", cetvrtiPogodak))
		std::cout<<"AddPogodak BIH, ENG, Goran, PrviPogodak -> " << "Pogodak uspjesno dodat" << std::endl;

	std::cout << "Pokusavam dodati pogodak na igraca koji ne postoji: \n";
	try{euro2024.AddPogodak(BOSNA_I_HERCEGOVINA, ENGLESKA, "Amer Jamakovic", cetvrtiPogodak);}
	catch (const std::exception& obj){std::cout << obj.what();}

	std::cout << "Kraj testiranja metode AddPogodak"<<crt;
	
	std::cout<<"\n Ispis klase Prventstvo:\n";
	std::cout<<'\n' << euro2024 << crt;

	std::cout << "Testiranje operatora() unutar klase Prventstvo:\n";
	std::cout << "igraci = euro2024():\n";
	std::vector<Igrac*> igraci = euro2024(Vrijeme(20, 15, 15), Vrijeme(20, 24, 15));

	std::cout << "Ispis igraci:\n\n";
	for (size_t i = 0; i < igraci.size(); i++)
		std::cout << igraci[i]->GetImePrezime() << std::endl;
	std::cout << "Kraj testiranja klase Prventstvo." << crt;

	std::cout << "The end"<<crt;
	//cin.get();
	return 0;
	system("pause>0");
}

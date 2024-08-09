#include <iostream>
#include<iomanip>
#include<string>
#include<vector>
#include<thread>
#include<mutex>
#include<regex>
#include<fstream>
#include<sstream>

using namespace std;


const char* crt = "\n-------------------------------------------\n";

enum Karakteristike { NARUDZBA, KVALITET, PAKOVANJE, ISPORUKA };
const char* ispisEnuma[]{ "NARUDZBA","KVALITET","PAKOVANJE","ISPORUKA" };

char* GetNizKaraktera(const char* sadrzaj, bool dealociraj = false) {
	if (sadrzaj == nullptr)return nullptr;
	int vel = strlen(sadrzaj) + 1;
	char* temp = new char[vel];
	strcpy_s(temp, vel, sadrzaj);
	if (dealociraj)
		delete[]sadrzaj;
	return temp;
}

template<class T1, class T2>
class Rjecnik {
	T1* _elementi1;
	T2* _elementi2;
	int _trenutno;
	bool _omoguciDupliranje;
public:
	Rjecnik(bool omoguciDupliranje = true) {
		_omoguciDupliranje = omoguciDupliranje;
		_trenutno = 0;
		_elementi1 = nullptr;
		_elementi2 = nullptr;
	}

	Rjecnik(const Rjecnik& obj) {
		_omoguciDupliranje = obj._omoguciDupliranje;
		_trenutno = obj._trenutno;
		_elementi1 = new T1[_trenutno];
		_elementi2 = new T2[_trenutno];

		for (size_t i = 0; i < _trenutno; i++)
		{
			_elementi1[i] = obj._elementi1[i];
			_elementi2[i] = obj._elementi2[i];
		}
	}

	void AddElement(T1 el1, T2 el2) {
		if (_omoguciDupliranje == false) {
			for (size_t i = 0; i < _trenutno; i++)
			{
				if (_elementi1[i] == el1 && _elementi2[i] == el2) {
					throw exception("Dupliranje nije moguce.");
				}
			}
		}
		T1* _temp1 = _elementi1;
		T2* _temp2 = _elementi2;

		_elementi1 = new T1[_trenutno+1];
		_elementi2 = new T2[_trenutno+1];
		for (size_t i = 0; i < _trenutno; i++)
		{
			_elementi1[i] = _temp1[i];
			_elementi2[i] = _temp2[i];
		}
		_elementi1[_trenutno] = el1;
		_elementi2[_trenutno] = el2;
		_trenutno++;
		delete[] _temp1; delete[] _temp2;
	}

	Rjecnik operator()(int OD, int Do) {
		Rjecnik temp = new Rjecnik();

		for (size_t i = OD; i <= Do; i++)
		{
			temp.AddElement(_elementi1[i], _elementi2[i]);
		}
		return temp;
	}

	Rjecnik& operator=(const Rjecnik& obj) {
		if (this != &obj) {
			delete[] _elementi1; _elementi1 = nullptr;
			delete[] _elementi2; _elementi2 = nullptr;

			_omoguciDupliranje = obj._omoguciDupliranje;
			_trenutno = obj._trenutno;
			_elementi1 = new T1[_trenutno];
			_elementi2 = new T2[_trenutno];

			for (size_t i = 0; i < _trenutno; i++)
			{
				_elementi1[i] = obj._elementi1[i];
				_elementi2[i] = obj._elementi1[i];
			}

		}
		return *this;
	}

	friend ostream& operator<< (ostream& COUT, const Rjecnik& obj) {
		for (size_t i = 0; i < obj._trenutno; i++)
			COUT << obj.getElement1(i) << " " << obj.getElement2(i) << endl;
		return COUT;
	}

	T1& getElement1(int lokacija)const { return _elementi1[lokacija]; }
	T2& getElement2(int lokacija)const { return _elementi2[lokacija]; }
	int getTrenutno() const { return _trenutno; }

	~Rjecnik() {
		delete[] _elementi1; _elementi1 = nullptr;
		delete[] _elementi2; _elementi2 = nullptr;
	}
};

class Datum {
	int* _dan, * _mjesec, * _godina;
public:
	Datum(int dan = 1, int mjesec = 1, int godina = 2000) {
		_dan = new int(dan);
		_mjesec = new int(mjesec);
		_godina = new int(godina);
	}

	Datum(const Datum& obj) {
		_dan = new int(*obj._dan);
		_mjesec = new int(*obj._mjesec);
		_godina = new int(*obj._godina);
	}
	
	Datum& operator=(const Datum& obj) {
		if (this != &obj) {

			delete _dan; _dan = nullptr;
			delete _mjesec; _mjesec = nullptr;
			delete _godina; _godina = nullptr;
		
			_dan = new int(*obj._dan);
			_mjesec = new int(*obj._mjesec);
			_godina = new int(*obj._godina);
		}
		return *this;
	}

	friend ostream& operator<< (ostream& COUT, const Datum& obj) {
		COUT << *obj._dan << "." << *obj._mjesec << "." << *obj._godina;
		return COUT;
	}
	
	~Datum() {
		delete _dan; _dan = nullptr;
		delete _mjesec; _mjesec = nullptr;
		delete _godina; _godina = nullptr;
	}
};

class ZadovoljstvoKupca {
	Rjecnik<Karakteristike, const char*> _komentariKarakteristika;
	int _ocjena; 
public:
	ZadovoljstvoKupca(int ocjena = 0) {
		_ocjena = ocjena;
	}

	ZadovoljstvoKupca(const ZadovoljstvoKupca& obj){
		_ocjena = obj._ocjena;
		for (size_t i = 0; i < obj._komentariKarakteristika.getTrenutno(); i++)
		{
			_komentariKarakteristika.AddElement(obj._komentariKarakteristika.getElement1(i), obj._komentariKarakteristika.getElement2(i));
		}
	}

	void DodajKomentarKarakteristike(Karakteristike enumeracija, const char* komentar) {
		for (size_t i = 0; i < _komentariKarakteristika.getTrenutno(); i++)
		{
			if (enumeracija == _komentariKarakteristika.getElement1(i)) { throw exception("Ocjena za karakteristiku vec dodana."); }
		}
		_komentariKarakteristika.AddElement(enumeracija, komentar);
	}

	bool operator==(ZadovoljstvoKupca& obj) {
		for (size_t i = 0; i < _komentariKarakteristika.getTrenutno(); i++)
		{
			if (_komentariKarakteristika.getElement1(i)==obj._komentariKarakteristika.getElement1(i))
			{
				return true;
			}
		}
		return false;
	}

	ZadovoljstvoKupca& operator=(const ZadovoljstvoKupca& obj) {
		if (this != &obj) {
			_ocjena = obj._ocjena;
			for (size_t i = 0; i < obj._komentariKarakteristika.getTrenutno(); i++)
			{
				_komentariKarakteristika.AddElement(obj._komentariKarakteristika.getElement1(i), obj._komentariKarakteristika.getElement2(i));
			}
		}
		return *this;
	}

	friend ostream& operator<<(ostream& COUT, const ZadovoljstvoKupca& obj) {
		COUT << "Zadovoljstvo kupca: \n";
		COUT << "Ocjena: " << obj._ocjena<<endl;
		COUT << "Komentari: \n";
		for (size_t i = 0; i < obj._komentariKarakteristika.getTrenutno(); i++)
		{
			COUT << ispisEnuma[obj._komentariKarakteristika.getElement1(i)] << "-> " << obj._komentariKarakteristika.getElement2(i) << endl;
		}
		return COUT;
	}
	int GetOcjena() { return _ocjena; }
	Rjecnik<Karakteristike, const char*>& GetKomentareKarakteristika() { return _komentariKarakteristika; }

	~ZadovoljstvoKupca() {};
};

class Osoba {
protected:
	char* _imePrezime;
	Datum _datumRodjenja;
public:
	Osoba(const char* imePrezime = "", Datum datumRodjenja = Datum()) : _datumRodjenja(datumRodjenja) {
		_imePrezime = GetNizKaraktera(imePrezime);
	}
	
	Osoba(const Osoba& obj) {
		_imePrezime = GetNizKaraktera(obj._imePrezime);
		_datumRodjenja = obj._datumRodjenja;
	}

	Osoba& operator=(const Osoba& obj) {
		if (this != &obj) {
			delete[] _imePrezime; _imePrezime = nullptr;

			_imePrezime = GetNizKaraktera(obj._imePrezime);
			_datumRodjenja = obj._datumRodjenja;
		}
		return *this;
	}
	
	friend ostream& operator<< (ostream& COUT, const Osoba& obj) {
		COUT << obj._imePrezime << " " << obj._datumRodjenja << endl;
		return COUT;
	}

	virtual void Info() = 0;
	char* GetImePrezime() { return _imePrezime; }
	Datum& GetDatumRodjenja() { return _datumRodjenja; }

	~Osoba() {
		delete[] _imePrezime; _imePrezime = nullptr;
	}
};

mutex brava;
class Kupac:public Osoba {
	char* _emailAdresa;
	Rjecnik<float, ZadovoljstvoKupca>* _kupovine;
	vector<int> _bodovi;
public:
	Kupac(const char* imePrezime = "", Datum datumRodjenja = Datum(), const char* emailAdresa = "") :
		Osoba(imePrezime, datumRodjenja){

		_emailAdresa = GetNizKaraktera(emailAdresa);
		_kupovine = new Rjecnik<float, ZadovoljstvoKupca>(false);
	}

	Kupac(const Kupac& obj) {
		_imePrezime = GetNizKaraktera(obj._imePrezime);
		_datumRodjenja = obj._datumRodjenja;
		_emailAdresa = GetNizKaraktera(obj._emailAdresa);
		for (size_t i = 0; i < obj._bodovi.size(); i++)
		{
			_bodovi.push_back(obj._bodovi[i]);
		}
		for (size_t i = 0; i < obj._kupovine->getTrenutno(); i++)
		{
			_kupovine->AddElement(obj._kupovine->getElement1(i), obj._kupovine->getElement2(i));
		}
		_kupovine = obj._kupovine;
	}

	void Mail(int provjera) {
		brava.lock();
		cout << crt;
		cout << "To: "<<_emailAdresa<<endl;
		cout << "Subject: Ostvareni bodovi\n"<<endl;
		cout << "Postovani, \n"<<endl;
		cout << "Prilikom posljednje kupovine ostvarili ste " << provjera << " bodova, tako da vas ukupan broj bodova iznosi " << GetBodoviUkupno()<<endl;
		cout << "\n Zahvaljujemo na kupovini\nPuno pozdrava\n";
		cout << crt;
		brava.unlock();
	}

	void DodajKupovinu(float cijena, ZadovoljstvoKupca zadovoljstvo) {
		_kupovine->AddElement(cijena, zadovoljstvo);
		int counter = cijena;
		int provjera = 0;
		if (counter > 10) {
			do
			{
				 counter -= 10;
				 provjera++;
			} while (counter > 10);
			_bodovi.push_back(provjera);
		}
		if (provjera > 5) {
			cout << "Slanje maila: \n";
			this_thread::sleep_for(chrono::seconds(3));
			thread posaljiMail(&Kupac::Mail,this, provjera);
			posaljiMail.join();
		}
	}

	int GetBodoviUkupno() {
		int ukupno = 0;
		for (size_t i = 0; i < _bodovi.size(); i++) ukupno += _bodovi[i];
		return ukupno;
	}

	void SacuvajBodove() {
		string naziv = _emailAdresa;
		naziv += ".txt";
		ofstream file(naziv);
		if (!file.fail()) {
			for (size_t i = 0; i < _bodovi.size(); i++)
			{
				file << _bodovi[i]<<endl;
			}
		}
		file.close();
	}

	void UcitajBodove() {
		string naziv = _emailAdresa;
		naziv += ".txt";
		const int max = 20;
		char red[max];
		string BrojString;
		int broj;

		ifstream file(naziv);
		if (!file.fail()) {
			while (file.eof()) {
				file.getline(red, max);
				BrojString = red;
				broj = stoi(BrojString);
				_bodovi.push_back(broj);
			}
		}
		file.close();
	}
	
	virtual void Info() { cout << *this; };

	Kupac& operator=(const Kupac& obj) {
		if (this != &obj) {
			delete[] _emailAdresa; _emailAdresa = nullptr;
			delete _kupovine; _kupovine = nullptr;
			delete _imePrezime; _imePrezime = nullptr;
			
			_imePrezime = GetNizKaraktera(obj._imePrezime);
			_datumRodjenja = obj._datumRodjenja;
			_emailAdresa = GetNizKaraktera(obj._emailAdresa);
			for (size_t i = 0; i < obj._bodovi.size(); i++)
			{
				_bodovi.push_back(obj._bodovi[i]);
			}
			for (size_t i = 0; i < obj._kupovine->getTrenutno(); i++)
			{
				_kupovine->AddElement(obj._kupovine->getElement1(i), obj._kupovine->getElement2(i));
			}
			_kupovine = obj._kupovine;
		}
		return *this;
	}

	Rjecnik<Karakteristike, const char*> GetKupovineByKomentar(const char* kljucnaRijec)
	{
		string KRijec = kljucnaRijec;
		Rjecnik<Karakteristike, const char*> rjecnik;
		
		for (size_t i = 0; i < _kupovine->getTrenutno(); i++)
		{
			for (size_t j = 0; j < _kupovine->getElement2(i).GetKomentareKarakteristika().getTrenutno(); j++)
			{
				string cijeliKomentar = _kupovine->getElement2(i).GetKomentareKarakteristika().getElement2(j);
				if (cijeliKomentar.find(KRijec) != -1) {
					rjecnik.AddElement(_kupovine->getElement2(i).GetKomentareKarakteristika().getElement1(j), _kupovine->getElement2(i).GetKomentareKarakteristika().getElement2(j));
				}
			}
		}

		return rjecnik;
	}

	friend ostream& operator<< (ostream& COUT, const Kupac& obj) {
		COUT << crt << obj._imePrezime << " " << obj._datumRodjenja << " " << obj._emailAdresa << " " << endl;
		COUT << "KUPOVINE -> " << crt;
		for (size_t i = 0; i < obj._kupovine->getTrenutno(); i++)
			COUT << "Iznos racuna: " << obj._kupovine->getElement1(i) << "KM, zadovoljstvo kupca: " << obj._kupovine->getElement2(i) << crt;
		COUT << "BODOVI -> ";
		for (size_t i = 0; i < obj._bodovi.size(); i++)
			COUT << obj._bodovi[i] << ", ";
		COUT << crt;
		return COUT;
	}

	char* GetEmail() { return _emailAdresa; }
	Rjecnik<float, ZadovoljstvoKupca>& GetKupovine() { return *_kupovine; }
	vector<int> GetBodovi() { return _bodovi; }

	~Kupac()
	{
		delete[] _emailAdresa; _emailAdresa = nullptr;
		delete _kupovine; _kupovine = nullptr;
	}
};


void main() {
	cout<<crt << "Testiranje klase rjecnik: \n";
	const int rjecnikTestSize = 9;
	Rjecnik<int, int> rjecnik1(false);

	cout << "Dodavanje elemenata u rjecnik preko AddElement metode: \n";
	for (int i = 0; i < rjecnikTestSize - 1; i++)
		rjecnik1.AddElement(i, i);

	cout << "Testiranje exception-a: \n";
	try {
		rjecnik1.AddElement(3, 3);
	}
	catch (exception& err) {
		cout << err.what() << crt;
	}
	rjecnik1.AddElement(9, 9);

	cout << "Ispis rjecni1 nakon dodavanja elemenata: \n";
	cout << rjecnik1 << crt;

	cout << "Testiranje operatora(): \n";
	cout << "rjecnik2=rjecnik1(int int)\n";
	Rjecnik<int, int> rjecnik2 = rjecnik1(2, 5);
	cout << "Ispis rjecnik2 nakon metode: \n";
	cout << rjecnik2 << crt;
	cout << "Ispis rjecnik1 nakon metode: \n";
	cout << rjecnik1;
	cout << "Kraj testiranja klase rjecnik\n" << crt;

	cout << "Testiranje klase kupac i osoba: \n";
	cout << "Kreiranje osoba* [3]: \n";
	const int maxKupaca = 3;
	Osoba* kupci[maxKupaca];
	cout << "setovanje osoba* [0 1 2] na new kupac: \n";
	kupci[0] = new Kupac("Denis Music", Datum(12, 1, 1980), "denis@fit.ba");
	kupci[1] = new Kupac("Jasmin Azemovic", Datum(12, 2, 1980), "jasmin@fit.ba");
	kupci[2] = new Kupac("Adel Handzic", Datum(12, 3, 1980), "adel@edu.fit.ba");
	cout << "Ispis dodanih kupaca: \n";
	for (size_t i = 0; i < maxKupaca; i++)
	{
		cout << *kupci[i];
	}
	cout << "Kraj ispisa" << crt;

	cout << "Testiranje klase zadovoljstvokupaca: \n";
	cout << "Kreiranje klase i dodavanja komentarakarakteristika: \n";
	ZadovoljstvoKupca zadovoljstvoKupca(7);
	zadovoljstvoKupca.DodajKomentarKarakteristike(NARUDZBA, "Nismo mogli odabrati sve potrebne opcije");
	zadovoljstvoKupca.DodajKomentarKarakteristike(KVALITET, "Kvalitet je ocekivan");

	cout << "Testiranje exception-a: \n";
	try {
		zadovoljstvoKupca.DodajKomentarKarakteristike(KVALITET, "Kvalitet je ocekivan");
	}
	catch (exception& err) {
		cout << err.what() << crt;
	}

	zadovoljstvoKupca.DodajKomentarKarakteristike(PAKOVANJE, "Pakovanje je bio osteceno");
	zadovoljstvoKupca.DodajKomentarKarakteristike(ISPORUKA, "Mada su najavili da ce proizvod biti isporucen u roku od 2 dana, cekali smo 5 dana");
	
	cout << "Ispisi zadovoljstokupaca: \n";
	cout << zadovoljstvoKupca;
	cout << "Kraj ispisa zadovoljstva\n";

	cout << "Kraj testiranja klase zadovoljstvokupaca\n";

	cout << "Testiranje bodova: \n";
	Kupac* denis = dynamic_cast<Kupac*>(kupci[0]);
	denis->DodajKupovinu(128, zadovoljstvoKupca);
	cout << "Ukupno bodova -> " << denis->GetBodoviUkupno();
	cout << crt;

	ZadovoljstvoKupca zadovoljstvoKupca2(4);
	zadovoljstvoKupca2.DodajKomentarKarakteristike(KVALITET, "Jako lose, proizvod ostecen");
	denis->DodajKupovinu(81, zadovoljstvoKupca2);
	cout << "Ukupno bodova -> " << denis->GetBodoviUkupno();
	
	cout << "\nTestiranje info metode: \n";
	denis->Info();
	

	cout << "Testiranje fstream metoda: \n";
	denis->SacuvajBodove();
	denis->UcitajBodove();
	cout << "Provjeriti txt dokument unutar foldera gdje se nalazi cpp\n";

	cout << "Testiranje pretrage: \n";
	Rjecnik<Karakteristike, const char*> osteceniProizvodi = denis->GetKupovineByKomentar("ostecen");
	cout << crt << "Rezultat pretrage -> " << crt << osteceniProizvodi << crt;

	for (size_t i = 0; i < maxKupaca; i++)
		delete kupci[i], kupci[i] = nullptr;
	cout << "\nKraj\n";


	cin.get();
	system("pause>0");
}
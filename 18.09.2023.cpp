#include <iostream>
#include<vector>
#include<sstream>
#include<fstream>
#include<mutex>
#include<regex>
#include<thread>
#include<iomanip>

using namespace std;

const char* PORUKA = "Ovaj ispit je uradjen demonstrativno, sa dosta dodataka u main-u, moze pomoci pri ucenju, nije nuzno najbrzi niti najbolji nacin\n"
"Obavezno pratiti postavku sa:\n\thttps://github.com/denis-music/cpp-programming-exams/blob/master/PRII_18092023\n\nHvala na citanju : )\n\n";

const char* crt = "\n-------------------------------------------\n";
enum Predmet { UIT, PRI, PRII, PRIII, RSI, RSII };
std::ostream& operator<<(std::ostream& COUT, Predmet predmet) {
    const char* ispisEnuma[]{ "UIT","PrI","PrII","PRIII","RSI","RSII" };
    COUT << ispisEnuma[predmet];
    return COUT;
}

const int MaxBrojRjesenja = 6;
const char* NIJE_VALIDNA = "<VRIJEDNOST_NIJE_VALIDNA>";

char* GetNizKaraktera(const char* sadrzaj) {
    if (sadrzaj == nullptr)return nullptr;
    int vel = strlen(sadrzaj) + 1;
    char* temp = new char[vel];
    strcpy_s(temp, vel, sadrzaj);
    return temp;
}

bool ValidirajLozinku(string lozinka) {
    return regex_search(lozinka, regex("(?=.*[A-Z])(?=.*[a-z])(?=.*[\\d])(?=.*[\\W _]).{7,}"));
}

template<class T1, class T2, int max = 10>
class Kolekcija {
    T1* _elementi1[max]{nullptr};//Dodao sam nullptr(nisam siguran da je dozvoljeno)
    T2* _elementi2[max]{nullptr};
    int* _trenutno;
public:
    Kolekcija() {_trenutno = new int(0);}
   
    Kolekcija(const Kolekcija& obj){
        _trenutno = new int(obj.getTrenutno());
        for (size_t i = 0; i < getTrenutno(); i++)
        {
            _elementi1[i] = new T1(obj.getElement1(i));
            _elementi2[i] = new T2(obj.getElement2(i));
        }
    }

    Kolekcija& operator=(const Kolekcija& obj) {
        if (this != &obj) {
            DealocirajNiz();
            delete _trenutno; _trenutno = nullptr;

            *_trenutno = obj.getTrenutno();
            for (size_t i = 0; i < getTrenutno(); i++)
            {
                _elementi1[i] = new T1(obj.getElement1(i));
                _elementi2[i] = new T2(obj.getElement2(i));
            }
        }
        return *this;
    }

    friend ostream& operator<< (ostream& COUT, Kolekcija& obj) {
        for (size_t i = 0; i < *obj._trenutno; i++)
            COUT << obj.getElement1(i) << " " << obj.getElement2(i) << endl;
        return COUT;
    }

    void AddElement(T1 element1, T2 element2) {
        if (*_trenutno == max) { throw runtime_error("Kolekcija popunjena"); }
        _elementi1[getTrenutno()] = new T1(element1);
        _elementi2[getTrenutno()] = new T2(element2);
        (*_trenutno)++;
    }

    void AddElement(T1 element1, T2 element2, int lokacija) {
        if (*_trenutno == max) { throw runtime_error("Kolekcija popunjena"); }
        if (lokacija >= max) { throw runtime_error("Unos nije validan"); }

        for (size_t i = *_trenutno; i > lokacija; i--)
        {
            _elementi1[i] = _elementi1[i - 1];
            _elementi2[i] = _elementi2[i - 1];
        }

        _elementi1[lokacija] = new T1(element1);
        _elementi2[lokacija] = new T2(element2);

        (*_trenutno)++;
    }

    T2& operator[](T1 element1) {
        for (size_t i = 0; i < getTrenutno(); i++)
            if (getElement1(i) == element1) { return *_elementi2[i]; } }

    Kolekcija* operator()(int start, int kraj) {
        Kolekcija* povratna = new Kolekcija();
        //Zadovoljavamo uslov da se vrate svi postojeci elementi u rangu start-kraj
        start = std::max(start, 0);//iako imam using namespace std u ovdje je potrebno koristi zbog max elementa unutar klase
        kraj = std::min(kraj, getTrenutno()-1);

        for (size_t i = start; i <= kraj; i++)
            povratna->AddElement(getElement1(i), getElement2(i));
        
        return povratna;
    }

    T1 getElement1(int lokacija)const { return *_elementi1[lokacija]; }
    T2 getElement2(int lokacija)const { return *_elementi2[lokacija]; }
    int getTrenutno() const { return *_trenutno; }

    void DealocirajNiz() {
        for (size_t i = 0; i < *_trenutno; i++) {
            delete _elementi1[i]; _elementi1[i] = nullptr;
            delete _elementi2[i]; _elementi2[i] = nullptr;
        }
    }

    ~Kolekcija() {
        DealocirajNiz();
        delete _trenutno; _trenutno = nullptr;
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

    Datum(const Datum& obj){
        _dan = new int(*obj._dan);
        _mjesec = new int(*obj._mjesec);
        _godina = new int(*obj._godina);
    }
    
    Datum& operator=(const Datum& obj) {
        if (this != &obj) {
            DealocirajDatum();

            _dan = new int(*obj._dan);
            _mjesec = new int(*obj._mjesec);
            _godina = new int(*obj._godina);
        }
        return *this;
    }

    bool PGodina(int godina) const noexcept{
        if (godina % 4 == 0 && (godina % 100 != 0) || godina % 400 == 0) { return true; }
        return false;
    }

    int BrojDana() const noexcept{
        int BrojDanaPoMjesecima[]{ 31,(PGodina(*_godina)?29:28),31,30,31,30,31,31,30,31,30,31};
        int UkupanBrojDana = 0;
        
        UkupanBrojDana += (*_dan);
        
        for (size_t i = 0; i < (*_mjesec)-1; i++)
            UkupanBrojDana += (BrojDanaPoMjesecima[i]);
        
        for (size_t i = 1; i < *_godina; i++)
            UkupanBrojDana += (PGodina(i) ? 366 : 365);
        
        return UkupanBrojDana;
    }

    bool operator<(const Datum& datum2) const noexcept {
        return BrojDana() < datum2.BrojDana();
    }

    bool operator<=(const Datum& datum2)const noexcept {
        return BrojDana() <= datum2.BrojDana();
    }

    bool operator>(const Datum& datum2)const noexcept {
        return BrojDana() > datum2.BrojDana();
    }

    bool operator>=(const Datum& datum2)const noexcept {
        return BrojDana() >= datum2.BrojDana();
    }

    bool operator!=(const Datum& datum2)const noexcept {
        return BrojDana() != datum2.BrojDana();
    }

    bool operator==(const Datum& datum2)const noexcept {
        return BrojDana() == datum2.BrojDana();
    }

    friend ostream& operator<< (ostream& COUT, const Datum& obj) {
        COUT << *obj._dan << "." << *obj._mjesec << "." << *obj._godina;
        return COUT;
    }

    void DealocirajDatum() {
        delete _dan; _dan = nullptr;
        delete _mjesec; _mjesec = nullptr;
        delete _godina; _godina = nullptr;
    }

    ~Datum() {
        delete _dan; _dan = nullptr;
        delete _mjesec; _mjesec = nullptr;
        delete _godina; _godina = nullptr;
    }
};

class Pitanje {
    char* _sadrzaj;
    //int se odnosi na ocjenu u opsegu  1 – 5, a Datum na datum kada je odgovor/rjesenje ocijenjeno
    Kolekcija<int, Datum*, MaxBrojRjesenja> _ocjeneRjesenja;
public:
    Pitanje(const char* sadrzaj = "") {_sadrzaj = GetNizKaraktera(sadrzaj);}
    
    Pitanje(const Pitanje& obj) {
        _sadrzaj = GetNizKaraktera(obj._sadrzaj);
        for (size_t i = 0; i < obj._ocjeneRjesenja.getTrenutno(); i++)
            _ocjeneRjesenja.AddElement(obj._ocjeneRjesenja.getElement1(i), obj._ocjeneRjesenja.getElement2(i));}

    friend bool operator==(const Pitanje& obj, const Pitanje& obj2) {
        if (obj._ocjeneRjesenja.getTrenutno()==obj2._ocjeneRjesenja.getTrenutno())
        {
            for (size_t i = 0; i < obj._ocjeneRjesenja.getTrenutno(); i++)
            {
                if (strcmp(obj._sadrzaj, obj2._sadrzaj) == 0 
                    &&obj._ocjeneRjesenja.getElement1(i)==obj2._ocjeneRjesenja.getElement1(i)
                    &&*obj._ocjeneRjesenja.getElement2(i)==*obj2._ocjeneRjesenja.getElement2(i)) 
                {
                    return true;
                }
            }
        }
        return false;}

    int DatumZadnjeOcjene() {//Vracam int vrijednost datuma kada je zadnja ocjena upisana
        if (_ocjeneRjesenja.getTrenutno() < 1) { return -1; }
        return _ocjeneRjesenja.getElement2(_ocjeneRjesenja.getTrenutno()-1)->BrojDana();}

    bool TrueAkoJeOcjenaPostojeca(const Datum& pocetak, const Datum& kraj) const noexcept {
        for (int i = 0; i < _ocjeneRjesenja.getTrenutno(); ++i) 
        {
             Datum trenutniDatum{*_ocjeneRjesenja.getElement2(i) };

            if (trenutniDatum >= pocetak && trenutniDatum <= kraj && _ocjeneRjesenja.getElement1(i) > 1) {
                return true;
            }
        }

        return false;
    }

    bool AddOcjena(Datum& datum, int ocjena) {
        if (datum.BrojDana() >= DatumZadnjeOcjene()&&
            datum.BrojDana()-DatumZadnjeOcjene() >= 3) {

            _ocjeneRjesenja.AddElement(ocjena, new Datum(datum));
            return true;}
        return false;}

    float ProsjecnaOcjena() const {
        if (_ocjeneRjesenja.getTrenutno() == 0) { return 0; }
        
        float rezultat = 0.0f;
        
        for (size_t i = 0; i < _ocjeneRjesenja.getTrenutno(); i++)
            rezultat += _ocjeneRjesenja.getElement1(i);
        
        return rezultat /= _ocjeneRjesenja.getTrenutno();
    }
    
    Pitanje& operator=(const Pitanje& obj) {
        if (this != &obj) {
            delete[] _sadrzaj; _sadrzaj = nullptr;

            _sadrzaj = GetNizKaraktera(obj._sadrzaj);
            for (size_t i = 0; i < obj._ocjeneRjesenja.getTrenutno(); i++)
                _ocjeneRjesenja.AddElement(obj._ocjeneRjesenja.getElement1(i), obj._ocjeneRjesenja.getElement2(i));}
        
        return *this;}

    friend ostream& operator<<(ostream& COUT, const Pitanje& obj) {
        COUT << "Sadrzaj pitanja: " << obj._sadrzaj;
        
        COUT << "\nOcjene: \n";
        for (size_t i = 0; i < obj._ocjeneRjesenja.getTrenutno(); i++)
            COUT << "Ocjena-> " << obj._ocjeneRjesenja.getElement1(i) << " datum ocjenjivanja: " << *obj._ocjeneRjesenja.getElement2(i) << '\n';
        COUT << "Prosjecna ocjena: " << obj.ProsjecnaOcjena();
        return COUT;
    }

    char* GetSadrzaj() { return _sadrzaj; }
    Kolekcija<int, Datum*, MaxBrojRjesenja> GetOcjene() const { return _ocjeneRjesenja; }

    ~Pitanje() {delete[] _sadrzaj; _sadrzaj = nullptr;}
};

class Ispit {
    Predmet* _predmet;
    vector<Pitanje> _pitanjaOdgovori;
public:
    Ispit(Predmet predmet = UIT) {_predmet = new Predmet(predmet);}
    
    Ispit(const Ispit& obj) {
        _predmet = new Predmet(*obj._predmet);
        for (size_t i = 0; i < obj._pitanjaOdgovori.size(); i++)
            _pitanjaOdgovori.push_back(obj._pitanjaOdgovori[i]);}

    float ProsjecnaOcjena() {
        float rezultat = 0.0f;
        for (size_t i = 0; i < _pitanjaOdgovori.size(); i++)
        {
            rezultat += _pitanjaOdgovori[i].ProsjecnaOcjena();
        }
        return rezultat /= _pitanjaOdgovori.size();
    }


    Ispit& operator=(const Ispit& obj) {
        if (this != &obj) {
            delete _predmet; _predmet = nullptr;

            _predmet = new Predmet(*obj._predmet);
            for (size_t i = 0; i < obj._pitanjaOdgovori.size(); i++)
                _pitanjaOdgovori.push_back(obj._pitanjaOdgovori[i]);}
        return *this;
    }
    
    friend ostream& operator<< (ostream& COUT, const Ispit& obj) {
        COUT << *obj._predmet << endl;
        for (size_t i = 0; i < obj._pitanjaOdgovori.size(); i++)
            COUT << obj._pitanjaOdgovori[i];
        return COUT;
    }

    vector<Pitanje>& GetPitanjaOdgovore() { return _pitanjaOdgovori; }

    Predmet GetPredmet() { return *_predmet; }

    ~Ispit() { delete _predmet; _predmet = nullptr; }
};

class Korisnik {
protected:
    char* _imePrezime;
    string _emailAdresa;
    string _lozinka;
    bool _aktivan;
public:
    Korisnik(const char* imePrezime, string emailAdresa, string lozinka){
        _imePrezime = GetNizKaraktera(imePrezime);
        _emailAdresa = emailAdresa;
        _lozinka = ValidirajLozinku(lozinka) ? lozinka : NIJE_VALIDNA;
        _aktivan = true;}

    Korisnik(const Korisnik& obj) {
        _imePrezime = GetNizKaraktera(obj._imePrezime);
        _emailAdresa = obj._emailAdresa;
        _lozinka = obj._lozinka;
        _aktivan = obj._aktivan;}
    
    Korisnik& operator=(const Korisnik& obj) {
        if (this != &obj) {
            delete[] _imePrezime; _imePrezime = nullptr;

            _imePrezime = GetNizKaraktera(obj._imePrezime);
            _emailAdresa = obj._emailAdresa;
            _lozinka = obj._lozinka;
            _aktivan = obj._aktivan;}
        return *this;
    }

    void SetAktivan(bool aktivan) { _aktivan = aktivan; }
    
    virtual void Info() = 0;

    friend ostream& operator<<(ostream& COUT, const Korisnik& obj) {
        COUT << "Korisnik: " << obj._imePrezime<<'\n';
        COUT << obj._emailAdresa << "::password-> " << obj._lozinka;
        COUT << (obj._aktivan) ? "Aktivan\n" : "Nije aktivan.\n";
        return COUT;
    }

    string GetEmail() { return _emailAdresa; }
    string GetLozinka() { return _lozinka; }
    char* GetImePrezime() { return _imePrezime; }
    bool GetAktivan() { return _aktivan; }

    virtual ~Korisnik() { delete[] _imePrezime; _imePrezime = nullptr; }
};

mutex brava;
class Kandidat:public Korisnik {
    vector<Ispit*> _polozeniPredmeti;
public:
    Kandidat(const char* imePrezime, string emailAdresa, string lozinka):Korisnik(imePrezime,emailAdresa,lozinka) {}

    Kandidat(const Kandidat& obj):Korisnik(obj._imePrezime,obj._emailAdresa,obj._lozinka) {
        for (size_t i = 0; i < obj._polozeniPredmeti.size(); i++)
            _polozeniPredmeti.push_back(new Ispit(*obj._polozeniPredmeti[i]));}

    float prosjekSvihIspita() {
        float rezultat = 0;
        for (size_t i = 0; i < _polozeniPredmeti.size(); i++)
        {
            rezultat += _polozeniPredmeti[i]->ProsjecnaOcjena();
        }
        return rezultat /= _polozeniPredmeti.size();
    }

    //MAGIJA ISPOD
    bool IstiAtributiPitanja(Pitanje pitanje) {
        for (size_t i = 0; i < _polozeniPredmeti.size(); i++) {
            auto pitanja = _polozeniPredmeti[i]->GetPitanjaOdgovore();
            if (std::find(pitanja.begin(), pitanja.end(), pitanje) != pitanja.end()) {
                return true;
            }
        }
        return false;
    }

    bool istiPredmet(Predmet predmet) {
        if (_polozeniPredmeti.empty()) { return false; }
        int ZadnjiPolozeniPredmet = _polozeniPredmeti.size() - 1;
        return (_polozeniPredmeti[ZadnjiPolozeniPredmet]->GetPredmet() == predmet);
    }

    bool ImaLiUsloveZaVecu() {
        if (_polozeniPredmeti.empty()) { return false; }
        int ZadnjiPolozeniPredmet = _polozeniPredmeti.size() - 1;

        float prosjecnaOcjena = _polozeniPredmeti[ZadnjiPolozeniPredmet]->ProsjecnaOcjena();
        int brojEvidentiranihPitanja = _polozeniPredmeti[ZadnjiPolozeniPredmet]->GetPitanjaOdgovore().size();

        return (prosjecnaOcjena >= 3.5 || brojEvidentiranihPitanja >= 3);
    }

    bool AddPitanje(Predmet predmet, Pitanje pitanje) {
        const char* odgovorNaPitanje = GetNizKaraktera(pitanje.GetSadrzaj());
        int ocjena = 0;

        if (_aktivan == false) { return false; }
        
        if (IstiAtributiPitanja(pitanje)) {return false;}

        if (_polozeniPredmeti.empty()) {

            Ispit* temp = new Ispit(predmet);
            temp->GetPitanjaOdgovore().push_back(pitanje);
            _polozeniPredmeti.push_back(temp);

            this_thread::sleep_for(chrono::seconds(2));
            thread mejlic([this,odgovorNaPitanje, pitanje]() {
                PosaljiMail(odgovorNaPitanje, pitanje);
                });
            mejlic.join();

            return true;}

        int ZadnjiPolozeniPredmet = _polozeniPredmeti.size() - 1;
        Ispit* ZadnjiIspit = _polozeniPredmeti[ZadnjiPolozeniPredmet];

        if (istiPredmet(predmet)) { 
            ZadnjiIspit->GetPitanjaOdgovore().push_back(pitanje);

            this_thread::sleep_for(chrono::seconds(2));
            thread mejlic([this,odgovorNaPitanje, pitanje]() {
                PosaljiMail(odgovorNaPitanje, pitanje);
                });
            mejlic.join();

            return true;}

        if (ZadnjiIspit->GetPitanjaOdgovore().size() >= 3 && ZadnjiIspit->ProsjecnaOcjena() > 3.5) {
            Ispit* temp = new Ispit(predmet);
            temp->GetPitanjaOdgovore().push_back(pitanje);
            _polozeniPredmeti.push_back(temp);
            return true;}
        return false;
    }
    //MAGIJA IZNAD

    Kolekcija<Pitanje, float> operator()(const Datum& pocetak, const Datum& kraj) const {
        Kolekcija<Pitanje, float> temp{};

        for (size_t i = 0; i < _polozeniPredmeti.size(); i++)
        {
            const vector<Pitanje>& privremeni = _polozeniPredmeti[i]->GetPitanjaOdgovore();

            for (size_t j = 0; j < privremeni.size() ; j++)
            {
                if (privremeni[j].TrueAkoJeOcjenaPostojeca(pocetak, kraj)) {
                    temp.AddElement(privremeni[j], privremeni[j].ProsjecnaOcjena());
                }
            }
        }

        return temp;

    }

    Kandidat& operator=(const Kandidat& obj) {
        if (this != &obj) {
            Korisnik::operator=(obj);

            for (size_t i = 0; i < obj._polozeniPredmeti.size(); i++)
                _polozeniPredmeti.push_back(new Ispit(*obj._polozeniPredmeti[i]));}
        return *this;}

    void Info() { cout << *this; }
   
    friend std::ostream& operator<< (std::ostream & COUT, Kandidat& obj) {
        COUT << (Korisnik&)obj << endl;
        COUT << "Polozeni predmeti:\n";
        for (size_t i = 0; i < obj._polozeniPredmeti.size(); i++)
            COUT << *obj._polozeniPredmeti[i];
        COUT << "Ukupni prosjek na svim ispitima: " << obj.prosjekSvihIspita();
        return COUT;}

    vector<Ispit*>& GetPolozeniPredmeti() { return _polozeniPredmeti; }

    void PosaljiMail(const char* odgovor, Pitanje pitanje) {
        int ocjene = 0;
        if (pitanje.GetOcjene().getTrenutno() == 0) { ocjene = 0; }
        if (pitanje.GetOcjene().getTrenutno() > 0) { ocjene = pitanje.GetOcjene().getElement1(pitanje.GetOcjene().getTrenutno() - 1); }
        brava.lock();
        cout << "\n====================MAIL=====================\n";
        cout << crt;
        cout << "From: info@kursevi.ba\n";
        cout << "To: "<< _emailAdresa<<'\n';
        cout << '\n' << "Postovani " << _imePrezime << " evidentirana vam je ocjena "<<ocjene<<" za odgovor na pitanje "<<odgovor<<'\n';
        cout << "Dosadasnji uspjeh za pitanje ->" << odgovor << " iznosi " << pitanje.ProsjecnaOcjena();
        cout << "\n Ukupni uspijeh na svim predmetima iznosi: "<<prosjekSvihIspita()<< endl;
        cout << "Pozdrav\n";
        cout << "\n EDUTeam.\n"<<crt;
        brava.unlock();
    }

    virtual ~Kandidat() {
        for (size_t i = 0; i < _polozeniPredmeti.size(); i++)
            delete _polozeniPredmeti[i], _polozeniPredmeti[i] = nullptr;
        cout << crt << "KANDIDAT :: DESTRUKTOR" << crt;}
};

const char* GetOdgovorNaPrvoPitanje() {
    cout << "Pitanje -> Pojasnite STACK i HEAP dio memorije, za šta su namijenjeni, te na koji način se trebaju koristiti (prednosti i nedostaci pojedinih slučajeva).\n";
    return "Odgovor -> Stack je dio memorije namijenjen za lokalne varijable, parametre funkcija i slicno.\n"
            "Prednost rada sa stack-om jeste sto ne moramo voditi racuna o alokaciji i dealokaciji memorije.\n"
            "Mana rada sa stack-om bi bila ogranicena kolicina memorije\n";

}
const char* GetOdgovorNaDrugoPitanje() {
    cout << "Pitanje -> Pojasnite preduslove za realizaciju polimorfizma, te koje su prednosti njegovog korištenja?\n";
    return "Odgovor -> Za realizaciju polimorfizma potrebna je jedna (ili vise) virtuelnih metoda.\n"
            "Mozemo koristiti i virtualni destruktor na isti nacin.\n"
            "Prednosti nasljedjivanja su da preko bazne klase radimo sa svim objektima koji su je naslijedili.\n";
}

void main(){

    cout << PORUKA;
//    cin.get();
//
    cout << GetOdgovorNaPrvoPitanje() << endl;
//    cin.get();
    cout << GetOdgovorNaDrugoPitanje() << endl;
//    cin.get();

    cout<<crt << "Testiranje klase Datum:\n";
    Datum   datum19062023(19, 6, 2023),
        datum20062023(20, 6, 2023),
        datum30062023(30, 6, 2023),
        datum05072023(5, 7, 2023);

    cout << "\nIspis datuma:\n" << datum19062023 << "\n" << datum20062023 << "\n" << datum30062023 << "\n" << datum05072023 << '\n';

    cout << "\nTestiranje operatora iz klase datum:\n";
    if (datum19062023 < datum20062023) {
        cout << "19.6. je manji\n";
    }
    if (datum30062023 > datum20062023) {
        cout << "30.6. je veci\n";
    }
    cout << "Testiranje BrojDana metode: \n"<<"19.6.2023 broj dana-> ";
    cout << datum19062023.BrojDana()<<'\n';

    cout<<crt << "Testiranje klase Kolekcija:\n";
    int kolekcijaTestSize = 10;
    cout << "Testiranje metode AddElement:\n";
    Kolekcija<int, int> kolekcija1;
    for (int i = 0; i < kolekcijaTestSize-2; i++)
        kolekcija1.AddElement(i, i);
    cout << "Kolekcija1 nakon AddElement metode:\n";
    cout << kolekcija1 << endl;

    try {cout << "Testiranje exception-a:\n";kolekcija1.AddElement(11, 11);}
    catch (exception& err) {cout << crt << "Greska -> " << err.what() << crt;}
    cout << "Kolekcija1 nakon exception-a:\n";
    cout << kolekcija1 << crt;

    cout << "Testiranje AddElement metode sa unosom lokacije:\n";
    kolekcija1.AddElement(9, 9, 2);
    cout << "Ispis Kolekcija1 nakon pomenute metode:\n";
    cout << kolekcija1 << crt;

    cout << "Testiranje operatora =(kolekcija2=kolekcija1)\n";
    Kolekcija<int, int> kolekcija2 = kolekcija1;
    cout << "Ispis kolekcija1 nakon operatora =:\n";
    cout << kolekcija1 << crt;
    cout << "Ispis kolekcija2 nakon pomenutog operatora:\n";
    cout << kolekcija2 << crt;

    cout << "Testiranje operatora[](mijenja vrijednost T2 na osnovu T1):\n";
    kolekcija1[9] = 2;
    cout << "Ispis kolekcija1 nakon pomenutog operatora:\n";
    cout << kolekcija1 << crt;

    cout << "Testiranje operatora() (vraca kolekcija* sa elementima od-do):\n";
    Kolekcija<int, int>* kolekcija3 = kolekcija1(1, 3);
    cout << "Ispis kolekcija3 nakon pomenute metode:\n";
    cout << *kolekcija3 << endl;
    delete kolekcija3;

    Pitanje 
        sortiranjeNiza("Navedite algoritme za sortiranje clanova niza?"),
        dinamickaMemorija("Navedite pristupe za upravljanje dinamickom memorijom?"),
        visenitnoProgramiranje("Na koji se sve nacin moze koristiti veci broj niti tokom izvrsenja programa?"),
        regex("Navedite par primjera regex validacije podataka?"),
        polimorfizam("Na koji nacin se realizuje polimorfizam?");

    cout << crt;
    cout << "Testiranje klase Pitanje:\n";
    cout << "\nAko je AddOcjena uredu, slijede 4 ispisa:\n";
    if (sortiranjeNiza.AddOcjena(datum19062023, 1))
        cout << "Ocjena evidentirana!" << endl;
    if (!sortiranjeNiza.AddOcjena(datum20062023, 5))
        cout << "Ocjena NIJE evidentirana!" << endl;
    if (sortiranjeNiza.AddOcjena(datum30062023, 5))
        cout << "Ocjena evidentirana!" << endl;
    if (polimorfizam.AddOcjena(datum19062023, 5))
        cout << "Ocjena evidentirana!" << endl;
    cout<<crt << "Testiranje operatora<< za klasu Pitanje:\n";
    cout<<'\n' << sortiranjeNiza << endl<<crt;

    cout << crt;
    cout << "Testiranje regexa: \n";
    cout << "Trebalo bi da slijede 4 ispisa: \n" << endl;
    if (ValidirajLozinku("john4Do*e"))
        cout << "Lozinka :: OK :)" << '\n';
    if (!ValidirajLozinku("john4Doe"))
        cout << "Lozinka :: Specijalni znak?" << '\n';
    if (!ValidirajLozinku("jo*4Da"))
        cout << "Lozinka :: 7 znakova?" << '\n';
    if (!ValidirajLozinku("4jo-hnoe"))
        cout << "Lozinka :: Veliko slovo?" << '\n';
    if (ValidirajLozinku("@john2Doe"))
        cout << "Lozinka:: OK :)" << '\n';

    cout << "\nValjda ih je 4 ;)\n";
    cout << crt;

    Korisnik* jasmin = new Kandidat("Jasmin Azemovic", "jasmin@kursevi.ba", "j@sm1N*");
    Korisnik* adel = new Kandidat("Adel Handzic", "adel@edu.kursevi.ba", "4Ade1*H");
    Korisnik* lozinkaNijeValidna = new Kandidat("John Doe", "john.doe@google.com", "johndoe");

    Kandidat* jasminPolaznik = dynamic_cast<Kandidat*>(jasmin);

    cout << "Testiranje AddPitanje metode unuar Kandidat klase: \n";
    cout << "\n Dodajemo prvo pitanje: \n";
    if (jasminPolaznik != nullptr) {

        if (jasminPolaznik->AddPitanje(PRI, dinamickaMemorija))
            cout << "Pitanje uspjesno dodano!" << crt;

        cout << "Pokusavam dodati pitanje za predmet veceg stepena: \n";
        if (!jasminPolaznik->AddPitanje(PRII, visenitnoProgramiranje))
            cout << "Pitanje NIJE uspjesno dodano!" << crt;

        cout << "Dodajem nova pitanja na prvi predmet: \n";
        if (jasminPolaznik->AddPitanje(PRI, visenitnoProgramiranje))
            cout << "Pitanje uspjesno dodano!" << crt;

        if (jasminPolaznik->AddPitanje(PRI, regex))
            cout << "Pitanje uspjesno dodano!" << crt;

        if (jasminPolaznik->AddPitanje(PRI, sortiranjeNiza))
            cout << "Pitanje uspjesno dodano!" << crt;

        cout << "Pokusavam dodati isto pitanje na prvi predmet: \n";
        if (!jasminPolaznik->AddPitanje(PRI, sortiranjeNiza))
            cout << "Pitanje NIJE uspjesno dodano!" << crt;
    }
    cout << "Ako ima 6 ispisa sve radi kako treba\nNadam se da ih je 6 :)\n" << crt;
        
    cout << "Dodavanje ocjene korisniku koji nije aktivan: \n";
    
    jasmin->SetAktivan(false);
    if (!jasminPolaznik->AddPitanje(PRII, polimorfizam)){
        cout << "Pitanje NIJE uspjesno dodano!" << crt;

        cout << "Ispis detaljnih podataka o kandidatu:\n";
        cout << *jasminPolaznik << crt;}

        
    cout << "Testiranje pretrage: \n";
    cout << crt;
    Datum ocijenjenOD(19, 6, 2023), ocijenjenDO(1, 7, 2023);
    Kolekcija<Pitanje, float> pretraga = (*jasminPolaznik)(ocijenjenOD, ocijenjenDO);

    cout << "U periodu od " << ocijenjenOD << " - " << ocijenjenDO << " ocijenjeno " << pretraga.getTrenutno() << " pitanja." << endl;
    for (size_t i = 0; i < pretraga.getTrenutno(); i++)
        cout << pretraga.getElement1(i) << endl << pretraga.getElement2(i) << crt;
    cout << "Kraj testiranja pretrage\n";
    cout << crt;

    delete jasmin;
    delete adel;
    delete lozinkaNijeValidna;
        
    cin.get();

    system("pause>0");
}

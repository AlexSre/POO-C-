#include <iostream>
#include <string>
#include <stdint.h>

using namespace std;

// ----- BEGIN STUDENT CODE -----
template <typename T> class  Lista{

    uint64_t nr_elemente;
    T * elemente;

    public:
    Lista() {
        nr_elemente = 0;
        elemente = NULL;
    };
    Lista(Lista<T> & other) {
        nr_elemente =other.nr_elemente;
        elemente = new T[nr_elemente];
        for (uint64_t i = 0; i < nr_elemente; i++) {
            elemente[i] = other.elemente[i];
        }
    };
    ~Lista() {
        delete[] elemente;
    }
    Lista<T> &operator=(const Lista<T> &other) {
        nr_elemente = other.nr_elemente;
        elemente = new T[nr_elemente];
        for (uint64_t i = 0; i < nr_elemente; i++) {
            elemente[i] = other.elemente[i];
        }
        return *this;
    };
    void adauga(const T & element_nou) {
        nr_elemente++;
        T * new_elemente = new T[nr_elemente];
        for (uint64_t i = 0; i < nr_elemente-1; i++) {
            new_elemente[i] = elemente[i];
        }
        new_elemente[nr_elemente-1] =element_nou;
        elemente = new T[nr_elemente];
        for (uint64_t i = 0; i < nr_elemente; i++) {
            elemente[i] = new_elemente[i];
        }

    }

    uint64_t get_nr_elemente() const{
        return nr_elemente;
    }

    T operator[](unsigned idx) const{
        return elemente[idx];
    }
};

class Angajat {

    string nume;
    uint32_t salariu;
    public:
    Angajat() {};
    Angajat(const string & n, const uint32_t & s) {
        nume = n;
        salariu = s;
    };
    string get_nume() {
        return nume;
    }
    uint32_t get_salariu() {
        return salariu;
    }

    void print() {
        cout <<"Angajat: "<< nume <<" "<< salariu <<endl;
    }

};

class Manager :public Angajat {

    public:
    Lista<Angajat> subordonati;
    Manager() {};
    Manager(const string & n, const uint32_t & s) : Angajat(n,s) {};

    void adauga_subordonat(const Angajat & angajat) {
        subordonati.adauga(angajat);
    }

    Lista<Angajat> get_subordonati() {
        return subordonati;
    }

    void print() {
        cout<<"Manager: "<< this->get_nume() <<" "<< this->get_salariu() <<endl;
        for(uint64_t i = 0; i < subordonati.get_nr_elemente(); i++) {
            subordonati[i].print();
        }
    }

};


// ----- END STUDENT CODE -----


// ----- DO NOT MODIFY BELOW THIS LINE -----
int main() {
    unsigned n;
    cin >> n;

    Manager* manageri = new Manager[n];

    for (unsigned i = 0; i < n; i++) {
        string nume;
        unsigned salariu, k;
        cin >> nume >> salariu >> k;

        manageri[i] = Manager(nume, salariu);

        for (unsigned j = 0; j < k; j++) {
            string nume_sub;
            unsigned salariu_sub;
            cin >> nume_sub >> salariu_sub;
            manageri[i].adauga_subordonat(Angajat(nume_sub, salariu_sub));
        }
    }

    string comanda;
    cin >> comanda;

    if (comanda == "print_toti") {
        for (unsigned i = 0; i < n; i++) {
            manageri[i].print();
        }
    }
    else if (comanda == "total_cheltuieli") {
        unsigned total = 0;
        for (unsigned i = 0; i < n; i++) {
            total += manageri[i].get_salariu();
            const Lista<Angajat>& subs = manageri[i].get_subordonati();
            for (unsigned j = 0; j < subs.get_nr_elemente(); j++) {
                total += subs[j].get_salariu();
            }
        }
        cout << total;
    }
    else if (comanda == "manager") {
        unsigned idx;
        cin >> idx;
        manageri[idx].print();
    }
    else if (comanda == "copie") {
        unsigned idx;
        cin >> idx;
        Manager copie(manageri[idx]);
        copie.print();
    }
    else if (comanda == "atribuire") {
        unsigned i, j;
        cin >> i >> j;
        manageri[i] = manageri[j];
        manageri[i].print();
        manageri[j].print();
    }
    else {
        cout << "Comanda invalida";
    }

    delete[] manageri;
    return 0;
}
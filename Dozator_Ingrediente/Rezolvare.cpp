#include <iostream>
#include <string>
#include <vector>
using namespace std;


class Fractie {
private:
    int numarator;
    int numitor;

    static int cmmdc(int a, int b) {
        if (a < 0) a = -a;
        if (b < 0) b = -b;
        while (b) {
            int r = a % b;
            a = b;
            b = r;
        }
        return a == 0 ? 1 : a;
    }

    void simplifica() {
        if (numitor < 0) {
            numitor = -numitor;
            numarator = -numarator;
        }
        int d = cmmdc(numarator, numitor);
        numarator /= d;
        numitor /= d;
    }

public:
    Fractie(int n = 0, int d = 1) {
        numarator = n;
        numitor = (d == 0 ? 1 : d);
        simplifica();
    }

    Fractie(const Fractie& other) {
        numarator = other.numarator;
        numitor = other.numitor;
    }

    Fractie& operator=(const Fractie& other) {
        if (this != &other) {
            numarator = other.numarator;
            numitor = other.numitor;
        }
        return *this;
    }

    Fractie operator+(const Fractie& other) const {
        return Fractie(numarator * other.numitor + other.numarator * numitor,
                       numitor * other.numitor);
    }

    Fractie operator*(const Fractie& other) const {
        return Fractie(numarator * other.numarator,
                       numitor * other.numitor);
    }

    bool operator==(const Fractie& other) const {
        return numarator == other.numarator && numitor == other.numitor;
    }

    bool operator<(const Fractie& other) const {
        return 1LL * numarator * other.numitor < 1LL * other.numarator * numitor;
    }

    void print() const {
        cout << numarator << "/" << numitor;
    }
};

class Ingredient {
private:
    string nume;
    Fractie cantitate;

public:
    Ingredient(const string& nume = "", const Fractie& cantitate = Fractie())
        : nume(nume), cantitate(cantitate) {}

    Ingredient(const Ingredient& other)
        : nume(other.nume), cantitate(other.cantitate) {
            countCopyConstructorCalls++;
        }

    Ingredient& operator=(const Ingredient& other) {
        if (this != &other) {
            nume = other.nume;
            cantitate = other.cantitate;
            callAssignOperatorCalls++;
        }
        return *this;
    }

    const string& get_nume() const {
        return nume;
    }

    const Fractie& get_cantitate() const {
        return cantitate;
    }

    void print() const {
        cout << nume << " ";
        cantitate.print();
        cout << "\n";
    }

private:
    static unsigned countCopyConstructorCalls;
    static unsigned callAssignOperatorCalls;
public:
    static void resetCounters(){
        countCopyConstructorCalls = 0;
        callAssignOperatorCalls = 0;
    }

    static unsigned get_countCopyConstructorCalls(){
        return countCopyConstructorCalls;
    }

    static unsigned get_callAssignOperatorCalls(){
        return callAssignOperatorCalls;
    }
};

unsigned Ingredient::countCopyConstructorCalls = 0;
unsigned Ingredient::callAssignOperatorCalls = 0;

int main() {

    int n, numarator, numitor;
    Fractie cantitate;
    Ingredient ingredient;
    vector <Ingredient> Ingrediente;
    string nume_ingredient,comanda;

    cin>>n;

    for (int i; i<n;i++) {
        cin>>nume_ingredient>>numarator>>numitor;
        Fractie cantitate(numarator, numitor);
        Ingrediente.push_back(Ingredient(nume_ingredient, cantitate));
    }

    cin>>comanda;

    if (comanda == "total") {

        for (int i=0; i< n; i++ ) {
            cantitate = cantitate + Ingrediente[i].get_cantitate();
        }
        cantitate.print();

    }else if (comanda == "max") {

        ingredient = Ingrediente[0];

        for (int i=1; i< n; i++ ) {
            if (ingredient.get_cantitate() < Ingrediente[i].get_cantitate() ) {
                ingredient = Ingrediente[i];
            }
        }

        ingredient.print();

    }else if (comanda == "equal") {
        cin>>numarator>>numitor;
        int cont = 0;
        Fractie cantitate(numarator, numitor);
        for (int i=0; i< n; i++ ) {
            if (cantitate == Ingrediente[i].get_cantitate() ) {
                cont++;
            }
        }

        cout<< cont;

    }else if (comanda == "copy") {
        int idx;
        cin>>idx;

        Ingredient copie(Ingrediente[idx]);
        copie.print();

    }else if (comanda == "assign") {

        int i,j;
        cin>>i>>j;
         Ingrediente[i]=Ingrediente[j];
        Ingrediente[i].print();
        Ingrediente[j].print();

    }else if (comanda == "double_all") {


        for (int i=0; i< n; i++ ) {
            Ingredient ingredient(Ingrediente[i].get_nume(),Ingrediente[i].get_cantitate()+Ingrediente[i].get_cantitate());
            Ingrediente[i]=ingredient;
            Ingrediente[i].print();
        }


    }else {

        cout << "Comanda invalida" << endl;

    }

    return 0;
}
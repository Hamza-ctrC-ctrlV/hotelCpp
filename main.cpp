#include <iostream>
using namespace std;

class hotel {};
class reservation {};

class Chambre {
protected:
    int numero;
    bool estOccupe;
    double prixBase;
public:
    Chambre(int num, double prix)
        : numero(num), prixBase(prix), estOccupe(false) {}

    virtual double calculerPrix() const = 0;

    virtual void afficher() const {
        cout << "Chambre " << numero
             << " | Prix Base: " << prixBase
             << " | Etat: " << (estOccupe ? "Occupée" : "Libre");
    }

    virtual ~Chambre() {}
};

class ChambreSimple : public Chambre {
private:
    double singpercentage;
public:
    ChambreSimple(int num, double prixBase, double perc)
        : Chambre(num, prixBase), singpercentage(perc) {} 

    double calculerPrix() const override {
        return prixBase * (1 + singpercentage / 100);
    }

    void afficher() const override {
        cout << "[Chambre Simple] ";
        Chambre::afficher();
        cout << " | Prix Final: " << calculerPrix() << endl;
    }
};

class doublee : public Chambre {
private:
    double doublepercentage;
public:
    doublee(int num, double prixBase, double perc)
        : Chambre(num, prixBase), doublepercentage(perc) {}  

    double calculerPrix() const override {
        return prixBase * (1 + doublepercentage / 100);
    }

    void afficher() const override {
        cout << "[Chambre double] ";
        Chambre::afficher();
        cout << " | Prix Final: " << calculerPrix() << endl;
    }
};

class suite : public Chambre {
private:
    double suitpercentage;
public:
    suite(int num, double prixBase, double perc)
        : Chambre(num, prixBase), suitpercentage(perc) {}  

    double calculerPrix() const override {
        return prixBase * (1 + suitpercentage / 100);
    }

    void afficher() const override {
        cout << "[Chambre suite] ";
        Chambre::afficher();
        cout << " | Prix Final: " << calculerPrix() << endl;
    }
};

class personnes {};
class client : public personnes {};
class employe : public personnes {};

int main() {}

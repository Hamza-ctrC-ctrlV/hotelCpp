#include <iostream>
using namespace std;


class hotel {};
class reservation {};
class chambre{
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
    ChambreSimple(int num, double prixBase)
        : Chambre(num, prixBase),singpercentage(sing) {}

    double calculerPrix() const override {
        return prixBase * (1 + singpercentage/100);  
    }

    void afficher() const override {
        cout << "[Chambre Simple] ";
        Chambre::afficher();
        cout << " | Prix Final: " << calculerPrix() << endl;
    }
};


class doublee : public chambre {class ChambreSimple : public Chambre {
    private:
    double doublepercentage;
public:
    ChambreSimple(int num, double prixBase)
        : Chambre(num, prixBase),doublepercentage(doub) {}

    double calculerPrix() const override {
        return prixBase * (1 + doublepercentage/100);  
    }

    void afficher() const override {
        cout << "[Chambre double] ";
        Chambre::afficher();
        cout << " | Prix Final: " << calculerPrix() << endl;
    }
};};


class suite : public chambre {class ChambreSimple : public Chambre {
    private:
    double suitpercentage;
public:
    ChambreSimple(int num, double prixBase)
        : Chambre(num, prixBase),suitpercentage(suit) {}

    double calculerPrix() const override {
        return prixBase * (1 + suitpercentage/100);  
    }

    void afficher() const override {
        cout << "[Chambre suit] ";
        Chambre::afficher();
        cout << " | Prix Final: " << calculerPrix() << endl;
    }
};};

class personnes{};
class client : public personnes {};
class employe : public personnes {};

int main (){}


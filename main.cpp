#include <iostream>
#include <string>
#include <vector>
#include <memory>
#include <algorithm>
#include <sstream> // // lib bach n9lb klma ar9am w l3ks (5<->"5")
#include <ctime>
using namespace std;

//Classes Personne, Client, Staff

class Personne {
protected:
    string nom;
    string prenom;
    string cin;
    string telephone;

public:
    Personne(string n, string p, string c, string tel)
        : nom(n), prenom(p), cin(c), telephone(tel) {
    }

    virtual void afficher() const {
        cout << "Nom: " << nom << " " << prenom
            << " | CIN: " << cin
            << " | Tel: " << telephone;
    }
    //surcharge bach tsma dkhlna dakchi li kan9raw hh 
    bool operator==(const Personne& autre) const {
        return cin == autre.cin;
    }
    //ta hna nfs l7aja 
    friend ostream& operator<<(ostream& os, const Personne& p) {
        os << p.nom << " " << p.prenom << " (CIN: " << p.cin << ")";
        return os;
    }

    string getNom() const { return nom; }
    string getPrenom() const { return prenom; }
    string getCin() const { return cin; }
    string getTelephone() const { return telephone; }

    void setNom(string n) { nom = n; }
    void setPrenom(string p) { prenom = p; }
    void setTelephone(string t) { telephone = t; }

    virtual ~Personne() {}
};

class Client : public Personne {
private:
    int numeroClient;
    static int compteurClients;

public:
    Client(string n, string p, string c, string tel)
        : Personne(n, p, c, tel) {
        numeroClient = ++compteurClients;
    }

    void afficher() const override {
        cout << "[Client #" << numeroClient << "] ";
        Personne::afficher();
        cout << endl;
    }

    bool operator<(const Client& autre) const {
        return numeroClient < autre.numeroClient;
    }

    int getNumeroClient() const { return numeroClient; }
};

int Client::compteurClients = 0;

class Staff : public Personne {
private:
    int idStaff;
    string poste;
    string motDePasse;
    static int compteurStaff;

public:
    Staff(string n, string p, string c, string tel, string post, string mdp)
        : Personne(n, p, c, tel), poste(post), motDePasse(mdp) {
        idStaff = ++compteurStaff;
    }

    void afficher() const override {
        cout << "[Staff #" << idStaff << "] ";
        Personne::afficher();
        cout << " | Poste: " << poste << endl;
    }

    int getIdStaff() const { return idStaff; }
    string getPoste() const { return poste; }

    bool verifierMotDePasse(string mdp) const {
        return motDePasse == mdp;
    }
};
// ta hadi b7al d reservation gha n7tajoha mn b3d 
int Staff::compteurStaff = 0;

// fonction bach n7sbo lyali 

int calculerNombreJours(const string& dateDebut, const string& dateFin) {
    int jour1, mois1, annee1;
    int jour2, mois2, annee2;
    char slash;

    stringstream ss1(dateDebut);
    ss1 >> jour1 >> slash >> mois1 >> slash >> annee1;

    stringstream ss2(dateFin);
    ss2 >> jour2 >> slash >> mois2 >> slash >> annee2;

    int totalDays1 = annee1 * 365 + mois1 * 30 + jour1;
    int totalDays2 = annee2 * 365 + mois2 * 30 + jour2;

    return totalDays2 - totalDays1;
}

//Classes Chambre

class Chambre {
protected:
    int numero;
    float prixbase;
    bool estOccupee;
    double pourcentage;

public:
    Chambre(int numero, float prixbase, double pourcentage = 0.0, bool estOccupee = false)
        : numero(numero), prixbase(prixbase), pourcentage(pourcentage), estOccupee(estOccupee) {
    }

    virtual double calculerPrixParNuit() const {
        return prixbase + (prixbase * pourcentage / 100.0);
    }

    double calculerPrixTotal(int nombreJours) const {
        return calculerPrixParNuit() * nombreJours;
    }

    virtual void afficher() const {
        cout << "Chambre " << numero << " [" << getNomType() << "] - "
            << calculerPrixParNuit() << " DH/nuit - "
            << (estOccupee ? "Occupée" : "Libre");
    }

    virtual void afficherDetail() const {
        cout << "Numéro de chambre: " << numero << endl;
        cout << "Type de chambre: " << getNomType() << endl;
        cout << "Prix de base: " << prixbase << " DH" << endl;
        cout << "Prix par nuit: " << calculerPrixParNuit() << " DH" << endl;
        cout << "Est occupée: " << (estOccupee ? "Oui" : "Non") << endl;
        cout << "Pourcentage: " << pourcentage << "%" << endl;
    }

    virtual string getNomType() const = 0;

    void modifier(float nouveauPrixbase, double nouveauPourcentage, bool nouvelleOccupee) {
        this->prixbase = nouveauPrixbase;
        this->pourcentage = nouveauPourcentage;
        this->estOccupee = nouvelleOccupee;
    }

    int getNumero() const { return numero; }
    float getPrixBase() const { return prixbase; }
    double getPourcentage() const { return pourcentage; }
    bool isOccupee() const { return estOccupee; }

    void setPrixbase(float prixbase) { this->prixbase = prixbase; }
    void setEstOccupee(bool estOccupee) { this->estOccupee = estOccupee; }
    void setPourcentage(double pourcentage) { this->pourcentage = pourcentage; }
    void setOccupe(bool occupe) { this->estOccupee = occupe; }

    virtual ~Chambre() {}
};
//prix base khlito hard coded bach nshl 3liya hh (3gzt)
class ChambreSingle : public Chambre {
public:
    ChambreSingle(int numero, float prixbase = 300.0, double pourcentage = 0.0, bool estOccupee = false)
        : Chambre(numero, prixbase, pourcentage, estOccupee) {
    }

    string getNomType() const override { return "Single"; }
};

class ChambreDouble : public Chambre {
public:
    ChambreDouble(int numero, float prixbase = 500.0, double pourcentage = 0.0, bool estOccupee = false)
        : Chambre(numero, prixbase, pourcentage, estOccupee) {
    }

    string getNomType() const override { return "Double"; }
};

class ChambreSuite : public Chambre {
public:
    ChambreSuite(int numero, float prixbase = 800.0, double pourcentage = 0.0, bool estOccupee = false)
        : Chambre(numero, prixbase, pourcentage, estOccupee) {
    }

    string getNomType() const override { return "Suite"; }
};

class ChambreDeluxe : public Chambre {
public:
    ChambreDeluxe(int numero, float prixbase = 1000.0, double pourcentage = 0.0, bool estOccupee = false)
        : Chambre(numero, prixbase, pourcentage, estOccupee) {
    }

    string getNomType() const override { return "Deluxe"; }
};

class ChambrePresidential : public Chambre {
public:
    ChambrePresidential(int numero, float prixbase = 1500.0, double pourcentage = 0.0, bool estOccupee = false)
        : Chambre(numero, prixbase, pourcentage, estOccupee) {
    }

    string getNomType() const override { return "Presidential"; }
};

class ChambreSuiteRoyal : public Chambre {
public:
    ChambreSuiteRoyal(int numero, float prixbase = 2000.0, double pourcentage = 0.0, bool estOccupee = false)
        : Chambre(numero, prixbase, pourcentage, estOccupee) {
    }

    string getNomType() const override { return "Suite Royale"; }
};

//Classe Reservation

class Reservation {
private:
    int numeroReservation;
    string cinClient;
    int numeroChambre;
    string dateDebut;
    string dateFin;
    int nombreJours;
    double prixTotal;
    static int compteurReservations;

public:
    Reservation(string cin, int numChambre, string debut, string fin, int jours, double prix)
        : cinClient(cin), numeroChambre(numChambre),
        dateDebut(debut), dateFin(fin), nombreJours(jours), prixTotal(prix) {
        numeroReservation = ++compteurReservations;
    }

    bool operator==(const Reservation& autre) const {
        return numeroReservation == autre.numeroReservation;
    }

    bool operator<(const Reservation& autre) const {
        return numeroReservation < autre.numeroReservation;
    }

    void afficher() const {
        cout << "Res #" << numeroReservation << " | Ch " << numeroChambre
            << " | " << dateDebut << " -> " << dateFin
            << " | " << nombreJours << " nuits | Total : " << prixTotal << " DH";
    }

    void afficherDetail(const string& nomClient, const string& prenomClient,
        const string& typeChambre) const {
        cout << "\n=== Réservation #" << numeroReservation << " ===" << endl;
        cout << "Client: " << nomClient << " " << prenomClient << endl;
        cout << "Chambre: " << numeroChambre << " [" << typeChambre << "]" << endl;
        cout << "Dates: " << dateDebut << " -> " << dateFin << endl;
        cout << "Nombre de nuits: " << nombreJours << endl;
        cout << "Prix par nuit: " << (prixTotal / nombreJours) << " DH" << endl;
        cout << "Prix total: " << prixTotal << " DH" << endl;
    }

    int getNumeroReservation() const { return numeroReservation; }
    string getCinClient() const { return cinClient; }
    int getNumeroChambre() const { return numeroChambre; }
    string getDateDebut() const { return dateDebut; }
    string getDateFin() const { return dateFin; }
    int getNombreJours() const { return nombreJours; }
    double getPrixTotal() const { return prixTotal; }

    void setDateDebut(string date) { dateDebut = date; }
    void setDateFin(string date) { dateFin = date; }
    void setNumeroChambre(int num) { numeroChambre = num; }
    void setNombreJours(int jours) { nombreJours = jours; }
    void setPrixTotal(double prix) { prixTotal = prix; }
};

//hadi katb9a t incrementa bach ykon 3dna id d kol reservation (chof constructeur bach tfhm)
int Reservation::compteurReservations = 0;

//Classe Hotel

class Hotel {
private:
    vector<shared_ptr<Chambre>> chambres;
    vector<shared_ptr<Staff>> staffMembers;
    vector<shared_ptr<Client>> clients;
    vector<shared_ptr<Reservation>> reservations;

public:
    Hotel() {
        initialiserDonnees();
    }

    void initialiserDonnees() {
        // Initial dyal staff 
        staffMembers.push_back(make_shared<Staff>("Med", "Hamza", "ADMIN001",
            "060000000", "chef li chaf kolchi", "admin123"));

        // Initial dyal chambres
        chambres.push_back(make_shared<ChambreSingle>(101, 300.0));
        chambres.push_back(make_shared<ChambreSingle>(102, 300.0));
        chambres.push_back(make_shared<ChambreDouble>(201, 500.0));
        chambres.push_back(make_shared<ChambreDouble>(202, 500.0));
        chambres.push_back(make_shared<ChambreSuite>(301, 800.0));
        chambres.push_back(make_shared<ChambreDeluxe>(401, 1000.0));
        chambres.push_back(make_shared<ChambrePresidential>(501, 1500.0));
        chambres.push_back(make_shared<ChambreSuiteRoyal>(601, 2000.0));
    }

    // bayna hadi 
    void ajouterClient(shared_ptr<Client> c) {
        clients.push_back(c);
    }

    void ajouterReservation(shared_ptr<Reservation> r) {
        reservations.push_back(r);
    }

    void afficherChambresLibres() const {
        cout << "\n=== Chambres Disponibles ===" << endl;
        bool found = false;
        for (const auto& chambre : chambres) {
            if (!chambre->isOccupee()) {
                chambre->afficher();
                cout << endl;
                found = true;
            }
        }
        if (!found) cout << "Aucune chambre libre." << endl;
    }

    void afficherToutesChambres() const {
        cout << "\n=== Toutes les Chambres ===" << endl;
        for (const auto& chambre : chambres) {
            chambre->afficher();
            cout << endl;
        }
    }
    //hadi f staff katbynlk ga3 les reservation
    void afficherToutesReservations() const {
        cout << "\n=== Toutes les Réservations ===" << endl;
        if (reservations.empty()) {
            cout << "Aucune réservation." << endl;
            return;
        }
        for (const auto& res : reservations) {
            auto client = trouverClientParCIN(res->getCinClient());
            string nomClient = client ? client->getNom() : "Inconnu";
            string prenomClient = client ? client->getPrenom() : "Inconnu";
            auto chambre = trouverChambre(res->getNumeroChambre());
            string typeChambre = chambre ? chambre->getNomType() : "Inconnu";
            res->afficherDetail(nomClient, prenomClient, typeChambre);
            cout << endl;
        }
    }

    //brasi ma3arfch hadchi ms lmohim c q khdam yay hhh
    shared_ptr<Chambre> trouverChambre(int numero) const {
        auto it = find_if(chambres.begin(), chambres.end(),
            [numero](const shared_ptr<Chambre>& ch) {
                return ch->getNumero() == numero;
            });
        return (it != chambres.end()) ? *it : nullptr;
    }

    shared_ptr<Client> trouverClientParCIN(string cin) const {
        auto it = find_if(clients.begin(), clients.end(),
            [&cin](const shared_ptr<Client>& cl) {
                return cl->getCin() == cin;
            });
        return (it != clients.end()) ? *it : nullptr;
    }

    shared_ptr<Staff> trouverStaffParMotDePasse(string mdp) {
        auto it = find_if(staffMembers.begin(), staffMembers.end(),
            [&mdp](const shared_ptr<Staff>& st) {
                return st->verifierMotDePasse(mdp);
            });
        return (it != staffMembers.end()) ? *it : nullptr;
    }

    shared_ptr<Reservation> trouverReservation(int numero) {
        auto it = find_if(reservations.begin(), reservations.end(),
            [numero](const shared_ptr<Reservation>& res) {
                return res->getNumeroReservation() == numero;
            });
        return (it != reservations.end()) ? *it : nullptr;
    }
    //hadi gha d menu client 
    void afficherReservationsClient(shared_ptr<Client> client) const {
        cout << "\n=== Vos Réservations ===" << endl;
        bool found = false;
        for (const auto& res : reservations) {
            if (res->getCinClient() == client->getCin()) {
                res->afficher();
                cout << endl;
                found = true;
            }
        }
        if (!found) cout << "Aucune réservation trouvée." << endl;
    }
    //hadi kat3tiha num d res katchof wach kayna kaylibiri dik room li dakhla f res w kayms7 babaha reservation mn vecteur  
    bool annulerReservation(int numeroReservation) {
        auto res = trouverReservation(numeroReservation);
        if (!res) {
            cout << "Réservation introuvable!" << endl;
            return false;
        }

        auto chambre = trouverChambre(res->getNumeroChambre());
        if (chambre) {
            chambre->setOccupe(false);
        }

        auto it = remove_if(reservations.begin(), reservations.end(),
            [numeroReservation](const shared_ptr<Reservation>& r) {
                return r->getNumeroReservation() == numeroReservation;
            });

        if (it != reservations.end()) {
            reservations.erase(it, reservations.end());
            cout << "\n✓ Réservation #" << numeroReservation << " annulée avec succès!" << endl;
            return true;
        }

        return false;
    }
    // katmodifier porcentage bach tl3 prix total dyal chambre (brix bas hard coded)
    bool modifierPourcentage(int numero, double nouveauPourcentage) {
        auto chambre = trouverChambre(numero);
        if (!chambre) {
            cout << "Chambre introuvable!" << endl;
            return false;
        }

        double ancienPrix = chambre->calculerPrixParNuit();
        chambre->setPourcentage(nouveauPourcentage);

        cout << "\nChambre " << numero << " - Prix par nuit: " << ancienPrix
            << " DH -> " << chambre->calculerPrixParNuit() << " DH" << endl;
        return true;
    }
};

//Menu

//machi chi7aja gha bach fach dkhl l monu tms7 dakchi li 9bl 
void clearScreen() {
#ifdef _WIN32
    system("cls");
#endif
}

void afficherMenuPrincipal() {
    cout << "\n╔════════════════════════════════════════╗" << endl;
    cout << "║   SYSTEME DE GESTION HOTELIERE        ║" << endl;
    cout << "║        VERSION ALPHA                   ║" << endl;
    cout << "╚════════════════════════════════════════╝" << endl;
    cout << "[1] Connexion Client" << endl;
    cout << "[2] Connexion Staff" << endl;
    cout << "[0] Quitter" << endl;
    cout << "\nChoix: ";
}

void afficherMenuClient() {
    cout << "\n╔════════════════════════════════════════╗" << endl;
    cout << "║          MENU CLIENT                   ║" << endl;
    cout << "╚════════════════════════════════════════╝" << endl;
    cout << "[1] Faire une réservation" << endl;
    cout << "[2] Voir mes réservations" << endl;
    cout << "[3] Annuler une réservation" << endl;
    cout << "[0] Retour menu principal" << endl;
    cout << "\nChoix: ";
}

void afficherMenuStaff() {
    cout << "\n╔════════════════════════════════════════╗" << endl;
    cout << "║          MENU STAFF                    ║" << endl;
    cout << "╚════════════════════════════════════════╝" << endl;
    cout << "[1] Voir toutes les chambres" << endl;
    cout << "[2] Voir toutes les réservations" << endl;
    cout << "[3] Modifier tarification" << endl;
    cout << "[4] Annuler une réservation" << endl;
    cout << "[0] Retour menu principal" << endl;
    cout << "\nChoix: ";
}

void menuClient(Hotel& hotel, shared_ptr<Client> client) {
    int choix;
    do {
        clearScreen();
        cout << "\nBienvenue " << *client << "!" << endl;
        afficherMenuClient();
        cin >> choix;
        cin.ignore();

        switch (choix) {
        case 1: {
            hotel.afficherChambresLibres();

            int numChambre;
            string dateDebut, dateFin;

            cout << "\nNuméro de chambre: ";
            cin >> numChambre;
            cin.ignore();

            auto chambre = hotel.trouverChambre(numChambre);
            if (!chambre) {
                cout << "Chambre introuvable!" << endl;
                break;
            }

            if (chambre->isOccupee()) {
                cout << "Chambre déjà occupée!" << endl;
                break;
            }

            cout << "Date de début (jj/mm/aaaa): ";
            getline(cin, dateDebut);
            cout << "Date de fin (jj/mm/aaaa): ";
            getline(cin, dateFin);

            int nombreJours = calculerNombreJours(dateDebut, dateFin);
            if (nombreJours <= 0) {
                cout << "Dates invalides!" << endl;
                break;
            }


            double prixParNuit = chambre->calculerPrixParNuit();
            double prixTotal = chambre->calculerPrixTotal(nombreJours);

            auto res = make_shared<Reservation>(client->getCin(), numChambre, dateDebut, dateFin, nombreJours, prixTotal);
            hotel.ajouterReservation(res);
            chambre->setOccupe(true);

            cout << "\n✓ Réservation créée avec succès!" << endl;
            cout << "Numéro de réservation: " << res->getNumeroReservation() << endl;
            cout << "Chambre: " << numChambre << " [" << chambre->getNomType() << "]" << endl;
            cout << "Période: " << dateDebut << " -> " << dateFin << endl;
            cout << "Nombre de nuits: " << nombreJours << endl;
            cout << "Prix par nuit: " << prixParNuit << " DH" << endl;
            cout << "Prix total: " << prixTotal << " DH" << endl;
            break;
        }

        case 2: {
            hotel.afficherReservationsClient(client);
            break;
        }

        case 3: {
            hotel.afficherReservationsClient(client);

            int numReservation;
            cout << "\nNuméro de réservation à annuler: ";
            cin >> numReservation;
            cin.ignore();

            int confirmation;
            cout << "Êtes-vous sûr de vouloir annuler la réservation #" << numReservation << "?" << endl;
            cout << "[1] Oui, annuler" << endl;
            cout << "[0] Non, retour" << endl;
            cout << "Choix: ";
            cin >> confirmation;
            cin.ignore();

            if (confirmation == 1) {
                hotel.annulerReservation(numReservation);
            }
            else {
                cout << "Annulation annulée." << endl;
            }
            break;
        }

        case 0:
            cout << "Retour au menu principal..." << endl;
            break;

        default:
            cout << "❌ Choix invalide!" << endl;
        }

        if (choix != 0) {
            cout << "\nAppuyez sur Entrée pour continuer...";
            cin.get();
        }
    } while (choix != 0);
}

void menuStaff(Hotel& hotel, shared_ptr<Staff> staff) {
    int choix;
    do {
        clearScreen();
        cout << "\nBienvenue " << *staff << " [" << staff->getPoste() << "]" << endl;
        afficherMenuStaff();
        cin >> choix;
        cin.ignore();

        switch (choix) {
        case 1:
            hotel.afficherToutesChambres();
            break;

        case 2:
            hotel.afficherToutesReservations();
            break;

        case 3: {
            hotel.afficherToutesChambres();

            int numChambre;
            double nouveauPourcentage;

            cout << "\nNuméro de chambre: ";
            cin >> numChambre;
            cout << "Nouveau pourcentage de majoration: ";
            cin >> nouveauPourcentage;
            cin.ignore();

            hotel.modifierPourcentage(numChambre, nouveauPourcentage);
            break;
        }

        case 4: {
            hotel.afficherToutesReservations();

            int numReservation;
            cout << "\nNuméro de réservation à annuler: ";
            cin >> numReservation;
            cin.ignore();

            int confirmation;
            cout << "Êtes-vous sûr de vouloir annuler la réservation #" << numReservation << "?" << endl;
            cout << "[1] Oui, annuler" << endl;
            cout << "[0] Non, retour" << endl;
            cout << "Choix: ";
            cin >> confirmation;
            cin.ignore();

            if (confirmation == 1) {
                hotel.annulerReservation(numReservation);
            }
            else {
                cout << "Annulation annulée." << endl;
            }
            break;
        }


        case 0:
            cout << "Retour au menu principal..." << endl;
            break;

        default:
            cout << "Choix invalide!" << endl;
        }

        if (choix != 0) {
            cout << "\nAppuyez sur Entrée pour continuer...";
            cin.get();
        }
    } while (choix != 0);
}

int main() {
    Hotel hotel;
    int choix;
    do {
        clearScreen();
        afficherMenuPrincipal();
        cin >> choix;
        cin.ignore();

        switch (choix) {
        case 1: {
            string cinInput;
            cout << "\nEntrez votre CIN: ";
            getline(cin, cinInput);

            auto client = hotel.trouverClientParCIN(cinInput);
            if (client) {
                menuClient(hotel, client);
            }
            else {
                cout << "\n=== Nouveau Client ===" << endl;
                string nom, prenom, tel;
                cout << "Nom: ";
                getline(cin, nom);
                cout << "Prénom: ";
                getline(cin, prenom);
                cout << "Téléphone: ";
                getline(cin, tel);

                auto nouveauClient = make_shared<Client>(nom, prenom, cinInput, tel);
                hotel.ajouterClient(nouveauClient);
                cout << "\nCompte créé" << endl;
                cout << "\nAppuyez sur Entrée pour continuer...";
                cin.get();
                menuClient(hotel, nouveauClient);
            }
            break;
        }

        case 2: {
            string mdp;
            cout << "\n Mot de passe: ";
            getline(cin, mdp);

            auto staff = hotel.trouverStaffParMotDePasse(mdp);
            if (staff) {
                menuStaff(hotel, staff);
            }
            else {
                cout << "Mot de passe incorrect!" << endl;
                cout << "\nAppuyez sur Entrée pour continuer...";
                cin.get();
            }
            break;
        }
        default:
            cout << "Choix invalide!" << endl;
            cout << "\nAppuyez sur Entrée pour continuer...";
            cin.get();
        }
    } while (choix != 0);

    return 0;
}
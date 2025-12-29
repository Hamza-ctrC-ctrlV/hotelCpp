#include <iostream>
#include <string>
#include <vector>
#include <memory>
#include <algorithm>
#include <sstream>
#include <ctime>
#include <fstream>
#include <iomanip>
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
        cout << "Name: " << nom << " " << prenom
            << " | CIN: " << cin
            << " | Phone: " << telephone;
    }

    bool operator==(const Personne& autre) const {
        return cin == autre.cin;
    }

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
    void setCin(string c) { cin = c; }
    void setTelephone(string t) { telephone = t; }

    virtual ~Personne() {}
};

class Client : public Personne {
private:
    int numeroClient;
    string motDePasse;
    int tentativesEchouees;
    bool compteActif;
    static int compteurClients;

public:
    Client(string n, string p, string c, string tel, string mdp = "1234",
        int tentatives = 0, bool actif = true)
        : Personne(n, p, c, tel), motDePasse(mdp),
        tentativesEchouees(tentatives), compteActif(actif) {
        numeroClient = ++compteurClients;
    }

    void afficher() const override {
        cout << "[Client #" << numeroClient << "] ";
        Personne::afficher();
        cout << " | Status: " << (compteActif ? "Active" : "BLOCKED") << endl;
    }

    bool operator<(const Client& autre) const {
        return numeroClient < autre.numeroClient;
    }

    int getNumeroClient() const { return numeroClient; }
    string getMotDePasse() const { return motDePasse; }
    int getTentativesEchouees() const { return tentativesEchouees; }
    bool isCompteActif() const { return compteActif; }

    void setMotDePasse(string mdp) { motDePasse = mdp; }
    void setCompteActif(bool actif) { compteActif = actif; }

    void incrementerTentatives() {
        tentativesEchouees++;
        if (tentativesEchouees >= 5) {
            compteActif = false;
        }
    }

    void reinitialiserTentatives() { tentativesEchouees = 0; }

    bool verifierMotDePasse(string mdp) const {
        return motDePasse == mdp;
    }

    string toFileString() const {
        return to_string(numeroClient) + "|" + getNom() + "|" +
            getPrenom() + "|" + getCin() + "|" + getTelephone() + "|" +
            motDePasse + "|" + to_string(tentativesEchouees) + "|" +
            (compteActif ? "Yes" : "No");
    }

    static shared_ptr<Client> fromFileString(const string& line) {
        stringstream ss(line);
        string numStr, nom, prenom, cin, tel, mdp, tentStr, actifStr;
        getline(ss, numStr, '|');
        getline(ss, nom, '|');
        getline(ss, prenom, '|');
        getline(ss, cin, '|');
        getline(ss, tel, '|');
        getline(ss, mdp, '|');
        getline(ss, tentStr, '|');
        getline(ss, actifStr, '|');

        auto client = make_shared<Client>(nom, prenom, cin, tel, mdp,
            stoi(tentStr), (actifStr == "Yes"));
        if (stoi(numStr) > compteurClients) {
            compteurClients = stoi(numStr);
        }
        return client;
    }
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
        cout << " | Position: " << poste << endl;
    }

    int getIdStaff() const { return idStaff; }
    string getPoste() const { return poste; }

    void setIdStaff(int id) { idStaff = id; }
    void setPoste(string p) { poste = p; }
    void setMotDePasse(string mdp) { motDePasse = mdp; }

    bool verifierMotDePasse(string mdp) const {
        return motDePasse == mdp;
    }

    string toFileString() const {
        return to_string(idStaff) + "|" + getNom() + "|" +
            getPrenom() + "|" + getCin() + "|" + getTelephone() + "|" +
            poste + "|" + motDePasse;
    }

    static shared_ptr<Staff> fromFileString(const string& line) {
        stringstream ss(line);
        string idStr, nom, prenom, cin, tel, post, mdp;
        getline(ss, idStr, '|');
        getline(ss, nom, '|');
        getline(ss, prenom, '|');
        getline(ss, cin, '|');
        getline(ss, tel, '|');
        getline(ss, post, '|');
        getline(ss, mdp, '|');

        auto staff = make_shared<Staff>(nom, prenom, cin, tel, post, mdp);
        if (stoi(idStr) > compteurStaff) {
            compteurStaff = stoi(idStr);
        }
        return staff;
    }


};

int Staff::compteurStaff = 0;

void clearScreen();

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
        cout << "Room " << numero << " [" << getNomType() << "] - "
            << calculerPrixParNuit() << " DH/night - "
            << (estOccupee ? "Occupied" : "Available");
    }

    virtual void afficherDetail() const {
        cout << "Room Number: " << numero << endl;
        cout << "Room Type: " << getNomType() << endl;
        cout << "Base Price: " << prixbase << " DH" << endl;
        cout << "Price per Night: " << calculerPrixParNuit() << " DH" << endl;
        cout << "Status: " << (estOccupee ? "Occupied" : "Available") << endl;
        cout << "Percentage: " << pourcentage << "%" << endl;
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

    string toFileString() const {
        return getNomType() + "|" + to_string(numero) + "|" +
            to_string(prixbase) + "|" + to_string(pourcentage) + "|" +
            (estOccupee ? "Yes" : "No");
    }

    virtual ~Chambre() {}
};

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
    int numeroClient;
    int numeroChambre;
    string dateDebut;
    string dateFin;
    int nombreJours;
    double prixTotal;
    static int compteurReservations;

public:
    Reservation(int numClient, int numChambre, string debut, string fin, int jours, double prix)
        : numeroClient(numClient), numeroChambre(numChambre),
        dateDebut(debut), dateFin(fin), nombreJours(jours), prixTotal(prix) {
        numeroReservation = ++compteurReservations;
    }

    bool operator==(const Reservation& autre) const {
        return numeroReservation == autre.numeroReservation;
    }

    bool operator<(const Reservation& autre) const {
        return numeroReservation < autre.numeroReservation;
    }

    void afficher(const Client& c) const {
        cout << "Res #" << numeroReservation
            << " | Guest: " << c.getNom() << " " << c.getPrenom()
            << " | Room " << numeroChambre
            << " | " << dateDebut << " -> " << dateFin
            << " | " << nombreJours << " nights | Total: " << prixTotal << " DH";
    }

    void afficherDetail(const string& nomClient, const string& prenomClient,
        const string& typeChambre) const {
        cout << "\n=== Reservation #" << numeroReservation << " ===" << endl;
        cout << "Client: " << nomClient << " " << prenomClient << " (Client #" << numeroClient << ")" << endl;
        cout << "Room: " << numeroChambre << " [" << typeChambre << "]" << endl;
        cout << "Dates: " << dateDebut << " -> " << dateFin << endl;
        cout << "Number of nights: " << nombreJours << endl;
        cout << "Price per night: " << (prixTotal / nombreJours) << " DH" << endl;
        cout << "Total price: " << prixTotal << " DH" << endl;
    }

    int getNumeroReservation() const { return numeroReservation; }
    int getNumeroClient() const { return numeroClient; }
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

    string toFileString() const {
        return to_string(numeroReservation) + "|" + to_string(numeroClient) + "|" +
            to_string(numeroChambre) + "|" + dateDebut + "|" + dateFin + "|" +
            to_string(nombreJours) + "|" + to_string(prixTotal);
    }

    static shared_ptr<Reservation> fromFileString(const string& line) {
        stringstream ss(line);
        string numStr, numClientStr, numChStr, debut, fin, joursStr, prixStr;
        getline(ss, numStr, '|');
        getline(ss, numClientStr, '|');
        getline(ss, numChStr, '|');
        getline(ss, debut, '|');
        getline(ss, fin, '|');
        getline(ss, joursStr, '|');
        getline(ss, prixStr, '|');

        auto res = make_shared<Reservation>(
            stoi(numClientStr),
            stoi(numChStr),
            debut,
            fin,
            stoi(joursStr),
            stod(prixStr)
        );

        if (stoi(numStr) > compteurReservations) {
            compteurReservations = stoi(numStr);
        }

        return res;
    }
};

int Reservation::compteurReservations = 0;

//Classe Hotel

class Hotel {
private:
    vector<shared_ptr<Chambre>> chambres;
    vector<shared_ptr<Staff>> staffMembers;
    vector<shared_ptr<Client>> clients;
    vector<shared_ptr<Reservation>> reservations;
    const string FICHIER_RESERVATIONS = "reservations.txt";
    const string FICHIER_CHAMBRES = "chambres.txt";
    const string FICHIER_CLIENTS = "clients.txt";
    const string FICHIER_STAFF = "staff.txt";

public:
    Hotel() {
        chargerStaff();
        chargerChambres();
        chargerClients();
        chargerReservations();
    }

    void chargerStaff() {
        ifstream file(FICHIER_STAFF.c_str());
        if (!file.is_open()) {
            return;
        }

        string line;
        // Skip header lines
        while (getline(file, line)) {
            if (line.empty() || line[0] == '#') {
                continue;
            }
            staffMembers.push_back(Staff::fromFileString(line));
        }
        file.close();
    }

    void chargerChambres() {
        ifstream file(FICHIER_CHAMBRES.c_str());
        if (!file.is_open()) {
            return;
        }

        string line;
        // Skip header lines
        while (getline(file, line)) {
            if (line.empty() || line[0] == '#') {
                continue;
            }

            stringstream ss(line);
            string type, numStr, prixStr, pourcStr, occStr;
            getline(ss, type, '|');
            getline(ss, numStr, '|');
            getline(ss, prixStr, '|');
            getline(ss, pourcStr, '|');
            getline(ss, occStr, '|');

            int num = stoi(numStr);
            float prix = stof(prixStr);
            double pourc = stod(pourcStr);
            bool occ = (occStr == "Yes");

            shared_ptr<Chambre> chambre;
            if (type == "Single") {
                chambre = make_shared<ChambreSingle>(num, prix, pourc, occ);
            }
            else if (type == "Double") {
                chambre = make_shared<ChambreDouble>(num, prix, pourc, occ);
            }
            else if (type == "Suite") {
                chambre = make_shared<ChambreSuite>(num, prix, pourc, occ);
            }
            else if (type == "Deluxe") {
                chambre = make_shared<ChambreDeluxe>(num, prix, pourc, occ);
            }
            else if (type == "Presidential") {
                chambre = make_shared<ChambrePresidential>(num, prix, pourc, occ);
            }
            else if (type == "Suite Royale") {
                chambre = make_shared<ChambreSuiteRoyal>(num, prix, pourc, occ);
            }

            if (chambre) {
                chambres.push_back(chambre);
            }
        }
        file.close();
    }

    void chargerClients() {
        ifstream file(FICHIER_CLIENTS.c_str());
        if (!file.is_open()) {
            return;
        }

        string line;
        // Skip header lines
        while (getline(file, line)) {
            if (line.empty() || line[0] == '#') {
                continue;
            }

            clients.push_back(Client::fromFileString(line));
        }

        file.close();
    }


    void chargerReservations() {
        ifstream file(FICHIER_RESERVATIONS.c_str());
        if (!file.is_open()) {
            return;
        }

        string line;
        // Skip header lines
        while (getline(file, line)) {
            if (line.empty() || line[0] == '#') {
                continue;
            }
            reservations.push_back(Reservation::fromFileString(line));
        }
        file.close();
    }

    void sauvegarderStaff() {
        ofstream file(FICHIER_STAFF.c_str());
        file << "# STAFF DATA FILE" << endl;
        file << "# Staff ID|surname|name|cin|phone number|position|password" << endl;
        for (const auto& staff : staffMembers) {
            file << staff->toFileString() << endl;
        }
        file.close();
    }

    void sauvegarderChambres() {
        ofstream file(FICHIER_CHAMBRES.c_str());
        file << "# ROOMS DATA FILE" << endl;
        file << "# Room type|room number|base price|percentage|occupied status" << endl;
        for (const auto& chambre : chambres) {
            file << chambre->toFileString() << endl;
        }
        file.close();
    }

    void sauvegarderClients() {
        ofstream file(FICHIER_CLIENTS.c_str());
        file << "# CLIENTS DATA FILE" << endl;
        file << "# Client ID|surname|name|cin|phone number|password|failed attempts|active account" << endl;

        for (const auto& client : clients) {
            file << client->toFileString() << endl;
        }

        file.close();
    }


    void sauvegarderReservations() {
        ofstream file(FICHIER_RESERVATIONS.c_str());
        file << "# RESERVATIONS DATA FILE" << endl;
        file << "# Reservation ID|Client ID|Room number|start date|end date|number of days|total price" << endl;
        for (const auto& res : reservations) {
            file << res->toFileString() << endl;
        }
        file.close();
    }

    void sauvegarderTout() {
        sauvegarderStaff();
        sauvegarderChambres();
        sauvegarderClients();
        sauvegarderReservations();
    }

    void ajouterClient(shared_ptr<Client> c) {
        clients.push_back(c);
        sauvegarderClients();
    }

    void ajouterStaff(shared_ptr<Staff> s) {
        staffMembers.push_back(s);
        sauvegarderStaff();
    }

    void ajouterChambre(shared_ptr<Chambre> c) {
        chambres.push_back(c);
        sauvegarderChambres();
    }

    void ajouterReservation(shared_ptr<Reservation> r) {
        reservations.push_back(r);
    }

    void afficherChambresLibres() const {
        cout << "\n=== Available Rooms ===" << endl;
        bool found = false;
        for (const auto& chambre : chambres) {
            if (!chambre->isOccupee()) {
                chambre->afficher();
                cout << endl;
                found = true;
            }
        }
        if (!found) cout << "No available rooms." << endl;
    }

    vector<shared_ptr<Chambre>> getChambresLibres() const {
        vector<shared_ptr<Chambre>> libres;
        for (const auto& chambre : chambres) {
            if (!chambre->isOccupee()) {
                libres.push_back(chambre);
            }
        }
        return libres;
    }

    void afficherToutesChambres() const {
        cout << "\n=== All Rooms ===" << endl;
        for (const auto& chambre : chambres) {
            chambre->afficher();
            cout << endl;
        }
    }

    void afficherToutesReservations() const {
        cout << "\n=== All Reservations ===" << endl;
        if (reservations.empty()) {
            cout << "No reservations." << endl;
            return;
        }
        for (const auto& res : reservations) {
            auto client = trouverClientParNumero(res->getNumeroClient());
            string nomClient = client ? client->getNom() : "Unknown";
            string prenomClient = client ? client->getPrenom() : "Unknown";
            auto chambre = trouverChambre(res->getNumeroChambre());
            string typeChambre = chambre ? chambre->getNomType() : "Unknown";
            res->afficherDetail(nomClient, prenomClient, typeChambre);
            cout << endl;
        }
    }

    void afficherTousClients() const {
        cout << "\n=== All Clients ===" << endl;
        if (clients.empty()) {
            cout << "No registered clients." << endl;
            return;
        }
        for (const auto& client : clients) {
            client->afficher();
        }
    }

    void afficherTousStaff() const {
        cout << "\n=== All Staff Members ===" << endl;
        if (staffMembers.empty()) {
            cout << "No staff members." << endl;
            return;
        }
        for (const auto& staff : staffMembers) {
            staff->afficher();
        }
    }

    shared_ptr<Chambre> trouverChambre(int numero) const {
        auto it = find_if(chambres.begin(), chambres.end(),
            [numero](const shared_ptr<Chambre>& ch) {
                return ch->getNumero() == numero;
            });
        return (it != chambres.end()) ? *it : nullptr;
    }

    shared_ptr<Client> trouverClientParNumero(int numero) const {
        auto it = find_if(clients.begin(), clients.end(),
            [numero](const shared_ptr<Client>& cl) {
                return cl->getNumeroClient() == numero;
            });
        return (it != clients.end()) ? *it : nullptr;
    }

    shared_ptr<Client> trouverClientParCIN(string cin) const {
        auto it = find_if(clients.begin(), clients.end(),
            [&cin](const shared_ptr<Client>& cl) {
                return cl->getCin() == cin;
            });
        return (it != clients.end()) ? *it : nullptr;
    }

    shared_ptr<Staff> trouverStaffParId(int id) {
        auto it = find_if(staffMembers.begin(), staffMembers.end(),
            [id](const shared_ptr<Staff>& st) {
                return st->getIdStaff() == id;
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

    void afficherReservationsClient(shared_ptr<Client> client) const {
        cout << "\n=== Your Reservations ===" << endl;
        bool found = false;
        for (const auto& res : reservations) {
            if (res->getNumeroClient() == client->getNumeroClient()) {
                res->afficher(*client);
                cout << endl;
                found = true;
            }
        }
        if (!found) cout << "No reservations found." << endl;
    }

    bool annulerReservation(int numeroReservation) {
        auto res = trouverReservation(numeroReservation);
        if (!res) {
            cout << "Reservation not found!" << endl;
            return false;
        }

        auto chambre = trouverChambre(res->getNumeroChambre());
        if (chambre) {
            chambre->setOccupe(false);
            sauvegarderChambres();
        }

        auto it = remove_if(reservations.begin(), reservations.end(),
            [numeroReservation](const shared_ptr<Reservation>& r) {
                return r->getNumeroReservation() == numeroReservation;
            });

        if (it != reservations.end()) {
            reservations.erase(it, reservations.end());
            sauvegarderReservations();
            cout << "\n✓ Reservation #" << numeroReservation << " cancelled successfully!" << endl;
            return true;
        }

        return false;
    }

    bool modifierPourcentage(int numero, double nouveauPourcentage) {
        auto chambre = trouverChambre(numero);
        if (!chambre) {
            cout << "Room not found!" << endl;
            return false;
        }

        double ancienPrix = chambre->calculerPrixParNuit();
        chambre->setPourcentage(nouveauPourcentage);
        sauvegarderChambres();

        cout << "\nRoom " << numero << " - Price per night: " << ancienPrix
            << " DH -> " << chambre->calculerPrixParNuit() << " DH" << endl;
        return true;
    }

    bool modifierPrixBase(int numero, float nouveauPrix) {
        auto chambre = trouverChambre(numero);
        if (!chambre) {
            cout << "Room not found!" << endl;
            return false;
        }

        float ancienPrix = chambre->getPrixBase();
        chambre->setPrixbase(nouveauPrix);
        sauvegarderChambres();

        cout << "\nRoom " << numero << " - Base price: " << ancienPrix
            << " DH -> " << nouveauPrix << " DH" << endl;
        return true;
    }

    bool modifierInformationsClient(int numeroClient) {
        auto client = trouverClientParNumero(numeroClient);
        if (!client) {
            cout << "\n❌ Client not found with number: " << numeroClient << endl;
            return false;
        }

        cout << "\n=== Modifying Client #" << client->getNumeroClient() << " ===" << endl;
        client->afficher();

        int choix;
        do {
            cout << "\n[1] Modify Name" << endl;
            cout << "[2] Modify Surname" << endl;
            cout << "[3] Modify CIN" << endl;
            cout << "[4] Modify Phone" << endl;
            cout << "[5] Modify Password" << endl;
            cout << "[6] Activate/Deactivate account" << endl;
            cout << "[7] Reset login attempts" << endl;
            cout << "[0] Finish modifications" << endl;
            cout << "\nChoice: ";
            cin >> choix;
            cin.ignore();

            switch (choix) {
            case 1: {
                string nouveauNom;
                cout << "New name: ";
                getline(cin, nouveauNom);
                client->setNom(nouveauNom);
                cout << "✓ Name modified!" << endl;
                break;
            }
            case 2: {
                string nouveauPrenom;
                cout << "New surname: ";
                getline(cin, nouveauPrenom);
                client->setPrenom(nouveauPrenom);
                cout << "✓ Surname modified!" << endl;
                break;
            }
            case 3: {
                string nouveauCIN;
                cout << "New CIN: ";
                getline(cin, nouveauCIN);
                client->setCin(nouveauCIN);
                cout << "✓ CIN modified!" << endl;
                break;
            }
            case 4: {
                string nouveauTel;
                cout << "New phone: ";
                getline(cin, nouveauTel);
                client->setTelephone(nouveauTel);
                cout << "✓ Phone modified!" << endl;
                break;
            }
            case 5: {
                string nouveauMdp;
                cout << "New password: ";
                getline(cin, nouveauMdp);
                client->setMotDePasse(nouveauMdp);
                cout << "✓ Password modified!" << endl;
                break;
            }
            case 6: {
                bool nouvelEtat = !client->isCompteActif();
                client->setCompteActif(nouvelEtat);
                cout << "✓ Account " << (nouvelEtat ? "activated" : "deactivated") << "!" << endl;
                break;
            }
            case 7: {
                client->reinitialiserTentatives();
                client->setCompteActif(true);
                cout << "✓ Attempts reset and account reactivated!" << endl;
                break;
            }
            case 0:
                cout << "Modifications completed." << endl;
                break;
            default:
                cout << "Invalid choice!" << endl;
            }
        } while (choix != 0);

        sauvegarderClients();
        cout << "\n✓ Modifications saved!" << endl;
        return true;
    }

    shared_ptr<Client> connexionClientAvecMotDePasse(string cin, string motDePasse) {
        auto client = trouverClientParCIN(cin);

        if (!client) {
            return nullptr;
        }

        if (!client->isCompteActif()) {
            cout << "\n❌ ACCOUNT BLOCKED!" << endl;
            cout << "Your account has been blocked due to too many failed attempts." << endl;
            cout << "Please contact administration to unlock your account." << endl;
            cout << "\nPress Enter to continue...";
            std::cin.get();
            return nullptr;
        }

        if (!client->verifierMotDePasse(motDePasse)) {
            client->incrementerTentatives();
            sauvegarderClients();

            int tentativesRestantes = 5 - client->getTentativesEchouees();

            if (client->isCompteActif()) {
                cout << "\n❌ Incorrect password!" << endl;
                cout << "⚠️  Remaining attempts: " << tentativesRestantes << "/5" << endl;
            }
            else {
                cout << "\n❌ ACCOUNT BLOCKED!" << endl;
                cout << "You have exceeded the maximum number of attempts (5)." << endl;
                cout << "Please contact administration to unlock your account." << endl;
            }
            cout << "\nPress Enter to continue...";
            std::cin.get();
            return nullptr;
        }

        client->reinitialiserTentatives();
        sauvegarderClients();
        return client;
    }

    shared_ptr<Staff> connexionStaffAvecMotDePasse(int idStaff, string motDePasse) {
        auto staff = trouverStaffParId(idStaff);

        if (!staff) {
            return nullptr;
        }

        if (!staff->verifierMotDePasse(motDePasse)) {
            cout << "\n❌ Incorrect password!" << endl;
            cout << "\nPress Enter to continue...";
            cin.get();
            return nullptr;
        }

        return staff;
    }

    void HumanRessourcesManagement(shared_ptr<Staff> staff) {
        clearScreen();
        afficherTousStaff();

        int id;
        cout << "\nStaff ID to manage: ";
        cin >> id;
        cin.ignore();

    auto staffMember = trouverStaffParId(id);
        if (!staffMember) {
            cout << "Staff member not found!" << endl;
            return;
        }

        cout << "Enter the password for staff member " << id << ": ";
        string password;
        getline(cin, password);

        if (!staffMember->verifierMotDePasse(password)) {
            cout << "Incorrect password! Access denied." << endl;
            return;
        }

        int choix;
        do {
            cout << "\n=== Managing Staff " << id << " ===" << endl;
            cout << "[1] Modify ID" << endl;
            cout << "[2] Modify poste" << endl;
            cout << "[3] Modify password" << endl;
            cout << "[4] Modify name" << endl;
            cout << "[5] Modify surname" << endl;
            cout << "[0] Return" << endl;
            cout << "\nChoice: ";
            cin >> choix;
            cin.ignore();

            switch (choix) {
            case 1: {
                int newid;
                cout << "New ID: ";
                cin >> newid;
                cin.ignore();
                staffMember->setIdStaff(newid);
                sauvegarderStaff();
                cout << "✓ ID modified!" << endl;
                break;
            }
            case 2: {
                string newposte;
                cout << "New poste: ";
                getline(cin, newposte);
                staffMember->setPoste(newposte);
                sauvegarderStaff();
                cout << "✓ Poste modified!" << endl;
                break;
            }
            case 3: {
                string newpassword;
                cout << "New password: ";
                getline(cin, newpassword);
                staffMember->setMotDePasse(newpassword);
                sauvegarderStaff();
                cout << "✓ Password modified!" << endl;
                break;
            }

            case 4: {
                string newname;
                cout << "New name: ";
                getline(cin, newname);
                staffMember->setNom(newname);
                sauvegarderStaff();
                cout << "✓ Name modified!" << endl;
                break;
            }

            case 5: {
                string newsurname;
                cout << "New surname: ";
                getline(cin, newsurname);
                staffMember->setPrenom(newsurname);
                sauvegarderStaff();
                cout << "✓ Surname modified!" << endl;
                break;
            }
            case 0:
                break;
            default:
                cout << "Invalid choice!" << endl;
            }
        } while (choix != 0);
    }


};

void clearScreen() {
#ifdef _WIN32
    system("cls");
#else
    system("clear");
#endif
}

void printCentered(const string& text, int width = 80) {
    int padding = (width - text.length()) / 2;
    cout << string(padding, ' ') << text << endl;
}

void printLine(char c = '=', int width = 80) {
    cout << string(width, c) << endl;
}

void afficherMenuPrincipal() {
    clearScreen();
    cout << endl;
    printLine();
    printCentered("HOTEL MANAGEMENT SYSTEM");
    printCentered("VERSION 2.0");
    printLine();
    cout << endl;
    printCentered("[1] Client Login");
    printCentered("[2] Staff Login");
    printCentered("[0] Exit");
    cout << endl;
    printLine();
    cout << "\n";
    printCentered("Choice: ", 40);
    cout << "                                        ";
}

void afficherMenuClient() {
    cout << endl;
    printLine();
    printCentered("CLIENT MENU");
    printLine();
    cout << endl;
    printCentered("[1] Reservation Request Entry");
    printCentered("[2] View Reservation File");
    printCentered("[3] Update Reservation");
    printCentered("[4] Reservation Cancellation Procedure");
    printCentered("[0] Return to Main Menu");
    cout << endl;
    printLine();
    cout << "\nChoice: ";
}

void afficherMenuStaff() {
    cout << endl;
    printLine();
    printCentered("STAFF MENU");
    printLine();
    cout << endl;
    printCentered("[1] Create Accommodation Unit");
    printCentered("[2] Manage Accommodation Unit");
    printCentered("[3] Update Client Data");
    printCentered("[4] Reservation Management");
    printCentered("[5] Human Resources Management");
    printCentered("[0] Return to Main Menu");
    cout << endl;
    printLine();
    cout << "\nChoice: ";
}

void fonctionnaliteSousDevelloppement() {
    cout << "\n";
    printLine('*');
    printCentered("⚠️  FUNCTIONALITY UNDER DEVELOPMENT ⚠️");
    printLine('*');
    cout << "\nPress Enter to continue...";
    cin.get();
}

void menuCreationChambre(Hotel& hotel) {
    clearScreen();
    cout << "\n=== Create Accommodation Unit ===" << endl;

    int numero;
    float prixBase;
    int typeChoix;

    cout << "\nRoom number (select [0] to exit): ";
    cin >> numero;
    cin.ignore();

    while(numero!=0){

    cout << "\nRoom Type:" << endl;
    cout << "[1] Single (Default: 300 DH)" << endl;
    cout << "[2] Double (Default: 500 DH)" << endl;
    cout << "[3] Suite (Default: 800 DH)" << endl;
    cout << "[4] Deluxe (Default: 1000 DH)" << endl;
    cout << "[5] Presidential (Default: 1500 DH)" << endl;
    cout << "[6] Suite Royale (Default: 2000 DH)" << endl;
    cout << "Choice: ";
    cin >> typeChoix;

    cout << "Base price (DH): ";
    cin >> prixBase;
    cin.ignore();

    shared_ptr<Chambre> nouvelleChambre;

    switch (typeChoix) {
    case 1: nouvelleChambre = make_shared<ChambreSingle>(numero, prixBase); break;
    case 2: nouvelleChambre = make_shared<ChambreDouble>(numero, prixBase); break;
    case 3: nouvelleChambre = make_shared<ChambreSuite>(numero, prixBase); break;
    case 4: nouvelleChambre = make_shared<ChambreDeluxe>(numero, prixBase); break;
    case 5: nouvelleChambre = make_shared<ChambrePresidential>(numero, prixBase); break;
    case 6: nouvelleChambre = make_shared<ChambreSuiteRoyal>(numero, prixBase); break;
    default:
        cout << "Invalid choice!" << endl;
        return;
    }

    hotel.ajouterChambre(nouvelleChambre);
    cout << "\n✓ Room created successfully!" << endl;
}}

void menuGestionChambre(Hotel& hotel) {
    clearScreen();
    hotel.afficherToutesChambres();

    int numero;
    cout << "\nRoom number to manage (select [0] to exit): ";
    cin >> numero;
    cin.ignore();

   while(numero!=0){

    auto chambre = hotel.trouverChambre(numero);
    if (!chambre) {
        cout << "Room not found!" << endl;
        return;
    }

    int choix;
    do {
        cout << "\n=== Managing Room " << numero << " ===" << endl;
        cout << "[1] Modify base price" << endl;
        cout << "[2] Modify percentage" << endl;
        cout << "[3] Change status (Available/Occupied)" << endl;
        cout << "[0] Return" << endl;
        cout << "\nChoice: ";
        cin >> choix;
        cin.ignore();

        switch (choix) {
        case 1: {
            float nouveauPrix;
            cout << "New base price: ";
            cin >> nouveauPrix;
            cin.ignore();
            hotel.modifierPrixBase(numero, nouveauPrix);
            break;
        }
        case 2: {
            double nouveauPourcentage;
            cout << "New percentage: ";
            cin >> nouveauPourcentage;
            cin.ignore();
            hotel.modifierPourcentage(numero, nouveauPourcentage);
            break;
        }
        case 3: {
            bool nouvelEtat = !chambre->isOccupee();
            chambre->setOccupe(nouvelEtat);
            hotel.sauvegarderChambres();
            cout << "✓ Room status changed to " << (nouvelEtat ? "Occupied" : "Available") << "!" << endl;
            break;
        }
        case 0:
            break;
        default:
            cout << "Invalid choice!" << endl;
        }
    } while (choix != 0);
}
}


void gestionreservation(Hotel& hotel, shared_ptr<Client> client) {
    clearScreen();
    hotel.afficherReservationsClient(client);

    int numero;
    shared_ptr<Reservation> reservation;
    bool validReservation = false;

    do {
        cout << "\nReservation number to manage (select [0] to exit): ";
        cin >> numero;
        cin.ignore();

        if(numero==0){break;}

        reservation = hotel.trouverReservation(numero);
        if (!reservation || reservation->getNumeroClient() != client->getNumeroClient()) {
            cout << "Reservation not found or does not belong to you! Please try again." << endl;
        } else {
            validReservation = true;
        }
    } while (!validReservation);


    int choix;
    do { if(numero==0){break;}
        cout << "\n=== Managing reservation " << numero << " ===" << endl;
        cout << "[1] Modify stay period" << endl;
        cout << "[2] Modify room" << endl;
        cout << "[0] Return" << endl;
        cout << "\nChoice: ";
        cin >> choix;
        cin.ignore();

        switch (choix) {
        case 1: {
            string nouvelleDateDebut, nouvelleDateFin;
            cout << "New start date (dd/mm/yyyy): ";
            getline(cin, nouvelleDateDebut);
            cout << "New end date (dd/mm/yyyy): ";
            getline(cin, nouvelleDateFin);

            int nouveauxJours = calculerNombreJours(nouvelleDateDebut, nouvelleDateFin);
            if (nouveauxJours <= 0) {
                cout << "Invalid dates!" << endl;
                break;
            }

            auto chambre = hotel.trouverChambre(reservation->getNumeroChambre());
            if (!chambre) {
                cout << "Room not found!" << endl;
                break;
            }

            double nouveauPrixTotal = chambre->calculerPrixTotal(nouveauxJours);

            reservation->setDateDebut(nouvelleDateDebut);
            reservation->setDateFin(nouvelleDateFin);
            reservation->setNombreJours(nouveauxJours);
            reservation->setPrixTotal(nouveauPrixTotal);

            hotel.sauvegarderReservations();
            cout << "✓ Stay period modified successfully!" << endl;
            break;
        }
        case 2: {
            hotel.afficherChambresLibres();
           auto roomlist=hotel.getChambresLibres();
             if (roomlist.empty()) {
                break;
            }
            int nouveauNumeroChambre;
            cout << "New room number: ";
            cin >> nouveauNumeroChambre;
            cin.ignore();

            auto nouvelleChambre = hotel.trouverChambre(nouveauNumeroChambre);
            if (!nouvelleChambre) {
                cout << "Room not found!" << endl;
                break;
            }

            if (nouvelleChambre->isOccupee()) {
                cout << "Room already occupied!" << endl;
                break;
            }

            // Free old room
            auto ancienneChambre = hotel.trouverChambre(reservation->getNumeroChambre());
            if (ancienneChambre) {
                ancienneChambre->setOccupe(false);
            }

            // Occupy new room
            nouvelleChambre->setOccupe(true);

            double nouveauPrixTotal = nouvelleChambre->calculerPrixTotal(reservation->getNombreJours());

            reservation->setNumeroChambre(nouveauNumeroChambre);
            reservation->setPrixTotal(nouveauPrixTotal);

            hotel.sauvegarderReservations();
            hotel.sauvegarderChambres();
            cout << "✓ Room modified successfully!" << endl;
            break;
        }

        case 0:
            break;
        default:
            cout << "Invalid choice!" << endl;
        }
    } while (choix != 0);
}


void menuClient(Hotel& hotel, shared_ptr<Client> client) {
    int choix;
    do {
        clearScreen();
        cout << "\nWelcome " << *client << "!" << endl;
        afficherMenuClient();
        cin >> choix;
        cin.ignore();

        switch (choix) {
        case 1: {
            hotel.afficherChambresLibres();
           auto roomlist=hotel.getChambresLibres();
             if (roomlist.empty()) {
                break;
            }

            int numChambre;
            string dateDebut, dateFin;

            cout << "\nRoom number: ";
            cin >> numChambre;
            cin.ignore();

            auto chambre = hotel.trouverChambre(numChambre);
            if (!chambre) {
                cout << "Room not found!" << endl;
                break;
            }

            if (chambre->isOccupee()) {
                cout << "Room already occupied!" << endl;
                break;
            }

            cout << "Start date (dd/mm/yyyy): ";
            getline(cin, dateDebut);
            cout << "End date (dd/mm/yyyy): ";
            getline(cin, dateFin);

            int nombreJours = calculerNombreJours(dateDebut, dateFin);
            if (nombreJours <= 0) {
                cout << "Invalid dates!" << endl;
                break;
            }

            double prixParNuit = chambre->calculerPrixParNuit();
            double prixTotal = chambre->calculerPrixTotal(nombreJours);

            auto res = make_shared<Reservation>(client->getNumeroClient(), numChambre, dateDebut, dateFin, nombreJours, prixTotal);
            hotel.ajouterReservation(res);
            chambre->setOccupe(true);
            hotel.sauvegarderReservations();
            hotel.sauvegarderChambres();

            cout << "\n✓ Reservation created successfully!" << endl;
            cout << "Reservation number: " << res->getNumeroReservation() << endl;
            cout << "Room: " << numChambre << " [" << chambre->getNomType() << "]" << endl;
            cout << "Period: " << dateDebut << " -> " << dateFin << endl;
            cout << "Number of nights: " << nombreJours << endl;
            cout << "Price per night: " << prixParNuit << " DH" << endl;
            cout << "Total price: " << prixTotal << " DH" << endl;
            break;
        }

        case 2: {
            hotel.afficherReservationsClient(client);
            break;
        }

        case 3: {
           gestionreservation(hotel, client);
            cout << "\nPress Enter to continue...";
            std::cin.get();
            break;
        }

        case 4: {
            hotel.afficherReservationsClient(client);

            int numReservation;
            cout << "\nReservation number to cancel (select [0] to exit): ";
            cin >> numReservation;
            cin.ignore();

            if(numReservation==0){
            break;}


            int confirmation;
            cout << "Are you sure you want to cancel reservation #" << numReservation << "?" << endl;
            cout << "[1] Yes, cancel" << endl;
            cout << "[0] No, return" << endl;
            cout << "Choice: ";
            cin >> confirmation;
            cin.ignore();

            if (confirmation == 1) {
                hotel.annulerReservation(numReservation);
            }
            else {
                cout << "Cancellation cancelled." << endl;
            }
            break;
        }

        case 0:
            cout << "Returning to main menu..." << endl;
            break;

        default:
            cout << "Invalid choice!" << endl;
        }

        if (choix != 0) {
            cout << "\nPress Enter to continue...";
            std::cin.get();
        }
    } while (choix != 0);
}

void menuStaff(Hotel& hotel, shared_ptr<Staff> staff) {
    int choix;
    do {
        clearScreen();
        cout << "\nWelcome " << *staff << " [" << staff->getPoste() << "]" << endl;
        afficherMenuStaff();
        cin >> choix;
        cin.ignore();

        switch (choix) {
        case 1: {
            menuCreationChambre(hotel);
            cout << "\nPress Enter to continue...";
            std::cin.get();
            break;
        }

        case 2: {
            menuGestionChambre(hotel);
            cout << "\nPress Enter to continue...";
            std::cin.get();
            break;
        }

        case 3: {
            hotel.afficherTousClients();

            int numeroClient;
            cout << "\nClient number to modify (select [0] to exit): ";
            cin >> numeroClient;
            cin.ignore();

            if(numeroClient==0){  cout << "\nPress Enter to continue...";
 break;}

            hotel.modifierInformationsClient(numeroClient);
            cout << "\nPress Enter to continue...";
            std::cin.get();
            break;
        }

        case 4: {
            hotel.afficherToutesReservations();

            int numReservation;
            cout << "\nReservation number to cancel (select [0] to exit): ";
            cin >> numReservation;
            cin.ignore();

            if(numReservation==0){ break;}

            int confirmation;
            cout << "Are you sure you want to cancel reservation #" << numReservation << "?" << endl;
            cout << "[1] Yes, cancel" << endl;
            cout << "[0] No, return" << endl;
            cout << "Choice: ";
            cin >> confirmation;
            cin.ignore();

            if (confirmation == 1) {
                hotel.annulerReservation(numReservation);
            }
            else {
                cout << "Cancellation cancelled." << endl;
            }

            cout << "\nPress Enter to continue...";
            std::cin.get();
            break;
        }

        case 5: {
            hotel.HumanRessourcesManagement(staff);
            break;
        }

        case 0:
            cout << "Returning to main menu..." << endl;
            break;

        default:
            cout << "Invalid choice!" << endl;
            cout << "\nPress Enter to continue...";
            std::cin.get();
        }

    } while (choix != 0);
}

int main() {
    Hotel hotel;
    int choix;

    do {
        afficherMenuPrincipal();
        cin >> choix;
        cin.ignore();

        switch (choix) {
        case 0: {
            clearScreen();
            cout << "\nThank you for using the Hotel Management System!" << endl;
            cout << "Goodbye!" << endl;
            break;
        }

        case 1: {
            string cinClient;
            string motDePasse;

            bool loginSuccess = false;

            while (!loginSuccess) {
                clearScreen();
                cout << endl;
                printLine();
                printCentered("CLIENT LOGIN");
                printLine();
                cout << endl;

                cout << "CIN: ";
                cin >> cinClient;
                cin.ignore();

                auto client = hotel.trouverClientParCIN(cinClient);

                if (!client) {
                    cout << "\n❌ Client not found!" << endl;
                    cout << "\n[1] Try again" << endl;
                    cout << "[2] Create new account" << endl;
                    cout << "[0] Return to main menu" << endl;
                    cout << "\nChoice: ";
                    int choixClient;
                    cin >> choixClient;
                    cin.ignore();

                    if (choixClient == 2) {
                        clearScreen();
                        cout << "\n=== New Client Registration ===" << endl;
                        string nom, prenom, cinNew, tel, mdp;
                        cout << "Name: ";
                        getline(std::cin, nom);
                        cout << "Surname: ";
                        getline(std::cin, prenom);
                        cout << "CIN: ";
                        getline(std::cin, cinNew);
                        cout << "Phone: ";
                        getline(std::cin, tel);
                        cout << "Password: ";
                        getline(std::cin, mdp);

                        auto nouveauClient = make_shared<Client>(nom, prenom, cinNew, tel, mdp);
                        hotel.ajouterClient(nouveauClient);
                        cout << "\n✓ Account created successfully!" << endl;
                        cout << "Your CIN is: " << cinNew << endl;
                        cout << "\nPress Enter to login...";
                        std::cin.get();
                    }
                    else if (choixClient == 0) {
                        break;
                    }
                    continue;
                }

                cout << "Password: ";
                getline(cin, motDePasse);

                auto clientConnecte = hotel.connexionClientAvecMotDePasse(cinClient, motDePasse);
                if (clientConnecte) {
                    loginSuccess = true;
                    menuClient(hotel, clientConnecte);
                }
            }
            break;
        }

        case 2: {
            int idStaff;
            string motDePasse;

            bool loginSuccess = false;

            while (!loginSuccess) {
                clearScreen();
                cout << endl;
                printLine();
                printCentered("STAFF LOGIN");
                printLine();
                cout << endl;

                cout << "Staff ID: ";
                cin >> idStaff;
                cin.ignore();

                cout << "Password: ";
                getline(cin, motDePasse);

                auto staffConnecte = hotel.connexionStaffAvecMotDePasse(idStaff, motDePasse);
                if (staffConnecte) {
                    loginSuccess = true;
                    menuStaff(hotel, staffConnecte);
                }
                else {
                    cout << "\n[1] Try again" << endl;
                    cout << "[0] Return to main menu" << endl;
                    cout << "\nChoice: ";
                    int choixRetry;
                    cin >> choixRetry;
                    cin.ignore();

                    if (choixRetry == 0) {
                        break;
                    }
                }
            }
            break;
        }

        default:
            clearScreen();
            cout << "\n❌ Invalid choice!" << endl;
            cout << "\nPress Enter to continue...";
            std::cin.get();
        }
    } while (choix != 0);

    hotel.sauvegarderTout();
    return 0;
}

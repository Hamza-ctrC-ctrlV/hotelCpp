#include <iostream>
#include <string>
#include <vector>
#include <memory>
#include <algorithm>
#include <sstream> // // lib bach n9lb klma ar9am w l3ks (5<->"5")
#include <ctime>
#include <fstream>
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

    // Convertit les données du client en format texte pour sauvegarde dans fichier
    // Format: NumeroClient|Nom|Prenom|CIN|Telephone
    string toFileString() const {
        return to_string(numeroClient) + "|" + getNom() + "|" + 
               getPrenom() + "|" + getCin() + "|" + getTelephone();
    }

    // Crée un objet Client à partir d'une ligne de fichier
    // Met à jour le compteur statique pour éviter les conflits d'ID lors de la création de nouveaux clients
    static shared_ptr<Client> fromFileString(const string& line) {
        stringstream ss(line);
        string numStr, nom, prenom, cin, tel;
        getline(ss, numStr, '|');
        getline(ss, nom, '|');
        getline(ss, prenom, '|');
        getline(ss, cin, '|');
        getline(ss, tel, '|');

        auto client = make_shared<Client>(nom, prenom, cin, tel);
        // Met à jour le compteur pour éviter les conflits d'ID
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
        cout << " | Poste: " << poste << endl;
    }

    int getIdStaff() const { return idStaff; }
    string getPoste() const { return poste; }

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
        // Set the counter to the loaded number to avoid conflicts
        if (stoi(idStr) > compteurStaff) {
            compteurStaff = stoi(idStr);
        }
        return staff;
    }
};
// ta hadi b7al d reservation n9dro n7tajoha mn b3d dakchi lach khlitha
int Staff::compteurStaff = 0;

// fonction bach n7sb lyali

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

    // Convertit les données de la chambre en format texte pour sauvegarde dans fichier
    // Format: Type|Numero|PrixBase|Pourcentage|EstOccupee (0 ou 1)
    string toFileString() const {
        return getNomType() + "|" + to_string(numero) + "|" + 
               to_string(prixbase) + "|" + to_string(pourcentage) + "|" + 
               (estOccupee ? "1" : "0");
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
    
    // Convertit les données de la réservation en format texte pour sauvegarde dans fichier
    // Format: NumeroReservation|CINClient|NumeroChambre|DateDebut|DateFin|NombreJours|PrixTotal
    string toFileString() const {
        return to_string(numeroReservation) + "|" + cinClient + "|" +
            to_string(numeroChambre) + "|" + dateDebut + "|" + dateFin + "|" +
            to_string(nombreJours) + "|" + to_string(prixTotal);
    }

    // Crée un objet Reservation à partir d'une ligne de fichier
    static shared_ptr<Reservation> fromFileString(const string& line) {
        stringstream ss(line);
        string numStr, cin, numChStr, debut, fin, joursStr, prixStr;
        getline(ss, numStr, '|');
        getline(ss, cin, '|');
        getline(ss, numChStr, '|');
        getline(ss, debut, '|');
        getline(ss, fin, '|');
        getline(ss, joursStr, '|');
        getline(ss, prixStr, '|');

        return make_shared<Reservation>(
            cin,
            stoi(numChStr),
            debut,
            fin,
            stoi(joursStr),
            stod(prixStr)
        );
    }
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
    // Noms des fichiers pour la persistance des données
    const string FICHIER_RESERVATIONS = "reservations.txt";
    const string FICHIER_CHAMBRES = "chambres.txt";
    const string FICHIER_CLIENTS = "clients.txt";

public:
    Hotel() {
        // Charge les données depuis les fichiers au démarrage
        chargerChambres();
        chargerClients();
        chargerReservations();
        
        // Si les fichiers n'existent pas, initialise les données par défaut
        if (chambres.empty()) {
            initialiserDonnees();
            sauvegarderChambres(); // Sauvegarde les chambres initiales
        }
        // Le staff est toujours initialisé en mémoire (pas sauvegardé dans fichier)
        if (staffMembers.empty()) {
            initialiserDonnees();
        }
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

    // Charge les chambres depuis le fichier chambres.txt
    // Reconstruit les objets Chambre selon leur type (Single, Double, Suite, etc.)
    void chargerChambres() {
        ifstream file(FICHIER_CHAMBRES);
        if (!file.is_open()) {
            return; // Fichier n'existe pas encore, sera créé lors de la première sauvegarde
        }

        string line;
        while (getline(file, line)) {
            if (!line.empty()) {
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
                bool occ = (occStr == "1");

                // Crée le bon type de chambre selon le type lu dans le fichier
                shared_ptr<Chambre> chambre;
                if (type == "Single") {
                    chambre = make_shared<ChambreSingle>(num, prix, pourc, occ);
                } else if (type == "Double") {
                    chambre = make_shared<ChambreDouble>(num, prix, pourc, occ);
                } else if (type == "Suite") {
                    chambre = make_shared<ChambreSuite>(num, prix, pourc, occ);
                } else if (type == "Deluxe") {
                    chambre = make_shared<ChambreDeluxe>(num, prix, pourc, occ);
                } else if (type == "Presidential") {
                    chambre = make_shared<ChambrePresidential>(num, prix, pourc, occ);
                } else if (type == "Suite Royale") {
                    chambre = make_shared<ChambreSuiteRoyal>(num, prix, pourc, occ);
                }

                if (chambre) {
                    chambres.push_back(chambre);
                }
            }
        }
        file.close();
    }

    // Charge les clients depuis le fichier clients.txt
    void chargerClients() {
        ifstream file(FICHIER_CLIENTS);
        if (!file.is_open()) {
            return; // Fichier n'existe pas encore, sera créé lors de la première sauvegarde
        }

        string line;
        while (getline(file, line)) {
            if (!line.empty()) {
                clients.push_back(Client::fromFileString(line));
            }
        }
        file.close();
    }

    // Charge les réservations depuis le fichier reservations.txt
    void chargerReservations() {
        ifstream file(FICHIER_RESERVATIONS);
        if (!file.is_open()) {
            cout << "📁 Fichier réservations introuvable." << endl;
            return; // Fichier n'existe pas encore, sera créé lors de la première sauvegarde
        }

        string line;
        while (getline(file, line)) {
            if (!line.empty()) {
                reservations.push_back(Reservation::fromFileString(line));
            }
        }
        file.close();

        cout << "✓ " << reservations.size() << " réservation(s) chargée(s)" << endl;
    }

    // Sauvegarde toutes les chambres dans le fichier chambres.txt
    void sauvegarderChambres() {
        ofstream file(FICHIER_CHAMBRES);
        for (const auto& chambre : chambres) {
            file << chambre->toFileString() << endl;
        }
        file.close();
    }

    // Sauvegarde tous les clients dans le fichier clients.txt
    void sauvegarderClients() {
        ofstream file(FICHIER_CLIENTS);
        for (const auto& client : clients) {
            file << client->toFileString() << endl;
        }
        file.close();
    }

    // Sauvegarde toutes les réservations dans le fichier reservations.txt
    void sauvegarderReservations() {
        ofstream file(FICHIER_RESERVATIONS);
        for (const auto& res : reservations) {
            file << res->toFileString() << endl;
        }
        file.close();
    }

    // Sauvegarde toutes les données (chambres, clients, réservations) dans leurs fichiers respectifs
    void sauvegarderTout() {
        sauvegarderChambres();
        sauvegarderClients();
        sauvegarderReservations();
    }

    // Ajoute un nouveau client et sauvegarde immédiatement dans le fichier
    void ajouterClient(shared_ptr<Client> c) {
        clients.push_back(c);
        sauvegarderClients(); // Sauvegarde automatique lors de l'ajout
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
            sauvegarderChambres(); // Sauvegarde le changement d'état de la chambre (libre)
        }

        auto it = remove_if(reservations.begin(), reservations.end(),
            [numeroReservation](const shared_ptr<Reservation>& r) {
                return r->getNumeroReservation() == numeroReservation;
            });

        if (it != reservations.end()) {
            reservations.erase(it, reservations.end());
            sauvegarderReservations(); // Sauvegarde la suppression de la réservation
            cout << "\n✓ Réservation #" << numeroReservation << " annulée avec succès!" << endl;
            return true;
        }

        return false;
    }
    // Modifie le pourcentage de majoration d'une chambre et sauvegarde le changement
    bool modifierPourcentage(int numero, double nouveauPourcentage) {
        auto chambre = trouverChambre(numero);
        if (!chambre) {
            cout << "Chambre introuvable!" << endl;
            return false;
        }

        double ancienPrix = chambre->calculerPrixParNuit();
        chambre->setPourcentage(nouveauPourcentage);
        sauvegarderChambres(); // Sauvegarde la modification du pourcentage

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
            hotel.sauvegarderReservations(); // Sauvegarde la nouvelle réservation
            hotel.sauvegarderChambres(); // Sauvegarde le changement d'état de la chambre (occupée)

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

    // Sauvegarde toutes les données avant de quitter le programme
    hotel.sauvegarderTout();
    return 0;
}
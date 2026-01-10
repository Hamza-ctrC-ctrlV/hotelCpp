#include <iostream>
#include <string>
#include <vector>
#include <memory>
#include <algorithm>
#include <sstream>
#include <ctime>
#include <fstream>
#include <iomanip>
#include <limits>
using namespace std;

// ============================================================================
// BASE CLASS: Personne (Person)
// Abstract base class representing any person in the system
// ============================================================================
class Personne {
protected:
    string nom;        // Last name
    string prenom;     // First name
    string cin;        // National ID number
    string telephone;  // Phone number

public:
    // Constructor: Initializes person with basic information
    Personne(string n, string p, string c, string tel)
        : nom(n), prenom(p), cin(c), telephone(tel) {
    }

    // Virtual function to display person information
    // Can be overridden by derived classes
    virtual void afficher() const {
        cout << "Name: " << nom << " " << prenom
            << " | CIN: " << cin
            << " | Phone: " << telephone;
    }

    // Equality operator: Two persons are equal if they have the same CIN
    bool operator==(const Personne& autre) const {
        return cin == autre.cin;
    }

    // Stream insertion operator for easy printing
    friend ostream& operator<<(ostream& os, const Personne& p) {
        os << p.nom << " " << p.prenom << " (CIN: " << p.cin << ")";
        return os;
    }

    // Getters: Retrieve private/protected member values
    string getNom() const { return nom; }
    string getPrenom() const { return prenom; }
    string getCin() const { return cin; }
    string getTelephone() const { return telephone; }

    // Setters: Modify private/protected member values
    void setNom(string n) { nom = n; }
    void setPrenom(string p) { prenom = p; }
    void setCin(string c) { cin = c; }
    void setTelephone(string t) { telephone = t; }

    // Virtual destructor for proper cleanup in inheritance hierarchy
    virtual ~Personne() {}
};

// ============================================================================
// DERIVED CLASS: Client
// Represents a hotel guest with account management features
// ============================================================================
class Client : public Personne {
private:
    int numeroClient;           // Unique client ID
    string motDePasse;          // Password for account access
    int tentativesEchouees;     // Failed login attempts counter
    bool compteActif;           // Account status (active/blocked)
    static int compteurClients; // Static counter for auto-incrementing IDs

public:
    // Constructor: Creates a new client with default password "1234"
    Client(string n, string p, string c, string tel, string mdp = "1234",
        int tentatives = 0, bool actif = true)
        : Personne(n, p, c, tel), motDePasse(mdp),
        tentativesEchouees(tentatives), compteActif(actif) {
        numeroClient = ++compteurClients; // Auto-increment client number
    }

    // Override: Display client information with account status
    void afficher() const override {
        cout << "[Client #" << numeroClient << "] ";
        Personne::afficher();
        cout << " | Status: " << (compteActif ? "Active" : "BLOCKED") << endl;
    }

    // Less-than operator: For sorting clients by ID
    bool operator<(const Client& autre) const {
        return numeroClient < autre.numeroClient;
    }

    // Getters for client-specific attributes
    int getNumeroClient() const { return numeroClient; }
    string getMotDePasse() const { return motDePasse; }
    int getTentativesEchouees() const { return tentativesEchouees; }
    bool isCompteActif() const { return compteActif; }

    // Setters for modifying client attributes
    void setMotDePasse(string mdp) { motDePasse = mdp; }
    void setCompteActif(bool actif) { compteActif = actif; }

    // Increment failed login attempts; block account after 5 attempts
    void incrementerTentatives() {
        tentativesEchouees++;
        if (tentativesEchouees >= 3) {
            compteActif = false; // Auto-block after 5 failed attempts
        }
    }

    // Reset failed attempts counter
    void reinitialiserTentatives() { tentativesEchouees = 0; }

    // Verify if provided password matches stored password
    bool verifierMotDePasse(string mdp) const {
        return motDePasse == mdp;
    }

    // Serialize client data to string format for file storage
    // Format: ID|Name|Surname|CIN|Phone|Password|Attempts|Active
    string toFileString() const {
        return to_string(numeroClient) + "|" + getNom() + "|" +
            getPrenom() + "|" + getCin() + "|" + getTelephone() + "|" +
            motDePasse + "|" + to_string(tentativesEchouees) + "|" +
            (compteActif ? "Yes" : "No");
    }

    // Deserialize client data from file string
    // Creates a Client object from pipe-separated string
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
        
        // Update static counter to avoid ID conflicts
        if (stoi(numStr) > compteurClients) {
            compteurClients = stoi(numStr);
        }
        return client;
    }
};

// Initialize static counter
int Client::compteurClients = 0;

// ============================================================================
// DERIVED CLASS: Staff
// Represents hotel employees with position and authentication
// ============================================================================
class Staff : public Personne {
private:
    int idStaff;              // Unique staff ID
    string poste;             // Job position/title
    string motDePasse;        // Password for staff access
    static int compteurStaff; // Static counter for auto-incrementing IDs

public:
    // Constructor: Creates a staff member with position and password
    Staff(string n, string p, string c, string tel, string post, string mdp)
        : Personne(n, p, c, tel), poste(post), motDePasse(mdp) {
        idStaff = ++compteurStaff; // Auto-increment staff ID
    }

    // Override: Display staff information with position
    void afficher() const override {
        cout << "[Staff #" << idStaff << "] ";
        Personne::afficher();
        cout << " | Position: " << poste << endl;
    }

    // Getters
    int getIdStaff() const { return idStaff; }
    string getPoste() const { return poste; }

    // Setters
    void setIdStaff(int id) { idStaff = id; }
    void setPoste(string p) { poste = p; }
    void setMotDePasse(string mdp) { motDePasse = mdp; }

    // Verify staff password
    bool verifierMotDePasse(string mdp) const {
        return motDePasse == mdp;
    }

    // Serialize staff data for file storage
    // Format: ID|Name|Surname|CIN|Phone|Position|Password
    string toFileString() const {
        return to_string(idStaff) + "|" + getNom() + "|" +
            getPrenom() + "|" + getCin() + "|" + getTelephone() + "|" +
            poste + "|" + motDePasse;
    }

    // Deserialize staff data from file string
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
        
        // Update static counter to avoid ID conflicts
        if (stoi(idStr) > compteurStaff) {
            compteurStaff = stoi(idStr);
        }
        return staff;
    }
};

// Initialize static counter
int Staff::compteurStaff = 0;

// Forward declaration for clearScreen function
void clearScreen();

// ============================================================================
// UTILITY FUNCTION: calculerNombreJours
// Calculates the number of days between two dates
// Date format: dd/mm/yyyy
// Returns: Number of days between dates
// Throws: logic_error for invalid date ranges, runtime_error for format errors
// ============================================================================
int calculerNombreJours(const string& dateDebut, const string& dateFin) {
    try {
        int jour1, mois1, annee1;
        int jour2, mois2, annee2;
        char slash;
        
        // Parse start date
        stringstream ss1(dateDebut);
        ss1 >> jour1 >> slash >> mois1 >> slash >> annee1;
        
        // Parse end date
        stringstream ss2(dateFin);
        ss2 >> jour2 >> slash >> mois2 >> slash >> annee2;
        
        // Simple date calculation (approximation: 30 days/month, 365 days/year)
        int totalDays1 = annee1 * 365 + mois1 * 30 + jour1;
        int totalDays2 = annee2 * 365 + mois2 * 30 + jour2;
        
        // Validate date range
        if (totalDays2 < totalDays1) {
            throw logic_error("End date cannot be before start date.");
        }
        if (totalDays2 == totalDays1) {
            throw logic_error("Stay must be at least one night.");
        }
        return totalDays2 - totalDays1;
    }
    catch (const logic_error& e) {
        throw; // Re-throw logic errors
    }
    catch (const exception& e) {
        throw runtime_error("Invalid date format (use dd/mm/yyyy)");
    }
}

// ============================================================================
// ABSTRACT BASE CLASS: Chambre (Room)
// Base class for all room types with pricing and occupancy management
// ============================================================================
class Chambre {
protected:
    int numero;         // Room number
    float prixbase;     // Base price per night
    bool estOccupee;    // Occupancy status
    double pourcentage; // Percentage adjustment (discount/markup)

public:
    // Constructor: Initialize room with number, base price, and percentage
    Chambre(int numero, float prixbase, double pourcentage = 0.0, bool estOccupee = false)
        : numero(numero), prixbase(prixbase), pourcentage(pourcentage), estOccupee(estOccupee) {
    }

    // Calculate price per night with percentage adjustment
    virtual double calculerPrixParNuit() const {
        return prixbase + (prixbase * pourcentage / 100.0);
    }

    // Calculate total price for multiple nights
    double calculerPrixTotal(int nombreJours) const {
        return calculerPrixParNuit() * nombreJours;
    }

    // Display basic room information
    virtual void afficher() const {
        cout << "Room " << numero << " [" << getNomType() << "] - "
            << calculerPrixParNuit() << " DH/night - "
            << (estOccupee ? "Occupied" : "Available");
    }

    // Display detailed room information
    virtual void afficherDetail() const {
        cout << "Room Number: " << numero << endl;
        cout << "Room Type: " << getNomType() << endl;
        cout << "Base Price: " << prixbase << " DH" << endl;
        cout << "Price per Night: " << calculerPrixParNuit() << " DH" << endl;
        cout << "Status: " << (estOccupee ? "Occupied" : "Available") << endl;
        cout << "Percentage: " << pourcentage << "%" << endl;
    }

    // Pure virtual function: Must be implemented by derived classes
    virtual string getNomType() const = 0;

    // Modify room attributes
    void modifier(float nouveauPrixbase, double nouveauPourcentage, bool nouvelleOccupee) {
        this->prixbase = nouveauPrixbase;
        this->pourcentage = nouveauPourcentage;
        this->estOccupee = nouvelleOccupee;
    }

    // Getters
    int getNumero() const { return numero; }
    float getPrixBase() const { return prixbase; }
    double getPourcentage() const { return pourcentage; }
    bool isOccupee() const { return estOccupee; }

    // Setters
    void setPrixbase(float prixbase) { this->prixbase = prixbase; }
    void setEstOccupee(bool estOccupee) { this->estOccupee = estOccupee; }
    void setPourcentage(double pourcentage) { this->pourcentage = pourcentage; }
    void setOccupe(bool occupe) { this->estOccupee = occupe; }

    // Serialize room data for file storage
    // Format: Type|Number|BasePrice|Percentage|Occupied
    string toFileString() const {
        return getNomType() + "|" + to_string(numero) + "|" +
            to_string(prixbase) + "|" + to_string(pourcentage) + "|" +
            (estOccupee ? "Yes" : "No");
    }

    // Virtual destructor for proper cleanup
    virtual ~Chambre() {}
};

// ============================================================================
// ROOM TYPE CLASSES
// Concrete implementations of different room types with default pricing
// Each class represents a specific category of accommodation
// ============================================================================

// Single Room: Basic room for one person (300 DH default)
class ChambreSingle : public Chambre {
public:
    ChambreSingle(int numero, float prixbase = 300.0, double pourcentage = 0.0, bool estOccupee = false)
        : Chambre(numero, prixbase, pourcentage, estOccupee) {
    }
    string getNomType() const override { return "Single"; }
};

// Double Room: Standard room for two people (500 DH default)
class ChambreDouble : public Chambre {
public:
    ChambreDouble(int numero, float prixbase = 500.0, double pourcentage = 0.0, bool estOccupee = false)
        : Chambre(numero, prixbase, pourcentage, estOccupee) {
    }
    string getNomType() const override { return "Double"; }
};

// Suite: Luxury suite (800 DH default)
class ChambreSuite : public Chambre {
public:
    ChambreSuite(int numero, float prixbase = 800.0, double pourcentage = 0.0, bool estOccupee = false)
        : Chambre(numero, prixbase, pourcentage, estOccupee) {
    }
    string getNomType() const override { return "Suite"; }
};

// Deluxe Room: Premium accommodation (1000 DH default)
class ChambreDeluxe : public Chambre {
public:
    ChambreDeluxe(int numero, float prixbase = 1000.0, double pourcentage = 0.0, bool estOccupee = false)
        : Chambre(numero, prixbase, pourcentage, estOccupee) {
    }
    string getNomType() const override { return "Deluxe"; }
};

// Presidential Suite: High-end luxury suite (1500 DH default)
class ChambrePresidential : public Chambre {
public:
    ChambrePresidential(int numero, float prixbase = 1500.0, double pourcentage = 0.0, bool estOccupee = false)
        : Chambre(numero, prixbase, pourcentage, estOccupee) {
    }
    string getNomType() const override { return "Presidential"; }
};

// Royal Suite: Ultimate luxury accommodation (2000 DH default)
class ChambreSuiteRoyal : public Chambre {
public:
    ChambreSuiteRoyal(int numero, float prixbase = 2000.0, double pourcentage = 0.0, bool estOccupee = false)
        : Chambre(numero, prixbase, pourcentage, estOccupee) {
    }
    string getNomType() const override { return "Suite Royale"; }
};

// ============================================================================
// CLASS: Reservation
// Manages hotel room reservations with date ranges and pricing
// ============================================================================
class Reservation {
private:
    int numeroReservation;              // Unique reservation ID
    int numeroClient;                   // Client ID who made the reservation
    int numeroChambre;                  // Reserved room number
    string dateDebut;                   // Check-in date
    string dateFin;                     // Check-out date
    int nombreJours;                    // Duration of stay in nights
    double prixTotal;                   // Total price for the stay
    static int compteurReservations;    // Static counter for auto-incrementing IDs

public:
    // Constructor: Creates a new reservation with all details
    Reservation(int numClient, int numChambre, string debut, string fin, int jours, double prix)
        : numeroClient(numClient), numeroChambre(numChambre),
        dateDebut(debut), dateFin(fin), nombreJours(jours), prixTotal(prix) {
        numeroReservation = ++compteurReservations; // Auto-increment reservation ID
    }

    // Equality operator: Compare reservations by ID
    bool operator==(const Reservation& autre) const {
        return numeroReservation == autre.numeroReservation;
    }

    // Less-than operator: For sorting reservations
    bool operator<(const Reservation& autre) const {
        return numeroReservation < autre.numeroReservation;
    }

    // Display reservation summary with client information
    void afficher(const Client& c) const {
        cout << "Res #" << numeroReservation
            << " | Guest: " << c.getNom() << " " << c.getPrenom()
            << " | Room " << numeroChambre
            << " | " << dateDebut << " -> " << dateFin
            << " | " << nombreJours << " nights | Total: " << prixTotal << " DH";
    }

    // Display detailed reservation information
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

    // Getters for reservation attributes
    int getNumeroReservation() const { return numeroReservation; }
    int getNumeroClient() const { return numeroClient; }
    int getNumeroChambre() const { return numeroChambre; }
    string getDateDebut() const { return dateDebut; }
    string getDateFin() const { return dateFin; }
    int getNombreJours() const { return nombreJours; }
    double getPrixTotal() const { return prixTotal; }

    // Setters for modifying reservation details
    void setDateDebut(string date) { dateDebut = date; }
    void setDateFin(string date) { dateFin = date; }
    void setNumeroChambre(int num) { numeroChambre = num; }
    void setNombreJours(int jours) { nombreJours = jours; }
    void setPrixTotal(double prix) { prixTotal = prix; }

    // Serialize reservation data for file storage
    // Format: ResID|ClientID|RoomNum|StartDate|EndDate|NumDays|TotalPrice
    string toFileString() const {
        return to_string(numeroReservation) + "|" + to_string(numeroClient) + "|" +
            to_string(numeroChambre) + "|" + dateDebut + "|" + dateFin + "|" +
            to_string(nombreJours) + "|" + to_string(prixTotal);
    }

    // Deserialize reservation data from file string
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

        // Update static counter to avoid ID conflicts
        if (stoi(numStr) > compteurReservations) {
            compteurReservations = stoi(numStr);
        }

        return res;
    }
};

// Initialize static counter
int Reservation::compteurReservations = 0;

// ============================================================================
// MAIN CLASS: Hotel
// Central management system handling all hotel operations
// Manages: rooms, clients, staff, reservations, and persistence
// ============================================================================
class Hotel {
private:
    // Data containers using smart pointers for automatic memory management
    vector<shared_ptr<Chambre>> chambres;           // All hotel rooms
    vector<shared_ptr<Staff>> staffMembers;         // All staff members
    vector<shared_ptr<Client>> clients;             // All registered clients
    vector<shared_ptr<Reservation>> reservations;   // All reservations
    
    // File paths for data persistence
    const string FICHIER_RESERVATIONS = "reservations.txt";
    const string FICHIER_CHAMBRES = "chambres.txt";
    const string FICHIER_CLIENTS = "clients.txt";
    const string FICHIER_STAFF = "staff.txt";

public:
    // Constructor: Load all data from files on startup
    Hotel() {
        chargerStaff();
        chargerChambres();
        chargerClients();
        chargerReservations();
    }

    // ========================================================================
    // DATA LOADING METHODS
    // Load data from text files into memory
    // ========================================================================
    
    // Load staff members from file
    void chargerStaff() {
        ifstream file(FICHIER_STAFF.c_str());
        if (!file.is_open()) {
            return; // File doesn't exist yet (first run)
        }

        string line;
        while (getline(file, line)) {
            // Skip empty lines and comments
            if (line.empty() || line[0] == '#') {
                continue;
            }
            staffMembers.push_back(Staff::fromFileString(line));
        }
        file.close();
    }

    // Load rooms from file and create appropriate room objects
    void chargerChambres() {
        ifstream file(FICHIER_CHAMBRES.c_str());
        if (!file.is_open()) {
            return;
        }

        string line;
        while (getline(file, line)) {
            if (line.empty() || line[0] == '#') {
                continue;
            }

            // Parse room data
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

            // Create appropriate room type based on type string
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

    // Load clients from file
    void chargerClients() {
        ifstream file(FICHIER_CLIENTS.c_str());
        if (!file.is_open()) {
            return;
        }

        string line;
        while (getline(file, line)) {
            if (line.empty() || line[0] == '#') {
                continue;
            }

            clients.push_back(Client::fromFileString(line));
        }

        file.close();
    }

    // Load reservations from file
    void chargerReservations() {
        ifstream file(FICHIER_RESERVATIONS.c_str());
        if (!file.is_open()) {
            return;
        }

        string line;
        while (getline(file, line)) {
            if (line.empty() || line[0] == '#') {
                continue;
            }
            reservations.push_back(Reservation::fromFileString(line));
        }
        file.close();
    }

    // ========================================================================
    // DATA SAVING METHODS
    // Persist data from memory to text files
    // ========================================================================
    
    // Save staff members to file
    void sauvegarderStaff() {
        ofstream file(FICHIER_STAFF.c_str());
        file << "# STAFF DATA FILE" << endl;
        file << "# Staff ID|surname|name|cin|phone number|position|password" << endl;
        for (const auto& staff : staffMembers) {
            file << staff->toFileString() << endl;
        }
        file.close();
    }

    // Save rooms to file
    void sauvegarderChambres() {
        ofstream file(FICHIER_CHAMBRES.c_str());
        file << "# ROOMS DATA FILE" << endl;
        file << "# Room type|room number|base price|percentage|occupied status" << endl;
        for (const auto& chambre : chambres) {
            file << chambre->toFileString() << endl;
        }
        file.close();
    }

    // Save clients to file
    void sauvegarderClients() {
        ofstream file(FICHIER_CLIENTS.c_str());
        file << "# CLIENTS DATA FILE" << endl;
        file << "# Client ID|surname|name|cin|phone number|password|failed attempts|active account" << endl;

        for (const auto& client : clients) {
            file << client->toFileString() << endl;
        }

        file.close();
    }

    // Save reservations to file
    void sauvegarderReservations() {
        ofstream file(FICHIER_RESERVATIONS.c_str());
        file << "# RESERVATIONS DATA FILE" << endl;
        file << "# Reservation ID|Client ID|Room number|start date|end date|number of days|total price" << endl;
        for (const auto& res : reservations) {
            file << res->toFileString() << endl;
        }
        file.close();
    }

    // Save all data to files (convenience method)
    void sauvegarderTout() {
        sauvegarderStaff();
        sauvegarderChambres();
        sauvegarderClients();
        sauvegarderReservations();
    }

    // ========================================================================
    // DATA MANIPULATION METHODS
    // Add new entities to the system
    // ========================================================================
    
    // Add a new client and save immediately
    void ajouterClient(shared_ptr<Client> c) {
        clients.push_back(c);
        sauvegarderClients();
    }

    // Add a new staff member and save immediately
    void ajouterStaff(shared_ptr<Staff> s) {
        staffMembers.push_back(s);
        sauvegarderStaff();
    }

    // Add a new room and save immediately
    void ajouterChambre(shared_ptr<Chambre> c) {
        chambres.push_back(c);
        sauvegarderChambres();
    }

    // Add a new reservation (saving happens when needed)
    void ajouterReservation(shared_ptr<Reservation> r) {
        reservations.push_back(r);
    }

    // ========================================================================
    // DISPLAY METHODS
    // Show various lists and information to users
    // ========================================================================
    
    // Display all available (unoccupied) rooms
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

    // Return a vector of available rooms for booking
    vector<shared_ptr<Chambre>> getChambresLibres() const {
        vector<shared_ptr<Chambre>> libres;
        for (const auto& chambre : chambres) {
            if (!chambre->isOccupee()) {
                libres.push_back(chambre);
            }
        }
        return libres;
    }

    // Display all rooms regardless of occupancy status
    void afficherToutesChambres() const {
        cout << "\n=== All Rooms ===" << endl;
        for (const auto& chambre : chambres) {
            chambre->afficher();
            cout << endl;
        }
    }

    // Display all reservations with client and room details
    void afficherToutesReservations() const {
        cout << "\n=== All Reservations ===" << endl;
        if (reservations.empty()) {
            cout << "No reservations." << endl;
            return;
        }
        for (const auto& res : reservations) {
            // Find associated client and room information
            auto client = trouverClientParNumero(res->getNumeroClient());
            string nomClient = client ? client->getNom() : "Unknown";
            string prenomClient = client ? client->getPrenom() : "Unknown";
            auto chambre = trouverChambre(res->getNumeroChambre());
            string typeChambre = chambre ? chambre->getNomType() : "Unknown";
            res->afficherDetail(nomClient, prenomClient, typeChambre);
            cout << endl;
        }
    }

    // Display all registered clients
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

    // Display all staff members
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

    // ========================================================================
    // SEARCH METHODS
    // Find specific entities in the system
    // ========================================================================
    
    // Find a room by its number
    shared_ptr<Chambre> trouverChambre(int numero) const {
        auto it = find_if(chambres.begin(), chambres.end(),
            [numero](const shared_ptr<Chambre>& ch) {
                return ch->getNumero() == numero;
            });
        return (it != chambres.end()) ? *it : nullptr;
    }

    // Find a client by their unique client number
    shared_ptr<Client> trouverClientParNumero(int numero) const {
        auto it = find_if(clients.begin(), clients.end(),
            [numero](const shared_ptr<Client>& cl) {
                return cl->getNumeroClient() == numero;
            });
        return (it != clients.end()) ? *it : nullptr;
    }

    // Find a client by their CIN (National ID)
    shared_ptr<Client> trouverClientParCIN(string cin) const {
        auto it = find_if(clients.begin(), clients.end(),
            [&cin](const shared_ptr<Client>& cl) {
                return cl->getCin() == cin;
            });
        return (it != clients.end()) ? *it : nullptr;
    }

    // Find a staff member by their ID
    shared_ptr<Staff> trouverStaffParId(int id) {
        auto it = find_if(staffMembers.begin(), staffMembers.end(),
            [id](const shared_ptr<Staff>& st) {
                return st->getIdStaff() == id;
            });
        return (it != staffMembers.end()) ? *it : nullptr;
    }

    // Find a reservation by its number
    shared_ptr<Reservation> trouverReservation(int numero) {
        auto it = find_if(reservations.begin(), reservations.end(),
            [numero](const shared_ptr<Reservation>& res) {
                return res->getNumeroReservation() == numero;
            });
        return (it != reservations.end()) ? *it : nullptr;
    }

    // Display all reservations for a specific client
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

    // ========================================================================
    // RESERVATION MANAGEMENT METHODS
    // Handle reservation operations
    // ========================================================================
    
    // Cancel a reservation and free the associated room
    bool annulerReservation(int numeroReservation) {
        auto res = trouverReservation(numeroReservation);
        if (!res) {
            cout << "Reservation not found!" << endl;
            return false;
        }

        // Free the room by setting it as unoccupied
        auto chambre = trouverChambre(res->getNumeroChambre());
        if (chambre) {
            chambre->setOccupe(false);
            sauvegarderChambres();
        }

        // Remove reservation from vector
        auto it = remove_if(reservations.begin(), reservations.end(),
            [numeroReservation](const shared_ptr<Reservation>& r) {
                return r->getNumeroReservation() == numeroReservation;
            });

        if (it != reservations.end()) {
            reservations.erase(it, reservations.end());
            sauvegarderReservations();
            cout << "\nReservation #" << numeroReservation << " cancelled successfully!" << endl;
            return true;
        }

        return false;
    }

    // ========================================================================
    // ROOM PRICING MANAGEMENT METHODS
    // Modify room prices and percentages
    // ========================================================================
    
    // Modify the percentage adjustment for a room's price
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

    // Modify the base price of a room
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

    // ========================================================================
    // CLIENT MANAGEMENT METHODS
    // Handle client information modifications
    // ========================================================================
    
    // Interactive menu to modify client information
    // Staff can update name, CIN, phone, password, and account status
    bool modifierInformationsClient(int numeroClient) {
        auto client = trouverClientParNumero(numeroClient);
        if (!client) {
            cout << "\nClient not found with number: " << numeroClient << endl;
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

            try {
                cin >> choix;
                if (cin.fail()) {
                    cin.clear();
                    cin.ignore(numeric_limits<streamsize>::max(), '\n');
                    cout << "Invalid input! Please enter a number." << endl;
                    continue;
                }
                cin.ignore();
            }
            catch (...) {
                cin.clear();
                cin.ignore(numeric_limits<streamsize>::max(), '\n');
                cout << "Invalid input!" << endl;
                continue;
            }

            switch (choix) {
            case 1: {
                string nouveauNom;
                cout << "New name: ";
                getline(cin, nouveauNom);
                client->setNom(nouveauNom);
                cout << "Name modified!" << endl;
                break;
            }
            case 2: {
                string nouveauPrenom;
                cout << "New surname: ";
                getline(cin, nouveauPrenom);
                client->setPrenom(nouveauPrenom);
                cout << "Surname modified!" << endl;
                break;
            }
            case 3: {
                string nouveauCIN;
                cout << "New CIN: ";
                getline(cin, nouveauCIN);
                client->setCin(nouveauCIN);
                cout << "CIN modified!" << endl;
                break;
            }
            case 4: {
                string nouveauTel;
                cout << "New phone: ";
                getline(cin, nouveauTel);
                client->setTelephone(nouveauTel);
                cout << "Phone modified!" << endl;
                break;
            }
            case 5: {
                string nouveauMdp;
                cout << "New password: ";
                getline(cin, nouveauMdp);
                client->setMotDePasse(nouveauMdp);
                cout << "Password modified!" << endl;
                break;
            }
            case 6: {
                bool nouvelEtat = !client->isCompteActif();
                client->setCompteActif(nouvelEtat);
                cout << "Account " << (nouvelEtat ? "activated" : "deactivated") << "!" << endl;
                break;
            }
            case 7: {
                // Reset failed login attempts and reactivate account
                client->reinitialiserTentatives();
                client->setCompteActif(true);
                cout << "Attempts reset and account reactivated!" << endl;
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
        cout << "\nModifications saved!" << endl;
        return true;
    }

    // ========================================================================
    // AUTHENTICATION METHODS
    // Handle login for clients and staff with security features
    // ========================================================================
    
    // Client login with password verification and attempt tracking
    // Blocks account after 5 failed attempts for security
    shared_ptr<Client> connexionClientAvecMotDePasse(string cin, string motDePasse) {
        auto client = trouverClientParCIN(cin);

        if (!client) {
            return nullptr; // Client not found
        }

        // Check if account is blocked
        if (!client->isCompteActif()) {
            cout << "\nACCOUNT BLOCKED!" << endl;
            cout << "Your account has been blocked due to too many failed attempts." << endl;
            cout << "Please contact administration to unlock your account." << endl;
            cout << "\nPress Enter to continue...";
            std::cin.get();
            return nullptr;
        }

        // Verify password
        if (!client->verifierMotDePasse(motDePasse)) {
            client->incrementerTentatives();
            sauvegarderClients();

            int tentativesRestantes = 3 - client->getTentativesEchouees();

            if (client->isCompteActif()) {
                cout << "\nIncorrect password!" << endl;
                cout << "Remaining attempts: " << tentativesRestantes << "/3" << endl;
            }
            else {
                cout << "\nACCOUNT BLOCKED!" << endl;
                cout << "You have exceeded the maximum number of attempts (3)." << endl;
                cout << "Please contact administration to unlock your account." << endl;
            }

            return nullptr;
        }

        // Successful login: Reset failed attempts
        client->reinitialiserTentatives();
        sauvegarderClients();
        return client;
    }

    // Staff login with password verification (no attempt limiting)
    shared_ptr<Staff> connexionStaffAvecMotDePasse(int idStaff, string motDePasse) {
        auto staff = trouverStaffParId(idStaff);

        if (!staff) {
            return nullptr; // Staff not found
        }

        if (!staff->verifierMotDePasse(motDePasse)) {
            cout << "\nIncorrect password!" << endl;
            cout << "\nPress Enter to continue...";
            cin.get();
            return nullptr;
        }

        return staff;
    }

    // ========================================================================
    // STAFF MANAGEMENT METHODS
    // Handle staff member information modifications (HR functions)
    // ========================================================================
    
    // Human Resources Management: Modify staff member details
    // Requires password verification to access staff record
    void HumanRessourcesManagement(shared_ptr<Staff> staff) {
        clearScreen();
        afficherTousStaff();

        int id;
        cout << "\n Select a staff ID to manage \n(select [0] to exit): ";
        try {
            cin >> id;
            if (cin.fail()) {
                cin.clear();
                cin.ignore(numeric_limits<streamsize>::max(), '\n');
                cout << "Invalid input!" << endl;
                return;
            }
            cin.ignore();
        }
        catch (...) {
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            cout << "Invalid input!" << endl;
            return;
        }

        auto staffMember = trouverStaffParId(id);
        if (!staffMember) {
            cout << "Staff member not found!" << endl;
            return;
        }

        // Security: Require password to modify staff data
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

            try {
                cin >> choix;
                if (cin.fail()) {
                    cin.clear();
                    cin.ignore(numeric_limits<streamsize>::max(), '\n');
                    cout << "Invalid input!" << endl;
                    continue;
                }
                cin.ignore();
            }
            catch (...) {
                cin.clear();
                cin.ignore(numeric_limits<streamsize>::max(), '\n');
                cout << "Invalid input!" << endl;
                continue;
            }

            switch (choix) {
            case 1: {
                int newid;
                cout << "New ID: ";
                try {
                    cin >> newid;
                    if (cin.fail()) throw exception();
                    cin.ignore();
                    staffMember->setIdStaff(newid);
                    sauvegarderStaff();
                    cout << "ID modified!" << endl;
                }
                catch (...) {
                    cin.clear();
                    cin.ignore(numeric_limits<streamsize>::max(), '\n');
                    cout << "Invalid input!" << endl;
                }
                break;
            }
            case 2: {
                string newposte;
                cout << "New poste: ";
                getline(cin, newposte);
                staffMember->setPoste(newposte);
                sauvegarderStaff();
                cout << "Poste modified!" << endl;
                break;
            }
            case 3: {
                string newpassword;
                cout << "New password: ";
                getline(cin, newpassword);
                staffMember->setMotDePasse(newpassword);
                sauvegarderStaff();
                cout << "Password modified!" << endl;
                break;
            }

            case 4: {
                string newname;
                cout << "New name: ";
                getline(cin, newname);
                staffMember->setNom(newname);
                sauvegarderStaff();
                cout << "Name modified!" << endl;
                break;
            }

            case 5: {
                string newsurname;
                cout << "New surname: ";
                getline(cin, newsurname);
                staffMember->setPrenom(newsurname);
                sauvegarderStaff();
                cout << "Surname modified!" << endl;
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

// ============================================================================
// UTILITY FUNCTIONS
// Screen management and UI helpers
// ============================================================================

// Clear the console screen (cross-platform)
void clearScreen() {
#ifdef _WIN32
    system("cls");  // Windows command
#else
    system("clear"); // Unix/Linux/Mac command
#endif
}

// Print centered text for menu headers
void printCentered(const string& text, int width = 80) {
    int padding = (width - text.length()) / 2;
    cout << string(padding, ' ') << text << endl;
}

// Print a line of characters (decoration)
void printLine(char c = '=', int width = 80) {
    cout << string(width, c) << endl;
}

// ============================================================================
// MENU DISPLAY FUNCTIONS
// Show formatted menus for different user types
// ============================================================================

// Display main menu for initial user selection
void afficherMenuPrincipal() {
    clearScreen();
    cout << endl;
    printLine();
    printCentered("HOTEL MANAGEMENT SYSTEM");
    printCentered("VERSION 2.0");
    printLine();
    cout << endl;
    cout << "                          [1] Client Login" << endl;
    cout << "                          [2] Client Register" << endl;
    cout << "                          [3] Staff Login" << endl;
    cout << "                          [0] Exit" << endl;
    cout << endl;
    printLine();
    cout << "\n";
    printCentered("Choice: ", 40);
    cout << "                                        ";
}

// Display menu options for authenticated clients
void afficherMenuClient() {
    cout << endl;
    printLine();
    printCentered("CLIENT MENU");
    printLine();
    cout << endl;
    cout << "                          [1] Reservation Request Entry" << endl;
    cout << "                          [2] View Reservation" << endl;
    cout << "                          [3] Update Reservation" << endl;
    cout << "                          [4] Reservation Cancellation Procedure" << endl;
    cout << "                          [0] Return to Main Menu" << endl;
    cout << endl;
    printLine();
    cout << "\nChoice: ";
}

// Display menu options for authenticated staff
void afficherMenuStaff() {
    cout << endl;
    printLine();
    printCentered("STAFF MENU");
    printLine();
    cout << endl;
    cout << "                          [1] Create Residence Unit" << endl;
    cout << "                          [2] Manage Residence Unit" << endl;
    cout << "                          [3] Update Client Data" << endl;
    cout << "                          [4] Reservation Management" << endl;
    cout << "                          [5] Human Resources Management" << endl;
    cout << "                          [0] Return to Main Menu" << endl;
    cout << endl;
    printLine();
    cout << "\nChoice: ";
}

// Placeholder for features under development
void fonctionnaliteSousDevelloppement() {
    cout << "\n";
    printLine('*');
    printCentered("FUNCTIONALITY UNDER DEVELOPMENT");
    printLine('*');
    cout << "\nPress Enter to continue...";
    cin.get();
}

// ============================================================================
// STAFF MENU FUNCTIONS
// Handle staff operations and room/client management
// ============================================================================

// Interactive menu for creating new rooms
// Staff can add multiple rooms of different types
void menuCreationChambre(Hotel& hotel) {
    clearScreen();
    cout << "\n=== Create Accommodation Unit ===" << endl;

    int numero;
    float prixBase;
    int typeChoix;

    cout << "\nRoom number (select [0] to exit): ";
    try {
        cin >> numero;
        if (cin.fail()) {
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            cout << "Invalid input!" << endl;
            return;
        }
        cin.ignore();
    }
    catch (...) {
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        cout << "Invalid input!" << endl;
        return;
    }

    // Loop to allow creating multiple rooms
    while (numero != 0) {

        cout << "\nRoom Type:" << endl;
        cout << "[1] Single (Default: 300 DH)" << endl;
        cout << "[2] Double (Default: 500 DH)" << endl;
        cout << "[3] Suite (Default: 800 DH)" << endl;
        cout << "[4] Deluxe (Default: 1000 DH)" << endl;
        cout << "[5] Presidential (Default: 1500 DH)" << endl;
        cout << "[6] Suite Royale (Default: 2000 DH)" << endl;
        cout << "Choice: ";
        try {
            cin >> typeChoix;
            if (cin.fail()) throw exception();
            cin.ignore();
        }
        catch (...) {
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            cout << "Invalid input!" << endl;
            continue;
        }

        cout << "Base price (DH): ";
        try {
            cin >> prixBase;
            if (cin.fail()) throw exception();
            cin.ignore();
        }
        catch (...) {
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            cout << "Invalid input!" << endl;
            continue;
        }

        shared_ptr<Chambre> nouvelleChambre;

        // Create appropriate room type based on selection
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
        cout << "\nRoom created successfully!" << endl;

        // Prompt for next room
        cout << "\nRoom number (select [0] to exit): ";
        try {
            cin >> numero;
            if (cin.fail()) throw exception();
            cin.ignore();
        }
        catch (...) {
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            break;
        }
    }
}

// Interactive menu for managing existing rooms
// Staff can modify price, percentage, or occupancy status
void menuGestionChambre(Hotel& hotel) {
    while (true) {
        clearScreen();
        hotel.afficherToutesChambres();

        int numero;
        cout << "\nRoom number to manage (select [0] to exit): ";
        try {
            cin >> numero;
            if (cin.fail()) {
                cin.clear();
                cin.ignore(numeric_limits<streamsize>::max(), '\n');
                cout << "Invalid input!" << endl;
                system("pause"); 
                continue; 
            }
            cin.ignore();
        }
        catch (...) {
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            cout << "Invalid input!" << endl;
            system("pause");
            continue;
        }
        if (numero == 0) {
            return; 
        }

        auto chambre = hotel.trouverChambre(numero);
        if (!chambre) {
            cout << "Room not found!" << endl;
            system("pause"); 
            continue; 
        }
        int choix;
        do {
            clearScreen();
            cout << "\n=== Managing Room " << numero << " ===" << endl;
            cout << "[1] Modify base price" << endl;
            cout << "[2] Modify percentage" << endl;
            cout << "[3] Change status (Available/Occupied)" << endl;
            cout << "[0] Return to Room List" << endl;
            cout << "\nChoice: ";

            try {
                cin >> choix;
                if (cin.fail()) {
                    cin.clear();
                    cin.ignore(numeric_limits<streamsize>::max(), '\n');
                    cout << "Invalid input!" << endl;
                    system("pause");
                    continue;
                }
                cin.ignore();
            }
            catch (...) {
                cin.clear();
                cin.ignore(numeric_limits<streamsize>::max(), '\n');
                cout << "Invalid input!" << endl;
                system("pause");
                continue;
            }

            switch (choix) {
            case 1: {
                float nouveauPrix;
                cout << "New base price: ";
                if (cin >> nouveauPrix) {
                    hotel.modifierPrixBase(numero, nouveauPrix);
                    cout << "Price updated." << endl;
                }
                else {
                    cin.clear(); cin.ignore(numeric_limits<streamsize>::max(), '\n');
                    cout << "Invalid price." << endl;
                }
                system("pause");
                break;
            }
            case 2: {
                double nouveauPourcentage;
                cout << "New percentage: ";
                if (cin >> nouveauPourcentage) {
                    hotel.modifierPourcentage(numero, nouveauPourcentage);
                    cout << "Percentage updated." << endl;
                }
                else {
                    cin.clear(); cin.ignore(numeric_limits<streamsize>::max(), '\n');
                    cout << "Invalid percentage." << endl;
                }
                system("pause");
                break;
            }
            case 3: {
                // Toggle room occupancy status
                bool nouvelEtat = !chambre->isOccupee();
                chambre->setOccupe(nouvelEtat);
                hotel.sauvegarderChambres();
                cout << "Room status changed to " << (nouvelEtat ? "Occupied" : "Available") << "!" << endl;
                system("pause");
                break;
            }
            case 0:
                break;
            default:
                cout << "Invalid choice!" << endl;
                system("pause");
            }
        } while (choix != 0);
    }
}

// ============================================================================
// CLIENT RESERVATION MANAGEMENT FUNCTIONS
// Allow clients to modify their existing reservations
// ============================================================================

// Client reservation management: Modify dates or change rooms
void gestionreservation(Hotel& hotel, shared_ptr<Client> client) {
    clearScreen();
    hotel.afficherReservationsClient(client);

    int numero;
    shared_ptr<Reservation> reservation;
    bool validReservation = false;

    // Validate reservation belongs to client
    do {
        cout << "\nReservation number to manage (select [0] to exit): ";
        try {
            cin >> numero;
            if (cin.fail()) {
                cin.clear();
                cin.ignore(numeric_limits<streamsize>::max(), '\n');
                cout << "Invalid input!" << endl;
                continue;
            }
            cin.ignore();
        }
        catch (...) {
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            cout << "Invalid input!" << endl;
            continue;
        }

        if (numero == 0) { break; }

        reservation = hotel.trouverReservation(numero);
        if (!reservation || reservation->getNumeroClient() != client->getNumeroClient()) {
            cout << "Reservation not found or does not belong to you! Please try again." << endl;
        }
        else {
            validReservation = true;
        }
    } while (!validReservation);


    int choix;
    do {
        if (numero == 0) { break; }
        cout << "\n=== Managing reservation " << numero << " ===" << endl;
        cout << "[1] Modify stay period" << endl;
        cout << "[2] Modify room" << endl;
        cout << "[0] Return" << endl;
        cout << "\nChoice: ";

        try {
            cin >> choix;
            if (cin.fail()) {
                cin.clear();
                cin.ignore(numeric_limits<streamsize>::max(), '\n');
                cout << "Invalid input!" << endl;
                continue;
            }
            cin.ignore();
        }
        catch (...) {
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            cout << "Invalid input!" << endl;
            continue;
        }

        switch (choix) {
        case 1: {
            // Modify reservation dates
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

            // Recalculate total price with new dates
            double nouveauPrixTotal = chambre->calculerPrixTotal(nouveauxJours);

            reservation->setDateDebut(nouvelleDateDebut);
            reservation->setDateFin(nouvelleDateFin);
            reservation->setNombreJours(nouveauxJours);
            reservation->setPrixTotal(nouveauPrixTotal);

            hotel.sauvegarderReservations();
            cout << "Stay period modified successfully!" << endl;
            break;
        }
        case 2: {
            // Change room for existing reservation
            hotel.afficherChambresLibres();
            auto roomlist = hotel.getChambresLibres();
            if (roomlist.empty()) {
                break;
            }

            int nouveauNumeroChambre;
            cout << "New room number: ";
            try {
                cin >> nouveauNumeroChambre;
                if (cin.fail()) throw exception();
                cin.ignore();
            }
            catch (...) {
                cin.clear();
                cin.ignore(numeric_limits<streamsize>::max(), '\n');
                cout << "Invalid input!" << endl;
                break;
            }

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

            // Recalculate price with new room
            double nouveauPrixTotal = nouvelleChambre->calculerPrixTotal(reservation->getNombreJours());

            reservation->setNumeroChambre(nouveauNumeroChambre);
            reservation->setPrixTotal(nouveauPrixTotal);

            hotel.sauvegarderReservations();
            hotel.sauvegarderChambres();
            cout << "Room modified successfully!" << endl;
            break;
        }

        case 0:
            break;
        default:
            cout << "Invalid choice!" << endl;
        }
    } while (choix != 0);
}

// ============================================================================
// CLIENT MENU HANDLER
// Main interface for client operations after login
// ============================================================================
void menuClient(Hotel& hotel, shared_ptr<Client> client) {
    int choix;
    do {
        clearScreen();
        cout << "\nWelcome " << *client << "!" << endl;
        afficherMenuClient();

        try {
            cin >> choix;
            if (cin.fail()) {
                cin.clear();
                cin.ignore(numeric_limits<streamsize>::max(), '\n');
                cout << "\nInvalid input! Please enter a number." << endl;
                cout << "Press Enter to continue...";
                cin.get();
                continue;
            }
            cin.ignore();
        }
        catch (...) {
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            cout << "\nInvalid input!" << endl;
            cout << "Press Enter to continue...";
            cin.get();
            continue;
        }

        switch (choix) {
        case 1: {
            // Create new reservation
            hotel.afficherChambresLibres();
            auto roomlist = hotel.getChambresLibres();
            if (roomlist.empty()) {
                break;
            }

            int numChambre;
            string dateDebut, dateFin;

            cout << "\nRoom number: ";
            try {
                cin >> numChambre;
                if (cin.fail()) throw exception();
                cin.ignore();
            }
            catch (...) {
                cin.clear();
                cin.ignore(numeric_limits<streamsize>::max(), '\n');
                cout << "Invalid input!" << endl;
                break;
            }

            auto chambre = hotel.trouverChambre(numChambre);
            if (!chambre) {
                cout << "Room not found!" << endl;
                break;
            }

            if (chambre->isOccupee()) {
                cout << "Room already occupied!" << endl;
                break;
            }

            // Get reservation dates
            cout << "Start date (dd/mm/yyyy): ";
            getline(cin, dateDebut);
            cout << "End date (dd/mm/yyyy): ";
            getline(cin, dateFin);

            int nombreJours = calculerNombreJours(dateDebut, dateFin);
            if (nombreJours <= 0) {
                cout << "Invalid dates!" << endl;
                break;
            }

            // Calculate pricing
            double prixParNuit = chambre->calculerPrixParNuit();
            double prixTotal = chambre->calculerPrixTotal(nombreJours);

            // Create and save reservation
            auto res = make_shared<Reservation>(client->getNumeroClient(), numChambre, dateDebut, dateFin, nombreJours, prixTotal);
            hotel.ajouterReservation(res);
            chambre->setOccupe(true);
            hotel.sauvegarderReservations();
            hotel.sauvegarderChambres();

            // Display confirmation
            cout << "\nReservation created successfully!" << endl;
            cout << "Reservation number: " << res->getNumeroReservation() << endl;
            cout << "Room: " << numChambre << " [" << chambre->getNomType() << "]" << endl;
            cout << "Period: " << dateDebut << " -> " << dateFin << endl;
            cout << "Number of nights: " << nombreJours << endl;
            cout << "Price per night: " << prixParNuit << " DH" << endl;
            cout << "Total price: " << prixTotal << " DH" << endl;
            break;
        }

        case 2: {
            // View client's reservations
            hotel.afficherReservationsClient(client);
            break;
        }

        case 3: {
            // Update existing reservation
            gestionreservation(hotel, client);
            break;
        }

        case 4: {
            // Cancel reservation
            hotel.afficherReservationsClient(client);

            int numReservation;
            cout << "\nReservation number to cancel (select [0] to exit): ";
            try {
                cin >> numReservation;
                if (cin.fail()) throw exception();
                cin.ignore();
            }
            catch (...) {
                cin.clear();
                cin.ignore(numeric_limits<streamsize>::max(), '\n');
                cout << "Invalid input!" << endl;
                break;
            }

            if (numReservation == 0) {
                break;
            }

            // Confirm cancellation
            int confirmation;
            cout << "Are you sure you want to cancel reservation #" << numReservation << "?" << endl;
            cout << "[1] Yes, cancel" << endl;
            cout << "[0] No, return" << endl;
            cout << "Choice: ";
            try {
                cin >> confirmation;
                if (cin.fail()) throw exception();
                cin.ignore();
            }
            catch (...) {
                cin.clear();
                cin.ignore(numeric_limits<streamsize>::max(), '\n');
                cout << "Invalid input!" << endl;
                break;
            }

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

// ============================================================================
// STAFF MENU HANDLER
// Main interface for staff operations after login
// ============================================================================
void menuStaff(Hotel& hotel, shared_ptr<Staff> staff) {
    int choix;
    do {
        clearScreen();
        cout << "\nWelcome " << *staff << " [" << staff->getPoste() << "]" << endl;
        afficherMenuStaff();

        try {
            cin >> choix;
            if (cin.fail()) {
                cin.clear();
                cin.ignore(numeric_limits<streamsize>::max(), '\n');
                cout << "\nInvalid input! Please enter a number." << endl;
                cout << "Press Enter to continue...";
                cin.get();
                continue;
            }
            cin.ignore();
        }
        catch (...) {
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            cout << "\nInvalid input!" << endl;
            cout << "Press Enter to continue...";
            cin.get();
            continue;
        }

        switch (choix) {
        case 1: {
            // Create new rooms
            menuCreationChambre(hotel);
            break;
        }

        case 2: {
            // Manage existing rooms
            menuGestionChambre(hotel);
            break;
        }

        case 3: {
            // Update client information
            hotel.afficherTousClients();

            int numeroClient;
            cout << "\nClient number to modify (select [0] to exit): ";
            try {
                cin >> numeroClient;
                if (cin.fail()) throw exception();
                cin.ignore();
            }
            catch (...) {
                cin.clear();
                cin.ignore(numeric_limits<streamsize>::max(), '\n');
                cout << "Invalid input!" << endl;
                cout << "\nPress Enter to continue...";
                cin.get();
                break;
            }

            if (numeroClient == 0) {
                cout << "\nPress Enter to continue...";
                break;
            }

            hotel.modifierInformationsClient(numeroClient);
            break;
        }

        case 4: {
            // Manage reservations (staff can cancel any reservation)
            hotel.afficherToutesReservations();

            int numReservation;
            cout << "\nReservation number to cancel (select [0] to exit): ";
            try {
                cin >> numReservation;
                if (cin.fail()) throw exception();
                cin.ignore();
            }
            catch (...) {
                cin.clear();
                cin.ignore(numeric_limits<streamsize>::max(), '\n');
                cout << "Invalid input!" << endl;
                cout << "\nPress Enter to continue...";
                cin.get();
                break;
            }

            if (numReservation == 0) { break; }

            int confirmation;
            cout << "Are you sure you want to cancel reservation #" << numReservation << "?" << endl;
            cout << "[1] Yes, cancel" << endl;
            cout << "[0] No, return" << endl;
            cout << "Choice: ";
            try {
                cin >> confirmation;
                if (cin.fail()) throw exception();
                cin.ignore();
            }
            catch (...) {
                cin.clear();
                cin.ignore(numeric_limits<streamsize>::max(), '\n');
                cout << "Invalid input!" << endl;
                cout << "\nPress Enter to continue...";
                cin.get();
                break;
            }

            if (confirmation == 1) {
                hotel.annulerReservation(numReservation);
            }
            else {
                cout << "Cancellation cancelled." << endl;
            }


            break;
        }

        case 5: {
            // Human Resources Management
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

// ============================================================================
// MAIN FUNCTION
// Entry point: Handles initial menu and user authentication
// ============================================================================
int main() {
    // Create hotel instance (loads all data from files)
    Hotel hotel;
    int choix;

    do {
        afficherMenuPrincipal();
        try {
            cin >> choix;
            if (cin.fail()) {
                cin.clear();
                cin.ignore(numeric_limits<streamsize>::max(), '\n');
                cout << "\nInvalid input! Please enter a number." << endl;
                cout << "Press Enter to continue...";
                cin.get();
                continue;
            }
            cin.ignore();
        }
        catch (...) {
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            cout << "\nInvalid input!" << endl;
            cout << "Press Enter to continue...";
            cin.get();
            continue;
        }

        switch (choix) {
        case 0: {
            // Exit application
            clearScreen();
            cout << "\nThank you for using the Hotel Management System!" << endl;
            cout << "Goodbye!" << endl;
            break;
        }

        case 1: {
            // Client Login
            string cinClient;
            string motDePasse;

            bool passwordCorrect = false; 
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
                    cout << "\nClient not found!" << endl;
                    cout << "\n[1] Try again" << endl;
                    cout << "[0] Return to main menu" << endl;
                    cout << "\nChoice: ";
                    int choixClient;
                    try {
                        cin >> choixClient;
                        if (cin.fail()) throw exception();
                        cin.ignore();
                    }
                    catch (...) {
                        cin.clear();
                        cin.ignore(numeric_limits<streamsize>::max(), '\n');
                        cout << "Invalid input!" << endl;
                        continue;
                    }
                    
                    continue;
                }
                bool passwordCorrect = false;
                bool goBackToMenu = false;

                // Password verification loop with retry option
                while (!passwordCorrect && !goBackToMenu) {
                    cout << "Password: ";
                    getline(cin, motDePasse);

                    auto clientConnecte = hotel.connexionClientAvecMotDePasse(cinClient, motDePasse);
                    if (clientConnecte) {
                        passwordCorrect = true;
                        loginSuccess = true;
                        menuClient(hotel, clientConnecte);
                    }
                    else {
                        cout << "\n[1] Try again" << endl;
                        cout << "[0] Go back to menu" << endl;
                        cout << "\nChoice: ";
                        int retryChoice;
                        cin >> retryChoice;
                        cin.ignore();
                        if (retryChoice == 0) {
                            goBackToMenu = true;
                        }
                    }
                }

                if (goBackToMenu) {
                    break;
                }
            }
            break;
        }

        case 2: {
            // Client Registration
            clearScreen();
            cout << "\n=== New Client Registration ===" << endl;
            string nom, prenom, cinNew, tel, mdp;
            cout << "Name: ";
            getline(std::cin, nom);
            cout << "Surname: ";
            getline(std::cin, prenom);
            cout << "CIN: ";
            getline(std::cin, cinNew);
            
            // Check for duplicate CIN
            if (hotel.trouverClientParCIN(cinNew)) {
                cout << "\nThis CIN already exists in the system!" << endl;
                cout << "Please use a different CIN or login with the existing account." << endl;
                cout << "\nPress Enter to continue...";
                std::cin.get();
                continue;
            }

            cout << "Phone: ";
            getline(std::cin, tel);
            cout << "Password: ";
            getline(std::cin, mdp);

            // Create and save new client
            auto nouveauClient = make_shared<Client>(nom, prenom, cinNew, tel, mdp);
            hotel.ajouterClient(nouveauClient);
            cout << "\nAccount created successfully!" << endl;
            cout << "Your CIN is: " << cinNew << endl;
            cout << "\nPress Enter to login...";
            std::cin.get();
            break; 
        }

        case 3: {
            // Staff Login
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
                try {
                    cin >> idStaff;
                    if (cin.fail()) throw exception();
                    cin.ignore();
                }
                catch (...) {
                    cin.clear();
                    cin.ignore(numeric_limits<streamsize>::max(), '\n');
                    cout << "Invalid input!" << endl;
                    cout << "\nPress Enter to continue...";
                    cin.get();
                    continue;
                }

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
                    try {
                        cin >> choixRetry;
                        if (cin.fail()) throw exception();
                        cin.ignore();
                    }
                    catch (...) {
                        cin.clear();
                        cin.ignore(numeric_limits<streamsize>::max(), '\n');
                        continue;
                    }

                    if (choixRetry == 0) {
                        break;
                    }
                }
            }
            break;
        }

        default:
            clearScreen();
            cout << "\nInvalid choice!" << endl;
            break;
        }
    } while (choix != 0);
    // Save all data before exiting
    hotel.sauvegarderTout();
    return 0;
}
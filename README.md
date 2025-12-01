# hotelCpp
PLAN DU PROJET C++:

constructeur+ destructeur -> methode new/delete (allocation dynamique).

//add skeleton diagram (relation between classes)
LES CLASSES :
personne(int age, string nom) : client(string CIN), staff(string poste, double salaire)


chambre (int numero,bool estOccupe,double prixBase):ChambreSimple(double pourcentage), doublee(double pourcentage), suite(double pourcentage)


// class pour stocker et gérer toutes les données du système
hotel (
vector<Chambre*> chambres
vector<Client> clients
vector<Employe> employes
vector<Reservation> reservations)


reservation(int idReservation,string client,Chambre* chambre,string dateDebut,string dateFin,double prixTotal)

methodes:
getfromfile();
loadfromfile();
facture();
afficher();
afficher_liste_des_chambre()

libraries :
include <chrono>; (pour calculer les nombres de nuit entre deux dates )

support:

https://chatgpt.com/share/692845e5-b540-8008-96b6-766ec899ebab

=================================================================
// using switchcase


Gestion d'hotelerie
username
password
welcome $user
----- MENU CLIENT ----- //to fix : detailing / adding descriptive comments
[1] Faire une réservation
[2] Afficher les infos de réservation
[3] Modifier une réservation
[4] Annuler une réservation
[0] Retour au menu principal
----- MENU STAFF -----
[1] Créer une chambre
[2] Modifier une chambre
[3] Modifier un client
[4] Modifier une réservation
[5] Gérer le staff
[6] Modifier les prix
[0] Retour au menu principal


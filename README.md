# hotelCpp
PLAN DU PROJET C++:

constructeur+ destructeur -> methode new/delete (allocation dynamique).


LES CLASSES :
https://emsiedu-my.sharepoint.com/:i:/g/personal/mohamedhamza_halouani_emsi-edu_ma/EVYkHa3KUU5BhQqDhU-FGyYBWAVK1eeo6rRr7mVQl5I-Cw?e=rdiXcJ

https://emsiedu-my.sharepoint.com/:i:/g/personal/mohamedhamza_halouani_emsi-edu_ma/EWT2DgMp13JNqUr-0o_lAikBF4Fj-ApMV0XZDx_LXUd32w?e=hMVF2F

https://emsiedu-my.sharepoint.com/:w:/g/personal/mohamedhamza_halouani_emsi-edu_ma/EayZaY5iHHdEjpGdxj2mYUsBa5LSINZWVxXYwFDeNSpfxQ?e=KiCQbg

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

Gestion d'hotelerie
username
password
welcome $user


// ========================= MENU CLIENT =========================
// Ce menu permet aux clients de l’hôtel de gérer tout le cycle de vie de leur réservation.


[1] Saisie d’une demande de réservation      
// → Créer une nouvelle demande de réservation.
//   Le système collectera les informations du client, les dates, le type de chambre, etc.

[2] Visualisation du dossier de réservation  
// → Afficher toutes les données enregistrées liées à une réservation existante.
//   Utilisé pour vérifier les dates, les détails de la chambre et les informations du client.

[3] Actualisation d’une réservation        
// → Modifier une réservation existante (changer dates, type de chambre, etc.)
//   Applique des règles de validation (disponibilité, vérification des dates, etc.)

[4] Procédure d’annulation de réservation    
// → Annuler une réservation existante.
//   Le système met à jour les disponibilités et supprime ou marque la réservation.

[0] Retour module principal                
// → Revenir au menu principal du système (menu racine).



// ========================== MENU STAFF =========================
// Ce menu est utilisé par le personnel de l’hôtel pour gérer les données opérationnelles.
// Le personnel a accès à des fonctions administratives non accessibles aux clients.


[1] Création d’une unité d’hébergement       
// → Créer une nouvelle chambre dans le système (Chambre Simple, Double, Suite...)
//   Le personnel saisit le numéro de chambre, la capacité, le prix de base et le type.

[2] Gestion d’une unité d’hébergement       
// → Modifier ou gérer les chambres existantes : changer le prix, l’état, le type, etc.

[3] Mise à jour des données clients          
// → Modifier les informations des clients stockées dans la base de données (nom, CIN, etc.)

[4] Gestion des réservations                 
// → Gestion des réservations par le personnel.
//   Permet de rechercher, modifier, valider ou supprimer des réservations.

[5] Gestion des ressources humaines          
// → Gérer les données du personnel (liste des employés, rôles, horaires, etc.)

[6] Paramétrage de la tarification hôtelière
// → Ajuster les règles de tarification, les prix saisonniers, remises, taxes, etc.

[0] Retour module principal                  
// → Revenir au menu principal de l’application.




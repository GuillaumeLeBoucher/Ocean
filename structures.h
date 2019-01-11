#define MAXNOM 31

typedef struct ClientNode { // structure du client
    int data;// Numéro de la socket
    struct ClientNode* prev;
    struct ClientNode* link;
    char ip[16];
    char name[31];
    char position[10];
    int x;
    int y;
    int vitesse;
    char direction[3];
} ClientList;

ClientList *newNode(int sockfd, char* ip) { //creation d'un espace pour un potentiel client
    ClientList *np = (ClientList *)malloc( sizeof(ClientList) );
    np->data = sockfd; //code bateau
    np->prev = NULL;
    np->link = NULL;
    strncpy(np->ip, ip, 16);
    strncpy(np->name, "NULL", 5);
    return np;
}


/**
 * structure bateau (A enlever ?)
 * ----------------
 */
typedef struct bateau
{
        char nom[MAXNOM];
        int identifiant;
        int * pidentifiant;
        /*beaucoup plus pratique pour l'envoi du buffer initial */
        char vitesse[2];  
        /* ajout de la même vitesse en int pour les "vraies" 
        manipulations */
        int vitesse_int;
        char direction[3];
        char position[10];
        int x;
        int y;
} Bateau;
 
 
/**
 * structure annuaire(A enlever ?)
 * ------------------
 */
typedef struct annuaire
{
	char identifiants_pris[4];
    struct bateau navire1;
    struct bateau navire2;
    struct bateau navire3;
} Annuaire;


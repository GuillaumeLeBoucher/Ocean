#include <errno.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <stdio.h>
#include <signal.h>
#include <netinet/in.h>
#include <netdb.h>
#include <stdlib.h>
#include <string.h>
#include <arpa/inet.h>
#include <ctype.h>


/**
 * fonction inscription
 * ====================
 *
 * inscrit un navire les données récupérées du buffer initial.
 *
 * Paramètres
 * ----------
 * Bateau * navire : pointeur vers un navire (struct) dont on met les attributs
 *          	     à jour
 * char * buffer   : buffer de la forme "Arago240N_003Ene4&" contenant les 
 * 				     informations nécessaires pour combler les vides de la 
 *					 structure navire.
 */
void caracteriser_navire(Bateau * navire, char * buffer);

/**
 * fonction est_bateau
 * ====================
 *
 * premet de verifier qu'on a bien un code bateau
 *
 * Paramètres
 * ----------
 * char message_entrant_buffer : le message qu'envoie le client
 */

int est_bateau (char* message_entrant_buffer);

/**
 * fonction nom_bateau
 * ====================
 *
 *Permet de renvoyer le nom du bateau
 *
 * Paramètres
 * ----------
 * char message_entrant_buffer : le message qu'envoie le client
 */

char* nom_bateau (char* message_entrant_buffer);

/**
 * fonction init_position
 * ====================
 *
 *Permet de renvoyer la position initiale du bateau (celle envoyée dans le buffer)
 *
 * Paramètres
 * ----------
 * char message_entrant_buffer : le message qu'envoie le client
 */

char* init_position (char* message_entrant_buffer);

/**
 * fonction init_direction
 * ====================
 *
 *Permet de renvoyer la direction initiale du bateau (celle envoyée dans le buffer)
 *
 * Paramètres
 * ----------
 * char message_entrant_buffer : le message qu'envoie le client
 */

char* init_direction (char* message_entrant_buffer);

/**
 * fonction init_vitesse
 * ====================
 *
 *Permet de renvoyer la vitesse initiale du bateau (celle envoyée dans le buffer)
 *
 * Paramètres
 * ----------
 * char message_entrant_buffer : le message qu'envoie le client
 */
int init_vitesse (char* message_entrant_buffer);

/**
 * fonction inscrire_navire
 * ========================
 *
 * inscrit le navire dans l'annuaire.
 *
 * Paramètres
 * ----------
 * Bateau * navire           : navire à inscrire
 * Annuaire * mon_annuaire   : pointeur vers l'annuaire
 * 
 */
void inscrire_navire(Bateau * navire, Annuaire * mon_annuaire);


/**
 * fonction affiche_navire
 * =======================
 *
 * affiche les caractéristiques d'un navire.
 * 
 */
void affiche_navire(Bateau navire);
/**
 * fonction affiche_annuaire
 * =========================
 *
 * affiche le contenu de l'annuaire. Sont inscrits dans l'annuaire uniquement
 * les navires actuellement en mer.
 * 
 */
void affiche_annuaire(Annuaire annuaire);

/**
 * fonction deplacement
 * =========================
 *
 * Fonction qui réalise les déplacemets du bateau en fonction de la direction du vent et de la vitesse initiale.
 * variables globales utilisées dirvent, forvent et le coefficient pour le temps kT, j'ai mis les dimensions de la carte en paramètres, mais cela peut être des var globales (H limite en y et L en x)
 *
 */
void deplacement(ClientList *Clientcourant,int H, int L,char dirvent, int forvent);
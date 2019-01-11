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
#include "fonctions_serveur.h"

#define max(x, y) (((x) > (y)) ? (x) : (y))


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
void caracteriser_navire(Bateau * navire, char * buffer)
{
	int i;
	char x_buf[3];
	char y_buf[3];
	bzero(y_buf, sizeof(y_buf));
	char vitesse_buf[3];
	bzero(vitesse_buf, sizeof(vitesse_buf));
	char* symbole_fin_buffer = "&";

	/* Pour inscrire le nom dans la variable navire, on lit chacune 
	   des premières lettres du buffer au plus 30 fois, puisque 30 
	   est la taille maximale d'un nom tel que défini dans 
	   structures.h ... */
	for (i=0;i<30;i++)
	{
		/* ...puis on les place dans la structure si ce sont
		   bien des lettres ...

					Arago240N_003Ene4&
					^
                                     buffer[0]	 
		 */
		if (isalpha(buffer[i])) 
		{
	    	char to_concatenate[] = {buffer[i], '\0'};
	    	strcat(navire->nom, to_concatenate);
		}
		/* ...et on arrête dès qu'on tombe sur un chiffre (le début
		   de la position) :
				
					Arago240N_003Ene4&
					     ^
					  buffer[5]
		 */
		else
		{
			break;
		}
	}
	/* La taille d'une position est fixe de la forme :

	   				240N_003E

	   On copie les trois digits dans un buffer temporaire y_buf :

	  				240N_003E
                                        ^--      _______
                                          ╰---> | y_buf |
                                                ╰-------╯
					
	   */
	strncpy(y_buf, &buffer[i], 3);  // N ~ Nord ~ y

	/* on convertit ce buffer en entier et on le met dans la 
	   structure */
	navire->y = atoi(y_buf);

	i += 5;
	
	/* on avance jusqu'au trois digits suivants et on fait la même
	   avec x_buf :

	  				240N_003E
                                             ^--      _______
                                               ╰---> | x_buf |
                                                     ╰-------╯

	 */
	
	strncpy(x_buf, &buffer[i], 3);  // E ~ East ~ x
	/* on convertit ce buffer en entier et on le met dans la 
	   structure */
	navire->x = atoi(x_buf);
	i += 4;

	/* La direction :

					Arago240N_003Ene4&
                                                      ^
	*/
	
	char dir_to_concatenate[] = {buffer[i], buffer[i+1], '\0'};
	strcat(navire->direction, dir_to_concatenate);
	i += 2; 

	/* Pour la vitesse, on copie les digits dans le buffer temporaire 
	   vitesse_buf jusqu'à rencontrer le terme spécial "&":

					Arago240N_003Ene4&
                                                        ^
	*/
	
	do
	{
		strncat(vitesse_buf, &buffer[i], 1);
		i += 1;
	}while(strcmp(&buffer[i], symbole_fin_buffer) != 0);
	
	navire->vitesse_int = atoi(vitesse_buf);

	
}

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

int est_bateau (char* message_entrant_buffer){
    char pos_x = message_entrant_buffer[8];
    char pos_y = message_entrant_buffer[13];
    char tiret = message_entrant_buffer[9];
    char direction = message_entrant_buffer[14];
    char fin = message_entrant_buffer[17];
    int control = 1;
    int i;
    for (i = 0; i < 5; i++)
    {
        if (isdigit((char) message_entrant_buffer[i]) != 0 ) { // si on a un chiffre, erreur
            control = 0;
        }       
    }
    for (i = 5; i < 8; i++)
    {
        if (isdigit(message_entrant_buffer[i]) == 0 ) { // si on a un caractère, erreur
            control = 0;
        }   
    }
    if (strcmp(&pos_x, "N") == 0){
        control =0;
        } 

    if (strcmp(&tiret, "_") == 0){
        control =0;
        }
    for (i = 10; i < 13; i++)
    {
        if (isdigit(message_entrant_buffer[i]) == 0 ) { // si on a un caractère, erreur
            control = 0;
        }   
    }
    if (strcmp(&pos_y, "E") == 0){
        control =0;
        } 
    if (strcmp(&direction, "n") == 0 || strcmp(&direction, "s") == 0 || strcmp(&direction, "e") == 0 || strcmp(&direction, "o") == 0){
        control =0;
        }
    for (i = 15; i < 17; i++)
    {
        if (isdigit(message_entrant_buffer[i]) == 0 ) { // si on a un caractère, erreur
            control = 0;
        }   
    }
    if (strcmp(&fin, "&") == 0){
        control =0;
        }
	return control;
}

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

char* nom_bateau (char* message_entrant_buffer){
    char * nom_bat;
    nom_bat = malloc(sizeof(char)*100);
    
    nom_bat[0] = message_entrant_buffer[0];
    nom_bat[1] = message_entrant_buffer[1];
    nom_bat[2] = message_entrant_buffer[2];
    nom_bat[3] = message_entrant_buffer[3];
    nom_bat[4] = message_entrant_buffer[4];
	return nom_bat;
}
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

char* init_position (char* message_entrant_buffer){
    char * position;
    position = malloc(sizeof(char)*100);
	
	position[0] = message_entrant_buffer[5];
	position[1] = message_entrant_buffer[6];
	position[2] = message_entrant_buffer[7];
	position[3] = message_entrant_buffer[8];
	position[4] = message_entrant_buffer[9];
	position[5] = message_entrant_buffer[10];
	position[6] = message_entrant_buffer[11];
	position[7] = message_entrant_buffer[12];
	position[8] = message_entrant_buffer[13];
	return position;
}
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
char* init_direction (char* message_entrant_buffer){
    char * direction;
    direction = malloc(sizeof(char)*100);
	
	direction[0] = message_entrant_buffer[14];
	return direction;
}

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
int init_vitesse (char* message_entrant_buffer){
    int * p_vitesse;
    int vitesse_reelle;
    p_vitesse = malloc(sizeof(int)*100);
	*p_vitesse = atoi(&message_entrant_buffer[15]);//+ atoi(&message_entrant_buffer[16]);
	//printf(" voila la vitesse buffer : %d + %d \n",atoi(&message_entrant_buffer[15]),atoi(&message_entrant_buffer[16]));
	vitesse_reelle = *p_vitesse;
	free(p_vitesse);
	return vitesse_reelle;
}

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
void inscrire_navire(Bateau * navire, Annuaire * mon_annuaire)
{
	mon_annuaire->navire1 = *navire;
}


/**
 * fonction affiche_navire
 * =======================
 *
 * affiche les caractéristiques d'un navire.
 * 
 */
void affiche_navire(Bateau navire)
{
		printf("nom du navire ................................ %s\n", 
			navire.nom);
		printf("identifiant .................................. %d\n", 
			navire.identifiant);
		printf("direction .................................... %s\n", 
			navire.direction);
		printf("vitesse ...................................... %d\n", 
			navire.vitesse_int);
		printf("coordonnée Y (Nord) .......................... %d\n", navire.y);
		printf("coordonnée X (Est) ........................... %d\n", navire.x);
}

/**
 * fonction affiche_annuaire
 * =========================
 *
 * affiche le contenu de l'annuaire. Sont inscrits dans l'annuaire uniquement
 * les navires actuellement en mer.
 * 
 */
void affiche_annuaire(Annuaire annuaire)
{
	int n_navires = 0;

	printf("VOICI LES NAVIRES CONTENUS DANS L'ANNUAIRE :\n");
	printf("---------------------------------------------------------\n");

	if (atoi(&annuaire.identifiants_pris[0]))
	{
		n_navires ++;
		affiche_navire(annuaire.navire1);
	}
	if (atoi(&annuaire.identifiants_pris[1]))
	{
		n_navires ++;
		printf("\n\n");
		affiche_navire(annuaire.navire2);
	}
	if (atoi(&annuaire.identifiants_pris[2]))
	{
		n_navires ++;
		printf("\n\n");
		affiche_navire(annuaire.navire3);
	}

    printf("---------------------------------------------------------\n");
	printf("TOTAL : %d\n", n_navires);
}

/**
 * fonction deplacement
 * =========================
 *
 * Fonction qui réalise les déplacemets du bateau en fonction de la direction du vent et de la vitesse initiale.
 * variables globales utilisées dirvent, forvent et le coefficient pour le temps kT, j'ai mis les dimensions de la carte en paramètres, mais cela peut être des var globales (H limite en y et L en x)
 *
 */

void deplacement(ClientList *Clientcourant,int H, int L,char dirvent, int forvent)
{
	//px!=&x en théorie
	int pv=Clientcourant->vitesse;
	int px=Clientcourant->x;
	int py=Clientcourant->y;
	int kT = 1;
	char dir=Clientcourant->direction[0];
	//Partie concernant le changement de vitesse du bateau
	switch(dir){
		case 'N': 
			switch (dirvent){
				case 'N':
					 Clientcourant->vitesse=max((int)(pv*(1+forvent/10)),20);
					 Clientcourant->x=(px+kT*(Clientcourant->vitesse))%L;
					 Clientcourant->y=py;
					 break;
				case 'S':
					 Clientcourant->vitesse=(int)(pv*1/forvent);
					 Clientcourant->x=(px+kT*(Clientcourant->vitesse))%L;
					 Clientcourant->y=py;
					 break;
				case 'E':
					 Clientcourant->vitesse=(int)(pv*1/forvent);
					 Clientcourant->x=(px+kT*(Clientcourant->vitesse))%L;
					 Clientcourant->y=(py+kT*(Clientcourant->vitesse))%H;
					 break;
				case 'O':
					 Clientcourant->vitesse=(int)(pv*1/forvent);
					 Clientcourant->x=(px+kT*(Clientcourant->vitesse))%L;
					 Clientcourant->y=(py-kT*(Clientcourant->vitesse))%H;
					 break;
			}
			break;
		case 'S':
			switch (dirvent){
				case 'N':
					 Clientcourant->vitesse=(int)(pv*1/forvent);
					 Clientcourant->x=(px-kT*(Clientcourant->vitesse))%L;
					 Clientcourant->y=py;
					 break;
				case 'S':
					 Clientcourant->vitesse=max((int)(pv*(1+forvent/10)),20);
					 Clientcourant->x=(px-kT*(Clientcourant->vitesse))%L;
					 Clientcourant->y=py;
					 break;
				case 'E':
					 Clientcourant->vitesse=(int)(pv*1/forvent);
					 Clientcourant->x=(px-kT*(Clientcourant->vitesse))%L;
					 Clientcourant->y=(py+kT*(Clientcourant->vitesse))%H; 
					 break;
				case 'O':
					 Clientcourant->vitesse=(int)(pv*1/forvent);
					 Clientcourant->x=(px-kT*(Clientcourant->vitesse))%L;
					 Clientcourant->y=(py-kT*(Clientcourant->vitesse))%H;
					 break;
			}
			break;
		case 'E':
			switch (dirvent){
				case 'N':
					 Clientcourant->vitesse=(int)(pv*1/forvent);
					 Clientcourant->x=(px+kT*(Clientcourant->vitesse))%L;
					 Clientcourant->y=(py+kT*(Clientcourant->vitesse))%H; 
					 break;
				case 'S':
					 Clientcourant->vitesse=(int)(pv*1/forvent);
					 Clientcourant->x=px;
					 Clientcourant->y=(py+kT*(Clientcourant->vitesse))%H;
					 break;
				case 'E':
					 Clientcourant->vitesse=max((int)(pv*(1+forvent/10)),20);
					 Clientcourant->x=px;
					 Clientcourant->y=(py+kT*(Clientcourant->vitesse))%H;
					 break;
				case 'O':
					 Clientcourant->vitesse=(int)(pv*1/forvent);
					 Clientcourant->x=px;
					 Clientcourant->y=(py+kT*(Clientcourant->vitesse))%H;
					 break;
			}
			break;
		case 'O':
			switch (dirvent){
				case 'N':
					 Clientcourant->vitesse=(int)(pv*1/forvent);
					 Clientcourant->x=(px+kT*(Clientcourant->vitesse))%L;
					 Clientcourant->y=(py-kT*(Clientcourant->vitesse))%H;
					 break;
				case 'S':
					 Clientcourant->vitesse=(int)(pv*1/forvent);
					 Clientcourant->x=(px-kT*(Clientcourant->vitesse))%L;
					 Clientcourant->y=(py-kT*(Clientcourant->vitesse))%H;
					 break;
				case 'E':
					 Clientcourant->vitesse=(int)(pv*1/forvent);
					 Clientcourant->x=px;
					 Clientcourant->y=(py-kT*(Clientcourant->vitesse))%H;
					 break;
				case 'O':
					 Clientcourant->vitesse=max((int)(pv*(1+forvent/10)),20);
					 Clientcourant->x=px;
					 Clientcourant->y=(py-kT*(Clientcourant->vitesse))%H;
					 break;
			}
			break;
		}	
return;
}
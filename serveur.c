#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <signal.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <pthread.h>
#include <errno.h>
#include <netdb.h>
#include <ctype.h>


#include "structures.h"
#include "fonctions_serveur.c"
#include "serveur.h"
#include "donnees.h"
#include "string.h"
#include "carte.h"

// Global variables
int ssocket = 0;
int csocket = 0;
char direction_vent;
int force_vent;
int server_sockfd = 0, client_sockfd = 0;
char *map;
int liste_rochers[100];
int nombre_rochers = 0 ;
ClientList *root, *now;


/**
 * fonction print_msg 
 * ------------------
 * 
 */
void print_msg(char *talker, char * chat) 
{
	fputs(talker, stdout);
	fputs(": ", stdout);
	fputs(chat, stdout);
	fflush(stdout);
}

/**
 * fonction quitter_sock
 * ------------------
 * 
 */

void quitter_sock(int sig) {// On cherche ici a fermer toutes les sockets lorsqu'on va shutdown le systeme
    ClientList *tmp;
    while (root != NULL) {// Tant qu'on a pas fermer la racine on continue, erreur de compilation lorsqu'on ferme le serveur, à corriger
        printf("\nFermeture Socket: %d\n", root->data);
        close(root->data); // On ferme la socket sur laquelle on se situe puis on remonte les liens
        tmp = root;
        root = root->link;
        free(tmp);
    }
    printf("Au revoir\n");// On a tout fermer, meme le serveur lui meme
    exit(EXIT_SUCCESS);
}

/**
 * fonction send_to_all_clients pour le chat et l'envoi de l'ocean toutes les minutes
 * ------------------
 * 
 */
void send_to_all_clients(ClientList *Client_courant, char tmp_buffer[]) {// Permet d'envoyer un message à tous les clients d'un coup
    ClientList *tmp = root->link;
    while (tmp != NULL) {// Tant qu'il reste des liens à explorer
        if (Client_courant->data != tmp->data) { // On envoi à tous les clients sauf à lui-meme
            printf("Envoyer a la socket %d: \"%s\" \n", tmp->data, tmp_buffer);
            send(tmp->data, tmp_buffer, LENGTH_SEND, 0);
        }
        tmp = tmp->link;
    }
}

/**
 * fonction gestion_vent pour changer la direction et la force du vent
 * ------------------
 * 
 */
void modif_vent(){
    force_vent = 0; // on initialise le vent à 0
    char message_console[LENGTH_MSG] = {};
    while(1){
		if (fgets(message_console, 32, stdin) != NULL && strcmp(left(message_console, 4),"vent") ==0 ) {
	                force_vent =  atoi(&message_console[4]);
	                direction_vent= message_console[5];
	                printf("Félicitation, vous venez de changer la force et la direction du vent, elle est de :%d%c\n", force_vent,direction_vent);
	                //fflush(stdin);
	    	}
	    else{
	        	printf("Malheureusement, la forme pour changer le vent n'est pas la bonne\n");
	        	//fflush(stdin);
	        	
	    	}
	}
}





/**
 * fonction client_handler pour gerer l'ensemble des connections qui rentrent dans le système
 * 
 */

void client_handler(void *p_client) { //gestionnaire de client, permet au serveur de gerer la liste de client
    int control_false = 0; // permet de controller si il n'y a pas d'erreur, passe a 1 si c'est le cas
    char code_bateau_entrant[LENGTH_NAME] = {};
    char recv_buffer[LENGTH_MSG] = {};
    char send_buffer[LENGTH_SEND] = {};
    ClientList *Client_courant = (ClientList *)p_client;

    char position[10] = {};  // obsolète, il faut gerer ça dans la stucture client
    char direction[3] = {};
    int vitesse =0;
    // Nomage des clients qui rentrent pour les iidentifier dans le serveur
    
    if (recv(Client_courant->data, code_bateau_entrant, LENGTH_NAME, 0) <= 0 || est_bateau(code_bateau_entrant) == 0 ) {
        printf("%s Erreur code bateau \n", Client_courant->ip);//si on ne recoit pas le bon code bateau, erreur
        control_false = 1;
    } else {
    	char * nom_bat = nom_bateau(code_bateau_entrant);
    	char * position = init_position(code_bateau_entrant);
    	char * direction = init_direction(code_bateau_entrant);
        vitesse = init_vitesse(code_bateau_entrant);
    	strncpy(Client_courant->position, position, 10);
    	strncpy(Client_courant->direction, direction, 10);
        strncpy(Client_courant->name, nom_bat, LENGTH_NAME); // On copie le nom que le client a rentre dans une chaine de char usable
        Client_courant->vitesse = vitesse;
        free(nom_bat);
        free(position);
        free(direction);
        int i = est_place_libre(Client_courant->position, liste_rochers, nombre_rochers);
        printf("%d\n", i);
        if (i != 0) { //le i est toujours égal à 1, probleme dans la fonction !!
            map=ajouter_bateau(map,position);   
            printf("Le bateau %s(%s)(%d) a rejoint l'ocean à la position %s en direction %s, à la vitesse %d.\n", Client_courant->name, Client_courant->ip, Client_courant->data, Client_courant->position,Client_courant->direction,Client_courant->vitesse);
            bzero(send_buffer,sizeof(send_buffer));
            sprintf(send_buffer, "Le bateau %s(%s) a rejoint l'ocean à la position %s en direction %s", Client_courant->name, Client_courant->ip, Client_courant->position, Client_courant->direction);// On met dans le buffer et on envoi a tout le monde que le nouveau bateau a rejoint l'ocean
            send_to_all_clients(Client_courant, send_buffer); // on envoi à tout le monde que un client vient de rejoindre la salle
            //print_map(map);
            printf("map");
        }
        else{
            printf("%s Erreur, le bateau se trouve sur une position deja occupée par un rocher ou un  autre bateau \n", Client_courant->ip);//si on ne recoit pas le bon code bateau, erreur
            bzero(send_buffer,sizeof(send_buffer));
            sprintf(send_buffer, "Quit");
            send(Client_courant->data, send_buffer, LENGTH_MSG, 0);
            control_false = 1;
        }
    }

    // Conversation entre les clients 
    while (1) {
        if (control_false ==1) {
            break;
        }    	
        
        int receive = recv(Client_courant->data, recv_buffer, LENGTH_MSG, 0);
        if (receive > 0) {
	    if (strcmp(recv_buffer, "P") == 0 ){
	    	printf("Le bateau %s(%s)(%d) a demandé sa position'.\n", Client_courant->name, Client_courant->ip, Client_courant->data);
			/* on copie la position dans le buffer et on l'envoit */
			sprintf(send_buffer, "%s, vous êtes à la position %s", Client_courant->name, Client_courant->position);
			//strncpy(buffer, position, 11);
			send(Client_courant->data, send_buffer, strlen(send_buffer), 0);
	    	/* on copie la position dans le buffer et on l'envoit */
	    	sprintf(send_buffer, "%s(%s) est à la position %s", Client_courant->name, Client_courant->ip, Client_courant->position);
            }
        else if (strlen(recv_buffer) == 0) {
                continue;
            }
            sprintf(send_buffer, "%s：%s from %s", Client_courant->name, recv_buffer, Client_courant->ip);

        } else if (receive == 0 || strcmp(recv_buffer, "exit") == 0) {
            printf("%s(%s)(%d) a quitté l'océan\n", Client_courant->name, Client_courant->ip, Client_courant->data);
            sprintf(send_buffer, "%s(%s) quitte l'océan", Client_courant->name, Client_courant->ip);
            control_false = 1;

	} else {
            printf("Erreur: -1\n");
            control_false = 1;
        }
        send_to_all_clients(Client_courant, send_buffer);
    }

    // Pour enlever un noeud
    close(Client_courant->data);
    if (Client_courant == now) { //on enleve un noeud situé au bord
        now = Client_courant->prev;
        now->link = NULL;
    } else { // on enleve un noeud situé au millieu
        Client_courant->prev->link = Client_courant->link;
        Client_courant->link->prev = Client_courant->prev;
    }
    //free(Client_courant);
}

/**
 * fonction client2code
 * ====================
 *
 * Retourne un code navire à partir d'un client.
 *
 *
 *
 * Paramètre:
 * ----------
 * ClientList * client : client dont on veut obtenir le code navire
 *
 * Retourne:
 * ---------
 * char * code_client : code client (sans le nom) de la forme 231N_003En21
 *
 */
char * client2code(ClientList * courant)
{

    char code_client[13] = {};
    int now_x = courant->x;
    int now_y = courant->y;
    int now_v = courant->vitesse;
    char * direction = courant->direction;
    char now_y_str[4]= {};
    char now_x_str[4]= {};
    char now_v_str[3]= {};

    //eventuellement remettre à zero le now_y_str mais pas faire bzero
    sprintf(now_y_str, "%d", now_x);
    strncpy(code_client, now_y_str, sizeof(now_y_str));
    strcat(code_client, "N_");

    //eventuellement remettre à zero le now_x_str mais pas faire bzero
    sprintf(now_x_str, "%d", now_x);
    strcat(code_client, now_x_str);
    strcat(code_client, "E");

    strcat(code_client, direction);

        if (now_v > 99)
        {
            perror("client_2_code lit une vitesse à trois chiffres");
            exit(EXIT_FAILURE);

        }

    //eventuellement remettre à zero le now_v_str mais pas faire bzero
    sprintf(now_v_str, "%d", now_v);
    strcat(code_client, now_v_str);

    return code_client;
}

/**
 * fonction transmettre_map
 * ========================
 * 
 * Transmet à tous les clients un buffer avec les informations de tous les  
 * clients, toutes les minutes
 *
 *
 */
void transmettre_map()
{
    int k;
    printf("\naaaaaaaaa\n");
    char buffer_transmis[100] = {};
    char * commande_map = "$m";
    strncpy(buffer_transmis, commande_map, 2);
    char code_bateau[13];
    printf("\naaaaaaaaa\n");
    // il n'y a aucun client dans l'océan 
    if (root == now)
    {
        perror("aucune liste client\n");
        exit(EXIT_FAILURE);
    }

    printf("\naaaaaaaaa\n"); //erreur à prtir de là

    ClientList *tmp = root->link;
    ClientList *tmp2 = root->link; // Pour conserver la valeur de tmp pour lui renvoyer apres la valeur du buffer
    // tant que la pile n'est pas vide 
    do
    {
        bzero(code_bateau, sizeof(code_bateau));
        char * code_bateau = client2code(tmp);
        strcat(buffer_transmis, code_bateau);
    
        // les codes navire sont séparés par une virgule 
        strcat(buffer_transmis, ",");
        tmp = tmp->link;
    }while (tmp != NULL); //alors il reste des navires dans l'ocean

    // caractère de fin 
    strcat(buffer_transmis, "&");

    printf("\naaaaaaaaa\n"); //Ca n'arrive pas jusque là
    
    // on attend dix secondes 
    sleep(10);
    printf("send_map\n");
    // send_to_all envoit à tout le monde sauf à lui même... 
    send_to_all_clients(tmp, buffer_transmis);
    send(tmp2->data, buffer_transmis, strlen(buffer_transmis), 0);
}



int main(int argc, char * argv[])
{
    signal(SIGINT, quitter_sock); // permet d'interrompre la communication via le clavier et la fct quitter_sock
    //free(map);
    map = init_map(liste_rochers,nombre_rochers);
    int port;
	char nom[30];
    // Creation de la socket
    ssocket = socket(AF_INET , SOCK_STREAM , 0);
    if (ssocket == -1) {
        printf("Erreur creation socket");
        exit(EXIT_FAILURE);
    }
	
	/* Paramétrage du contexte d'adressage serveur et client*/
	/* Socket information */
	port = atoi(argv[1]);
    struct sockaddr_in serveur_info, client_info;
    int s_addrlen = sizeof(serveur_info);
    int c_addrlen = sizeof(client_info);
    memset(&serveur_info, 0, s_addrlen);
    memset(&client_info, 0, c_addrlen);
	serveur_info.sin_family=PF_INET;
	serveur_info.sin_port=htons(port);
	serveur_info.sin_addr.s_addr = htonl(INADDR_ANY);

   /* variables d'Ocean : */
	Annuaire mon_annuaire;
	/* initialisation des identifiants libres de l'annuaire */
	strcpy(mon_annuaire.identifiants_pris, "000");
	
	/* s'il manque un argument */
	if (argc!=2)
	{
		fprintf(stderr,"Usage : %s port-number", argv[0]);
		exit(1);
	}


	/* si la socket ne se crée pas */
	if ((ssocket=socket(AF_INET, SOCK_STREAM, 0)) == -1)
	{
		perror("socket rendez-vous");
		exit(1);
	}

	/* on récupère le nom de la mchine sur laquelle le serveur est lancé */
	if (gethostname(nom, 30)==-1)
	{
		perror("Qui suis-je ?");
		exit(1);
	}

	/* affichage d'un identifiant et du nom de la machine sur laquelle le 
	   serveur est lancé */
	printf("Machine: %s\n", nom);
	printf("-------------------------\n");
	/* si la socket ne se bind pas avec son contexte d'adressage */
	if (bind(ssocket, (struct sockaddr *) &serveur_info, sizeof(serveur_info))==-1)
	{
		perror("bind");
		exit(1);
	}
	/* si la socket ne rentre pas en mode listen */
	if (listen(ssocket,3)==-1)
	{
		perror("listen");
		exit(1);
	}

    /* Paramètres pour la conversation client-serveur */
	socklen_t addr_size;
	char buffer[1024];
	Bateau * navire1 = (Bateau *)malloc(sizeof(Bateau));
	Bateau * navire2 = (Bateau *)malloc(sizeof(Bateau));
	Bateau * navire3 = (Bateau *)malloc(sizeof(Bateau));
	navire1->identifiant = 0;
	navire2->identifiant = 0;
	navire3->identifiant = 0;
	int navire_courant;
	pid_t childpid;

	// Affichage de l'adresse IP du serveur
    getsockname(ssocket, (struct sockaddr*) &serveur_info, (socklen_t*) &s_addrlen);
    printf("Demarrage serveur sur: %s:%d\n", inet_ntoa(serveur_info.sin_addr), ntohs(serveur_info.sin_port));

    // On initialise un nouveau noeud pour les potentiels clients
    root = newNode(ssocket, inet_ntoa(serveur_info.sin_addr));
    now = root;
    printf("\nLe vent a été initialisé à 0, pour le modifier, rentrez : ventNew_forceNew_dir\n");
    while (1) {
        csocket = accept(ssocket, (struct sockaddr*) &client_info, (socklen_t*) &c_addrlen);

        // Affiche l'adresse IP du client
        getpeername(csocket, (struct sockaddr*) &client_info, (socklen_t*) &c_addrlen);
        printf("Le Client d'adresse %s:%d tente de se connecter à l'Ocean...\n", inet_ntoa(client_info.sin_addr), ntohs(client_info.sin_port));        /**
		 * -------------------------------------
		 * La connection est maintenant établie.
		 * -------------------------------------
		 */



        // On ajoute le nouveau client à la liste des clients
        ClientList *c = newNode(csocket, inet_ntoa(client_info.sin_addr));
        c->prev = now;
        now->link = c;
        now = c;

        pthread_t gestion_client_menu;
        if (pthread_create(&gestion_client_menu, NULL, (void *)client_handler, (void *)c) != 0) {
            perror("Pthread erreur!\n");
            exit(EXIT_FAILURE);
        }
        pthread_t gestion_vent;
        if (pthread_create(&gestion_vent, NULL, (void * )modif_vent, (void *)c) != 0) {
            perror("Pthread erreur!\n");
            exit(EXIT_FAILURE);
        }
        
        pthread_t transmission_map;
        if (pthread_create(&transmission_map, NULL, (void * )transmettre_map, (void *)c) != 0) 
        {
            perror("Pthread erreur!\n");
            exit(EXIT_FAILURE);
        }
    }

    return 0;
}

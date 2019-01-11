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

#include "carte.h"


int x=0;
int y=0;
//int liste_rochers[100];
//int nombre_rochers = 0 ;


char *init_map(int liste_rochers[100],int nombre_rochers) {
FILE * fic;

	fic = fopen("ocean.txt", "rt");
	if ( fic == NULL){
		perror("1");
	}
	char ligne[100];

	int xi = 0;
	int yi=0;
	

	int compteur =0;
	int compteur2 =0; // pour la liste de rochers

	//char map[100][100];
	

	while (compteur!=2){
		fgets(ligne,100,fic);
		if (compteur ==0) {
			x=atoi(ligne)+2;
		}
		else {
			y=atoi(ligne)+2;
		}
		compteur += 1;
	}

	char* map = malloc(x*y*sizeof(char)+1);
	for (xi=0 ; xi<x ;xi++){
		for (yi=0 ; yi<y ; yi++) {
			map[xi+yi*x]='-';
		}
	}
	while (fgets(ligne,100,fic) != NULL) {
		if (compteur%2==0) {
			xi=x-(atoi(ligne)+1);
			liste_rochers[compteur2]=atoi(ligne);
			compteur2+=1;			
		}
		else {
			yi=atoi(ligne);
			map[xi+yi*x] = 'R';
			liste_rochers[compteur2]=atoi(ligne);
			compteur2+=1;
			nombre_rochers+=1;
		}
	compteur += 1;
	
	}
	// printer la map
	/*
	for (xi=0 ; xi<x ;xi++){	
		for (yi=0 ; yi<y ; yi++) {
			if(xi==0||yi==0||xi==x-1||yi==y-1){
				printf("*");
			}
			else{
				printf("%c", map[xi+yi*x]);
			}
		}
		printf("\n");
	}*/
	fclose(fic);
	return map;
	
}
void print_map(char* map){
	int xi;
	int yi;
	for (xi=0 ; xi<x ;xi++){	
		for (yi=0 ; yi<y ; yi++) {
			if(xi==0||yi==0||xi==x-1||yi==y-1){
				printf("*");
			}
			else{
				printf("%c", map[xi+yi*x]);
			}
		}
		printf("\n");
	}
	return;
}

char *ajouter_bateau(char *map, char *position) {
	int i;
	char nord[4];
	int nordint;
	char est[4];
	int estint;
	char*map2=map;
	nord[0]= position[0];
	nord[1]= position[1];
	nord[2]= position[2];
	nordint=atoi(nord);
	est[0]= position[5];
	est[1]= position[6];
	est[2]= position[7];
	estint=atoi(est);
	//printf("%d\n",nordint); 
	//printf("%d",estint);
	map2[(x-nordint-1)+estint*x]='B';
	int xi;
	int yi;
	return map2;	
}

int est_place_libre(char position[10], int *listeRochers,int nombre_rochers ) {
	char nord[4];
	char est[4];
	int acceptation = 1;
	nord[0]= position[0];
	nord[1]= position[1];
	nord[2]= position[2];
	est[0]= position[5];
	est[1]= position[6];
	est[2]= position[7];
	int nord_nouveau_client = atoi(nord);
	int est_nouveau_client = atoi(est);

	int k1;
	int k2=0;
	
	// 0 : place non libre
	// 1 : place libre
	int tailleListe=0;
	//gestion des autres bateaux
	/*
	if (listeClients != VIDE) {
		for (k1=0, k1< tailleListe ; k1++) {
			if (nord_nouveau_client == listeClients[k1]->y_buf && est_nouveau_client == listeClients[k1]->y_buf) {
				acceptation = 0;
				return acceptation;
			}
		}	
	}*/
	//printf("%d",nombre_rochers);

	//gestion des rochers
	while (k2<=nombre_rochers*2-2) {
		if (nord_nouveau_client == listeRochers[k2] && est_nouveau_client == listeRochers[k2+1]) {
			acceptation = 0;
			printf("Il y a un rocher ici\n");
			return acceptation;
		}		
		k2+=2;
	}

	//gestions des bords
	if (nord_nouveau_client == 0 || nord_nouveau_client == x || est_nouveau_client == 0 || est_nouveau_client == y) 
	{
		acceptation = 0;
		printf("Il y a un bord ici \n");
		return acceptation;
	}
	return acceptation;
}

int est_place_libre2(char position[10], int *listeRochers,int nombre_rochers ) {
	int acceptation = 1;
	char nord[4];
	char est[4];
	nord[0]= position[0];
	nord[1]= position[1];
	nord[2]= position[2];
	est[0]= position[5];
	est[1]= position[6];
	est[2]= position[7];
	int nord_nouveau_client = atoi(nord);
	int est_nouveau_client = atoi(est);


	int k1;
	int k2=0;
	
	// 0 : place non libre
	// 1 : place libre
	int tailleListe=0;
	//gestion des autres bateaux
	/*
	if (listeClients != VIDE) {
		for (k1=0, k1< tailleListe ; k1++) {
			if (nord_nouveau_client == listeClients[k1]->y_buf && est_nouveau_client == listeClients[k1]->y_buf) {
				acceptation = 0;
				return acceptation;
			}
		}	
	}*/
	//printf("%d",nombre_rochers);

	//gestion des rochers
	while (k2<=nombre_rochers*2-2) {
		if (nord_nouveau_client == listeRochers[k2] && est_nouveau_client == listeRochers[k2+1]) {
			acceptation = 0;
			printf("Il y a un rocher ici\n");
			return acceptation;
		}		
		k2+=2;
	}

	//gestions des bords
	if (nord_nouveau_client == 0 || nord_nouveau_client == x || est_nouveau_client == 0 || est_nouveau_client == y) 
	{
		acceptation = 0;
		printf("Il y a un bord ici \n");
		return acceptation;
	}
	return acceptation;
}




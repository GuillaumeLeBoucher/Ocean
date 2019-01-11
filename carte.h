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

char *init_map(int liste_rochers[100],int nombre_rochers, int x, int y);
char* ajouter_bateau(char *map, char *position, int x, int y);
int est_place_libre(char position[10], int *listeRochers,int nombre_rochers, int x, int y);
void print_map(char* map, int x, int y);
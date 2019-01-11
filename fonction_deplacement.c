//pour .h
void deplacement(Clienlist *Clientcourant,int H, int L);

// ! Attention, ne jamais incrémenter les variables à l'intérieur de max !
#define max(x, y) (((x) > (y)) ? (x) : (y))

//variables globales utilisées dirvent, forvent et le coefficient pour le temps kT, j'ai mis les dimensions de la carte en paramètres, mais cela peut être des var globales (H limite en y et L en x)

void deplacement(Clientlist *Clientcourant,int H, int L)
{
	//px!=&x en théorie
	int pv=Clientcourant->vitesse;
	int px=Clientcourant->x;
	int py=Clientcourant->y;
	//Partie concernant le changement de vitesse du bateau
	switch(char dir=Clientcourant->direction[0]){
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

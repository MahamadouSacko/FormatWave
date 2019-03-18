#include "wave.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <stdint.h>




wave_t wave_new(uint32_t f, uint16_t p, uint16_t c, uint32_t B){
	wave_t w = malloc (sizeof(struct WAVE));
	strncpy(w->RIFF,"RIFF",4);
	strncpy(w->WAVE,"WAVE",4);
	strncpy(w->fmt,"fmt ",4);
	strncpy(w->data,"data",4);
	w->nombre = 16;
	w->formatAudio = 1;
	w->c = c;
	w->f = f;
	w->p = p;
	
	w->b = (c*p)/8;
	w->r = f*(w->b);
	w->D = B*(w->b);
	w->S = w->D + 44 - 8;
	// on alloue la taille des données (D). Les donées sont de type int8_t
	w->donees = (int8_t*)malloc(w->D);
	// remplissage des cases mémoire de 0 (sur 8 bits)
	uint32_t i;
	for(i = 0; i < w->D; i++){
		*(w->donees +i) = 0;
	}
	return w;
}

void wave_delete(wave_t w){
	free(w->donees);
	free(w);
}

int64_t wave_get(wave_t w, uint32_t i, uint16_t j){
	int l; //compteur
	// récupération des donées au bloc i, canal j
	int8_t* p = (w->donees + (i * w->b) + (j*(w->p/8)));
	int64_t ampli = 0;
	//affectation des 8 bits(du int8_t) dans 64 bits (int64_t)
	ampli = p[(w->p/8)-1];
	//boucle qui divise les 64 bits en blocs de taille 6 ou 16 ou 24 (l=nombre maximum de blocs -- jusqu à 0)
	for(l = (w->p/8)-2; l >= 0; l--){ // le décalage se fait en foction du nombre d'octets prélevés donc en fonction de la précision p
		// décalage de 1 octet des bits prélevés vers la droite
		ampli *= 256;
		ampli += (uint8_t)p[l];
	}

	return ampli;
}

void wave_set(wave_t w, uint32_t i, uint16_t j, int64_t a){
	int l; //compteur
    if(w->p == 8){
    	int8_t bits8 = a;
    	int8_t* p = (w->donees + (i * w->b) + (j*(w->p/8)));
    	*p = bits8;
    }else if(w->p == 16){
    	//conversion de l'amplitude de 64 bits à 16 bits
    	int16_t bits16 = a;
    	//pointeur sur le canal demandé
    	int8_t* p = (w->donees + (i * w->b) + (j*(w->p/8)));
    	//conversion du nombre en little-endian
	   	int8_t m[2];
		*((int16_t*) m) = bits16;
	    int8_t* pointeurSurTab = m;
        int16_t ampli = 0;
            ampli = pointeurSurTab[1];
            for(l = 0; l >= 0; l--){
                ampli *= 256;
                ampli += (uint8_t)pointeurSurTab[l];
            }
        //entrée de la valeur de l'amplitude dans le canal
    	*(int16_t*)p = ampli;
    }else if(w->p == 24){
    	//conversion de l'amplitude de 64 bits à 24(32) bits
    	int32_t bits24 = a;
    	// test: printf("%X\n",bits24);
    	//conversion du nombre en little-endian
	   	int8_t m[3];
		*((int32_t*) m) = bits24;
  		int8_t* pointeurSurTab = m;
        int32_t ampli = 0;
            ampli = pointeurSurTab[2];
            for(l = 1; l >= 0; l--){
                ampli *= 256;
                ampli += (uint8_t)pointeurSurTab[l];
            }
           // test: printf("%X\n",ampli);
        //entrée de la valeur de l'amplitude dans le canal
		*((int32_t*) m) = ampli;
		//pointeur sur le canal demandé
      	int8_t* p = (w->donees + (i * w->b) + (j*(w->p/8)));
		*(p+0) = *(m+0);
		*(p+1) = *(m+1);
		*(p+2) = *(m+2);
		/*test: int8_t* pointeur = (w->donees + (i * w->b) + (j*(w->p/8)));
		printf("%p\n",(int8_t*)p);
		printf("%p\n",(int8_t*)pointeur);*/

    }

}

//fonction qui rajoute signal sur wave, s = pointeur sur fonction qui prend 
//un double et renvoie un double
/*void add_signal(wave_t w, double (*s)(double)){

}*/

wave_t wave_load(const char* fname){
	//création de pointeur sur le fichier
	FILE* fEntree = fopen(fname, "r");
    if(fEntree == NULL){
        printf("Le nom que vous avez saisi n'est pas valide!\nAucun fichier ne sera ouvert!\n");
        return NULL;
    }
	//création de structure wave_t ou l'on charge le fichier
	wave_t fichierCharge = (wave_t)malloc(sizeof(struct WAVE));
	//copie de l'entête
	fread(fichierCharge, sizeof(struct WAVE),1,fEntree);
	//copie des données
	fichierCharge->donees = malloc(fichierCharge->D);
	fread(fichierCharge->donees, sizeof(uint8_t),fichierCharge->D,fEntree);
	fclose(fEntree);

	if (strncmp(&(fichierCharge->RIFF[0]), "RIFF", 4) != 0){	
    	//fprintf(stderr, "Ce n'est pas un fichier wav\n");
		wave_delete(fichierCharge);
	   	return NULL;
	}else if (strncmp(&(fichierCharge->WAVE[0]), "WAVE", 4) != 0){
		//fprintf(stderr, "Ce n'est pas un fichier wav\n");
		wave_delete(fichierCharge);
	   	return NULL;
	}else if (strncmp(&(fichierCharge->fmt[0]), "fmt ", 4) != 0){
		//fprintf(stderr, "Ce n'est pas un fichier wav\n");
		wave_delete(fichierCharge);
	   	return NULL;
	}
	return fichierCharge;
}


bool wave_save(const char* fname, wave_t w){
	FILE* sauvegarde = fopen(fname,"w");
	bool succes = 1;
	if(fwrite(w,sizeof(struct WAVE),1,sauvegarde) != 0){
		if(fwrite(w->donees,sizeof(uint8_t),w->D,sauvegarde) != 0){
			succes = 0;
		}
	}
	fclose(sauvegarde);
	wave_delete(w);
	
	return succes;
}

void wave_canal(wave_t w, uint16_t c){
	if(c != w->c){
 	//création d'un tableau de données de taille adapté au nouveau nombre de canaux
 		int8_t* donees = NULL;
 		if(w->p < 24){
 			donees = malloc(c*((w->p)/8)*(w->D/w->b));
 		}else if(w->p == 24){
 			donees = malloc(c*4*(w->D/w->b));
 		}
 		unsigned int i = 0;
 		int j = 0;
 		int k;
 		int l;
 		// on remplit le nouveau tableau de 0
 		for(i = 0; i < c * ((w->p)/8) * (w->D/w->b); i++){
			*(donees + i) = 0;
		}

		i = 0;
		j = 0;
	// si le nouveau nombre de canaux est inférieur au nb canaux ancien
if(c < w->c){
 	if(c == 1){
 		if(w->p == 8){
 			// tant qu'on a pas parcouru tout le tableau de données source
 			while(i < w->c * (w->p/8) * (w->D/w->b)){
 				for(k = 0; k < w->c; k++){
 					*(donees + j) = *(donees + j) + *(w->donees + i + k);
 				}
 				donees[j] = (donees[j])/w->c;
 				i = i + w->c;
 				j = j + 1;
 			}
 		}else if(w->p == 16){
 	 		while(i < w->c * (w->p/8) * (w->D/w->b)){
 				for(k = 0; k < w->c; k++){
 					*(int16_t*)(donees + j) = *(int16_t*)(donees + j) + *(int16_t*)(w->donees + i + 2*k);
 				}
 				*(int16_t*)(donees + j) = *(int16_t*)(donees + j)/(w->c);
 				i = i + 2*(w->c);
 				j = j + 2;
 				
 			}
 		}else if(w->p == 24){
 		while(i < w->c * (w->p/8) * (w->D/w->b)){
 				for(k=0; k < w->c; k++){
 					*(int32_t*)(donees+j) = *(int32_t*)(donees + j) + *(int32_t*)(w->donees + i + 4*k);
 				}
 				*(int32_t*)(donees + j) = *(int32_t*)(donees + j)/w->c;
 				i = i + 4*(w->c);
 				j = j+4;
 			}
 		}
 	}else if(c == 2){
 		if(w->p == 8){
 			while(i < w->c * (w->p/8) * (w->D/w->b)){
 				*(donees + j) = *(w->donees + i);
 				for(k = 1; k < w->c; k++){
 					*(donees + j + 1) = *(donees + j + 1)  + *(w->donees + i + k);
 				}
 				*(donees + j + 1) = *(donees + j + 1)/(w->c - 1);
 				i = i + w->c;
 				j = j+2;
 			}
 		}else if(w->p == 16){
 			while(i < w->c * (w->p/8) * (w->D/w->b)){
 				*(int16_t*)(donees+j) = *(int16_t*)(w->donees + i);
 				for(k = 1; k < w->c; k++){
 					*(donees + j + 2) = *(int16_t*)(donees + j + 2)  + *(int16_t*)(w->donees + i + 2*k);
 				}
 				*(int16_t*)(donees + j + 2) = *(int16_t*)(donees + j + 2)/(w->c - 1);
 				i = i + 2*(w->c);
 				j = j+4;
 			}
 		}else if(w->p == 24){
 			while(i < w->c * (w->p/8) * (w->D/w->b)){
 				*(int32_t*)(donees+j) = *(int32_t*)(w->donees + i);
 				for(k = 1; k < w->c; k++){
 					*(int32_t*)(donees + j + 4) = *(int32_t*)(donees + j + 4)  + *(int32_t*)(w->donees + i + 4*k);
 				}
 				*(int32_t*)(donees + j + 4) = *(int32_t*)(donees + j + 4)/(w->c - 1);
 				i = i + 4*(w->c);
 				j = j+8;
 			}
 		}

 	}else if(c == 3){
 		if(w->p == 8){
 			while(i < w->c * (w->p/8) * (w->D/w->b)){
 				*(donees + j) = *(w->donees + i);
 				*(donees + j + 1) = *(w->donees + i + 1);
 				for(k = 2; k < w->c; k++){
 					*(donees + j + 2) = *(donees + j + 2)  + *(w->donees + i + k);
 				}
 				*(donees + j + 2) = *(donees + j + 2)/(w->c - 2);
 				i = i + w->c;
 				j = j+3;
 			}
 		}else if(w->p == 16){
 			while(i < w->c * (w->p/8) * (w->D/w->b)){
 				*(int16_t*)(donees+j) = *(int16_t*)(w->donees + i);
 				*(int16_t*)(donees + j + 2) = *(int16_t*)(w->donees + i + 2);
 				for(k = 2; k < w->c; k++){
 					*(int16_t*)(donees + j + 4) = *(int16_t*)(donees + j + 4)  + *(int16_t*)(w->donees + i + 2*k);
 				}
 				*(int16_t*)(donees + j + 4) = *(int16_t*)(donees + j + 4)/(w->c - 2);
 				i = i + 2*(w->c);
 				j = j+6;
 			}
 		}else if(w->p == 24){
 			while(i < w->c * (w->p/8) * (w->D/w->b)){
 				*(int32_t*)(donees+j) = *(int32_t*)(w->donees + i);
 				*(int32_t*)(donees + j + 4) = *(int32_t*)(w->donees + i + 4);
 				for(k = 2; k < w->c; k++){
 					*(int32_t*)(donees + j + 8) = *(int32_t*)(donees + j + 8)  + *(int32_t*)(w->donees + i + 4*k);
 				}
 				*(int32_t*)(donees + j + 8) = *(int32_t*)(donees + j + 8)/(w->c - 2);
 				i = i + 4*(w->c);
 				j = j+12;
 			}
 		}

 	}else if(c == 4){
 		if(w->p == 8){
 			while(i < w->c * (w->p/8) * (w->D/w->b)){
 				*(donees + j) = *(w->donees + i);
 				*(donees + j + 1) = *(w->donees + i + 1);
 				*(donees + j + 2) = *(w->donees + i + 2);
 				for(k = 3; k < w->c; k++){
 					*(donees + j + 3) = *(donees + j + 3)  + *(w->donees + i + k);
 				}
 				*(donees + j + 2) = *(donees + j + 2)/(w->c - 3);
 				i = i + w->c;
 				j = j+4;
 			}
 		}else if(w->p == 16){
 			while(i < w->c * (w->p/8) * (w->D/w->b)){
 				*(int16_t*)(donees+j) = *(int16_t*)(w->donees + i);
 				*(int16_t*)(donees + j + 2) = *(int16_t*)(w->donees + i + 2);
 				*(int16_t*)(donees + j + 4) = *(int16_t*)(w->donees + i + 4);
 				for(k = 3; k < w->c; k++){
 					*(int16_t*)(donees + j + 6) = *(int16_t*)(donees + j + 6)  + *(int16_t*)(w->donees + i + 2*k);
 				}
 				*(int16_t*)(donees + j + 6) = *(int16_t*)(donees + j + 6)/(w->c - 3);
 				i = i + 2*(w->c);
 				j = j+8;
 			}
 		}else if(w->p == 24){
 			while(i < w->c * (w->p/8) * (w->D/w->b)){
 				*(int32_t*)(donees+j) = *(int32_t*)(w->donees + i);
 				*(int32_t*)(donees + j + 4) = *(int32_t*)(w->donees + i + 4);
 				*(int32_t*)(donees + j + 8) = *(int32_t*)(w->donees + i + 8);
 				for(k = 3; k < w->c; k++){
 					*(int32_t*)(donees + j + 12) = *(int32_t*)(donees + j + 12)  + *(int32_t*)(w->donees + i + 4*k);
 				}
 				*(int32_t*)(donees + j + 12) = *(int32_t*)(donees + j + 12)/(w->c - 3);
 				i = i + 4*(w->c);
 				j = j+16;
 			}
 		}

 	}else if(c == 5){
 		if(w->p == 8){
 			while(i < w->c * (w->p/8) * (w->D/w->b)){
 				*(donees + j) = *(w->donees + i);
 				*(donees + j + 1) = *(w->donees + i + 1);
 				*(donees + j + 2) = *(w->donees + i + 2);
 				*(donees + j + 3) = *(w->donees + i + 3);
 				for(k = 4; k < w->c; k++){
 					*(donees + j + 4) = *(donees + j + 4)  + *(w->donees + i + k);
 				}
 				*(donees + j + 4) = *(donees + j + 4)/(w->c - 4);
 				i = i + w->c;
 				j = j+5;
 			}
 		}else if(w->p == 16){
 			while(i < w->c * (w->p/8) * (w->D/w->b)){
 				*(int16_t*)(donees+j) = *(int16_t*)(w->donees + i);
 				*(int16_t*)(donees + j + 2) = *(int16_t*)(w->donees + i + 2);
 				*(int16_t*)(donees + j + 4) = *(int16_t*)(w->donees + i + 4);
 				*(int16_t*)(donees + j + 6) = *(int16_t*)(w->donees + i + 6);
 				for(k = 4; k < w->c; k++){
 					*(int16_t*)(donees + j + 8) = *(int16_t*)(donees + j + 8)  + *(int16_t*)(w->donees + i + 2*k);
 				}
 				*(int16_t*)(donees + j + 8) = *(int16_t*)(donees + j + 8)/(w->c - 4);
 				i = i + 2*(w->c);
 				j = j+10;
 			}
 		}else if(w->p == 24){
 			while(i < w->c * (w->p/8) * (w->D/w->b)){
 				*(int32_t*)(donees+j) = *(int32_t*)(w->donees + i);
 				*(int32_t*)(donees + j + 4) = *(int32_t*)(w->donees + i + 4);
 				*(int32_t*)(donees + j + 8) = *(int32_t*)(w->donees + i + 8);
 				*(int32_t*)(donees + j + 12) = *(int32_t*)(w->donees + i + 12);
 				for(k = 4; k < w->c; k++){
 					*(int32_t*)(donees + j + 16) = *(int32_t*)(donees + j + 16)  + *(int32_t*)(w->donees + i + 4*k);
 				}
 				*(int32_t*)(donees + j + 16) = *(int32_t*)(donees + j + 16)/(w->c - 4);
 				i = i + 4*(w->c);
 				j = j+20;
 			}
 		}

 	}
 }else if(c > w->c){
 	i = 0;
 	j = 0;
		if(w->p == 8){
 			while(i < w->c * (w->p/8) * (w->D/w->b)){
 				for(k = 0; k < w->c; k++){
 					*(donees + j + k) = *(w->donees + i + k);
 				}
 				for(l = k; l < c ; l++){
 					*(donees + j + l) = *(w->donees + i + (k-1));
 				}
 				i = i + w->c;
 				j = j + c;
 			}
 		}else if(w->p == 16){
 			while(i < w->c * (w->p/8) * (w->D/w->b)){
 				for(k = 0; k < w->c; k++){
 					*(int16_t*)(donees + j + 2*k) = *(int16_t*)(w->donees + i + 2*k);
 					//printf("%ld\n",*(int16_t*)(donees + j + 2*k));
 				}
 				for(l = k; l < c ; l++){
 					*(int16_t*)(donees + j + 2*l) = *(int16_t*)(w->donees + i + 2*(k-1));
 					//printf("%ld\n",*(int16_t*)(donees + j + 2*l));
 				}
 				i = i + 2*(w->c);
 				j = j + 2*c;
 						
 			}
 		}else if(w->p == 24){
 			while(i < w->c * (w->p/8) * (w->D/w->b)){
 				for(k = 0; k < w->c; k++){
 					*(int32_t*)(donees + j + 4*k) = *(int32_t*)(w->donees + i + 4*k);
 				}
 				for(l = k; l < c; l++){
 					*(int32_t*)(donees + j + 4*l) = *(int32_t*)(w->donees + i + 4*(k - 1));
 				}
 				i = i + 4*(w->c);
 				j = j + 4*c;
 			}
 		}

 }
 	uint32_t B = w->D/w->b;
 	w->c = c;
 	w->b = c *(w->p)/8;
 	w->r = w->f * w->b;
 	w->D = B * w->b;
 	w->S = w->D + 44 - 8;
 	free(w->donees);
 	w->donees = donees;

 }
}

 void wave_reverse(wave_t w){
 	int8_t* nouvellesDonnees = malloc(w->D);
 	int i = w->c * (w->D/(w->p/8));
 	unsigned int j = 0;

 	while(i > 0){
 		while(j < w->c * (w->D/(w->p/8))){
 			nouvellesDonnees[j] = w->donees[i];
 			j = j + 1;
 			i = i - 1;
 		}
 	}
 	free(w->donees);
 	w->donees = nouvellesDonnees;
 }

 wave_t wave_split(wave_t w, int* pc){
 	int i;
 	uint32_t j; 
 	int l;
 	wave_t tabWave = malloc(w->c * sizeof(struct WAVE));
 	//declaration de l'entête de chaque WAVE du tableau
 	for(i = 0; i < w->c; i++){
 		strncpy((tabWave + i)->RIFF,"RIFF",4);
		strncpy((tabWave + i)->WAVE,"WAVE",4);
		strncpy((tabWave + i)->fmt,"fmt ",4);
		strncpy((tabWave + i)->data,"data",4);
		(tabWave + i)->nombre = 16;
 		(tabWave + i)->formatAudio = 1;
 		(tabWave + i)->c = 1;
 		(tabWave + i)->f = w->f;
 		(tabWave + i)->p = w->p;
 		(tabWave + i)->b = ((tabWave + i)->c)*((tabWave + i)->p)/8; 
 		(tabWave + i)->r = (tabWave + i)->f * (tabWave + i)->b;
 		(tabWave + i)->D = w->D/w->c;
 		(tabWave + i)->S = (tabWave + i)->D + 44 - 8;
 		(tabWave + i)->donees = (int8_t*)malloc((tabWave + i)->D);
		for(j = 0; j < (tabWave + i)->D; j++){
			*((tabWave + i)->donees + j) = 0;
		}
}
	//si wave sur 8 bits
	if(w->p == 8){
		// on parcourt chaque wave
 		for(i = 0; i < w->c; i++){
 			l = 0;
 			//tant qu'on ne depasse pas la fin de w, on copie dans le nouveau wave un canal de w
			for(j = i; j < (w->D/(w->p/8)); j = j + w->c){
				*((tabWave + i)->donees + l) = *(w->donees + j);
				l = l + 1;
			}
		}
  	}else if(w->p == 16){
 		for(i = 0; i < w->c; i++){
 			l = 0;
			for(j = i; j < (w->D/(w->p/8)); j = j + (2* w->c)){
				*((int16_t*)(tabWave + i)->donees + l) = *((int16_t*)w->donees + j);
				l = l + 2;
			}
		}

 	}else if(w->p == 24){
 	 	for(i = 0; i < w->c; i++){
 	 		l = 0;
 			for(j = i; j < (w->D/(w->p/8)); j = j+ (4* w->c)){
				*((int32_t*)(tabWave + i)->donees + l)=  *((int32_t*)w->donees + j);
				l = l + 4;
			}
		}	
	}

	//pc = malloc(sizeof(int));
	*pc = w->c;
	free(w->donees);
 	free(w);
	return tabWave;


}

wave_t wave_merge(wave_t w, int c){
	unsigned int i = 0;
	uint32_t j = 0;
	int l = 0;
	int k;
	wave_t newWave = malloc(sizeof(struct WAVE));
		strncpy(newWave->RIFF,"RIFF",4);
		strncpy(newWave->WAVE,"WAVE",4);
		strncpy(newWave->fmt,"fmt ",4);
		strncpy(newWave->data,"data",4);
		newWave->nombre = 16;
 		newWave->formatAudio = 1;
 		newWave->c = c;
 		newWave->f = (w + 0)->f;
 		newWave->p = (w + 0)->p;
 		newWave->b = c * (w + 0)->p/8; 
 		newWave->r = newWave->f * newWave->b;
 		uint32_t D = 0;
 		for(k = 0; k < c; k++){
			D = D + (w + k)->D;
		}
 		newWave->D = D;
		newWave->S = newWave->D + 44 - 8;
 		newWave->donees = (int8_t*)malloc(newWave->D);
		for(j = 0; j < newWave->D; j++){
			*(newWave->donees + j) = 0;
		}
	k = 0;

	//si wave sur 8 bits
	if(w->p == 8){
		while(i < newWave->D/(newWave->p/8)){
			while(k < c){
				*(newWave->donees + i) = *((w + k)->donees + l);
				i = i + 1;
				k = k + 1;
			}
			l = l + 1;
			k = 0;
		}
	}else if(w->p == 16){
 		while(i < newWave->D){
			while(k < c){
				*((int16_t*)(newWave->donees + i)) = *((int16_t*)((w + k)->donees + l));
				i = i + 2;
				k = k + 1;
			}
			l = l + 2;
			k = 0;
		}
 	}else if(w->p == 24){
 		while(i < newWave->D/(newWave->p/8)){
			while(k < c){
				*((int32_t*)(newWave->donees + i)) =  *((int32_t*)((w + k)->donees + l));
				i = i + 4;
				k = k + 1;
			}
			l = l + 4;
			k = 0;
		}

 	}
	return newWave;
}
wave_t wave_crop(wave_t w, uint32_t start, uint32_t length){
	//start = nombre de blocs?

	uint i;
	uint j;
	uint pos;
	// définition du nouveau wave
	wave_t waveCrop = malloc(sizeof(struct WAVE));

	strncpy(waveCrop->RIFF,"RIFF",4);
	strncpy(waveCrop->WAVE,"WAVE",4);
	strncpy(waveCrop->fmt,"fmt ",4);
	strncpy(waveCrop->data,"data",4);
	waveCrop->nombre = 16;
	waveCrop->formatAudio = 1;
 	waveCrop->c = w->c;
 	waveCrop->f = w->f;
 	waveCrop->p = w->p;
 	waveCrop->b = w->b; 
 	waveCrop->r = w->r;
 	waveCrop->D = w->r * length;
	waveCrop->S = waveCrop->D + 44 - 8;



	waveCrop->donees = (int8_t*)malloc(waveCrop->D);
	for(j = 0; j < waveCrop->D; j++){
		*(waveCrop->donees + j) = 0;
	}

	
	pos = start * waveCrop->r;
	for(i = 0; i < waveCrop->D && pos < w->D; i++){
			*(waveCrop->donees + i) = *(w->donees + pos);
			pos = pos + 1;
	}
	if(i < length * w->b - 1){
		return NULL;
	}
	return waveCrop;
}

wave_t wave_scale(wave_t w, double s){
	wave_t x = malloc (sizeof(struct WAVE));
	strncpy(x->RIFF,"RIFF",4);
	strncpy(x->WAVE,"WAVE",4);
	strncpy(x->fmt,"fmt ",4);
	strncpy(x->data,"data",4);
	x->nombre = w->nombre;
	x->formatAudio = w->formatAudio;
	x->c = w->c;
	x->f = w->f * s;
	x->p = w->p;
	
	x->b = w->b;
	x->r = x->f * x->b;
	x->D = w->D;
	x->S = w->S;
	// on alloue la taille des données (D). Les donées sont de type int8_t
	x->donees = (int8_t*)malloc(x->D);
	uint32_t i;
	for(i = 0; i < x->D; i++){
		*(x->donees +i) =*(w->donees +i);
	}
	return x;
}



    


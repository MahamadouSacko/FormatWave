#include "menu.h"
#include "args.h"
#include "wave.h"

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stdbool.h>

wave_t newOpen = NULL;
char* fname = NULL;


void ouvrir() {
  	if(newOpen != NULL){
  		printf("Un fichier audio est déjà chargé!\nVeuillez saisir le nom de la sauvegarde:\n");
  		char* sauvegarde = malloc(50*sizeof(char));
  		scanf("%s", sauvegarde);
  		wave_save(sauvegarde, newOpen);
  		free(sauvegarde);
  	}
  	printf("Veuillez saisir le nom du fichier que vous souhaitez ouvrir:\n");
  	fname = malloc(50*sizeof(char));	
  	scanf("%s", fname);
  	printf("%s\n",fname);
    newOpen =  wave_load(fname);
}


void w_8bits_mono_11(){
	if(newOpen != NULL){
		wave_canal(newOpen, 1);
		uint32_t f = 11025;
		uint16_t p = 8;
		uint16_t c = 1;
		uint32_t B = newOpen->D/(c*(p/8));
		wave_t new = wave_new(f,p,c,B);
		free(new->donees);
		new->donees = newOpen->donees;

		int choix;
		printf("Voulez vous saisir le nom de la sauvegarde (saisissez 1 pour oui, 2 pour non)?\n");
		scanf("%d", &choix);
		if(choix == 1){
			printf("Veuillez saisir le nom de la sauvegarde: \n");
			char* sauvegarde = malloc(50*sizeof(char));
  			scanf("%s", sauvegarde);
  			bool testSauvegarde = wave_save(sauvegarde, new);
  			if(testSauvegarde == 0){
  				printf("Sauvegarde effectuée avec succés\n");
  			}
  			free(sauvegarde);
		}else if(choix == 2){
			bool testSauvegarde = wave_save(fname, new);
			if(testSauvegarde == 0){
  				printf("Sauvegarde effectuée avec succés\n");
  			}
		}
	}else if(newOpen == NULL){
		printf("Action impossible! Veuillez charger un fichier .wav\n");
	}

}

void w_16bits_stereo_44(){
	if(newOpen != NULL){
		wave_canal(newOpen, 2);
		uint32_t f = 44100;
		uint16_t p = 16;
		uint16_t c = 2;
		uint32_t B = newOpen->D/(c*(p/8));
		wave_t new = wave_new(f,p,c,B);
		free(new->donees);
		new->donees = newOpen->donees;

		int choix;
		printf("Voulez vous saisir le nom de la sauvegarde (saisissez 1 pour oui, 2 pour non)?\n");
		scanf("%d", &choix);
		if(choix == 1){
			printf("Veuillez saisir le nom de la sauvegarde:\n");
			char* sauvegarde = malloc(50*sizeof(char));
  			scanf("%s", sauvegarde);
  			bool testSauvegarde = wave_save(sauvegarde, new);
  			if(testSauvegarde == 0){
  				printf("Sauvegarde effectuée avec succés\n");
  			}
  			free(sauvegarde);
		}else if(choix == 2){
				bool testSauvegarde = wave_save(fname, new);
			if(testSauvegarde == 0){
  				printf("Sauvegarde effectuée avec succés\n");
  			}
		}
	}else if(newOpen == NULL){
		printf("Action impossible! Veuillez charger un fichier .wav\n");
	}
}

void w_24bits_51_192(){
	if(newOpen != NULL){
		wave_canal(newOpen, 6);
		uint32_t f = 192000;
		uint16_t p = 24;
		uint16_t c = 6;
		uint32_t B = newOpen->D/(c*(p/8));
		wave_t new = wave_new(f,p,c,B);
		free(new->donees);
		new->donees = newOpen->donees;

		int choix;
		printf("Voulez vous saisir le nom de la sauvegarde (saisissez 1 pour oui, 2 pour non)?\n");
		scanf("%d", &choix);
		if(choix == 1){
			printf("Veuillez saisir le nom de la sauvegarde:\n");
			char* sauvegarde = malloc(50*sizeof(char));
  			scanf("%s", sauvegarde);
  			bool testSauvegarde = wave_save(sauvegarde, new);
  			if(testSauvegarde == 0){
  				printf("Sauvegarde effectuée avec succés\n");
  			}
  			free(sauvegarde);
		}else if(choix == 2){
			bool testSauvegarde = wave_save(fname, new);
			if(testSauvegarde == 0){
  				printf("Sauvegarde effectuée avec succés\n");
  			}
		}
	}else if(newOpen == NULL){
		printf("Action impossible! Veuillez charger un fichier .wav\n");
	}
}

void customise(){
	if(newOpen != NULL){
		uint32_t f;
		uint16_t p;
		uint16_t c;
		printf("Quelle précision voulez-vous avoir par canal (8, 16 ou 24 bits) ?\n");
		scanf("%hd",&p);
		printf("Combien de canaux voulez-vous obtenir (entre 1 et 6) ?\n");
		scanf("%hd",&c);
		printf("Quelle frequence voulez-vous obtenir ?\n");
		scanf("%d",&f);
		wave_canal(newOpen, c);

		uint32_t B = newOpen->D/(c*(p/8));
		wave_t new = wave_new(f,p,c,B);
		free(new->donees);
		new->donees = newOpen->donees;

		int choix;
		printf("Voulez vous saisir le nom de la sauvegarde (saisissez 1 pour oui, 2 pour non)?\n");
		scanf("%d", &choix);
		if(choix == 1){
			printf("Veuillez saisir le nom de la sauvegarde:\n");
			char* sauvegarde = malloc(50*sizeof(char));
  			scanf("%s", sauvegarde);
  			bool testSauvegarde = wave_save(sauvegarde, new);
  			if(testSauvegarde == 0){
  				printf("Sauvegarde effectuée avec succés\n");
  			}
  			free(sauvegarde);
		}else if(choix == 2){
			bool testSauvegarde = wave_save(fname, new);
			if(testSauvegarde == 0){
  				printf("Sauvegarde effectuée avec succés\n");
  			}
		}
	}else if(newOpen == NULL){
		printf("Action impossible! Veuillez charger un fichier .wav\n");
	}
}



void info(){
	if(newOpen != NULL){
		int z;
		printf("\n");
		printf("Nom: %s\n", fname);
		printf("Type: ");
		for(z=0;z<4;z++){
			printf("%c",newOpen->WAVE[z]);
		}
		printf("\n");
		uint32_t duree = newOpen->D/newOpen->r;
		printf("Duree: ");
		uint32_t heures = duree/3600;
		printf("%2d:", heures);
		uint32_t minutes = (duree/60)-(heures*60);
		printf("%2d:", minutes);
		uint32_t secondes = duree - (minutes*60) - (heures*3600);
		printf("%2d\n", secondes);
	}else if(newOpen == NULL){
		printf("Action impossible! Veuillez charger un fichier .wav\n");
	}

}

void inverse(){
	if(newOpen != NULL){
		 wave_reverse(newOpen);
		 printf("Veuillez saisir le nom de la sauvegarde:\n");
		 char* sauvegarde = malloc(50*sizeof(char));
  		 scanf("%s", sauvegarde);
  		 bool testSauvegarde = wave_save(sauvegarde, newOpen);
  		 if(testSauvegarde == 0){
  			 printf("Sauvegarde effectuée avec succés\n");
  		 }
  		 free(sauvegarde);
	}else{
		printf("Action impossible! Veuillez charger un fichier .wav\n");
	}
}

void decoupe(){
	if(newOpen != NULL){
		uint32_t start = 0;
		uint32_t length = 0;
		printf("Saisissez la date de départ (en secondes): ");
		scanf("%d",&start);
		printf("Saisissez la durée du morceau (en secondes): ");
		scanf("%d",&length);

		 newOpen = wave_crop(newOpen,start,length);
		 printf("erreur\n");
		 printf("Veuillez saisir le nom de la sauvegarde:\n");
		 char* sauvegarde = malloc(50*sizeof(char));
  		 scanf("%s", sauvegarde);
  		 bool testSauvegarde = wave_save(sauvegarde, newOpen);
  		 if(testSauvegarde == 0){
  			 printf("Sauvegarde effectuée avec succés\n");
  		 }
  		 free(sauvegarde);
	}else{
		printf("Action impossible! Veuillez charger un fichier .wav\n");
	}

}

void redimensionner(){
	if(newOpen != NULL){
		double s = 0;
		printf("Saisissez la ratio selon laquelle vous voulez accélerer le morceau: ");
		scanf("%lf",&s);

		 newOpen = wave_scale(newOpen,s);
		 printf("Veuillez saisir le nom de la sauvegarde:\n");
		 char* sauvegarde = malloc(50*sizeof(char));
  		 scanf("%s", sauvegarde);
  		 bool testSauvegarde = wave_save(sauvegarde, newOpen);
  		 if(testSauvegarde == 0){
  			 printf("Sauvegarde effectuée avec succés\n");
  		 }
  		 free(sauvegarde);
	}else{
		printf("Action impossible! Veuillez charger un fichier .wav\n");
	}

}

void tabCanaux(){


}

 
bool cont = true;
 
void quit() {
  cont = false;
  if(newOpen != NULL){
  	printf("Veuillez saisir le nom de la sauvegarde:\n");
		char* sauvegarde = malloc(50*sizeof(char));
  		scanf("%s", sauvegarde);
  		bool testSauvegarde = wave_save(sauvegarde, newOpen);
  		if(testSauvegarde == 0){
  			printf("Sauvegarde effectuée avec succés\n");
  		}
  		free(sauvegarde);  
  	}
}


int main(){
  menu_t* m;
  menu_t* sm;
  menu_t* ssm;
 printf("bienvenur sur notre application\n");
 //création du menu principal
  m = createMenu("Menu Principal");

 // création du sous-menu fichier
  sm = createMenu("Fichier");
  addSubMenu(m,sm);
// ajout de fonction ouvrir à fichier
  addMenuAction(sm,"Ouvrir",ouvrir);
// ajout de sous menu enregistrer à fichier
  ssm = createMenu("Enregistrer");
  addSubMenu(sm,ssm);
  // ajout de fonctions de sauvegarde à enregistrer
  addMenuAction(ssm,"WAVE 8bits Mono 11.025kHz",w_8bits_mono_11);
  addMenuAction(ssm,"WAVE 16bits Stereo 44,1kHz",w_16bits_stereo_44);
  addMenuAction(ssm,"WAVE 24bits 5.1 192kHz",w_24bits_51_192);
  addMenuAction(ssm,"WAVE customisé",customise);

// ajout de sous menu informations à fichier
  addMenuAction(sm,"Informations",info);
// ajout de sous menu informations à fichier 
  addMenuAction(sm,"Quitter",quit);

  sm = createMenu("Durée et tempo");
  addSubMenu(m,sm);
  addMenuAction(sm,"Inverser",inverse);
  addMenuAction(sm,"Découper",decoupe);
  addMenuAction(sm,"Redimensionner",redimensionner);

  /*sm = createMenu("Canaux");
  addSubMenu(m,sm);
  addMenuAction(sm,"Créer tableau de wave mono",nombreCanaux);*/
 
  while(cont) launchMenu(m);
 
  deleteMenu(m);
 


	return EXIT_SUCCESS;
}
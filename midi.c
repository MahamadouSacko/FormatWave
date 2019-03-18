#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <stdint.h>


struct midi{
	char MThd[4];
	uint32_t s;
	uint16_t formatmidi;
	uint16_t nbpistes;
	uint16_t horloge;
	char MTrk[4];
	uint32_t sizebloc;
	int8_t* donees;
	uint32_t finbloc;
};


typedef struct midi* midi_t;


struct evenement{
	uint8_t message;
	uint16_t donees;
};

typedef struct evenement* even_t;


midi_t midi_new(uint32_t nbpistes , uint16_t horloge ){
	midi_t m = malloc (sizeof(struct midi));
	strncpy(m->MThd,"MThd",4);
	strncpy(m->MTrk,"MTrk",4);
	m->s = 6;
	m->formatmidi =0;
	m->nbpistes = nbpistes;
	m->horloge = horloge;
	m->sizebloc = 14-8;//cas midi une seul piste
	m->donees = malloc(sizeof(struct evenement));
	//m->donees->message=message;
	//m->donees->donees=donees;
	// remplissage des cases mémoire de 0 (sur 8 bits)
	/*uint32_t i;
	for(i = 0; i < w->D; i++){
		*(w->donees +i) = 0;
	}*/
	return m;
}

void midi_delete(midi_t w){
	free(w->donees);
	free(w);
	w = NULL;
}


int main()
{
	
	return EXIT_SUCCESS;;
}
#ifndef WAVE_H
#define WAVE_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <stdint.h>


struct WAVE{
	char RIFF[4];
	uint32_t S;
	char WAVE[4];
	char fmt[4];
	uint32_t nombre;
	uint16_t formatAudio;
	uint16_t c;
	uint32_t f;
	uint32_t r;
	uint16_t b;
	uint16_t p;
	char data[4];
	uint32_t D;
	int8_t* donees;
};


typedef struct WAVE* wave_t;




wave_t wave_new(uint32_t f, uint16_t p, uint16_t c, uint32_t B);
void wave_delete(wave_t w);
int64_t wave_get(wave_t w, uint32_t i, uint16_t j);
void wave_set(wave_t w, uint32_t i, uint16_t j, int64_t a);
void add_signal(wave_t w, double (*s)(double));
wave_t wave_load(const char* fname);
bool wave_save(const char* fname, wave_t w);
void wave_canal(wave_t w, uint16_t c);
void wave_reverse(wave_t w);
wave_t wave_split(wave_t w, int* pc);
wave_t wave_merge(wave_t w, int c);
wave_t wave_crop(wave_t w, uint32_t start, uint32_t length);
wave_t wave_scale(wave_t w, double s);


#endif //WAVE_H
    


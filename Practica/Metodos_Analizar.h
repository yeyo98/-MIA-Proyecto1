#ifndef METODOS_ANALIZAR_H
#define METODOS_ANALIZAR_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <stdbool.h>

#include "Estructuras.h"
#include "Metodos.h"

void Analizar_Path(char* cadena, int* tamanio);
void Analizar_Size(char* cadena, int* tamanio);
void Analizar_Fit(char* cadena, int* tamanio);
void Analizar_Unit(char* comando, int* tamanio);
void Analizar_Unit_F(char* comando, int* tamanio);

void Analizar_Type(char* comando, int* tamanio);
void Analizar_Delete(char* comando, int* tamanio);
void Analizar_Add(char* comando, int* tamanio);
void Analizar_Name(char* comando, int* tamanio);

void Analizar_Id(char* comando, int* tamanio);
void Analizar_Fs(char* comando, int* tamanio);

void Analizar_Usr(char* comando, int* tamanio);
void Analizar_Pwd(char* comando, int* tamanio);
void Analizar_Grp(char* comando, int* tamanio);

void Analizar_P(char* comando, int *tamanio);
void Analizar_Cont(char* comando, int *tamanio);


#endif

#ifndef RMDISK_H
#define RMDISK_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <stdbool.h>

#include "Metodos.h"
#include "Estructuras.h"
#include "Metodos_Analizar.h"

void Analizar_Rmdisk(char* comando, int tamanio);


void Validar_Rmdisk();
void Borrar_Disco();
void Imprimir_Rmdisk();


#endif

#ifndef REP_H
#define REP_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <stdbool.h>

#include "Estructuras.h"
#include "Lista_Mount.h"
#include "Mkdisk.h"
#include "Estructuras.h"
#include "Metodos.h"
#include "Metodos_Analizar.h"
#include "Metodos_Bloques.h"

void Analizar_Rep(char* comando, int tamanio);

void Validar_Rep();
void Hacer_Reporte();

void Crear_Pdf(char* ruta);
void Crear_Grafo(char* ruta);

void Crear_Bm_Inode(char* ruta);
void Crear_Bm_Block(char* ruta);
void Graficar_SuperBlock(char* ruta);


#endif

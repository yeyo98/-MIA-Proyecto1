#ifndef MKFS_H
#define MKFS_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <stdbool.h>
#include <math.h>

#include "Estructuras.h"
#include "Metodos.h"
#include "Metodos_Analizar.h"
#include "Lista_Mount.h"
#include "Mkdisk.h"
#include "Metodos_Bloques.h"

void Analizar_Mkfs(char* comando, int tamanio);
void Validar_Mkfs();

void Formatear_Particion();
void Formateo_Fast(char* ruta);
void Formateo_Full(char* ruta);

void LLenar_SBlock(SuperBlock* sblock, int n, int inicio);

void Crear_Raiz(char* ruta, SuperBlock sblock, Nodo* datos_particion);


void Estado_Mkfs();


#endif

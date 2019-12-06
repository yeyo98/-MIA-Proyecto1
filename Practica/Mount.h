#ifndef MOUNT_H
#define MOUNT_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <stdbool.h>

#include "Metodos_Analizar.h"
#include "Metodos.h"
#include "Estructuras.h"
#include "Mkdisk.h"
#include "Fdisk.h"
#include "Lista_Mount.h"
#include "Bitacora_Mount.h"



int controlLetra ;

void Analizar_Mount(char* comando, int tamanio);

void Validar_Mount();
void Montar_Disco();

void Estado_Mount();
void LLenar_Arreglo();



#endif

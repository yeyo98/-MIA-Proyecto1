#ifndef UNMOUNT_H
#define UNMOUNT_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <stdbool.h>

#include "Mkdisk.h"
#include "Estructuras.h"
#include "Metodos.h"
#include "Metodos_Analizar.h"
#include "Fdisk.h"
#include "Lista_Mount.h"
#include "Bitacora_Mount.h"

void Analizar_Unmount(char* comando, int tamanio);

void Validar_Unmount();

void Quitar_Disco();



#endif

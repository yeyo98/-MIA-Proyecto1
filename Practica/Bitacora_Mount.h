#ifndef BITACORA_MOUNT_H
#define BITACORA_MOUNT_H

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

#include "Mount.h"

typedef struct Bnodo{
    char path[2000];
    int cont;
    char letra;
    //int cont;
    struct Bnodo* siguiente;
    struct BNodo* anterior;
}Bnodo;

void Insertar_Bitacora(char* p, char l, int c);
void Actualizar_Bitacora(char* path);
//void Eliminar_Bitacora(char* id );
//void Ordenar_Disco(void);
//void Graficar_Disco(void);
bool Existe_Bitacora(char* path);
Bnodo* Get_Bitacora(char* path);//SI ES LA MISMA RUTA, ES EL MISMO DISCO


#endif

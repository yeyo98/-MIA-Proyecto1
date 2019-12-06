#ifndef LISTA_MOUNT_H
#define LISTA_MOUNT_H

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

#include "Mount.h"
#include "Bitacora_Mount.h"
#include "Metodos_Analizar.h"
#include "Metodos.h"
#include "Estructuras.h"


typedef struct Nodo{
    char nada;
    int num;
    int inicio;
    char path[2000];
    struct Nodo* siguiente;
    char name[1000];
    char letra;
    struct Nodo* anterior;
    int size;
    char type;
}Nodo;

char letra[26];

void Insertar_Disco(Nodo* nuevo);
//void EditarLD(void);
void Desmontar_Disco(char* id );
//void Ordenar_Disco(void);
//void Graficar_Disco(void);
bool Existe_Mount(char* path, char* id);
bool Tengo_Mount(char l, int num);
Nodo* Get_Mount(char* path);//SI ES LA MISMA RUTA, ES EL MISMO DISCO
char* Get_Path(char l, int num);
int Get_Inicio(char l, int num);

void Contador_Mount_Unmount(Nodo* nuevo,int opt);
//void Contador_Unmount(Nodo* nuevo);

void Graficar_Mount(void);

#endif

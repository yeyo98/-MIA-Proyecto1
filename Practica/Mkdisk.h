#ifndef MKDISK_H
#define MKDISK_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <stdbool.h>
#include <time.h>
#include <dirent.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>

#include "Estructuras.h"
#include "Metodos_Analizar.h"
#include "Metodos.h"

typedef struct Particion{
    char part_status; //INDICA SI LA PARTICION ESTA ACTIVA O NO
    char part_type; //INDICA EL TIPO DE PARTICON P Ó E
    char part_fit; //TIPO DE AJUSTE B O F O W
    int part_start; //INDICA EN QUE BYTE DEL DISCO INICIA LA PARTICION
    int part_size; //CONTIENE EL TAMAÑO TOTAL DE LA PARTICION
    char part_name[16]; //NOMBRE DE LA PARTICION

}Particion;

typedef struct MBR{
    int mbr_tamanio; //TAMANIO TOTAL DEL DISCO EN BYTES
    char mbr_fecha_creacion[128]; //FECHA Y HORA DE CREACION DEL DISCO
    int mbr_disk_signature; //NUMERO RANDOM, QUE IDENTIFICA DE FORMA UNICA A CADA DISCO
    char disk_fit; //TIPO DE AJUSTE DE LA PARTICION

    Particion mbr_particion[4];     //ESTRUCTURA DE LAS PARTICIONES

}MBR;


void Analizar_Mkdisk(char* comando, int tamanio);
void Validar_Mkdisk(void);

void Crear_Disco(bool raid);

void Limpiar_Variables(void);
void Imprimir_Estado(void);



#endif

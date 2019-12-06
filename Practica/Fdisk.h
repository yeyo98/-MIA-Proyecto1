#ifndef FDISK_H
#define FDISK_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <stdbool.h>

#include "Metodos.h"
#include "Metodos_Analizar.h"
#include "Estructuras.h"
#include "Mkdisk.h"

typedef struct EBR{
    char part_status; //INDICA SI LA PARTICION ESTA ACTIVA O NO
    char part_fit;  //TIPO DE PARTICION B, F, W
    int part_start; //INDICA EN QUE BYTE COMIENZA
    int part_size; //CONTIENE EL TAMAÑO TOTAL DE LA PARTICION EN BYTES
    int part_next; //BYTE PARA EL PROXIMO EBR
    char part_name[16]; //NOMBRE DE LA PARTICION
}EBR;


void Analizar_Fdisk(char* comando, int tamanio);
void Estado_Fdisk();

void Validar_Fdisk();
void Crear_Particion(bool raid);
void Eliminar_Particion( );
void Modificar_Particion( );

int Calcular_Espacio();
int Calcular_Espacio_Add();
Particion LLenar_Particion(int inicio);
bool Nombre_noRepetido(MBR mbr, FILE* disco);
void Particion_Add(bool raid);
void Particion_Delete(bool raid);
Particion Vaciar_Particion();


bool Existe_Extendida(MBR mbr);
EBR LLenar_EBR(int inicio);

void Estado_Fdisk();






#endif

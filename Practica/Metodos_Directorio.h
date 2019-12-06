#ifndef METODOS_DIRECTORIO_H
#define METODOS_DIRECTORIO_H

#include "Metodos.h"
#include "Metodos_Bloques.h"
#include "Metodos_Analizar.h"
#include "Metodos_Archivo.h"
#include "Estructuras.h"

int Existe_Directorio(FILE* disco,SuperBlock sblock,int size, char* ruta);

void Crear_Directorio( FILE* disco, SuperBlock sblock, int inicio,int size ,char* ruta);
void Enlazar_Inodo_Directorio(FILE *disco, SuperBlock sblock, int inicio, int size,char* ruta,int pos);
int Crear_Inodo_Directorio(FILE* disco, SuperBlock sblock, int inicio, int size);
int Crear_BCarpeta_Directorio(FILE* disco, SuperBlock sblock, int inicio, int size,int anterior);
void Eliminar_Directorio( FILE* disco, SuperBlock sblock, int inicio,int size ,char* ruta);


#endif

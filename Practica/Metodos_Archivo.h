#ifndef METODOS_ARCHIVO_H
#define METODOS_ARCHIVO_H

#include "Metodos.h"
#include "Metodos_Bloques.h"
#include "Metodos_Analizar.h"
#include "Metodos_Directorio.h"
#include "Estructuras.h"

int Existe_Archivo(FILE* disco,SuperBlock sblock, int size,char* ruta);

int Buscar_Inodo(FILE*disco,SuperBlock sblock, int size, char* ruta,int pos,int cont);
int Buscar_BCarpeta(FILE *disco, SuperBlock sblock, int size, char *ruta,int pos,int cont);
int Buscar_BApuntador(FILE *disco, SuperBlock sblock, int size, char *ruta,int pos,int cont);

void Crear_Archivo(FILE *disco, SuperBlock sblock, int inicio, int size, char *ruta);
void Enlazar_Inodo_Archivo(FILE *disco, SuperBlock sblock, int inicio, int size, char *ruta, int pos);
bool Enlazar_BCarpeta_Archivo(FILE *disco, SuperBlock sblock, char *ruta,int pos,int pos_i);
bool Enlazar_BApuntador_Archivo(FILE *disco, SuperBlock sblock,int size, int inicio, char *ruta,int pos,int pos_a);

int Crear_Inodo_Archivo(FILE* disco, SuperBlock sblock, int inicio, int size);
int Crear_BCarpeta(FILE* disco, SuperBlock sblock, int inicio, int size);
int Crear_BApuntador(FILE* disco, SuperBlock sblock, int inicio, int size, int nivel);


void Eliminar_Archivo( FILE* disco, SuperBlock sblock, int inicio,int size ,char* ruta);
void Eliminar_Inodo_Archivo(FILE *disco, SuperBlock sblock, int inicio, int size,char* ruta,int pos);
void Eliminar_BCarpeta_Archivo(FILE *disco, SuperBlock sblock,int inicio, int size,char *ruta,int pos);
void Eliminar_BApuntador_Archivo(FILE *disco, SuperBlock sblock,int size, int inicio, char *ruta,int pos);
void Aux_Eliminar_Inodo(FILE *disco, SuperBlock sblock, int inicio, int size,char* ruta,int pos);
bool Aux_Eliminar_BCarpeta(FILE *disco, SuperBlock sblock,int inicio, int size,char *ruta,int pos);
bool Aux_Eliminar_BApuntador(FILE *disco, SuperBlock sblock,int size, int inicio, char *ruta,int pos);



void Escribir_Archivo(FILE* disco, SuperBlock sblock, int inicio, int size , char* ruta, char *texto);
void Escribir_Inodo_Archivo(FILE* disco, SuperBlock sblock, int inicio, int size , char *texto, int posI);
bool Escribir_BArchivo(FILE* disco, SuperBlock sblock,char* texto, int posA, int* index);
int Crear_BArchivo(FILE* disco, SuperBlock sblock, int inicio, int size);
bool Escribir_BApuntador_Archivo(FILE *disco, SuperBlock sblock,int size, int inicio, char *texto,int pos,int* index);


void Leer_Archivo(FILE* disco, SuperBlock sblock, int size , char *contenido, char* ruta);
char* Leer_Inodo_Archivo(FILE* disco, SuperBlock sblock, int size , int posI, char *contenido);
bool Leer_BArchivo(FILE* disco, SuperBlock sblock,char* contenido, int posA, int* index);
bool Leer_BApuntador_Archivo(FILE *disco, SuperBlock sblock,int size, char *contenido,int pos,int* index);


void Editar_Archivo(FILE* disco, SuperBlock sblock, int inicio, int size , char* ruta, char *texto);
void Editar_Inodo_Archivo(FILE* disco, SuperBlock sblock, int inicio, int size , char *texto, int posI);
bool Editar_BArchivo(FILE* disco, SuperBlock sblock,char* texto, int posA, int* index);
bool Editar_BApuntador_Archivo(FILE *disco, SuperBlock sblock,int size, int inicio, char *texto,int pos,int* index);


void Borrar_Contenido_Archivo(FILE* disco, SuperBlock sblock, int inicio, int size , char* ruta, int *cantidad);
void Borrar_Inodo_Archivo(FILE* disco, SuperBlock sblock, int inicio, int size , int *cantidad, int posI);
bool Borrar_BArchivo(FILE* disco, SuperBlock sblock, int *cantidad, int posA, int* index);
bool Borrar_BApuntador_Archivo(FILE *disco, SuperBlock sblock,int size, int inicio, int *cantidad,int pos,int* index);




#endif






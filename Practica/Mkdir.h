#ifndef MKDIR_H
#define MKDIR_H

#include "Estructuras.h"
#include "Metodos.h"
#include "Metodos_Analizar.h"
#include "Metodos_Archivo.h"
#include "Metodos_Directorio.h"

void Analizar_Mkdir(char* comando, int tamanio);
void Validar_Mkdir();
void Ejecutar_Comandos_Mkdir();

void Ejecutar_P(FILE *disco, SuperBlock sblock, int size, int inicio);
//void Ejecutar_Size(FILE *disco, SuperBlock sblock, int size, int inicio);
//void Ejecutar_Cont(FILE *disco, SuperBlock sblock, int size, int inicio);

//void Verificar_File();
//bool Comprobar_File(FILE* disco,SuperBlock sblock,int size,int inicio);


#endif





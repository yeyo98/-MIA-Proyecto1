#ifndef MKFILE_H
#define MKFILE_H

#include "Estructuras.h"
#include "Metodos.h"
#include "Metodos_Analizar.h"
#include "Metodos_Archivo.h"
#include "Mkdir.h"

void Analizar_Mkfile(char* comando, int tamanio);
void Validar_Mkfile();
void Ejecutar_Comandos();

//void Ejecutar_P(FILE *disco, SuperBlock sblock, int size, int inicio);
void Ejecutar_Size(FILE *disco, SuperBlock sblock, int size, int inicio);
void Ejecutar_Cont(FILE *disco, SuperBlock sblock, int size, int inicio);

void Verificar_File();
bool Comprobar_File(FILE* disco,SuperBlock sblock,int size,int inicio);
//bool Existe_Group(char*contenido,char* grupo);


#endif





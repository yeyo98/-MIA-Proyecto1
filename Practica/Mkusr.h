#ifndef MKUSR_H
#define MKUSR_H

#include "Estructuras.h"
#include "Metodos.h"
#include "Metodos_Analizar.h"
#include "Metodos_Archivo.h"

void Analizar_Mkusr(char* comando, int tamanio);
void Validar_Mkusr();
void Verificar_Usuario();
bool Comprobar_Usuario(FILE* disco,SuperBlock sblock,int size,int inicio);
bool Existe_Group(char*contenido,char* grupo);

#endif

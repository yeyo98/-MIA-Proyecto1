#ifndef EDIT_H
#define EDIT_H

#include "Estructuras.h"
#include "Metodos.h"
#include "Metodos_Analizar.h"
#include "Metodos_Archivo.h"
#include "Mkdir.h"

void Analizar_Edit(char* comando, int tamanio);
void Validar_Edit();
void Ejecutar_Comandos_Edit();

//void Ejecutar_P(FILE *disco, SuperBlock sblock, int size, int inicio);
void Ejecutar_Size_Edit(FILE *disco, SuperBlock sblock, int size, int inicio);
void Ejecutar_Cont_Edit(FILE *disco, SuperBlock sblock, int size, int inicio);

void Verificar_Edit();
bool Comprobar_Edit(FILE* disco,SuperBlock sblock,int size,int inicio);
//bool Existe_Group(char*contenido,char* grupo);


#endif





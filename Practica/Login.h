#ifndef LOGIN_H
#define LOGIN_H

#include "Estructuras.h"
#include "Metodos.h"
#include "Metodos_Analizar.h"
#include "Metodos_Archivo.h"

void Analizar_Login(char* comando, int tamanio);
void Validar_Login();
void Verificar_Login();
bool Comprobar_Login(FILE* disco,SuperBlock sblock,int size);
int Get_Id_Group(char *contenido, char* grupo);

#endif

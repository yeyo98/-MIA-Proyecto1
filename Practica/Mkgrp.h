#ifndef MKGRP_H
#define MKGRP_H

#include "Estructuras.h"
#include "Metodos.h"
#include "Metodos_Analizar.h"
#include "Metodos_Archivo.h"

void Analizar_Mkgrp(char* comando, int tamanio);
void Validar_Mkgrp();
void Verificar_Grupo();
bool Comprobar_Grupo(FILE* disco,SuperBlock sblock,int size,int inicio);

#endif

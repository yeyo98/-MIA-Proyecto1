#ifndef EXEC_H
#define EXEC_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <stdbool.h>

#include "Estructuras.h"
#include "Metodos.h"
#include "Metodos_Analizar.h"

void Analizar_Exec(char* comando, int tamanio);

void Validar_Exec(void);
void Abrir_Archivo();
void Hay_Salto(char* cadena);

#endif

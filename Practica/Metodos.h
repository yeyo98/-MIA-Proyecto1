#ifndef METODOS_H
#define METODOS_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <stdbool.h>
#include "Estructuras.h"

#include <time.h>
#include <dirent.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>

#define COLOR_RED     "\x1b[1;31m"
#define COLOR_GREEN   "\x1b[1;32m"
#define COLOR_YELLOW  "\x1b[1;33m"
#define COLOR_BLUE    "\x1b[1;34m"
#define COLOR_MAGENTA "\x1b[1;35m"
#define COLOR_CYAN    "\x1b[1;36m"
#define RESET   "\x1b[0m"

#define ROJO  0
#define VERDE 1
#define AMARILLO 2
#define AZUL 3
#define MAGENTA 4
#define CYAN 5


bool Validar_Numero(char* num);
void Crear_Carpetas(char* ruta);
void Obtener_Fecha(char* fecha);
void Eliminar_Salto_Linea(char* cadena);
void Eliminar_Espacios(char* cadena);

void Mensaje_Aceptacion(char* mensaje);
void Mensaje_Negacion(char* mensaje);
void Mensaje_Advertencia(char* mensaje);
void Mensaje_Green(char* mensaje);
void Mensaje_Blue(char* mensaje);
void Mensaje_Magenta(char* mensaje);

void Mensaje_Color(char* mensaje, int color);

void Ruta_Raid(char* ruta);

#endif

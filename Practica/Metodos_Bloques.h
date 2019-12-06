#ifndef METODOS_BLOQUES_H
#define METODOS_BLOQUES_H

#include "Metodos.h"
#include "Lista_Mount.h"
#include "math.h"

int Calculo_Posicion_Inodo(SuperBlock sblock, int i);
int Calculo_Posicion_Block(SuperBlock sblock, int i);
int Calcular_N(int tam_particion);

void Modificar_Count_I(SuperBlock *sblock, int inicio, FILE* disco, int tipo, int inode_start);
void Modificar_Count_B(SuperBlock *sblock, int inicio, FILE* disco, int tipo, int block_start);
void Marcar_BitMap_I(int inode_start, int size, FILE* disco, char a, int posicion);
void Marcar_BitMap_B(int block_starts, int size, FILE* disco, char a, int posicion);

int BM_Inodo_Disponible(FILE* disco,int inode_start,int size);
int BM_Bloque_Disponible(FILE* disco,int block_start,int size);

void Modificar_Inodo(FILE* disco, Inodo inodo, int pos, int tipo);

#endif

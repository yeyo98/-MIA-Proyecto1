#ifndef ESTRUCTURAS_H
#define ESTRUCTURAS_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <stdbool.h>

typedef struct Datos{
    //CARACTERISTICAS DEL MKDISK
    int size ;
    char fit ;
    char unit ;
    char path[1000];

    //CARACTERISTICAS DEL FDISK
    char unit_f;
    char type;
    char fit_f;
    char deleteP;
    int add;
    bool negado;    //FALSE PARA ADD,   TRUE PARA DELETE *ESTE SE USARA PARA VER SI ATRIBUTO ADD ES NEGADO O NO*
    char name[1000];
    char id[1000];

    //CARACTERISTICAS DEL MKFS
    char fs;

    //CARACTERISTICAS DEL LOGIN
    char usr[1000];
    char pwd[1000];
    char grp[1000];

    // CARACTERISTICAS DEL MKFILE
    bool p;
    char cont[1000];

}Datos;


//============ SUPER BLOQUE ==============
typedef struct SuperBlock{

    int s_filesystem_type; //GUARDA EL NUMERO QUE IDENTIFICA EL SISTEMA DE ARCHIVOS UTILIZAOO
    int s_inodes_count; //GUARDA EL NUMERO TOTAL DE INODOS
    int s_blocks_count; //GUARDA EL NUMERO TOTAL DE BLOQUES
    int s_free_blocks_count;//CONTIENE EL NÚMERO DE BLOQUES LIBRES
    int s_free_inodes_count;//CONTIENE EL NÚMERO DE INODOS LIBRES
    char s_mtime[16];//ULTIMA FECHA EN EL QUE EL SISTEMA FUE MONTADO
    char s_umtime[16];//ULTIMA FECHA EN EL QUE EL SISTEMA FUE DESMONTADO
    int s_mnt_count;//INDICA CUANTAS VECES SE HA MONTADO EL SISTEMA
    int s_magic;//VALOR QUE IDENTIFICA AL SISTEMA DE ARCHIVOS, TENDRÁ EL VALOR 0XEF53
    int s_inodes_size;//TAMANIO DEL INODO
    int s_block_size;//TAMAÑO DEL BLOQUE
    int s_first_ino;//PRIMER INODO LIBRE
    int s_first_blo;//PRIMER BLOQUE LIBRE
    int s_bm_inode_start;//GUARDARÁ EL INICIO DEL BITMAP DE INODOS
    int s_bm_block_start;//GUARDARÁ EL INICIO DEL BITMAP DE BLOQUES
    int s_inode_start;//GUARDARÁ EL INICIO DE LA TABLA DE INODOS
    int s_block_start;//GUARDARÁ EL INICO DE LA TABLA DE BLOQUES

}SuperBlock;

//=============== JOURNALING ===============
typedef struct Journal{

    char journal_tipo_operacion; //EL TIPO DE OPERACIÓN A REALIZARSE
    char journal_tipo;//
    char journal_nombre[12];//
    char journal_contenido; //MODIFICAR
    char journal_fecha[16];
    int journal_propietario;
    int journal_permisos;

}Journal;



typedef struct Inodo{

    int i_uid;//UID DEL USUARIO PROPIETARIO DEL ARCHIVO O CARPETA
    int l_gid;//GID DEL GRUPO AL QUE PERTENECE EL ARCHIVO O CARPETA.
    int i_size;//TAMAÑO DEL ARCHIVO EN BYTES
    char i_atime[16];//ÚLTIMA FECHA EN QUE SE LEYÓ EL INODO SIN MODIFICARLO
    char i_ctime[16];//FECHA EN LA QUE SE CREÓ EL INODO
    char i_mtime[16];//ÚLTIMA FECHA EN LA QUE SE MODIFICÓ EL INODO
    int iblock[16];//ARRAY EN LOS QUE LOS PRIMEROS 12 REGISTROS SON BLOQUES DIRECTOS. 13 SIMPLE, 14 DOBLE Y 15 TRIPLE INDIRECTO
    char i_type;//INDICA SI ES ARCHIVO O CARPETA. TENDRÁ LOS SIGUIENTES VALORES: 1 = ARCHIVO 0 = CARPETA
    int i_perm;//GUARDARÁ LOS PERMISOS DEL ARCHIVO O CARPETA

}Inodo;


//============== STRUCT PARA BLOQUE DE CARPETAS ==========
typedef struct Content{

    char b_name[12]; //NOMBRE DE LA CARPETA O ARCHIVO
    int b_inodo;//APUNTADOR HACIA UN INODO ASOCIADO AL ARCHIVO O CARPETA

}Content;


//============== BLOQUE DE CARPETAS =============
typedef struct B_Carpeta{

    Content b_content[4];

}B_Carpeta;

//============= BLOQUE DE ARCHIVO =============
typedef struct B_Archivo{

    char b_content[64];

}B_Archivo;

//============ BLOQUE DE APUNTADORES ============
typedef struct B_Apuntadores{

    int b_pointers[16];

}B_Apuntadores;


char mensaje[2000];

typedef struct Datos_Logueado
{
    //CARACTERISTICAS DEL LOGIN
    char usr[15];
    char pwd[15];
    char id[15];
    char grp[15];
    int usuario;    // ESTE ES EL ID DEL USUARIO
    int grupo;      // ESTE ES EL ID DEL GRUPO
}Datos_Logueado;





Datos caracteristicas;      //VARIABLE AUXILIAR PARA GUARDAR LOS VALORES
Datos_Logueado daLoguer;

void Limpiar_Datos(void);   //METODO PARA LIMPIAR LOS VALORES
void DesLoguear(void);
void Aux_Llenar_Loguer();


#endif

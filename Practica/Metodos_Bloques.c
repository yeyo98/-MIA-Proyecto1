#include "Metodos_Bloques.h"

int Calculo_Posicion_Inodo(SuperBlock sblock, int i)
{
    return sblock.s_inode_start + (sizeof(Inodo) * i );
}

int Calculo_Posicion_Block(SuperBlock sblock, int i)
{
    return sblock.s_block_start + (sizeof(B_Carpeta) * i );
}

int Calcular_N(int tam_particion)
{
    int tam_SuperBlock = sizeof(SuperBlock);
    int tam_journling = sizeof(Journal);
    int tam_inodo = sizeof(Inodo);
    int tam_block = sizeof(B_Carpeta);

    int operador = (tam_particion - tam_SuperBlock);
    int denominador = 1 + tam_journling + 3 + tam_inodo + 3*tam_block;
    int n = floor(operador / denominador);

    return n;
}

void Modificar_Count_I(SuperBlock *sblock, int inicio, FILE* disco, int tipo, int inode_start)
{
    sblock->s_first_ino = BM_Inodo_Disponible(disco,sblock->s_bm_inode_start,inode_start);

    if(tipo == 0)//AUMENTAR
    {
        sblock->s_free_inodes_count = sblock->s_free_inodes_count -1;

    }else if(tipo == 1)//DECREMENTAR
    {
        sblock->s_free_inodes_count = sblock->s_free_inodes_count +1;
    }
    fseek(disco,inicio,SEEK_SET);
    fwrite(sblock, sizeof(SuperBlock), 1, disco);
}


void Modificar_Count_B(SuperBlock *sblock, int inicio, FILE* disco, int tipo, int block_start)
{
    sblock->s_first_blo = BM_Bloque_Disponible(disco,sblock->s_bm_inode_start,block_start);

    if(tipo == 0)//AUMENTAR
    {
        sblock->s_free_blocks_count = sblock->s_free_blocks_count -1;

    }else if(tipo == 1)//DECREMENTAR
    {
        sblock->s_free_blocks_count = sblock->s_free_blocks_count +1;
    }
    fseek(disco,inicio,SEEK_SET);
    fwrite(sblock, sizeof(SuperBlock), 1, disco);
}

void Marcar_BitMap_I(int inode_start, int size, FILE* disco, char a, int posicion)
{
    fseek(disco,inode_start,SEEK_SET);//ME POSICIONO EN INICIO DEL BITMAP DE INODOS
    int n = Calcular_N(size);

    char Bm_inodo[n];//TENGO LA BASE PARA OBTENER EL BITMAP DE INODOS
    fread(Bm_inodo,sizeof(char), n,disco);//OBTENGO EL BITMAP DE INODOS

    Bm_inodo[posicion] = a;
    fseek(disco,inode_start,SEEK_SET);//ME POSICIONO EN INICIO DEL BITMAP DE INODOS
    fwrite(Bm_inodo,sizeof(Bm_inodo),1,disco);

}

void Marcar_BitMap_B(int block_start,int size, FILE* disco, char a, int posicion)
{
    fseek(disco,block_start,SEEK_SET);//ME POSICIONO EN INICIO DEL BITMAP DE BLOQUE
    int n = Calcular_N(size);

    char Bm_bloque[3*n];//TENGO LA BASE PARA OBTENER EL BITMAP DE BLOQUE
    fread(Bm_bloque,sizeof(char), 3*n,disco);//OBTENGO EL BITMAP DE BLOQUE

    Bm_bloque[posicion] = a;
    fseek(disco,block_start,SEEK_SET);//ME POSICIONO EN INICIO DEL BITMAP DE BLOQUE
    fwrite(Bm_bloque,sizeof(Bm_bloque),1,disco);
}


int BM_Inodo_Disponible(FILE* disco,int inode_start,int size)
{
    fseek(disco,inode_start,SEEK_SET);//ME POSICIONO EN INICIO DEL BITMAP DE INODOS
    int n = Calcular_N(size);
    char Bm_inodo[n];//TENGO LA BASE PARA OBTENER EL BITMAP DE INODOS
    fread(Bm_inodo,sizeof(char),n,disco);

    for(unsigned int i=0; i < sizeof(Bm_inodo) ;i++)
    {
        if(Bm_inodo[i] == '0')
            return i;
    }
    return -1;
}

int BM_Bloque_Disponible(FILE* disco,int block_start,int size)
{
    fseek(disco,block_start,SEEK_SET);//ME POSICIONO EN INICIO DEL BITMAP DE BLOQUE
    int n = Calcular_N(size);

    char Bm_bloque[3*n];//TENGO LA BASE PARA OBTENER EL BITMAP DE BLOQUE
    fread(Bm_bloque,sizeof(char), 3*n,disco);//OBTENGO EL BITMAP DE BLOQUE


    for(unsigned int i=0; i < sizeof(Bm_bloque) ;i++)
    {
        if(Bm_bloque[i] == '0')
            return i;
    }
    return -1;
}



void Modificar_Inodo(FILE* disco, Inodo inodo, int pos, int tipo)
{
    if(tipo == 0)//ESTE VA SER PARA MODIFICAR FECHA DE LECTURA
    {
        memset(inodo.i_atime,'\0',sizeof(inodo.i_atime));
        Obtener_Fecha(inodo.i_atime);

    }else if(tipo == 1)//ESTE VA SER PARA MODIFICAR FECHA DE MODIFICACION
    {
        memset(inodo.i_mtime,'\0',sizeof(inodo.i_mtime));
        Obtener_Fecha(inodo.i_mtime);
    }


    fseek(disco,pos,SEEK_SET);
    fwrite(&inodo,sizeof(Inodo),1,disco);

}















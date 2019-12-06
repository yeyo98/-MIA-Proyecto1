#include "Metodos_Directorio.h"

int Existe_Directorio(FILE *disco, SuperBlock sblock, int size, char *ruta)
{
    return Buscar_Inodo(disco,sblock,size,ruta,0,0);    //METODO QUE VIENE DE METODOS_ARCHIVO
}


//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
//%%%%%%%%%%%%%%%%%%%%%%      METODOS PARA CREAR DIRECTORIO      %%%%%%%%%%%%%%%%%%%%
//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

void Crear_Directorio( FILE* disco, SuperBlock sblock, int inicio,int size ,char* ruta)
{
    //OBTENER LOS NOMBRES DE LAS RUTAS
    char delim[] = "/";
    char nuevo[1000];
    memset(nuevo,'\0',sizeof(nuevo));
    strcat(nuevo,ruta);

    char* nombres[1000];
    memset(nombres,'\0',sizeof(nombres));

    char *ptr = strtok(nuevo, delim);

    int aux=0;
    while(ptr != NULL)
    {
        nombres [aux++] = ptr;
        ptr = strtok(NULL, delim);
    }


    char auxRuta[1000];
    memset(auxRuta,'\0',sizeof(auxRuta));

    if(aux == 1)        //SIGNIFICA QUE ESTA EN LA RAIZ
    {

        Enlazar_Inodo_Directorio(disco,sblock,inicio,size,ruta,0);

    }else               //SI NO ES PORQUE ESTA DENTRO DE UNA CARPETA
    {
        auxRuta[0] = '/';

        for(int i=0; i<aux-1 ;i++)
        {
            if(i != 0)
                strcat(auxRuta,"/");

            strcat(auxRuta,nombres[i]);
        }
        int pos = Existe_Directorio(disco,sblock,size,auxRuta);

        if(pos != -1)
            Enlazar_Inodo_Directorio(disco,sblock,inicio,size,ruta,pos);
        else
            Mensaje_Color("*** ERROR!! LA DIRECCION NO EXISTE PARA EL DIRECTORIO ***\n",ROJO);

    }
}


void Enlazar_Inodo_Directorio(FILE *disco, SuperBlock sblock, int inicio, int size,char* ruta,int pos)
{
    int pos_block = Crear_Inodo_Directorio(disco,sblock,inicio,size);      //=======================> AQUI

    if(pos_block != -1)             //EN TEORIA YA NO HABRIA ESPACIO
    {
        Inodo inodo_aux;
        int n = Calcular_N(size);
        char Bm_bloque[3 * n];

        //======================= OBTENGO EL INODO  =====================
        fseek(disco, Calculo_Posicion_Inodo(sblock,pos) ,SEEK_SET); //ME POSICIONO EN EL INODO
        fread(&inodo_aux,sizeof(Inodo),1,disco);

        Modificar_Inodo(disco,inodo_aux,  Calculo_Posicion_Inodo(sblock,pos) ,1);


        //==================== OBTENGO EL BM DE LOS INODOS ==============
        //fseek(disco,sblock.s_bm_inode_start,SEEK_SET);
        //fread(Bm_inodo,sizeof(char),n,disco);
        //==================== OBTENGO EL BM DE LOS BLOQUES =============
        fseek(disco,sblock.s_bm_block_start,SEEK_SET);
        fread(Bm_bloque,sizeof(char),3*n,disco);


        for(int i=0;i<16;i++)
        {
            if(inodo_aux.iblock[i] != -1)   //SI ES -1 DEBO CREAR UN NUEVO B_CARPETA
            {
                int posB = inodo_aux.iblock[i];
                bool r ;

                if(Bm_bloque[posB] == '1')        //SI ES 1 ES UN BLOQUE CARPETA
                {
                    r =Enlazar_BCarpeta_Archivo(disco,sblock,ruta, posB,pos_block);

                }else if(Bm_bloque[posB] == '3')  //SI ES 3 ES UN BLOQUE APUNTADOR
                {
                    r = Enlazar_BApuntador_Archivo(disco,sblock,size,inicio,ruta,posB,pos_block);
                }

                if(r){break;}
            }
            else
            {
                if(i == 13)//NIVEL 1
                {
                    int posicion = Crear_BApuntador(disco, sblock,inicio,size,1);
                    inodo_aux.iblock[i] = posicion;

                    fseek(disco, Calculo_Posicion_Inodo(sblock,pos) ,SEEK_SET); //ME POSICIONO EN EL INODO
                    fwrite(&inodo_aux,sizeof(Inodo),1,disco);

                    bool r =Enlazar_BApuntador_Archivo(disco,sblock,size,inicio,ruta, inodo_aux.iblock[i],pos_block);
                    if(r){break;}

                }else if(i == 14)// NIVEL 2
                {
                    int posicion = Crear_BApuntador(disco, sblock,inicio,size,2);
                    inodo_aux.iblock[i] = posicion;

                    fseek(disco, Calculo_Posicion_Inodo(sblock,pos) ,SEEK_SET); //ME POSICIONO EN EL INODO
                    fwrite(&inodo_aux,sizeof(Inodo),1,disco);

                    bool r =Enlazar_BApuntador_Archivo(disco,sblock,size,inicio,ruta, inodo_aux.iblock[i],pos_block);
                    if(r){break;}

                }else if(i == 15)// NIVEL 3
                {
                    int posicion = Crear_BApuntador(disco, sblock,inicio,size,3);
                    inodo_aux.iblock[i] = posicion;

                    fseek(disco, Calculo_Posicion_Inodo(sblock,pos) ,SEEK_SET); //ME POSICIONO EN EL INODO
                    fwrite(&inodo_aux,sizeof(Inodo),1,disco);

                    bool r =Enlazar_BApuntador_Archivo(disco,sblock,size,inicio,ruta, inodo_aux.iblock[i],pos_block);
                    if(r){break;}

                }
                else        //ESTE SERIA PARA UNA CARPETA NORMAL
                {
                    //==========  CREO UN BLOQUE CARPETA Y ACTULIZO MI INODO ============
                    int posicion = Crear_BCarpeta(disco,sblock,inicio,size);
                    inodo_aux.iblock[i] = posicion;

                    fseek(disco, Calculo_Posicion_Inodo(sblock,pos) ,SEEK_SET); //ME POSICIONO EN EL INODO
                    fwrite(&inodo_aux,sizeof(Inodo),1,disco);

                    bool r =Enlazar_BCarpeta_Archivo(disco,sblock,ruta, inodo_aux.iblock[i],pos_block);
                    if(r){break;}
                }
            }
        }
    }
    else
    {
        Mensaje_Color("*** YA NO HAY MAS ESPACIO ***\n",ROJO);
    }

}


int Crear_Inodo_Directorio(FILE* disco, SuperBlock sblock, int inicio, int size)
{
    int posicion = BM_Inodo_Disponible(disco,sblock.s_bm_inode_start,size);


    if(posicion != -1)
    {
        Inodo inodo;
        inodo.i_uid = daLoguer.usuario;         //UID DEL USUARIO PROPIETARIO DEL ARCHIVO O CARPETA
        inodo.l_gid = daLoguer.grupo;           //GID DEL GRUPO AL QUE PERTENECE EL ARCHIVO O CARPETA.
        inodo.i_size = 0;                       //TAMAÑO DEL ARCHIVO EN BYTES
        Obtener_Fecha(inodo.i_atime);           //ÚLTIMA FECHA EN QUE SE LEYÓ EL INODO SIN MODIFICARLO
        Obtener_Fecha(inodo.i_ctime);           //FECHA EN LA QUE SE CREÓ EL INODO
        Obtener_Fecha(inodo.i_mtime);           //ÚLTIMA FECHA EN LA QUE SE MODIFICÓ EL INODO
        memset(inodo.iblock,-1,sizeof(inodo.iblock)/4);
        inodo.i_type = '0';                     //INDICA SI ES ARCHIVO O CARPETA. TENDRÁ LOS SIGUIENTES VALORES: 1 = ARCHIVO 0 = CARPETA
        inodo.i_perm = 777;                     //GUARDARÁ LOS PERMISOS DEL ARCHIVO O CARPETA


        //=============== CREO SU BLOQUE CARPETA CON LOS PRIMEROS INDICES LLENOS  ==================
        inodo.iblock[0] = Crear_BCarpeta_Directorio(disco,sblock, inicio, size,posicion);



        fseek(disco, Calculo_Posicion_Inodo(sblock,posicion),SEEK_SET); //ME DESPLAZO HASTA EL COMIENZO DE LOS INODOS
        fwrite(&inodo, sizeof(Inodo), 1 ,  disco);

        //===============   AHORA ACTUALIZO LOS BITMAP'S  ===================
        Marcar_BitMap_I(sblock.s_bm_inode_start, size, disco, '1', posicion);
        //Marcar_BitMap_B(sblock.s_bm_block_start,datos_particion->size, disco, '1', 0);

        //===============   AHORA ACTUALIZO EL SUPERBLOQUE  ===================
        Modificar_Count_I(&sblock, inicio, disco, 0,size);
        //Modificar_Count_B(&sblock,datos_particion->inicio, disco, 0, datos_particion->size);
    }

    return posicion;

}


int Crear_BCarpeta_Directorio(FILE* disco, SuperBlock sblock, int inicio, int size,int anterior)
{
    int posicion = BM_Bloque_Disponible(disco,sblock.s_bm_block_start,size);

    if(posicion != -1)
    {
        B_Carpeta b_carpeta;

        for(int i=0;i<4;i++)
        {
            b_carpeta.b_content[i].b_inodo=-1;
            memset(b_carpeta.b_content[i].b_name,'\0',sizeof(b_carpeta.b_content[i].b_name));
        }

        strcpy( b_carpeta.b_content[0].b_name ,".");
        b_carpeta.b_content[0].b_inodo=anterior;

        strcpy( b_carpeta.b_content[1].b_name ,"..");
        b_carpeta.b_content[1].b_inodo=anterior;


        fseek(disco, Calculo_Posicion_Block(sblock,posicion),SEEK_SET); //ME DESPLAZO HASTA EL COMIENZO DE LOS INODOS
        fwrite(&b_carpeta, sizeof(B_Carpeta), 1 ,  disco);

        //===============   AHORA ACTUALIZO LOS BITMAP'S  ===================
        //Marcar_BitMap_I(sblock.s_bm_inode_start, size, disco, '1', posicion);
        Marcar_BitMap_B(sblock.s_bm_block_start,size, disco, '1', posicion);

        //===============   AHORA ACTUALIZO EL SUPERBLOQUE  ===================
        //Modificar_Count_I(&sblock, inicio, disco, 0,size);
        Modificar_Count_B(&sblock,inicio, disco, 0, size);
    }

    return posicion;
}


//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
//%%%%%%%%%%%%%%%%%%%%%%      METODOS PARA ELIMINAR DIRECTORIO      %%%%%%%%%%%%%%%%%
//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

void Eliminar_Directorio( FILE* disco, SuperBlock sblock, int inicio,int size ,char* ruta)
{
    Eliminar_Archivo( disco,  sblock,  inicio, size , ruta);
}
















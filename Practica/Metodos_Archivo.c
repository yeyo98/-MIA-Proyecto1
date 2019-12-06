#include "Metodos_Archivo.h"

//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
//%%%%%%%%%%%%%%%%%%%%%%%      METODOS PARA BUSCAR INODO       %%%%%%%%%%%%%%%%%%%%%%
//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

int Existe_Archivo(FILE* disco,SuperBlock sblock, int size,char* ruta)
{

    return Buscar_Inodo(disco,sblock,size,ruta,0,0);

}

int Buscar_Inodo(FILE *disco, SuperBlock sblock, int size, char *ruta,int pos,int cont)
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

    //SACAR VALORES DE MI PARTICION
    int r = -1;
    Inodo inodo_aux;
    int n = Calcular_N(size);
    //char Bm_inodo[n];
    char Bm_bloque[3 * n];

    //======================= OBTENGO EL INODO  =====================
    fseek(disco, Calculo_Posicion_Inodo(sblock,pos) ,SEEK_SET); //ME POSICIONO AL COMIENZO DE LOS INODOS
    fread(&inodo_aux,sizeof(Inodo),1,disco);

    Modificar_Inodo(disco,inodo_aux,  Calculo_Posicion_Inodo(sblock,pos) ,0);


    //==================== OBTENGO EL BM DE LOS INODOS ==============
    //fseek(disco,sblock.s_bm_inode_start,SEEK_SET);
    //fread(Bm_inodo,sizeof(char),n,disco);
    //==================== OBTENGO EL BM DE LOS BLOQUES =============
    fseek(disco,sblock.s_bm_block_start,SEEK_SET);
    fread(Bm_bloque,sizeof(char),3*n,disco);



    for(int i=0; i < 16 ;i++)
    {
        if(inodo_aux.iblock[i] != -1)
        {
            int pos = inodo_aux.iblock[i];

            if(pos >-1 && pos <16)
            {
                if(Bm_bloque[pos] == '1')        //SI ES UN BLOQUE CARPETA
                {
                    r = Buscar_BCarpeta(disco,sblock,size,ruta,pos,cont);
                    if(r != -1) break;

                }else if(Bm_bloque[pos] == '3')  //SI ES UN BLOQUE APUNTADOR
                {
                    r = Buscar_BApuntador(disco,sblock,size,ruta,pos,cont);
                    if(r != -1) break;
                }
                /*
                 *else if(Bm_bloque[pos] == '2')  //SI ES UN BLOQUE ARCHIVO
                 */
            }
        }

    }
    return r;
}


int Buscar_BCarpeta(FILE *disco, SuperBlock sblock, int size, char *ruta,int pos,int cont)
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

    B_Carpeta aux_carpeta;
    int r = -1;

    fseek(disco,Calculo_Posicion_Block(sblock,pos),SEEK_SET);
    fread(&aux_carpeta,sizeof(B_Carpeta),1,disco);

    for(int i=0; i<4 ;i++)
    {
        char* name = aux_carpeta.b_content[i].b_name;

        if(strcasecmp(name,nombres[cont]) == 0)
        {
            int lim =0;

            for(int j=0; j<1000 ;j++)//CONTANDO EL NUMERO DE NOMBRES USADOS
            {
                if(nombres[j] != '\0')
                    lim++;
                else
                    break;
            }

            if(cont+1 < lim)
            {
                r = Buscar_Inodo(disco,sblock,size,ruta, aux_carpeta.b_content[i].b_inodo,cont+1 );
                if(r != -1) break;

            }else//CONDICION PARA VERIFICAR QUE LLEGAMOS AL DESTINO
            {
                r = aux_carpeta.b_content[i].b_inodo;
                break;
            }
        }
    }
    return r;
}


int Buscar_BApuntador(FILE *disco, SuperBlock sblock, int size, char *ruta, int pos, int cont)
{
    int r = -1;
    B_Apuntadores aux_apuntador;
    int n = Calcular_N(size);
    char Bm_bloque[3 * n];

    //==================== OBTENGO EL BM DE LOS BLOQUES =============
    fseek(disco,sblock.s_bm_block_start,SEEK_SET);
    fread(Bm_bloque,sizeof(char),3*n,disco);


    fseek(disco,Calculo_Posicion_Block(sblock,pos),SEEK_SET);
    fread(&aux_apuntador,sizeof(B_Apuntadores),1,disco);

    for(int i=0; i<16 ;i++)
    {
        if(Bm_bloque[ aux_apuntador.b_pointers[i] ] == '1')//SI ES BLOQUE CARPETA
        {
            r = Buscar_BCarpeta(disco,sblock,size,ruta,aux_apuntador.b_pointers[i],cont);
            if(r != -1) break;

        }else if(Bm_bloque[ aux_apuntador.b_pointers[i] ] == '3')//SI ES BLOQUE APUNTADOR
        {
            r = Buscar_BApuntador(disco,sblock,size,ruta,aux_apuntador.b_pointers[i],cont);
            if(r != -1) break;
        }
    }
    return r;
}

//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
//%%%%%%%%%%%%%%%%%%%%%%%       METODOS PARA CREAR INODO       %%%%%%%%%%%%%%%%%%%%%%
//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%


void Crear_Archivo( FILE* disco, SuperBlock sblock, int inicio,int size ,char* ruta)
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

        Enlazar_Inodo_Archivo(disco,sblock,inicio,size,ruta,0);

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
            Enlazar_Inodo_Archivo(disco,sblock,inicio,size,ruta,pos);
        else
            Mensaje_Color("*** ERROR!! LA DIRECCION NO EXISTE PARA EL ARCHIVO ***\n",ROJO);

    }
}



void Enlazar_Inodo_Archivo(FILE *disco, SuperBlock sblock, int inicio, int size,char* ruta,int pos)
{
    int pos_block = Crear_Inodo_Archivo(disco,sblock,inicio,size);      //=======================> AQUI

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
            if(inodo_aux.iblock[i] > -1 && inodo_aux.iblock[i] < 16)   //SI ES -1 DEBO CREAR UN NUEVO B_CARPETA
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

bool Enlazar_BCarpeta_Archivo(FILE *disco, SuperBlock sblock, char *ruta,int pos,int pos_i)
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

    B_Carpeta aux_carpeta;
    bool r = false;

    fseek(disco,Calculo_Posicion_Block(sblock,pos),SEEK_SET);
    fread(&aux_carpeta,sizeof(B_Carpeta),1,disco);

    for(int i=0; i<4 ;i++)
    {
        int apt = aux_carpeta.b_content[i].b_inodo;

        if(apt == -1)   //SI ES IGUAL ES PORQUE EL ESPACIO ESTA DIPONIBLES
        {
            strcpy( aux_carpeta.b_content[i].b_name,nombres[aux-1]);
            aux_carpeta.b_content[i].b_inodo = pos_i;

            fseek(disco,Calculo_Posicion_Block(sblock,pos),SEEK_SET);
            fwrite(&aux_carpeta,sizeof(B_Carpeta),1,disco);

            return true;
        }
    }
    return r;
}


bool Enlazar_BApuntador_Archivo(FILE *disco, SuperBlock sblock,int size, int inicio, char *ruta,int pos,int pos_a)
{
    B_Apuntadores aux_apuntador;
    bool r = false;
    int n = Calcular_N(size);
    char Bm_bloque[3 * n];

    fseek(disco,Calculo_Posicion_Block(sblock,pos),SEEK_SET);
    fread(&aux_apuntador,sizeof(B_Apuntadores),1,disco);

    //==================== OBTENGO EL BM DE LOS INODOS ==============
    //fseek(disco,sblock.s_bm_inode_start,SEEK_SET);
    //fread(Bm_inodo,sizeof(char),n,disco);
    //==================== OBTENGO EL BM DE LOS BLOQUES =============
    fseek(disco,sblock.s_bm_block_start,SEEK_SET);
    fread(Bm_bloque,sizeof(char),3*n,disco);

    for(int i=0; i<16 ;i++)
    {
        int apt = aux_apuntador.b_pointers[i];

            if(apt != -1 )    //PRIMERO BUSCAREMOS EN LOS BLOQUES EXISTENTES
            {
                if( Bm_bloque[apt] == '1' ) //EN EL CASO DE BLOQUE CARPETA
                {
                    r = Enlazar_BCarpeta_Archivo(disco,sblock,ruta, apt, pos_a);
                    if(r)return true;

                }else if(Bm_bloque[apt] == '3') //EN EL CASO DE BLOQUE APUNTADOR
                {
                    r = Enlazar_BApuntador_Archivo(disco,sblock,size,inicio,ruta,apt,pos_a);
                    if(r)return true;
                }

            }else   //SI ES IGUAL ES PORQUE EL ESPACIO ESTA DIPONIBLES
            {
                //==========  CREO UN BLOQUE CARPETA Y ACTULIZO MI INODO ============
                int posicion = Crear_BCarpeta(disco,sblock,inicio,size);
                aux_apuntador.b_pointers[i] = posicion;

                fseek(disco, Calculo_Posicion_Block(sblock,pos) ,SEEK_SET); //ME POSICIONO EN EL INODO
                fwrite(&aux_apuntador,sizeof(B_Apuntadores),1,disco);

                r =Enlazar_BCarpeta_Archivo(disco,sblock,ruta, posicion,pos_a);
                if(r){break;}

            }

    }
    return r;
}




int Crear_Inodo_Archivo(FILE* disco, SuperBlock sblock, int inicio, int size)
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
        //memset(inodo.iblock,-1,sizeof(inodo.iblock));
        for(int i=0;i<16;i++)
            inodo.iblock[i] = -1;

        inodo.i_type = '1';                     //INDICA SI ES ARCHIVO O CARPETA. TENDRÁ LOS SIGUIENTES VALORES: 1 = ARCHIVO 0 = CARPETA
        inodo.i_perm = 777;                     //GUARDARÁ LOS PERMISOS DEL ARCHIVO O CARPETA

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


int Crear_BCarpeta(FILE* disco, SuperBlock sblock, int inicio, int size)
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


int Crear_BApuntador(FILE* disco, SuperBlock sblock, int inicio, int size,int nivel)// CREO LOS BLOQUE APUNTADORES DE CUALQUIER NIVEL
{
    int posicion = BM_Bloque_Disponible(disco,sblock.s_bm_block_start,size);
    if(posicion != -1)
    {
        B_Apuntadores b_apuntador;

        for(int i=0;i<16;i++)
            b_apuntador.b_pointers[i] = -1;

        if(nivel > 1)
        {
            int apt = Crear_BApuntador(disco,sblock,inicio,size,nivel-1);
            b_apuntador.b_pointers[0] = apt;

            //=========== OBTENGO OTRA VEZ LA POSICION ==============
            posicion = BM_Bloque_Disponible(disco,sblock.s_bm_block_start,size);

        }

        fseek(disco, Calculo_Posicion_Block(sblock,posicion),SEEK_SET); //ME DESPLAZO HASTA EL COMIENZO DE LOS INODOS
        fwrite(&b_apuntador, sizeof(B_Apuntadores), 1 ,  disco);
        //===============   AHORA ACTUALIZO LOS BITMAP'S  ===================
        //Marcar_BitMap_I(sblock.s_bm_inode_start, size, disco, '1', posicion);
        Marcar_BitMap_B(sblock.s_bm_block_start,size, disco, '3', posicion);

        //===============   AHORA ACTUALIZO EL SUPERBLOQUE  ===================
        //Modificar_Count_I(&sblock, inicio, disco, 0,size);
        Modificar_Count_B(&sblock,inicio, disco, 0, size);

    }
    return posicion;
}


//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
//%%%%%%%%%%%%%%%%%%%%%%%       METODOS PARA ELIMINAR ARCHIVO       %%%%%%%%%%%%%%%%%
//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

void Eliminar_Archivo( FILE* disco, SuperBlock sblock, int inicio,int size ,char* ruta)
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

        Eliminar_Inodo_Archivo(disco,sblock,inicio,size,ruta,0);
        Aux_Eliminar_Inodo(disco,sblock,inicio,size,ruta,0);

    }else               //SI NO ES PORQUE ESTA DENTRO DE UNA CARPETA
    {
        auxRuta[0] = '/';

        for(int i=0; i<aux-1 ;i++)
        {
            if(i != 0)
                strcat(auxRuta,"/");

            strcat(auxRuta,nombres[i]);
        }
        int pos = Existe_Directorio(disco,sblock,size,ruta);    //RUTA FINAL
        int posApt = Existe_Directorio(disco,sblock,size,auxRuta);


        if(pos != -1)
        {
            Eliminar_Inodo_Archivo(disco,sblock,inicio,size,ruta,pos);

            Aux_Eliminar_Inodo(disco,sblock,inicio,size,ruta,posApt);

            //===============   AHORA ACTUALIZO LOS BITMAP'S  ===================
            Marcar_BitMap_I(sblock.s_bm_inode_start, size, disco, '0', pos);
            //Marcar_BitMap_B(sblock.s_bm_block_start,size, disco, '0', apt);

            //===============   AHORA ACTUALIZO EL SUPERBLOQUE  ===================
            Modificar_Count_I(&sblock, inicio, disco, 1,size);
            //Modificar_Count_B(&sblock,inicio, disco, 1, size);

        }
        else
        {
            Mensaje_Color("*** ERROR!! LA DIRECCION NO EXISTE PARA EL ARCHIVO ***\n",ROJO);
        }

    }
}


void Eliminar_Inodo_Archivo(FILE *disco, SuperBlock sblock, int inicio, int size,char* ruta,int pos)
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
        //if(inodo_aux.iblock[i] != -1 && inodo_aux.iblock[i] != 0)
        if(inodo_aux.iblock[i] > 0 && inodo_aux.iblock[i] < 16)
        {
            int posB = inodo_aux.iblock[i];

            if(Bm_bloque[posB] == '1')        //SI ES 1 ES UN BLOQUE CARPETA
            {
                Eliminar_BCarpeta_Archivo(disco,sblock,inicio,size,ruta, posB);

            }else if(Bm_bloque[posB] == '3')  //SI ES 3 ES UN BLOQUE APUNTADOR
            {
                Eliminar_BApuntador_Archivo(disco,sblock,size,inicio,ruta,posB);
            }
            //SI ES 2 ES UN BLOQUE CARPETA Ó MAS QUE TODO CUALQUIERA

            //===============   AHORA ACTUALIZO LOS BITMAP'S  ===================
            //Marcar_BitMap_I(sblock.s_bm_inode_start, size, disco, '1', posicion);
            Marcar_BitMap_B(sblock.s_bm_block_start,size, disco, '0', posB);

            //===============   AHORA ACTUALIZO EL SUPERBLOQUE  ===================
            //Modificar_Count_I(&sblock, inicio, disco, 0,size);
            Modificar_Count_B(&sblock,inicio, disco, 1, size);

        }
    }
}




void Eliminar_BCarpeta_Archivo(FILE *disco, SuperBlock sblock,int inicio, int size,char *ruta,int pos)
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

    B_Carpeta aux_carpeta;

    fseek(disco,Calculo_Posicion_Block(sblock,pos),SEEK_SET);
    fread(&aux_carpeta,sizeof(B_Carpeta),1,disco);

    for(int i=0; i<4 ;i++)
    {
        int apt = aux_carpeta.b_content[i].b_inodo;

        if(apt != -1 && strcmp(aux_carpeta.b_content[i].b_name,".") != 0 && strcmp(aux_carpeta.b_content[i].b_name,"..") != 0)
            //SI ES IGUAL ES PORQUE EL ESPACIO ESTA DIPONIBLES
        {
            Eliminar_Inodo_Archivo(disco,sblock,inicio,size,ruta,apt);


            //===============   AHORA ACTUALIZO LOS BITMAP'S  ===================
            Marcar_BitMap_I(sblock.s_bm_inode_start, size, disco, '0', apt);
            //Marcar_BitMap_B(sblock.s_bm_block_start,size, disco, '0', apt);

            //===============   AHORA ACTUALIZO EL SUPERBLOQUE  ===================
            Modificar_Count_I(&sblock, inicio, disco, 1,size);
            //Modificar_Count_B(&sblock,inicio, disco, 1, size);


        }

    }
}



void Eliminar_BApuntador_Archivo(FILE *disco, SuperBlock sblock,int size, int inicio, char *ruta,int pos)
{
    B_Apuntadores aux_apuntador;
    int n = Calcular_N(size);
    char Bm_bloque[3 * n];

    fseek(disco,Calculo_Posicion_Block(sblock,pos),SEEK_SET);
    fread(&aux_apuntador,sizeof(B_Apuntadores),1,disco);

    //==================== OBTENGO EL BM DE LOS INODOS ==============
    //fseek(disco,sblock.s_bm_inode_start,SEEK_SET);
    //fread(Bm_inodo,sizeof(char),n,disco);
    //==================== OBTENGO EL BM DE LOS BLOQUES =============
    fseek(disco,sblock.s_bm_block_start,SEEK_SET);
    fread(Bm_bloque,sizeof(char),3*n,disco);

    for(int i=0; i<16 ;i++)
    {
        int apt = aux_apuntador.b_pointers[i];

            if(apt != -1 )    //PRIMERO BUSCAREMOS EN LOS BLOQUES EXISTENTES
            {
                if( Bm_bloque[apt] == '1' ) //EN EL CASO DE BLOQUE CARPETA
                {
                    Eliminar_BCarpeta_Archivo(disco,sblock,inicio,size,ruta, apt);

                }else if(Bm_bloque[apt] == '3') //EN EL CASO DE BLOQUE APUNTADOR
                {
                    Eliminar_BApuntador_Archivo(disco,sblock,size,inicio,ruta,apt);
                }
                //EN EL CASO DE BLOQUE CARPETA Ó CUALQUIER OTRO

                //===============   AHORA ACTUALIZO LOS BITMAP'S  ===================
                //Marcar_BitMap_I(sblock.s_bm_inode_start, size, disco, '1', posicion);
                Marcar_BitMap_B(sblock.s_bm_block_start,size, disco, '0', apt);

                //===============   AHORA ACTUALIZO EL SUPERBLOQUE  ===================
                //Modificar_Count_I(&sblock, inicio, disco, 0,size);
                Modificar_Count_B(&sblock,inicio, disco, 1, size);

            }
    }
}


void Aux_Eliminar_Inodo(FILE *disco, SuperBlock sblock, int inicio, int size,char* ruta,int pos)
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
        //if(inodo_aux.iblock[i] != -1 && inodo_aux.iblock[i] != 0)
        if(inodo_aux.iblock[i] > 0 && inodo_aux.iblock[i] < 16)
        {
            int posB = inodo_aux.iblock[i];
            bool r = false;

            if(Bm_bloque[posB] == '1')        //SI ES 1 ES UN BLOQUE CARPETA
            {
                r = Aux_Eliminar_BCarpeta(disco,sblock,inicio,size,ruta, posB);

            }else if(Bm_bloque[posB] == '3')  //SI ES 3 ES UN BLOQUE APUNTADOR
            {
                r = Aux_Eliminar_BApuntador(disco,sblock,size,inicio,ruta,posB);
            }
            if(r)break;

        }
    }
}


bool Aux_Eliminar_BCarpeta(FILE *disco, SuperBlock sblock,int inicio, int size,char *ruta,int pos)
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

    B_Carpeta aux_carpeta;

    fseek(disco,Calculo_Posicion_Block(sblock,pos),SEEK_SET);
    fread(&aux_carpeta,sizeof(B_Carpeta),1,disco);

    for(int i=0; i<4 ;i++)
    {
        int apt = aux_carpeta.b_content[i].b_inodo;

        if(apt != -1 )//SI ES IGUAL ES PORQUE EL ESPACIO ESTA DIPONIBLES
        {
            if(strcmp(aux_carpeta.b_content[i].b_name, nombres[aux-1] ) == 0)
            {
                memset(aux_carpeta.b_content[i].b_name,'\0',sizeof(aux_carpeta.b_content[i].b_name));
                aux_carpeta.b_content[i].b_inodo = -1;

                fseek(disco,Calculo_Posicion_Block(sblock,pos),SEEK_SET);
                fwrite(&aux_carpeta,sizeof(B_Carpeta),1,disco);
                return true;
            }
        }

    }
    return false;
}



bool Aux_Eliminar_BApuntador(FILE *disco, SuperBlock sblock,int size, int inicio, char *ruta,int pos)
{
    B_Apuntadores aux_apuntador;
    int n = Calcular_N(size);
    char Bm_bloque[3 * n];

    fseek(disco,Calculo_Posicion_Block(sblock,pos),SEEK_SET);
    fread(&aux_apuntador,sizeof(B_Apuntadores),1,disco);

    //==================== OBTENGO EL BM DE LOS INODOS ==============
    //fseek(disco,sblock.s_bm_inode_start,SEEK_SET);
    //fread(Bm_inodo,sizeof(char),n,disco);
    //==================== OBTENGO EL BM DE LOS BLOQUES =============
    fseek(disco,sblock.s_bm_block_start,SEEK_SET);
    fread(Bm_bloque,sizeof(char),3*n,disco);

    for(int i=0; i<16 ;i++)
    {
        int apt = aux_apuntador.b_pointers[i];

            if(apt != -1 )    //PRIMERO BUSCAREMOS EN LOS BLOQUES EXISTENTES
            {
                bool r = false;
                if( Bm_bloque[apt] == '1' ) //EN EL CASO DE BLOQUE CARPETA
                {
                   r = Aux_Eliminar_BCarpeta(disco,sblock,inicio,size,ruta, apt);

                }else if(Bm_bloque[apt] == '3') //EN EL CASO DE BLOQUE APUNTADOR
                {
                   r =  Aux_Eliminar_BApuntador(disco,sblock,size,inicio,ruta,apt);
                }
                return r;
            }
    }
    return false;
}


//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
//%%%%%%%%%%%%%%%%%%%%%%%%%%      METODOS PARA ESCRIBIR       %%%%%%%%%%%%%%%%%%%%%%%
//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

void Escribir_Archivo( FILE* disco, SuperBlock sblock, int inicio,int size ,char* ruta,char*texto)
{
    int pos = Existe_Archivo(disco,sblock,size,ruta);

    if(pos != -1)
    {
        Escribir_Inodo_Archivo(disco,sblock,inicio,size,texto,pos);
    }
    else
    {
        Mensaje_Color("**** ERROR!! no existe el archivo para escribir!! ***\n",ROJO);
    }
}


void Escribir_Inodo_Archivo( FILE* disco, SuperBlock sblock, int inicio,int size ,char*texto, int posI)
{
    Inodo inodo_aux;
    int n = Calcular_N(size);
    char Bm_bloque[3 * n];

    //======================= OBTENGO EL INODO  =====================
    fseek(disco, Calculo_Posicion_Inodo(sblock,posI) ,SEEK_SET); //ME POSICIONO EN EL INODO
    fread(&inodo_aux,sizeof(Inodo),1,disco);

    Modificar_Inodo(disco,inodo_aux,  Calculo_Posicion_Inodo(sblock,posI) ,1);

    //==================== OBTENGO EL BM DE LOS BLOQUES =============
    fseek(disco,sblock.s_bm_block_start,SEEK_SET);
    fread(Bm_bloque,sizeof(char),3*n,disco);

    int index = 0;
    bool r = false;

    for(int i=0;i<16;i++)
    {
        int posB = inodo_aux.iblock[i];
        if(inodo_aux.iblock[i] != -1 )
        {

            if(Bm_bloque[posB] == '2')        //SI ES 2 ES UN BLOQUE CARPETA Ó MAS QUE TODO CUALQUIERA
            {
                r = Escribir_BArchivo(disco,sblock,texto, posB,&index);
                if(r)break;

            }else if(Bm_bloque[posB] == '3')  //SI ES 3 ES UN BLOQUE APUNTADOR
            {
                r = Escribir_BApuntador_Archivo(disco, sblock,size, inicio, texto,posB,&index);
                if(r) break;
            }
        }
        else        //SI ES -1 CREAMOS UN BLOQUE ARCHIVO
        {
            if(i == 13)//NIVEL 1
            {
                int posicion = Crear_BApuntador(disco, sblock,inicio,size,1);
                inodo_aux.iblock[i] = posicion;

                fseek(disco, Calculo_Posicion_Inodo(sblock,posI) ,SEEK_SET); //ME POSICIONO EN EL INODO
                fwrite(&inodo_aux,sizeof(Inodo),1,disco);

                r = Escribir_BApuntador_Archivo(disco, sblock,size, inicio, texto,posB,&index);
                if(r){break;}

            }else if(i == 14)// NIVEL 2
            {
                int posicion = Crear_BApuntador(disco, sblock,inicio,size,2);
                inodo_aux.iblock[i] = posicion;

                fseek(disco, Calculo_Posicion_Inodo(sblock,posI) ,SEEK_SET); //ME POSICIONO EN EL INODO
                fwrite(&inodo_aux,sizeof(Inodo),1,disco);

                r = Escribir_BApuntador_Archivo(disco, sblock,size, inicio, texto,posB,&index);
                if(r){break;}

            }else if(i == 15)// NIVEL 3
            {
                int posicion = Crear_BApuntador(disco, sblock,inicio,size,3);
                inodo_aux.iblock[i] = posicion;

                fseek(disco, Calculo_Posicion_Inodo(sblock,posI) ,SEEK_SET); //ME POSICIONO EN EL INODO
                fwrite(&inodo_aux,sizeof(Inodo),1,disco);

                r = Escribir_BApuntador_Archivo(disco, sblock,size, inicio, texto,posB,&index);
                if(r){break;}

            }
            else        //ESTE SERIA PARA UNA CARPETA NORMAL
            {
                //==========  CREO UN BLOQUE ARCHIVO Y ACTULIZO MI INODO ============
                int posicion = Crear_BArchivo(disco,sblock,inicio,size);
                inodo_aux.iblock[i] = posicion;

                fseek(disco, Calculo_Posicion_Inodo(sblock,posI) ,SEEK_SET); //ME POSICIONO EN EL INODO
                fwrite(&inodo_aux,sizeof(Inodo),1,disco);

                r =Escribir_BArchivo(disco,sblock,texto, posicion,&index);
                if(r){break;}
            }
        }
    }
}

bool Escribir_BArchivo(FILE* disco, SuperBlock sblock,char* texto, int posA, int* index)
{
    B_Archivo archivo_aux;
    char contenido[10000];
    memset(contenido,'\0',sizeof(contenido));
    strcat(contenido,texto);

    char r = false;

    //======================= OBTENGO EL BLOQUE  =====================
    fseek(disco, Calculo_Posicion_Block(sblock,posA) ,SEEK_SET); //ME POSICIONO EN EL INODO
    fread(&archivo_aux,sizeof(B_Archivo),1,disco);

    for(int i=0;i<64;i++)
    {
        if(archivo_aux.b_content[i] == '\0')
        {
            if(contenido[*index] != '\0')   //SI ES IGUAL ES PORQUE YA NO HAY MAS TEXTO QUE INGRESAR
            {
                archivo_aux.b_content[i] = contenido[(*index)++];
            }else{r = true; break;}
        }
    }

    //ACTUALIZO MI CONTENIDO
    fseek(disco, Calculo_Posicion_Block(sblock,posA) ,SEEK_SET); //ME POSICIONO EN EL INODO
    fwrite(&archivo_aux,sizeof(B_Archivo),1,disco);


    return r;
}


int Crear_BArchivo(FILE* disco, SuperBlock sblock, int inicio, int size)// CREO LOS BLOQUE ARCHIVO DE CUALQUIER NIVEL
{
    int posicion = BM_Bloque_Disponible(disco,sblock.s_bm_block_start,size);
    if(posicion != -1)
    {
        B_Archivo b_archivo;

        for(int i=0;i<64;i++)
            b_archivo.b_content[i] = '\0';


        fseek(disco, Calculo_Posicion_Block(sblock,posicion),SEEK_SET); //ME DESPLAZO HASTA EL COMIENZO DE LOS INODOS
        fwrite(&b_archivo, sizeof(B_Archivo), 1 ,  disco);
        //===============   AHORA ACTUALIZO LOS BITMAP'S  ===================
        //Marcar_BitMap_I(sblock.s_bm_inode_start, size, disco, '1', posicion);
        Marcar_BitMap_B(sblock.s_bm_block_start,size, disco, '2', posicion);

        //===============   AHORA ACTUALIZO EL SUPERBLOQUE  ===================
        //Modificar_Count_I(&sblock, inicio, disco, 0,size);
        Modificar_Count_B(&sblock,inicio, disco, 0, size);

    }
    return posicion;
}


bool Escribir_BApuntador_Archivo(FILE *disco, SuperBlock sblock,int size, int inicio, char *texto,int pos,int* index)
{
    B_Apuntadores aux_apuntador;
    bool r = false;
    int n = Calcular_N(size);
    char Bm_bloque[3 * n];

    fseek(disco,Calculo_Posicion_Block(sblock,pos),SEEK_SET);
    fread(&aux_apuntador,sizeof(B_Apuntadores),1,disco);

    //==================== OBTENGO EL BM DE LOS BLOQUES =============
    fseek(disco,sblock.s_bm_block_start,SEEK_SET);
    fread(Bm_bloque,sizeof(char),3*n,disco);

    for(int i=0; i<16 ;i++)
    {
        int apt = aux_apuntador.b_pointers[i];

            if(apt != -1 )    //PRIMERO BUSCAREMOS EN LOS BLOQUES EXISTENTES
            {
                if( Bm_bloque[apt] == '1' ) //EN EL CASO DE BLOQUE CARPETA
                {
                    r = Escribir_BArchivo(disco,sblock,texto,apt,index);
                    if(r)return true;

                }else if(Bm_bloque[apt] == '3') //EN EL CASO DE BLOQUE APUNTADOR
                {
                    r = Escribir_BApuntador_Archivo(disco,sblock,size,inicio,texto,apt,index);
                    if(r)return true;
                }

            }else   //SI ES IGUAL ES PORQUE EL ESPACIO ESTA DIPONIBLES
            {
                //==========  CREO UN BLOQUE CARPETA Y ACTULIZO MI INODO ============
                int posicion = Crear_BArchivo(disco,sblock,inicio,size);
                aux_apuntador.b_pointers[i] = posicion;

                fseek(disco, Calculo_Posicion_Block(sblock,pos) ,SEEK_SET); //ME POSICIONO EN EL INODO
                fwrite(&aux_apuntador,sizeof(B_Apuntadores),1,disco);

                r = Escribir_BArchivo(disco,sblock,texto,apt,index);
                if(r){break;}

            }
    }
    return r;
}



//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
//%%%%%%%%%%%%%%%%%%%%%%%%%%%%       METODOS PARA LEER       %%%%%%%%%%%%%%%%%%%%%%%%
//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

void Leer_Archivo( FILE* disco, SuperBlock sblock,int size ,char* contenido,char* ruta)
{
    //char contenido[10000];
    memset(contenido,'\0',sizeof(contenido));
    int pos = Existe_Archivo(disco,sblock,size,ruta);

    if(pos != -1)
    {
        Leer_Inodo_Archivo(disco,sblock,size,pos,contenido);
    }
    else
    {
        Mensaje_Color("**** ERROR!! no existe el archivo para leer!! ***\n",ROJO);
    }
}

char* Leer_Inodo_Archivo( FILE* disco, SuperBlock sblock,int size , int posI,char*contenido)
{
    Inodo inodo_aux;
    int n = Calcular_N(size);
    char Bm_bloque[3 * n];

    //======================= OBTENGO EL INODO  =====================
    fseek(disco, Calculo_Posicion_Inodo(sblock,posI) ,SEEK_SET); //ME POSICIONO EN EL INODO
    fread(&inodo_aux,sizeof(Inodo),1,disco);

    Modificar_Inodo(disco,inodo_aux,  Calculo_Posicion_Inodo(sblock,posI) ,1);

    //==================== OBTENGO EL BM DE LOS BLOQUES =============
    fseek(disco,sblock.s_bm_block_start,SEEK_SET);
    fread(Bm_bloque,sizeof(char),3*n,disco);

    int index = 0;
    bool r = false;
    //char contenido[10000];
    //memset(contenido,'\0',sizeof(contenido));

    for(int i=0;i<16;i++)
    {
        int posB = inodo_aux.iblock[i];
        if(inodo_aux.iblock[i] != -1 )
        {

            if(Bm_bloque[posB] == '2')        //SI ES 2 ES UN BLOQUE CARPETA Ó MAS QUE TODO CUALQUIERA
            {
                r = Leer_BArchivo(disco,sblock,contenido, posB,&index);
                if(r)break;

            }else if(Bm_bloque[posB] == '3')  //SI ES 3 ES UN BLOQUE APUNTADOR
            {
                r = Leer_BApuntador_Archivo(disco, sblock,size, contenido,posB,&index);
                if(r) break;
            }
        }
    }
    return contenido;
}

bool Leer_BArchivo(FILE* disco, SuperBlock sblock,char* contenido, int posA, int* index)
{
    B_Archivo archivo_aux;

    char r = false;

    //======================= OBTENGO EL BLOQUE  =====================
    fseek(disco, Calculo_Posicion_Block(sblock,posA) ,SEEK_SET); //ME POSICIONO EN EL INODO
    fread(&archivo_aux,sizeof(B_Archivo),1,disco);

    for(int i=0;i<64;i++)
    {
        if(archivo_aux.b_content[i] != '\0')
        {
            contenido[(*index)++]  = archivo_aux.b_content[i];   //SI ES IGUAL ES PORQUE YA NO HAY MAS TEXTO QUE INGRESAR

        }else{r = true; break;}
    }
    return r;
}


bool Leer_BApuntador_Archivo(FILE *disco, SuperBlock sblock,int size, char *contenido,int pos,int* index)
{
    B_Apuntadores aux_apuntador;
    bool r = false;
    int n = Calcular_N(size);
    char Bm_bloque[3 * n];

    fseek(disco,Calculo_Posicion_Block(sblock,pos),SEEK_SET);
    fread(&aux_apuntador,sizeof(B_Apuntadores),1,disco);

    //==================== OBTENGO EL BM DE LOS BLOQUES =============
    fseek(disco,sblock.s_bm_block_start,SEEK_SET);
    fread(Bm_bloque,sizeof(char),3*n,disco);

    for(int i=0; i<16 ;i++)
    {
        int apt = aux_apuntador.b_pointers[i];

            if(apt != -1 )    //PRIMERO BUSCAREMOS EN LOS BLOQUES EXISTENTES
            {
                if( Bm_bloque[apt] == '1' ) //EN EL CASO DE BLOQUE CARPETA
                {
                    r = Leer_BArchivo(disco,sblock,contenido,apt,index);
                    if(r)return true;

                }else if(Bm_bloque[apt] == '3') //EN EL CASO DE BLOQUE APUNTADOR
                {
                    r = Leer_BApuntador_Archivo(disco,sblock,size,contenido,apt,index);
                    if(r)return true;
                }
            }
    }
    return r;
}



//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
//%%%%%%%%%%%%%%%%%%%%%%%%%%       METODOS PARA EDITAR       %%%%%%%%%%%%%%%%%%%%%%%%
//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%


void Editar_Archivo(FILE* disco, SuperBlock sblock, int inicio, int size , char* ruta, char *texto)
{
    int pos = Existe_Archivo(disco,sblock,size,ruta);

    if(pos != -1)
    {
        Editar_Inodo_Archivo(disco,sblock,inicio,size,texto,pos);
    }
    else
    {
        Mensaje_Color("**** ERROR!! no existe el archivo para escribir!! ***\n",ROJO);
    }
}

void Editar_Inodo_Archivo(FILE* disco, SuperBlock sblock, int inicio, int size , char *texto, int posI)
{
    Inodo inodo_aux;
    int n = Calcular_N(size);
    char Bm_bloque[3 * n];

    //======================= OBTENGO EL INODO  =====================
    fseek(disco, Calculo_Posicion_Inodo(sblock,posI) ,SEEK_SET); //ME POSICIONO EN EL INODO
    fread(&inodo_aux,sizeof(Inodo),1,disco);

    Modificar_Inodo(disco,inodo_aux,  Calculo_Posicion_Inodo(sblock,posI) ,1);

    //==================== OBTENGO EL BM DE LOS BLOQUES =============
    fseek(disco,sblock.s_bm_block_start,SEEK_SET);
    fread(Bm_bloque,sizeof(char),3*n,disco);

    int index = 0;
    bool r = false;

    for(int i=0;i<16;i++)
    {
        int posB = inodo_aux.iblock[i];
        if(inodo_aux.iblock[i] > -1 && inodo_aux.iblock[i] < 16)
        {
            if(Bm_bloque[posB] == '2')        //SI ES 2 ES UN BLOQUE CARPETA Ó MAS QUE TODO CUALQUIERA
            {
                r = Editar_BArchivo(disco,sblock,texto, posB,&index);
                if(r)break;

            }else if(Bm_bloque[posB] == '3')  //SI ES 3 ES UN BLOQUE APUNTADOR
            {
                r = Editar_BApuntador_Archivo(disco, sblock,size, inicio, texto,posB,&index);
                if(r) break;
            }
        }
        else        //SI ES -1 CREAMOS UN BLOQUE ARCHIVO
        {
            if(i == 13)//NIVEL 1
            {
                int posicion = Crear_BApuntador(disco, sblock,inicio,size,1);
                inodo_aux.iblock[i] = posicion;

                fseek(disco, Calculo_Posicion_Inodo(sblock,posI) ,SEEK_SET); //ME POSICIONO EN EL INODO
                fwrite(&inodo_aux,sizeof(Inodo),1,disco);

                r = Editar_BApuntador_Archivo(disco, sblock,size, inicio, texto,posB,&index);
                if(r){break;}

            }else if(i == 14)// NIVEL 2
            {
                int posicion = Crear_BApuntador(disco, sblock,inicio,size,2);
                inodo_aux.iblock[i] = posicion;

                fseek(disco, Calculo_Posicion_Inodo(sblock,posI) ,SEEK_SET); //ME POSICIONO EN EL INODO
                fwrite(&inodo_aux,sizeof(Inodo),1,disco);

                r = Editar_BApuntador_Archivo(disco, sblock,size, inicio, texto,posB,&index);
                if(r){break;}

            }else if(i == 15)// NIVEL 3
            {
                int posicion = Crear_BApuntador(disco, sblock,inicio,size,3);
                inodo_aux.iblock[i] = posicion;

                fseek(disco, Calculo_Posicion_Inodo(sblock,posI) ,SEEK_SET); //ME POSICIONO EN EL INODO
                fwrite(&inodo_aux,sizeof(Inodo),1,disco);

                r = Editar_BApuntador_Archivo(disco, sblock,size, inicio, texto,posB,&index);
                if(r){break;}

            }
            else        //ESTE SERIA PARA UNA CARPETA NORMAL
            {
                //==========  CREO UN BLOQUE ARCHIVO Y ACTULIZO MI INODO ============
                int posicion = Crear_BArchivo(disco,sblock,inicio,size);
                inodo_aux.iblock[i] = posicion;

                fseek(disco, Calculo_Posicion_Inodo(sblock,posI) ,SEEK_SET); //ME POSICIONO EN EL INODO
                fwrite(&inodo_aux,sizeof(Inodo),1,disco);

                r =Editar_BArchivo(disco,sblock,texto, posicion,&index);
                if(r){break;}
            }
        }
    }
}

bool Editar_BArchivo(FILE* disco, SuperBlock sblock,char* texto, int posA, int* index)
{
    B_Archivo archivo_aux;
    char contenido[10000];
    memset(contenido,'\0',sizeof(contenido));
    strcat(contenido,texto);

    char r = false;

    //======================= OBTENGO EL BLOQUE  =====================
    fseek(disco, Calculo_Posicion_Block(sblock,posA) ,SEEK_SET); //ME POSICIONO EN EL INODO
    fread(&archivo_aux,sizeof(B_Archivo),1,disco);

    for(int i=0;i<64;i++)
    {
        if(contenido[*index] != '\0')   //SI ES IGUAL ES PORQUE YA NO HAY MAS TEXTO QUE INGRESAR
        {
            archivo_aux.b_content[i] = contenido[(*index)++];
        }else{r = true; break;}
    }

    //ACTUALIZO MI CONTENIDO
    fseek(disco, Calculo_Posicion_Block(sblock,posA) ,SEEK_SET); //ME POSICIONO EN EL INODO
    fwrite(&archivo_aux,sizeof(B_Archivo),1,disco);

    return r;
}

bool Editar_BApuntador_Archivo(FILE *disco, SuperBlock sblock,int size, int inicio, char *texto,int pos,int* index)
{
    B_Apuntadores aux_apuntador;
    bool r = false;
    int n = Calcular_N(size);
    char Bm_bloque[3 * n];

    fseek(disco,Calculo_Posicion_Block(sblock,pos),SEEK_SET);
    fread(&aux_apuntador,sizeof(B_Apuntadores),1,disco);

    //==================== OBTENGO EL BM DE LOS BLOQUES =============
    fseek(disco,sblock.s_bm_block_start,SEEK_SET);
    fread(Bm_bloque,sizeof(char),3*n,disco);

    for(int i=0; i<16 ;i++)
    {
        int apt = aux_apuntador.b_pointers[i];

        if(apt != -1 )    //PRIMERO BUSCAREMOS EN LOS BLOQUES EXISTENTES
        {
            if( Bm_bloque[apt] == '1' ) //EN EL CASO DE BLOQUE CARPETA
            {
                r = Editar_BArchivo(disco,sblock,texto,apt,index);
                if(r)return true;

            }else if(Bm_bloque[apt] == '3') //EN EL CASO DE BLOQUE APUNTADOR
            {
                r = Editar_BApuntador_Archivo(disco,sblock,size,inicio,texto,apt,index);
                if(r)return true;
            }

        }else   //SI ES IGUAL ES PORQUE EL ESPACIO ESTA DIPONIBLES
        {
            //==========  CREO UN BLOQUE CARPETA Y ACTULIZO MI INODO ============
            int posicion = Crear_BArchivo(disco,sblock,inicio,size);
            aux_apuntador.b_pointers[i] = posicion;

            fseek(disco, Calculo_Posicion_Block(sblock,pos) ,SEEK_SET); //ME POSICIONO EN EL INODO
            fwrite(&aux_apuntador,sizeof(B_Apuntadores),1,disco);

            r = Editar_BArchivo(disco,sblock,texto,apt,index);
            if(r){break;}

        }
    }
    return r;
}


//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
//%%%%%%%%%%%%%%%%%%%%%%%%%%       METODOS PARA BORRAR       %%%%%%%%%%%%%%%%%%%%%%%%
//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%


void Borrar_Contenido_Archivo(FILE* disco, SuperBlock sblock, int inicio, int size , char* ruta, int *cantidad)
{
    int pos = Existe_Archivo(disco,sblock,size,ruta);

    if(pos != -1)
    {
        Borrar_Inodo_Archivo(disco,sblock,inicio,size,cantidad,pos);
    }
    else
    {
        Mensaje_Color("**** ERROR!! no existe el archivo para escribir!! ***\n",ROJO);
    }
}

void Borrar_Inodo_Archivo(FILE* disco, SuperBlock sblock, int inicio, int size , int *cantidad, int posI)
{
    Inodo inodo_aux;
    int n = Calcular_N(size);
    char Bm_bloque[3 * n];

    //======================= OBTENGO EL INODO  =====================
    fseek(disco, Calculo_Posicion_Inodo(sblock,posI) ,SEEK_SET); //ME POSICIONO EN EL INODO
    fread(&inodo_aux,sizeof(Inodo),1,disco);

    Modificar_Inodo(disco,inodo_aux,  Calculo_Posicion_Inodo(sblock,posI) ,1);

    //==================== OBTENGO EL BM DE LOS BLOQUES =============
    fseek(disco,sblock.s_bm_block_start,SEEK_SET);
    fread(Bm_bloque,sizeof(char),3*n,disco);

    int index = 0;
    bool r = false;

    for(int i=0;i<16;i++)
    {
        int posB = inodo_aux.iblock[i];
        if(inodo_aux.iblock[i] > -1 && inodo_aux.iblock[i] < 16)
        {
            if(Bm_bloque[posB] == '2')        //SI ES 2 ES UN BLOQUE CARPETA Ó MAS QUE TODO CUALQUIERA
            {
                r = Borrar_BArchivo(disco,sblock,cantidad, posB,&index);
                if(r)break;

            }else if(Bm_bloque[posB] == '3')  //SI ES 3 ES UN BLOQUE APUNTADOR
            {
                r = Borrar_BApuntador_Archivo(disco, sblock,size, inicio, cantidad,posB,&index);
                if(r) break;
            }
        }
        else        //SI ES -1 CREAMOS UN BLOQUE ARCHIVO
        {
            if(i == 13)//NIVEL 1
            {
                int posicion = Crear_BApuntador(disco, sblock,inicio,size,1);
                inodo_aux.iblock[i] = posicion;

                fseek(disco, Calculo_Posicion_Inodo(sblock,posI) ,SEEK_SET); //ME POSICIONO EN EL INODO
                fwrite(&inodo_aux,sizeof(Inodo),1,disco);

                r = Borrar_BApuntador_Archivo(disco, sblock,size, inicio, cantidad,posB,&index);
                if(r){break;}

            }else if(i == 14)// NIVEL 2
            {
                int posicion = Crear_BApuntador(disco, sblock,inicio,size,2);
                inodo_aux.iblock[i] = posicion;

                fseek(disco, Calculo_Posicion_Inodo(sblock,posI) ,SEEK_SET); //ME POSICIONO EN EL INODO
                fwrite(&inodo_aux,sizeof(Inodo),1,disco);

                r = Borrar_BApuntador_Archivo(disco, sblock,size, inicio, cantidad,posB,&index);
                if(r){break;}

            }else if(i == 15)// NIVEL 3
            {
                int posicion = Crear_BApuntador(disco, sblock,inicio,size,3);
                inodo_aux.iblock[i] = posicion;

                fseek(disco, Calculo_Posicion_Inodo(sblock,posI) ,SEEK_SET); //ME POSICIONO EN EL INODO
                fwrite(&inodo_aux,sizeof(Inodo),1,disco);

                r = Borrar_BApuntador_Archivo(disco, sblock,size, inicio, cantidad,posB,&index);
                if(r){break;}

            }
            else        //ESTE SERIA PARA UNA CARPETA NORMAL
            {
                //==========  CREO UN BLOQUE ARCHIVO Y ACTULIZO MI INODO ============
                int posicion = Crear_BArchivo(disco,sblock,inicio,size);
                inodo_aux.iblock[i] = posicion;

                fseek(disco, Calculo_Posicion_Inodo(sblock,posI) ,SEEK_SET); //ME POSICIONO EN EL INODO
                fwrite(&inodo_aux,sizeof(Inodo),1,disco);

                r =Borrar_BArchivo(disco,sblock,cantidad, posicion,&index);
                if(r){break;}
            }
        }
    }
}

bool Borrar_BArchivo(FILE* disco, SuperBlock sblock, int *cantidad, int posA, int* index)
{
    B_Archivo archivo_aux;

    char r = false;

    //======================= OBTENGO EL BLOQUE  =====================
    fseek(disco, Calculo_Posicion_Block(sblock,posA) ,SEEK_SET); //ME POSICIONO EN EL INODO
    fread(&archivo_aux,sizeof(B_Archivo),1,disco);

    for(int i=0;i<64;i++)
    {
        if( (*cantidad) != 0)   //SI ES IGUAL ES PORQUE YA NO HAY MAS TEXTO QUE INGRESAR
        {
            archivo_aux.b_content[i] = '\0';
            (*cantidad)--;
        }else{r = true; break;}
    }

    //ACTUALIZO MI CONTENIDO
    fseek(disco, Calculo_Posicion_Block(sblock,posA) ,SEEK_SET); //ME POSICIONO EN EL INODO
    fwrite(&archivo_aux,sizeof(B_Archivo),1,disco);

    return r;
}

bool Borrar_BApuntador_Archivo(FILE *disco, SuperBlock sblock,int size, int inicio, int *cantidad,int pos,int* index)
{
    B_Apuntadores aux_apuntador;
    bool r = false;
    int n = Calcular_N(size);
    char Bm_bloque[3 * n];

    fseek(disco,Calculo_Posicion_Block(sblock,pos),SEEK_SET);
    fread(&aux_apuntador,sizeof(B_Apuntadores),1,disco);

    //==================== OBTENGO EL BM DE LOS BLOQUES =============
    fseek(disco,sblock.s_bm_block_start,SEEK_SET);
    fread(Bm_bloque,sizeof(char),3*n,disco);

    for(int i=0; i<16 ;i++)
    {
        int apt = aux_apuntador.b_pointers[i];

        if(apt != -1 )    //PRIMERO BUSCAREMOS EN LOS BLOQUES EXISTENTES
        {
            if( Bm_bloque[apt] == '1' ) //EN EL CASO DE BLOQUE CARPETA
            {
                r = Borrar_BArchivo(disco,sblock,cantidad,apt,index);
                if(r)return true;

            }else if(Bm_bloque[apt] == '3') //EN EL CASO DE BLOQUE APUNTADOR
            {
                r = Borrar_BApuntador_Archivo(disco,sblock,size,inicio,cantidad,apt,index);
                if(r)return true;
            }

        }else   //SI ES IGUAL ES PORQUE EL ESPACIO ESTA DIPONIBLES
        {
            //==========  CREO UN BLOQUE CARPETA Y ACTULIZO MI INODO ============
            int posicion = Crear_BArchivo(disco,sblock,inicio,size);
            aux_apuntador.b_pointers[i] = posicion;

            fseek(disco, Calculo_Posicion_Block(sblock,pos) ,SEEK_SET); //ME POSICIONO EN EL INODO
            fwrite(&aux_apuntador,sizeof(B_Apuntadores),1,disco);

            r = Borrar_BArchivo(disco,sblock,cantidad,apt,index);
            if(r){break;}

        }
    }
    return r;
}













#include "Mkfs.h"

void Analizar_Mkfs(char* comando, int tamanio)
{
    //Limpiar_Variables();
    Limpiar_Datos();
    caracteristicas.deleteP = 'u';

    char pivote[1000];
    memset(pivote,'\0',sizeof(pivote));


    int pos =0;
    while(comando[tamanio] != '\0')
    {

        if(comando[tamanio] == '~'){//ENTRA EN EL ~:~ PARA VER EL VALOR

            if(comando[tamanio+1] == ':' && comando[tamanio+2] == '~'){

                tamanio+=3;
                if(strcasecmp(pivote,"-id") == 0){

                    Analizar_Id(comando,&tamanio);

                }else if(strcasecmp(pivote,"-type") == 0){

                    Analizar_Delete(comando,&tamanio);

                }else if(strcasecmp(pivote,"-fs") == 0){

                    Analizar_Fs(comando,&tamanio);

                }else{
                    //printf("\t*** Error!! Caracteristica MKFS \"%s\" no reconocida ****\n",pivote);
                    memset(mensaje,'\0',sizeof(mensaje));
                    sprintf(mensaje,"\t*** Error!! Caracteristica MKFS \"%s\" no reconocida ****\n",pivote);
                    Mensaje_Color(mensaje,ROJO);
                }

                memset(pivote,'\0',sizeof(pivote));
                pos = 0;

            }else{//EN TEORIA ESTE CASO PASARA SI NO SE CUMPLE LO COMETIDO ~:~
                //printf("\t*** ERROR!! la cadena \"~:~\"  esta incompleta ***\n");
                Mensaje_Color("\t*** ERROR!! la cadena \"~:~\"  esta incompleta ***\n",ROJO);
                break;
            }


        }else if(comando[tamanio] == '#'){//SE ENCUENTRA CON EL INICIO DE UN COMENTARIO
            if(tamanio == 0)//SE LO ENCUENTRA AL INICIO DEL COMANDO
                return;
            //=============== AQUI VA EL METODO DE VALIDACION ==============
            //Estado_Mkfs();
            Validar_Mkfs();

            return;
        }else{
            pivote[pos] = comando[tamanio];
            pos++;
        }

        tamanio++;
    }

    //Estado_Mkfs();
    Validar_Mkfs();
}

void Validar_Mkfs()
{
    if(strcasecmp(caracteristicas.id,"nel") != 0)//COMPRUEBA SI TIENE UN NAME GUARDADO
    {
        Formatear_Particion();

    }else
    {
        //printf("*** ERROR!! Falta Parametro obligatorio \"-id\" ***\n");
        Mensaje_Color("*** ERROR!! Falta Parametro obligatorio \"-id\" ***\n",ROJO);
    }
}


void Formatear_Particion()
{
    int parseo = -1;
    if(Validar_Numero(&caracteristicas.id[3])){
        parseo = atoi(&caracteristicas.id[3]);
        int valor = Get_Inicio(caracteristicas.id[2],parseo);
        if(valor != -1)
        {
            char* ruta = Get_Path(caracteristicas.id[2],parseo);

            if(caracteristicas.deleteP == 'a')
            {
                Formateo_Fast(ruta);
                //printf("=== El Formateo fast a sido realizado exitosamente!! ===\n");
                Mensaje_Color("=== El Formateo fast a sido realizado exitosamente!! ===\n",CYAN);
            }else if(caracteristicas.deleteP == 'u')
            {
                Formateo_Full(ruta);

                Mensaje_Color("=== El Formateo full a sido realizado exitosamente!! ===\n",CYAN);
            }


        }else
        {
            //printf("*** ERROR!! No existe un particion montada con este id ***\n");
            Mensaje_Color("*** ERROR!! No existe un particion montada con este id ***\n",ROJO);
        }
    }else
    {
        printf("*** ERROR!! Dato no compatible ---> ***\n");
        Mensaje_Color("*** ERROR!! Dato no compatible ---> ***\n",ROJO);
    }
}


void Formateo_Fast(char* ruta)
{
    FILE* disco;

    disco = fopen(ruta,"r+b");

    if(disco == NULL){
        printf("*** ERROR!! Al abrir el disco ***\n");
        return;
    }

    Nodo* datos_particion = Get_Mount(ruta);

    SuperBlock sblock;
    fseek(disco,datos_particion->inicio,SEEK_SET);
    fread(&sblock,sizeof(SuperBlock),1,disco);//SE LEE EL DISCO CON EL TAMAÑO DEL SUPERBLOQUE

    int n = Calcular_N(datos_particion->size);

    if(sblock.s_block_size == 0)//SI ES VERDADERO NO HAY SUPER BLOCK, SI ES FALSE ES PORQUE YA EXITE SUPER BLOCK
    {

        LLenar_SBlock(&sblock, n,datos_particion->inicio);

        char bitmap_inodo[n];       //BITMAP DE INODOS
        memset(bitmap_inodo,'0',sizeof(bitmap_inodo));
        char bitmap_bloque[3*n];    //BITMAP DE BLOQUES
        memset(bitmap_bloque,'0',sizeof(bitmap_bloque));

        //LLENAR LA PARTICION CON LOS INODOS, BLOQUES Y LOS BITMAP'S
        fseek(disco,datos_particion->inicio,SEEK_SET);
        fwrite(&sblock,sizeof(SuperBlock),1,disco);

        fseek(disco,sblock.s_bm_inode_start,SEEK_SET);
        fwrite(bitmap_inodo,sizeof(bitmap_inodo),1,disco);

        fseek(disco,sblock.s_bm_block_start,SEEK_SET);
        fwrite(bitmap_bloque,sizeof(bitmap_bloque),1,disco);

    }else
    {
        sblock.s_first_blo = 0;
        sblock.s_first_ino = 0;
        sblock.s_free_blocks_count = 3 * n * sizeof(B_Carpeta);
        sblock.s_free_inodes_count = n * sizeof(Inodo);


        char bitmap_inodo[n];       //BITMAP DE INODOS
        memset(bitmap_inodo,'0',sizeof(bitmap_inodo));
        char bitmap_bloque[3*n];    //BITMAP DE BLOQUES
        memset(bitmap_bloque,'0',sizeof(bitmap_bloque));


        //LLENAR LA PARTICION CON LOS INODOS, BLOQUES Y LOS BITMAP'S
        fseek(disco,datos_particion->inicio,SEEK_SET);
        fwrite(&sblock,sizeof(SuperBlock),1,disco);

        fseek(disco,sblock.s_bm_inode_start,SEEK_SET);
        fwrite(bitmap_inodo,sizeof(bitmap_inodo),1,disco);

        fseek(disco,sblock.s_bm_block_start,SEEK_SET);
        fwrite(bitmap_bloque,sizeof(bitmap_bloque),1,disco);

    }

    fclose(disco);

    Crear_Raiz(ruta,sblock,datos_particion);//AQUI CREO LA CARPETA RAIZ (/)
}


void Formateo_Full(char* ruta)
{
    FILE* disco;

    disco = fopen(ruta,"r+b");

    if(disco == NULL){
        printf("*** ERROR!! Al abrir el disco ***\n");
        return;
    }

    Nodo* datos_particion = Get_Mount(ruta);

    SuperBlock sblock;
    fseek(disco,datos_particion->inicio,SEEK_SET);
    fread(&sblock,sizeof(SuperBlock),1,disco);//SE LEE EL DISCO CON EL TAMAÑO DEL SUPERBLOQUE

    int n = Calcular_N(datos_particion->size);

    if(sblock.s_block_size == 0)//SI ES VERDADERO NO HAY SUPER BLOCK, SI ES FALSE ES PORQUE YA EXITE SUPER BLOCK
    {

        LLenar_SBlock(&sblock, n,datos_particion->inicio);

        char bitmap_inodo[n];       //BITMAP DE INODOS
        memset(bitmap_inodo,'0',sizeof(bitmap_inodo));
        char bitmap_bloque[3*n];    //BITMAP DE BLOQUES
        memset(bitmap_bloque,'0',sizeof(bitmap_bloque));

        //LLENAR LA PARTICION CON LOS INODOS, BLOQUES Y LOS BITMAP'S
        fseek(disco,datos_particion->inicio,SEEK_SET);
        fwrite(&sblock,sizeof(SuperBlock),1,disco);

        fseek(disco,sblock.s_bm_inode_start,SEEK_SET);
        fwrite(bitmap_inodo,sizeof(bitmap_inodo),1,disco);

        fseek(disco,sblock.s_bm_block_start,SEEK_SET);
        fwrite(bitmap_bloque,sizeof(bitmap_bloque),1,disco);

    }else
    {
        sblock.s_first_blo = 0;
        sblock.s_first_ino = 0;
//        sblock.s_free_blocks_count = 3 * n * sizeof(B_Carpeta);
//        sblock.s_free_inodes_count = n * sizeof(Inodo);
        sblock.s_free_blocks_count = 3 * n ;
        sblock.s_free_inodes_count = n ;


        char bitmap_inodo[n];       //BITMAP DE INODOS
        memset(bitmap_inodo,'0',sizeof(bitmap_inodo));
        char bitmap_bloque[3*n];    //BITMAP DE BLOQUES
        memset(bitmap_bloque,'0',sizeof(bitmap_bloque));


        //LLENAR LA PARTICION CON LOS INODOS, BLOQUES Y LOS BITMAP'S
        fseek(disco,datos_particion->inicio,SEEK_SET);
        fwrite(&sblock,sizeof(SuperBlock),1,disco);

        fseek(disco,sblock.s_bm_inode_start,SEEK_SET);
        fwrite(bitmap_inodo,sizeof(bitmap_inodo),1,disco);

        fseek(disco,sblock.s_bm_block_start,SEEK_SET);
        fwrite(bitmap_bloque,sizeof(bitmap_bloque),1,disco);

        //LIMPIAR LAS SECCIONES DE INODOS Y BLOQUES
        char inodo[n*sizeof(Inodo)];
        memset(inodo,'\0',sizeof(inodo));
        fseek(disco,sblock.s_inode_start,SEEK_SET);
        fwrite(inodo,sizeof(inodo),1,disco);

        char block[3*n*sizeof(B_Carpeta)];
        memset(block,'\0',sizeof(block));
        fseek(disco,sblock.s_block_start,SEEK_SET);
        fwrite(block,sizeof(block),1,disco);

    }

    fclose(disco);

    Crear_Raiz(ruta,sblock,datos_particion);//AQUI CREO LA CARPETA RAIZ (/)

}





void LLenar_SBlock(SuperBlock* sblock,int n,int inicio)
{
    sblock->s_filesystem_type = 3;
//    sblock->s_inodes_count = n * sizeof(Inodo);
//    sblock->s_blocks_count = 3 * n * sizeof(B_Carpeta);
//    sblock->s_free_blocks_count = 3 * n * sizeof(B_Carpeta);
//    sblock->s_free_inodes_count = n * sizeof(Inodo);
    sblock->s_inodes_count = n ;
    sblock->s_blocks_count = 3 * n ;
    sblock->s_free_blocks_count = 3 * n ;
    sblock->s_free_inodes_count = n ;

    memset(sblock->s_mtime,'\0',sizeof(sblock->s_mtime));
    Obtener_Fecha(sblock->s_mtime);

    memset(sblock->s_umtime,'\0',sizeof(sblock->s_umtime));
    Obtener_Fecha(sblock->s_umtime);

    sblock->s_mnt_count = sblock->s_mnt_count +1;//INDICA CUANTAS VECES SE HA MONTADO EL SISTEMA
    sblock->s_magic = 61267 ;//VALOR QUE IDENTIFICA AL SISTEMA DE ARCHIVOS, TENDRÁ EL VALOR 0XEF53
    sblock->s_inodes_size = sizeof(Inodo);//TAMANIO DEL INODO
    sblock->s_block_size = sizeof(B_Carpeta);//TAMAÑO DEL BLOQUE
    sblock->s_first_ino = 0;//PRIMER INODO LIBRE
    sblock->s_first_blo = 0;//PRIMER BLOQUE LIBRE
    sblock->s_bm_inode_start = inicio + sizeof(SuperBlock)+ 1 + (n * sizeof(Journal)) +1;//GUARDARÁ EL INICIO DEL BITMAP DE INODOS
    sblock->s_bm_block_start = sblock->s_bm_inode_start + n + 1 ;//GUARDARÁ EL INICIO DEL BITMAP DE BLOQUES
    sblock->s_inode_start = sblock->s_bm_block_start + (3*n) + 1 ;//GUARDARÁ EL INICIO DE LA TABLA DE INODOS
    sblock->s_block_start = sblock->s_inode_start + n * sizeof(Inodo) +1 ;//GUARDARÁ EL INICO DE LA TABLA DE BLOQUES
}



void Crear_Raiz(char* ruta, SuperBlock sblock , Nodo* datos_particion )
{
    FILE* disco;

    disco = fopen(ruta,"r+b");

    if(disco == NULL){
        printf("*** ERROR!! Al abrir el disco ***\n");
        return;
    }

    //============ PRIMERO VOY A CREAR EL INODO RAIZ =================
    Inodo inodo;
    inodo.i_uid = 1;//UID DEL USUARIO PROPIETARIO DEL ARCHIVO O CARPETA
    inodo.l_gid = 1;//GID DEL GRUPO AL QUE PERTENECE EL ARCHIVO O CARPETA.
    inodo.i_size = 0;//TAMAÑO DEL ARCHIVO EN BYTES
    Obtener_Fecha(inodo.i_atime);//ÚLTIMA FECHA EN QUE SE LEYÓ EL INODO SIN MODIFICARLO
    Obtener_Fecha(inodo.i_ctime);//FECHA EN LA QUE SE CREÓ EL INODO
    Obtener_Fecha(inodo.i_mtime);//ÚLTIMA FECHA EN LA QUE SE MODIFICÓ EL INODO
    //iblock[15];//ARRAY EN LOS QUE LOS PRIMEROS 12 REGISTROS SON BLOQUES DIRECTOS. 13 SIMPLE, 14 DOBLE Y 15 TRIPLE INDIRECTO
    inodo.i_type = '0';//INDICA SI ES ARCHIVO O CARPETA. TENDRÁ LOS SIGUIENTES VALORES: 1 = ARCHIVO 0 = CARPETA
    inodo.i_perm = 777;//GUARDARÁ LOS PERMISOS DEL ARCHIVO O CARPETA

//    for(int i=0; i<15 ;i++)
//    {
//        inodo.iblock[i] = -1;
//    }

    memset(inodo.iblock,-1,sizeof(inodo.iblock));

    inodo.iblock[0] = 0;

    fseek(disco, Calculo_Posicion_Inodo(sblock,0),SEEK_SET); //ME DESPLAZO HASTA EL COMIENZO DE LOS INODOS
    fwrite(&inodo, sizeof(Inodo), 1 ,  disco);


    //============ CREO EL BLOQUE_CARPETA PARA LA RAIZ ==============
    B_Carpeta b_carpeta;

    for(int i=0; i<15 ;i++)
        b_carpeta.b_content[i].b_inodo = -1;


    memset(b_carpeta.b_content[0].b_name,'\0',sizeof(b_carpeta.b_content[0].b_name));
    strcpy(b_carpeta.b_content[0].b_name, ".");     //LE COLOCO EL NOMBRE
    b_carpeta.b_content[0].b_inodo = 0;

    memset(b_carpeta.b_content[1].b_name,'\0',sizeof(b_carpeta.b_content[1].b_name));
    strcpy(b_carpeta.b_content[1].b_name, "..");    //LE COLOCO EL NOMBRE
    b_carpeta.b_content[1].b_inodo = 0;


    fseek(disco, Calculo_Posicion_Block(sblock,0) ,SEEK_SET); //ME DESPLAZO HASTA EL COMIENZO DE LOS BLOQUES
    fwrite(&b_carpeta, sizeof(B_Carpeta), 1 ,  disco);


    //===============   AHORA ACTUALIZO LOS BITMAP'S  ===================
    Marcar_BitMap_I(sblock.s_bm_inode_start,datos_particion->size, disco, '1', 0);
    Marcar_BitMap_B(sblock.s_bm_block_start,datos_particion->size, disco, '1', 0);

    //===============   AHORA ACTUALIZO EL SUPERBLOQUE  ===================
    Modificar_Count_I(&sblock,datos_particion->inicio, disco, 0,datos_particion->size);
    Modificar_Count_B(&sblock,datos_particion->inicio, disco, 0, datos_particion->size);


    //%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%  CREO UN NUEVO INODO PARA USERS.TXT  %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

    //============ AHORA CREO EL INODO PARA EL .txt =================
    Inodo inodoA;
    inodoA.i_uid = 1;//UID DEL USUARIO PROPIETARIO DEL ARCHIVO O CARPETA
    inodoA.l_gid = 1;//GID DEL GRUPO AL QUE PERTENECE EL ARCHIVO O CARPETA.
    inodoA.i_size = 52;//TAMAÑO DEL ARCHIVO EN BYTES
    Obtener_Fecha(inodoA.i_atime);//ÚLTIMA FECHA EN QUE SE LEYÓ EL INODO SIN MODIFICARLO
    Obtener_Fecha(inodoA.i_ctime);//FECHA EN LA QUE SE CREÓ EL INODO
    Obtener_Fecha(inodoA.i_mtime);//ÚLTIMA FECHA EN LA QUE SE MODIFICÓ EL INODO
    //iblock[15];//ARRAY EN LOS QUE LOS PRIMEROS 12 REGISTROS SON BLOQUES DIRECTOS. 13 SIMPLE, 14 DOBLE Y 15 TRIPLE INDIRECTO
    inodoA.i_type = '1';//INDICA SI ES ARCHIVO O CARPETA. TENDRÁ LOS SIGUIENTES VALORES: 1 = ARCHIVO 0 = CARPETA
    inodoA.i_perm = 777;//GUARDARÁ LOS PERMISOS DEL ARCHIVO O CARPETA

//    for(int i=0; i<15 ;i++)
//        inodoA.iblock[i] = -1;
    memset(inodoA.iblock,-1,sizeof(inodoA.iblock));


    inodoA.iblock[0] = 1; //EN TEORIA ES DONDE COMIENZA LOS BLOQUES, PORQUE AHI PONDRE LAS CARPETA


    fseek(disco, Calculo_Posicion_Inodo(sblock,1) ,SEEK_SET);
    fwrite(&inodoA,sizeof(Inodo),1,disco);//ESCRIBO EL INODO ASOCIADO AL BLOQUE ARCHIVO


    //===================== ACTUALIZO BLOQUE INODOS ==================
    Marcar_BitMap_I(sblock.s_bm_inode_start,datos_particion->size, disco, '1', 1);
    Modificar_Count_I(&sblock,datos_particion->inicio,disco,0,datos_particion->size);


    //=========== ENLAZO NUEVO INODO CON BLOQUE ARCHIVO ===============
    memset(b_carpeta.b_content[2].b_name,'\0',sizeof(b_carpeta.b_content[0].b_name));
    strcpy(b_carpeta.b_content[2].b_name, "users.txt");     //LE COLOCO EL NOMBRE
    b_carpeta.b_content[2].b_inodo = 1;             //LE COLOCO EL APUNTADOR DEL NUEVO INODO

    fseek(disco, Calculo_Posicion_Block(sblock,0) ,SEEK_SET); //ME DESPLAZO HASTA AL PRIMER BLOQUE PARA ACTUALIZARLO
    fwrite(&b_carpeta, sizeof(B_Carpeta), 1 ,  disco);


    //===================== CREO EL BLOQUE ARCHIVO ==================
    B_Archivo b_archivo;
    memset(b_archivo.b_content,'\0',sizeof(b_archivo.b_content));

    strcpy(b_archivo.b_content , "1,G,root      \n1,U,root      ,root      ,123       \n");


    fseek(disco, Calculo_Posicion_Block(sblock,1) ,SEEK_SET); //ME DESPLAZO HASTA EL BLOQUE DISPONIBLE
    fwrite(&b_archivo, sizeof(B_Archivo), 1 ,  disco);


    //===================== ACTUALIZO BLOQUES ARCHIVO==================
    Marcar_BitMap_B(sblock.s_bm_block_start,datos_particion->size, disco, '2', 1);
    Modificar_Count_B(&sblock,datos_particion->inicio, disco, 0, datos_particion->size);

    fclose(disco);
}



































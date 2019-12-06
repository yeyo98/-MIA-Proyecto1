#include "Rep.h"

void Analizar_Rep(char* comando, int tamanio)
{
    Limpiar_Datos();

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

                }else if(strcasecmp(pivote,"-name") == 0){
                    Analizar_Name(comando,&tamanio);

                }else if(strcasecmp(pivote,"-path") == 0){
                    Analizar_Path(comando,&tamanio);

                }else{
                    //printf("\t*** Error!! Caracteristica REP \"%s\" no reconocida ****\n",pivote);
                    memset(mensaje,'\0',sizeof(mensaje));
                    sprintf(mensaje,"\t*** Error!! Caracteristica REP \"%s\" no reconocida ****\n",pivote);
                    Mensaje_Negacion(mensaje);
                }

                memset(pivote,'\0',sizeof(pivote));
                pos = 0;

            }else{//EN TEORIA ESTE CASO PASARA SI NO SE CUMPLE LO COMETIDO ~:~
                //printf("\t*** ERROR!! la cadena \"~:~\"  esta incompleta ***\n");
                Mensaje_Negacion("\t*** ERROR!! la cadena \"~:~\"  esta incompleta ***\n");
                break;
            }


        }else if(comando[tamanio] == '#'){//SE ENCUENTRA CON EL INICIO DE UN COMENTARIO
            if(tamanio == 0)//SE LO ENCUENTRA AL INICIO DEL COMANDO
                return;
            //=============== AQUI VA EL METODO DE VALIDACION ==============
            //Imprimir_Estado();
            Validar_Rep();

            return;
        }else{
            pivote[pos] = comando[tamanio];
            pos++;
        }

        tamanio++;
    }

    //Imprimir_Estado();
    Validar_Rep();
}

void Validar_Rep()
{
    if(strcasecmp(caracteristicas.name,"nel") != 0)//COMPRUEBA SI TIENE UN NAME GUARDADO
    {
        if(strcasecmp(caracteristicas.path,"no") != 0)//COMPRUEBA SI TIENE UN PATH GUARDADO
        {
            if(strcasecmp(caracteristicas.id,"nel") != 0)//COMPRUEBA SI TIENE UN PATH GUARDADO
            {
                Hacer_Reporte();
            }else
            {
                //printf("*** ERROR!! Falta Parametro obligatorio \"-id\" ***\n");
                Mensaje_Negacion("*** ERROR!! Falta Parametro obligatorio \"-id\" ***\n");
            }
        }else
        {
            //printf("*** ERROR!! Falta Parametro obligatorio \"-path\" ***\n");
            Mensaje_Negacion("*** ERROR!! Falta Parametro obligatorio \"-path\" ***\n");
        }
    }else
    {
        //printf("*** ERROR!! Falta Parametro obligatorio \"-name\" ***\n");
        Mensaje_Negacion("*** ERROR!! Falta Parametro obligatorio \"-name\" ***\n");
    }
}


void Hacer_Reporte()
{
    char l = caracteristicas.id[2];
    int n = -1;
    if(Validar_Numero(&caracteristicas.id[3])){
        n = atoi( &caracteristicas.id[3] );

        if(Tengo_Mount(l,n))
        {
            if(strcasecmp(caracteristicas.name,"mbr") == 0)//INFORMACION MBR Y EBR PFD
            {
                char* aux = Get_Path(l, n);
                if(strcmp(aux,"no") != 0){
                    Crear_Pdf(aux);
                    //printf("=== Se creo el reporte \"MBR\" exitosamente ===\n");

                }

            }else if(strcasecmp(caracteristicas.name,"disk") == 0)//GRAFO DEL DISCO
            {
                char* aux = Get_Path(l, n);
                if(strcmp(aux,"no") != 0){
                    Crear_Grafo(aux);
                    //printf("=== Se creo el reporte \"DISK\" exitosamente ===\n");
                }


            }else if(strcasecmp(caracteristicas.name,"inode") == 0)
            {


            }else if(strcasecmp(caracteristicas.name,"journaling") == 0)
            {


            }else if(strcasecmp(caracteristicas.name,"block") == 0)
            {


            }else if(strcasecmp(caracteristicas.name,"bm_inode") == 0)
            {
                char* aux = Get_Path(l, n);
                if(strcmp(aux,"no") != 0){
                    Crear_Bm_Inode(aux);
                    Mensaje_Color("=== Se creo el reporte \"BM_INODE\" exitosamente ===\n",CYAN);
                }

            }else if(strcasecmp(caracteristicas.name,"bm_block") == 0)
            {
                char* aux = Get_Path(l, n);
                if(strcmp(aux,"no") != 0){
                    Crear_Bm_Block(aux);
                    Mensaje_Color("=== Se creo el reporte \"BM_BLOCK\" exitosamente ===\n",CYAN);
                }

            }else if(strcasecmp(caracteristicas.name,"tree") == 0)
            {
                char* aux = Get_Path(l, n);
                if(strcmp(aux,"no") != 0){
                    Graficar_Tree(aux);
                    Mensaje_Color("=== Se creo el reporte \"TREE\" exitosamente ===\n",CYAN);;
                }

            }else if(strcasecmp(caracteristicas.name,"sb") == 0)
            {
                char* aux = Get_Path(l, n);
                if(strcmp(aux,"no") != 0){
                    Graficar_SuperBlock(aux);
                    Mensaje_Color("=== Se creo el reporte \"SB\" exitosamente ===\n",CYAN);;
                }

            }else if(strcasecmp(caracteristicas.name,"file") == 0)
            {


            }else if(strcasecmp(caracteristicas.name,"ls") == 0)
            {


            }else
            {
                //printf("*** ERROR!! Caracteristica -name incompatible, debe ser \"mbr\" o \"disk\" ***\n");
                //printf("*** ERROR!! Caracteristica -name incompatible ***\n");
                Mensaje_Negacion("*** ERROR!! Caracteristica -name incompatible ***\n");
            }
        }else
        {
            //printf("*** ERROR!! Esta particion no esta montada ***\n");
            Mensaje_Negacion("*** ERROR!! Esta particion no esta montada ***\n");
        }
    }else
    {
        //printf("*** ERROR!! Esta particion no esta montada ---> ***\n");
        Mensaje_Negacion("*** ERROR!! Esta particion no esta montada ---> ***\n");
    }
}

void Crear_Pdf(char* ruta)
{
    FILE* disco;
    MBR mbr;
    disco = fopen(ruta,"r+b");

    if(disco == NULL)
    {
        //printf("\t*** ERROR!! A la hora de abrir el disco ***\n");
        Mensaje_Negacion("\t*** ERROR!! A la hora de abrir el disco ***\n");
        fclose(disco);
        return;
    }

    fread(&mbr,sizeof(MBR),1,disco);
    fclose(disco);


    char auxRuta[2000];
    memset(auxRuta,'\0',sizeof(auxRuta));
    for(int i=0;i< strlen(caracteristicas.path);i++)
    {
        if(i == strlen(caracteristicas.path) -4){
            auxRuta[i] = '.';
            auxRuta[i+1] = 'd';
            auxRuta[i+2] = 'o';
            auxRuta[i+3] = 't';
            break;
        }

        auxRuta[i] = caracteristicas.path[i];
    }

    Crear_Carpetas(caracteristicas.path); //CREO LAS CARPETAS NECESARIAS

    FILE* pdf;
    pdf = fopen(auxRuta,"wt");

    if(pdf == NULL){
        //printf("\nError al crear el archivo\n");
        Mensaje_Negacion("\nError al crear el archivo\n");
        return;
    }

    //-------------- INICIO DEL CODIGO GRAPHVIZ PARA PDF-----------------
    char buffer[2000];
    memset(buffer,'\0',sizeof(buffer));

    char begin[] = "digraph G {\n node [shape=plaintext]\n structMBR [label=<\n<TABLE BORDER=\"0\" CELLBORDER=\"1\" CELLSPACING=\"0\">\n";
    fputs(begin,pdf);
    memset(begin,0,sizeof(begin));

    //-------------- CUERPO DEL GRAPHVIZ DEL PDF-----------------
    fputs("<tr><td colspan=\"2\"><b>MBR</b></td><td></td></tr>",pdf); //NOMBRE
    fputs("<tr>\n<td><b>NOMBRE</b></td>\n<td><b>VALOR</b></td>\n</tr>",pdf); //CABECERA

    sprintf(buffer,"<tr>\n<td><b>mbr_tamaño</b></td>\n<td>%d</td>\n</tr>",mbr.mbr_tamanio);
    fputs(buffer,pdf);
    memset(buffer,'\0',sizeof(buffer));

    sprintf(buffer,"<tr>\n<td><b>mbr_fecha_creacion</b></td>\n<td>%s</td>\n</tr>",mbr.mbr_fecha_creacion);
    fputs(buffer,pdf);
    memset(buffer,'\0',sizeof(buffer));

    sprintf(buffer,"<tr>\n<td><b>mbr_disk_signature</b></td>\n<td>%d</td>\n</tr>",mbr.mbr_disk_signature);
    fputs(buffer,pdf);
    memset(buffer,'\0',sizeof(buffer));

    sprintf(buffer,"<tr>\n<td><b>disk_fit</b></td>\n<td>%c</td>\n</tr>",mbr.disk_fit);
    fputs(buffer,pdf);
    memset(buffer,'\0',sizeof(buffer));

    Particion ext;
    for(int i=0; i<4 ;i++)
    {
        Particion actual = mbr.mbr_particion[i];
        if(actual.part_status != '0')//SI NO ESTA DISPONIBLE NO GRAFICAR
        {
            sprintf(buffer,"<tr>\n<td><b>part_status_%d</b></td>\n<td>%c</td>\n</tr>",i+1,actual.part_status);
            fputs(buffer,pdf);
            memset(buffer,'\0',sizeof(buffer));

            sprintf(buffer,"<tr>\n<td><b>part_type_%d</b></td>\n<td>%c</td>\n</tr>",i+1,actual.part_type);
            fputs(buffer,pdf);
            memset(buffer,'\0',sizeof(buffer));

            sprintf(buffer,"<tr>\n<td><b>part_fit_%d</b></td>\n<td>%c</td>\n</tr>",i+1,actual.part_fit);
            fputs(buffer,pdf);
            memset(buffer,'\0',sizeof(buffer));

            sprintf(buffer,"<tr>\n<td><b>part_start_%d</b></td>\n<td>%d</td>\n</tr>",i+1,actual.part_start);
            fputs(buffer,pdf);
            memset(buffer,'\0',sizeof(buffer));

            sprintf(buffer,"<tr>\n<td><b>part_size_%d</b></td>\n<td>%d</td>\n</tr>",i+1,actual.part_size);
            fputs(buffer,pdf);
            memset(buffer,'\0',sizeof(buffer));

            sprintf(buffer,"<tr>\n<td><b>part_name_%d</b></td>\n<td>%s</td>\n</tr>",i+1,actual.part_name);
            fputs(buffer,pdf);
            memset(buffer,'\0',sizeof(buffer));

            if( actual.part_type == 'e' )
            {
                ext = actual;
            }
        }
    }

    if(ext.part_fit != '\0')
    {
        FILE* disco;
        //MBR mbr;
        disco = fopen(ruta,"r+b");

        if(disco == NULL)
        {
            //printf("\t*** ERROR!! A la hora de abrir el disco ***\n");
            Mensaje_Negacion("\t*** ERROR!! A la hora de abrir el disco ***\n");
            return;
        }

        EBR ebr;
        fseek(disco, ext.part_start ,SEEK_SET);
        fread(&ebr,sizeof(EBR),1,disco);

        int i =1;
        while(ebr.part_next != 0)//INDICA QUE ESTOY EN EL FINAL
        {
            if(ebr.part_status != '0')
            {
                fputs("</TABLE>>];\n",pdf);
                sprintf(buffer,"structEBR%d [label=<\n<TABLE BORDER=\"0\" CELLBORDER=\"1\" CELLSPACING=\"0\">\n",i);
                fputs(buffer,pdf);
                memset(buffer,'\0',sizeof(buffer));

                sprintf(buffer,"<tr><td colspan=\"2\"><b>EBR%d</b></td><td></td></tr>\n",i);//NOMBRE
                fputs(buffer,pdf);
                memset(buffer,'\0',sizeof(buffer));
                fputs("<tr>\n<td><b>NOMBRE</b></td>\n<td><b>VALOR</b></td>\n</tr>\n",pdf); //CABECERA



                sprintf(buffer,"<tr>\n<td><b>part_status_%d</b></td>\n<td>%c</td>\n</tr>",i,ebr.part_status);
                fputs(buffer,pdf);
                memset(buffer,'\0',sizeof(buffer));

                sprintf(buffer,"<tr>\n<td><b>part_fit_%d</b></td>\n<td>%c</td>\n</tr>",i,ebr.part_fit);
                fputs(buffer,pdf);
                memset(buffer,'\0',sizeof(buffer));

                sprintf(buffer,"<tr>\n<td><b>part_start_%d</b></td>\n<td>%d</td>\n</tr>",i,ebr.part_start);
                fputs(buffer,pdf);
                memset(buffer,'\0',sizeof(buffer));

                sprintf(buffer,"<tr>\n<td><b>part_size_%d</b></td>\n<td>%d</td>\n</tr>",i,ebr.part_size);
                fputs(buffer,pdf);
                memset(buffer,'\0',sizeof(buffer));

                sprintf(buffer,"<tr>\n<td><b>part_next_%d</b></td>\n<td>%d</td>\n</tr>",i,ebr.part_next);
                fputs(buffer,pdf);
                memset(buffer,'\0',sizeof(buffer));

                sprintf(buffer,"<tr>\n<td><b>part_name_%d</b></td>\n<td>%s</td>\n</tr>",i,ebr.part_name);
                fputs(buffer,pdf);
                memset(buffer,'\0',sizeof(buffer));

                i++;
            }
            fseek(disco, ebr.part_next ,SEEK_SET);
            fread(&ebr,sizeof(EBR),1,disco);
        }
        fclose(disco);
    }

    fputs("</TABLE>>];\n}",pdf);
    fclose(pdf);

    sprintf(buffer,"dot -Tpdf -O %s",auxRuta);
    system(buffer); //ESTE PARA CREAR

    Mensaje_Aceptacion("=== Se creo el reporte \"MBR\" exitosamente ===\n");
}

void Crear_Grafo(char* ruta)
{
    FILE* disco;
    MBR mbr;
    disco = fopen(ruta,"r+b");

    if(disco == NULL)
    {
        //printf("\t*** ERROR!! A la hora de abrir el disco ***\n");
        Mensaje_Negacion("\t*** ERROR!! A la hora de abrir el disco ***\n");
        return;
    }

    fread(&mbr,sizeof(MBR),1,disco);



    char auxRuta[2000];
    memset(auxRuta,'\0',sizeof(auxRuta));
    for(int i=0;i< strlen(caracteristicas.path);i++)
    {
        if(i == strlen(caracteristicas.path) -4){
            auxRuta[i] = '.';
            auxRuta[i+1] = 'd';
            auxRuta[i+2] = 'o';
            auxRuta[i+3] = 't';
            break;
        }

        auxRuta[i] = caracteristicas.path[i];
    }

    Crear_Carpetas(caracteristicas.path); //CREO LAS CARPETAS NECESARIAS

    FILE* pdf;
    pdf = fopen(auxRuta,"wt");

    if(pdf == NULL){
        printf("\nError al crear el archivo\n");
        Mensaje_Negacion("\nError al crear el archivo\n");
        return;
    }

    //-------------- INICIO DEL CODIGO GRAPHVIZ PARA PDF-----------------
    char buffer[2000];
    memset(buffer,'\0',sizeof(buffer));

    char begin[] = "digraph G {\n node [shape=plaintext]\n structMBR [label=<\n<TABLE BORDER=\"0\" CELLBORDER=\"1\" CELLSPACING=\"0\" CELLPADDING=\"6\" COLOR=\"BLACK\">\n";
    fputs(begin,pdf);
    memset(begin,0,sizeof(begin));

    fputs("<tr>\n<td colspan=\"4\" rowspan=\"4\" bgcolor=\"RED\" >MBR</td>\n",pdf);//AGREGO EL MBR

    int maxSize = mbr.mbr_tamanio;
    int recorrido = sizeof(mbr);
    Particion ext;

    int contExt = 0;

    for(int i =0; i<4 ;i++)
    {
        Particion actual = mbr.mbr_particion[i]; //PARTICION ACTUAL

        if(actual.part_status != '0')//SI ESTA 0 NO ESTA DISPONIBLE, ENTONCES ESTA LIBRE
        {
            if(tolower(actual.part_type) != 'e')//SI ES EXTENDIDA QUE SE VAYA HACER OTRA COSA
            {
                int porcent;
                int fragmentacion = (actual.part_start-recorrido);
                if(  fragmentacion > 5 )//EN TEORIA SI ES MAYOR A 0 HAY FRAGMENTACION
                {
                    porcent = (fragmentacion * 100)/maxSize;
                    //sprintf(buffer,"<td colspan=\"4\" rowspan=\"4\" bgcolor=\"#12d80f\" >Libre<br/> %d%c del disco</td>\n",porcent,'%');
                    sprintf(buffer,"<td colspan=\"4\" rowspan=\"4\" bgcolor=\"#b2b7b2\" >Libre<br/> %d%c del disco<br/>%d bytes</td>\n",porcent,'%',fragmentacion);
                    fputs(buffer,pdf);
                    memset(buffer,'\0',sizeof(buffer));
                }
                porcent = (actual.part_size * 100)/maxSize;
                sprintf(buffer,"<td colspan=\"4\" rowspan=\"4\" bgcolor=\"BLUE\" >%s<br/> (Primaria)<br/> %d%c del disco<br/>%d bytes</td>\n",actual.part_name,porcent,'%',actual.part_size);
                fputs(buffer,pdf);
                memset(buffer,'\0',sizeof(buffer));
                recorrido+=actual.part_size;
            }else
            {
                ext = actual;
                EBR ebr;
                fseek(disco, actual.part_start ,SEEK_SET);
                fread(&ebr,sizeof(EBR),1,disco);

                int porcent;
                int fragmentacion = (actual.part_start-recorrido);
                if(  fragmentacion > 5 )//EN TEORIA SI ES MAYOR A 0 HAY FRAGMENTACION
                {
                    porcent = (fragmentacion * 100)/maxSize;
                    //sprintf(buffer,"<td colspan=\"4\" rowspan=\"4\" bgcolor=\"#12d80f\" >Libre<br/> %d%c del disco</td>\n",porcent,'%');
                    sprintf(buffer,"<td colspan=\"4\" rowspan=\"4\" bgcolor=\"#b2b7b2\" >Libre<br/> %d%c del disco<br/>%d bytes</td>\n",porcent,'%',fragmentacion);
                    fputs(buffer,pdf);
                    memset(buffer,'\0',sizeof(buffer));
                }

                while(ebr.part_next != 0)//INDICA QUE NO HAY MAS
                {
                    contExt+=2;

                    recorrido = ebr.part_next;
                    fseek(disco, ebr.part_next ,SEEK_SET);
                    fread(&ebr,sizeof(EBR),1,disco);
                }
                recorrido = ext.part_size + ext.part_start; // EN TEORIA ES LA SUMA HASTA EL FINAL DE EXT
                contExt+=2;//AQUI PONER EL RESTO

                sprintf(buffer,"<td colspan=\"%d\" rowspan=\"2\" bgcolor=\"RED\">Extendida</td>\n",2*contExt);
                fputs(buffer,pdf);
                memset(buffer,'\0',sizeof(buffer));


            }
        }
        else
        {
            i++;

            if(i == 4)
            {
                int porcent = ((maxSize-recorrido) * 100)/maxSize;
                if(porcent > 0)
                {
                    sprintf(buffer,"<td colspan=\"4\" rowspan=\"4\" bgcolor=\"BLUE\" >%s<br/> (Primaria)<br/> %d%c del disco<br/>%d bytes</td>\n",actual.part_name,porcent,'%',maxSize-recorrido);
                    fputs(buffer,pdf);
                    memset(buffer,'\0',sizeof(buffer));//ACTUALIZAR RECORRIDO
                    recorrido = maxSize;
                }
            }else
            {
                while(i<4 )
                {
                    if(mbr.mbr_particion[i].part_status != '0')// SI ES DISTINTA ENCONTRO FIN
                    {
                        int porcent = ((mbr.mbr_particion[i].part_start-recorrido) * 100)/maxSize;
                        if(porcent > 0)
                        {
                            //sprintf(buffer,"<td colspan=\"4\" rowspan=\"4\" bgcolor=\"#12d80f\" >Libre<br/> %d%c del disco</td>\n",porcent,'%');
                            sprintf(buffer,"<td colspan=\"4\" rowspan=\"4\" bgcolor=\"#b2b7b2\" >Libre<br/> %d%c del disco<br/>%d bytes</td>\n",porcent,'%',(mbr.mbr_particion[i].part_start-recorrido));
                            fputs(buffer,pdf);
                            memset(buffer,'\0',sizeof(buffer));
                            recorrido = mbr.mbr_particion[i].part_start;
                        }
                        i--;
                        break;
                    }
                    else if(i == 3)
                    {
                        int porcent = ((maxSize-recorrido) * 100)/maxSize;
                        if(porcent > 0)
                        {
                            //sprintf(buffer,"<td colspan=\"4\" rowspan=\"4\" bgcolor=\"#12d80f\" >Libre<br/> %d%c del disco</td>\n",porcent,'%');
                            sprintf(buffer,"<td colspan=\"4\" rowspan=\"4\" bgcolor=\"#b2b7b2\" >Libre<br/> %d%c del disco<br/>%d bytes</td>\n",porcent,'%',(maxSize-recorrido));
                            fputs(buffer,pdf);
                            memset(buffer,'\0',sizeof(buffer));
                            recorrido = maxSize;
                        }
                    }
                }
            }

        }
    }
    //int porcent = ((maxSize-recorrido) * 100)/maxSize;//AGREGAR CONDICION DE 0
    int resta = maxSize-recorrido;
    int por = resta * 100;
    if(por < 0)
        por = por * -1;
    int porcent = por / maxSize;
    if(porcent >0)
    {
        //sprintf(buffer,"<td colspan=\"4\" rowspan=\"4\" bgcolor=\"#12d80f\" >Libre<br/> %d%c del disco</td>\n",porcent,'%');
        sprintf(buffer,"<td colspan=\"4\" rowspan=\"4\" bgcolor=\"#b2b7b2\" >Libre<br/> %d%c del disco<br/>%d bytes</td>\n",porcent,'%',resta);
        fputs(buffer,pdf);
        memset(buffer,'\0',sizeof(buffer));
    }

    fputs("</tr>\n\n",pdf);

    fputs("<tr>\n<td></td>\n</tr>\n",pdf);

    if(ext.part_fit != '\0')//EXT GUARDA LA PARTICION EXTENDIDA = ACTUAL
    {
        fputs("<tr>\n",pdf);


        EBR ebr;
        //EBR ebr_anterior;
        int acumulacion = 0; //TAMANIO QUE SE A ESTADO GUARDANDO

        fseek(disco, ext.part_start ,SEEK_SET);
        fread(&ebr,sizeof(EBR),1,disco);

        while(ebr.part_next != 0)//INDICA QUE NO HAY MAS
        {
            sprintf(buffer,"<td colspan=\"%d\" rowspan=\"2\" bgcolor=\"RED\">EBR</td>\n",2);
            fputs(buffer,pdf);
            memset(buffer,'\0',sizeof(buffer));

            //int a = (ebr.part_size * 100) / ext.part_size;
            int a = (ebr.part_size * 100) / maxSize;

            if(a > 0)
            {
                acumulacion+= ebr.part_size;

                sprintf(buffer,"<td colspan=\"2\" rowspan=\"2\" bgcolor=\"#37e8e6\" >%s<br/> (Logica)<br/> %d%c del disco<br/>%d bytes</td>\n",ebr.part_name,a,'%',ebr.part_size);
                fputs(buffer,pdf);
                memset(buffer,'\0',sizeof(buffer));
            }

            //ebr_anterior = ebr;

            recorrido = ebr.part_next;
            fseek(disco, ebr.part_next ,SEEK_SET);
            fread(&ebr,sizeof(EBR),1,disco);
        }
        recorrido = ext.part_size + ext.part_start; // EN TEORIA ES LA SUMA HASTA EL FINAL DE EXT

        /*sprintf(buffer,"<td colspan=\"%d\" rowspan=\"2\">EBR</td>\n",2);
        fputs(buffer,pdf);
        memset(buffer,'\0',sizeof(buffer));*/

        //int a = ((ext.part_size - ebr.part_start) * 100) / (ext.part_size + ext.part_start );

        int a = ((ext.part_size - acumulacion ) * 100) / maxSize ; // (SIZE_EXT - SUMA_EXT * 100)/MAXSIZE

        if(a > -1)
        {
            //(buffer,"<td colspan=\"2\" rowspan=\"2\" bgcolor=\"#12d80f\" >LIBRE<br/> %d%c del disco</td>\n",a,'%');
            sprintf(buffer,"<td colspan=\"2\" rowspan=\"2\" bgcolor=\"#b2b7b2\" >LIBRE<br/> %d%c del disco<br/>%d bytes</td>\n",a,'%',(ext.part_size - acumulacion) );
            fputs(buffer,pdf);
            memset(buffer,'\0',sizeof(buffer));
        }


        fputs("</tr>\n\n",pdf);
    }



    fputs("<tr>\n<td></td>\n</tr>\n",pdf);
    fputs("</TABLE>>];\n}",pdf);

    fclose(pdf);
    fclose(disco);

    sprintf(buffer,"dot -Tpng -O %s",auxRuta);
    system(buffer); //ESTE PARA CREAR

    Mensaje_Aceptacion("=== Se creo el reporte \"DISK\" exitosamente ===\n");
}

void Graficar_SuperBlock(char* ruta)
{
    FILE* disco;
    //MBR mbr;
    disco = fopen(ruta,"r+b");

    if(disco == NULL)
    {
        Mensaje_Color("\t*** ERROR!! A la hora de abrir el disco ***\n",ROJO);
        return;
    }
    //fread(&mbr,sizeof(MBR),1,disco);

    Nodo* datos_particion = Get_Mount(ruta);
    fseek(disco,datos_particion->inicio,SEEK_SET);//ME POSICIONO AL INICIO DE LA PARTICION PARA OBTENER EL SUPERBLOQUE

    SuperBlock sblock;

    fread(&sblock,sizeof(SuperBlock),1,disco);//OBTENGO EL SUPERBLOQUE


    char auxRuta[2000];
    memset(auxRuta,'\0',sizeof(auxRuta));
    for(unsigned int i=0;i< strlen(caracteristicas.path);i++)
    {
        if(i == strlen(caracteristicas.path) -4){
            auxRuta[i] = '.';
            auxRuta[i+1] = 'd';
            auxRuta[i+2] = 'o';
            auxRuta[i+3] = 't';
            break;
        }

        auxRuta[i] = caracteristicas.path[i];
    }

    Crear_Carpetas(caracteristicas.path); //CREO LAS CARPETAS NECESARIAS

    fclose(disco);

    FILE* pdf;
    pdf = fopen(auxRuta,"wt");

    if(pdf == NULL){
        Mensaje_Color("\nError al crear el archivo\n",ROJO);
        return;
    }

    //-------------- INICIO DEL CODIGO GRAPHVIZ PARA PDF-----------------
    char buffer[2000];
    memset(buffer,'\0',sizeof(buffer));

    char begin[] = "digraph G {\n node [shape=plaintext]\n structMBR [label=<\n<TABLE BORDER=\"0\" CELLBORDER=\"1\" CELLSPACING=\"0\">\n";
    fputs(begin,pdf);
    memset(begin,0,sizeof(begin));

    //-------------- CUERPO DEL GRAPHVIZ DEL PDF-----------------
    sprintf(buffer,"<tr><td colspan=\"2\"><b>SUPERBLOCK %s</b></td><td></td></tr>",datos_particion->name);
    fputs("",pdf); //NOMBRE
    fputs("<tr>\n<td><b>NOMBRE</b></td>\n<td><b>VALOR</b></td>\n</tr>",pdf); //CABECERA


    sprintf(buffer,"<tr>\n<td><b>s_inodes_count</b></td>\n<td>%d</td>\n</tr>",sblock.s_inodes_count);
    fputs(buffer,pdf);
    memset(buffer,'\0',sizeof(buffer));

    sprintf(buffer,"<tr>\n<td><b>s_blocks_count</b></td>\n<td>%d</td>\n</tr>",sblock.s_blocks_count);
    fputs(buffer,pdf);
    memset(buffer,'\0',sizeof(buffer));

    sprintf(buffer,"<tr>\n<td><b>s_free_blocks_count</b></td>\n<td>%d</td>\n</tr>",sblock.s_free_blocks_count);
    fputs(buffer,pdf);
    memset(buffer,'\0',sizeof(buffer));

    sprintf(buffer,"<tr>\n<td><b>s_free_inodes_count</b></td>\n<td>%d</td>\n</tr>",sblock.s_free_inodes_count);
    fputs(buffer,pdf);
    memset(buffer,'\0',sizeof(buffer));


    sprintf(buffer,"<tr>\n<td><b>s_mtime</b></td>\n<td>%s</td>\n</tr>",sblock.s_mtime);
    fputs(buffer,pdf);
    memset(buffer,'\0',sizeof(buffer));


    sprintf(buffer,"<tr>\n<td><b>s_umtime</b></td>\n<td>%s</td>\n</tr>",sblock.s_umtime);
    fputs(buffer,pdf);
    memset(buffer,'\0',sizeof(buffer));

    sprintf(buffer,"<tr>\n<td><b>s_mnt_count</b></td>\n<td>%d</td>\n</tr>",sblock.s_mnt_count);
    fputs(buffer,pdf);
    memset(buffer,'\0',sizeof(buffer));

    sprintf(buffer,"<tr>\n<td><b>s_magic</b></td>\n<td>%s</td>\n</tr>", "0xEF53" );  //POSIBLEMENTE MODIFICAR
    fputs(buffer,pdf);
    memset(buffer,'\0',sizeof(buffer));

    sprintf(buffer,"<tr>\n<td><b>s_inode_size</b></td>\n<td>%d</td>\n</tr>",sblock.s_inodes_size);
    fputs(buffer,pdf);
    memset(buffer,'\0',sizeof(buffer));

    sprintf(buffer,"<tr>\n<td><b>s_block_size</b></td>\n<td>%d</td>\n</tr>",sblock.s_block_size);
    fputs(buffer,pdf);
    memset(buffer,'\0',sizeof(buffer));

    sprintf(buffer,"<tr>\n<td><b>s_first_ino</b></td>\n<td>%d</td>\n</tr>",sblock.s_first_ino);
    fputs(buffer,pdf);
    memset(buffer,'\0',sizeof(buffer));

    sprintf(buffer,"<tr>\n<td><b>s_first_blo</b></td>\n<td>%d</td>\n</tr>",sblock.s_first_blo);
    fputs(buffer,pdf);
    memset(buffer,'\0',sizeof(buffer));

    sprintf(buffer,"<tr>\n<td><b>s_bm_inode_start</b></td>\n<td>%d</td>\n</tr>",sblock.s_bm_inode_start);
    fputs(buffer,pdf);
    memset(buffer,'\0',sizeof(buffer));

    sprintf(buffer,"<tr>\n<td><b>s_bm_block_start</b></td>\n<td>%d</td>\n</tr>",sblock.s_bm_block_start);
    fputs(buffer,pdf);
    memset(buffer,'\0',sizeof(buffer));

    sprintf(buffer,"<tr>\n<td><b>s_inode_start</b></td>\n<td>%d</td>\n</tr>",sblock.s_inode_start);
    fputs(buffer,pdf);
    memset(buffer,'\0',sizeof(buffer));

    sprintf(buffer,"<tr>\n<td><b>s_block_start</b></td>\n<td>%d</td>\n</tr>",sblock.s_block_start);
    fputs(buffer,pdf);
    memset(buffer,'\0',sizeof(buffer));


    fputs("</TABLE>>];\n}",pdf);
    fclose(pdf);

    sprintf(buffer,"dot -Tpdf -O %s",auxRuta);
    system(buffer); //ESTE PARA CREAR
}


void Crear_Bm_Inode(char* ruta)
{
    FILE* disco;
    //MBR mbr;
    disco = fopen(ruta,"r+b");

    if(disco == NULL)
    {
        Mensaje_Color("\t*** ERROR!! A la hora de abrir el disco ***\n",ROJO);
        return;
    }

    Nodo* datos_particion = Get_Mount(ruta);
    fseek(disco,datos_particion->inicio,SEEK_SET);//ME POSICIONO AL INICIO DE LA PARTICION PARA OBTENER EL SUPERBLOQUE

    SuperBlock sblock;

    fread(&sblock,sizeof(SuperBlock),1,disco);//OBTENGO EL SUPERBLOQUE

    fseek(disco,sblock.s_bm_inode_start,SEEK_SET);//ME POSICIONO EN INICIO DEL BITMAP DE INODOS

    int n = Calcular_N(datos_particion->size);

    char Bm_inodo[n];//TENGO LA BASE PARA OBTENER EL BITMAP DE INODOS
    fread(Bm_inodo,sizeof(Bm_inodo), 1,disco);//OBTENGO EL BITMAP DE INODOS

    fclose(disco);


    Crear_Carpetas(caracteristicas.path); //CREO LAS CARPETAS NECESARIAS


    char auxRuta[2000];
    memset(auxRuta,'\0',sizeof(auxRuta));
    for(unsigned int i=0;i< strlen(caracteristicas.path);i++)
    {
        if(i == strlen(caracteristicas.path) -4){
            auxRuta[i] = '.';
            auxRuta[i+1] = 't';
            auxRuta[i+2] = 'x';
            auxRuta[i+3] = 't';
            break;
        }

        auxRuta[i] = caracteristicas.path[i];
    }

    FILE* txt;
    txt = fopen(auxRuta,"wt");

    if(txt == NULL){
        Mensaje_Color("\nError al crear el archivo\n",ROJO);
        return;
    }

    char buffer[9000];
    memset(buffer,'\0',sizeof(buffer));

    int salto = 1; int auxP = 0;
    for(int i=0 ; i<n  ;i++)
    {
        if(salto==30)
        {
            buffer[auxP] = Bm_inodo[i];
            auxP++;
            buffer[auxP] = '\n';
            fputs(buffer,txt);
            salto = 1; auxP = 0;
            memset(buffer,'\0',sizeof(buffer));
        }else
        {
            buffer[auxP] = Bm_inodo[i];
            auxP++;
            buffer[auxP] = ' ';
            salto++; auxP++;
        }
    }

    fputs(buffer,txt);

    fclose(txt);

}


void Crear_Bm_Block(char* ruta)
{
    FILE* disco;
    //MBR mbr;
    disco = fopen(ruta,"r+b");

    if(disco == NULL)
    {
        Mensaje_Color("\t*** ERROR!! A la hora de abrir el disco ***\n",ROJO);
        return;
    }

    Nodo* datos_particion = Get_Mount(ruta);
    fseek(disco,datos_particion->inicio,SEEK_SET);//ME POSICIONO AL INICIO DE LA PARTICION PARA OBTENER EL SUPERBLOQUE

    SuperBlock sblock;

    fread(&sblock,sizeof(SuperBlock),1,disco);//OBTENGO EL SUPERBLOQUE

    fseek(disco,sblock.s_bm_block_start,SEEK_SET);//ME POSICIONO EN INICIO DEL BITMAP DE BLOQUES

    int n = Calcular_N(datos_particion->size);

    int tamB = 3 * n;

    char Bm_bloque[tamB];//TENGO LA BASE PARA OBTENER EL BITMAP DE BLOQUE
    fread(Bm_bloque,sizeof(Bm_bloque), 1,disco);//OBTENGO EL BITMAP DE BLOQUE

    fclose(disco);


    Crear_Carpetas(caracteristicas.path); //CREO LAS CARPETAS NECESARIAS


    char auxRuta[2000];
    memset(auxRuta,'\0',sizeof(auxRuta));
    for(unsigned int i=0;i< strlen(caracteristicas.path);i++)
    {
        if(i == strlen(caracteristicas.path) -4){
            auxRuta[i] = '.';
            auxRuta[i+1] = 't';
            auxRuta[i+2] = 'x';
            auxRuta[i+3] = 't';
            break;
        }

        auxRuta[i] = caracteristicas.path[i];
    }

    FILE* txt;
    txt = fopen(auxRuta,"wt");

    if(txt == NULL){
        Mensaje_Color("\nError al crear el archivo\n",ROJO);
        return;
    }

    char buffer[9000];
    memset(buffer,'\0',sizeof(buffer));

    int salto = 1; int auxP = 0;
    for(int i=0 ; i < tamB  ;i++)
    {

        if(Bm_bloque[i] != '0')
        {
            buffer[auxP] = '1';
            auxP++;
        }else
        {
            buffer[auxP] = '0';
            auxP++;
        }

        if(salto==30)
        {
            buffer[auxP] = '\n';
            fputs(buffer,txt);
            salto = 1; auxP = 0;
            memset(buffer,'\0',sizeof(buffer));
        }else
        {
            buffer[auxP] = ' ';
            salto++; auxP++;
        }
    }
    fputs(buffer,txt);
    fclose(txt);
}


void Graficar_Tree(char* ruta)
{

}


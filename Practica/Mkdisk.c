#include "Mkdisk.h"


void Analizar_Mkdisk(char* comando, int tamanio)
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
                if(strcasecmp(pivote,"-size") == 0){

                    Analizar_Size(comando,&tamanio);


                }else if(strcasecmp(pivote,"-fit") == 0){
                    Analizar_Fit(comando,&tamanio);

                }else if(strcasecmp(pivote,"-unit") == 0){
                    Analizar_Unit(comando,&tamanio);

                }else if(strcasecmp(pivote,"-path") == 0){
                    Analizar_Path(comando,&tamanio);

                }else{
                    //printf("\t*** Error!! Caracteristica MKDISK \"%s\" no reconocida ****\n",pivote);
                    memset(mensaje,'\0',sizeof(mensaje));
                    sprintf(mensaje,"\t*** Error!! Caracteristica MKDISK \"%s\" no reconocida ****\n",pivote);
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
            Validar_Mkdisk();

            return;
        }else{
            pivote[pos] = comando[tamanio];
            pos++;
        }

        tamanio++;
    }

    //Imprimir_Estado();
    Validar_Mkdisk();

}


void Imprimir_Estado(void)
{
    printf("\t\tsize: %d \n",caracteristicas.size);
    printf("\t\tfit: %c \n",caracteristicas.fit);
    printf("\t\tunit: %c \n",caracteristicas.unit);
    printf("\t\tpath: %s \n",caracteristicas.path);
}


void Validar_Mkdisk(void)
{
    if(caracteristicas.size != -1)//COMPRUEBA SI TIENE UN SIZE GUARDADO
    {
        if(strcasecmp(caracteristicas.path,"no") != 0)//COMPRUEBA SI TIENE UN PATH GUARDADO
        {
            Crear_Disco(true);
        }else
        {
            //printf("*** ERROR!! Falta Parametro obligatorio \"-path\" ***\n");
            Mensaje_Negacion("\t*** ERROR!! Falta Parametro obligatorio \"-path\" ***\n");
        }
    }else
    {
         //printf("*** ERROR!! Falta Parametro obligatorio \"-size\" ***\n");
         Mensaje_Negacion("\t*** ERROR!! Falta Parametro obligatorio \"-size\" ***\n");
    }
}


void Crear_Disco(bool raid)
{
    FILE* disco;
    MBR mbr;

    if(caracteristicas.unit == 'k' )
    {
        mbr.mbr_tamanio = caracteristicas.size * 1024;
    }else{
        mbr.mbr_tamanio = caracteristicas.size * 1024 * 1024;
    }
    mbr.disk_fit = caracteristicas.fit;

    /*time_t tiempo = time(0);
    struct tm *tlocal = localtime(&tiempo);
    //char output[128];
    //strftime(output,128,"%d/%m/%y %H:%M:%S",tlocal);
    strftime(mbr.mbr_fecha_creacion,128,"%d/%m/%y %H:%M:%S",tlocal);*/


    Obtener_Fecha(mbr.mbr_fecha_creacion);//OBTENGO LA FECHA Y HORA

    time_t t;
    srand( (unsigned) time(&t) );
    mbr.mbr_disk_signature = rand() & 10000;

    Particion auxP;
    auxP.part_status = '0'; //EL CERO ME INDICA FALSE Y EL UNO TRUE

    for(int i=0; i<4 ; i++)
        mbr.mbr_particion[i] = auxP;

    char* aux = caracteristicas.path;

    Crear_Carpetas(aux);    //CREO LAS CARPETAS NECESARIAS

    disco = fopen(aux,"wb");

    if(disco == NULL)
    {
        //printf("\t*** ERROR!! A la hora de crear el disco ***\n");
        if(raid)
            Mensaje_Negacion("\t*** ERROR!! A la hora de crear el disco ***\n");
        return;
    }
    rewind(disco);
    fwrite(&mbr,sizeof(MBR),1,disco);


    fseek(disco,mbr.mbr_tamanio-1,SEEK_SET);
    fwrite("\0",1,1,disco);

    fclose(disco);
    //printf("\t=== El Disco a sido creado exitosamente!! ===\n");
    if(raid)
        Mensaje_Aceptacion("\t=== El Disco a sido creado exitosamente!! ===\n");


    if(raid)//SI ES TRUE CREO EL RAID
    {
        Ruta_Raid(caracteristicas.path);
        Crear_Disco(false);
    }


    //char a[2000]; memset(a,'\0',sizeof(a));
    //sprintf(a,"%s\n", caracteristicas.path);
    //Mensaje_Magenta(a);

}




















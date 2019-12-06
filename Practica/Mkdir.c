#include "Mkdir.h"

void Analizar_Mkdir(char* comando, int tamanio)
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
                if(strcasecmp(pivote,"-path") == 0){
                    Analizar_Path(comando,&tamanio);

                }else{
                    memset(mensaje,'\0',sizeof(mensaje));
                    sprintf(mensaje,"\t*** Error!! Caracteristica MKDIR \"%s\" no reconocida ****\n",pivote);
                    Mensaje_Negacion(mensaje);
                }
                memset(pivote,'\0',sizeof(pivote));
                pos = 0;

            }else{//EN TEORIA ESTE CASO PASARA SI NO SE CUMPLE LO COMETIDO ~:~
                Mensaje_Negacion("\t*** ERROR!! la cadena \"~:~\"  esta incompleta ***\n");
                break;
            }
        }else if(comando[tamanio] == ' ' || comando[tamanio] == '\n')   // VERIFICO SI ES UN ESPACIO PARA EL PARAMETRO -p
        {
            if( !strcasecmp(pivote,"-p"))
            {
                Analizar_P(comando,&tamanio);
                memset(pivote,'\0',sizeof(pivote));
                pos = 0;
            }else if(strcasecmp(pivote,"\0") == 0)
            {
                break;
            }else{//EN TEORIA ESTE CASO PASARA SI NO SE CUMPLE QUE NO LLEVA NADA
                Mensaje_Negacion("\t*** ERROR!! El parametro -p no lleva nada ***\n");
                break;
            }
        }
        else if(comando[tamanio] == '#')
        {//SE ENCUENTRA CON EL INICIO DE UN COMENTARIO
            if(tamanio == 0)//SE LO ENCUENTRA AL INICIO DEL COMANDO
                return;
            //=============== AQUI VA EL METODO DE VALIDACION ==============
            Validar_Mkdir();
            return;
        }else{
            pivote[pos] = comando[tamanio];
            pos++;
        }
        tamanio++;
    }
    Validar_Mkdir();
}

void Validar_Mkdir()
{
    if(strcmp(caracteristicas.path,"no") != 0)
    {
        Ejecutar_Comandos_Mkdir();
    }else{
        printf("*** ERROR!! Falta Parametro obligatorio \"-path\" ***\n");
    }
}

void Ejecutar_Comandos_Mkdir()
{
    int parseo = -1;
    if(Validar_Numero(&daLoguer.id[3]))
    {
        parseo = atoi(&daLoguer.id[3]);
        int valor = Get_Inicio(daLoguer.id[2],parseo);
        if(valor != -1)
        {
            char* ruta = Get_Path(daLoguer.id[2],parseo);

            FILE* disco = fopen(ruta,"r+b");
            if(disco == NULL)
                    return;
            Nodo* datos_particion = Get_Mount(ruta);

            SuperBlock sblock;
            fseek(disco,datos_particion->inicio,SEEK_SET);
            fread(&sblock,sizeof(SuperBlock),1,disco);//SE LEE EL DISCO CON EL TAMAÑO DEL SUPERBLOQUE

            if(caracteristicas.p)   // SI ES TRUE ENTONCES CREO LAS CARPETAS NECESARIAS
            {
                Ejecutar_P(disco,sblock,datos_particion->size,datos_particion->inicio);
            }

            int r = Existe_Directorio(disco,sblock,datos_particion->size,caracteristicas.path);
            if( r == -1 )   // SI ES IGUAL ENTONCES LO CREA
            {
                Crear_Directorio(disco,sblock,datos_particion->inicio,datos_particion->size,caracteristicas.path);
            }


            int ret = Existe_Directorio(disco,sblock,datos_particion->size,caracteristicas.path);
            if(ret != -1)
                Mensaje_Color("=== Se creo el nuevo Directorio ===\n",CYAN);
            else
                Mensaje_Color("*** ERROR!! Este Directorio no se creo ***\n",ROJO);


            fclose(disco);
        }else
        {
            Mensaje_Color("*** ERROR!! No existe un particion montada con este id ***\n",ROJO);
        }
    }
}

void Ejecutar_P(FILE *disco, SuperBlock sblock, int size, int inicio)
{
    char delim[] = "/";
    char nuevo[1000];
    memset(nuevo,'\0',sizeof(nuevo));
    strcat(nuevo,caracteristicas.path);

    char *ptr = strtok(nuevo,delim);

    char* carpetas[100];
    memset(carpetas,'\0',sizeof(carpetas));

    int sizeR=0;
    while(ptr != NULL)
    {
        carpetas[sizeR] = ptr;
        sizeR++;
        ptr = strtok(NULL, delim);
    }

    char auxRuta[1000];
    memset(auxRuta,'\0',sizeof(auxRuta));
    auxRuta[0] = '/';

    for(int i=0; i<sizeR-1 ;i++)
    {
        if(i != 0)
            strcat(auxRuta,"/");

        strcat(auxRuta,carpetas[i]);

        int r = Existe_Directorio(disco,sblock,size,auxRuta);
        if( r == -1 )   // SI ES IGUAL ENTONCES LO CREA
        {
            Crear_Directorio(disco,sblock,inicio,size,auxRuta);
        }
    }
}















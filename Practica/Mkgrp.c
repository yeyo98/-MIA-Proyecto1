#include "Mkgrp.h"

void Analizar_Mkgrp(char* comando, int tamanio)
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
                if(strcasecmp(pivote,"-name") == 0)
                {
                    Analizar_Name(comando, &tamanio);

                }else{
                    memset(mensaje,'\0',sizeof(mensaje));
                    sprintf(mensaje,"\t*** Error!! Caracteristica MKGRP \"%s\" no reconocida ****\n",pivote);
                    Mensaje_Negacion(mensaje);
                }

                memset(pivote,'\0',sizeof(pivote));
                pos = 0;

            }else{//EN TEORIA ESTE CASO PASARA SI NO SE CUMPLE LO COMETIDO ~:~
                Mensaje_Negacion("\t*** ERROR!! la cadena \"~:~\"  esta incompleta ***\n");
                break;
            }


        }else if(comando[tamanio] == '#'){//SE ENCUENTRA CON EL INICIO DE UN COMENTARIO
            if(tamanio == 0)//SE LO ENCUENTRA AL INICIO DEL COMANDO
                return;
            //=============== AQUI VA EL METODO DE VALIDACION ==============
            Validar_Mkgrp();

            return;
        }else{
            pivote[pos] = comando[tamanio];
            pos++;
        }

        tamanio++;
    }
    Validar_Mkgrp();
}

void Validar_Mkgrp()
{
    if(strcasecmp(caracteristicas.name,"nel") != 0)//COMPRUEBA SI TIENE UN NAME GUARDADO
    {
        Verificar_Grupo();
    }else
    {
        Mensaje_Negacion("*** ERROR!! Falta Parametro obligatorio \"-name\" ***\n");
    }
}

void Verificar_Grupo()
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

            bool ret = Comprobar_Grupo(disco,sblock,datos_particion->size,datos_particion->inicio);
            if(ret)
                Mensaje_Color("=== Se creo el nuevo grupo ===\n",VERDE);
            else
                Mensaje_Color("*** ERROR!! Este grupo ya existe ***\n",ROJO);

            fclose(disco);
        }else
        {
            Mensaje_Color("*** ERROR!! No existe un particion montada con este id ***\n",ROJO);
        }
    }
}

bool Comprobar_Grupo(FILE *disco, SuperBlock sblock, int size,int inicio)
{
    // METODO PARA LEER UN ARCHIVO
    char contenido[10000];
    memset(contenido,'\0',sizeof(contenido));

    Leer_Archivo(disco,sblock,size,contenido,"/users.txt"); // OBTENGO EL CONTENIDO DE USERS

    char linea[50];
    memset(linea,'\0',sizeof(linea));
    int cont = 0;
    int index = 0;
    int cantidad = 1;

    while(contenido[cont] != '\0')
    {
        while(contenido[cont] != '\n')  // OBTENGO UNA LINEA HASTA EL SALTO DE LINEA
            linea[index++] = contenido[cont++];

        cont ++;    // AUMENTO PARA IGNORAR EL SALTO DE LINEA
        index =0;

        char delim[] = ",";

        char *ptr = strtok(linea,delim);

        char* auxiliar[6];
        memset(auxiliar,'\0',sizeof(auxiliar));

        int size=0;
        while(ptr != NULL)
        {
            auxiliar[size++] =ptr;
            ptr = strtok(NULL, delim);
        }

        if( ! strcasecmp( auxiliar[1], "g" ))
        {
            Eliminar_Espacios(auxiliar[2]);
            if( !strcmp( auxiliar[2],caracteristicas.name ) )
                return false;
            cantidad++;
        }
        memset(linea,'\0',sizeof(linea)); // LIMPIAR
    }
    // "1,G,prueba    \n"
    char auxNombre[10];
    memset(auxNombre,' ',sizeof(auxNombre));

    for(int i=0;i<10;i++)
    {
        if(caracteristicas.name[i] != '\0')
            auxNombre[i] = caracteristicas.name[i];
    }

    char buffer[16];
    memset( buffer,'\0',sizeof(buffer) );
    sprintf(buffer,"%d,G,%s",cantidad,auxNombre);
    buffer[14] = '\n'; buffer[15] = '\0';

    //printf("%s",buffer);
    // MANDO A GUARDAR MI NUEVO GRUPO
    Escribir_Archivo(disco,sblock,inicio,size,"/users.txt",buffer);

//    memset(contenido,'\0',sizeof(contenido));
//    Leer_Archivo(disco,sblock,size,contenido,"/users.txt"); // OBTENGO EL CONTENIDO DE USERS

    return true;
}











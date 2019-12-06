#include "Rem.h"

void Analizar_Rem(char* comando, int tamanio)
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
                    sprintf(mensaje,"\t*** Error!! Caracteristica REM \"%s\" no reconocida ****\n",pivote);
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
            Validar_Rem();

            return;
        }else{
            pivote[pos] = comando[tamanio];
            pos++;
        }
        tamanio++;
    }
    Validar_Rem();
}


void Validar_Rem()
{
    if(strcasecmp(caracteristicas.path,"no") != 0)//COMPRUEBA SI TIENE UN PATH GUARDADO
    {
        Ejecutar_Rem();
    }else
    {
        Mensaje_Negacion("\t*** ERROR!! Falta Parametro obligatorio \"-path\" ***\n");
    }
}

void Ejecutar_Rem()
{
    int bandera;
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

            bandera = Existe_Archivo(disco,sblock,datos_particion->size,caracteristicas.path);
            if(bandera != -1)
            {
                Eliminar_Archivo(disco,sblock,datos_particion->inicio,datos_particion->size,caracteristicas.path);
                Mensaje_Color("=== El Archivo y/o directorio fue eliminado ===\n",CYAN);
                return;
            }
//            bandera = Existe_Directorio(disco,sblock,datos_particion->size,caracteristicas.path);
//            if(bandera != -1)
//            {
//                Eliminar_Directorio(disco,sblock,datos_particion->inicio,datos_particion->size,caracteristicas.path);
//                Mensaje_Color("=== El Archivo y/o directorio fue eliminado ===\n",CYAN);
//                return;
//            }

            Mensaje_Color("*** ERROR!! El archivo o directorio a eliminar no existe ***\n",ROJO);

            fclose(disco);
        }else
        {
            Mensaje_Color("*** ERROR!! No existe un particion montada con este id ***\n",ROJO);
        }
    }
}







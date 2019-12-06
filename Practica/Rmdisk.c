#include "Rmdisk.h"

void Analizar_Rmdisk(char* comando, int tamanio)
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
                if(strcasecmp(pivote,"-path") == 0)
                {
                    Analizar_Path(comando,&tamanio);

                }else{
                    //printf("\t*** Error!! Caracteristica RMDISK \"%s\" no reconocida ****\n",pivote);
                    memset(mensaje,'\0',sizeof(mensaje));
                    sprintf(mensaje,"\t*** Error!! Caracteristica RMDISK \"%s\" no reconocida ****\n",pivote);
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
            //Imprimir_Rmdisk();
            Validar_Rmdisk();

            return;
        }else{
            pivote[pos] = comando[tamanio];
            pos++;
        }

        tamanio++;
    }

    //Imprimir_Rmdisk();
    Validar_Rmdisk();


}

void Validar_Rmdisk()
{
    if(strcasecmp(caracteristicas.path,"no") != 0)//COMPRUEBA SI TIENE UN PATH GUARDADO
    {
        Borrar_Disco();
    }else
    {
        //printf("*** ERROR!! Falta Parametro obligatorio \"-path\" ***\n");
        Mensaje_Negacion("*** ERROR!! Falta Parametro obligatorio \"-path\" ***\n");
    }
}


void Borrar_Disco()
{
    bool bandera = true;
    do
    {
        //printf("Esta seguro de querer eliminar el disco Y/N\n");
        Mensaje_Green("Esta seguro de querer eliminar el disco Y/N\n");

        char respuesta[1000];
        fgets(respuesta,1000,stdin);
        Eliminar_Salto_Linea(respuesta);

        if(strcasecmp(respuesta,"y") == 0 )
        {
            if(remove(caracteristicas.path) == 0)

                //printf("El Disco fue eliminado satisfactoriamente!!\n");
                Mensaje_Aceptacion("El Disco fue eliminado satisfactoriamente!!\n");
            else
                //printf("No se pudo eliminar el archivo!!\n");
                Mensaje_Negacion("No se pudo eliminar el archivo!!\n");

            bandera = false;
        }else if(strcasecmp(respuesta,"n") == 0)
        {
            //printf("El Disco no se elimino!!\n");
            Mensaje_Aceptacion("El Disco no se elimino!!\n");
            bandera = false;
        }else{
            //printf("*** Ingrese una opcion correcta!!! ***\n");
            Mensaje_Advertencia("*** Ingrese una opcion correcta!!! ***\n");

        }
    }while(bandera);


}

void Imprimir_Rmdisk(){

    printf("\t\tpath: %s \n",caracteristicas.path);
}

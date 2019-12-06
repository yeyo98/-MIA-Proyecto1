#include "Unmount.h"

void Analizar_Unmount(char* comando, int tamanio)
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

                }else{
                    //printf("\t*** Error!! Caracteristica UNMOUNT \"%s\" no reconocida ****\n",pivote);
                    memset(mensaje,'\0',sizeof(mensaje));
                    sprintf(mensaje,"\t*** Error!! Caracteristica UNMOUNT \"%s\" no reconocida ****\n",pivote);
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
            //Estado_Mount();
            Validar_Unmount();

            return;
        }else{
            pivote[pos] = comando[tamanio];
            pos++;
        }

        tamanio++;
    }

    //Estado_Mount();
    Validar_Unmount();
}

void Validar_Unmount()
{
    if(strcasecmp(caracteristicas.id,"nel") != 0)//COMPRUEBA SI TIENE UN ID GUARDADO
    {
        Quitar_Disco();

    }else
    {
     //printf("*** ERROR!! Falta Parametro obligatorio \"-id\" ***\n");
     Mensaje_Negacion("*** ERROR!! Falta Parametro obligatorio \"-id\" ***\n");
    }
}

void Quitar_Disco()
{
    Desmontar_Disco(caracteristicas.id);
}
















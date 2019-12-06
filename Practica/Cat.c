#include "Cat.h"

void Analizar_Cat(char* comando, int tamanio)
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
                    sprintf(mensaje,"\t*** Error!! Caracteristica CAT \"%s\" no reconocida ****\n",pivote);
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
            //Validar_Rem();

            return;
        }else{
            pivote[pos] = comando[tamanio];
            pos++;
        }
        tamanio++;
    }
    //Validar_Rem();
}

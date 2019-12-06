#include "Exec.h"

/*================ NOTA =================
 *
 *     VARIABLE CARACTERISTICA DEL STRUCT DATO LO SACO DE ESTRUCTURAS.H
 *
 * ======================================
 */

bool haySalto = false;

void Analizar_Exec(char* comando, int tamanio)
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
                    printf("*** Error!! Caracteristica EXEC \"%s\" no reconocida ****\n",pivote);
                }

                memset(pivote,'\0',sizeof(pivote));
                pos = 0;

            }else{//EN TEORIA ESTE CASO PASARA SI NO SE CUMPLE LO COMETIDO ~:~
                printf("*** ERROR!! la cadena \"~:~\"  esta incompleta ***\n");
                break;
            }


        }else if(comando[tamanio] == '#'){//SE ENCUENTRA CON EL INICIO DE UN COMENTARIO
            if(tamanio == 0)//SE LO ENCUENTRA AL INICIO DEL COMANDO
                return;
            //Elegir_Comando(pivote, comando, tamanio+1);
            return;
        }else{
            pivote[pos] = comando[tamanio];
            pos++;
        }

        tamanio++;
    }

    Validar_Exec();

}

void Validar_Exec(void)
{
    if(strcmp(caracteristicas.path,"no") != 0)
    {
        Abrir_Archivo();
    }else{
        printf("*** ERROR!! Falta Parametro obligatorio \"-path\" ***\n");
    }
}

void Abrir_Archivo()
{
    FILE* archivo;
    char texto[5000];
    memset(texto,'\0',sizeof(texto));

    archivo = fopen(caracteristicas.path,"r");

    if(archivo == NULL){
        printf("*** ERROR!! No existe archivo con la siguiente ruta \"%s\" ***\n", caracteristicas.path);
    }

    while(fgets(texto,5000,archivo) != NULL)
    {
        printf("%s",texto);//IMPRIMO LINEA POR LINEA DEL ARCHIVO

        Hay_Salto(texto);
        while(haySalto){
            char auxBuffer[1000];
            fgets(auxBuffer,1000,archivo);
            printf("%s",auxBuffer);
            strcat(texto," ");
            strcat(texto,auxBuffer);

            Hay_Salto(texto);
        }

        int tamanio = 0;
        Contar_Comando(texto, &tamanio);
        Analizar_Comando(texto, tamanio);

        memset(texto,'\0',sizeof(texto));
    }

    fclose(archivo);

}


void Hay_Salto(char* cadena)
{
    char *p;
    p =  strchr(cadena,  '\\');
    if(p)
    {
        *p = '\0';
        haySalto = true;
    }else{
        haySalto = false;
    }
}

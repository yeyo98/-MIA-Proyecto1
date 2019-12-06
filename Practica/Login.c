#include "Login.h"

void Analizar_Login(char* comando, int tamanio)
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

                }else if(strcasecmp(pivote,"-usr") == 0){

                    Analizar_Usr(comando,&tamanio);

                }else if(strcasecmp(pivote,"-pwd") == 0){

                    Analizar_Pwd(comando,&tamanio);

                }else{
                    //printf("\t*** Error!! Caracteristica MKFS \"%s\" no reconocida ****\n",pivote);
                    memset(mensaje,'\0',sizeof(mensaje));
                    sprintf(mensaje,"\t*** Error!! Caracteristica Login \"%s\" no reconocida ****\n",pivote);
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
            Validar_Login();

            return;
        }else{
            pivote[pos] = comando[tamanio];
            pos++;
        }

        tamanio++;
    }

    //Estado_Mkfs();
    Validar_Login();
}

void Validar_Login()
{
    if(strcasecmp(caracteristicas.usr,"nel") != 0)//COMPRUEBA SI TIENE UN USR GUARDADO
    {
        if(strcasecmp(caracteristicas.pwd,"nel") != 0)//COMPRUEBA SI TIENE UN PWD GUARDADO
        {
            if(strcasecmp(caracteristicas.id,"nel") != 0)//COMPRUEBA SI TIENE UN ID GUARDADO
            {
                Verificar_Login();

            }else
            {
                Mensaje_Color("*** ERROR!! Falta Parametro obligatorio \"-id\" ***\n",ROJO);
            }
        }else
        {
            Mensaje_Color("*** ERROR!! Falta Parametro obligatorio \"-pwd\" ***\n",ROJO);
        }
    }else
    {
        Mensaje_Color("*** ERROR!! Falta Parametro obligatorio \"-urs\" ***\n",ROJO);
    }
}


void Verificar_Login()
{
    int parseo = -1;
    if(Validar_Numero(&caracteristicas.id[3]))
    {
        parseo = atoi(&caracteristicas.id[3]);
        int valor = Get_Inicio(caracteristicas.id[2],parseo);
        if(valor != -1)
        {
            char* ruta = Get_Path(caracteristicas.id[2],parseo);

            FILE* disco = fopen(ruta,"r+b");
            if(disco == NULL)
                    return;
            Nodo* datos_particion = Get_Mount(ruta);

            SuperBlock sblock;
            fseek(disco,datos_particion->inicio,SEEK_SET);
            fread(&sblock,sizeof(SuperBlock),1,disco);//SE LEE EL DISCO CON EL TAMAÑO DEL SUPERBLOQUE

            bool ret = Comprobar_Login(disco,sblock,datos_particion->size);
            if(ret)
                Mensaje_Color("=== EL USUARIO A SIDO LOGUEADO ===\n",VERDE);
            else
                Mensaje_Color("*** ERROR!! El usuario o contrasenia no coinciden o no existen\n",ROJO);

            fclose(disco);
        }else
        {
            Mensaje_Color("*** ERROR!! No existe un particion montada con este id ***\n",ROJO);
        }
    }
}

bool Comprobar_Login(FILE* disco,SuperBlock sblock,int size)
{
    // METODO PARA LEER UN ARCHIVO
    char contenido[10000];
    memset(contenido,'\0',sizeof(contenido));

    Leer_Archivo(disco,sblock,size,contenido,"/users.txt"); // OBTENGO EL CONTENIDO DE USERS
    //printf("%s\n",contenido);

    char linea[50];
    memset(linea,'\0',sizeof(linea));
    int cont = 0;
    int index = 0;

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

        if( ! strcasecmp( auxiliar[1], "u" ))
        {
            Eliminar_Espacios(auxiliar[3]); Eliminar_Espacios(auxiliar[4]);
            if( !strcmp( auxiliar[3],caracteristicas.usr ) && !strcmp( auxiliar[4],caracteristicas.pwd ))
            {
                strcpy(daLoguer.usr,caracteristicas.usr);
                strcpy(daLoguer.pwd,caracteristicas.pwd);
                strcpy(daLoguer.id,caracteristicas.id);
                strcpy(daLoguer.grp,auxiliar[2]);

                daLoguer.usuario = atoi(auxiliar[0]);                   // ID DEL USUARIO

                Eliminar_Espacios(auxiliar[2]);
                daLoguer.grupo = Get_Id_Group(contenido,auxiliar[2]);   // ID DEL GRUPO
                return true;
            }
        }
        memset(linea,'\0',sizeof(linea)); // LIMPIAR
    }
    return false;
}


int Get_Id_Group(char*contenido,char* grupo)
{

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
            if( !strcmp( auxiliar[2],grupo ) )
                return atoi(auxiliar[0]);
            cantidad++;
        }
        memset(linea,'\0',sizeof(linea)); // LIMPIAR
    }
    return 1;
}






















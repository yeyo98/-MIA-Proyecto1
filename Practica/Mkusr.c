#include "Mkusr.h"

void Analizar_Mkusr(char* comando, int tamanio)
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
                if(strcasecmp(pivote,"-grp") == 0){

                    Analizar_Grp(comando,&tamanio);

                }else if(strcasecmp(pivote,"-usr") == 0){

                    Analizar_Usr(comando,&tamanio);

                }else if(strcasecmp(pivote,"-pwd") == 0){

                    Analizar_Pwd(comando,&tamanio);

                }else{
                    memset(mensaje,'\0',sizeof(mensaje));
                    sprintf(mensaje,"\t*** Error!! Caracteristica MKUSR \"%s\" no reconocida ****\n",pivote);
                    Mensaje_Color(mensaje,ROJO);
                }

                memset(pivote,'\0',sizeof(pivote));
                pos = 0;

            }else{//EN TEORIA ESTE CASO PASARA SI NO SE CUMPLE LO COMETIDO ~:~
                Mensaje_Color("\t*** ERROR!! la cadena \"~:~\"  esta incompleta ***\n",ROJO);
                break;
            }


        }else if(comando[tamanio] == '#'){//SE ENCUENTRA CON EL INICIO DE UN COMENTARIO
            if(tamanio == 0)//SE LO ENCUENTRA AL INICIO DEL COMANDO
                return;
            //=============== AQUI VA EL METODO DE VALIDACION ==============
            Validar_Mkusr();

            return;
        }else{
            pivote[pos] = comando[tamanio];
            pos++;
        }

        tamanio++;
    }
    Validar_Mkusr();
}

void Validar_Mkusr()
{
    if(strcasecmp(caracteristicas.usr,"nel") != 0)//COMPRUEBA SI TIENE UN USR GUARDADO
    {
        if(strcasecmp(caracteristicas.pwd,"nel") != 0)//COMPRUEBA SI TIENE UN PWD GUARDADO
        {
            if(strcasecmp(caracteristicas.grp,"nel") != 0)//COMPRUEBA SI TIENE UN ID GUARDADO
            {
                Verificar_Usuario();

            }else
            {
                Mensaje_Color("*** ERROR!! Falta Parametro obligatorio \"-grp\" ***\n",ROJO);
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

void Verificar_Usuario()
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

            bool ret = Comprobar_Usuario(disco,sblock,datos_particion->size,datos_particion->inicio);
            if(ret)
                Mensaje_Color("=== Se creo el nuevo Usuario ===\n",VERDE);
            else
                Mensaje_Color("*** ERROR!! Este Usuario ya existe ***\n",ROJO);

            fclose(disco);
        }else
        {
            Mensaje_Color("*** ERROR!! No existe un particion montada con este id ***\n",ROJO);
        }
    }
}

bool Comprobar_Usuario(FILE *disco, SuperBlock sblock, int size, int inicio)
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

        if( ! strcasecmp( auxiliar[1], "u" ))
        {
            Eliminar_Espacios(auxiliar[3]);
            if( !strcmp( auxiliar[3],caracteristicas.usr ) )
                return false;
            cantidad++;
        }
        memset(linea,'\0',sizeof(linea)); // LIMPIAR
    }

    if( Existe_Group(contenido,caracteristicas.grp) )     // VERIFICO SI EXISTE LE GRUPO
    {
        // "1,U,prueba    ,prueba    ,123       \n"
        char auxGrupo[11];
        memset(auxGrupo,' ',sizeof(auxGrupo));

        for(int i=0;i<10;i++)
        {
            if(caracteristicas.grp[i] != '\0')
                auxGrupo[i] = caracteristicas.grp[i];
        }
        auxGrupo[10] = '\0';

        char auxUsuario[11];
        memset(auxUsuario,' ',sizeof(auxUsuario));

        for(int i=0;i<10;i++)
        {
            if(caracteristicas.usr[i] != '\0')
                auxUsuario[i] = caracteristicas.usr[i];
        }
        auxUsuario[10] = '\0';

        char auxPassword[11];
        memset(auxPassword,' ',sizeof(auxPassword));

        //printf("%s",caracteristicas.pwd);

        for(int i=0;i<10;i++)
        {
            if(caracteristicas.pwd[i] != '\0')
                auxPassword[i] = caracteristicas.pwd[i];
        }
        auxPassword[10] = '\0';

        // ME QUDE ANALIZANDO LOS NOMBRES PARA LOS USUARIOS

        char buffer[38];
        memset( buffer,'\0',sizeof(buffer) );
        sprintf(buffer,"%d,U,%s,%s,%s",cantidad,auxGrupo,auxUsuario,auxPassword);
        buffer[36] = '\n'; buffer[37] = '\0';

        // MANDO A GUARDAR MI NUEVO USUARIO
        Escribir_Archivo(disco,sblock,inicio,size,"/users.txt",buffer);
        return true;
    }
    return false;
}


bool Existe_Group(char*contenido,char* grupo)
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
            Eliminar_Espacios(grupo);
            if( !strcmp( auxiliar[2],grupo ) )
                return true;
            cantidad++;
        }
        memset(linea,'\0',sizeof(linea)); // LIMPIAR
    }
    return false;
}



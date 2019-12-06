#include "Mount.h"


void Analizar_Mount(char* comando, int tamanio)
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

                }else if(strcasecmp(pivote,"-name") == 0)
                {
                    Analizar_Name(comando, &tamanio);

                }else{
                    //printf("\t*** Error!! Caracteristica MOUNT \"%s\" no reconocida ****\n",pivote);
                    memset(mensaje,'\0',sizeof(mensaje));
                    sprintf(mensaje,"\t*** Error!! Caracteristica MOUNT \"%s\" no reconocida ****\n",pivote);
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
            Validar_Mount();

            return;
        }else{
            pivote[pos] = comando[tamanio];
            pos++;
        }

        tamanio++;
    }

    //Estado_Mount();
    Validar_Mount();
}

void Validar_Mount()
{
    if(strcasecmp(caracteristicas.name,"nel") != 0)//COMPRUEBA SI TIENE UN NAME GUARDADO
    {
        if(strcasecmp(caracteristicas.path,"no") != 0)//COMPRUEBA SI TIENE UN PATH GUARDADO
        {
            Montar_Disco();
        }else
        {
            //printf("*** ERROR!! Falta Parametro obligatorio \"-path\" ***\n");
            Mensaje_Negacion("*** ERROR!! Falta Parametro obligatorio \"-path\" ***\n");
        }
    }else
    {
     //printf("*** ERROR!! Falta Parametro obligatorio \"-name\" ***\n");
     Mensaje_Negacion("*** ERROR!! Falta Parametro obligatorio \"-name\" ***\n");
    }
}



void Montar_Disco()
{
    LLenar_Arreglo();

    MBR mbr;

    FILE* disco;
    disco = fopen(caracteristicas.path,"r+b");

    if(disco == NULL){
        //printf("ERROR!! A la hora de abrir el disco, Verifique la ruta o si existe el disco\n");
        Mensaje_Negacion("ERROR!! A la hora de abrir el disco, Verifique la ruta o si existe el disco\n");
        return;
    }

    fread(&mbr,sizeof(MBR),1,disco);


    if( Nombre_noRepetido(mbr,disco) )//SI ES TRUE EXITE, SI NO! NO HAY UNA PARTICION CON ESE NOMBRE
    {
        Particion part;
        EBR ebr;

        bool bandera = false;

        for(int i=0; i<4 ;i++)
        {
            part = mbr.mbr_particion[i];

            if(part.part_type == 'e') //EN DADO CASO DE QUE SEA EXTENDIDA
            {

                if(strcmp(part.part_name , caracteristicas.name) == 0)
                {
                    bandera = true; break;
                }

                fseek(disco,part.part_start,SEEK_SET); //ME POSICIONO EN DONDE COMIENZA LAS LOGICAS

                int finPart = part.part_size + part.part_start;
                int corrida = 0;
                int pesoEBR = sizeof(EBR);

                fread(&ebr,sizeof(EBR),1,disco);//CONSIGO EL EBR INICIAL

                corrida = pesoEBR + ebr.part_start;

                while(corrida < finPart)
                {
                    if(strcasecmp( ebr.part_name, caracteristicas.name )==0 && ebr.part_status != 0)
                    {
                        break;
                    }else
                    {
                        if(ebr.part_next != 0)//EN TEORIA SI YA NO HAY SIGUIENTE LLEGO AL FINAL DE LA LISTA
                        {
                            int espacio = ebr.part_next;
                            corrida = pesoEBR + espacio;
                            fseek(disco, espacio, SEEK_SET);
                            fread(&ebr,sizeof(EBR),1,disco);//CONSIGO EL EBR SIGUIENTE
                        }else
                        {
                            break;
                        }
                    }
                }



            }else //SI NO EN DADO CASO DE QUE SEA PRIMARIA
            {
                if(strcmp(part.part_name , caracteristicas.name) == 0)
                {
                    bandera = true; break;
                }
            }

        }

        Nodo* aux = Get_Mount(caracteristicas.path);

        if(bandera)
        {
            if(aux->inicio == -1)//SI ES -1 NO ESTA EL DISCO DISCO, SI ES != -1 ESE DISCO YA ESTA
            {
                Nodo* nuevo = (Nodo*)malloc(sizeof(Nodo));
                memset(nuevo->path,'\0',sizeof(nuevo->path));
                strcpy(nuevo->path , caracteristicas.path);
                memset(nuevo->name,'\0',sizeof(nuevo->name));
                strcpy(nuevo->name , caracteristicas.name);
                nuevo->inicio = part.part_start;
                nuevo->letra = letra[controlLetra];
                nuevo->size = part.part_size;
                nuevo->type = part.part_type;
                //nuevo->id[0] = 'v';
                //nuevo->id[1] = 'd';
                //nuevo->id[2] = letra[controlLetra];
                //nuevo->id[3] = '1';
                nuevo->num = 1;

                Insertar_Disco(nuevo);

            }else
            {
                Bnodo* viejo = Get_Bitacora(caracteristicas.path);

                Nodo* nuevo = (Nodo*)malloc(sizeof(Nodo));
                memset(nuevo->path,'\0',sizeof(nuevo->path));
                strcpy(nuevo->path , caracteristicas.path);
                memset(nuevo->name,'\0',sizeof(nuevo->name));
                strcpy(nuevo->name , caracteristicas.name);
                nuevo->inicio = part.part_start;
                nuevo->letra = viejo->letra;
                nuevo->size = part.part_size;
                nuevo->type = part.part_type;
                //nuevo->id[0] = 'v';
                //nuevo->id[1] = 'd';
                //nuevo->id[2] = viejo->letra;

                int b = viejo->cont;
                //printf("el b: %d\n",b);
                //nuevo->id[3] = b++;
                nuevo->num = b+1;
                Insertar_Disco(nuevo);

            }

        }else
        {
            if(aux->inicio == -1)//SI ES -1 NO ESTA EL DISCO DISCO, SI ES != -1 ESE DISCO YA ESTA
            {
                Nodo* nuevo = (Nodo*)malloc(sizeof(Nodo));
                memset(nuevo->path,'\0',sizeof(nuevo->path));
                strcpy(nuevo->path , caracteristicas.path);
                memset(nuevo->name,'\0',sizeof(nuevo->name));
                strcpy(nuevo->name , caracteristicas.name);
                nuevo->inicio = ebr.part_start;
                nuevo->letra = letra[controlLetra];
                nuevo->size = ebr.part_size;
                nuevo->type = 'l';
                nuevo->num = 1;

                Insertar_Disco(nuevo);

            }else
            {
                Bnodo* viejo = Get_Bitacora(caracteristicas.path);

                Nodo* nuevo = (Nodo*)malloc(sizeof(Nodo));
                memset(nuevo->path,'\0',sizeof(nuevo->path));
                strcpy(nuevo->path , caracteristicas.path);
                memset(nuevo->name,'\0',sizeof(nuevo->name));
                strcpy(nuevo->name , caracteristicas.name);
                nuevo->inicio = ebr.part_start;
                nuevo->letra = viejo->letra;
                nuevo->size = ebr.part_size;
                nuevo->type = 'l';

                int b = viejo->cont;
                nuevo->num = b+1;
                Insertar_Disco(nuevo);

            }
        }

    }else
    {
        //printf("*** ERROR!! No exite ninguna particion con este nombre ***\n");
        Mensaje_Negacion("*** ERROR!! No exite ninguna particion con este nombre ***\n");
    }



    fclose(disco);
}


void LLenar_Arreglo()
{
    int a = 97;
    for(int i=0;i<26;i++)
        letra[i] = a++;

}

void Estado_Mount()
{
    printf("\t\tpath: %s \n",caracteristicas.path);
    printf("\t\tname: %s \n",caracteristicas.name);
}

#include "Fdisk.h"
//#define RED     "\x1b[31m"
//#define GREEN   "\x1b[32m"
//#define RESET   "\x1b[0m"

void Analizar_Fdisk(char* comando, int tamanio)
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
                if(strcasecmp(pivote,"-size") == 0){

                    Analizar_Size(comando,&tamanio);

                }else if(strcasecmp(pivote,"-fit") == 0){
                    Analizar_Fit(comando,&tamanio);

                }else if(strcasecmp(pivote,"-unit") == 0){
                    Analizar_Unit_F(comando,&tamanio);

                }else if(strcasecmp(pivote,"-path") == 0){
                    Analizar_Path(comando,&tamanio);

                }else if(strcasecmp(pivote,"-type") == 0)
                {
                    Analizar_Type(comando,&tamanio);

                }else if(strcasecmp(pivote,"-delete") == 0)
                {
                    Analizar_Delete(comando, &tamanio);

                }else if(strcasecmp(pivote,"-name") == 0)
                {
                    Analizar_Name(comando, &tamanio);

                }else if(strcasecmp(pivote,"-add") == 0)
                {
                    Analizar_Add(comando, &tamanio);

                }else{
                    //printf("\t*** Error!! Caracteristica FDISK \"%s\" no reconocida ****\n",pivote);
                    memset(mensaje,'\0',sizeof(mensaje));
                    sprintf(mensaje,"\t*** Error!! Caracteristica FDISK \"%s\" no reconocida ****\n",pivote);
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
            //Estado_Fdisk();
            Validar_Fdisk();

            return;
        }else{
            pivote[pos] = comando[tamanio];
            pos++;
        }

        tamanio++;
    }

    //Estado_Fdisk();
    Validar_Fdisk();
}


void Estado_Fdisk()
{
    printf("\t\tsize: %d \n",caracteristicas.size);
    printf("\t\tfit: %c \n",caracteristicas.fit_f);
    printf("\t\tunit: %c \n",caracteristicas.unit_f);
    printf("\t\tpath: %s \n",caracteristicas.path);

    printf("\t\ttype: %c \n",caracteristicas.type);
    printf("\t\tdelete: %c \n",caracteristicas.deleteP);
    printf("\t\tadd: %d \n",caracteristicas.add);
    printf("\t\tname: %s \n",caracteristicas.name);
}

void Validar_Fdisk()
{
    if(strcasecmp(caracteristicas.path,"no") != 0)
    {
        if(caracteristicas.add != 0) //EN EL CASO DE AGREGAR O QUITAR A UNA PARTICION
        {

                Particion_Add(true);

        }else if(caracteristicas.deleteP != '\0')//EN EL CASO DE ELIMINAR UNA PARTICION
        {
            if(strcasecmp(caracteristicas.name,"nel") != 0)
            {
                if(caracteristicas.deleteP != '\0')
                {
                    Particion_Delete(true);
                }
                else
                {
                    //printf("\t*** ERROR!! Falta Parametro obligatorio \"fast o full\" para eliminar la Particion ***\n");
                    Mensaje_Negacion("\t*** ERROR!! Falta Parametro obligatorio \"fast o full\" para eliminar la Particion ***\n");
                }
            }else
            {
                //printf("\t*** ERROR!! Falta Parametro obligatorio \"-name\" para eliminar la Particion ***\n");
                Mensaje_Negacion("\t*** ERROR!! Falta Parametro obligatorio \"-name\" para eliminar la Particion ***\n");
            }

        }else//EN EL CASO DE CREAR PARTICION
        {
            if(caracteristicas.size != -1)
            {
                Crear_Particion(true);
            }
            else
            {
                //printf("\t*** ERROR!! Falta Parametro obligatorio \"-size\" para crear la Particion ***\n");
                Mensaje_Negacion("\t*** ERROR!! Falta Parametro obligatorio \"-size\" para crear la Particion ***\n");
            }
        }
    }else
    {
        //printf("\t*** ERROR!! Falta Parametro obligatorio \"-path\" ***\n");
        Mensaje_Negacion("\t*** ERROR!! Falta Parametro obligatorio \"-path\" ***\n");
    }
}

Particion Llenar_Particion(int inicio)
{
    Particion r;

    r.part_status = '1';
    r.part_type = caracteristicas.type;
    r.part_fit = caracteristicas.fit_f;
    r.part_start = inicio+1;
    //r.part_start = inicio;

    r.part_size = Calcular_Espacio();

    strcat( r.part_name, caracteristicas.name );

    return r;
}


void Crear_Particion(bool raid)
{
    MBR mbr;

    FILE* disco;
    disco = fopen(caracteristicas.path,"r+b");

    if(disco == NULL){
        //printf("ERROR!! A la hora de abrir el disco, Verifique la ruta o si existe el disco\n");
        if(raid)
            Mensaje_Negacion("ERROR!! A la hora de abrir el disco, Verifique la ruta o si existe el disco\n");
        return;
    }

    fread(&mbr,sizeof(MBR),1,disco);

    //printf("%d\n",mbr.mbr_tamanio);


    int pesoMBR = sizeof(mbr);
    bool cabeParticion = false;
    int sizeParticion = Calcular_Espacio(); //TAMAÑO DE LA PARTICION DESEADA

    int corrida = pesoMBR;
    int noEspacio = false;

    bool auxLogica = false;     //VARIABLE AUXILIAR PARA EXTENDIDA
    int posLogica = false;      //VARIABLE AUXILIAR PARA POSICION EN EXTENDIDA

    //COMPRUEBO QUE TIPO DE PARTICION SE DESEA
    switch(caracteristicas.type)
    {
        //============== PARTICION PRIMARIA ============
        case 'p':
            if(mbr.disk_fit == 'f')    //EN LA PRIMERA QUE QUEPA
            {
                //BUSCO QUE POSICION ESTA DISPONIBLE
                for(int i=0; i<4 ;i++)
                {
                    if(mbr.mbr_particion[i].part_status == '0')//SIGNIFICA QUE ESTA DISPONIBLE ESA PARTICION
                    {
                        bool isParticion = false;
                        int nextParticion=0;        //INICIO DE LA SIGUIENTE PARTICION

                        for(int j=i+1; j<4 ;j++)
                        {
                            if(mbr.mbr_particion[j].part_status != '0')//BUSCA SI HAY OTRA PARTICION MAS ADELANTE
                            {
                                isParticion = true;
                                nextParticion = mbr.mbr_particion[j].part_start;
                                break;
                            }
                        }

                        if(isParticion)//HAY PARTICION ADELANTE
                        {
                            int resto = nextParticion - corrida;

                            if(sizeParticion <= resto )//VERIFICO SI AUN HAY ESPACIO
                            {
                                if(!Nombre_noRepetido(mbr,disco))
                                {
                                    mbr.mbr_particion[i] = Llenar_Particion(corrida);
                                    rewind(disco);

                                    fwrite(&mbr,sizeof(mbr),1,disco);
                                    //fseek(disco,mbr.mbr_particion[i].part_start,SEEK_SET);
                                    //fwrite(&mbr.mbr_particion[i] , sizeof(Particion), 1 ,disco);


                                    //printf("\t=== Se Creo la particion \"%s\" Exitosamente!! ===\n",caracteristicas.name);
                                    memset(mensaje,'\0',sizeof(mensaje));
                                    sprintf(mensaje,"\t=== Se Creo la particion \"%s\" Exitosamente!! ===\n",caracteristicas.name);
                                    if(raid)
                                        Mensaje_Aceptacion(mensaje);

                                    noEspacio = true;
                                    break;
                                }else
                                {
                                    noEspacio = true;
                                    //printf("\t*** ERROR!! No se creo la particion \"%s\" no se puede repetir nombres en un mismo disco!! ***\n",caracteristicas.name);
                                    memset(mensaje,'\0',sizeof(mensaje));
                                    sprintf(mensaje,"\t*** ERROR!! No se creo la particion \"%s\" no se puede repetir nombres en un mismo disco!! ***\n",caracteristicas.name);
                                    if(raid)
                                        Mensaje_Negacion(mensaje);
                                }
                            }else
                            {
                                if(i == 3){
                                    noEspacio = true;
                                    //printf("\t*** ERROR!! No se creo la particion \"%s\" no alcanza el espacio!! ***\n",caracteristicas.name);
                                    memset(mensaje,'\0',sizeof(mensaje));
                                    sprintf(mensaje,"\t*** ERROR!! No se creo la particion \"%s\" no alcanza el espacio!! ***\n",caracteristicas.name);
                                    if(raid)
                                        Mensaje_Negacion(mensaje);
                                }
                            }

                        }else //NO HAY PARTICION ADELANTE
                        {
                            int resto = mbr.mbr_tamanio - corrida;

                            if(sizeParticion <= resto )//VERIFICO SI AUN HAY ESPACIO
                            {
                                if(!Nombre_noRepetido(mbr,disco) )
                                {
                                    mbr.mbr_particion[i] = Llenar_Particion(corrida);
                                    rewind(disco);

                                    fwrite(&mbr,sizeof(mbr),1,disco);
                                    //fseek(disco,mbr.mbr_particion[i].part_start,SEEK_SET);
                                    //fwrite(&mbr.mbr_particion[i] , sizeof(Particion), 1 ,disco);
                                    //printf("\t=== Se Creo la particion \"%s\" Exitosamente!! ===\n",caracteristicas.name);
                                    memset(mensaje,'\0',sizeof(mensaje));
                                    sprintf(mensaje,"\t=== Se Creo la particion \"%s\" Exitosamente!! ===\n",caracteristicas.name);
                                    if(raid)
                                        Mensaje_Aceptacion(mensaje);

                                    noEspacio = true;
                                    break;
                                }else
                                {
                                    noEspacio = true;
                                    //printf("\t*** ERROR!! No se creo la particion \"%s\" no se puede repetir nombres en un mismo disco!! ***\n",caracteristicas.name);
                                    memset(mensaje,'\0',sizeof(mensaje));
                                    sprintf(mensaje,"\t*** ERROR!! No se creo la particion \"%s\" no se puede repetir nombres en un mismo disco!! ***\n",caracteristicas.name);
                                    if(raid)
                                        Mensaje_Negacion(mensaje);
                                }
                            }else
                            {
                                noEspacio = true;
                                //printf("\t*** ERROR!! No se creo la particion \"%s\" no alcanza el espacio!! ***\n",caracteristicas.name);
                                memset(mensaje,'\0',sizeof(mensaje));
                                sprintf(mensaje,"\t*** ERROR!! No se creo la particion \"%s\" no alcanza el espacio!! ***\n",caracteristicas.name);
                                if(raid)
                                    Mensaje_Advertencia(mensaje);
                            }

                        }

                    }else
                    {
                        //corrida+= mbr.mbr_particion[i].part_size;
                        corrida = mbr.mbr_particion[i].part_start + mbr.mbr_particion[i].part_size;
                        if(i == 3){
                            cabeParticion = true;
                            noEspacio = true;
                        }
                    }
                }

            }
            else if(mbr.disk_fit == 'w')  //EN EL ESPACIO MAS GRANDE QUE QUEPA
            {
                int nextParticion=0;        //INICIO DE LA SIGUIENTE PARTICION
                int tamanioMayor = 0;
                int startMayor = 0;
                int posParticion = 0;
                int resto = 0;

                bool bandera = false; //ME INDICA SI NO HAY PARTICIONES

                for(int i=0; i<4 ;i++) //BUSCO QUE POSICION ESTA DISPONIBLE
                {

                    if(mbr.mbr_particion[i].part_status == '0')//SIGNIFICA QUE ESTA DISPONIBLE ESA PARTICION
                    {

                        for(int j=i+1; j<4 ;j++)//RECORRO EL RESTO DE PARTICIONES
                        {
                            if(mbr.mbr_particion[j].part_status != '0')//BUSCA SI HAY OTRA PARTICION MAS ADELANTE
                            {
                                bandera = true;
                                nextParticion = mbr.mbr_particion[j].part_start;


                                resto = nextParticion - corrida;
                                if( resto > tamanioMayor)//CALCULO SI EL NUEVO ESPACIO ES MAS GRANDE DEL YA GUARDADO
                                {
                                    tamanioMayor = nextParticion - corrida;
                                    posParticion = i;
                                    //EN TEORIA LA CORRIDA SERIA EL PUNTO DE ORIGEN
                                }
                                //break;
                            }
                        }

                        if(!bandera)//EN TEORIA SOLO ENTRADA SI TODO LO DEMAS ESTA VACIO
                        {
                            posParticion = i;

                            //if(i == 0)//ME INDICA QUE ES EL INICIO
                            //    resto = mbr.mbr_tamanio - corrida;
                            resto = mbr.mbr_tamanio - corrida;

                            break;
                        }

                        if(i == 3)//ESTA CONDICION SE CUMPLIRA SOLO CON LA ULTIMA PARTICION VACIA
                        {
                            resto = mbr.mbr_tamanio - corrida;
                            if( resto > tamanioMayor)//CALCULO SI EL NUEVO ESPACIO ES MAS GRANDE DEL YA GUARDADO
                            {
                                tamanioMayor = mbr.mbr_tamanio - corrida;
                                posParticion = i;
                                //EN TEORIA LA CORRIDA SERIA EL PUNTO DE ORIGEN
                            }
                        }

                    }else
                    {
                        //corrida+= mbr.mbr_particion[i].part_size;
                        corrida = mbr.mbr_particion[i].part_start + mbr.mbr_particion[i].part_size;
                        if(i == 3 && tamanioMayor != 0){
                            cabeParticion = true;
                            noEspacio = true;
                        }
                    }
                }

                    if(sizeParticion <= resto )//VERIFICO SI AUN HAY ESPACIO
                    {
                        if(!Nombre_noRepetido(mbr,disco) )
                        {
                            mbr.mbr_particion[posParticion] = Llenar_Particion(corrida);
                            rewind(disco);

                            fwrite(&mbr,sizeof(mbr),1,disco);

                            //fseek(disco,mbr.mbr_particion[posParticion].part_start,SEEK_SET);
                            //fwrite(&mbr.mbr_particion[posParticion] , sizeof(Particion), 1 ,disco);

                            //printf("\t=== Se Creo la particion \"%s\" Exitosamente!! ===\n",caracteristicas.name);
                            memset(mensaje,'\0',sizeof(mensaje));
                            sprintf(mensaje,"\t=== Se Creo la particion \"%s\" Exitosamente!! ===\n",caracteristicas.name);
                            if(raid)
                                Mensaje_Aceptacion(mensaje);

                            noEspacio = true;
                            break;
                        }else{

                            noEspacio = true;
                            //printf("\t*** ERROR!! No se creo la particion \"%s\" no se puede repetir nombres en un mismo disco!! ***\n",caracteristicas.name);
                            memset(mensaje,'\0',sizeof(mensaje));
                            sprintf(mensaje,"\t*** ERROR!! No se creo la particion \"%s\" no se puede repetir nombres en un mismo disco!! ***\n",caracteristicas.name);
                            if(raid)
                                Mensaje_Negacion(mensaje);
                        }
                    }else
                    {
                        noEspacio = true;
                        //printf("\t*** ERROR!! No se creo la particion \"%s\" no alcanza el espacio!! ***\n",caracteristicas.name);
                        memset(mensaje,'\0',sizeof(mensaje));
                        sprintf(mensaje,"\t*** ERROR!! No se creo la particion \"%s\" no alcanza el espacio!! ***\n",caracteristicas.name);
                        if(raid)
                            Mensaje_Advertencia(mensaje);
                    }


            }else if(mbr.disk_fit == 'b')  //EN EL ESPACIO MAS PEQUEÑO QUE QUEPA
            {
                int nextParticion=0;        //INICIO DE LA SIGUIENTE PARTICION
                int tamanioMayor = 0;
                int posParticion = 0;
                int resto = 0;

                bool bandera = false; //ME INDICA SI NO HAY PARTICIONES

                for(int i=0; i<4 ;i++) //BUSCO QUE POSICION ESTA DISPONIBLE
                {

                    if(mbr.mbr_particion[i].part_status == '0')//SIGNIFICA QUE ESTA DISPONIBLE ESA PARTICION
                    {

                        for(int j=i+1; j<4 ;j++)//RECORRO EL RESTO DE PARTICIONES
                        {
                            if(mbr.mbr_particion[j].part_status != '0')//BUSCA SI HAY OTRA PARTICION MAS ADELANTE
                            {
                                bandera = true;
                                nextParticion = mbr.mbr_particion[j].part_start;


                                resto = nextParticion - corrida;
                                if( resto < tamanioMayor && sizeParticion <= resto)//CALCULO SI EL NUEVO ESPACIO ES MAS GRANDE DEL YA GUARDADO
                                {
                                    tamanioMayor = nextParticion - corrida;
                                    posParticion = i;
                                    //EN TEORIA LA CORRIDA SERIA EL PUNTO DE ORIGEN
                                }else if(tamanioMayor == 0)
                                {
                                    tamanioMayor = nextParticion - corrida;
                                    posParticion = i;
                                }
                                //break;
                            }
                        }

                        if(!bandera)//EN TEORIA SOLO ENTRARA SI TODO LO DEMAS ESTA VACIO
                        {
                            posParticion = i;

                            //if(i == 0)//ME INDICA QUE ES EL INICIO
                            //    resto = mbr.mbr_tamanio - corrida;
                            resto = mbr.mbr_tamanio - corrida;

                            break;
                        }

                        if(i == 3)//ESTA CONDICION SE CUMPLIRA SOLO CON LA ULTIMA PARTICION VACIA
                        {
                            resto = mbr.mbr_tamanio - corrida;
                            if( resto < tamanioMayor && sizeParticion <= resto)//CALCULO SI EL NUEVO ESPACIO ES MAS GRANDE DEL YA GUARDADO
                            {
                                tamanioMayor = mbr.mbr_tamanio - corrida;
                                posParticion = i;
                                //EN TEORIA LA CORRIDA SERIA EL PUNTO DE ORIGEN
                            }else if(tamanioMayor == 0)
                            {
                                tamanioMayor = mbr.mbr_tamanio - corrida;
                                posParticion = i;
                            }
                        }

                    }else
                    {
                        //corrida+= mbr.mbr_particion[i].part_size;
                        corrida = mbr.mbr_particion[i].part_start + mbr.mbr_particion[i].part_size;
                        if(i == 3 && tamanioMayor != 0){
                            cabeParticion = true;
                            noEspacio = true;
                        }
                    }
                }

                    if(sizeParticion <= resto )//VERIFICO SI AUN HAY ESPACIO
                    {
                        if(!Nombre_noRepetido(mbr, disco) )
                        {
                            mbr.mbr_particion[posParticion] = Llenar_Particion(corrida);
                            rewind(disco);

                            fwrite(&mbr,sizeof(mbr),1,disco);

                            //fseek(disco,mbr.mbr_particion[posParticion].part_start,SEEK_SET);
                            //fwrite(&mbr.mbr_particion[posParticion] , sizeof(Particion), 1 ,disco);

                            //printf("\t=== Se Creo la particion \"%s\" Exitosamente!! ===\n",caracteristicas.name);
                            memset(mensaje,'\0',sizeof(mensaje));
                            sprintf(mensaje,"\t=== Se Creo la particion \"%s\" Exitosamente!! ===\n",caracteristicas.name);
                            if(raid)
                                Mensaje_Aceptacion(mensaje);

                            noEspacio = true;
                            break;
                        }else{

                            noEspacio = true;
                            //printf("\t*** ERROR!! No se creo la particion \"%s\" no se puede repetir nombres en un mismo disco!! ***\n",caracteristicas.name);
                            memset(mensaje,'\0',sizeof(mensaje));
                            sprintf(mensaje,"\t*** ERROR!! No se creo la particion \"%s\" no se puede repetir nombres en un mismo disco!! ***\n",caracteristicas.name);
                            if(raid)
                                Mensaje_Negacion(mensaje);
                        }
                    }else
                    {
                        noEspacio = true;
                        //printf("\t*** ERROR!! No se creo la particion \"%s\" no alcanza el espacio!! ***\n",caracteristicas.name);
                        memset(mensaje,'\0',sizeof(mensaje));
                        sprintf(mensaje,"\t*** ERROR!! No se creo la particion \"%s\" no alcanza el espacio!! ***\n",caracteristicas.name);
                        if(raid)
                            Mensaje_Advertencia(mensaje);
                    }
            }

            if(!noEspacio)//MENSAJE DE ERROR DE QUE NO HAY ESPACIO SUFICIENTE
            {
                //printf("\t*** ERROR!! No hay espacio suficiente en el disco para crear la particion ***\n");
                if(raid)
                Mensaje_Negacion("\t*** ERROR!! No hay espacio suficiente en el disco para crear la particion ***\n");
            }


            if(cabeParticion)
            {
                //printf("\t*** ERROR!! Ya no se puede crear mas particiones en este disco ***\n");
                if(raid)
                Mensaje_Negacion("\t*** ERROR!! Ya no se puede crear mas particiones en este disco ***\n");
            }

            break;

        //========= PARTICION EXTENDIA ========
        case 'e':

            if(mbr.disk_fit == 'f')    //EN LA PRIMERA QUE QUEPA
            {
                //BUSCO QUE POSICION ESTA DISPONIBLE
                for(int i=0; i<4 ;i++)
                {
                    if(mbr.mbr_particion[i].part_status == '0')//SIGNIFICA QUE ESTA DISPONIBLE ESA PARTICION
                    {
                        bool isParticion = false;
                        int nextParticion=0;        //INICIO DE LA SIGUIENTE PARTICION

                        for(int j=i+1; j<4 ;j++)
                        {
                            if(mbr.mbr_particion[j].part_status != '0')//BUSCA SI HAY OTRA PARTICION MAS ADELANTE
                            {
                                isParticion = true;
                                nextParticion = mbr.mbr_particion[j].part_start;
                                break;
                            }
                        }

                        if(isParticion)//HAY PARTICION ADELANTE
                        {
                            int resto = nextParticion - corrida;

                            if(sizeParticion <= resto )//VERIFICO SI AUN HAY ESPACIO
                            {
                                if(!Nombre_noRepetido(mbr, disco))
                                {
                                    if(!Existe_Extendida(mbr))//FALSE PARA CREARLA Y TRUE DE MENSAJE ERROR
                                    {
                                        mbr.mbr_particion[i] = Llenar_Particion(corrida);
                                        rewind(disco);

                                        fwrite(&mbr,sizeof(mbr),1,disco);
                                        fseek(disco,mbr.mbr_particion[i].part_start,SEEK_SET);

                                        EBR ebr = LLenar_EBR(corrida);
                                        fwrite(&ebr, sizeof(ebr), 1, disco);
                                        //fwrite(&mbr.mbr_particion[i] , sizeof(Particion), 1 ,disco);

                                        //printf("\t=== Se Creo la particion Extendida \"%s\" Exitosamente!! ===\n",caracteristicas.name);
                                        memset(mensaje,'\0',sizeof(mensaje));
                                        sprintf(mensaje,"\t=== Se Creo la particion Extendida \"%s\" Exitosamente!! ===\n",caracteristicas.name);
                                        if(raid)
                                            Mensaje_Aceptacion(mensaje);

                                        noEspacio = true;
                                    }else
                                    {
                                        //printf("\t*** ERROR!! No se pudo crear la particion Extentida \"%s\" porque ya existe una extendida!! ***\n",caracteristicas.name);

                                        noEspacio = true;
                                    }
                                    break;
                                }else
                                {
                                    noEspacio = true;
                                    //printf("\t*** ERROR!! No se creo la particion Extendida \"%s\" no se puede repetir nombres en un mismo disco!! ***\n",caracteristicas.name);
                                    memset(mensaje,'\0',sizeof(mensaje));
                                    sprintf(mensaje,"\t*** ERROR!! No se creo la particion Extendida \"%s\" no se puede repetir nombres en un mismo disco!! ***\n",caracteristicas.name);
                                    if(raid)
                                        Mensaje_Negacion(mensaje);
                                }
                            }else
                            {
                                if(i == 3){
                                    noEspacio = true;
                                    //printf("\t*** ERROR!! No se creo la particion Extendida \"%s\" no alcanza el espacio!! ***\n",caracteristicas.name);
                                    memset(mensaje,'\0',sizeof(mensaje));
                                    sprintf(mensaje,"\t*** ERROR!! No se creo la particion Extendida \"%s\" no alcanza el espacio!! ***\n",caracteristicas.name);
                                    if(raid)
                                        Mensaje_Negacion(mensaje);
                                }
                            }

                        }else //NO HAY PARTICION ADELANTE
                        {
                            int resto = mbr.mbr_tamanio - corrida;

                            if(sizeParticion <= resto )//VERIFICO SI AUN HAY ESPACIO
                            {
                                if(!Nombre_noRepetido(mbr, disco) )
                                {
                                    if(!Existe_Extendida(mbr))//FALSE PARA CREARLA Y TRUE DE MENSAJE ERROR
                                    {
                                        mbr.mbr_particion[i] = Llenar_Particion(corrida);
                                        rewind(disco);

                                        fwrite(&mbr,sizeof(mbr),1,disco);
                                        fseek(disco,mbr.mbr_particion[i].part_start,SEEK_SET);

                                        EBR ebr = LLenar_EBR(corrida);
                                        fwrite(&ebr, sizeof(ebr), 1, disco);
                                        //fwrite(&mbr.mbr_particion[i] , sizeof(Particion), 1 ,disco);

                                        //printf("\t=== Se Creo la particion Extendida \"%s\" Exitosamente!! ===\n",caracteristicas.name);
                                        memset(mensaje,'\0',sizeof(mensaje));
                                        sprintf(mensaje,"\t=== Se Creo la particion Extendida \"%s\" Exitosamente!! ===\n",caracteristicas.name);
                                        if(raid)
                                        Mensaje_Aceptacion(mensaje);

                                        noEspacio = true;
                                        //break;
                                    }else
                                    {
                                        //printf("\t*** ERROR!! No se pudo crear la particion Extentida \"%s\" porque ya existe una extendida!! ***\n",caracteristicas.name);
                                        memset(mensaje,'\0',sizeof(mensaje));
                                        sprintf(mensaje,"\t*** ERROR!! No se pudo crear la particion Extentida \"%s\" porque ya existe una extendida!! ***\n",caracteristicas.name);
                                        if(raid)
                                        Mensaje_Negacion(mensaje);
                                        noEspacio = true;
                                    }
                                    break;
                                }else
                                {
                                    noEspacio = true;
                                    //printf("\t*** ERROR!! No se creo la particion Extendida \"%s\" no se puede repetir nombres en un mismo disco!! ***\n",caracteristicas.name);
                                    memset(mensaje,'\0',sizeof(mensaje));
                                    sprintf(mensaje,"\t*** ERROR!! No se creo la particion Extendida \"%s\" no se puede repetir nombres en un mismo disco!! ***\n",caracteristicas.name);
                                    if(raid)
                                    Mensaje_Negacion(mensaje);
                                }
                                break;
                            }else
                            {
                                noEspacio = true;
                                //printf("\t*** ERROR!! No se creo la particion Extendida \"%s\" no alcanza el espacio!! ***\n",caracteristicas.name);
                                memset(mensaje,'\0',sizeof(mensaje));
                                sprintf(mensaje,"\t*** ERROR!! No se creo la particion Extendida \"%s\" no alcanza el espacio!! ***\n",caracteristicas.name);
                                if(raid)
                                Mensaje_Advertencia(mensaje);
                            }

                        }

                    }else
                    {
                        //corrida+= mbr.mbr_particion[i].part_size;
                        corrida = mbr.mbr_particion[i].part_start + mbr.mbr_particion[i].part_size;
                        if(i == 3){
                            cabeParticion = true;
                            noEspacio = true;
                        }
                    }
                }

            }else if(mbr.disk_fit == 'w')  //EN EL ESPACIO MAS GRANDE QUE QUEPA
            {
                int nextParticion=0;        //INICIO DE LA SIGUIENTE PARTICION
                int tamanioMayor = 0;
                //int startMayor = 0;
                int posParticion = 0;
                int resto = 0;

                bool bandera = false; //ME INDICA SI NO HAY PARTICIONES

                for(int i=0; i<4 ;i++) //BUSCO QUE POSICION ESTA DISPONIBLE
                {

                    if(mbr.mbr_particion[i].part_status == '0')//SIGNIFICA QUE ESTA DISPONIBLE ESA PARTICION
                    {

                        for(int j=i+1; j<4 ;j++)//RECORRO EL RESTO DE PARTICIONES
                        {
                            if(mbr.mbr_particion[j].part_status != '0')//BUSCA SI HAY OTRA PARTICION MAS ADELANTE
                            {
                                bandera = true;
                                nextParticion = mbr.mbr_particion[j].part_start;


                                resto = nextParticion - corrida;
                                if( resto > tamanioMayor)//CALCULO SI EL NUEVO ESPACIO ES MAS GRANDE DEL YA GUARDADO
                                {
                                    tamanioMayor = nextParticion - corrida;
                                    posParticion = i;
                                    //EN TEORIA LA CORRIDA SERIA EL PUNTO DE ORIGEN
                                }
                                //break;
                            }
                        }

                        if(!bandera)//EN TEORIA SOLO ENTRADA SI TODO LO DEMAS ESTA VACIO
                        {
                            posParticion = i;

                            //if(i == 0)//ME INDICA QUE ES EL INICIO
                            //    resto = mbr.mbr_tamanio - corrida;
                            resto = mbr.mbr_tamanio - corrida;

                            break;
                        }

                        if(i == 3)//ESTA CONDICION SE CUMPLIRA SOLO CON LA ULTIMA PARTICION VACIA
                        {
                            resto = mbr.mbr_tamanio - corrida;
                            if( resto > tamanioMayor)//CALCULO SI EL NUEVO ESPACIO ES MAS GRANDE DEL YA GUARDADO
                            {
                                tamanioMayor = mbr.mbr_tamanio - corrida;
                                posParticion = i;
                                //EN TEORIA LA CORRIDA SERIA EL PUNTO DE ORIGEN
                            }
                        }

                    }else
                    {
                        //corrida+= mbr.mbr_particion[i].part_size;
                        corrida = mbr.mbr_particion[i].part_start + mbr.mbr_particion[i].part_size;
                        if(i == 3 && tamanioMayor != 0){
                            cabeParticion = true;
                            noEspacio = true;
                        }
                    }
                }

                    if(sizeParticion <= resto )//VERIFICO SI AUN HAY ESPACIO
                    {
                        if(!Nombre_noRepetido(mbr, disco) )
                        {
                            if(!Existe_Extendida(mbr))//FALSE PARA CREARLA Y TRUE DE MENSAJE ERROR
                            {
                                mbr.mbr_particion[posParticion] = Llenar_Particion(corrida);
                                rewind(disco);

                                fwrite(&mbr,sizeof(mbr),1,disco);

                                fseek(disco,mbr.mbr_particion[posParticion].part_start,SEEK_SET);

                                EBR ebr = LLenar_EBR(corrida);
                                fwrite(&ebr, sizeof(ebr), 1, disco);
                                //fwrite(&mbr.mbr_particion[posParticion] , sizeof(Particion), 1 ,disco);

                                //printf("\t=== Se Creo la particion \"%s\" Exitosamente!! ===\n",caracteristicas.name);
                                memset(mensaje,'\0',sizeof(mensaje));
                                sprintf(mensaje,"\t=== Se Creo la particion Extendida \"%s\" Exitosamente!! ===\n",caracteristicas.name);
                                if(raid)
                                Mensaje_Aceptacion(mensaje);

                                noEspacio = true;

                            }else
                            {
                                //printf("\t*** ERROR!! No se pudo crear la particion \"%s\" porque ya existe una extendida!! ***\n",caracteristicas.name);
                                memset(mensaje,'\0',sizeof(mensaje));
                                sprintf(mensaje,"\t*** ERROR!! No se pudo crear la particion Extentida \"%s\" porque ya existe una extendida!! ***\n",caracteristicas.name);
                                if(raid)
                                Mensaje_Negacion(mensaje);
                                noEspacio = true;
                            }
                            break;
                        }else{

                            noEspacio = true;
                            //printf("\t*** ERROR!! No se creo la particion \"%s\" no se puede repetir nombres en un mismo disco!! ***\n",caracteristicas.name);
                            memset(mensaje,'\0',sizeof(mensaje));
                            sprintf(mensaje,"\t*** ERROR!! No se creo la particion Extendida \"%s\" no se puede repetir nombres en un mismo disco!! ***\n",caracteristicas.name);
                            if(raid)
                            Mensaje_Negacion(mensaje);
                        }
                    }else
                    {
                        noEspacio = true;
                        //printf("\t*** ERROR!! No se creo la particion \"%s\" no alcanza el espacio!! ***\n",caracteristicas.name);
                        memset(mensaje,'\0',sizeof(mensaje));
                        sprintf(mensaje,"\t*** ERROR!! No se creo la particion Extendida \"%s\" no alcanza el espacio!! ***\n",caracteristicas.name);
                        if(raid)
                        Mensaje_Advertencia(mensaje);
                    }
            }
            else if(mbr.disk_fit == 'b')  //EN EL ESPACIO MAS PEQUEÑO QUE QUEPA
            {
                int nextParticion=0;        //INICIO DE LA SIGUIENTE PARTICION
                int tamanioMayor = 0;
                int posParticion = 0;
                int resto = 0;

                bool bandera = false; //ME INDICA SI NO HAY PARTICIONES

                for(int i=0; i<4 ;i++) //BUSCO QUE POSICION ESTA DISPONIBLE
                {

                    if(mbr.mbr_particion[i].part_status == '0')//SIGNIFICA QUE ESTA DISPONIBLE ESA PARTICION
                    {

                        for(int j=i+1; j<4 ;j++)//RECORRO EL RESTO DE PARTICIONES
                        {
                            if(mbr.mbr_particion[j].part_status != '0')//BUSCA SI HAY OTRA PARTICION MAS ADELANTE
                            {
                                bandera = true;
                                nextParticion = mbr.mbr_particion[j].part_start;


                                resto = nextParticion - corrida;
                                if( resto < tamanioMayor && sizeParticion <= resto)//CALCULO SI EL NUEVO ESPACIO ES MAS GRANDE DEL YA GUARDADO
                                {
                                    tamanioMayor = nextParticion - corrida;
                                    posParticion = i;
                                    //EN TEORIA LA CORRIDA SERIA EL PUNTO DE ORIGEN
                                }else if(tamanioMayor == 0)
                                {
                                    tamanioMayor = nextParticion - corrida;
                                    posParticion = i;
                                }
                                //break;
                            }
                        }

                        if(!bandera)//EN TEORIA SOLO ENTRARA SI TODO LO DEMAS ESTA VACIO
                        {
                            posParticion = i;
                            resto = mbr.mbr_tamanio - corrida;
                            break;
                        }

                        if(i == 3)//ESTA CONDICION SE CUMPLIRA SOLO CON LA ULTIMA PARTICION VACIA
                        {
                            resto = mbr.mbr_tamanio - corrida;
                            if( resto < tamanioMayor && sizeParticion <= resto)//CALCULO SI EL NUEVO ESPACIO ES MAS GRANDE DEL YA GUARDADO
                            {
                                tamanioMayor = mbr.mbr_tamanio - corrida;
                                posParticion = i;
                                //EN TEORIA LA CORRIDA SERIA EL PUNTO DE ORIGEN
                            }else if(tamanioMayor == 0)
                            {
                                tamanioMayor = mbr.mbr_tamanio - corrida;
                                posParticion = i;
                            }
                        }

                    }else
                    {
                        //corrida+= mbr.mbr_particion[i].part_size;
                        corrida = mbr.mbr_particion[i].part_start + mbr.mbr_particion[i].part_size;
                        if(i == 3 && tamanioMayor != 0){
                            cabeParticion = true;
                            noEspacio = true;
                        }
                    }
                }

                    if(sizeParticion <= resto )//VERIFICO SI AUN HAY ESPACIO
                    {
                        if(!Nombre_noRepetido(mbr, disco) )
                        {
                            if(!Existe_Extendida(mbr))//FALSE PARA CREARLA Y TRUE DE MENSAJE ERROR
                            {
                                mbr.mbr_particion[posParticion] = Llenar_Particion(corrida);
                                rewind(disco);

                                fwrite(&mbr,sizeof(mbr),1,disco);

                                fseek(disco,mbr.mbr_particion[posParticion].part_start,SEEK_SET);


                                EBR ebr = LLenar_EBR(corrida);
                                fwrite(&ebr, sizeof(ebr), 1, disco);
                                //fwrite(&mbr.mbr_particion[posParticion] , sizeof(Particion), 1 ,disco);

                                //printf("\t=== Se Creo la particion \"%s\" Exitosamente!! ===\n",caracteristicas.name);
                                memset(mensaje,'\0',sizeof(mensaje));
                                sprintf(mensaje,"\t=== Se Creo la particion Extendida \"%s\" Exitosamente!! ===\n",caracteristicas.name);
                                if(raid)
                                Mensaje_Aceptacion(mensaje);

                                noEspacio = true;
                            }else
                            {
                                //printf("\t*** ERROR!! No se pudo crear la particion \"%s\" porque ya existe una extendida!! ***\n",caracteristicas.name);
                                memset(mensaje,'\0',sizeof(mensaje));
                                sprintf(mensaje,"\t*** ERROR!! No se pudo crear la particion Extentida \"%s\" porque ya existe una extendida!! ***\n",caracteristicas.name);
                                if(raid)
                                Mensaje_Negacion(mensaje);
                                noEspacio = true;
                            }

                            break;
                        }else{

                            noEspacio = true;
                            //printf("\t*** ERROR!! No se creo la particion \"%s\" no se puede repetir nombres en un mismo disco!! ***\n",caracteristicas.name);
                            memset(mensaje,'\0',sizeof(mensaje));
                            sprintf(mensaje,"\t*** ERROR!! No se creo la particion Extendida \"%s\" no se puede repetir nombres en un mismo disco!! ***\n",caracteristicas.name);
                            if(raid)
                            Mensaje_Negacion(mensaje);
                        }
                    }else
                    {
                        noEspacio = true;
                        //printf("\t*** ERROR!! No se creo la particion \"%s\" no alcanza el espacio!! ***\n",caracteristicas.name);
                        memset(mensaje,'\0',sizeof(mensaje));
                        sprintf(mensaje,"\t*** ERROR!! No se creo la particion Extendida \"%s\" no alcanza el espacio!! ***\n",caracteristicas.name);
                        if(raid)
                        Mensaje_Advertencia(mensaje);
                    }
            }

            if(!noEspacio)//MENSAJE DE ERROR DE QUE NO HAY ESPACIO SUFICIENTE
            {
                //printf("\t*** ERROR!! No hay espacio suficiente en el disco para crear la particion ***\n");
                if(raid)
                Mensaje_Negacion("\t*** ERROR!! No hay espacio suficiente en el disco para crear la particion ***\n");
            }

            if(cabeParticion)
            {
                //printf("\t*** ERROR!! Ya no se puede crear mas particiones en este disco ***\n");
                if(raid)
                Mensaje_Negacion("\t*** ERROR!! Ya no se puede crear mas particiones en este disco ***\n");
            }

            break;

        //========= PARTICION LOGICA ========
        case 'l':

            for(int i=0; i<4 ;i++)//COMPRUEBO SI HAY EXTENDIDA
            {
                if(mbr.mbr_particion[i].part_type == 'e' && mbr.mbr_particion[i].part_status != '0' ){
                    auxLogica = true; posLogica = i;
                    break;}
            }


            if(!Nombre_noRepetido(mbr, disco))
            {
                if(auxLogica)//ANALIZO PRIMERO SI EXITE UN EXTENDIDA PARA LA LOGICA
                {
                    //rewind(disco);
                    fseek(disco, mbr.mbr_particion[posLogica].part_start, SEEK_SET);
                    EBR ebr;
                    fread(&ebr,sizeof(EBR),1,disco);//CONSIGO EL EBR INICIAL

                    Particion extendida = mbr.mbr_particion[posLogica];
                    int pesoEBR = sizeof(EBR);
                    corrida = pesoEBR + ebr.part_start;

                    //if(extendida.part_fit != 'p' )    //EN LA PRIMERA QUE QUEPA
                    if(true)
                    {
                        EBR auxEBR = ebr; int finP = extendida.part_size + extendida.part_start;
                        while(corrida < finP)//PARA VER SI AUN HAY ESPACIO EN EL DISCO
                        {
                            if(auxEBR.part_next == 0)//SI ES CERO INDICA QUE NO HAY UN SIGUIENTE
                            {
                                int resto = finP - corrida; //ESPACIO QUE QUEDA

                                if(sizeParticion <= resto)
                                {
                                    auxEBR.part_fit = caracteristicas.fit_f;
                                    auxEBR.part_size = sizeParticion;
                                    auxEBR.part_next = corrida + sizeParticion +1;
                                    memset( auxEBR.part_name, '\0', sizeof(auxEBR.part_name));
                                    strcpy( auxEBR.part_name , caracteristicas.name );

                                    fseek(disco, auxEBR.part_start, SEEK_SET);
                                    fwrite(&auxEBR, sizeof(EBR), 1, disco );//AQUI ACTUALIZO EL EBR YA ESTADO


                                    EBR nuevo = LLenar_EBR( corrida + sizeParticion );


                                    fseek(disco, nuevo.part_start, SEEK_SET);
                                    fwrite(&nuevo, sizeof(EBR), 1, disco );//AQUI ACTUALIZO EL EBR YA ESTADO

                                    //printf("\t=== Se Creo la particion logica \"%s\" Exitosamente!! ===\n",caracteristicas.name);
                                    memset(mensaje,'\0',sizeof(mensaje));
                                    sprintf(mensaje,"\t=== Se Creo la particion logica \"%s\" Exitosamente!! ===\n",caracteristicas.name);
                                    if(raid)
                                    Mensaje_Aceptacion(mensaje);

                                }else
                                {
                                    //printf("\t*** ERROR!! No se creo la particion Logica \"%s\" no alcanza el espacio!! ***\n",caracteristicas.name);
                                    memset(mensaje,'\0',sizeof(mensaje));
                                    sprintf(mensaje,"\t*** ERROR!! No se creo la particion Logica \"%s\" no alcanza el espacio!! ***\n",caracteristicas.name);
                                    if(raid)
                                    Mensaje_Advertencia(mensaje);
                                }

                                break;
                            }else{//INDICA QUE HAY UN SIGUIENTE, POR LO TANTO HAY QUE SEGUIR RECORRIENDO
                                int as = auxEBR.part_next;
                                corrida = pesoEBR + as;
                                fseek(disco, as, SEEK_SET);
                                fread(&auxEBR,sizeof(EBR),1,disco);//CONSIGO EL EBR SIGUIENTE

                            }
                        }
                    }


                }else
                {
                    //printf("\t*** ERROR!! No se pudo crear la particion Logica \"%s\" porque no existe una extendida!! ***\n",caracteristicas.name);
                    memset(mensaje,'\0',sizeof(mensaje));
                    sprintf(mensaje,"\t*** ERROR!! No se pudo crear la particion Logica \"%s\" porque no existe una extendida!! ***\n",caracteristicas.name);
                    if(raid)
                    Mensaje_Negacion(mensaje);
                    noEspacio = true;
                }
            }else{

                noEspacio = true;
                //printf("\t*** ERROR!! No se creo la particion \"%s\" no se puede repetir nombres en un mismo disco!! ***\n",caracteristicas.name);
                memset(mensaje,'\0',sizeof(mensaje));
                sprintf(mensaje,"\t*** ERROR!! No se creo la particion \"%s\" no se puede repetir nombres en un mismo disco!! ***\n",caracteristicas.name);
                if(raid)
                Mensaje_Negacion(mensaje);
            }






            break;
    }





    fclose(disco);


    if(raid)//SI ES TRUE MODIFICO SU RAID
    {
        Ruta_Raid(caracteristicas.path);
        Crear_Particion(false);
    }
}


void Particion_Delete(bool raid)
{
    MBR mbr;

        FILE* disco;
        disco = fopen(caracteristicas.path,"r+b");

        if(disco == NULL){
            //printf("\t*** ERROR!! A la hora de abrir el disco, Verifique la ruta o si existe el disco ***\n");
            if(raid)
            Mensaje_Negacion("\t*** ERROR!! A la hora de abrir el disco, Verifique la ruta o si existe el disco ***\n");
            return;
        }

        fread(&mbr,sizeof(MBR),1,disco);

        bool encontrado = false;
        //int startParticion = 0;
        int posicion = 0;

        for(int i=0; i<4 ;i++)
        {
            if( strcasecmp( mbr.mbr_particion[i].part_name , caracteristicas.name ) == 0 && mbr.mbr_particion[i].part_status != '0')
            {
                encontrado = true;
                //startParticion = mbr.mbr_particion[i].part_start;
                posicion = i;
                break;
            }
        }

        bool bandera = true;
        do{
            //printf("Esta seguro de querer eliminar esta particion Y/N\n");
            if(raid)
            Mensaje_Green("Esta seguro de querer eliminar esta particion Y/N\n");

            char respuesta[1000];
            if(raid)
            {

                fgets(respuesta,1000,stdin);
                Eliminar_Salto_Linea(respuesta);
            }else
            {
                respuesta[0] = 'y';
            }

            if(strcasecmp(respuesta,"y") == 0 )
            {

                if(caracteristicas.deleteP == 'a')//========== ELEMINACION FAST
                {
                    if(encontrado)//ENCONTRO LA PARTICION CON EL MISMO NOMBRE
                    {
                        if(mbr.mbr_particion[posicion].part_type == 'p'){
                            mbr.mbr_particion[posicion].part_status = '0';

                            //fseek(disco, mbr.mbr_particion[posicion].part_start, SEEK_SET);
                            //rewind(disco);
                            //fwrite(&mbr.mbr_particion[posicion],sizeof(Particion),1,disco);

                            rewind(disco);
                            fwrite(&mbr,sizeof(MBR),1,disco);

                            //printf("\t*** Se borro la particion con el nombre \"%s\" en este disco ***\n",caracteristicas.name);
                            memset(mensaje,'\0',sizeof(mensaje));
                            sprintf(mensaje,"\t*** Se borro la particion con el nombre \"%s\" en este disco ***\n",caracteristicas.name);
                            if(raid)
                            Mensaje_Aceptacion(mensaje);

                            bandera = false;
                        }else if(mbr.mbr_particion[posicion].part_type == 'e')
                        {
                            mbr.mbr_particion[posicion].part_status = '0';

                            //fseek(disco, mbr.mbr_particion[posicion].part_start, SEEK_SET);
                            //rewind(disco);
                            //fwrite(&mbr.mbr_particion[posicion],sizeof(Particion),1,disco);

                            rewind(disco);
                            fwrite(&mbr,sizeof(MBR),1,disco);
                            //printf("\t*** Se borro la particion con el nombre \"%s\" en este disco ***\n",caracteristicas.name);
                            memset(mensaje,'\0',sizeof(mensaje));
                            sprintf(mensaje,"\t*** Se borro la particion con el nombre \"%s\" en este disco ***\n",caracteristicas.name);
                            if(raid)
                            Mensaje_Aceptacion(mensaje);

                            bandera = false;
                        }
                    }else
                    {
                        //printf("\t*** ERROR!! No se encontro ninguna particion con el nombre \"%s\" en este disco ***\n",caracteristicas.name);
                        memset(mensaje,'\0',sizeof(mensaje));
                        sprintf(mensaje,"\t*** ERROR!! No se encontro ninguna particion con el nombre \"%s\" en este disco ***\n",caracteristicas.name);
                        if(raid)
                        Mensaje_Negacion(mensaje);
                        bandera = false;
                    }

                    }else if(caracteristicas.deleteP == 'u')//ELIMINACION FULL
                    {

                            if(encontrado)//ENCONTRO LA PARTICION CON EL MISMO NOMBRE
                            {
                                if(mbr.mbr_particion[posicion].part_type == 'p')
                                {
                                    //mbr.mbr_particion[posicion].part_status = '0';

                                    fseek(disco, mbr.mbr_particion[posicion].part_start, SEEK_SET);

                                    //fwrite(&mbr.mbr_particion[posicion],sizeof(Particion),1,disco);

                                    char vaciar[mbr.mbr_particion[posicion].part_size];

                                    //memset(vaciar, '\0' ,mbr.mbr_particion[posicion].part_size);
                                    fwrite(vaciar,sizeof(vaciar),mbr.mbr_particion[posicion].part_size,disco);

                                    rewind(disco);

                                    mbr.mbr_particion[posicion] = Vaciar_Particion();

                                    fwrite(&mbr,sizeof(MBR),1,disco);
                                    //printf("\t*** Se borro la particion con el nombre \"%s\" en este disco ***\n",caracteristicas.name);
                                    memset(mensaje,'\0',sizeof(mensaje));
                                    sprintf(mensaje,"\t*** Se borro la particion con el nombre \"%s\" en este disco ***\n",caracteristicas.name);
                                    if(raid)
                                    Mensaje_Aceptacion(mensaje);

                                }else if(mbr.mbr_particion[posicion].part_type == 'e')
                                {
                                    //mbr.mbr_particion[posicion].part_status = '0';

                                    fseek(disco, mbr.mbr_particion[posicion].part_start, SEEK_SET);

                                    //fwrite(&mbr.mbr_particion[posicion],sizeof(Particion),1,disco);

                                    char vaciar[mbr.mbr_particion[posicion].part_size];

                                    //memset(vaciar, '\0' ,mbr.mbr_particion[posicion].part_size);
                                    fwrite(vaciar,sizeof(vaciar),mbr.mbr_particion[posicion].part_size,disco);

                                    rewind(disco);

                                    mbr.mbr_particion[posicion] = Vaciar_Particion();

                                    fwrite(&mbr,sizeof(MBR),1,disco);
                                    //printf("\t*** Se borro la particion con el nombre \"%s\" en este disco ***\n",caracteristicas.name);
                                    memset(mensaje,'\0',sizeof(mensaje));
                                    sprintf(mensaje,"\t*** Se borro la particion con el nombre \"%s\" en este disco ***\n",caracteristicas.name);
                                    if(raid)
                                    Mensaje_Aceptacion(mensaje);

                                }
                                bandera = false;
                            }else
                            {
                                //printf("\t*** ERROR!! No se encontro ninguna particion con el nombre \"%s\" en este disco ***\n",caracteristicas.name);
                                memset(mensaje,'\0',sizeof(mensaje));
                                sprintf(mensaje,"\t*** ERROR!! No se encontro ninguna particion con el nombre \"%s\" en este disco ***\n",caracteristicas.name);
                                if(raid)
                                Mensaje_Negacion(mensaje);
                                bandera = false;
                            }
                        }

            }else if(strcasecmp(respuesta,"n") == 0)
            {
                //printf("El Disco no se elimino!!\n");
                if(raid)
                Mensaje_Aceptacion("El Disco no se elimino!!\n");
                bandera = false;
            }else{
                //printf("*** Ingrese una opcion correcta!!! ***\n");
                if(raid)
                Mensaje_Advertencia("*** Ingrese una opcion correcta!!! ***\n");
            }

        }
        while(bandera);


        fclose(disco);

        if(raid)//SI ES TRUE LO VUELVO A EJECUTAR
        {
            Ruta_Raid(caracteristicas.path);
            Particion_Delete(false);
        }

}

void Particion_Add(bool raid)
{
    MBR mbr;

    FILE* disco;
    disco = fopen(caracteristicas.path,"r+b");

    if(disco == NULL){
        //printf("\t*** ERROR!! A la hora de abrir el disco, Verifique la ruta o si existe el disco ***\n");
        if(raid)
        Mensaje_Negacion("\t*** ERROR!! A la hora de abrir el disco, Verifique la ruta o si existe el disco ***\n");
        return;
    }
    fread(&mbr,sizeof(MBR),1,disco);

    int espacio = Calcular_Espacio_Add();
    int posicion = 0;
    bool encontrado = false;
    Particion part;

    for(int i=0; i<4 ;i++)
    {
        if( strcasecmp( mbr.mbr_particion[i].part_name , caracteristicas.name ) == 0 && mbr.mbr_particion[i].part_status != '0')
        {
            encontrado = true;
            //startParticion = mbr.mbr_particion[i].part_start;
            posicion = i;

            part = mbr.mbr_particion[i];
            break;
        }
    }

    if(encontrado)//ENCONTRO LA PARTICION CON EL MISMO NOMBRE
    {
        if(caracteristicas.negado)//SI ES TRUE VAMOS A QUITAR ESPACIO
        {
            if(espacio >= part.part_size)//SI EL ESPACIO A QUITAR ES MAYOR AL DISPONIBLE MENSAJE ERROR
            {
                //printf(RED "\t*** ERROR!! El espacio deseado a quitar es mas de lo hay disponible en la particion \"%s\" en este disco ***\n" RESET,caracteristicas.name);
                memset(mensaje,'\0',sizeof(mensaje));
                sprintf(mensaje,"\t*** ERROR!! El espacio deseado a quitar es mas de lo hay disponible en la particion \"%s\" en este disco ***\n",caracteristicas.name);
                if(raid)
                Mensaje_Negacion(mensaje);
                return;
            }else//SI NO PUES SE PUEDE ELIMINAR EL ESPACIO
            {
                part.part_size = part.part_size - espacio;

                mbr.mbr_particion[posicion] = part;

                rewind(disco);//REINICIO EL APUNTADOR AL INCIO DEL DISCO
                fwrite(&mbr,sizeof(mbr),1, disco);//ACTUALIZO EL MBR
            }



        }else//SI ES FALSE VAMOS AGREGAR ESPACIO
        {
            if(posicion == 3)//SI ES TRUE ES PORQUE ES EL ULTIMO PEDAZO
            {
                int sobrante = mbr.mbr_tamanio - (part.part_start + part.part_size);
                if(sobrante >= espacio)
                {
                    part.part_size = part.part_size + espacio;

                    mbr.mbr_particion[posicion] = part;

                    rewind(disco);//REINICIO EL APUNTADOR AL INCIO DEL DISCO
                    fwrite(&mbr,sizeof(mbr),1, disco);//ACTUALIZO EL MBR
                }else
                {
                    //printf(RED "\t*** ERROR!! El espacio deseado a agregar es mas de lo hay disponible en la particion \"%s\" en este disco ***\n" RESET,caracteristicas.name);
                    memset(mensaje,'\0',sizeof(mensaje));
                    sprintf(mensaje,"\t*** ERROR!! El espacio deseado a agregar es mas de lo hay disponible en la particion \"%s\" en este disco ***\n",caracteristicas.name);
                    if(raid)
                    Mensaje_Negacion(mensaje);
                    return;
                }

            }else
            {
                //Particion siguiente = mbr.mbr_particion[posicion + 1];
                //int sobrante = siguiente.part_start - (part.part_start + part.part_size);

                Particion siguiente;
                int sobrante = 0;
                int posSiguientes = posicion+1;

                while(posSiguientes < 5 )
                {
                    if(posSiguientes == 4)//ESTE NOS INDICA QUE NO HAY, ENTONCES HAY QUE HACERLO CON EL TAMANIO DEL DISCO
                    {
                        sobrante = mbr.mbr_tamanio - (part.part_start + part.part_size);
                        break;
                    }
                    else
                    {
                        siguiente = mbr.mbr_particion[posSiguientes++];

                        if(siguiente.part_status != '0')//VERIFICAMOS SI HAY PARTICION
                        {
                           sobrante = siguiente.part_start - (part.part_start + part.part_size);
                           break;
                        }
                    }

                }


                if(sobrante >= espacio)
                {
                    part.part_size = part.part_size + espacio;

                    mbr.mbr_particion[posicion] = part;

                    rewind(disco);//REINICIO EL APUNTADOR AL INCIO DEL DISCO
                    fwrite(&mbr,sizeof(mbr),1, disco);//ACTUALIZO EL MBR
                }else
                {
                    //printf(RED "\t*** ERROR!! El espacio deseado a agregar es mas de lo hay disponible en la particion \"%s\" en este disco ***\n" RESET,caracteristicas.name);
                    memset(mensaje,'\0',sizeof(mensaje));
                    sprintf(mensaje,"\t*** ERROR!! El espacio deseado a agregar es mas de lo hay disponible en la particion \"%s\" en este disco ***\n",caracteristicas.name);
                    if(raid)
                    Mensaje_Negacion(mensaje);
                    return;
                }
            }


        }
        //printf(GREEN "\t=== El espacio de la particion \"%s\" en este disco a sido modificado ===\n" RESET,caracteristicas.name);
        memset(mensaje,'\0',sizeof(mensaje));
        sprintf(mensaje,"\t=== El espacio de la particion \"%s\" en este disco a sido modificado ===\n",caracteristicas.name);
        if(raid)
        Mensaje_Aceptacion(mensaje);

    }else
    {
        //printf(RED "\t*** ERROR!! No se encontro ninguna particion con el nombre \"%s\" en este disco ***\n" RESET,caracteristicas.name);
        memset(mensaje,'\0',sizeof(mensaje));
        sprintf(mensaje,"\t*** ERROR!! No se encontro ninguna particion con el nombre \"%s\" en este disco ***\n",caracteristicas.name);
        if(raid)
        Mensaje_Negacion(mensaje);
    }

    fclose(disco);

    if(raid)//SI ES TRUE MODIFICO SU RAID
    {
        Ruta_Raid(caracteristicas.path);
        Particion_Add(false);
    }

}


Particion Vaciar_Particion()
{
    Particion r;

    r.part_status = '0';
    r.part_type = 'p';
    r.part_fit = 'w';
    r.part_start = 0;


    r.part_size = 0;

    strcat( r.part_name, "nel" );


    return r;
}



int Calcular_Espacio()
{
    int r;
    if(caracteristicas.unit_f == 'b'){
        r = caracteristicas.size;
    }else if(caracteristicas.unit_f == 'k'){
        r = caracteristicas.size * 1024;
    }else if(caracteristicas.unit_f == 'm'){
        r = caracteristicas.size * 1024 * 1024;
    }
    return r;
}

int Calcular_Espacio_Add()
{
    int r;
    if(caracteristicas.unit_f == 'b'){
        r = caracteristicas.add;
    }else if(caracteristicas.unit_f == 'k'){
        r = caracteristicas.add * 1024;
    }else if(caracteristicas.unit_f == 'm'){
        r = caracteristicas.add * 1024 * 1024;
    }
    return r;
}


bool Nombre_noRepetido(MBR mbr, FILE* disco)
{
    //bool r = false;
    for(int i=0; i<4 ;i++)
    {
        Particion part = mbr.mbr_particion[i];

        if(part.part_type == 'e')//EN DADO CASO DE QUE SEA EXTENDIDA
        {
            if( strcasecmp( part.part_name , caracteristicas.name ) == 0 && part.part_status != '0')
                return true;

            fseek(disco,part.part_start,SEEK_SET); //ME POSICIONO EN DONDE COMIENZA LAS LOGICAS

            int finPart = part.part_size + part.part_start;
            EBR ebr;
            int corrida = 0;
            int pesoEBR = sizeof(EBR);

            fread(&ebr,sizeof(EBR),1,disco);//CONSIGO EL EBR INICIAL

            corrida = pesoEBR + ebr.part_start;

            while(corrida < finPart)
            {
                if(strcasecmp( ebr.part_name, caracteristicas.name )==0 && ebr.part_status != 0)
                {
                    return true;
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



        }else//SI NO EN DADO CASO DE QUE SEA PRIMARIA
        {
            if( strcasecmp( part.part_name , caracteristicas.name ) == 0 && part.part_status != '0')
                return true;
        }
    }
    return false;
}

bool Existe_Extendida(MBR mbr)
{
    //bool r = false;
    for(int i=0; i<4 ;i++)
    {
        if(mbr.mbr_particion[i].part_type == 'e' && mbr.mbr_particion[i].part_status != '0' )
            return true;
    }
    return false;
}

EBR LLenar_EBR(int inicio)
{
    EBR r;
    r.part_status = '1';
    r.part_fit = caracteristicas.fit_f;
    r.part_start = inicio+1;
    r.part_size = 0;
    r.part_next = 0; //CERO PARA DECIR QUE NO HAY
    memset(r.part_name,'\0',sizeof(r.part_name));
    strcat( r.part_name , caracteristicas.name);

    return r;
}

































#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <stdbool.h>

#include "Metodos.h"
#include "Estructuras.h"
#include "Lista_Mount.h"
#include "Metodos_Archivo.h"
#include "Metodos_Directorio.h"

#include "Metodos.h"
#include "Mkdisk.h"
#include "Rmdisk.h"
#include "Fdisk.h"
#include "Mount.h"
#include "Unmount.h"
#include "Rep.h"
#include "Exec.h"

#include "Mkfs.h"
#include "Login.h"
#include "Mkgrp.h"
#include "Mkusr.h"
#include "Cat.h"
#include "Rem.h"
#include "Mkfile.h"

#include "Mkdir.h"


// INCLUDES PARA LOS SOCKES
#include <arpa/inet.h>
#include <sys/socket.h>


void Analizar_Comando(char* cadena, int tamanio);
void Contar_Comando(char* cadena,int* tamanio);
void Elegir_Comando(char* cadena, char* comando, int posicion);
void Salto_Linea(char* cadena);

void Imprimir_Encabezado(void);

void Conexion();
void Peticion();
int open_socket();
void bind_to_port(int socket, int port);

bool isSalir = false;
bool isSalto = false;

//int listener = 0;

int main(void)
{
    //strcpy(logeado.usr,"nel-1");
    //    strcpy(logeado.pwd,"nel-1");
    Aux_Llenar_Loguer();
    DesLoguear();
    //Conexion();


    Imprimir_Encabezado();

        do {
            isSalir = false;
            char buffer[5000] ;
            int tamanio = 0;

            //OBTENGO MI LINEA DE COMANDO
            fgets(buffer, 5000 , stdin);
            Salto_Linea(buffer);

            while(isSalto)
            {
                char auxBuffer[1000];
                fgets(auxBuffer,1000,stdin);
                strcat(buffer," ");
                strcat(buffer,auxBuffer);

                Salto_Linea(buffer);
            }

            Contar_Comando(buffer, &tamanio);

            Analizar_Comando(buffer,tamanio);



        }while(!isSalir);
    return 0;
}


void Contar_Comando(char* cadena,int* tamanio)
{
    char auxC[5000];
    strcpy(auxC , cadena);
    int* auxT = tamanio;

    char auxSalida[5000];
    memset(auxSalida, '\0', sizeof(auxSalida));

    for(int i=0; auxC[i] != '\0' ;i++){
        //printf("%c\n",auxC[i]);
        auxSalida[i] = auxC[i];
        (*auxT)++;
    }

}

void Analizar_Comando(char* cadena, int tamanio)
{
    char comando[tamanio];
    char pivote[tamanio];

    memset(comando,'\0',sizeof(comando));
    memset(pivote,'\0',sizeof(pivote));

    int aux=0;
    for(int i =0; i< tamanio ; i++){
        if(i == 0 && cadena[i] == ' '){
            while(cadena[i] == ' '){
                i++; aux++;
            }

            comando[i-aux] = cadena[i];

        }else{
            comando[i-aux] = cadena[i];
        }
    }
    aux = 0;
    for(int i=0; comando[i] != '\0';i++)
    {

        if(comando[i] == ' ' || comando[i] == '\n')//AL ENCONTRARSE CON EL PRIMER ESPACIO EN TEORIA YA TENGO EL PRIMER COMANDO
        {
            while(comando[i] == ' '){
                i++; aux++;
            }


            Elegir_Comando(pivote, comando, i);
            return;

        }else if(comando[i] == '#'){//SE ENCUENTRA CON EL INICIO DE UN COMENTARIO
            if(i == 0)//SE LO ENCUENTRA AL INICION DEL COMANDO
                return;
            Elegir_Comando(pivote, comando, i+1);
            return;
        }else{
            pivote[i] = comando[i];
        }
    }
}

void Elegir_Comando(char* cadena, char* comando, int posicion)
{
    if(strcasecmp(cadena,"mkdisk") == 0){
        Analizar_Mkdisk(comando,posicion);


    }else if(strcasecmp(cadena,"rmdisk") == 0){
        Analizar_Rmdisk(comando,posicion);

    }else if(strcasecmp(cadena,"fdisk") == 0){
        Analizar_Fdisk(comando,posicion);

    }else if(strcasecmp(cadena,"mount") == 0){
        Analizar_Mount(comando,posicion);


    }else if(strcasecmp(cadena,"graficar_mount") == 0){
        Graficar_Mount();

    }else if(strcasecmp(cadena,"unmount") == 0){
        Analizar_Unmount(comando,posicion);

    }else if(strcasecmp(cadena,"pause") == 0){
        //printf("\nPresione enter para poder continuar....\n");
        Mensaje_Advertencia("\nPresione enter para poder continuar....\n");
        char aux[1000];
        fgets(aux,1000,stdin);

    }else if(strcasecmp(cadena,"rep") == 0){
        Analizar_Rep(comando,posicion);

    }else if(strcasecmp(cadena,"exec") == 0){

        Analizar_Exec(comando,posicion);

    }else if(strcasecmp(cadena,"exit") == 0){
        isSalir = true;

    }else if(strcasecmp(cadena,"clear") == 0){
        system("clear");
        Imprimir_Encabezado();
    }
    else if(strcasecmp(cadena,"mkfs") == 0){
        Analizar_Mkfs(comando,posicion);
    }
    else if(strcasecmp(cadena,"login") == 0){
        if( strcasecmp(daLoguer.usr,"nel") == 0 )
            Analizar_Login(comando,posicion);
        else
            Mensaje_Color("*** ERROR!! Ya hay un usuario logeado ***\n",ROJO);
    }
    else if(strcasecmp(cadena,"logout") == 0){

        DesLoguear();
        Mensaje_Color("=== El logout se realizo exitosamente!! ===\n",AMARILLO);
    }
    else if(strcasecmp(cadena,"mkgrp") == 0){
        if( strcasecmp(daLoguer.usr,"root") == 0 )
            Analizar_Mkgrp(comando,posicion);
        else
            Mensaje_Color("*** ERROR!! Solo usuario root puede usar este comando ***\n",ROJO);

    }
    else if(strcasecmp(cadena,"rmgrp") == 0){

    }else if(strcasecmp(cadena,"mkusr") == 0){
        if( strcasecmp(daLoguer.usr,"root") == 0 )
            Analizar_Mkusr(comando,posicion);
        else
            Mensaje_Color("*** ERROR!! Solo usuario root puede usar este comando ***\n",ROJO);
    }
    else if(strcasecmp(cadena,"rmusr") == 0){

    }else if(strcasecmp(cadena,"chmod") == 0){

    }
    else if(strcasecmp(cadena,"mkfile") == 0){
        Analizar_Mkfile(comando,posicion);

    }else if(strcasecmp(cadena,"cat") == 0){

    }else if(strcasecmp(cadena,"rem") == 0){
        Analizar_Rem(comando,posicion);

    }else if(strcasecmp(cadena,"edit") == 0){

    }else if(strcasecmp(cadena,"ren") == 0){

    }else if(strcasecmp(cadena,"mkdir") == 0){
        Analizar_Mkdir(comando,posicion);

    }else if(strcasecmp(cadena,"cp") == 0){

    }else if(strcasecmp(cadena,"mv") == 0){

    }else if(strcasecmp(cadena,"find") == 0){

    }else if(strcasecmp(cadena,"chown") == 0){

    }else if(strcasecmp(cadena,"chgrp") == 0){

    }else if(strcasecmp(cadena,"recovery") == 0){

    }else if(strcasecmp(cadena,"loss") == 0){

    }else if(strcasecmp(cadena,"probando") == 0){



        int parseo = -1;
        if(Validar_Numero(&caracteristicas.id[3])){
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

                // METODO PARA SABER SI EXISTE ARCHIVO
                int a = Existe_Archivo(disco,sblock,datos_particion->size,"/users.txt");

                if(a != -1){ printf("Encontrado -> %d\n",a); }else{ printf("No encontrado -> %d\n",a); }

                // METODO PARA SABER SI EXISTE UN DIRECTORIO
                a = Existe_Directorio(disco,sblock,datos_particion->size,"/home");

                if(a != -1){ printf("Encontrado -> %d\n",a); }else{ printf("No encontrado -> %d\n",a); }

                // METODOS PARA CREAR DIRECTORIO
                Crear_Directorio(disco,sblock,datos_particion->inicio,datos_particion->size,"/home");
                a = Existe_Directorio(disco,sblock,datos_particion->size,"/home");

                if(a != -1){ printf("Encontrado -> %d\n",a); }else{ printf("No encontrado -> %d\n",a); }

                // METODOS PARA CREAR ARCHIVOS
                Crear_Archivo(disco,sblock,datos_particion->inicio,datos_particion->size,"/prueba.txt");
                Crear_Archivo(disco,sblock,datos_particion->inicio,datos_particion->size,"/texto2.txt");
                Crear_Archivo(disco,sblock,datos_particion->inicio,datos_particion->size,"/home/texto3.txt");

                Crear_Archivo(disco,sblock,datos_particion->inicio,datos_particion->size,"/texto4.txt");

                a = Existe_Archivo(disco,sblock,datos_particion->size,"/home/texto3.txt");
                if(a != -1){ printf("Encontrado -> %d\n",a); }else{ printf("No encontrado -> %d\n",a); }

                // METODO PARA ELIMINAR ARCHIVO
                Eliminar_Archivo(disco,sblock,datos_particion->inicio,datos_particion->size,"/home/texto3.txt");

                a = Existe_Archivo(disco,sblock,datos_particion->size,"/home/texto3.txt");
                if(a != -1){ printf("Encontrado -> %d\n",a); }else{ printf("No encontrado -> %d\n",a); }

                a = Existe_Archivo(disco,sblock,datos_particion->size,"/texto4.txt");
                if(a != -1){ printf("Encontrado -> %d\n",a); }else{ printf("No encontrado -> %d\n",a); }

                // METODO PARA ESCRIBIR EN UN ARCHIVO
                //Escribir_Archivo(disco,sblock,datos_particion->inicio,datos_particion->size,"/users.txt","/users.txt");
                Escribir_Archivo(disco,sblock,datos_particion->inicio,datos_particion->size,"/users.txt","2,U,Usuarios  ,diego     ,123456    \n");

                // METODO PARA LEER UN ARCHIVO
                char contenido[10000];
                Leer_Archivo(disco,sblock,datos_particion->size,contenido,"/users.txt");
                printf("%s\n",contenido);

                // METODO PARA EDITAR UN ARCHIVO (LO EDITA DESDE EL INICIO)
                char* Texto_Nuevo = "Ingresando texto nuevo\n salto como no";
                Editar_Archivo(disco,sblock,datos_particion->inicio,datos_particion->size,"/users.txt",Texto_Nuevo);

                memset(contenido,'\0',sizeof(contenido));
                Leer_Archivo(disco,sblock,datos_particion->size,contenido,"/users.txt");
                printf("%s\n",contenido);

                int tam = strlen(contenido);
                Borrar_Contenido_Archivo(disco,sblock,datos_particion->inicio,datos_particion->size,"/users.txt",&tam);

                memset(contenido,'\0',sizeof(contenido));
                Leer_Archivo(disco,sblock,datos_particion->size,contenido,"/users.txt");
                printf("%s\n",contenido);

            }else
            {
                //printf("*** ERROR!! No existe un particion montada con este id ***\n");
                Mensaje_Color("*** ERROR!! No existe un particion montada con este id ***\n",ROJO);
            }
        }



    }else if(strcasecmp(cadena,"") == 0){

    }
    else{
        memset(mensaje,'\0',sizeof(mensaje));
        sprintf(mensaje,"El comando \"%s\" no es reconocible o no existe!!\n",cadena);
        //printf("El comando \"%s\" no es reconocible o no existe!!\n",cadena);
        Mensaje_Color(mensaje,ROJO);
    }
}

void Salto_Linea(char* cadena)
{
    char *p;
    p =  strchr(cadena,  '\\');
    if(p)
    {
        *p = '\0';
        isSalto = true;
    }else{
        isSalto = false;
    }
}

void Imprimir_Encabezado(void)
{
    printf("=================== BIENVENIDO!! ===================\n");
    printf("================ CARNET: 201602723!! ===============\n");
    printf("================ DIEGO CABALLEROS !! ===============\n\n");

}

void Conexion()
{
    const int PORT = 7200;
    char* advice[]={"mensaje","d","tres","cuatro"};
    int listener = open_socket();
    //listener = open_socket();
    if(listener == -1){
        printf("Error con el listener\n");
    }

    bind_to_port(listener, PORT);
    if(listen (listener,10 )== -1){
        printf("No es posible escuchar el puerto\n");
        return;
    }

    if(listen (listener,10) == -1){
        printf("No es posible escuchar el puerto\n");
        return;
    }

    printf("Enlazando al puerto\n");


    while(1){
            struct sockaddr_storage client;
            unsigned int addres_size = sizeof(client);
            printf("Esperando al cliente\n");
            int connect = accept(listener, (struct sockaddr*) &client,&addres_size);
            if(connect == -1){
                printf("No se puede conectar socket secundario\n");
            }
            printf("Atendiendo al cliente\n");
            char buffer[200];

            printf("\nEsperando insruccion...\n");
            recv(connect, buffer, 100, 0); //recepción
            printf("%s", buffer);
            printf("\n");

            char cadena[1000];

            for(int i=0;i<1000;i++){
                cadena[i]=0;
            }


            int iniciaCadena = 0;
            int j=0;
            int i=0;
            int cont=0;
            for(i=0;i<200;i++){
                if(buffer[i]=='$' && j!=0){
                    break;
                }
                if(iniciaCadena == 1){
                    if(buffer[i]=='%')
                        cadena[j] = ' ';
                    else
                        cadena[j] = buffer[i];
                    cont++;
                    j++;
                    continue;
                }
                if(buffer[i]=='$'){
                    iniciaCadena = 1;
                }
            }
          //  printf("----------------\n");
          //  printf("----------------\n");
          //  printf("%s", cadena);
          //  printf("\n----------------\n");
          //  printf("----------------\n");

//            readComands(cadena,cont);

            int tamanio = 0;
            Contar_Comando(cadena, &tamanio);
            cadena[tamanio++] ='\n';
            Mensaje_Green(cadena);
            printf("\n");
            Analizar_Comando(cadena,tamanio);
            memset(buffer,'\0',sizeof(buffer));
            memset(cadena,'\0',sizeof(cadena));

            close(connect);
        }

}

//void Peticion()
//{
//    while(1){
//            struct sockaddr_storage client;
//            unsigned int addres_size = sizeof(client);
//            printf("Esperando al cliente\n");
//            int connect = accept(listener, (struct sockaddr*) &client,&addres_size);
//            if(connect == -1){
//                printf("No se puede conectar socket secundario\n");
//            }
//            printf("Atendiendo al cliente\n");
//            char buffer[200];

//            printf("\nEsperando insruccion...\n");
//            recv(connect, buffer, 100, 0); //recepción
//            printf("%s", buffer);
//            printf("\n");

//            char cadena[1000];

//            for(int i=0;i<1000;i++){
//                cadena[i]=0;
//            }


//            int iniciaCadena = 0;
//            int j=0;
//            int i=0;
//            int cont=0;
//            for(i=0;i<200;i++){
//                if(buffer[i]=='$' && j!=0){
//                    break;
//                }
//                if(iniciaCadena == 1){
//                    if(buffer[i]=='%')
//                        cadena[j] = ' ';
//                    else
//                        cadena[j] = buffer[i];
//                    cont++;
//                    j++;
//                    continue;
//                }
//                if(buffer[i]=='$'){
//                    iniciaCadena = 1;
//                }
//            }
//          //  printf("----------------\n");
//          //  printf("----------------\n");
//          //  printf("%s", cadena);
//          //  printf("\n----------------\n");
//          //  printf("----------------\n");

////            readComands(cadena,cont);



//            int tamanio = 0;
//            //Contar_Comando(cadena, &tamanio);
//            Analizar_Comando(cadena,cont);

//            close(connect);
//        }
//}

int open_socket(){
    int s = socket(PF_INET,SOCK_STREAM,0);
    if(s == -1)
        printf("error al abrir socket\n");
    return s;
}

void bind_to_port(int socket, int port){
    struct sockaddr_in name;
    name.sin_family = PF_INET;
    name.sin_port = (in_port_t)htons(port);
    name.sin_addr.s_addr = htonl(INADDR_ANY);

    int reuse = 1;
    if(setsockopt(socket,SOL_SOCKET,SO_REUSEADDR,(char)&reuse,sizeof(int)) == -1){
        printf("No es posible reusar el socket\n");
    }
    int c = bind(socket,(struct sockaddr*) &name, sizeof(name));
    if(c == -1){
        printf("No se puede enlazar al puerto: la direccion ya esta en uso\n");
    }

}

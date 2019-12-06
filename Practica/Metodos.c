#include "Metodos.h"


bool Validar_Numero(char* num)
{
    for(int i=0; num[i]!='\0' ;i++)
    {
        if(!isdigit(num[i])){
            printf("\t*** ERROR!! Ingrese un numero valido ***\n");
            return false;
        }

    }
    return true;
}

void Crear_Carpetas(char* ruta)
{
    char delim[] = "/";
    char nuevo[1000];
    memset(nuevo,'\0',sizeof(nuevo));
    strcat(nuevo,ruta);

    char *ptr = strtok(nuevo,delim);


    char* carpetas[100];
    memset(carpetas,'\0',sizeof(carpetas));

    int size=0;
    while(ptr != NULL)
    {
        carpetas[size] = ptr;
        size++;
        ptr = strtok(NULL, delim);
    }

    char auxRuta[1000];
    memset(auxRuta,'\0',sizeof(auxRuta));
    auxRuta[0] = '/';

    for(int i=0; i<size-1 ;i++)
    {
        if(i != 0)
            strcat(auxRuta,"/");

        strcat(auxRuta,carpetas[i]);

        struct stat st = {0};

        if(stat(auxRuta,&st) == -1)
            mkdir(auxRuta,0700);

    }
}

void Obtener_Fecha(char* fecha)
{
    time_t tiempo = time(0);
    struct tm *tlocal = localtime(&tiempo);

    strftime(fecha,16,"%d/%m/%y %H:%M:%S",tlocal);
}

void Eliminar_Salto_Linea(char* cadena)
{
    char *p;
    p =  strchr(cadena,  '\n');
    if(p)
        *p = '\0';
}

void Eliminar_Espacios(char* cadena)
{
    char *p;
    p =  strchr(cadena,  ' ');
    if(p)
        *p = '\0';
}

void Ruta_Raid(char *ruta)
{
    char aux[2000];
    memset(aux,'\0',sizeof(aux));

    int index = 0;

    for(int i=0; ruta[i] != '\0' ;i++)
    {
        if(ruta[i] == '.')  //SI LLEGA AL PUNTO ENTONCES YA ESTAMOS EN LA EXTENSION
        {
            aux[index++] = '_';
            aux[index++] = 'r';
            aux[index++] = 'a';
            aux[index++] = '1';
            aux[index++] = ruta[i];

        }else
        {
            aux[index++] = ruta[i];
        }
    }

    memset(caracteristicas.path, '\0', sizeof(caracteristicas.path));
    strcpy(caracteristicas.path, aux);

}


void Mensaje_Aceptacion(char* mensaje)
{
    printf(COLOR_CYAN "%s" RESET,mensaje);
    /*printf(COLOR_RED "%s rojo\n" RESET,mensaje);
    printf(GREEN "%s verde\n" RESET,mensaje);
    printf(YELLOW "%s amarillo\n" RESET,mensaje);
    printf(BLUE "%s azul\n" RESET,mensaje);
    printf(MAGENTA "%s magenta\n" RESET,mensaje);
    printf(CYAN "%s cyan\n" RESET,mensaje);*/
}


void Mensaje_Negacion(char* mensaje)
{
    printf(COLOR_RED "%s" RESET,mensaje);
}

void Mensaje_Advertencia(char* mensaje)
{
    printf(COLOR_YELLOW "%s" RESET,mensaje);
}

void Mensaje_Green(char* mensaje)
{
    printf(COLOR_GREEN "%s" RESET,mensaje);
}

void Mensaje_Blue(char* mensaje)
{
    printf(COLOR_BLUE "%s" RESET,mensaje);
}

void Mensaje_Magenta(char* mensaje)
{
    printf(COLOR_MAGENTA "%s" RESET,mensaje);
}


void Mensaje_Color(char* mensaje, int color)
{
    switch(color)
    {
    case 0:
        printf(COLOR_RED "%s" RESET,mensaje);
        break;

    case 1:
        printf(COLOR_GREEN "%s" RESET,mensaje);
        break;

    case 2:
        printf(COLOR_YELLOW "%s" RESET,mensaje);
        break;

    case 3:
        printf(COLOR_BLUE "%s" RESET,mensaje);
        break;

    case 4:
        printf(COLOR_MAGENTA "%s" RESET,mensaje);
        break;

    case 5:
        printf(COLOR_CYAN "%s" RESET,mensaje);
        break;
    }
}











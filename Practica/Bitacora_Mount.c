#include "Bitacora_Mount.h"

Bnodo* primeroB = NULL;
Bnodo* ultimoB = NULL;

//-------------------- INSERTAR UN NUEVO DATO -------------------
void Ingresar_Bitacora(char* p, char l, int c){
    Bnodo* nuevo = (Bnodo*)malloc(sizeof(Bnodo));
    strcpy(nuevo->path,p);
    nuevo->letra = l;
    nuevo->cont =  c;

    if(! Existe_Bitacora(nuevo->path))//SI NO EXISTE SE CREARA EL NUEVO MOUNT, SI EXITE MENSAJE DE AVISO
    {
        if(primeroB==NULL){
            primeroB=nuevo;
            primeroB->siguiente=NULL;
            primeroB->anterior=NULL;
            ultimoB=primeroB;
        }else{
            ultimoB->siguiente=nuevo;
            nuevo->siguiente=NULL;
            nuevo->anterior=ultimoB;
            ultimoB=nuevo;
        }
        controlLetra++;
        //printf("*** La particion ha sido montada con: %s ***\n",nuevo->id);
    }else
    {
        Actualizar_Bitacora(p);
        //printf("*** ERROR!! Esta particion ya esta montada ***\n");
    }
    //OrdenarLD();
}

//---------------------- EDITAR LISTA DOBLE -------------------------
void Actualizar_Bitacora(char* path){

    Bnodo* actual = (Bnodo*)malloc(sizeof(Bnodo));
    actual=primeroB;
    if(primeroB!=NULL)
    {
        while(actual!=NULL){
            if(strcmp(path,actual->path) == 0)
            {
                //cond=false;
                actual->cont++; break;
            }
            //if(actual->siguiente== NULL || actual->siguiente->id!=busqueda)
            //break;

            actual=actual->siguiente;
        }
        //if(cond==true)
        //    printf("'\nEl nodo deseado no existe con ese ID\n");

    }else{
        //printf("\nLa lista doblemente enlazada esta vacia\n");
    }
}


/*void Eliminar_Bitacora(char* id){

    bool cond=true,cond2=false;

    Bnodo* actual = (Bnodo*)malloc(sizeof(Bnodo));
    actual = primeroB;
    Bnodo* anterior = (Bnodo*)malloc(sizeof(Bnodo));
    anterior=NULL;

    if(primeroB != NULL)
    {
        while(actual!=NULL)
        {
            if(strcmp( actual->id ,id ) == 0)
            {
                if(actual==primeroB){
                    primeroB=primeroB->siguiente;
                    primeroB->anterior = NULL;
                }else if(actual==ultimoB){
                    anterior->siguiente=NULL;
                    ultimoB = anterior;
                }else{
                    anterior->siguiente = actual->siguiente;
                    actual->siguiente->anterior = anterior;
                }
                cond=false;cond2=true;
            }
            anterior=actual;
            actual=actual->siguiente;

            if(cond2)
                break;
        }
        if(cond==true)
            printf("*** ERROR!! No existe una particion montada con ese id ***\n");
        else
            free(anterior);

    }else{
        printf("*** ERROR!! No existe una particion montada con ese id ***\n");
    }
}*/


bool Existe_Bitacora(char* path) //ESTE ME SERVIRA SI EXITE EL DISCO
{
    Bnodo* actual = (Bnodo*)malloc(sizeof(Bnodo));
    actual = primeroB;
    Bnodo* anterior = (Bnodo*)malloc(sizeof(Bnodo));
    anterior=NULL;

    if(primeroB != NULL)
    {
        while(actual!=NULL)
        {
            if(strcmp( actual->path, path ) == 0)
                return true;
            anterior=actual;
            actual=actual->siguiente;
        }
    }
    return false;
}

Bnodo* Get_Bitacora(char* path)
{
    Bnodo* actual = (Bnodo*)malloc(sizeof(Bnodo));
    actual = primeroB;
    Bnodo* anterior = (Bnodo*)malloc(sizeof(Bnodo));
    anterior=NULL;

    if(primeroB != NULL)
    {
        while(actual!=NULL)
        {
            if(strcmp( actual->path, path ) == 0)
                return actual;
            anterior=actual;
            actual=actual->siguiente;
        }
    }
    return NULL;
}

#include "Lista_Mount.h"

Nodo* primero = NULL;
Nodo* ultimo = NULL;

//-------------------- INSERTAR UN NUEVO DATO -------------------
void Insertar_Disco(Nodo* nuevo){
    //Nodo* nuevo = d;

    if(! Existe_Mount(nuevo->path,nuevo->name))//SI NO EXISTE SE CREARA EL NUEVO MOUNT, SI EXITE MENSAJE DE AVISO
    {
        if(primero==NULL){
            primero=nuevo;
            primero->siguiente=NULL;
            primero->anterior=NULL;
            ultimo=primero;
        }else{
            ultimo->siguiente=nuevo;
            nuevo->siguiente=NULL;
            nuevo->anterior=ultimo;
            ultimo=nuevo;
        }
        //controlLetra++;
        Ingresar_Bitacora(nuevo->path,nuevo->letra,1);
        //printf("*** La particion ha sido montada con: %s ***\n",nuevo->id);
        Contador_Mount_Unmount(nuevo,0);
        //printf("*** La particion ha sido montada con: vd%c%d ***\n",nuevo->letra,nuevo->num);
        memset(mensaje,'\0',sizeof(mensaje));
        sprintf(mensaje,"*** La particion ha sido montada con: vd%c%d ***\n",nuevo->letra,nuevo->num);
        Mensaje_Aceptacion(mensaje);

    }else
    {
        //printf("*** ERROR!! Esta particion ya esta montada ***\n");
        Mensaje_Negacion("*** ERROR!! Esta particion ya esta montada ***\n");
    }
    //OrdenarLD();
}

void Desmontar_Disco(char* id){
    //char busqueda[15];
    //strcat(busqueda,id);

    bool cond=true,cond2=false;

    Nodo* actual = (Nodo*)malloc(sizeof(Nodo));
    actual = primero;
    Nodo* anterior = (Nodo*)malloc(sizeof(Nodo));
    anterior=NULL;

    if(primero != NULL)
    {
        while(actual!=NULL)
        {
            //char a[5];
            //memset(a,'\0',sizeof(a));
            //strcpy(a,actual->id);
            //printf("%s\n",a);
            //printf("%s\n",id);
            //if(strcmp( a ,id ) == 0)
            int o = -1;
            if(Validar_Numero(&caracteristicas.id[3])){
                o = atoi(&id[3]);
                if(id[2] == actual->letra && o == actual->num)
                {
                    //CONTADOR DE DESMONTURA
                    char* path = Get_Path(id[2],o);
                    if(strcmp(path,"no") != 0)
                        Contador_Mount_Unmount(Get_Mount(path),1 );

                    if(actual==primero){
                        primero=primero->siguiente;
                        if(primero != NULL)
                            primero->anterior = NULL;
                    }else if(actual==ultimo){
                        anterior->siguiente=NULL;
                        ultimo = anterior;
                    }else{
                        anterior->siguiente = actual->siguiente;
                        actual->siguiente->anterior = anterior;
                    }
                    cond=false;cond2=true;
                    //printf("*** Se Desmonto la particion ***\n");
                    Mensaje_Aceptacion("*** Se Desmonto la particion ***\n");
                }
            }else
            {
                printf("*** ERROR!! dato no compatible ---> ***\n");
            }
            anterior=actual;
            actual=actual->siguiente;

            if(cond2)
                break;
        }
        if(cond==true)
            //printf("*** ERROR!! No existe una particion montada con ese id ***\n");
            Mensaje_Negacion("*** ERROR!! No existe una particion montada con ese id ***\n");
        else
            free(anterior);

    }else{
        //printf("*** ERROR!! No existe una particion montada con ese id ***\n");
        Mensaje_Negacion("*** ERROR!! No existe una particion montada con ese id ***\n");
    }
}


bool Existe_Mount(char* path, char* id)
{
    Nodo* actual = (Nodo*)malloc(sizeof(Nodo));
    actual = primero;
    Nodo* anterior = (Nodo*)malloc(sizeof(Nodo));
    anterior=NULL;

    if(primero != NULL)
    {
        while(actual!=NULL)
        {
            if(strcmp( actual->name ,id ) == 0 && strcmp( actual->path, path ) == 0)
                return true;
            anterior=actual;
            actual=actual->siguiente;
        }
    }
    return false;
}

Nodo* Get_Mount(char* path)
{
    Nodo* actual = (Nodo*)malloc(sizeof(Nodo));
    actual = primero;
    Nodo* anterior = (Nodo*)malloc(sizeof(Nodo));
    anterior=NULL;

    Nodo* aux = (Nodo*)malloc(sizeof(Nodo));
    aux->inicio = -1;

    if(primero != NULL)
    {
        while(actual!=NULL)
        {
            if(strcmp( actual->path, path ) == 0)
                return actual;
            anterior=actual;
            actual=actual->siguiente;
        }
    }
    return aux;
}

bool Tengo_Mount(char l, int num)
{
    Nodo* actual = (Nodo*)malloc(sizeof(Nodo));
    actual = primero;
    Nodo* anterior = (Nodo*)malloc(sizeof(Nodo));
    anterior=NULL;


    if(primero != NULL)
    {
        while(actual!=NULL)
        {
            if(actual->letra == l && actual->num == num)
                return true;
            anterior=actual;
            actual=actual->siguiente;
        }
    }
    return false;
}

char* Get_Path(char l, int num)
{
    Nodo* actual = (Nodo*)malloc(sizeof(Nodo));
    actual = primero;
    Nodo* anterior = (Nodo*)malloc(sizeof(Nodo));
    anterior=NULL;


    if(primero != NULL)
    {
        while(actual!=NULL)
        {
            if( actual->letra == l && actual->num == num)
                return actual->path;
            anterior=actual;
            actual=actual->siguiente;
        }
    }
    return "no";
}

int Get_Inicio(char l, int num)
{
    Nodo* actual = (Nodo*)malloc(sizeof(Nodo));
    actual = primero;
    Nodo* anterior = (Nodo*)malloc(sizeof(Nodo));
    anterior=NULL;


    if(primero != NULL)
    {
        while(actual!=NULL)
        {
            if( actual->letra == l && actual->num == num)
                return actual->inicio;
            anterior=actual;
            actual=actual->siguiente;
        }
    }
    return -1;
}

void Contador_Mount_Unmount(Nodo* nuevo,int opt)
{
    FILE* disco;

    disco = fopen(nuevo->path,"r+b");

    if(disco == NULL){
        //printf("*** ERROR!! Al abrir el disco ***\n");
        Mensaje_Negacion("*** ERROR!! Al abrir el disco ***\n");
        return;
    }

    //MBR mbr;
    //fread(&mbr,sizeof(MBR),1,disco);

    SuperBlock sblock;
    fseek(disco,nuevo->inicio,SEEK_SET);
    fread(&sblock,sizeof(SuperBlock),1,disco);//SE LEE EL DISCO CON EL TAMAÑO DEL SUPERBLOQUE

    if(sblock.s_block_size != 0)//SI ES VERDADERO NO HAY SUPER BLOCK, SI ES FALSE ES PORQUE YA EXITE SUPER BLOCK
    {
        if(opt == 0){
            sblock.s_mnt_count = sblock.s_mnt_count +1;
            Obtener_Fecha(sblock.s_mtime);
        }else if(opt == 1)
        {
            Obtener_Fecha(sblock.s_umtime);
        }


        //LLENAR LA PARTICION CON LOS INODOS, BLOQUES Y LOS BITMAP'S
        fseek(disco,nuevo->inicio,SEEK_SET);
        fwrite(&sblock,sizeof(SuperBlock),1,disco);

    }


    fclose(disco);
}


void Graficar_Mount(void)
{
    FILE *fd;
    if(primero!=NULL)
    {

        Nodo* actual = (Nodo*)malloc(sizeof(Nodo));
        actual=primero;

        char c[150];
        char d[150];
        char copy[150];
        char copy2[150];
        bool cond=false;

        //char direccion[] = "/home/diego/GraficoLD.dot";
        char direccion[] = "/home/diego/Escritorio/Grafico_Mount.dot";

        fd = fopen(direccion,"wt");

        if(fd == NULL){
            //printf("\nError al crear el Grafica_Mount'n");
            Mensaje_Negacion("\nError al crear el Grafica_Mount\n");
            return;
        }
        //-------------- INICIO DEL CODIGO GRAPHVIZ -----------------
        char begin[] = "digraph G {\n";
        fputs(begin,fd);

        fputs("rankdir=\"LR\"\n",fd);
        fputs("node [shape=box]\n",fd);

        memset(c,0,sizeof(c));

        //-------------- CUERPO DEL GRAPHVIZ -----------------
        while(actual!=NULL)
        {
            //sprintf(c,"\"id: vd%c%d\n name: %s\n path: %s\"",actual->letra,actual->num,actual->name,actual->path);
            sprintf(c,"\"id: vd%c%d\n name: %s\n type: %c\"",actual->letra,actual->num,actual->name,actual->type);

            /*if(cond!=true){
                strcpy(copy,c);
                strcat(copy," [shape=box];\n"); //  IDENTIFICA CABEZA
            }

            strcpy(copy2,c);
            strcat(copy2," [shape=doublecircle];\n"); //  IDENTIFICA FINAL
            cond=true;*/

            if(actual->siguiente!=NULL){
                //sprintf(d," -> \"id: vd%c%d\n name: %s\n path: %s\"\n",actual->siguiente->letra,actual->siguiente->num,actual->siguiente->name,actual->siguiente->path);
                sprintf(d," -> \"id: vd%c%d\n name: %s\n type: %c\"\n",actual->siguiente->letra,actual->siguiente->num,actual->siguiente->name,actual->siguiente->type);
                strcat(c,d);

            }else{
            //-------------- FIN DEL GRAPHVIZ --------------------
                fputs(copy,fd);
                fputs(copy2,fd);
                fputs("}",fd);
                fclose(fd);break;
            }

            fputs(c,fd);

            //sprintf(c,"\"id: %d  dato: %c\"",actual->siguiente->id,actual->siguiente->dato);
            //sprintf(d," -> \"id: %d  dato: %c\"\n",actual->id,actual->dato);
            //strcat(c,d);

            //fputs(c,fd);

            actual=actual->siguiente;
        }

        //system("dot -Tpng GraficoLD.dot -o GraficoLD.png");
        //system("nohup display /home/diego/GraficoLD.png &");
        system("dot -Tpng -O /home/diego/Escritorio/Grafico_Mount.dot");
        system("xdg-open /home/diego/Escritorio/Grafico_Mount.dot.png");

        //system("clear");

        return;
    }
}






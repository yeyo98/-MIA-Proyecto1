#include "Metodos_Analizar.h"


void Analizar_Path(char* comando, int *tamanio)
{
    char pivote[1000];
    memset(pivote,'\0',sizeof(pivote));

    int pos =0;
    while(comando[*tamanio] != '\0')
    {

        if(comando[*tamanio] == ' ' || comando[*tamanio] == '\n' ){

            while(comando[*tamanio] == ' '){
                (*tamanio)++;
            }
            (*tamanio)--;

            strcpy(caracteristicas.path,pivote);

            break;
        }else if(comando[*tamanio] == '#'){//SE ENCUENTRA CON EL INICIO DE UN COMENTARIO
            if(*tamanio == 0)//SE LO ENCUENTRA AL INICIO DEL COMANDO
                return;
            //Elegir_Comando(pivote, comando, *tamanio+1);
            return;
        }else if(comando[*tamanio] == '\"' ){
            (*tamanio)++;
            while(comando[*tamanio] != '\"'){
                pivote[pos] = comando[*tamanio];
                pos++;
                (*tamanio)++;

            }
        }else{
            while(comando[*tamanio] != ' ' && comando[*tamanio] != '\n' ){
                pivote[pos] = comando[*tamanio];
                pos++;
                (*tamanio)++;

            }
            (*tamanio)--;
        }

        (*tamanio)++;
    }
}

void Analizar_Size(char* comando, int *tamanio)
{
    char pivote[1000];
    memset(pivote,'\0',sizeof(pivote));

    int pos =0;
    while(comando[*tamanio] != '\0')
    {

        if(comando[*tamanio] == ' ' || comando[*tamanio] == '\n' ){

            while(comando[*tamanio] == ' '){
                (*tamanio)++;
            }
            (*tamanio)--;


            if(Validar_Numero(pivote))
                caracteristicas.size = atoi(pivote);

            break;
        }else if(comando[*tamanio] == '#'){//SE ENCUENTRA CON EL INICIO DE UN COMENTARIO
            return;
        }else
        {
            while(comando[*tamanio] != ' ' && comando[*tamanio] != '\n' ){
                pivote[pos] = comando[*tamanio];
                pos++;
                (*tamanio)++;

            }
            (*tamanio)--;
        }

        (*tamanio)++;
    }
}

void Analizar_Fit(char* comando, int *tamanio)
{
    char pivote[1000];
    memset(pivote,'\0',sizeof(pivote));

    int pos =0;
    while(comando[*tamanio] != '\0')
    {

        if(comando[*tamanio] == ' ' || comando[*tamanio] == '\n' ){

            while(comando[*tamanio] == ' '){
                (*tamanio)++;
            }
            (*tamanio)--;


            if(strcasecmp(pivote,"bf") == 0  )
            {
                caracteristicas.fit = 'b';
            }else if(strcasecmp(pivote,"ff") == 0)
            {
                caracteristicas.fit = 'f';
            }else if(strcasecmp(pivote,"wf") == 0)
            {
                caracteristicas.fit = 'w';
            }else
            {
                printf("\t*** ERROR!! El parametro \"%s\" no es valido para -fit ***\n",pivote);
            }


            break;
        }else if(comando[*tamanio] == '#'){//SE ENCUENTRA CON EL INICIO DE UN COMENTARIO
            return;
        }else
        {
            while(comando[*tamanio] != ' ' && comando[*tamanio] != '\n' ){
                pivote[pos] = comando[*tamanio];
                pos++;
                (*tamanio)++;

            }
            (*tamanio)--;
        }

        (*tamanio)++;
    }
}


void Analizar_Unit(char* comando, int *tamanio)
{
    char pivote[1000];
    memset(pivote,'\0',sizeof(pivote));

    int pos =0;
    while(comando[*tamanio] != '\0')
    {

        if(comando[*tamanio] == ' ' || comando[*tamanio] == '\n' ){

            while(comando[*tamanio] == ' '){
                (*tamanio)++;
            }
            (*tamanio)--;


            if(strcasecmp(pivote,"k") == 0 )
            {
                caracteristicas.unit = 'k';
            }else if( strcasecmp(pivote,"m") == 0 )
            {
                caracteristicas.unit = 'm';
            }else
            {
                printf("\t*** ERROR!! El parametro \"%s\" no es valido para -unit ***\n",pivote);
            }


            break;
        }else if(comando[*tamanio] == '#'){//SE ENCUENTRA CON EL INICIO DE UN COMENTARIO
            return;
        }else
        {
            while(comando[*tamanio] != ' ' && comando[*tamanio] != '\n' ){
                pivote[pos] = comando[*tamanio];
                pos++;
                (*tamanio)++;

            }
            (*tamanio)--;
        }

        (*tamanio)++;
    }
}

void Analizar_Unit_F(char* comando, int *tamanio)
{
    char pivote[1000];
    memset(pivote,'\0',sizeof(pivote));

    int pos =0;
    while(comando[*tamanio] != '\0')
    {

        if(comando[*tamanio] == ' ' || comando[*tamanio] == '\n' ){

            while(comando[*tamanio] == ' '){
                (*tamanio)++;
            }
            (*tamanio)--;


            if(strcasecmp(pivote,"k") == 0 )
            {
                caracteristicas.unit_f = 'k';
            }else if( strcasecmp(pivote,"m") == 0 )
            {
                caracteristicas.unit_f = 'm';
            }else
            {
                printf("\t*** ERROR!! El parametro \"%s\" no es valido para -unit ***\n",pivote);
            }


            break;
        }else if(comando[*tamanio] == '#'){//SE ENCUENTRA CON EL INICIO DE UN COMENTARIO
            return;
        }else
        {
            while(comando[*tamanio] != ' ' && comando[*tamanio] != '\n' ){
                pivote[pos] = comando[*tamanio];
                pos++;
                (*tamanio)++;

            }
            (*tamanio)--;
        }

        (*tamanio)++;
    }
}



void Analizar_Type(char* comando, int *tamanio)
{
    char pivote[1000];
    memset(pivote,'\0',sizeof(pivote));

    int pos =0;
    while(comando[(*tamanio)] != '\0')
    {

        if(comando[(*tamanio)] == ' ' || comando[(*tamanio)] == '\n' ){

            while(comando[(*tamanio)] == ' '){
                (*tamanio)++;
            }
            (*tamanio)--;

            if(strcasecmp(pivote,"p") == 0 )
            {
                caracteristicas.type = 'p';
            }else if( strcasecmp(pivote,"e") == 0 )
            {
                caracteristicas.type = 'e';
            }else if( strcasecmp(pivote,"l") == 0 )
            {
                caracteristicas.type = 'l';
            }else
            {
                printf("\t*** ERROR!! El parametro \"%s\" no es valido para -type ***\n",pivote);
            }


            break;
        }else if(comando[(*tamanio)] == '#'){//SE ENCUENTRA CON EL INICIO DE UN COMENTARIO
            return;
        }else
        {
            while(comando[(*tamanio)] != ' ' && comando[(*tamanio)] != '\n' ){
                pivote[pos] = comando[(*tamanio)];
                pos++;
                (*tamanio)++;

            }
            (*tamanio)--;
        }

        (*tamanio)++;
    }
}


void Analizar_Delete(char* comando, int *tamanio)
{
    char pivote[1000];
    memset(pivote,'\0',sizeof(pivote));

    int pos =0;
    while(comando[*(tamanio)] != '\0')
    {

        if(comando[*(tamanio)] == ' ' || comando[*(tamanio)] == '\n' ){

            while(comando[*(tamanio)] == ' '){
                *(tamanio) = *(tamanio) +1;
            }
            *(tamanio) = *(tamanio)-1;

            if(strcasecmp(pivote,"fast") == 0  )
            {
                caracteristicas.deleteP = 'a';
            }else if(strcasecmp(pivote,"full") == 0)
            {
                caracteristicas.deleteP = 'u';
            }else
            {
                printf("\t*** ERROR!! El parametro \"%s\" no es valido para -fit ***\n",pivote);
            }

            break;
        }else if(comando[*(tamanio)] == '#'){//SE ENCUENTRA CON EL INICIO DE UN COMENTARIO
            return;
        }else
        {
            while(comando[*(tamanio)] != ' ' && comando[*(tamanio)] != '\n' ){
                pivote[pos] = comando[*(tamanio)];
                pos++;
                *(tamanio) = *(tamanio) +1;

            }
            *(tamanio)=*(tamanio)-1;
        }

        *(tamanio) = *(tamanio) +1;
    }
}


void Analizar_Add(char* comando, int *tamanio)
{
    char pivote[1000];
    memset(pivote,'\0',sizeof(pivote));

    int pos =0;
    while(comando[(*tamanio)] != '\0')
    {

        if(comando[(*tamanio)] == ' ' || comando[(*tamanio)] == '\n' ){

            while(comando[(*tamanio)] == ' '){
                (*tamanio)++;
            }
            (*tamanio)--;


            if(pivote[0] == '-')
            {
                char auxP[1000];
                for(int i=0; i<strlen(pivote)-1 ;i++)
                    auxP[i] = pivote[i+1];

                caracteristicas.negado = true;
                if(Validar_Numero(auxP))
                     caracteristicas.add = (atoi(pivote) *-1);

            }else
            {

                if(Validar_Numero(pivote))
                    caracteristicas.add = atoi(pivote);

            }

            break;
        }else if(comando[(*tamanio)] == '#'){//SE ENCUENTRA CON EL INICIO DE UN COMENTARIO
            return;
        }else
        {
            while(comando[(*tamanio)] != ' ' && comando[(*tamanio)] != '\n' ){
                pivote[pos] = comando[(*tamanio)];
                pos++;
                (*tamanio)++;

            }
            (*tamanio)--;
        }

        (*tamanio)++;
    }
}


void Analizar_Name(char* comando, int* tamanio)
{
    char pivote[1000];
    memset(pivote,'\0',sizeof(pivote));

    int pos =0;
    while(comando[*(tamanio)] != '\0')
    {

        if(comando[*(tamanio)] == ' ' || comando[*(tamanio)] == '\n' ){

            while(comando[*(tamanio)] == ' '){
                *(tamanio) = *(tamanio) +1;
            }
            strcpy(caracteristicas.name,pivote);
            *(tamanio) = *(tamanio) -1;

            break;
        }else if(comando[*(tamanio)] == '#'){//SE ENCUENTRA CON EL INICIO DE UN COMENTARIO
            if(*(tamanio) == 0)//SE LO ENCUENTRA AL INICIO DEL COMANDO
                return;
            //Elegir_Comando(pivote, comando, *(tamanio)+1);
            return;
        }else if(comando[*(tamanio)] == '\"' ){
            *(tamanio) = *(tamanio) +1;
            while(comando[*(tamanio)] != '\"'){
                pivote[pos] = comando[*(tamanio)];
                pos++;
                *(tamanio) = *(tamanio) +1;

            }
        }else{
            while(comando[*(tamanio)] != ' ' && comando[*(tamanio)] != '\n' ){
                pivote[pos] = comando[*(tamanio)];
                pos++;
                *(tamanio) = *(tamanio) +1;

            }
            *(tamanio)=*(tamanio)-1;
        }
        *(tamanio) = *(tamanio) +1;
    }
}


void Analizar_Id(char* comando, int* tamanio)
{
    char pivote[1000];
    memset(pivote,'\0',sizeof(pivote));

    int pos =0;
    while(comando[*(tamanio)] != '\0')
    {

        if(comando[*(tamanio)] == ' ' || comando[*(tamanio)] == '\n' ){

            while(comando[*(tamanio)] == ' '){
                *(tamanio) = *(tamanio) +1;
            }
            strcpy(caracteristicas.id,pivote);
            *(tamanio) = *(tamanio) -1;

            break;
        }else if(comando[*(tamanio)] == '#'){//SE ENCUENTRA CON EL INICIO DE UN COMENTARIO
            if(*(tamanio) == 0)//SE LO ENCUENTRA AL INICIO DEL COMANDO
                return;
            //Elegir_Comando(pivote, comando, *(tamanio)+1);
            return;
        }else if(comando[*(tamanio)] == '\"' ){
            *(tamanio) = *(tamanio) +1;
            while(comando[*(tamanio)] != '\"'){
                pivote[pos] = comando[*(tamanio)];
                pos++;
                *(tamanio) = *(tamanio) +1;

            }
        }else{
            while(comando[*(tamanio)] != ' ' && comando[*(tamanio)] != '\n' ){
                pivote[pos] = comando[*(tamanio)];
                pos++;
                *(tamanio) = *(tamanio) +1;

            }
            *(tamanio)=*(tamanio)-1;
        }
        *(tamanio) = *(tamanio) +1;
    }
}

void Analizar_Fs(char* comando, int *tamanio)
{
    char pivote[1000];
    memset(pivote,'\0',sizeof(pivote));

    int pos =0;
    while(comando[*(tamanio)] != '\0')
    {

        if(comando[*(tamanio)] == ' ' || comando[*(tamanio)] == '\n' ){

            while(comando[*(tamanio)] == ' '){
                *(tamanio) = *(tamanio) +1;
            }
            *(tamanio) = *(tamanio) -1 ;

            if(strcasecmp(pivote,"ext2") == 0  )
            {
                caracteristicas.fs = '3';
            }else if(strcasecmp(pivote,"ext3") == 0)
            {
                caracteristicas.fs = '3';
            }else
            {
                printf("\t*** ERROR!! El parametro \"%s\" no es valido para -fs ***\n",pivote);
            }

            break;
        }else if(comando[*(tamanio)] == '#'){//SE ENCUENTRA CON EL INICIO DE UN COMENTARIO
            return;
        }else
        {
            while(comando[*(tamanio)] != ' ' && comando[*(tamanio)] != '\n' ){
                pivote[pos] = comando[*(tamanio)];
                pos++;
                *(tamanio) = *(tamanio) +1;

            }
            *(tamanio)=*(tamanio)-1;
        }

        *(tamanio) = *(tamanio) +1;
    }
}



void Analizar_Usr(char* comando, int* tamanio)
{
    char pivote[1000];
    memset(pivote,'\0',sizeof(pivote));

    int pos =0;
    while(comando[*(tamanio)] != '\0')
    {

        if(comando[*(tamanio)] == ' ' || comando[*(tamanio)] == '\n' ){

            while(comando[*(tamanio)] == ' '){
                *(tamanio) = *(tamanio) +1;
            }
            strcpy(caracteristicas.usr,pivote);
            *(tamanio) = *(tamanio) -1;

            break;
        }else if(comando[*(tamanio)] == '#'){//SE ENCUENTRA CON EL INICIO DE UN COMENTARIO
            if(*(tamanio) == 0)//SE LO ENCUENTRA AL INICIO DEL COMANDO
                return;
            //Elegir_Comando(pivote, comando, *(tamanio)+1);
            return;
        }else if(comando[*(tamanio)] == '\"' ){
            *(tamanio) = *(tamanio) +1;
            while(comando[*(tamanio)] != '\"'){
                pivote[pos] = comando[*(tamanio)];
                pos++;
                *(tamanio) = *(tamanio) +1;

            }
        }else{
            while(comando[*(tamanio)] != ' ' && comando[*(tamanio)] != '\n' ){
                pivote[pos] = comando[*(tamanio)];
                pos++;
                *(tamanio) = *(tamanio) +1;

            }
            *(tamanio)=*(tamanio)-1;
        }
        *(tamanio) = *(tamanio) +1;
    }
}


void Analizar_Pwd(char* comando, int* tamanio)
{
    char pivote[1000];
    memset(pivote,'\0',sizeof(pivote));

    int pos =0;
    while(comando[*(tamanio)] != '\0')
    {

        if(comando[*(tamanio)] == ' ' || comando[*(tamanio)] == '\n' ){

            while(comando[*(tamanio)] == ' '){
                *(tamanio) = *(tamanio) +1;
            }
            strcpy(caracteristicas.pwd,pivote);
            *(tamanio) = *(tamanio) -1;

            break;
        }else if(comando[*(tamanio)] == '#'){//SE ENCUENTRA CON EL INICIO DE UN COMENTARIO
            if(*(tamanio) == 0)//SE LO ENCUENTRA AL INICIO DEL COMANDO
                return;
            //Elegir_Comando(pivote, comando, *(tamanio)+1);
            return;
        }else if(comando[*(tamanio)] == '\"' ){
            *(tamanio) = *(tamanio) +1;
            while(comando[*(tamanio)] != '\"'){
                pivote[pos] = comando[*(tamanio)];
                pos++;
                *(tamanio) = *(tamanio) +1;

            }
        }else{
            while(comando[*(tamanio)] != ' ' && comando[*(tamanio)] != '\n' ){
                pivote[pos] = comando[*(tamanio)];
                pos++;
                *(tamanio) = *(tamanio) +1;

            }
            *(tamanio)=*(tamanio)-1;
        }
        *(tamanio) = *(tamanio) +1;
    }
}

void Analizar_Grp(char* comando, int* tamanio)
{
    char pivote[1000];
    memset(pivote,'\0',sizeof(pivote));

    int pos =0;
    while(comando[*(tamanio)] != '\0')
    {
        if(comando[*(tamanio)] == ' ' || comando[*(tamanio)] == '\n' ){

            while(comando[*(tamanio)] == ' '){
                *(tamanio) = *(tamanio) +1;
            }
            strcpy(caracteristicas.grp,pivote);
            *(tamanio) = *(tamanio) -1;

            break;
        }else if(comando[*(tamanio)] == '#'){//SE ENCUENTRA CON EL INICIO DE UN COMENTARIO
            if(*(tamanio) == 0)//SE LO ENCUENTRA AL INICIO DEL COMANDO
                return;
            //Elegir_Comando(pivote, comando, *(tamanio)+1);
            return;
        }else if(comando[*(tamanio)] == '\"' ){
            *(tamanio) = *(tamanio) +1;
            while(comando[*(tamanio)] != '\"'){
                pivote[pos] = comando[*(tamanio)];
                pos++;
                *(tamanio) = *(tamanio) +1;

            }
        }else{
            while(comando[*(tamanio)] != ' ' && comando[*(tamanio)] != '\n' ){
                pivote[pos] = comando[*(tamanio)];
                pos++;
                *(tamanio) = *(tamanio) +1;
            }
            *(tamanio)=*(tamanio)-1;
        }
        *(tamanio) = *(tamanio) +1;
    }
}

void Analizar_P(char* comando, int *tamanio)
{
    char pivote[1000];
    memset(pivote,'\0',sizeof(pivote));

    int pos =0;
    while(comando[*tamanio] != '\0')
    {
        if(comando[*tamanio] == ' ' || comando[*tamanio] == '\n' ){

            while(comando[*tamanio] == ' '){
                (*tamanio)++;
            }
            (*tamanio)--;

            caracteristicas.p = true;

            break;
        }else if(comando[*tamanio] == '#'){//SE ENCUENTRA CON EL INICIO DE UN COMENTARIO
            return;
        }else
        {
            while(comando[*tamanio] != ' ' && comando[*tamanio] != '\n' ){
                pivote[pos] = comando[*tamanio];
                pos++;
                (*tamanio)++;
            }
            (*tamanio)--;
        }
        (*tamanio)++;
    }
}

void Analizar_Cont(char* comando, int *tamanio)
{
    char pivote[1000];
    memset(pivote,'\0',sizeof(pivote));

    int pos =0;
    while(comando[*tamanio] != '\0')
    {
        if(comando[*tamanio] == ' ' || comando[*tamanio] == '\n' ){

            while(comando[*tamanio] == ' '){
                (*tamanio)++;
            }
            (*tamanio)--;

            strcpy(caracteristicas.cont,pivote);

            break;
        }else if(comando[*tamanio] == '#'){//SE ENCUENTRA CON EL INICIO DE UN COMENTARIO
            if(*tamanio == 0)//SE LO ENCUENTRA AL INICIO DEL COMANDO
                return;
            //Elegir_Comando(pivote, comando, *tamanio+1);
            return;
        }else if(comando[*tamanio] == '\"' ){
            (*tamanio)++;
            while(comando[*tamanio] != '\"'){
                pivote[pos] = comando[*tamanio];
                pos++;
                (*tamanio)++;
            }
        }else{
            while(comando[*tamanio] != ' ' && comando[*tamanio] != '\n' ){
                pivote[pos] = comando[*tamanio];
                pos++;
                (*tamanio)++;
            }
            (*tamanio)--;
        }
        (*tamanio)++;
    }
}




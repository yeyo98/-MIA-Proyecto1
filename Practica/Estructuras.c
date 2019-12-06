#include "Estructuras.h"


void Limpiar_Datos(void){
    //Datos* a = &caracteristicas;
    caracteristicas.size = -1;
    caracteristicas.fit = 'f';
    caracteristicas.unit = 'm';
    memset(caracteristicas.path, '\0', sizeof(caracteristicas.path));
    strcpy(caracteristicas.path , "no");

    caracteristicas.unit_f = 'k';
    caracteristicas.type = 'p';
    caracteristicas.fit_f = 'w';
    caracteristicas.deleteP = '\0';
    caracteristicas.add = 0;
    caracteristicas.negado = false;

    memset(caracteristicas.name, '\0', sizeof(caracteristicas.name));
    strcat(caracteristicas.name, "nel");

    memset(caracteristicas.id, '\0', sizeof(caracteristicas.id));
    strcat(caracteristicas.id, "nel");

    caracteristicas.fs = '3';

    memset(caracteristicas.usr, '\0', sizeof(caracteristicas.usr));
    strcat(caracteristicas.usr, "nel");

    memset(caracteristicas.pwd, '\0', sizeof(caracteristicas.pwd));
    strcat(caracteristicas.pwd, "nel");

    memset(caracteristicas.grp, '\0', sizeof(caracteristicas.grp));
    strcat(caracteristicas.grp, "nel");

    caracteristicas.p = false;
    memset(caracteristicas.cont, '\0', sizeof(caracteristicas.cont));
    strcpy(caracteristicas.cont , "no");
}

void DesLoguear(void)
{
    memset(daLoguer.usr, '\0', sizeof(daLoguer.usr));
    strcat(daLoguer.usr, "nel");

    memset(daLoguer.pwd, '\0', sizeof(daLoguer.pwd));
    strcat(daLoguer.pwd, "nel");

    memset(daLoguer.id, '\0', sizeof(daLoguer.id));
    strcat(daLoguer.id, "nel");

    memset(daLoguer.grp, '\0', sizeof(daLoguer.grp));
    strcat(daLoguer.grp, "nel");

    daLoguer.usuario = 0;
    daLoguer.grupo = 0;
}

void Aux_Llenar_Loguer()
{
    daLoguer.usuario = 1;
    daLoguer.grupo = 1;
}

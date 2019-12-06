
#Generacion de discos para la calificacion de la segunda fase FAVOR VERIFICAR QUE NO REALIZA Y COMENTAR LOS COMANDOS
#GRACIAS Y EXITOS EN SU CALIFICACIÓN... 
MKdisk -sizE=120 -unit=M -path="/home/archivos/fase 2/D1.disk" -fit=FF
mkdisK -sizE=120 -unit=M -path="/home/archivos/fase 2/D2.disk" -fit=FF

#Crearemos las mismas 12 particiones en elprimer disco
#PrimarIA 1 es de 10 Mb
#PrimarIA 2 es de 10 Mb
#PrimarIA 3 es de 10 Mb
#eL RESTO son de 5 Mb (La extendida es de 50 Mb)
#Tipo:		Primaria	Primaria	Extendida	Logica	Logica	Logica	Logica	Logica	Logica	Logica	Logica Primaria
#Ajuste:	Primer 		Mejor		-			Peor	Peor	Peor	Peor	Peor	Peor	Peor	Mejor   Peor
#name:	PRI 1		PRI 2		EXT			LOG 1	LOG 2	LOG 3	LOG 4	LOG 5	LOG 6	LOG 7	LOG8   PRI 3

fdisk -sizE=10 -UniT=M -path="/home/archivos/fase 2/D1.disk" -type=P -fit=FF -name="PRI1"
fdisk -path="/home/archivos/fase 2/D1.disk" -sizE=10000 -fit=BF -name="PRI2"
FdisK -path="/home/archivos/fase 2/D1.disk" -type=E -name="EXT" -sizE=51200
fdisk -type=L -sizE=5120 -Unit=K -path="/home/archivos/fase 2/D1.disk" -name="LOG1"
fdisK -name="LOG2" -sizE=5120 -path="/home/archivos/fase 2/D1.disk" -type=L
fdisk -type=L -path="/home/archivos/fase 2/D2.disk" -name="LOG3" -sizE=5120 -uniT=K
fdiSk -sizE=5120 -unIt=K -PAth="/home/archivos/fase 2/D1.disk" -name="LOG4" -type=L
fdisk -type=L -sizE=5120 -unit=K -path="/home/archivos/fase 2/D1.disk" -name="LOG5"
FdisK -sizE=5120 -name="LOG6" -UNit=K -path="/home/archivos/fase 2/D1.disk" -type=L
fdIsk -type=L -sizE=5120 -unit=K -path="/home/archivos/fase 2/D1.disk" -name="LOG7"
fdIsk -type=L -sizE=5120 -unit=K -path="/home/archivos/fase 2/D1.disk" -name="LOG8" -fit=BF
fdisk -unit=M -path="/home/archivos/fase 2/D1.disk" -sizE=10 -name="PRI3"
#Crearemos las mismas 12 particiones en el segundo disco
#PrimarIA 1 es de 10 Mb
#PrimarIA 2 es de 10 Mb
#PrimarIA 3 es de 10 Mb
#eL RESTO son de 5 Mb (La extendida es de 50 Mb)
#Tipo:		Primaria	Primaria	Extendida	Logica	Logica	Logica	Logica	Logica	Logica	Logica	Logica Primaria
#Ajuste:	Primer 		Mejor		-			Peor	Peor	Peor	Peor	Peor	Peor	Peor	Mejor   Peor
#name:	PRI 1		PRI 2		EXT			LOG 1	LOG 2	LOG 3	LOG 4	LOG 5	LOG 6	LOG 7	LOG8   PRI 3
fdisk -sizE=10 -UniT=M -path="/home/archivos/fase 2/D2.disk" -type=P -fit=FF -name="PRI1"
fdisk -path="/home/archivos/fase 2/D2.disk" -sizE=10000 -fit=BF -name="PRI2"
FdisK -path="/home/archivos/fase 2/D2.disk" -type=E -name="EXT" -sizE=51200
fdisk -type=L -sizE=5120 -Unit=K -path="/home/archivos/fase 2/D2.disk" -name="LOG1"
fdisK -name="LOG2" -sizE=5120 -path="/home/archivos/fase 2/D2.disk" -type=L
fdisk -type=L -path="/home/archivos/fase 2/D2.disk" -name="LOG 3" -sizE=5120 -uniT=K
fdiSk -sizE=5120 -unIt=K -PAth="/home/archivos/fase 2/D2.disk" -name="LOG4" -type=L
fdisk -type=L -sizE=5120 -unit=K -path="/home/archivos/fase 2/D2.disk" -name="LOG5"
FdisK -sizE=5120 -name="LOG 6" -UNit=K -path="/home/archivos/fase 2/D2.disk" -type=L
fdIsk -type=L -sizE=5120 -unit=K -path="/home/archivos/fase 2/D2.disk" -name="LOG7"
fdIsk -type=L -sizE=5120 -unit=K -path="/home/archivos/fase 2/D2.disk" -name="LOG8" -fit=BF
fdisk -unit=M -path="/home/archivos/fase 2/D2.disk" -sizE=10 -name="PRI3"
#Debería generar los ids: vda1, vdb1, vda2, vdb2, vda3, vdb3
#vda para el D2.disk y vdb para d1.disk
mouNt -NamE="PRI1"  -path="/home/archivos/fase 2/D2.disk"
moUnt -PAth="/home/archivos/fase 2/D2.disk" -name="PRI2"
mOunt -name="LOG1" -path="/home/archivos/fase 2/D2.disk"
mouNt -path="/home/archivos/fase 2/D1.disk" -NamE="PRI1"
moUnt -name="PRI2" -PAth="/home/archivos/fase 2/D1.disk"
mouNt -path="/home/archivos/fase 2/D1.disk" -name="LOG1"


#Crea el sistema de archivos EXT3 en todas las particiones primarias PRI1 
mkfs -id=vda1 -type=Full 
Mkfs -id=vdb1 
#Crea el sistema de archivos EXT3 en todas las particiones Logicas
mkfs -id=vda3 -type=Full 
Mkfs -id=vdb3 -type=FasT
#nos logueamos en la particion PRI1 con nuestro usuario root, cambiar pwd a su no.carne
loGin -usr=root -pwd=123 -id=vda1
#Crearemos un grupo llamado "MySql" ,"Oracle" y "DB2"
mkgrp -name="MySql"
mkgrp -name="Oracle"
mkgrp -name="DB2"
#Crearemos 3 usuarios llamados user1, user2 y user3
Mkusr -usr="user1" -grp=MySql -pwd=user1
Mkusr -usr="user2" -grp=Oracle -pwd=user2
Mkusr -usr="user3" -grp=MySql -pwd=user3
#vamos a ver el archivo user.txt en el reporte tree
rep -name=tree -Path="/home/archivos/reportes/tree01.jpg" -id=vda1


pause


#eliminaremos el grupo DB2 y usuario user3
rmgrp -name="DB2"
rmusr -usr=user3 
#vamos a ver el archivo user.txt en el reporte file
rep -name=file -Path="/home/archivos/reportes/file01.txt" -id=vda1 -ruta="/users.txt"


pause


#vamos a cerrar sesión en vda1 e iniciar sesión en vda3 (Logica1)-------------------------------------------
LogouT
loGin -usr=root -pwd=123 -id=vda3
#Crearemos un grupo llamado "MySql" ,"Oracle" y "DB2"
mkgrp -name="MySql"
mkgrp -name="Oracle"
mkgrp -name="DB2"
#Crearemos 3 usuarios llamados user1, user2 y user3
Mkusr -usr="user1" -grp=MySql -pwd=user1
Mkusr -usr="user2" -grp=Oracle -pwd=user2
Mkusr -usr="user3" -grp=MySql -pwd=user3
#vamos a ver el archivo user.txt en el reporte tree
rep -name=tree -Path="/home/archivos/reportes/tree02.jpg" -id=vda3

pause

#eliminaremos el grupo DB2 y usuario user3
rmgrp -name="DB2"
rmusr -usr=user3 
#vamos a ver el archivo user.txt en el reporte FILE
rep -name=file -Path="/home/archivos/reportes/file02.txt" -id=vda3 -ruta="/user.txt"

pause


#Cerramos sesión e iniciamos sesión con user1 en vda1-----------------------------------------------------
LogouT
loGin -usr=user1 -pwd=user1 -id=vda1
#Crearemos la ruta y las padres que no existan
mkdir -path="/home/archivos/mia/fase2" -p
mkdir -path="/home/archivos/mia/carpeta2" -p
mkdir -path="/home/archivos/mia/carpeta3/carpeta7/carpeta8/carpeta9/carpeta10/carpeta11" -p
mkdir -path="/home/archivos/mia/carpeta4/carpeta7/carpeta8/carpeta9/carpeta10/carpeta11/carpeta7/carpeta8/carpeta9/carpeta10/carpeta11" -p
mkdir -path="/home/archivos/mia/carpeta2/a1"
mkdir -path="/home/archivos/mia/carpeta2/a2"
mkdir -path="/home/archivos/mia/carpeta2/a3"
mkdir -path="/home/archivos/mia/carpeta2/a4"
mkdir -path="/home/archivos/mia/carpeta2/a5"
mkdir -path="/home/archivos/mia/carpeta2/a6"
mkdir -path="/home/archivos/mia/carpeta2/a7"
mkdir -path="/home/archivos/mia/carpeta2/a8"
mkdir -path="/home/archivos/mia/carpeta2/a9"
mkdir -path="/home/archivos/mia/carpeta2/a10"
mkdir -path="/home/archivos/mia/fase2/a1"
mkdir -path="/home/archivos/mia/fase2/a2"
mkdir -path="/home/archivos/mia/fase2/a3"
mkdir -path="/home/archivos/mia/fase2/a4"
mkdir -path="/home/archivos/mia/fase2/a5"
mkdir -path="/home/archivos/mia/fase2/a6"
mkdir -path="/home/archivos/mia/fase2/a7"
mkdir -path="/home/archivos/mia/fase2/a8"
mkdir -path="/home/archivos/mia/fase2/a9"
mkdir -path="/home/archivos/mia/fase2/a10"
mkdir -path="/home/archivos/mia/fase2/a11"
mkdir -path="/home/archivos/mia/fase2/a12"
mkdir -path="/home/archivos/mia/fase2/a13"
mkdir -path="/home/archivos/mia/fase2/a14"
mkdir -path="/home/archivos/mia/fase2/a15"
mkdir -path="/home/archivos/mia/fase2/a16"
mkdir -path="/home/archivos/mia/fase2/a17"
mkdir -path="/home/archivos/mia/fase2/a18"
mkdir -path="/home/archivos/mia/fase2/a19"
mkdir -path="/home/archivos/mia/fase2/a20"
mkdir -path="/home/archivos/mia/fase2/a21"
mkdir -path="/home/archivos/mia/fase2/a22"
mkdir -path="/home/archivos/mia/fase2/a23"
mkdir -path="/home/archivos/mia/fase2/a24"
mkdir -path="/home/archivos/mia/fase2/a25"
mkdir -path="/home/archivos/mia/fase2/a26"
mkdir -path="/home/archivos/mia/fase2/a27"
mkdir -path="/home/archivos/mia/fase2/a28"
mkdir -path="/home/archivos/mia/fase2/a29"
mkdir -path="/home/archivos/mia/fase2/a30"
mkdir -path="/home/archivos/mia/fase2/a31"
mkdir -path="/home/archivos/mia/fase2/a32"
mkdir -path="/home/archivos/mia/fase2/a33"
mkdir -path="/home/archivos/mia/fase2/a34"
mkdir -path="/home/archivos/mia/fase2/a35"
mkdir -path="/home/archivos/mia/fase2/a36"
mkdir -path="/home/archivos/mia/fase2/a37"
mkdir -path="/home/archivos/mia/fase2/a38"
mkdir -path="/home/archivos/mia/fase2/a39"
mkdir -path="/home/archivos/mia/fase2/a40"
mkdir -path="/home/archivos/mia/fase2/a41"
mkdir -path="/home/archivos/mia/fase2/a42"
mkdir -path="/home/archivos/mia/fase2/a43"
mkdir -path="/home/archivos/mia/fase2/a44"
mkdir -path="/home/archivos/mia/fase2/a45"
mkdir -path="/home/archivos/mia/fase2/a46"
mkdir -path="/home/archivos/mia/fase2/a47"
mkdir -path="/home/archivos/mia/fase2/a48"

#Primer indirecto
mkdir -path="/home/archivos/mia/fase2/b1"
mkdir -path="/home/archivos/mia/fase2/b2"
mkdir -path="/home/archivos/mia/fase2/b3"
mkdir -path="/home/archivos/mia/fase2/b4"
mkdir -path="/home/archivos/mia/fase2/b5"
mkdir -path="/home/archivos/mia/fase2/b6"
mkdir -path="/home/archivos/mia/fase2/b7"
mkdir -path="/home/archivos/mia/fase2/b8"
mkdir -path="/home/archivos/mia/fase2/b9"
mkdir -path="/home/archivos/mia/fase2/b10"
mkdir -path="/home/archivos/mia/fase2/b11"
mkdir -path="/home/archivos/mia/fase2/b12"
mkdir -path="/home/archivos/mia/fase2/b13"
mkdir -path="/home/archivos/mia/fase2/b14"
mkdir -path="/home/archivos/mia/fase2/b15"
mkdir -path="/home/archivos/mia/fase2/b16"
mkdir -path="/home/archivos/mia/fase2/b17"
mkdir -path="/home/archivos/mia/fase2/b18"
mkdir -path="/home/archivos/mia/fase2/b19"
mkdir -path="/home/archivos/mia/fase2/b20"
mkdir -path="/home/archivos/mia/fase2/b21"
mkdir -path="/home/archivos/mia/fase2/b22"
mkdir -path="/home/archivos/mia/fase2/b23"
mkdir -path="/home/archivos/mia/fase2/b24"
mkdir -path="/home/archivos/mia/fase2/b25"
mkdir -path="/home/archivos/mia/fase2/b26"
mkdir -path="/home/archivos/mia/fase2/b27"
mkdir -path="/home/archivos/mia/fase2/b28"
mkdir -path="/home/archivos/mia/fase2/b29"
mkdir -path="/home/archivos/mia/fase2/b30"
mkdir -path="/home/archivos/mia/fase2/b31"
mkdir -path="/home/archivos/mia/fase2/b32"
mkdir -path="/home/archivos/mia/fase2/b33"
mkdir -path="/home/archivos/mia/fase2/b34"
mkdir -path="/home/archivos/mia/fase2/b35"
mkdir -path="/home/archivos/mia/fase2/b36"
mkdir -path="/home/archivos/mia/fase2/b37"
mkdir -path="/home/archivos/mia/fase2/b38"
mkdir -path="/home/archivos/mia/fase2/b39"
mkdir -path="/home/archivos/mia/fase2/b40"
mkdir -path="/home/archivos/mia/fase2/b41"
mkdir -path="/home/archivos/mia/fase2/b42"
mkdir -path="/home/archivos/mia/fase2/b43"
mkdir -path="/home/archivos/mia/fase2/b44"
mkdir -path="/home/archivos/mia/fase2/b45"
mkdir -path="/home/archivos/mia/fase2/b46"
mkdir -path="/home/archivos/mia/fase2/b47"
mkdir -path="/home/archivos/mia/fase2/b48"
mkdir -path="/home/archivos/mia/fase2/b49"
mkdir -path="/home/archivos/mia/fase2/b50"
mkdir -path="/home/archivos/mia/fase2/b51"
mkdir -path="/home/archivos/mia/fase2/b52"
mkdir -path="/home/archivos/mia/fase2/b53"
mkdir -path="/home/archivos/mia/fase2/b54"
mkdir -path="/home/archivos/mia/fase2/b55"
mkdir -path="/home/archivos/mia/fase2/b56"
mkdir -path="/home/archivos/mia/fase2/b57"
mkdir -path="/home/archivos/mia/fase2/b58"
mkdir -path="/home/archivos/mia/fase2/b59"
mkdir -path="/home/archivos/mia/fase2/b60"
mkdir -path="/home/archivos/mia/fase2/b61"
mkdir -path="/home/archivos/mia/fase2/b62"
mkdir -path="/home/archivos/mia/fase2/b63"
mkdir -path="/home/archivos/mia/fase2/b64"

#Segundo indirecto
mkdir -path="/home/archivos/mia/fase2/c1"
mkdir -path="/home/archivos/mia/fase2/c2"
mkdir -path="/home/archivos/mia/fase2/c3"
mkdir -path="/home/archivos/mia/fase2/c4"
mkdir -path="/home/archivos/mia/fase2/c5"
mkdir -path="/home/archivos/mia/fase2/c6"
mkdir -path="/home/archivos/mia/fase2/c7"
mkdir -path="/home/archivos/mia/fase2/c8"
mkdir -path="/home/archivos/mia/fase2/c9"
mkdir -path="/home/archivos/mia/fase2/c10"
mkdir -path="/home/archivos/mia/fase2/c11"
mkdir -path="/home/archivos/mia/fase2/c12"
mkdir -path="/home/archivos/mia/fase2/c13"
mkdir -path="/home/archivos/mia/fase2/c14"
mkdir -path="/home/archivos/mia/fase2/c15"
mkdir -path="/home/archivos/mia/fase2/c16"

#Validar que no cree dos veces el mismo directorio
mkdir -path="/home/archivos/mia/fase2/c16"
mkdir -path="/home/archivos/mia/fase2/a10"
mkdir -path="/home/archivos/mia/fase2/a12"
mkdir -path="/home/archivos/mia/fase2/b60"

#---Danto permisos recursivamente a todo lo q esta dentro
chmod -path=/home/archivos/mia/fase2/ -R -ugo=777
chown -path=/home/archivos/mia/fase2/ -R -usr=user1

rep -name=bm_inode -Path="/home/archivos/reportes/bm_inode01.txt" -id=vda1
rep -name=bm_block -Path="/home/archivos/reportes/bm_block01.txt" -id=vda1
rep -name=sb -patH="/home/archivos/reportes/sb01.jpg" -id=vda1
#rep -name=Journaling -patH="/home/archivos/reportes/bitacora01.txt" -id=vda1
rep -name=tree -Path="/home/archivos/reportes/tree03.jpg" -id=vda1
#uso de bloques e inodos

pause

#Cerramos sesion
Logout

loGin -usr=user1 -pwd=user1 -id=vda1

#Eliminación de carpetas 
#Error dado que no es usuario ROOT-------------------------------
rem -path="/home/archivos/mia/carpeta2/a1"

#Eliminar 
Rem -path="/home/archivos/mia/fase2/a1"
rem -path="/home/archivos/mia/fase2/b2"
#generamos el reporte tree para ver el uso de apuntadores----------------()
rep -name=tree -Path="/home/archivos/reportes/treeADIREC1.jpg" -id=vda1

pause

Logout

loGin -usr=root -pwd=123 -id=vda1
#Eliminación de carpetas 
#ELIMINACION CORRECTA RECURSIVA DADO QUE ES usuario ROOT
Rem -path="/home/archivos/mia/carpeta2/a1"
#generamos el reporte tree para ver el uso de apuntadores
rep -name=tree -Path="/home/archivos/reportes/treeADIREC2.jpg" -id=vda1
pause
lOGOUT

#--------------------------------------------------------------------------------Si esta malo arriba no hay permisos


loGin -usr=user1 -pwd=user1 -id=vda1

mkFile -SIZE=100 -PatH="/home/archivos/mia/fase2/archivito/a.txt" -p
mkfile -path="/home/archivos/mia/fase2/archivito/b.txt" -SIZE=100
#DEBE CREAR EL archivo arch.txt en su compu o concatenar el contenido del cont como sea esta bien -----
mkfile -path=/home/archivos/mia/fase2/archivito/c.txt -p -cont="/home/arch.txt" 

Cat -file=/home/archivos/mia/fase2/archivito/a.txt
Cat -file=/home/archivos/mia/fase2/archivito/b.txt
Cat -file=/home/archivos/mia/fase2/archivito/c.txt

pause

#elimina arvhivo b.txt
Rem -path="/home/archivos/mia/fase2/archivito/b.txt"
Cat -file=/home/archivos/mia/fase2/archivito/a.txt
Cat -file=/home/archivos/mia/fase2/archivito/b.txt
Cat -file=/home/archivos/mia/fase2/archivito/c.txt

pause


#EDITAR ARCHIVO fase1.txt
edit -path="/home/archivos/mia/fase2/archivito/a.txt" -cont ="Hola mundo :D Que buena la vida despues de ganar archivos :D"
edit -path="/home/archivos/mia/fase2/archivito/c.txt" -cont ="agregue Contenido a c.txt"

#Error ruta no existente
Edit -PatH="/home/archivos/mia/fish/prueba.txt" -cont="error archivo no existe"

#Editar nombre del archivo c.txt
ren -PatH=/home/archivos/mia/fase2/archivito/c.txt -name=califica.txt
rep -name=tree -Path="/home/archivos/reportes/treeEdit.jpg" -id=vda1

pause

#CP
#copiar carpeta a3 adentro de a4
cp -Path="/home/archivos/mia/fase2/a3" -dest="/home/archivos/mia/fase2/a4"

#copiar carpeta archivito adentro de a4
cp -Path="/home/archivos/mia/fase2/archivito" -dest="/home/archivos/mia/fase2/a4"

#copia el archivo a.txt en la carpeta a4
cp -Path="/home/mia/fase2/archivito/a.txt" -dest="/home/archivos/mia/fase2/a4"

rep -name=tree -Path="/home/archivos/reportes/treeCopy1.jpg" -id=vda1

pause

#MV
mv -Path="/home/archivos/mia/fase2/a4/a.txt" -dest="/home/archivos/mia/fase2"
#error de mover por que no es root
mv -Path="/home/archivos/mia/fase2/archivito" -dest="/"
rep -name=tree -Path="/home/archivos/reportes/treeMove1.png" -id=vda1

pause


lOGout
loGin -usr=root -pwd=123 -id=vda1
#FIND
find -Path="/home/archivos/mia/fase2" -name=califica.txt

pause


Logout

#Cerramos sesion e iniciamos como Root
loGin -usr=root -pwd=123 -id=vda1

#rep -name=file -Path="/home/archivos/reportes/fileusu1.txt" -id=vda1 -ruta="/usr.txt"
#PERMISOS
#Mkusr -id=vda1 -name="user1" -grp=MySql -pwd=user1
#Mkusr -id=vda1 -name="user2" -grp=Oracle -pwd=user2
#Cambia el grupo del user2
chgrp -usr=user2 -grp=MySql
#Cambia el grupo del user1
chgrp -usr=user1 -grp=Oracle
rep -name=file -Path="/home/archivos/reportes/fileChgrp.txt" -id=vda1 -ruta="/users.txt"


#REPORTE JOUTNALING
rep -name=Journaling -patH="/home/archivos/reportes/bitacora02.txt" -id=vda1


#FALLO EN EL SISTEMA Y RECUPERACION DE SISTEMA DE ARCHIVOS
#Simulando la perdida del sistema de archivos EXT3 de la partición1
Loss -id=vda1

rep -name=tree -Path="/home/archivos/reportes/treeLos.jpg" -id=vda1

pause
#uso de bloques e inodos
#Simular la recuperación del sistema mediante comando
Recovery -id=vda1


rep -name=tree -Path="/home/archivos/reportes/treeRecovery.jpg" -id=vda1
#uso de bloques e inodos
pause

logouT











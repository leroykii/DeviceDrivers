#!/bin/bash
#carga_modulo
driver=first
modulo=gradiant
dispositivo=/dev/bares
permisos=666 # de la bestia

#insertamos el módulo y averiguamos su número mayor
/sbin/insmod ${driver}.ko
mayor=`cat /proc/devices | grep ${modulo} | cut -d' ' -f1`

#Borramos los dispositivos mi_cinta? de anteriores pruebas y
# creamos los nuevos con el número mayor correcto
# y los permisos que queramos
rm -f ${dispositivo}0
rm -f ${dispositivo}1
mknod ${dispositivo}0 c ${mayor} 0
mknod ${dispositivo}1 c ${mayor} 1
chmod ${permisos} ${dispositivo}?
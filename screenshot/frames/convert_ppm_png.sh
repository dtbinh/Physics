#!/bin/bash
#Conversor entre formatos de imagem - Vinícius Evandro Gregório

# salva o diretorio atual
currentdir=`pwd`

# recuperar todos os arquivos *.png
for original in *.ppm; do

   #substitui as extensões
   nome="${original/.ppm/.png}"

   echo $nome

   # converte o .png em .ico
   convert $original $nome

done

# mover para o diretório da nova extensão
dirtemp=$currentdir/png

# se o diretorio não existir, entao criar o diretorio
if [ ! -e $dirtemp ];
  then
    mkdir $dirtemp
fi

mv *.png $dirtemp

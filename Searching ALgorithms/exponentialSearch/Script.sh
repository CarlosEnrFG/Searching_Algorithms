#!/bin/bash

gcc tiempo.c -c
gcc exponentialSearch.c tiempo.o -o Exponential

echo "Busqueda expoencial comenzzando"
for N in 1000000 2000000 3000000 4000000 5000000 6000000 7000000 8000000 9000000 10000000
do
  for X in 322486 14700764 3128036 6337399 61396 10393545 2147445644 1295390003 450057883 187645041 1980098116 152503 5000 1493283650 214826 1843349527 1360839354 2109248666 2147470852 0
  do
	  echo "Exponencial $N buscando $X"
    ./Exponential $N $X < numeros10millones.txt >> Exponential.txt
    echo "Terminando"
  done
done
echo "Busqueda exponencial terminada"

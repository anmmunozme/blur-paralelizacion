#!/bin/bash


#compilar blur-effect.cpp
echo 'compilando...'
g++ blur-seq.cpp -o blur-seq `pkg-config opencv --libs` -fpermissive

#run exec
echo 'Ejecutando...'
echo "resultados:">resultados.txt

echo 'Imagen 720px'
echo '* Imagen 720px:'>>resultados.txt
for((kernel = 3; kernel <= 15; kernel += 2))
do
	echo "kernel = $kernel:"
	echo "kernel = $kernel: ">>resultados.txt


	(time ./blur-seq imagen720.png $kernel)>>resultados.txt 2>&1
		echo "">>resultados.txt
done

echo 'Imagen 1080px'
echo '* Imagen 1080px:'>>resultados.txt
for((kernel = 3; kernel <= 15; kernel += 2))
do
	echo "kernel = $kernel:"
	echo "kernel = $kernel: ">>resultados.txt

	(time ./blur-seq imagen1080.jpg $kernel)>>resultados.txt 2>&1
		echo "">>resultados.txt
done

echo 'Imagen 4k'
echo '* Imagen 4k:'>>resultados.txt
for((kernel = 3; kernel <= 15; kernel += 2))
do
	echo "kernel = $kernel:"
	echo "kernel = $kernel: ">>resultados.txt

	(time ./blur-seq imagen4k.jpg $kernel)>>resultados.txt 2>&1
		echo "">>resultados.txt
done


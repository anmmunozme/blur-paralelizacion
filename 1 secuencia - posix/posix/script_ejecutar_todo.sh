#!/bin/bash


#compilar blur-effect.cpp
echo 'compilando...'
g++ blur-effect.cpp -o blur-effect `pkg-config opencv --libs` -fpermissive -pthread

echo 'Ejecutando...'
echo "resultados:">resultados.txt

echo 'Imagen 720px'
echo '* Imagen 720px:'>>resultados.txt
for((kernel = 3; kernel <= 17; kernel += 2))
do
	echo "kernel = $kernel:"
	echo "kernel = $kernel: ">>resultados.txt

	for((thread = 1; thread <= 16; thread *= 2)) 
	do
		echo "threads = $thread"
		echo "threads = $thread">>resultados.txt

	(time ./blur-effect imagen720.png $kernel $thread)>>resultados.txt 2>&1
		echo "">>resultados.txt
	done
done

echo 'Imagen 1080px'
echo '* Imagen 1080px:'>>resultados.txt
for((kernel = 3; kernel <= 17; kernel += 2))
do
	echo "kernel = $kernel:"
	echo "kernel = $kernel: ">>resultados.txt

	for((thread = 1; thread <= 16; thread *= 2)) 
	do
		echo "threads = $thread"
		echo "threads = $thread">>resultados.txt

	(time ./blur-effect imagen1080.jpg $kernel $thread)>>resultados.txt 2>&1
		echo "">>resultados.txt
	done
done

echo 'Imagen 4k'
echo '* Imagen 4k:'>>resultados.txt
for((kernel = 3; kernel <= 17; kernel += 2))
do
	echo "kernel = $kernel:"
	echo "kernel = $kernel: ">>resultados.txt

	for((thread = 1; thread <= 16; thread *= 2)) 
	do
		echo "threads = $thread"
		echo "threads = $thread">>resultados.txt

	(time ./blur-effect imagen4k.jpg $kernel $thread)>>resultados.txt 2>&1
		echo "">>resultados.txt
	done
done


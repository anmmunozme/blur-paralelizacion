#!/bin/bash


#compilar blur-effect.cpp
echo 'compilando...'
nvcc blur-effect.cu -o blur-effect `pkg-config opencv --libs`

#run exec
echo 'Ejecutando...'
echo "resultados:">resultados.txt

echo 'Imagen 720px'
echo '* Imagen 720px:'>>resultados.txt
for((kernel = 3; kernel <= 15; kernel += 2))
do
	echo "kernel = $kernel:"
	echo "kernel = $kernel: ">>resultados.txt

	echo "threads = 1"
		echo "threads = 1">>resultados.txt

	(time ./blur-effect imagen720.png $kernel 1)>>resultados.txt 2>&1
		echo "">>resultados.txt

	for((thread = 48; thread <= 384; thread *= 2)) 
	do
		echo "threads = $thread"
		echo "threads = $thread">>resultados.txt

	(time ./blur-effect imagen720.png $kernel $thread)>>resultados.txt 2>&1
		echo "">>resultados.txt
	done
done

echo 'Imagen 1080px'
echo '* Imagen 1080px:'>>resultados.txt
for((kernel = 3; kernel <= 15; kernel += 2))
do
	echo "kernel = $kernel:"
	echo "kernel = $kernel: ">>resultados.txt

	echo "threads = 1"
		echo "threads = 1">>resultados.txt

	(time ./blur-effect imagen1080.jpg $kernel 1)>>resultados.txt 2>&1
		echo "">>resultados.txt

	for((thread = 48; thread <= 384; thread *= 2)) 
	do
		echo "threads = $thread"
		echo "threads = $thread">>resultados.txt

	(time ./blur-effect imagen1080.jpg $kernel $thread)>>resultados.txt 2>&1
		echo "">>resultados.txt
	done
done

echo 'Imagen 4k'
echo '* Imagen 4k:'>>resultados.txt
for((kernel = 3; kernel <= 15; kernel += 2))
do
	echo "kernel = $kernel:"
	echo "kernel = $kernel: ">>resultados.txt

	echo "threads = 1"
		echo "threads = 1">>resultados.txt

	(time ./blur-effect imagen4k.jpg $kernel 1)>>resultados.txt 2>&1
		echo "">>resultados.txt

	for((thread = 48; thread <= 384; thread *= 2)) 
	do
		echo "threads = $thread"
		echo "threads = $thread">>resultados.txt

	(time ./blur-effect imagen4k.jpg $kernel $thread)>>resultados.txt 2>&1
		echo "">>resultados.txt
	done
done


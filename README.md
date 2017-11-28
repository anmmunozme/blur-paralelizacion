# blur-paralelizacion

Introducción

En la siguiente se presenta la paralelización del algoritmo blur, mostrando un paralelo entre una implementación secuencial, una utilizando hilos POSIX, usando la herramienta OpenMP y por medio de tarjeta gráfica NVIDIA (Cuda).
De acuerdo a cada método usado se muestran sus resultados y explicación de la metodología que se usó. 

Prerequisitos

1. Se debe tener una máquina Ubuntu.

2.Para el correcto funcionamiento del algoritmo se requiere de la instalación de la librería OpenCV.

3. Para la implementación CUDA es necesario una tarjeta Nvidia.

Probando el algoritmo

Cada implementación fue probada con imágenes de 720p, 1080p y 4K, variandoel kernel del efecto borroso para ver el cambio en tiempo y como cada kernel afectaba de manera diferente a la imagen, a los hilos y tarjeta gráfica.
Se puedieron ver cambios significativos en el tiempo de procesamiento de las imágenes entre cada herramienta de paralelización, aunque el tipo de máquina usada puede mejorar o hacer peor el rendimiento.

Comandos para ejecutar cada uno de los modelos implementados

    -Secuencial: 
    
        g++ blur-seq.cpp -o blur-seq `pkg-config opencv --libs` -fpermissive
        
        ./blur-seq imagen3720.jpg 16
     
    - Posix:
    
        g++ blur-effect.cpp -o blur-effect `pkg-config opencv --libs` -fpermissive -pthread
        
        ./blur-effect nombre_imagen tamano_kernel numero_hilos
        
    - OpenMP:
    
        g++ -fopenmp blur-effect.cpp -o blur-effect `pkg-config opencv --libs` -fpermissive
        ./blur-effect nombre_imagen tamano_kernel numero_hilos
        
     -CUDA:
     
        nvcc blur-effect.cu -o blur-effect `pkg-config opencv --libs`
 
Cada modelo cuenta con un script para hacer la ejecución  con diferentes pruebas de kernel y número de hilos.
Para su ejecución, es necesario correr el siguiente script.

    sudo ./script_ejecutar_todo.sh
     
        
        
        

Construido con

    - OpenCV
    - Lenguaje de programación C++

Autores

    -   Ángela María Muñoz Medina
    -   Juan Esteban Caicedo Palacio

Agradecimientos

    -   César Pedraza - Profesor
    -   Departamento de Ingeniería de Sistemas 

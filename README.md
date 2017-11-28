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

Construido con
    - OpenCV
    - Lenguaje de programación C++

Autores
    -   Ángela María Muñoz Medina
    -   Juan Esteban Caicedo Palacio

Agradecimientos
    -   César Pedraza - Profesor
    -   Departamento de Ingeniería de Sistemas 

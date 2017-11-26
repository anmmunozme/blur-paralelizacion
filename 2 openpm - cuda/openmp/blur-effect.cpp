#include <stdio.h>
#include <stdlib.h>
#include <opencv2/opencv.hpp>
#include <time.h>
#include <omp.h>

#define MAX_THREAD 16

//g++ blur-effect.cpp -o blur-effect `pkg-config opencv --libs` -fpermissive -fopenmp
//./blur-effect imagen3720.jpg 3 3

//para ejecutar el script primera vez si no se tiene permiso: sudo chmod +x script_ejecutar_todo.sh
//sudo ./script_ejecutar_todo.sh

using namespace cv;

//Estructura para separar los 3 canales de color
struct pixel{
 uchar R, G, B;
};

Mat im;
pixel *image;

int N_THREADS;
int KERNEL_SIZE;

pthread_t threads[MAX_THREAD];

char add_c[3] = {'b','-',0};

//Calculo de efecto blur recorriendo la imagen y calculando el promedio dado por el kernel
void blur(int initIteration, int endIteration){

	int rows = im.rows;
	int cols = im.cols;
	int i,j, k, l;


    for(i = initIteration; i <= endIteration; i++){
        for(j = 0; j < cols; j++){

                int b = 0, r = 0, g = 0;
		int c = 0;
                for(k = i - KERNEL_SIZE; k <= i + KERNEL_SIZE; k++){
                    for(l = j - KERNEL_SIZE; l <= j + KERNEL_SIZE; l++){
							int kernel_row_pos = (k < 0 ? k + rows : k);
							int kernel_col_pos = (l < 0 ? l + cols : l);
            						b += image[kernel_row_pos*im.cols + kernel_col_pos].B;
            						g += image[kernel_row_pos*im.cols + kernel_col_pos].G;
            						r += image[kernel_row_pos*im.cols + kernel_col_pos].R;
            						c++;
                    }
                }
            	image[i*im.cols + j].B = (b/c);
            	image[i*im.cols + j].G = (g/c);
            	image[i*im.cols + j].R = (r/c);

        }
    }
}



int main(int argc, const char *argv[]){

	clock_t tStart = clock();

	if( argc < 3){
		printf("Usage: blur-effect <img_path> <kernel_size> <thread_number>\n");
		return -1;
	}

	im = imread(argv[1]);
	sscanf(argv[2],"%d", &KERNEL_SIZE);
	sscanf(argv[3],"%d", &N_THREADS);

	if(!im.data){
		printf("no image data\n");
		return -1;
	}

	image = (pixel*)malloc(sizeof(struct pixel)*im.rows*im.cols);


	for(int i = 0; i < im.rows; i++){
		for(int j = 0; j < im.cols; j++){
			image[i*im.cols + j].B = im.at<Vec3b>(i,j)[0];
			image[i*im.cols + j].G = im.at<Vec3b>(i,j)[1];
			image[i*im.cols + j].R = im.at<Vec3b>(i,j)[2];
		}
	}

  #pragma omp parallel num_threads(N_THREADS) //seccion de codigo que queremos paralelizar
    {
      //Calculo de particiones que se asignara a cada hilo
      int id = omp_get_thread_num();
      int nTh = omp_get_num_threads();
      int procFirst, procLast;

      for(int i = id; i < nTh; i += im.rows/N_THREADS){
        procFirst = (int)((im.rows/N_THREADS)*i);
        procLast = (i + 1) * (im.rows/N_THREADS) -1;
        #pragma omp critical
	blur(procFirst,procLast);
      }
    }

//cambiar los valores de la imagen por el promedio calculado

	for(int i = 0; i < im.rows; i++){
		for(int j = 0; j < im.cols; j++){
			im.at<Vec3b>(i,j)[0] = image[i*im.cols + j].B;
			im.at<Vec3b>(i,j)[1] = image[i*im.cols + j].G;
			im.at<Vec3b>(i,j)[2] = image[i*im.cols + j].R;
		}
	}

	free(image);

	char * name = malloc(sizeof(argv[1]) + sizeof(add_c) + 1);
	name[0] = 0;
	strcat(name,add_c);
	strcat(name,argv[1]);
	imwrite(name, im);

	printf("Time taken: %.4fs\n", (double)(clock() - tStart)/CLOCKS_PER_SEC);

	return 0;
}

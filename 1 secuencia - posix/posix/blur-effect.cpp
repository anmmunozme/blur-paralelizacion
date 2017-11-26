#include <stdio.h>
#include <opencv2/opencv.hpp>
#include <time.h>

#define MAX_THREAD 16

//g++ blur-effect.cpp -o blur-effect `pkg-config opencv --libs` -fpermissive -pthread
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

//Estructura para guardar los hilos creados con su punto de inicio y final
struct data{
	int is, ie;
}d[MAX_THREAD];

int N_THREADS;
int KERNEL_SIZE = 15;

pthread_t threads[MAX_THREAD];

char add_c[3] = {'b','-',0};

//Calculo de efecto blur recorriendo la imagen y calculando el promedio dado por el kernel
void *blur(void *args){

	struct data *dat;
	dat = args;
	int initIteration = dat->is;
	int endIteration = dat->ie;

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


//Calculo de particiones que se asignara a cada hilo
	for(int i = 0; i < N_THREADS; i++){
		d[i].is = (int)((im.rows/N_THREADS)*i);
		d[i].ie = d[i].is + (im.rows/N_THREADS) -1;
	}
//creacion y manejo de hilos
  	for(int i = 0; i <N_THREADS; i++){ 
  	  if( pthread_create(&threads[i], NULL, blur, (void *)&d[i]) != 0)
  	    perror("could not create thread\n");
  	}

  	for(int i = 0; i <N_THREADS; i++){ 
  	  if( pthread_join(threads[i], NULL) != 0)
  	    perror("could not create thread\n");
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

	printf("Time taken: %.2fs\n", (double)(clock() - tStart)/CLOCKS_PER_SEC);
    
	return 0;
}



































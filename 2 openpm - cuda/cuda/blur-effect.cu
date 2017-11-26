#include <stdio.h>
#include <stdlib.h>
#include <opencv2/opencv.hpp>
#include <time.h>
#include <cuda_runtime.h>
#define BLOCKSPERGRID 1


//nvcc blur-effect.cu -o blur-effect `pkg-config opencv --libs`

using namespace cv;

//Estructura para separar los 3 canales de color
struct pixel{
 uchar R, G, B;
};

pixel *image;


char add_c[3] = {'b','-',0};

//Calculo de efecto blur recorriendo la imagen y calculando el promedio dado por el kernel
__global__ void blur(pixel *image,int KERNEL_SIZE, int rows, int cols, int numThreads){

	int initIteration;
	int endIteration;
	int i,j, k, l, index;
	index = (blockDim.x * blockIdx.x) + threadIdx.x;
	initIteration = (rows/numThreads)*index;
	endIteration = initIteration + (rows/numThreads) - 1;

    for(i = initIteration; i <= endIteration; i++){
        for(j = 0; j < cols; j++){
                int b = 0, r = 0, g = 0,c = 0;
                for(k = i - KERNEL_SIZE; k <= i + KERNEL_SIZE; k++){
                    for(l = j - KERNEL_SIZE; l <= j + KERNEL_SIZE; l++){
            			b += image[k*cols + l].B;
            			g += image[k*cols + l].G;
            			r += image[k*cols + l].R;
            			c++;
                    }
                }
            	image[i*cols + j].B = (b/c);
            	image[i*cols + j].G = (g/c);
            	image[i*cols + j].R = (r/c);

        }
    }
}



int main(int argc, const char *argv[]){

	Mat im;
	int num_threads;
	int KERNEL_SIZE;
	clock_t tStart = clock();
	cudaError_t error = cudaSuccess;

	if( argc < 3){
		printf("Usage: <img_path> <kernel_size> <thread_number>\n");
		return -1;
	}


	sscanf(argv[2],"%d", &KERNEL_SIZE);
	sscanf(argv[3],"%d", &num_threads);
	im = imread(argv[1]);

	image = (pixel*)malloc(sizeof(struct pixel)*im.rows*im.cols);


	for(int i = 0; i < im.rows; i++){
		for(int j = 0; j < im.cols; j++){
			image[i*im.cols + j].B = im.at<Vec3b>(i,j)[0];
			image[i*im.cols + j].G = im.at<Vec3b>(i,j)[1];
			image[i*im.cols + j].R = im.at<Vec3b>(i,j)[2];
		}
	}
	pixel *gpu_im;
	error = cudaMalloc((void**)&gpu_im,sizeof(struct pixel)*im.rows*im.cols);

	if (error != cudaSuccess){
	        fprintf(stderr, "(error allocating in device code %s)!\n", cudaGetErrorString(error));
	        exit(EXIT_FAILURE);
	}

	error = cudaMemcpy(gpu_im,image,sizeof(struct pixel)*im.rows*im.cols,cudaMemcpyHostToDevice);
	int threadsblocks = (num_threads/BLOCKSPERGRID);


	blur<<<BLOCKSPERGRID,threadsblocks>>>(gpu_im, KERNEL_SIZE, num_threads, im.rows, im.cols);
	error = cudaMemcpy(image, gpu_im,sizeof(struct pixel)*im.rows*im.cols,cudaMemcpyDeviceToHost);


//cambiar los valores de la imagen por el promedio calculado

	for(int i = 0; i < im.rows; i++){
		for(int j = 0; j < im.cols; j++){
			im.at<Vec3b>(i,j)[0] = image[i*im.cols + j].B;
			im.at<Vec3b>(i,j)[1] = image[i*im.cols + j].G;
			im.at<Vec3b>(i,j)[2] = image[i*im.cols + j].R;
		}
	}

	error = cudaFree(gpu_im);

	char * name = (char*)malloc(sizeof(argv[1]) + sizeof(add_c) + 1);
	name[0] = 0;
	strcat(name,add_c);
	strcat(name,argv[1]);
	imwrite(name, im);

	free(image);

	printf("Time taken: %.4fs\n", (double)(clock() - tStart)/CLOCKS_PER_SEC);

	return 0;
}

#include <stdio.h>
#include <opencv2/opencv.hpp>

//g++ blur-seq.cpp -o blur-seq `pkg-config opencv --libs` -fpermissive
//./blur-seq imagen3720.jpg 16

using namespace cv;

struct pixel{
 uchar R, G, B;
};

Mat im;
pixel *image;

int KERNEL_SIZE = 15;

char add_c[3] = {'b','-',0};

void blur(){

	int rows = im.rows;
	int cols = im.cols;
	int i,j, k, l;


    for(i = 0; i < rows; i++){
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
	if( argc < 2){
		printf("Usage: blur-seq <img_path> <kernel_size>\n");
		return -1;
	}

	im = imread(argv[1]);
	sscanf(argv[2],"%d", &KERNEL_SIZE);

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

  blur();

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

	return 0;
}

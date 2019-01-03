#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include <math.h>

void convolution(int tamImagen, double * imagen, int tamFiltro,double *filtro, int stride, int padding);
int aplicarFiltro(int i, int j, int tamImagen, double * imagen, int tamFiltro,double *filtro, int padding);
void rellenarCanal(double* canal, int tamImagen, int padding);
void rellenarFiltro(double* filtro, int tamFiltro);
//Argumento tamaño imagen, tamaño del filtro, stride(el tamaño del step), imagen de un canal solo.
int main(int argc, char *argv[]){
	int stride = 1;
	int tamFiltro = 7;
	int padding = 1;
	int tamImagen = 4000;

	if(argc > 4){
		stride = atoi(argv[1]);
		tamFiltro = atoi(argv[2]);
		padding = atoi(argv[3]);
		tamImagen = atoi(argv[4]);
	}

	printf("Ejecutando con parámetros(stride:%i,tamaño filtro:%i,padding:%i,tamaño imagen:%i)!\n",stride,tamFiltro,padding,tamImagen);


	srand(time(NULL));
	//Imagen formada por tres canales
	double * canalRojo = malloc (pow(tamImagen+padding*2,2)*sizeof(double));
	double * canalAzul = malloc (pow(tamImagen+padding*2,2)*sizeof(double));
	double * canalVerde = malloc (pow(tamImagen+padding*2,2)*sizeof(double));
	//Rellenar filtro
	double * filtro = malloc (pow(tamFiltro,2)*sizeof(double));
	rellenarFiltro(filtro, tamFiltro);
	//Rellenar canales
	rellenarCanal(canalRojo, tamImagen, padding);
	rellenarCanal(canalAzul, tamImagen, padding);
	rellenarCanal(canalVerde, tamImagen, padding);
	double **canales = malloc (3*sizeof(double));
	canales[0] = canalRojo;
	canales[1] = canalAzul;
	canales[2] = canalVerde;
	clock_t start = clock();
	for(int i = 0; i < 3; i++){
		convolution(tamImagen,canales[i], tamFiltro, filtro, stride, padding);
	}
	clock_t end = clock();
	float seconds = (float) (end-start) / CLOCKS_PER_SEC;
	printf("Tiempo de ejecución: %fs.\n", seconds);
	return 0;
}

//Aplicar convolución
void convolution(int tamImagen, double* imagen, int tamFiltro, double *filtro, int stride, int padding){

	double * res = (double *) malloc (pow(tamImagen+2*padding,2)*sizeof(double));
	//Recorrer imagen y aplicar filtro. Resultado tiene tam idéntico o menor
	for (int i = 0; i < tamImagen + 2*padding ; i+= stride)
	{
		for (int j = 0; j < tamImagen + 2*padding; j+= stride)
		{

			res[tamImagen*i + j]= aplicarFiltro(i,j,tamImagen, imagen, tamFiltro, filtro, padding);
		}
	}
}
//Aplicar filtro en una determinada casilla
int aplicarFiltro(int posX, int posY, int tamImagen, double * imagen, int tamFiltro, double *filtro,int padding){

	double combinacionLineal = 0.0;
	int comienzo = - (tamFiltro - 1)/2;
	int fin = -comienzo;
	int i = 0, j = 0;
	for(int k = comienzo; k < fin; k++)
	{
		for(int t = comienzo; t < fin; t++)
		{
			int posi = posX + k;
			int posj = posY + t;
			if(posj >= 0 && posi >= 0 && posi < (tamImagen + 2*padding) && posj < (tamImagen + 2*padding)){
				combinacionLineal += imagen[2*padding*posi + posj]* (*(filtro + tamFiltro*i + j));
			}
			j++;
		}
		i++;
	}

	return combinacionLineal;
}
//Valores aleatorios continuos entre 0 y 100
void rellenarFiltro(double* filtro, int tamFiltro){
	for (int i = 0; i < tamFiltro ; i++)
	{
		for (int j = 0; j < tamFiltro; j++)
		{
			filtro[i*tamFiltro + j] = rand() % 100;
		}
	}
}
//Valores aleatorios del canal de imagen de 0 a 255
void rellenarCanal(double* canal, int tamImagen, int padding){
	for (int i = 0; i < tamImagen + 2*padding ; i++)
	{
		for (int j = 0; j < tamImagen + 2*padding; j++)
		{
			if(padding){
				if(i < padding || j < padding){
					canal[i*(tamImagen+2*padding) + j] = 0;
				}
				else{
					canal[i*(tamImagen+2*padding) + j] = rand() % 256;

				}
			}
			else{
				canal[i*(tamImagen+2*padding) + j] = rand() % 256;
			}

		}
	}
}

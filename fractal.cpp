#include <stdlib.h>
#include <stdio.h>


// Configuracion del fractal

#define TAMANO_IMAGEN 1024
#define TAMANO_TABLA_COLORES 1800

#define MAXIMO_ITERACIONES 50

// Sirve para definir el cuadrado a dibujar (y hacer zoom)
#define CENTRO_X 0
#define CENTRO_Y 0
#define LADO_CUADRADO 4



// Escribe la matriz de colores a una imagen.
// Utiliza el formato .ppm que es mas sencillo de escribir que BPM, JPG, etc
void escribirMatrizAImagen(int** matriz){

	FILE* fp;
	fp = fopen("resultado.ppm", "w");

	// Escribir las cabeceras y las dimensiones
	fprintf(fp, "P6\n%d %d\n255\n", TAMANO_IMAGEN, TAMANO_IMAGEN);

	// Escribir cada color
	for(int i=0; i<TAMANO_IMAGEN; i++){
		for(int j=0; j<TAMANO_IMAGEN; j++){
			fwrite(&matriz[i][j], 3, 1, fp);
		}
	}

	fclose(fp);
}

// Obtiene el numero de iteraciones que se necesitan para que converja
// (y asi obtener el color).
int numeroIteraciones(
	float z_real, 
	float z_imaginario, 
	int iteraciones, 
	float c_real, 
	float c_imaginario){

	// Si supera el numero de iteraciones permitidas
	if(iteraciones == MAXIMO_ITERACIONES) return -1;

	iteraciones++;

	// Si converge
	if(abs((z_real*z_real) + (z_imaginario*z_imaginario)) >= 4)
		return iteraciones;

	// Crear el nuevo complejo para pasarlo nuevamente a la funcion.
	// El complejo "nuevo" es = Z^2 + C
	float nuevo_real = (z_real*z_real) - (z_imaginario*z_imaginario) + c_real;
	float nuevo_imaginario = (2 * z_real * z_imaginario) + c_imaginario;

	// Llamada recursiva
	return numeroIteraciones(nuevo_real, nuevo_imaginario, iteraciones, c_real, c_imaginario);
}





int main(){


	// Obtener las coordenadas donde dibujar el dibujo (un cuadrado)
	float min_x = CENTRO_X - (LADO_CUADRADO * 0.5);
	float min_y = CENTRO_Y - (LADO_CUADRADO * 0.5);
	float max_x = CENTRO_X + (LADO_CUADRADO * 0.5);
	float max_y = CENTRO_Y + (LADO_CUADRADO * 0.5);

	// Crea la matriz para guardar la imagen
	int** matriz = new int*[TAMANO_IMAGEN];
	for (int i = 0; i < TAMANO_IMAGEN; i++ ){
		matriz[i] = new int[TAMANO_IMAGEN];
	}


	// Crear la tabla de colores
	int* tabla_colores = new int[TAMANO_TABLA_COLORES];

	// Llenar la tabla de colores de forma arbitraria
	// (dividiendo todo el espectro desde 0x000000 hasta 0xffffff de forma
	// equitativa por la cantidad de elementos en la tabla de colores)
	float parte = 0xFFFFFF/TAMANO_TABLA_COLORES;

	for(int i=0; i<TAMANO_TABLA_COLORES; i++){		
		tabla_colores[i] = parte * i;
	}

	// Recorrer todos los pixeles de la imagen
	for(int y=0; y<TAMANO_IMAGEN; y++){

		for(int x=0; x<TAMANO_IMAGEN; x++){

			// Obtener el complejo asociado al pixel
			float c_real			= min_x+(((max_x-min_x)/TAMANO_IMAGEN)*x);
			float c_imaginario 	= min_y+(((max_y-min_y)/TAMANO_IMAGEN)*y);

			// Obtener numero de iteraciones para este complejo
			int numero_iteraciones = numeroIteraciones(0, 0, 0, c_real, c_imaginario);

			// Pintar el pixel
			matriz[y][x] = tabla_colores[numero_iteraciones % TAMANO_TABLA_COLORES];

		}
	}




	escribirMatrizAImagen(matriz);

	return 0;
}
/** Universidad Simón Bolívar
*   Departamento de Computación y Tecnología de la Información
*   CI5437 - Inteligencia Artificial I
*   Trimestre Sept-Dic 2016
*
*   Proyecto III SAT
*   Programa Principal. Genera archivos .cnf para cada instancia y las resuelve utilizando el SAT solver Minisat
* 
*   Hecho por: Luis Carlos Díaz 11-10293
*              Gabriel Iglesias 11-10476
*              Fernando Saraiva 09-19794
*            
*/

#include <fstream>
#include <iostream>
#include <cstdlib>
#include <string.h>

using namespace std;

void cierraLazo(int filas, int columnas, FILE *output) {
 
    //ESQUINAS
    int horizontales[4];
    horizontales[0] = 1;
    horizontales[1] = columnas;
    horizontales[2] = filas*columnas + 1;
    horizontales[3] = filas*columnas + columnas;
 
    int verticales[4];
    verticales[0] = (filas+1)*columnas + 1;
    verticales[1] = (filas+1)*columnas + columnas + 1;
    verticales[2] = 2*filas*columnas + filas;
    verticales[3] = 2*filas*columnas + filas + columnas;
 
    //SEGMENTOS HORIZONTALES
   
    for (int i = 0; i < 4; i++) {
        fprintf(output, "%d %d 0\n", -horizontales[i], verticales[i]);
        if (i % 2 == 0) {
            fprintf(output, "%d %d %d 0\n", -horizontales[i], horizontales[i]+1, verticales[i]+1);
            fprintf(output, "%d %d %d 0\n", -horizontales[i], -(horizontales[i]+1), -(verticales[i]+1));           
        }
        else {
            fprintf(output, "%d %d %d 0\n", -horizontales[i], horizontales[i]-1, verticales[i]-1);
            fprintf(output, "%d %d %d 0\n", -horizontales[i], -(horizontales[i]-1), -(verticales[i]-1));
        }
    }
 
    //SEGMENTOS VERTICALES
    for (int i = 0; i < 4; i++) {
        fprintf(output, "%d %d 0\n", -verticales[i], horizontales[i]);
        if (i < 2) {
            fprintf(output, "%d %d %d 0\n", -verticales[i], verticales[i]+columnas+1, horizontales[i]+columnas);
            fprintf(output, "%d %d %d 0\n", -verticales[i], -(verticales[i]+columnas+1), -(horizontales[i]+columnas));         
        }
        else {
            fprintf(output, "%d %d %d 0\n", -verticales[i], verticales[i]-columnas-1, horizontales[i]-columnas);
            fprintf(output, "%d %d %d 0\n", -verticales[i], -(verticales[i]-columnas-1), -(horizontales[i]-columnas));
        }
    }
 
    //LADOS
    //SEGMENTOS HORIZONTALES SUPERIORES
    for (int i = 1; i < columnas - 1; i++) {
        int seg = i+1;
        int vertical = (filas+1)*columnas + seg;
        fprintf(output, "%d %d %d %d 0\n", -seg, seg-1, seg+1, vertical);
        fprintf(output, "%d %d %d %d 0\n", -seg, -(seg-1), -(seg+1), -vertical);
        fprintf(output, "%d %d %d %d 0\n", -seg, seg-1, vertical+1, vertical);
        fprintf(output, "%d %d %d %d 0\n", -seg, -(seg-1), -(vertical+1), -vertical);
        fprintf(output, "%d %d %d %d 0\n", -seg, seg-1, seg+1, vertical+1);
        fprintf(output, "%d %d %d %d 0\n", -seg, -(seg-1), -(seg+1), -(vertical+1));
        fprintf(output, "%d %d %d %d 0\n", -seg, vertical+1, seg+1, vertical);
        fprintf(output, "%d %d %d %d 0\n", -seg, -(vertical+1), -(seg+1), -vertical);
    }
 
    //SEGMENTOS HORIZONTALES INFERIORES
    for (int i = 1; i < columnas - 1; i++) {
        int seg = filas*columnas+i+1;
        int vertical = (filas+1)*columnas + seg - 1;
        fprintf(output, "%d %d %d %d 0\n", -seg, seg-1, seg+1, vertical);
        fprintf(output, "%d %d %d %d 0\n", -seg, -(seg-1), -(seg+1), -vertical);
        fprintf(output, "%d %d %d %d 0\n", -seg, seg-1, vertical+1, vertical);
        fprintf(output, "%d %d %d %d 0\n", -seg, -(seg-1), -(vertical+1), -vertical);
        fprintf(output, "%d %d %d %d 0\n", -seg, seg-1, seg+1, vertical+1);
        fprintf(output, "%d %d %d %d 0\n", -seg, -(seg-1), -(seg+1), -(vertical+1));
        fprintf(output, "%d %d %d %d 0\n", -seg, vertical+1, seg+1, vertical);
        fprintf(output, "%d %d %d %d 0\n", -seg, -(vertical+1), -(seg+1), -vertical);
    }
 
    //SEGMENTOS VERTICALES IZQUIERDA
    for (int i = 1; i < filas - 1; i ++) {
        int seg = (filas+1)*columnas + 1 + i*(columnas+1);
        int horizontal1 = 1+(i*columnas);
        int horizontal2 = 1+(1+i)*columnas;
        fprintf(output, "%d %d %d %d 0\n", -seg, seg-columnas-1, seg+columnas+1, horizontal1);
        fprintf(output, "%d %d %d %d 0\n", -seg, -(seg-columnas-1), -(seg+columnas+1), -horizontal1);
        fprintf(output, "%d %d %d %d 0\n", -seg, seg-columnas-1, seg+columnas+1, horizontal2);
        fprintf(output, "%d %d %d %d 0\n", -seg, -(seg-columnas-1), -(seg+columnas+1), -horizontal2);
        fprintf(output, "%d %d %d %d 0\n", -seg, seg-columnas-1, horizontal2, horizontal1);
        fprintf(output, "%d %d %d %d 0\n", -seg, -(seg-columnas-1), -horizontal2, -horizontal1);
        fprintf(output, "%d %d %d %d 0\n", -seg, horizontal2, seg+columnas+1, horizontal1);
        fprintf(output, "%d %d %d %d 0\n", -seg, -horizontal2, -(seg+columnas+1), -horizontal1);
    }
   
    //SEGMENTOS VERTICALES DERECHA
    for (int i = 1; i < filas - 1; i ++) {
        int seg = (filas+1)*columnas + 1 + i*(columnas+1) + columnas;
        int horizontal1 = 1+(i*columnas)+columnas-1;
        int horizontal2 = 1+(1+i)*columnas+columnas-1;
        fprintf(output, "%d %d %d %d 0\n", -seg, seg-columnas-1, seg+columnas+1, horizontal1);
        fprintf(output, "%d %d %d %d 0\n", -seg, -(seg-columnas-1), -(seg+columnas+1), -horizontal1);
        fprintf(output, "%d %d %d %d 0\n", -seg, seg-columnas-1, seg+columnas+1, horizontal2);
        fprintf(output, "%d %d %d %d 0\n", -seg, -(seg-columnas-1), -(seg+columnas+1), -horizontal2);
        fprintf(output, "%d %d %d %d 0\n", -seg, seg-columnas-1, horizontal2, horizontal1);
        fprintf(output, "%d %d %d %d 0\n", -seg, -(seg-columnas-1), -horizontal2, -horizontal1);
        fprintf(output, "%d %d %d %d 0\n", -seg, horizontal2, seg+columnas+1, horizontal1);
        fprintf(output, "%d %d %d %d 0\n", -seg, -horizontal2, -(seg+columnas+1), -horizontal1);
    }
   
    //SEGMENTOS INTERIORES HORIZONTALES
   
    int contador = 0;
    for (int i = columnas+1; i <= filas*columnas; i++) {
        int vertical = filas*columnas+i;
        if (contador == 0) {
            fprintf(output, "%d %d %d %d %d 0\n", -i, i+1, vertical, vertical+1, vertical+columnas+1);
            fprintf(output, "%d %d %d %d %d 0\n", -i, -(i+1), -vertical, -(vertical+1), -(vertical+columnas+1));
            fprintf(output, "%d %d %d %d %d 0\n", -i, vertical+columnas+2, vertical, vertical+1, vertical+columnas+1);
            fprintf(output, "%d %d %d %d %d 0\n", -i, -(vertical+columnas+2), -vertical, -(vertical+1), -(vertical+columnas+1));
            fprintf(output, "%d %d %d %d %d 0\n", -i, i+1, vertical, vertical+1, vertical+columnas+2);
            fprintf(output, "%d %d %d %d %d 0\n", -i, -(i+1), -vertical, -(vertical+1), -(vertical+columnas+2));
            fprintf(output, "%d %d %d %d %d 0\n", -i, i+1, vertical, vertical+columnas+2, vertical+columnas+1);
            fprintf(output, "%d %d %d %d %d 0\n", -i, -(i+1), -vertical, -(vertical+columnas+2), -(vertical+columnas+1));
            fprintf(output, "%d %d %d %d %d 0\n", -i, i+1, vertical+columnas+2, vertical+1, vertical+columnas+1);
            fprintf(output, "%d %d %d %d %d 0\n", -i, -(i+1), -(vertical+columnas+2), -(vertical+1), -(vertical+columnas+1));
        }
        else if (contador == columnas-1) {
            fprintf(output, "%d %d %d %d %d 0\n", -i, i-1, vertical, vertical+1, vertical+columnas+1);
            fprintf(output, "%d %d %d %d %d 0\n", -i, -(i-1), -vertical, -(vertical+1), -(vertical+columnas+1));
            fprintf(output, "%d %d %d %d %d 0\n", -i, vertical+columnas+2, vertical, vertical+1, vertical+columnas+1);
            fprintf(output, "%d %d %d %d %d 0\n", -i, -(vertical+columnas+2), -vertical, -(vertical+1), -(vertical+columnas+1));
            fprintf(output, "%d %d %d %d %d 0\n", -i, i-1, vertical, vertical+1, vertical+columnas+2);
            fprintf(output, "%d %d %d %d %d 0\n", -i, -(i-1), -vertical, -(vertical+1), -(vertical+columnas+2));
            fprintf(output, "%d %d %d %d %d 0\n", -i, i-1, vertical, vertical+columnas+2, vertical+columnas+1);
            fprintf(output, "%d %d %d %d %d 0\n", -i, -(i-1), -vertical, -(vertical+columnas+2), -(vertical+columnas+1));
            fprintf(output, "%d %d %d %d %d 0\n", -i, i-1, vertical+columnas+2, vertical+1, vertical+columnas+1);
            fprintf(output, "%d %d %d %d %d 0\n", -i, -(i-1), -(vertical+columnas+2), -(vertical+1), -(vertical+columnas+1));
            contador = 0;
        }
        else {
            fprintf(output, "%d %d %d %d %d %d 0\n", -i, i+1, i-1, vertical, vertical+1, vertical+columnas+1);
            fprintf(output, "%d %d %d %d %d %d 0\n", -i, -(i+1), -(i-1), -vertical, -(vertical+1), -(vertical+columnas+1));
            fprintf(output, "%d %d %d %d %d %d 0\n", -i, i-1, vertical+columnas+2, vertical, vertical+1, vertical+columnas+1);
            fprintf(output, "%d %d %d %d %d %d 0\n", -i, -(i-1), -(vertical+columnas+2), -vertical, -(vertical+1), -(vertical+columnas+1));
            fprintf(output, "%d %d %d %d %d %d 0\n", -i, i+1, i-1, vertical, vertical+1, vertical+columnas+2);
            fprintf(output, "%d %d %d %d %d %d 0\n", -i, -(i+1), -(i-1), -vertical, -(vertical+1), -(vertical+columnas+2));
            fprintf(output, "%d %d %d %d %d %d 0\n", -i, i+1, i-1, vertical, vertical+columnas+2, vertical+columnas+1);
            fprintf(output, "%d %d %d %d %d %d 0\n", -i, -(i+1), -(i-1), -vertical, -(vertical+columnas+2), -(vertical+columnas+1));
            fprintf(output, "%d %d %d %d %d %d 0\n", -i, i+1, i-1, vertical+columnas+2, vertical+1, vertical+columnas+1);
            fprintf(output, "%d %d %d %d %d %d 0\n", -i, -(i+1), -(i-1), -(vertical+columnas+2), -(vertical+1), -(vertical+columnas+1));
            fprintf(output, "%d %d %d %d %d %d 0\n", -i, i+1, vertical, vertical+1, vertical+columnas+1, vertical+columnas+2);
            fprintf(output, "%d %d %d %d %d %d 0\n", -i, -(i+1), -vertical, -(vertical+1), -(vertical+columnas+1), -(vertical+columnas+2));
        }
        contador+=1;
    }
   
    //SEGMENTOS INTERIORES VERTICALES
    contador = 0;
    int acc = 0;
    for (int i = columnas*filas+filas+1; i < 2*columnas*filas+columnas+filas; i++) {
        if (contador == columnas) {
            contador = -1;
            acc-=1;
        }
        if (contador > 0) {
            if (i < (columnas+1)*(filas+1)) {
                fprintf(output, "%d %d %d %d %d 0\n", -i, acc, acc+1, acc+columnas, acc+columnas+1);
                fprintf(output, "%d %d %d %d %d 0\n", -i, -acc, -(acc+1), -(acc+columnas), -(acc+columnas+1));
                fprintf(output, "%d %d %d %d %d 0\n", -i, i+columnas+1, acc+1, acc+columnas, acc+columnas+1);
                fprintf(output, "%d %d %d %d %d 0\n", -i, -(i+columnas+1), -(acc+1), -(acc+columnas), -(acc+columnas+1));
                fprintf(output, "%d %d %d %d %d 0\n", -i, acc, i+columnas+1, acc+columnas, acc+columnas+1);
                fprintf(output, "%d %d %d %d %d 0\n", -i, -acc, -(i+columnas+1), -(acc+columnas), -(acc+columnas+1));
                fprintf(output, "%d %d %d %d %d 0\n", -i, acc, acc+1, i+columnas+1, acc+columnas+1);
                fprintf(output, "%d %d %d %d %d 0\n", -i, -acc, -(acc+1), -(i+columnas+1), -(acc+columnas+1));
                fprintf(output, "%d %d %d %d %d 0\n", -i, acc, acc+1, acc+columnas, i+columnas+1);
                fprintf(output, "%d %d %d %d %d 0\n", -i, -acc, -(acc+1), -(acc+columnas), -(i+columnas+1));
            }
            else if (i > 2*columnas*filas+filas) {
                fprintf(output, "%d %d %d %d %d 0\n", -i, acc, acc+1, acc+columnas, acc+columnas+1);
                fprintf(output, "%d %d %d %d %d 0\n", -i, -acc, -(acc+1), -(acc+columnas), -(acc+columnas+1));
                fprintf(output, "%d %d %d %d %d 0\n", -i, i-columnas-1, acc+1, acc+columnas, acc+columnas+1);
                fprintf(output, "%d %d %d %d %d 0\n", -i, -(i-columnas-1), -(acc+1), -(acc+columnas), -(acc+columnas+1));
                fprintf(output, "%d %d %d %d %d 0\n", -i, acc, i-columnas-1, acc+columnas, acc+columnas+1);
                fprintf(output, "%d %d %d %d %d 0\n", -i, -acc, -(i-columnas-1), -(acc+columnas), -(acc+columnas+1));
                fprintf(output, "%d %d %d %d %d 0\n", -i, acc, acc+1, i-columnas-1, acc+columnas+1);
                fprintf(output, "%d %d %d %d %d 0\n", -i, -acc, -(acc+1), -(i-columnas-1), -(acc+columnas+1));
                fprintf(output, "%d %d %d %d %d 0\n", -i, acc, acc+1, acc+columnas, i-columnas-1);
                fprintf(output, "%d %d %d %d %d 0\n", -i, -acc, -(acc+1), -(acc+columnas), -(i-columnas-1));
            }
            else {
                fprintf(output, "%d %d %d %d %d %d 0\n", -i, acc, acc+1, acc+columnas, acc+columnas+1, i-columnas-1);
                fprintf(output, "%d %d %d %d %d %d 0\n", -i, -acc, -(acc+1), -(acc+columnas), -(acc+columnas+1), -(i-columnas-1));
                fprintf(output, "%d %d %d %d %d %d 0\n", -i, i+columnas+1, acc+1, acc+columnas, acc+columnas+1, i-columnas-1);
                fprintf(output, "%d %d %d %d %d %d 0\n", -i, -(i+columnas+1), -(acc+1), -(acc+columnas), -(acc+columnas+1), -(i-columnas-1));
                fprintf(output, "%d %d %d %d %d %d 0\n", -i, acc, i+columnas+1, acc+columnas, acc+columnas+1, i-columnas-1);
                fprintf(output, "%d %d %d %d %d %d 0\n", -i, -acc, -(i+columnas+1), -(acc+columnas), -(acc+columnas+1), -(i-columnas-1));
                fprintf(output, "%d %d %d %d %d %d 0\n", -i, acc, acc+1, i+columnas+1, acc+columnas+1, i-columnas-1);
                fprintf(output, "%d %d %d %d %d %d 0\n", -i, -acc, -(acc+1), -(i+columnas+1), -(acc+columnas+1), -(i-columnas-1));
                fprintf(output, "%d %d %d %d %d %d 0\n", -i, acc, acc+1, acc+columnas, i+columnas+1, i-columnas-1);
                fprintf(output, "%d %d %d %d %d %d 0\n", -i, -acc, -(acc+1), -(acc+columnas), -(i+columnas+1), -(i-columnas-1));
                fprintf(output, "%d %d %d %d %d %d 0\n", -i, acc, acc+1, acc+columnas, acc+columnas+1, i+columnas+1);
                fprintf(output, "%d %d %d %d %d %d 0\n", -i, -acc, -(acc+1), -(acc+columnas), -(acc+columnas+1), -(i+columnas+1));
            }
        }
        contador+=1;
        acc+=1;
    }
};

void restricciones0(int arriba,int abajo, int izquierda, int derecha, FILE * output) {
	fprintf(output, "%d 0\n", (-1)*arriba);
	fprintf(output, "%d 0\n", (-1)*abajo);
	fprintf(output, "%d 0\n", (-1)*izquierda);
	fprintf(output, "%d 0\n", (-1)*derecha);
};

void restricciones1(int arriba,int abajo, int izquierda, int derecha, FILE * output) {
	fprintf(output, "%d %d %d %d 0\n", arriba,abajo,izquierda,derecha);
	fprintf(output, "%d %d 0\n", (-1)*arriba, (-1)*abajo);
	fprintf(output, "%d %d 0\n", (-1)*arriba, (-1)*izquierda);
	fprintf(output, "%d %d 0\n", (-1)*arriba, (-1)*derecha);
	fprintf(output, "%d %d 0\n", (-1)*abajo, (-1)*izquierda);
	fprintf(output, "%d %d 0\n", (-1)*abajo, (-1)*derecha);
	fprintf(output, "%d %d 0\n", (-1)*izquierda,(-1)*derecha);
};

void restricciones2(int arriba,int abajo, int izquierda, int derecha, FILE * output) {
	fprintf(output, "%d %d %d 0\n", arriba, derecha, abajo);
	fprintf(output, "%d %d %d 0\n", arriba, derecha, izquierda);
	fprintf(output, "%d %d %d 0\n", izquierda, derecha, abajo);
	fprintf(output, "%d %d %d 0\n", arriba, izquierda, abajo);
	fprintf(output, "%d %d %d 0\n", (-1)*arriba, (-1)*derecha, (-1)*abajo);
	fprintf(output, "%d %d %d 0\n", (-1)*arriba, (-1)*derecha, (-1)*izquierda);
	fprintf(output, "%d %d %d 0\n", (-1)*izquierda, (-1)*derecha, (-1)*abajo);
	fprintf(output, "%d %d %d 0\n", (-1)*arriba, (-1)*izquierda, (-1)*abajo);
};

void restricciones3(int arriba,int abajo, int izquierda, int derecha, FILE * output) {
	fprintf(output, "%d %d %d %d 0\n", (-1)*arriba, (-1)*abajo, (-1)*izquierda, (-1)*derecha);
	fprintf(output, "%d %d 0\n", arriba, abajo);
	fprintf(output, "%d %d 0\n", arriba, izquierda);
	fprintf(output, "%d %d 0\n", arriba, derecha);
	fprintf(output, "%d %d 0\n", abajo, izquierda);
	fprintf(output, "%d %d 0\n", abajo, derecha);
	fprintf(output, "%d %d 0\n", izquierda, derecha);
};

void restricciones4(int arriba,int abajo, int izquierda, int derecha, FILE * output) {
	fprintf(output, "%d 0\n", arriba);
	fprintf(output, "%d 0\n", abajo);
	fprintf(output, "%d 0\n", izquierda);
	fprintf(output, "%d 0\n", derecha);
};

void leerResultadoSat(FILE * in_file,FILE * out_file, int N, int M) {

	char str[10];
	int segments[2048];
	int var;
	char c;
	int hor = M * (N+1);
	int ver = N * (M+1);
  

	fscanf (in_file, "%s", &str);
	// cout << "El valor de str es: " << str << endl;
	if (strcmp("SAT",str) == 0) {
		// cout << "Entro al if" << endl;
		while (fscanf (in_file, "%d", &var) != EOF) {
			// printf("El valor de var es: %d\n", var);
			// cout << "El valor de var es: " << c << endl;
			if (var > 0) {
				segments[var] = 1;
			} else if (var < 0) {
				segments[(-1)*var] = 0;
			}
		}
	};

	fclose(in_file);

	int X = 1; // aux variables
	int Y = 1; // aux variables
	int i = 1;
	int j = 1;

	while ((i <= hor+1) && (j < ver+1)){
			while (i <= X*M) {
				// printf("El valor de i es: %d y el segments[%d] es: %d\n", i,i,segments[i]);
				fprintf(out_file,"%d",segments[i]);
				i++;
			}
			fprintf(out_file," ");
			X++;
	
			while (j <= Y*(M+1)) {
				// printf("El valor de j es: %d y el segments[%d] es: %d\n", j,hor+j,segments[hor+j]);
				fprintf(out_file,"%d",segments[hor+j]);
				j++;
			}
			fprintf(out_file," ");
			Y++;
	}
	while (i <= X*M) {
		// printf("El valor de i es: %d y el segments[%d] es: %d\n", i,i,segments[i]);
		fprintf(out_file,"%d",segments[i]);
		i++;
	}
};
  	


int main(int argc, char const *argv[]) {
	
	char data[10];
	int nro_filas, nro_columnas, nro_segmentos;
	
	ifstream in_file;
	FILE* out_file;
	
	out_file = fopen("output.txt","a");
	in_file.open("input.txt");  // Open file for reading.

	// read number of rows from the file
	in_file >> data;
	nro_filas = atoi(data);

	// read number of columns from the file
	in_file >> data;
	nro_columnas = atoi(data);

	if (argc == 1) {

		nro_segmentos = nro_columnas*(nro_filas+1) + nro_filas*(nro_columnas+1);

		// cout << "p cnf " << nro_segmentos << endl;

		char c;
		int j = 0;

		for (int i = 0; i < nro_filas ; i++) {
			// remover espacios
			while (in_file.get(c) && j<nro_columnas) {
				// cout << "C vale " << c << endl;
				if ((c >= 9 && c <= 13) || c == 32){
					continue;
				} else {
					int arriba;
					int abajo;
					int izquierda;
					int derecha;

					arriba = j + (i*nro_columnas)+1;
					abajo = j + ((i+1)*nro_columnas)+1;
					izquierda = j + (nro_columnas+1) * i + 1 + ((nro_filas+1)*nro_columnas);
					derecha = izquierda + 1;
					switch (c) {
						case '0':
							restricciones0(arriba,abajo,izquierda,derecha,out_file);
							break;
						case '1':
							restricciones1(arriba,abajo,izquierda,derecha,out_file);
							break;
						case '2':
							restricciones2(arriba,abajo,izquierda,derecha,out_file);
							break;
						case '3':
							restricciones3(arriba,abajo,izquierda,derecha,out_file);
							break;
						case '4':
							restricciones4(arriba,abajo,izquierda,derecha,out_file);
							break;
						case '.':
							break;
					}
					j++;
				}
			}
			j=0;
		}

		cierraLazo(nro_filas, nro_columnas, out_file);

		in_file.close();  // Close file.
		fclose(out_file);

		return 0;
	} else {

		FILE* input;
		FILE* output;
		input = fopen("ola.txt","r");
		output = fopen("Results.txt","w");
		leerResultadoSat(input,output, nro_filas, nro_columnas);

		fclose(output);
	}
}

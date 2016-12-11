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
#include <string>

using namespace std;


void restricciones0(int arriba,int abajo, int izquierda, int derecha, FILE * output) {
	fprintf(output, "%d %d %d %d '0'\n", arriba,abajo,izquierda,derecha);
};

int main(int argc, char const *argv[]) {
	
	char data[10];
	int nro_filas, nro_columnas, nro_segmentos;
	
	ifstream in_file;
	FILE* out_file;
	
	out_file = fopen("output.txt","a");
	in_file.open("example_input.txt");  // Open file for reading.

	// read number of rows from the file
	in_file >> data;
	nro_filas = atoi(data);

	// read number of columns from the file
	in_file >> data;
	nro_columnas = atoi(data);

	nro_segmentos = nro_columnas*(nro_filas+1) + nro_filas*(nro_columnas+1);

	cout << "p cnf " << nro_segmentos << endl;

	char c;
	int j = 0;

	for (int i = 0; i < nro_filas ; i++) {
		// remover espacios
		while (in_file.get(c) && j<nro_columnas) {
			cout << "C vale " << c << endl;
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
						// restricciones1(arriba,abajo,izquierda,derecha,out_file);
						break;
					case '2':
						// restricciones2(arriba,abajo,izquierda,derecha,out_file);
						break;
					case '3':
						// restricciones3(arriba,abajo,izquierda,derecha,out_file);
						break;
					case '4':
						// restricciones4(arriba,abajo,izquierda,derecha,out_file);
						break;
					case '.':
						// restriccionesPunto(arriba,abajo,izquierda,derecha,out_file);
						break;
				}
				j++;
			}
		}
		j=0;
	}


	// ciclo para leer cada una de las filas.
	// 	i++;
	// 	// ciclo para leer cada una de las filas.
	// 	continue;
	// }

	in_file.close();  // Close file.
	fclose(out_file);

	return 0;
}

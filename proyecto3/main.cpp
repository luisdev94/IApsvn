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
#include <string>

using namespace std;

int main(int argc, char const *argv[]) {
	
	char data[10];
	int nro_filas, nro_columnas, nro_segmentos;
	
	ifstream in_file;
	
	in_file.open("example_input.txt");  // Open file for reading.

	// read number of rows from the file
	in_file >> data;
	nro_filas = std::stoi(data);

	// read number of columns from the file
	in_file >> data;
	nro_columnas = std::stoi(data);

	nro_segmentos = nro_columnas*(nro_filas+1) + nro_filas*(nro_columnas+1);

	cout << "p cnf " << nro_segmentos << endl;

	for (int i = 1; i <= nro_filas; ++i) {
		// ciclo para leer cada una de las filas.
		continue;
	}

	in_file.close();  // Close file.

	return 0;
}
#include "pacientes.h"
#include "interfaz.h"
#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <stdlib.h>

// --- VALIDACIONES ---

// Esta funcion revisa que el texto (como el nombre) tenga al menos 3 letras
// y que solo contenga letras o espacios, nada de numeros ni simbolos raros.
int esTextoValido(const char *s) {
	if (strlen(s) < 3) return 0;
	for (int i = 0; s[i] != '\0'; i++) if (!isalpha(s[i]) && s[i] != ' ') return 0;
	return 1;
}

// Esta funcion asegura que lo que escribas sean solo numeros del 0 al 9.
// Sirve para validar la cedula o el telefono.
int esNumericoPuro(const char *s) {
	if (strlen(s) == 0) return 0;
	for (int i = 0; s[i] != '\0'; i++) if (!isdigit(s[i])) return 0;
	return 1;
}

//ARCHIVOS 

// Aqui leemos el archivo de texto para cargar los datos en la memoria del programa (RAM).
// Usamos punteros (*total) para saber cuantos pacientes hay en total y actualizar ese numero.
void cargarPacientes(Paciente pacientes[], int *total) {
	FILE *archivo = fopen("pacientes.txt", "r");
	*total = 0;
	
	// Si el archivo no existe (por ejemplo, la primera vez que corres el programa),
	// simplemente no hacemos nada y salimos.
	if (!archivo) return;
	
	// Leemos linea por linea usando el formato separado por punto y coma (;)
	// El %[^;] significa "lee todo hasta encontrar un ;"
	while (fscanf(archivo, "%[^;];%[^;];%d;%[^;];%[^\n]\n",
				  pacientes[*total].nombre, pacientes[*total].cedula,
				  &pacientes[*total].edad, pacientes[*total].telefono,
				  pacientes[*total].correo) == 5) {
		(*total)++; if (*total >= 100) break;
	}
	fclose(archivo);
}

// Esta funcion hace lo contrario: toma los datos de la memoria y los guarda en el disco duro.
// Abre el archivo en modo "w" (write), lo que borra lo anterior y escribe todo de nuevo actualizado.
void guardarPacientes(Paciente pacientes[], int total) {
	FILE *archivo = fopen("pacientes.txt", "w");
	if (!archivo) return;
	for (int i = 0; i < total; i++) {
		fprintf(archivo, "%s;%s;%d;%s;%s\n",
				pacientes[i].nombre, pacientes[i].cedula,
				pacientes[i].edad, pacientes[i].telefono, pacientes[i].correo);
	}
	fclose(archivo);
}

// FUNCIONES 

void registrarPaciente(Paciente pacientes[], int *total) {
	char buffer[100];
	
	// Verificamos que no nos pasemos del limite de 100 pacientes para no romper la memoria
	if (*total >= 100) {
		printf("\n    [ERROR] Limite de pacientes alcanzado.\n");
		pausa();
		return;
	}
	imprimirEncabezado("REGISTRO DE PACIENTE");
	
	// Usamos un bucle "do-while" para obligar al usuario a escribir un nombre valido.
	// Si escribe numeros o simbolos, se repite la pregunta.
	do {
		printf("  Nombre Completo: "); scanf(" %[^\n]", pacientes[*total].nombre);
	} while(!esTextoValido(pacientes[*total].nombre));
	
	// Lo mismo aqui: no dejamos avanzar hasta que la cedula sean solo numeros
	do {
		printf("  Cedula (ID): "); scanf(" %[^\n]", buffer);
	} while(!esNumericoPuro(buffer));
	strcpy(pacientes[*total].cedula, buffer);
	
	// Validamos que la edad sea un numero y que sea mayor a 0
	int validEdad = 0;
	do {
		printf("  Edad: "); 
		// scanf devuelve 1 si leyo un numero correctamente, si no, limpiamos el error
		if(scanf("%d", &pacientes[*total].edad) == 1 && pacientes[*total].edad > 0) validEdad = 1;
		else { while(getchar()!='\n'); printf("    [!] Edad invalida.\n"); }
	} while(!validEdad);
	
	printf("  Telefono: "); scanf(" %[^\n]", pacientes[*total].telefono);
	printf("  Correo: "); scanf(" %[^\n]", pacientes[*total].correo);
	
	// Aumentamos el contador total porque acabamos de agregar uno nuevo
	(*total)++;
	printf("\n    [EXITO] Paciente registrado correctamente.\n");
	pausa();
}

int consultarPacientes(Paciente pacientes[], int total) {
	imprimirEncabezado("LISTA DE PACIENTES");
	// Imprimimos una cabecera ordenada con columnas fijas
	printf("  %-25s | %-12s | %-12s\n", "NOMBRE", "CEDULA", "TELEFONO");
	imprimirLinea();
	
	// Recorremos toda la lista para mostrar cada paciente
	for (int i = 0; i < total; i++) {
		printf("  %-25s | %-12s | %-12s\n", pacientes[i].nombre, pacientes[i].cedula, pacientes[i].telefono);
	}
	if (total == 0) printf("\n    [INFO] No hay pacientes registrados.\n");
	imprimirLinea();
	return total;
}

// Esta es la parte mas truculenta: Borrar un dato de un archivo de texto.
// Como no podemos borrar una linea directamente, copiamos TODO a un archivo nuevo (temporal)
// EXCEPTO el paciente que queremos eliminar.
void eliminarPacienteAdmin(void) {
	Paciente tempP[100]; int total=0; cargarPacientes(tempP, &total);
	
	// Si no hay pacientes, no tiene sentido intentar borrar
	if(consultarPacientes(tempP, total) == 0) { pausa(); return; }
	
	FILE *f = fopen("pacientes.txt", "r");
	FILE *temp = fopen("temp_pac.txt", "w"); // Archivo temporal auxiliar
	
	char cedElim[20];
	char nom[50], ced[20], tel[20], cor[50];
	int edad;
	int found=0;
	
	if (!f || !temp) return;
	
	printf("\n  >> Ingrese CEDULA del paciente a eliminar (0 para volver): "); 
	scanf(" %[^\n]", cedElim);
	
	// Opcion de seguridad para cancelar si te arrepientes
	if (strcmp(cedElim, "0") == 0) { fclose(f); fclose(temp); remove("temp_pac.txt"); return; }
	
	// Leemos el archivo original linea por linea
	while(fscanf(f, "%[^;];%[^;];%d;%[^;];%[^\n]\n", nom, ced, &edad, tel, cor) == 5) {
		// Si la cedula coincide, lo marcamos como encontrado y NO lo escribimos en el temporal (asi se borra)
		if(strcmp(ced, cedElim) == 0) { 
			found=1; 
			printf("    [INFO] Eliminando a: %s (ID: %s)\n", nom, ced); 
		}
		// Si no es el que buscamos, lo copiamos tal cual al archivo temporal
		else {
			fprintf(temp, "%s;%s;%d;%s;%s\n", nom, ced, edad, tel, cor);
		}
	}
	fclose(f); fclose(temp);
	
	// Si encontramos y borramos al paciente, eliminamos el archivo viejo y renombramos el nuevo
	if(found) { 
		remove("pacientes.txt"); 
		rename("temp_pac.txt", "pacientes.txt"); 
		printf("\n    [EXITO] Paciente eliminado de la base de datos.\n");
	}
	else { 
		// Si no lo encontramos, borramos el temporal que creamos por gusto
		remove("temp_pac.txt"); 
		printf("\n    [ERROR] Paciente con cedula '%s' no encontrado.\n", cedElim); 
	}
	pausa();
}

// --- MENU ---

void menuPacientes(void) {
	Paciente p[100]; 
	int t = 0; 
	int op;
	
	do {
		// Cargamos los pacientes al inicio del bucle para asegurar que siempre vemos datos actualizados
		cargarPacientes(p, &t); 
		imprimirEncabezado("GESTION DE PACIENTES (ADMIN)");
		printf("    [1] Registrar Nuevo Paciente\n");
		printf("    [2] Listar Todos\n");
		printf("    [3] Eliminar Paciente\n");
		printf("    [4] Volver\n");
		imprimirLinea();
		printf("   >> Seleccione una opcion: ");
		
		// Esto es proteccion anti-bucle infinito:
		// Si el usuario escribe una letra en vez de un numero, scanf falla.
		// Limpiamos el error con getchar para poder preguntar de nuevo.
		if(scanf("%d", &op)!=1) { 
			while(getchar()!='\n'); 
			printf("\n    [ERROR] Opcion invalida. Ingrese solo numeros.\n");
			pausa(); 
			op = -1; 
			continue; 
		}
		
		switch(op) {
		case 1: 
			registrarPaciente(p, &t); 
			// Es vital guardar despues de registrar, si no los datos se perderian al cerrar el programa
			guardarPacientes(p, t); 
			break;
		case 2: 
			consultarPacientes(p, t); 
			pausa(); 
			break;
		case 3: 
			eliminarPacienteAdmin(); 
			break;
		case 4: 
			break; // Salimos del bucle
		default:
			printf("\n    [ERROR] Opcion desconocida.\n");
			pausa();
			break;
		}
		
	} while(op!=4);
}

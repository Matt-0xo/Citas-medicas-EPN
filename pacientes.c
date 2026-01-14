#include "pacientes.h"
#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <stdlib.h>

//Verificacion si la cadena contiene solo dígitos (entero positivo)
int esNumeroEnteroPositivo(const char *s) {
	if (s[0] == '\0') return 0;
	for (int i = 0; s[i] != '\0'; i++) {
		if (!isdigit(s[i])) return 0;
	}
	return 1;
}

//Cargar el listado
void cargarPacientes(Paciente pacientes[], int *total) {
	FILE *archivo = fopen("pacientes.txt", "r");
	*total = 0;
	
	if (archivo == NULL) return;
	
	while (fscanf(archivo, "%[^;];%[^;];%d;%[^;];%[^\n]\n",
				  pacientes[*total].nombre,
				  pacientes[*total].cedula,
				  &pacientes[*total].edad,
				  pacientes[*total].telefono,
				  pacientes[*total].correo) == 5) {
		(*total)++;
		if (*total >= 100) break;
	}
	
	fclose(archivo);
}

void guardarPacientes(Paciente pacientes[], int total) {
	FILE *archivo = fopen("pacientes.txt", "w");
	
	if (archivo == NULL) {
		printf("No se pudo guardar la informacion.\n");
		return;
	}
	
	for (int i = 0; i < total; i++) {
		fprintf(archivo, "%s;%s;%d;%s;%s\n",
				pacientes[i].nombre,
				pacientes[i].cedula,
				pacientes[i].edad,
				pacientes[i].telefono,
				pacientes[i].correo);
	}
	
	fclose(archivo);
	printf("Datos guardados.\n");
}

void registrarPaciente(Paciente pacientes[], int *total) {
	
	if (*total >= 100) {
		printf("No se pueden registrar mas pacientes.\n");
		return;
	}
	
	char cedTemp[20];
	char edadTexto[10];
	char telefonoTemp[20];
	
	printf("Ingrese nombre: ");
	scanf(" %[^\n]", pacientes[*total].nombre);
	
	//Validar cedula
	while (1) {
		int repetido = 0;
		
		printf("Ingrese cedula (10 digitos): ");
		scanf(" %[^\n]", cedTemp);
		
		if (strlen(cedTemp) == 10 && esNumeroEnteroPositivo(cedTemp)) {
			
			for (int i = 0; i < *total; i++) {
				if (strcmp(pacientes[i].cedula, cedTemp) == 0) {
					repetido = 1;
					break;
				}
			}
			
			if (repetido) {
				printf("Error: La cedula ya existe.\n");
			} else {
				strcpy(pacientes[*total].cedula, cedTemp);
				break;
			}
			
		} else {
			printf("Error: La cedula debe tener 10 digitos numericos.\n");
		}
	}
	
	//Validar edad
	while (1) {
		printf("Ingrese edad: ");
		scanf(" %[^\n]", edadTexto);
		
		if (esNumeroEnteroPositivo(edadTexto)) {
			pacientes[*total].edad = atoi(edadTexto);
			break;
		} else {
			printf("Error: Solo numeros enteros positivos.\n");
		}
	}
	
	//Validar telefono
	while (1) {
		printf("Ingrese telefono: ");
		scanf(" %[^\n]", telefonoTemp);
		
		if (esNumeroEnteroPositivo(telefonoTemp)) {
			strcpy(pacientes[*total].telefono, telefonoTemp);
			break;
		} else {
			printf("Error: Solo se permiten numeros.\n");
		}
	}
	
	printf("Ingrese correo: ");
	scanf(" %[^\n]", pacientes[*total].correo);
	
	(*total)++;
	printf("Paciente registrado con exito.\n");
}

//Ejecicion del listado de pacientes
void consultarPacientes(Paciente pacientes[], int total) {
	
	if (total == 0) {
		printf("No hay pacientes registrados.\n");
		return;
	}
	
	printf("\n.-- LISTA DE PACIENTES --.\n");
	for (int i = 0; i < total; i++) {
		printf("\nPaciente %d:\n", i + 1);
		printf("Nombre: %s\n", pacientes[i].nombre);
		printf("Cedula: %s\n", pacientes[i].cedula);
		printf("Edad: %d\n", pacientes[i].edad);
		printf("Telefono: %s\n", pacientes[i].telefono);
		printf("Correo: %s\n", pacientes[i].correo);
	}
}

//Ejecucion de el menu desde el main
void menuPacientes(void) {
	Paciente pacientes[100];
	int total = 0;
	int opcion;
	
	cargarPacientes(pacientes, &total);
	
	while (1) {
		printf("\n.-- Seccion de pacientes --.\n");
		printf("1. Registrar paciente\n");
		printf("2. Consultar pacientes\n");
		printf("3. Guardar y volver\n");
		printf("Seleccione una opcion: ");
		
		while (scanf("%d", &opcion) != 1) {
			printf("Entrada invalida.\n");
			while (getchar() != '\n');
		}
		
		switch (opcion) {
		case 1:
			
			//Ejecucion de la funcion registrar pacientes
			registrarPaciente(pacientes, &total);
			break;
		case 2:
			
			//Ejecucion de la funcion consultar pacientes
			consultarPacientes(pacientes, total);
			break;
		case 3:
			
			guardarPacientes(pacientes, total);
			return;
		default:
			printf("Opcion invalida.\n");
		}
	}
}

#include "medicos.h"
#include <string.h>
#include <ctype.h>

//Validacion del horario
int validarHorario(const char *h) {
	int h1, m1, h2, m2;
	
	//Verificar caracteres validos
	for (int i = 0; h[i] != '\0'; i++) {
		if (!isdigit(h[i]) && h[i] != ':' && h[i] != '-') {
			return 0;
		}
	}
	
	//Leer desde cadena
	if (sscanf(h, "%d:%d-%d:%d", &h1, &m1, &h2, &m2) != 4) {
		return 0;
	}
	
	//Rangos validos
	if (h1 < 0 || h1 > 23 || h2 < 0 || h2 > 23) return 0;
	if (m1 < 0 || m1 > 59 || m2 < 0 || m2 > 59) return 0;
	
	//Hora inicial menor que final
	if (h1 > h2 || (h1 == h2 && m1 >= m2)) return 0;
	
	return 1;
}

//Cargar el listado
void cargarMedicos(Medico medicos[], int *total) {
	FILE *archivo = fopen("medicos.txt", "r");
	*total = 0;
	
	if (archivo == NULL) return;
	
	while (fscanf(archivo, "%[^;];%[^;];%[^;];%[^\n]\n",
				  medicos[*total].codigo,
				  medicos[*total].nombre,
				  medicos[*total].especialidad,
				  medicos[*total].horario) == 4) {
		(*total)++;
		if (*total >= 100) break;
	}
	
	fclose(archivo);
}

//Guardar informacion en el archivo
void guardarMedicos(Medico medicos[], int total) {
	FILE *archivo = fopen("medicos.txt", "w");
	if (archivo == NULL) return;
	
	for (int i = 0; i < total; i++) {
		fprintf(archivo, "%s;%s;%s;%s\n",
				medicos[i].codigo,
				medicos[i].nombre,
				medicos[i].especialidad,
				medicos[i].horario);
	}
	
	fclose(archivo);
}

//Registro de medicos
void registrarMedico(Medico medicos[], int *total) {
	char codigoTemp[20];
	int duplicado;
	
	if (*total >= 100) {
		printf("No se pueden registrar mas medicos.\n");
		return;
	}
	
	//Codigo unico
	while (1) {
		duplicado = 0;
		printf("Codigo del medico: ");
		scanf(" %[^\n]", codigoTemp);
		
		if (strlen(codigoTemp) == 0) continue;
		
		for (int i = 0; i < *total; i++) {
			if (strcmp(medicos[i].codigo, codigoTemp) == 0) {
				duplicado = 1;
				break;
			}
		}
		
		if (!duplicado) {
			strcpy(medicos[*total].codigo, codigoTemp);
			break;
		}
		printf("Codigo ya existe.\n");
	}
	
	//Nombre
	do {
		printf("Nombre: ");
		scanf(" %[^\n]", medicos[*total].nombre);
	} while (strlen(medicos[*total].nombre) == 0);
	
	//Especialidad
	do {
		printf("Especialidad: ");
		scanf(" %[^\n]", medicos[*total].especialidad);
	} while (strlen(medicos[*total].especialidad) == 0);
	
	//Horario
	while (1) {
		printf("Horario (Hora:Minuto-Hora:Minuto): ");
		scanf(" %[^\n]", medicos[*total].horario);
		
		if (validarHorario(medicos[*total].horario)) break;
		printf("Horario invalido.\n");
	}
	
	(*total)++;
	printf("Medico registrado correctamente.\n");
}

//Impresion del listado modificado
void consultarMedicos(Medico medicos[], int total) {
	if (total == 0) {
		printf("No hay medicos registrados.\n");
		return;
	}
	
	printf("\n.-- LISTA DE MEDICOS --.\n");
	for (int i = 0; i < total; i++) {
		printf("\nCodigo: %s\n", medicos[i].codigo);
		printf("Nombre: %s\n", medicos[i].nombre);
		printf("Especialidad: %s\n", medicos[i].especialidad);
		printf("Horario: %s\n", medicos[i].horario);
	}
}

//Menu de la seccion de medicos
void menuMedicos(void) {
	Medico medicos[100];
	int total = 0, opcion;
	
	cargarMedicos(medicos, &total);
	
	while (1) {
		printf("\n.-- Seccion de medicos --.\n");
		printf("1. Registrar medico\n");
		printf("2. Consultar medicos\n");
		printf("3. Guardar y volver\n");
		printf("Seleccione una opcion: ");
		
		while (scanf("%d", &opcion) != 1) {
			printf("Entrada invalida. Ingrese numero: ");
			while (getchar() != '\n');
		}
		
		if (opcion == 1)
			registrarMedico(medicos, &total);
		else if (opcion == 2)
			consultarMedicos(medicos, total);
		else if (opcion == 3) {
			guardarMedicos(medicos, total);
			break;
		}
		else
				 printf("Opcion invalida.\n");
	}
}

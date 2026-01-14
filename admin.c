#include "admin.h"
#include "pacientes.h"
#include "medicos.h"
#include "citas.h"
#include <stdio.h>

void menuAdmin(void) {
	int opcion;
	
	do {
		printf("\n -_-_-Administracion del Hopital (Acceso completo)-_-_- \n");
		printf("1. Gestionar pacientes\n");
		printf("2. Gestionar medicos\n");
		printf("3. Gestionar citas\n");
		printf("4. Salir\n");
		printf("Seleccione una opcion: ");
		
		while (scanf("%d", &opcion) != 1) {
			while (getchar() != '\n');
			printf("Entrada invalida. Ingrese un numero: ");
		}
		
		switch (opcion) {
		case 1:
			menuPacientes();
			break;
			
		case 2:
			menuMedicos();
			break;
			
		case 3:
			menuAdminCitas();
			break;
			
		case 4:
			printf("Saliendo del menu administrador...\n");
			break;
			
		default:
			printf("Opcion invalida.\n");
		}
		
	} while (opcion != 4);
}

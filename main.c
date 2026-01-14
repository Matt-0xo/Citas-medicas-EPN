#include <stdio.h>
#include <string.h>
#include "usuarios.h"
#include "admin.h"
#include "citas.h"

int main() {
	Usuario u;
	int opcion;
	
	do {
		printf("\n-_-_- Sistema de registro del Centro Hospitalario -_-_-\n");
		printf("\n1. Iniciar sesion\n");
		printf("2. Registrarse\n");
		printf("3. Salir\n");
		printf("\nSeleccione una opcion:\n");
		
		while (scanf("%d", &opcion) != 1) {
			while (getchar() != '\n');
			printf("Entrada invalida. Ingrese un numero: \n");
		}
		
		while (getchar() != '\n');
		
		if (opcion == 1) {
			if (login(&u)) {
				if (strcmp(u.rol, "ADMIN") == 0)
					menuAdmin();
				else if (strcmp(u.rol, "PACIENTE") == 0)
					menuPaciente(u.idRef);
				else if (strcmp(u.rol, "MEDICO") == 0)
					menuMedico(u.idRef);
			} else {
				printf("Usuario o clave incorrectos.\n");
			}
		}
		
		else if (opcion == 2) {
			registrarUsuario();
		}
		
	} while (opcion != 3);
	
	printf("\nSaliendo del sistema...\n");
	return 0;
}

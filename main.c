#include <stdio.h>
#include <string.h>
#include <stdlib.h>   // NECESARIO para system()
#include "usuarios.h"
#include "admin.h"
#include "citas.h"

int main() {
	Usuario u;
	int opcion;
	
	//configuración visual
	//Fondo blanco, texto azul
	//Limpiar pantalla
	system("color F1");   
	system("cls");        
	
	do {
		printf("\n");
		printf("===============================================\n");
		printf("   SISTEMA DE REGISTRO DEL CENTRO HOSPITALARIO  \n");
		printf("===============================================\n");
		printf("\n");
		printf("  1. Iniciar sesion\n");
		printf("  2. Registrarse\n");
		printf("  3. Salir\n");
		printf("\n");
		printf("-----------------------------------------------\n");
		printf("Seleccione una opcion: ");
		
		// Validación de entrada numérica
		while (scanf("%d", &opcion) != 1) {
			while (getchar() != '\n');
			printf("Entrada inválida. Ingrese un número: ");
		}
		while (getchar() != '\n');
		// Limpiar pantalla después de elegir opción
		system("cls");
		
		
		if (opcion == 1) {
			if (login(&u)) {
				
				system("cls");
				printf("Bienvenido %s\n", u.usuario);
				printf("-----------------------------------------------\n");
				
				if (strcmp(u.rol, "ADMIN") == 0) {
					menuAdmin();
				}
				else if (strcmp(u.rol, "PACIENTE") == 0) {
					menuPaciente(u.idRef);
				}
				else if (strcmp(u.rol, "MEDICO") == 0) {
					menuMedico(u.idRef);
				}
				
			} else {
				printf("Usuario o clave incorrectos.\n");
			}
		}
		else if (opcion == 2) {
			printf("\n.....REGISTRO DE USUARIO.....\n");
			registrarUsuario();
		}
		else if (opcion != 3) {
			printf("Opcion invalida. Vuelva a intentarlo.\n");
		}
		
	} while (opcion != 3);
	
	printf("\nGracias por usar el sistema.\n");
	printf("Saliendo del programa...\n");
	
	return 0;
}

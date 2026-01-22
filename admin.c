#include "admin.h"
#include "pacientes.h"
#include "medicos.h"
#include "citas.h"
#include "interfaz.h"
#include <stdio.h>

void menuAdminGeneral(void) {
	int op;
	// Usamos un bucle para que el menu se repita hasta que el admin decida salir
	do {
		imprimirEncabezado("PANEL ADMINISTRADOR");
		printf("    Acceso completo - Gestion Total\n");
		printf("   --------------------------------\n");
		printf("    [1] Gestionar Pacientes (Registrar / Listar / Eliminar)\n");
		printf("    [2] Gestionar Medicos (Registrar / Listar / Eliminar)\n");
		printf("    [3] Gestionar Citas (Ver Todas / Cancelar / Reagendar)\n");
		printf("    [4] Cerrar Sesion\n");
		imprimirLinea();
		printf("   >> Seleccione una opcion: ");
		
		// --- PROTECCION DE ERRORES ---
		// Si el usuario escribe letras en lugar de numeros, el programa normalmente fallaria.
		// Con esto, detectamos el error, limpiamos el teclado y le avisamos.
		if (scanf("%d", &op) != 1) { 
			while(getchar() != '\n'); // Borramos las letras que quedaron en la memoria
			printf("\n    [ERROR] Opcion invalida. Ingrese solo numeros.\n");
			pausa(); 
			op = -1; // Forzamos un valor invalido para que el menu se repita
			continue; 
		}
		
		// Este switch funciona como un semaforo: dirige al admin a la seccion correcta
		switch(op) {
		case 1: 
			// Llama al menu de pacientes (que ya arreglamos para permitir registrar/eliminar)
			menuPacientes(); 
			break;
		case 2: 
			// Llama al menu de medicos (donde puede modificar horarios)
			menuMedicos(); 
			break; 
		case 3: 
			// Llama al menu especial de citas para administradores
			menuAdminCitas(); 
			break;
		case 4: 
			printf("\n    Cerrando sesion de administrador...\n"); 
			break;
		default: 
			// Si pone un numero que no es 1, 2, 3 o 4
			printf("\n    [ERROR] Opcion desconocida.\n");
			pausa();
			break;
		}
	} while(op != 4);
}

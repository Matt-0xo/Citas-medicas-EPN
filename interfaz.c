#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "interfaz.h" // Conectamos con el .h

// Aqui va la logica real
void configurarConsola() {
	system("color F0"); // Fondo Blanco, Letras Negras
}

void limpiarPantalla() {
	system("cls");
}

void pausa() {
	printf("\n  > Presione ENTER para continuar...");
	fflush(stdin); 
	getchar(); 
}

void imprimirEncabezado(const char *titulo) {
	limpiarPantalla();
	
	// Obtener fecha y hora
	time_t t = time(NULL);
	struct tm tm = *localtime(&t);
	
	printf("\n");
	printf("  ============================================================\n");
	printf("  FECHA: %02d/%02d/%d  |  HORA: %02d:%02d\n", 
		   tm.tm_mday, tm.tm_mon + 1, tm.tm_year + 1900, 
		   tm.tm_hour, tm.tm_min);
	printf("  ============================================================\n");
	printf("        SISTEMA HOSPITALARIO - %s\n", titulo);
	printf("  ============================================================\n");
	printf("\n");
}

void imprimirLinea() {
	printf("  ------------------------------------------------------------\n");
}

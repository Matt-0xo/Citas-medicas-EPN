
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "interfaz.h" 

// Esta funcion configura los colores de la consola al iniciar.
void configurarConsola() {
	system("color F0"); 
}

// Esta funcion borra todo lo que hay en la pantalla.
// Sirve para que los menus no se amontonen uno encima del otro.
void limpiarPantalla() {
	system("cls");
}

// Esta funcion detiene el programa momentaneamente.
// Es util para que el usuario pueda leer un mensaje de error o exito antes de continuar.
void pausa() {
	printf("\n  > Presione ENTER para continuar...");
	fflush(stdin); // Limpiamos el teclado por si quedaron teclas 'basura'
	getchar(); // Esperamos a que la persona presione una tecla
}

// Esta funcion dibuja el encabezado que ves arriba en todos los menus.
// Muestra la fecha, la hora y el titulo de la seccion actual.
void imprimirEncabezado(const char *titulo) {
	limpiarPantalla();
	
	// Le pedimos al reloj de la computadora la hora actual
	time_t t = time(NULL);
	struct tm tm = *localtime(&t);
	
	printf("\n");
	// Imprimimos la fecha y hora con un formato bonito (Dia/Mes/Anio | Hora:Minuto)
	printf("  FECHA: %02d/%02d/%d  |  HORA: %02d:%02d\n\n", 
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
  

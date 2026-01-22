#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "usuarios.h"
#include "pacientes.h"
#include "medicos.h"
#include "citas.h"
#include "admin.h"   
#include "interfaz.h"

// Esta funcion funciona como un "semaforo" o distribuidor.
// Recibe al usuario que acaba de loguearse y revisa su ROL para enviarlo al menu correcto.
void despacharUsuario(Usuario u) {
	// Usamos strcmp para comparar texto. Si el rol es ADMIN, lo mandamos al panel general.
	if (strcmp(u.rol, "ADMIN") == 0) {
		menuAdminGeneral(); 
	} 
	else if (strcmp(u.rol, "MEDICO") == 0) {
		// Al medico le pasamos su codigo (idRef) para que el sistema sepa quien es
		menuMedico(u.idRef); 
	} 
	else if (strcmp(u.rol, "PACIENTE") == 0) {
		// Al paciente le pasamos su cedula (idRef) para cargar su historial
		menuPaciente(u.idRef); 
	} 
	else {
		// Esto es por seguridad, por si hay un usuario con rol raro en el archivo
		printf("\n  [ERROR] Rol desconocido.\n");
		pausa();
	}
}

int main() {
	// Configuramos el color blanco y negro de la consola
	configurarConsola(); 
	
	// Esta linea "siembra" la semilla del generador de numeros aleatorios con la hora actual.
	// Sin esto, los codigos de citas generados serian siempre los mismos cada vez que abres el programa.
	srand(time(NULL));   
	
	int op;
	Usuario u;
	
	// Bucle principal: El programa nunca se cierra a menos que el usuario elija la opcion Salir
	do {
		imprimirEncabezado("BIENVENIDO");
		printf("    [1]  Iniciar Sesion\n");
		printf("    [2]  Registrarse (Nuevo Usuario)\n");
		printf("    [3]  Salir del Sistema\n");
		imprimirLinea();
		printf("   >> Seleccione una opcion: ");
		
		// --- PROTECCION CONTRA ERRORES ---
		// scanf devuelve 1 si leyo un numero correctamente.
		// Si devuelve 0, significa que el usuario escribio letras o simbolos.
		if (scanf("%d", &op) != 1) {
			// Limpiamos el teclado (buffer) para quitar las letras erroneas
			while (getchar() != '\n'); 
			printf("\n    [ERROR] Opcion invalida. Ingrese solo numeros.\n");
			pausa();
			op = -1; // Forzamos que se repita el bucle
			continue;
		}
		// Limpiamos el 'Enter' que queda flotando despues de escribir el numero
		while (getchar() != '\n'); 		
		
		switch (op) {
		case 1:
			// Llamamos a la funcion login. Si devuelve 1 (Verdadero), entramos.
			if (login(&u)) {
				printf("\n    [OK] Acceso concedido. Bienvenido %s.\n", u.usuario);
				pausa();
				// Mandamos al usuario a su menu correspondiente
				despacharUsuario(u);
			}
			break;
		case 2:
			// Llama al proceso de crear cuenta nueva
			registrarUsuario();
			break;
		case 3:
			printf("\n    Saliendo del sistema...\n");
			break;
		default:
			printf("\n    [ERROR] Opcion desconocida. Intente de nuevo.\n");
			pausa();
			break;
		}
		
	} while (op != 3); // Repetir mientras no sea la opcion 3
	
	return 0;
}

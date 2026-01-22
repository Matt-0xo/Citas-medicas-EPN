#include "usuarios.h"
#include "pacientes.h"
#include "medicos.h"
#include "interfaz.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h> // Necesario para validar si son letras y numeros

// --- FUNCIONES DE VALIDACION ---

// Validacion de seguridad: El nombre de usuario solo debe tener letras y numeros.
// Esto evita que pongan espacios o simbolos que podrian dañar el archivo de texto.
int esUsuarioValido(const char *s) {
	// Primero revisamos que tenga al menos 3 caracteres
	if (strlen(s) < 3) return 0; 
	
	// Revisamos letra por letra
	for (int i = 0; s[i] != '\0'; i++) {
		// La funcion isalnum revisa si es letra (A-Z) o numero (0-9).
		// Si encuentra un simbolo raro (como @, space, #), devuelve error.
		if (!isalnum(s[i])) return 0; 
	}
	return 1;
}

// --- FUNCIONES PRINCIPALES ---

// Esta funcion busca en el archivo de texto si el nombre de usuario ya esta ocupado.
int existeUsuario(const char *nombre) {
	FILE *f = fopen("usuarios.txt", "r");
	Usuario u;
	
	// Si el archivo no existe, significa que nadie se ha registrado aun.
	if (!f) return 0;
	
	// Leemos linea por linea el archivo
	while (fscanf(f, "%[^;];%[^;];%[^;];%[^\n]\n", u.usuario, u.clave, u.rol, u.idRef) == 4) {
		// Si encontramos el nombre exacto, cerramos y decimos que SI existe (1)
		if (strcmp(u.usuario, nombre) == 0) { fclose(f); return 1; }
	}
	fclose(f); return 0;
}

// Funcion principal para entrar al sistema (Login)
int login(Usuario *u) {
	FILE *f = fopen("usuarios.txt", "r");
	char usuarioIn[50], claveIn[50];
	Usuario temp;
	int encontrado = 0;
	
	// Si no hay archivo de usuarios, no se puede iniciar sesion
	if (!f) {
		printf("\n    [ERROR] No hay usuarios registrados.\n");
		pausa();
		return 0;
	}
	
	imprimirEncabezado("INICIO DE SESION");
	
	// --- 1. INGRESAR USUARIO ---
	// Permitimos que el usuario escriba '0' si quiere arrepentirse y volver al menu
	printf("  Usuario (0 para volver): ");
	fgets(usuarioIn, sizeof(usuarioIn), stdin);
	usuarioIn[strcspn(usuarioIn, "\n")] = '\0'; // Quitamos el salto de linea del Enter
	
	if (strcmp(usuarioIn, "0") == 0) {
		fclose(f);
		return 0; 
	}
	
	// --- 2. INGRESAR CLAVE ---
	printf("  Clave:   ");
	fgets(claveIn, sizeof(claveIn), stdin);
	claveIn[strcspn(claveIn, "\n")] = '\0';
	
	char linea[200];
	// Leemos el archivo buscando coincidencia de Usuario Y Clave
	while (fgets(linea, sizeof(linea), f)) {
		if (sscanf(linea, "%[^;];%[^;];%[^;];%[^\n]", temp.usuario, temp.clave, temp.rol, temp.idRef) == 4) {
			// Limpieza de caracteres basura
			temp.idRef[strcspn(temp.idRef, "\r")] = '\0'; 
			temp.idRef[strcspn(temp.idRef, "\n")] = '\0';
			
			// Si el usuario y la clave coinciden
			if (strcmp(temp.usuario, usuarioIn) == 0 && strcmp(temp.clave, claveIn) == 0) {
				// Copiamos los datos al puntero 'u' para que el sistema sepa quien entro
				*u = temp;
				encontrado = 1;
				break;
			}
		}
	}
	fclose(f);
	
	if (!encontrado) {
		printf("\n    [!] Credenciales incorrectas.\n");
		pausa();
	}
	return encontrado;
}

// Proceso para crear una cuenta nueva
void registrarUsuario(void) {
	Usuario u;
	int opcionRol;
	
	imprimirEncabezado("NUEVA CUENTA");
	
	// --- 1. USUARIO ---
	// Este bucle se repite hasta que el usuario ingrese un nombre valido y disponible
	do {
		printf("  Nombre de Usuario (0 para volver): ");
		fgets(u.usuario, sizeof(u.usuario), stdin);
		u.usuario[strcspn(u.usuario, "\n")] = '\0';
		
		// Opcion para cancelar el registro
		if (strcmp(u.usuario, "0") == 0) {
			printf("\n    [INFO] Operacion cancelada.\n");
			pausa();
			return;
		}
		
		// VALIDACION 1: Que no este vacio
		if(strlen(u.usuario) == 0) {
			printf("    [ERROR] El usuario no puede estar vacio.\n");
			continue;
		}
		
		// VALIDACION 2: Que solo tenga letras y numeros (Seguridad)
		if(!esUsuarioValido(u.usuario)) {
			printf("    [ERROR] El usuario solo puede contener letras y numeros (Min 3 chars).\n");
			printf("            No use espacios ni simbolos.\n");
			continue; // Repite el bucle
		}
		
		// VALIDACION 3: Que no exista otro usuario con el mismo nombre
		if (existeUsuario(u.usuario)) {
			printf("    [ERROR] El usuario '%s' ya existe. Intente otro.\n", u.usuario);
		}
	} while (strlen(u.usuario) == 0 || !esUsuarioValido(u.usuario) || existeUsuario(u.usuario));
	
	// --- 2. CLAVE ---
	do {
		printf("  Clave de Acceso (0 para volver):   ");
		fgets(u.clave, sizeof(u.clave), stdin);
		u.clave[strcspn(u.clave, "\n")] = '\0';
		
		if (strcmp(u.clave, "0") == 0) {
			printf("\n    [INFO] Operacion cancelada.\n");
			pausa();
			return;
		}
		
		if(strlen(u.clave) == 0) printf("    [ERROR] La clave no puede estar vacia.\n");
	} while (strlen(u.clave) == 0);
	
	// --- 3. TIPO DE CUENTA ---
	// Aqui preguntamos si es Paciente o Medico
	do {
		printf("\n  Tipo de Cuenta:\n    [1] Paciente\n    [2] Medico\n    [0] Volver\n   >> ");
		
		if (scanf("%d", &opcionRol) != 1) { 
			while(getchar() != '\n'); 
			opcionRol = -1; 
		} else {
			while(getchar() != '\n'); 
		}
		
		if (opcionRol == 0) {
			printf("\n    [INFO] Operacion cancelada.\n");
			pausa();
			return;
		}
		
		if (opcionRol != 1 && opcionRol != 2) {
			printf("\n    [ERROR] Opcion invalida. Por favor ingrese 1 o 2.\n");
		}
		
	} while (opcionRol != 1 && opcionRol != 2);
	
	// Guardamos el rol en texto para el archivo
	if (opcionRol == 1) strcpy(u.rol, "PACIENTE"); else strcpy(u.rol, "MEDICO");
	
	imprimirEncabezado("VINCULACION DE DATOS");
	
	// AQUI OCURRE LA VINCULACION AUTOMATICA:
	// Antes de crear el usuario, llamamos al modulo correspondiente (Pacientes o Medicos)
	// para que el usuario llene sus datos personales (Nombre real, edad, cedula, etc.)
	if (opcionRol == 1) {
		Paciente pacientes[100]; int total = 0; cargarPacientes(pacientes, &total);
		registrarPaciente(pacientes, &total);
		
		// "Amarramos" el usuario con la cedula del paciente recien creado
		strcpy(u.idRef, pacientes[total-1].cedula);
		guardarPacientes(pacientes, total); 
	} else {
		Medico medicos[100]; int totalM = 0; cargarMedicos(medicos, &totalM);
		registrarMedico(medicos, &totalM);
		
		// "Amarramos" el usuario con el codigo del medico recien creado
		strcpy(u.idRef, medicos[totalM-1].codigo);
		guardarMedicos(medicos, totalM);
	}
	
	// Finalmente, guardamos el usuario y su contraseña en el archivo
	FILE *f = fopen("usuarios.txt", "a");
	if (f) {
		fprintf(f, "%s;%s;%s;%s\n", u.usuario, u.clave, u.rol, u.idRef);
		fclose(f);
		printf("\n    ============================================================");
		printf("\n    [EXITO] CUENTA CREADA CORRECTAMENTE.");
		printf("\n    Usuario: %s", u.usuario);
		printf("\n    Rol: %s", u.rol);
		printf("\n    ============================================================");
	}
	pausa();
}

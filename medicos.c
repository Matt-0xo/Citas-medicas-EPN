#include "medicos.h"
#include "interfaz.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

// Esta funcion revisa si el horario tiene sentido (ejemplo: que no pongas la hora 25 o minutos 70)
int validarFormatoHorario(const char *h) {
	int h1, m1, h2, m2;
	
	// Intentamos leer 4 numeros del texto con el formato hora:minuto-hora:minuto
	if (sscanf(h, "%d:%d-%d:%d", &h1, &m1, &h2, &m2) != 4) return 0;
	
	// Revisamos que las horas esten entre 0 y 23
	if (h1 < 0 || h1 > 23 || h2 < 0 || h2 > 23) return 0;
	
	// Convertimos todo a minutos para asegurar que la hora de fin sea DESPUES de la de inicio
	if ((h1*60+m1) >= (h2*60+m2)) return 0; 
	return 1;
}

void cargarMedicos(Medico medicos[], int *total) {
	// Abrimos el archivo solo para lectura
	FILE *archivo = fopen("medicos.txt", "r");
	*total = 0;
	
	// Si el archivo no existe, no hacemos nada
	if (!archivo) return;
	
	// Leemos linea por linea separando los datos por punto y coma (;)
	// Esto llena la lista de medicos en la memoria del programa
	while (fscanf(archivo, "%[^;];%[^;];%[^;];%[^\n]\n",
				  medicos[*total].codigo, medicos[*total].nombre,
				  medicos[*total].especialidad, medicos[*total].horario) == 4) {
		(*total)++; if (*total >= 100) break;
	}
	fclose(archivo);
}

void guardarMedicos(Medico medicos[], int total) {
	// Abrimos el archivo en modo escritura ("w") para sobrescribirlo todo con los datos nuevos
	FILE *archivo = fopen("medicos.txt", "w");
	if (!archivo) return;
	
	// Recorremos la lista de la memoria y la escribimos en el archivo de texto
	for (int i = 0; i < total; i++) {
		fprintf(archivo, "%s;%s;%s;%s\n",
				medicos[i].codigo, medicos[i].nombre,
				medicos[i].especialidad, medicos[i].horario);
	}
	fclose(archivo);
}

void registrarMedico(Medico medicos[], int *total) {
	char cod[20];
	// Verificamos que no hayamos llenado el limite de la lista
	if (*total >= 100) return;
	imprimirEncabezado("REGISTRO DE MEDICO");
	
	int ok=0;
	// Repetimos este bloque hasta que el usuario ingrese un codigo valido
	do {
		printf("  Codigo Unico: "); scanf(" %[^\n]", cod);
		int dup=0;
		// Buscamos en toda la lista si ese codigo ya existe para evitar duplicados
		for(int i=0; i<*total; i++) if(strcmp(medicos[i].codigo, cod)==0) dup=1;
		
		if(dup) printf("    [!] Codigo repetido.\n"); else ok=1;
	} while(!ok);
	
	// Si todo esta bien, guardamos el codigo
	strcpy(medicos[*total].codigo, cod);
	
	printf("  Nombre: "); scanf(" %[^\n]", medicos[*total].nombre);
	printf("  Especialidad: "); scanf(" %[^\n]", medicos[*total].especialidad);
	
	// Pedimos el horario y no dejamos avanzar hasta que tenga el formato correcto
	do {
		printf("  Horario (HH:MM-HH:MM): "); scanf(" %[^\n]", medicos[*total].horario);
		if (!validarFormatoHorario(medicos[*total].horario)) {
			printf("    [ERROR] Formato incorrecto. Ej: 08:00-16:00.\n");
		}
	} while (!validarFormatoHorario(medicos[*total].horario));
	
	// Aumentamos el contador de medicos y terminamos
	(*total)++;
	printf("\n  [OK] Medico registrado.\n");
	pausa();
}

int consultarMedicos(Medico medicos[], int total) {
	imprimirEncabezado("LISTA DE MEDICOS");
	// Imprimimos una tabla ordenada con columnas
	printf("  %-10s | %-20s | %-15s\n", "CODIGO", "NOMBRE", "HORARIO");
	imprimirLinea();
	for (int i = 0; i < total; i++) {
		printf("  %-10s | %-20s | %-15s\n", medicos[i].codigo, medicos[i].nombre, medicos[i].horario);
	}
	if(total == 0) printf("\n    [INFO] No hay medicos.\n");
	imprimirLinea();
	return total;
}

void eliminarMedicoAdmin(void) {
	// Cargamos los datos primero para mostrar la lista al usuario
	Medico tempM[100]; int total=0; cargarMedicos(tempM, &total);
	if(consultarMedicos(tempM, total) == 0) { pausa(); return; }
	
	FILE *f = fopen("medicos.txt", "r");
	// Creamos un archivo temporal vacio donde copiaremos a los que NO se eliminan
	FILE *temp = fopen("temp_med.txt", "w");
	char codElim[20], cod[20], nom[50], esp[50], hor[20];
	int found=0;
	
	if (!f || !temp) return;
	printf("  >> Codigo a eliminar (0 para volver): "); 
	scanf(" %[^\n]", codElim);
	
	// Si escribe 0, cerramos todo y cancelamos
	if (strcmp(codElim, "0") == 0) { fclose(f); fclose(temp); remove("temp_med.txt"); return; }
	
	// Leemos linea por linea el archivo original
	while(fscanf(f, "%[^;];%[^;];%[^;];%[^\n]\n", cod, nom, esp, hor) == 4) {
		// Si encontramos el codigo que queremos eliminar, NO lo escribimos en el temporal
		if(strcmp(cod, codElim) == 0) { found=1; printf("  [INFO] Eliminado: %s\n", nom); }
		// Si no es el eliminado, lo copiamos al archivo temporal
		else fprintf(temp, "%s;%s;%s;%s\n", cod, nom, esp, hor);
	}
	fclose(f); fclose(temp);
	
	// Si lo eliminamos, borramos el archivo viejo y renombramos el temporal como el nuevo original
	if(found) { remove("medicos.txt"); rename("temp_med.txt", "medicos.txt"); }
	else { remove("temp_med.txt"); printf("  [!] Medico no encontrado.\n"); }
	pausa();
}

void modificarHorarioMedico(const char *codigoMedico) {
	FILE *f = fopen("medicos.txt", "r");
	// Usamos la misma estrategia del archivo temporal para editar
	FILE *temp = fopen("temp_med_h.txt", "w");
	char cod[20], nom[50], esp[50], hor[40], nuevo[40];
	int encontrado=0;
	
	if(!f || !temp) return;
	imprimirEncabezado("MODIFICACION DE HORARIO");
	
	// Leemos todo el archivo original
	while(fscanf(f, "%[^;];%[^;];%[^;];%[^\n]\n", cod, nom, esp, hor) == 4) {
		// Si encontramos al medico que queremos editar
		if (strcmp(cod, codigoMedico) == 0) {
			encontrado = 1;
			printf("    Medico: %s\n", nom);
			printf("    Horario Actual: %s\n", hor);
			int valid = 0;
			// Pedimos el nuevo horario hasta que sea valido
			do {
				printf("    Nuevo Horario (HH:MM-HH:MM) [0 para volver]: ");
				scanf(" %[^\n]", nuevo);
				// Si quiere cancelar, mantenemos el horario viejo
				if (strcmp(nuevo, "0") == 0) { strcpy(nuevo, hor); break; }
				if (validarFormatoHorario(nuevo)) valid = 1;
				else printf("    [!] Formato invalido.\n");
			} while (!valid);
			// Escribimos en el archivo temporal con el NUEVO horario
			fprintf(temp, "%s;%s;%s;%s\n", cod, nom, esp, nuevo);
		} else {
			// Si no es el medico a editar, lo copiamos tal cual estaba
			fprintf(temp, "%s;%s;%s;%s\n", cod, nom, esp, hor);
		}
	}
	fclose(f); fclose(temp);
	
	// Reemplazamos el archivo viejo con el actualizado
	if(encontrado) { 
		remove("medicos.txt"); 
		rename("temp_med_h.txt", "medicos.txt"); 
		printf("\n    [EXITO] Horario actualizado.\n"); 
	}
	else {
		remove("temp_med_h.txt");
		printf("\n    [ERROR] Medico con codigo '%s' no encontrado.\n", codigoMedico);
	}
	pausa();
}

void menuMedicos(void) {
	Medico m[100]; int t=0; int op;
	char codBusqueda[20]; 
	
	do {
		// Cargamos los medicos al inicio para tener la lista actualizada
		cargarMedicos(m, &t);
		imprimirEncabezado("GESTION DE MEDICOS (ADMIN)");
		printf("    [1] Registrar Nuevo Medico\n");
		printf("    [2] Listar Todos\n");
		printf("    [3] Eliminar Medico\n");
		printf("    [4] Modificar Horario de un Medico\n");
		printf("    [5] Volver\n");
		imprimirLinea();
		printf("   >> Seleccione una opcion: ");
		
		// --- VALIDACION IMPORTANTE ---
		// Si el usuario escribe letras en vez de numeros, scanf devuelve error
		if(scanf("%d", &op) != 1) { 
			// Limpiamos la 'basura' del teclado para que no se cicle infinitamente
			while(getchar()!='\n'); 
			printf("\n    [ERROR] Opcion invalida. Debe ingresar un numero.\n");
			// Hacemos una pausa para que el usuario pueda leer el mensaje de error
			pausa(); 
			op = -1; // Forzamos una opcion invalida para repetir el menu
			continue; 
		}
		// -----------------------
		
		switch(op){
		case 1: registrarMedico(m, &t); guardarMedicos(m,t); break;
		case 2: consultarMedicos(m, t); pausa(); break;
		case 3: eliminarMedicoAdmin(); break;
		case 4: 
			// Solo entramos si hay medicos registrados
			if(consultarMedicos(m, t) > 0) { 
				printf("\n  >> Ingrese CODIGO del medico a editar (0 volver): ");
				scanf(" %[^\n]", codBusqueda);
				// Si no escribio 0, procedemos a modificar
				if(strcmp(codBusqueda, "0") != 0) {
					modificarHorarioMedico(codBusqueda);
				}
			} else {
				pausa();
			}
			break;
		}
	} while(op!=5);
}

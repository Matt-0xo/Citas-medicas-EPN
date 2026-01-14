#include "usuarios.h"
#include "pacientes.h"
#include "medicos.h"
#include <stdio.h>
#include <string.h>

int existeUsuario(const char *nombre) {
	FILE *f = fopen("usuarios.txt", "r");
	Usuario u;
	
	if (!f) return 0;
	
	while (fscanf(f, "%[^;];%[^;];%[^;];%[^\n]\n",
				  u.usuario, u.clave, u.rol, u.idRef) == 4) {
		
		if (strcmp(u.usuario, nombre) == 0) {
			fclose(f);
			return 1;
		}
	}
	
	fclose(f);
	return 0;
}

int login(Usuario *u) {
	FILE *f = fopen("usuarios.txt", "r");
	char linea[200];
	char usuarioIn[50], claveIn[50];
	
	if (!f) {
		printf("No hay usuarios registrados.\n");
		return 0;
	}
	
	printf("Usuario: ");
	fgets(usuarioIn, sizeof(usuarioIn), stdin);
	usuarioIn[strcspn(usuarioIn, "\n")] = '\0';
	
	while (fgets(linea, sizeof(linea), f)) {
		Usuario temp;
		
		sscanf(linea, "%[^;];%[^;];%[^;];%[^\n]",
			   temp.usuario, temp.clave, temp.rol, temp.idRef);
		
		if (strcmp(temp.usuario, usuarioIn) == 0) {
			
			printf("Clave: ");
			fgets(claveIn, sizeof(claveIn), stdin);
			claveIn[strcspn(claveIn, "\n")] = '\0';
			
			if (strcmp(temp.clave, claveIn) == 0) {
				*u = temp;
				fclose(f);
				return 1;
			} else {
				fclose(f);
				printf("Usuario o clave incorrectos.\n");
				return 0;
			}
		}
	}
	
	fclose(f);
	printf("Usuario o clave incorrectos.\n");
	return 0;
}

void registrarUsuario(void) {
	FILE *f = fopen("usuarios.txt", "a");
	Usuario u;
	
	if (!f) {
		printf("No se pudo abrir el archivo.\n");
		return;
	}
	
	//Para darse un usuario unico
	do {
		printf("\nNuevo usuario: ");
		fgets(u.usuario, sizeof(u.usuario), stdin);
		u.usuario[strcspn(u.usuario, "\n")] = '\0';
		
		if (existeUsuario(u.usuario))
			printf("Error: usuario ya existe.\n");
		
	} while (existeUsuario(u.usuario));
	
	printf("Clave: ");
	fgets(u.clave, sizeof(u.clave), stdin);
	u.clave[strcspn(u.clave, "\n")] = '\0';
	
	//Rol controlado
	int rolValido = 0;
	int opcionRol;
	
	do {
		printf("Registrarse como:\n");
		printf("Paciente (1)\n");
		printf("Medico (2)\n");
		printf("Elija una opcion: ");

		//En caso de ingresarse letras
		if (scanf("%d", &opcionRol) != 1) {
			printf("Dato invalido. Ingresar solo 1 o 2\n");
			while (getchar() != '\n');
			continue;
		}
		
		while (getchar() != '\n');
		
		if (opcionRol == 1) {
			strcpy(u.rol, "PACIENTE");
			rolValido = 1;
		}
		else if (opcionRol == 2) {
			strcpy(u.rol, "MEDICO");
			rolValido = 1;
		}
		
		//En caso de ingresarse un numero no admitido
		else {
			printf("Opcion invalida. Seleccione solo 1 o 2.\n");
		}
		
	} while (!rolValido);
	
	printf("Codigo unico (ID): ");
	fgets(u.idRef, sizeof(u.idRef), stdin);
	u.idRef[strcspn(u.idRef, "\n")] = '\0';
	
	fprintf(f, "%s;%s;%s;%s\n",
			u.usuario, u.clave, u.rol, u.idRef);
	
	fclose(f);
	printf("Usuario registrado correctamente.\n");

	if (strcmp(u.rol, "PACIENTE") == 0) {
		Paciente pacientes[100];
		int total = 0;
		Paciente p;
		
		cargarPacientes(pacientes, &total);
		
		int existe = 0;
		for (int i = 0; i < total; i++) {
			if (strcmp(pacientes[i].cedula, u.idRef) == 0) {
				existe = 1;
				break;
			}
		}
		
		if (!existe) {
			strcpy(p.cedula, u.idRef);
			
			printf("\n--- Registro de datos del paciente ---\n");
			
			printf("Nombre completo: ");
			fgets(p.nombre, sizeof(p.nombre), stdin);
			p.nombre[strcspn(p.nombre, "\n")] = '\0';
			
			printf("Edad: ");
			scanf("%d", &p.edad);
			while (getchar() != '\n');
			
			printf("Telefono: ");
			fgets(p.telefono, sizeof(p.telefono), stdin);
			p.telefono[strcspn(p.telefono, "\n")] = '\0';
			
			printf("Correo: ");
			fgets(p.correo, sizeof(p.correo), stdin);
			p.correo[strcspn(p.correo, "\n")] = '\0';
			
			pacientes[total++] = p;
			guardarPacientes(pacientes, total);
			
			printf("Paciente registrado correctamente.\n");
		}
	}

	if (strcmp(u.rol, "MEDICO") == 0) {
		Medico medicos[100];
		int totalM = 0;
		Medico m;
		
		cargarMedicos(medicos, &totalM);
		
		int existeM = 0;
		for (int i = 0; i < totalM; i++) {
			if (strcmp(medicos[i].codigo, u.idRef) == 0) {
				existeM = 1;
				break;
			}
		}
		
		if (!existeM) {
			strcpy(m.codigo, u.idRef);
			
			printf("\n--- Registro de datos del medico ---\n");
			
			printf("Nombre completo: ");
			fgets(m.nombre, sizeof(m.nombre), stdin);
			m.nombre[strcspn(m.nombre, "\n")] = '\0';
			
			printf("Especialidad: ");
			fgets(m.especialidad, sizeof(m.especialidad), stdin);
			m.especialidad[strcspn(m.especialidad, "\n")] = '\0';
			
			do {
				printf("Horario (Hora:Minuto-Hora:Minuto): ");
				fgets(m.horario, sizeof(m.horario), stdin);
				m.horario[strcspn(m.horario, "\n")] = '\0';
			} while (!validarHorario(m.horario));
			
			medicos[totalM++] = m;
			guardarMedicos(medicos, totalM);
			
			printf("Medico registrado correctamente.\n");
		}
	}
}

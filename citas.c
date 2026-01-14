#include "citas.h"
#include "medicos.h"
#include <stdio.h>
#include <string.h>

#define MAX 100

int cargarCitas(Cita citas[]) {
	FILE *f = fopen("citas.txt", "r");
	int total = 0;
	
	if (!f) return 0;
	
	while (fscanf(f, "%[^;];%[^;];%[^;];%[^;];%[^;];%[^;];%[^\n]\n",
				  citas[total].id,
				  citas[total].cedulaPaciente,
				  citas[total].codigoMedico,
				  citas[total].fecha,
				  citas[total].hora,
				  citas[total].estado,
				  citas[total].diagnostico) == 7) {
		total++;
		if (total >= MAX) break;
	}
	
	fclose(f);
	return total;
}

void guardarCitas(Cita citas[], int total) {
	FILE *f = fopen("citas.txt", "w");
	if (!f) return;
	
	for (int i = 0; i < total; i++) {
		fprintf(f, "%s;%s;%s;%s;%s;%s;%s\n",
				citas[i].id,
				citas[i].cedulaPaciente,
				citas[i].codigoMedico,
				citas[i].fecha,
				citas[i].hora,
				citas[i].estado,
				citas[i].diagnostico);
	}
	
	fclose(f);
}

void agendarCitaAdmin(void) {
	char cedula[20];
	
	printf("Ingrese cedula del paciente: ");
	scanf(" %19s", cedula);
	
	agendarCita(cedula);
}

void listarTodasCitas(void) {
	Cita citas[MAX];
	int total = cargarCitas(citas);
	
	if (total == 0) {
		printf("No hay citas.\n");
		return;
	}
	
	for (int i = 0; i < total; i++) {
		printf("\nID: %s\nPaciente: %s\nMedico: %s\nFecha: %s %s\nEstado: %s\n",
			   citas[i].id,
			   citas[i].cedulaPaciente,
			   citas[i].codigoMedico,
			   citas[i].fecha,
			   citas[i].hora,
			   citas[i].estado);
	}
}

void menuAdminCitas() {
	int op;
	
	do {
		printf("\n-- Control de citas (Administrador) --\n");
		printf("1. Crear cita\n");
		printf("2. Listar citas\n");
		printf("3. Reprogramar cita\n");
		printf("4. Volver\n");
		printf("Opcion: ");
		
		if (scanf("%d", &op) != 1) {
			while (getchar() != '\n');
			printf("Entrada invalida.\n");
			continue;
		}
		while (getchar() != '\n');
		
		switch (op) {
		case 1:
			agendarCitaAdmin();
			break;
			
		case 2:
			listarTodasCitas();
			break;
			
		case 3:
			reprogramarCitaAdmin();
			break;
			
		case 4:
			printf("Volviendo...\n");
			break;
			
		default:
			printf("Opcion invalida.\n");
		}
		
	} while (op != 4);
}

void reprogramarCitaAdmin(void) {
	Cita citas[200];
	int total = 0;
	char idBuscar[10];
	FILE *f = fopen("citas.txt", "r");
	
	if (!f) return;
	
	while (fscanf(f, "%[^;];%[^;];%[^;];%[^;];%[^;];%[^;];%[^\n]\n",
				  citas[total].id,
				  citas[total].cedulaPaciente,
				  citas[total].codigoMedico,
				  citas[total].fecha,
				  citas[total].hora,
				  citas[total].estado,
				  citas[total].diagnostico) == 7) {
		total++;
	}
	fclose(f);
	
	printf("ID de cita: ");
	fgets(idBuscar, sizeof(idBuscar), stdin);
	idBuscar[strcspn(idBuscar, "\n")] = '\0';
	
	for (int i = 0; i < total; i++) {
		if (strcmp(citas[i].id, idBuscar) == 0 &&
			strcmp(citas[i].estado, "ACTIVA") == 0) {
			
			printf("Nueva fecha: ");
			fgets(citas[i].fecha, sizeof(citas[i].fecha), stdin);
			citas[i].fecha[strcspn(citas[i].fecha, "\n")] = '\0';
			
			printf("Nueva hora: ");
			fgets(citas[i].hora, sizeof(citas[i].hora), stdin);
			citas[i].hora[strcspn(citas[i].hora, "\n")] = '\0';
			
			if (!medicoDisponible(citas[i].codigoMedico, citas[i].fecha, citas[i].hora)) {
				printf("Horario no disponible.\n");
				return;
			}
			
			FILE *fw = fopen("citas.txt", "w");
			for (int j = 0; j < total; j++) {
				fprintf(fw, "%s;%s;%s;%s;%s;%s;%s\n",
						citas[j].id,
						citas[j].cedulaPaciente,
						citas[j].codigoMedico,
						citas[j].fecha,
						citas[j].hora,
						citas[j].estado,
						citas[j].diagnostico);
			}
			fclose(fw);
			
			printf("Cita reprogramada por administrador.\n");
			return;
		}
	}
	
	printf("Cita no encontrada.\n");
}

void menuPaciente(char cedulaPaciente[]) {
	int op;
	
	do {
		printf("\n--- Seccion: Paciente ---\n");
		printf("1. Agendar cita\n");
		printf("2. Listar mis citas\n");
		printf("3. Cancelar cita\n");
		printf("4. Reprogramar cita\n");
		printf("5. Salir\n");
		printf("Opcion: ");
		
		scanf("%d", &op);
		while (getchar() != '\n');
		
		switch (op) {
		case 1:
			agendarCita(cedulaPaciente);
			break;
		case 2:
			listarHistorialPaciente(cedulaPaciente);
			break;
		case 3:
			cancelarCita(cedulaPaciente);
			break;
		case 4:
			reprogramarCitaPaciente(cedulaPaciente);
			break;
		}
		
	} while (op != 5);
}


void listarHistorialPaciente(const char *cedulaPaciente) {
	FILE *f = fopen("citas.txt", "r");
	Cita c;
	int hay = 0;
	
	if (!f) {
		printf("No hay citas registradas.\n");
		return;
	}
	
	printf("\n-_- Historial de citas asignadas -_-\n");
	
	while (fscanf(f, "%[^;];%[^;];%[^;];%[^;];%[^;];%[^;];%[^\n]\n",
				  c.id,
				  c.cedulaPaciente,
				  c.codigoMedico,
				  c.fecha,
				  c.hora,
				  c.estado,
				  c.diagnostico) == 7) {
		
		if (strcmp(c.cedulaPaciente, cedulaPaciente) == 0) {
			
			hay = 1;
			printf("\nID: %s", c.id);
			printf("\nMedico: %s", c.codigoMedico);
			printf("\nFecha: %s", c.fecha);
			printf("\nHora: %s", c.hora);
			printf("\nEstado: %s", c.estado);
			
			if (strcmp(c.estado, "ATENDIDA") == 0)
				printf("\nDiagnostico: %s", c.diagnostico);
			
			printf("\n");
		}
	}
	
	fclose(f);
	
	if (!hay)
		printf("No tiene citas registradas.\n");
}

void agendarCita(char cedulaPaciente[]) {
	Cita c;
	FILE *f;
	
	strcpy(c.cedulaPaciente, cedulaPaciente);
	
	printf("Codigo del medico: ");
	scanf(" %19s", c.codigoMedico);
	
	if (!medicoExiste(c.codigoMedico)) {
		printf("El medico no existe.\n");
		return;
	}
	
	printf("Fecha (Dia/Mes/Año): ");
	scanf(" %14s", c.fecha);
	
	printf("Hora (Hora:Minuto): ");
	scanf(" %9s", c.hora);
	
	if (!citaDisponible(c.codigoMedico, c.fecha, c.hora)) {
		printf("Horario no disponible.\n");
		return;
	}
	
	generarIdCita(c.id);
	strcpy(c.estado, "ACTIVA");
	strcpy(c.diagnostico, "N/A");
	
	f = fopen("citas.txt", "a");
	if (!f) {
		printf("Error al guardar cita.\n");
		return;
	}
	
	fprintf(f, "%s;%s;%s;%s;%s;%s;%s\n",
			c.id,
			c.cedulaPaciente,
			c.codigoMedico,
			c.fecha,
			c.hora,
			c.estado,
			c.diagnostico);
	
	fclose(f);
	printf("Cita agendada exitosamente. ID: %s\n", c.id);
}

int medicoExiste(char codigo[]) {
	FILE *f = fopen("medicos.txt", "r");
	Medico m;
	
	if (!f) return 0;
	
	while (fscanf(f, "%[^;];%[^;];%[^;];%[^\n]\n",
				  m.codigo,
				  m.nombre,
				  m.especialidad,
				  m.horario) == 4) {
		if (strcmp(m.codigo, codigo) == 0) {
			fclose(f);
			return 1;
		}
	}
	
	fclose(f);
	return 0;
}

int citaDisponible(char codigo[], char fecha[], char hora[]) {
	FILE *f = fopen("citas.txt", "r");
	Cita c;
	
	if (!f) return 1;
	
	while (fscanf(f, "%[^;];%[^;];%[^;];%[^;];%[^;];%[^;];%[^\n]\n",
				  c.id,
				  c.cedulaPaciente,
				  c.codigoMedico,
				  c.fecha,
				  c.hora,
				  c.estado,
				  c.diagnostico) == 7) {
		
		if (strcmp(c.codigoMedico, codigo) == 0 &&
			strcmp(c.fecha, fecha) == 0 &&
			strcmp(c.hora, hora) == 0 &&
			strcmp(c.estado, "ACTIVA") == 0) {
			
			fclose(f);
			return 0;
		}
	}
	
	fclose(f);
	return 1;
}

void generarIdCita(char id[]) {
	FILE *f = fopen("citas.txt", "r");
	int contador = 1;
	char linea[300];
	
	if (f) {
		while (fgets(linea, sizeof(linea), f))
			contador++;
		fclose(f);
	}
	
	sprintf(id, "C%03d", contador);
}

void cancelarCita(char cedula[]) {
	FILE *f = fopen("citas.txt", "r");
	FILE *temp = fopen("temp.txt", "w");
	Cita c;
	char id[10];
	int encontrado = 0;
	
	if (!f || !temp) {
		printf("Error de archivos.\n");
		return;
	}
	
	printf("Ingrese ID de la cita a cancelar: ");
	scanf(" %9s", id);
	
	while (fscanf(f, "%[^;];%[^;];%[^;];%[^;];%[^;];%[^;];%[^\n]\n",
				  c.id,
				  c.cedulaPaciente,
				  c.codigoMedico,
				  c.fecha,
				  c.hora,
				  c.estado,
				  c.diagnostico) == 7) {
		
		if (strcmp(c.id, id) == 0 &&
			strcmp(c.cedulaPaciente, cedula) == 0 &&
			strcmp(c.estado, "ACTIVA") == 0) {
			
			strcpy(c.estado, "CANCELADA");
			encontrado = 1;
		}
		
		fprintf(temp, "%s;%s;%s;%s;%s;%s;%s\n",
				c.id,
				c.cedulaPaciente,
				c.codigoMedico,
				c.fecha,
				c.hora,
				c.estado,
				c.diagnostico);
	}
	
	fclose(f);
	fclose(temp);
	remove("citas.txt");
	rename("temp.txt", "citas.txt");
	
	if (encontrado)
		printf("Cita cancelada correctamente.\n");
	else
		printf("No se pudo cancelar la cita.\n");
}

void reprogramarCitaPaciente(char cedulaPaciente[]) {
	Cita citas[200];
	int total = 0;
	char idBuscar[10];
	FILE *f = fopen("citas.txt", "r");
	
	if (!f) {
		printf("No hay citas registradas.\n");
		return;
	}
	
	while (fscanf(f, "%[^;];%[^;];%[^;];%[^;];%[^;];%[^;];%[^\n]\n",
				  citas[total].id,
				  citas[total].cedulaPaciente,
				  citas[total].codigoMedico,
				  citas[total].fecha,
				  citas[total].hora,
				  citas[total].estado,
				  citas[total].diagnostico) == 7) {
		total++;
	}
	fclose(f);
	
	printf("Ingrese ID de la cita a reprogramar: ");
	fgets(idBuscar, sizeof(idBuscar), stdin);
	idBuscar[strcspn(idBuscar, "\n")] = '\0';
	
	for (int i = 0; i < total; i++) {
		if (strcmp(citas[i].id, idBuscar) == 0 &&
			strcmp(citas[i].cedulaPaciente, cedulaPaciente) == 0 &&
			strcmp(citas[i].estado, "ACTIVA") == 0) {
			
			printf("Nueva fecha (Año-Mes-Dia): ");
			fgets(citas[i].fecha, sizeof(citas[i].fecha), stdin);
			citas[i].fecha[strcspn(citas[i].fecha, "\n")] = '\0';
			
			printf("Nueva hora (Hora:Minuto): ");
			fgets(citas[i].hora, sizeof(citas[i].hora), stdin);
			citas[i].hora[strcspn(citas[i].hora, "\n")] = '\0';
			
			if (!medicoDisponible(citas[i].codigoMedico, citas[i].fecha, citas[i].hora)) {
				printf("El medico no esta disponible en ese horario.\n");
				return;
			}
			
			FILE *fw = fopen("citas.txt", "w");
			for (int j = 0; j < total; j++) {
				fprintf(fw, "%s;%s;%s;%s;%s;%s;%s\n",
						citas[j].id,
						citas[j].cedulaPaciente,
						citas[j].codigoMedico,
						citas[j].fecha,
						citas[j].hora,
						citas[j].estado,
						citas[j].diagnostico);
			}
			fclose(fw);
			
			printf("Cita reprogramada correctamente.\n");
			return;
		}
	}
	
	printf("Cita no encontrada o no permitida.\n");
}

//Funciones solo de medico
void menuMedico(char codigoMedico[]) {
	Cita citas[MAX];
	int total = cargarCitas(citas);
	int op;
	
	do {
		printf("\n-- Gestion de tareas (Medico) --\n");
		printf("1. Ver mis citas activas\n");
		printf("2. Ver mis citas atendidas\n");
		printf("3. Cerrar cita\n");
		printf("4. Salir\n");
		printf("Opcion: ");
		scanf("%d", &op);
		
		switch (op) {
		case 1:
			listarCitasMedico(codigoMedico);
			break;
		case 2:
			listarCitasAtendidasMedico(codigoMedico);
			break;
		case 3:
		cerrarCitaMedica(codigoMedico);
			break;
		}
	} while (op != 4);
}

void listarCitasMedico(const char *codigoMedico) {
	FILE *f = fopen("citas.txt", "r");
	Cita c;
	int encontro = 0;
	
	if (!f) {
		printf("No existen citas registradas.\n");
		return;
	}
	
	printf("\n-- Citas (Listado personal) --\n");
	
	while (fscanf(f, "%[^;];%[^;];%[^;];%[^;];%[^;];%[^;];%[^\n]\n",
				  c.id,
				  c.cedulaPaciente,
				  c.codigoMedico,
				  c.fecha,
				  c.hora,
				  c.estado,
				  c.diagnostico) == 7) {
		
		if (strcmp(c.codigoMedico, codigoMedico) == 0 &&
			strcmp(c.estado, "ACTIVA") == 0) {
			
			encontro = 1;
			
			printf("\nCita ID: %s\n", c.id);
			printf("Paciente: %s\n", c.cedulaPaciente);
			printf("Fecha: %s\n", c.fecha);
			printf("Hora: %s\n", c.hora);
		}
	}
	
	fclose(f);
	
	if (!encontro) {
		printf("No tiene citas asignadas.\n");
	}
}

void listarCitasAtendidasMedico(const char *codigoMedico) {
	FILE *f = fopen("citas.txt", "r");
	Cita c;
	int hay = 0;
	
	if (!f) {
		printf("No hay citas registradas.\n");
		return;
	}
	
	printf("\n-- CITAS ATENDIDAS --\n");
	
	while (fscanf(f, "%[^;];%[^;];%[^;];%[^;];%[^;];%[^;];%[^\n]\n",
				  c.id,
				  c.cedulaPaciente,
				  c.codigoMedico,
				  c.fecha,
				  c.hora,
				  c.estado,
				  c.diagnostico) == 7) {
		
		if (strcmp(c.codigoMedico, codigoMedico) == 0 &&
			strcmp(c.estado, "ATENDIDA") == 0) {
			
			hay = 1;
			printf("\nID: %s", c.id);
			printf("\nPaciente: %s", c.cedulaPaciente);
			printf("\nFecha: %s", c.fecha);
			printf("\nHora: %s", c.hora);
			printf("\nDiagnostico: %s\n", c.diagnostico);
		}
	}
	
	fclose(f);
	
	if (!hay)
		printf("No hay citas atendidas.\n");
}

void cerrarCitaMedica(const char *codigoMedico) {
	FILE *f = fopen("citas.txt", "r");
	FILE *temp = fopen("temp.txt", "w");
	Cita c;
	char id[10];
	int opcion, encontrada = 0;
	
	if (!f || !temp) {
		printf("Error al abrir archivos.\n");
		return;
	}
	
	listarCitasMedico(codigoMedico);
	
	printf("\nIngrese ID de la cita: ");
	scanf(" %[^\n]", id);
	
	printf("\nEstado de la cita:\n");
	printf("1. Atendida\n");
	printf("2. Paciente no asistio\n");
	printf("3. Medico no pudo atender\n");
	printf("Opcion: ");
	scanf("%d", &opcion);
	
	while (fscanf(f, "%[^;];%[^;];%[^;];%[^;];%[^;];%[^;];%[^\n]\n",
				  c.id,
				  c.cedulaPaciente,
				  c.codigoMedico,
				  c.fecha,
				  c.hora,
				  c.estado,
				  c.diagnostico) == 7) {
		
		if (strcmp(c.id, id) == 0 &&
			strcmp(c.codigoMedico, codigoMedico) == 0 &&
			strcmp(c.estado, "ACTIVA") == 0) {
			
			encontrada = 1;
			
			if (opcion == 1) {
				strcpy(c.estado, "ATENDIDA");
				printf("Ingrese diagnostico: ");
				scanf(" %[^\n]", c.diagnostico);
			}
			else if (opcion == 2) {
				strcpy(c.estado, "NO_ASISTIO");
				strcpy(c.diagnostico, "");
			}
			else if (opcion == 3) {
				strcpy(c.estado, "MEDICO_AUSENTE");
				strcpy(c.diagnostico, "");
			}
		}
		
		fprintf(temp, "%s;%s;%s;%s;%s;%s;%s\n",
				c.id,
				c.cedulaPaciente,
				c.codigoMedico,
				c.fecha,
				c.hora,
				c.estado,
				c.diagnostico);
	}
	
	fclose(f);
	fclose(temp);
	remove("citas.txt");
	rename("temp.txt", "citas.txt");
	
	if (encontrada)
		printf("Cita cerrada correctamente.\n");
	else
		printf("No se pudo cerrar la cita.\n");
}

int medicoDisponible(const char *codigoMedico, const char *fecha, const char *hora) {
	FILE *f = fopen("citas.txt", "r");
	Cita c;
	
	if (!f) return 1;
	
	while (fscanf(f, "%[^;];%[^;];%[^;];%[^;];%[^;];%[^;];%[^\n]\n",
				  c.id, c.cedulaPaciente, c.codigoMedico,
				  c.fecha, c.hora, c.estado, c.diagnostico) == 7) {
		
		if (strcmp(c.codigoMedico, codigoMedico) == 0 &&
			strcmp(c.fecha, fecha) == 0 &&
			strcmp(c.hora, hora) == 0 &&
			strcmp(c.estado, "ACTIVA") == 0) {
			
			fclose(f);
			//En caso de que este ocupado
			return 0;
		}
	}
	
	fclose(f);
	//En caso de que este disponible
	return 1;
}

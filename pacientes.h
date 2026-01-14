#ifndef PACIENTES_H
#define PACIENTES_H

typedef struct {
	char nombre[50];
	char cedula[20];
	int edad;
	char telefono[20];
	char correo[40];
} Paciente;

//Funciones para el funcionamiento del modulo pacientes
int esNumeroEnteroPositivo(const char *s);

void menuPacientes(void);

void cargarPacientes(Paciente pacientes[], int *total);
void guardarPacientes(Paciente pacientes[], int total);

void registrarPaciente(Paciente pacientes[], int *total);
void consultarPacientes(Paciente pacientes[], int total);

#endif

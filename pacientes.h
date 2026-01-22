#ifndef PACIENTES_H
#define PACIENTES_H

// --- CABECERA DE GESTION DE PACIENTES ---

typedef struct {
	char nombre[50];    
	char cedula[20];    
	int edad;           
	char telefono[20];  
	char correo[50];   
} Paciente;

// FUNCIONES

// Lee el archivo "pacientes.txt" y carga los datos en la memoria para usarlos
void cargarPacientes(Paciente pacientes[], int *total);

// Guarda todos los cambios de la memoria de vuelta al archivo de texto
void guardarPacientes(Paciente pacientes[], int total);

// Pide los datos personales y agrega un paciente nuevo a la lista
void registrarPaciente(Paciente pacientes[], int *total);

// Muestra en pantalla una lista ordenada de todos los pacientes
int consultarPacientes(Paciente pacientes[], int total);

// Permite al administrador borrar a un paciente usando su numero de cedula
void eliminarPacienteAdmin(void);

// Muestra el menu con las opciones (Registrar, Listar, Eliminar, Volver)
void menuPacientes(void);

#endif

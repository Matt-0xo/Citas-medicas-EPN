#ifndef MEDICOS_H
#define MEDICOS_H

// --- CABECERA DE GESTION DE MEDICOS ---
typedef struct {
	char codigo[20];     
	char nombre[50];      
	char especialidad[40]; 
	char horario[40];     
} Medico;

//FUNCIONES

// Lee el archivo de texto y carga los medicos en la memoria del programa
void cargarMedicos(Medico medicos[], int *total);

// Guarda la lista de medicos de la memoria al archivo de texto (para no perder datos)
void guardarMedicos(Medico medicos[], int total);

// Pide los datos al usuario y agrega un nuevo medico a la lista
void registrarMedico(Medico medicos[], int *total);

// Muestra en pantalla una tabla con todos los medicos registrados
int consultarMedicos(Medico medicos[], int total);

// Permite al administrador borrar a un medico por su codigo
void eliminarMedicoAdmin(void);

// Funcion especifica para cambiar solo el horario de atencion de un doctor
void modificarHorarioMedico(const char *codigoMedico);

// Muestra el menu con todas estas opciones (Registrar, Listar, Eliminar, Modificar)
void menuMedicos(void); 

#endif

#ifndef CITAS_H
#define CITAS_H

typedef struct {
	char id[10];
	char cedulaPaciente[20];
	char codigoMedico[20];
	char fecha[15];
	char hora[10];
	char estado[15];
	char diagnostico[200];
} Cita;

//Menu por nivel de usuario
void menuAdminCitas(void);
void menuPaciente(char cedula[]);
void menuMedico(char codigoMedico[]);

//Menu de funciones de citas
void menuPaciente(char cedulaPaciente[]);
void agendarCita(char cedulaPaciente[]);
void listarCitasPaciente(char cedulaPaciente[]);
void listarHistorialPaciente(const char *cedulaPaciente);
void listarCitasAtendidasMedico(const char *codigoMedico);
void cancelarCita(char cedulaPaciente[]);

//Validaciones para las citas
int medicoExiste(char codigoMedico[]);
int horarioValido(char hora[]);
int medicoDisponible(const char *codigoMedico, const char *fecha, const char *hora);

#endif

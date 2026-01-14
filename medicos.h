#ifndef MEDICOS_H
#define MEDICOS_H

#include <stdio.h>

//Estructura para el registro de medicos
typedef struct {
	char codigo[20];
	char nombre[50];
	char especialidad[40];
	char horario[40];
} Medico;

//Validaciones
int validarHorario(const char *h);

//Funciones de spolo ejecucion
void cargarMedicos(Medico medicos[], int *total);
void guardarMedicos(Medico medicos[], int total);

void registrarMedico(Medico medicos[], int *total);
void consultarMedicos(Medico medicos[], int total);

//Menu de la seccion de medicos
void menuMedicos(void);

#endif

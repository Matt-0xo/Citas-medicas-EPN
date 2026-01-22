#ifndef CITAS_H
#define CITAS_H

// --- CABECERA DE CITAS MEDICAS ---
// Este archivo contiene las funciones publicas para gestionar las citas.
// Permite que el sistema sepa que existen menus para cada tipo de usuario.

// Muestra el menu exclusivo para pacientes (Ver historial, Agendar, Cancelar)
// Recibe la cedula del paciente (idRef) para mostrar solo sus datos.
void menuPaciente(char idRef[]);

// Muestra el menu exclusivo para medicos (Ver agenda, Atender pacientes)
// Recibe el codigo del medico (idRef) para mostrar solo sus citas.
void menuMedico(char idRef[]);

// Muestra el menu de gestion de citas para el Administrador (Ver todas, Cancelar cualquiera)
void menuAdminCitas(void);

#endif

#ifndef USUARIOS_H
#define USUARIOS_H

// --- CABECERA DE USUARIOS Y SEGURIDAD ---
// Aqui definimos como son las cuentas para entrar al sistema.
// Definimos la estructura de la "Cuenta".
typedef struct {
	char usuario[30];  
	char clave[30];    
	char rol[15];      
	char idRef[20];   
} Usuario;

//FUNCIONES 

// Funcion para pedir usuario y clave, y verificar si son correctos
int login(Usuario *usuarioLogueado);

// Guia paso a paso para crear una cuenta nueva y vincularla con un Medico o Paciente
void registrarUsuario(void);

// Revisa si el nombre de usuario ya esta ocupado para evitar duplicados
int existeUsuario(const char *nombre);

#endif

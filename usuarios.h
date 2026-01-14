#ifndef USUARIOS_H
#define USUARIOS_H

typedef struct {
	char usuario[30];
	char clave[30];
	char rol[15];
	char idRef[20];
} Usuario;

int login(Usuario *usuarioLogueado);
void registrarUsuario(void);
int existeUsuario(const char *nombre);

#endif

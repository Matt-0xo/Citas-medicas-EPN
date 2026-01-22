#ifndef INTERFAZ_H
#define INTERFAZ_H

// --- CABECERA DE LA INTERFAZ ---
// Aqui avisamos al programa que existen funciones para manejar la pantalla (colores, limpieza, etc.)

// Configura los colores (Fondo Blanco, Letras Negras) al iniciar
void configurarConsola();

// Borra todo el texto de la pantalla para que se vea limpio
void limpiarPantalla();

// Detiene el programa hasta que el usuario presione Enter
void pausa();

// Muestra el titulo, la fecha y la hora en la parte superior de cada menu
void imprimirEncabezado(const char *titulo);

// Dibuja una linea de guiones para separar secciones y que se vea ordenado
void imprimirLinea();

#endif


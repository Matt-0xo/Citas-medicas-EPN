#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>
#include <ctype.h>
#include "citas.h"
#include "medicos.h" 
#include "interfaz.h"

// Definimos una estructura temporal para poder leer los datos del archivo.
// Es como una "caja" donde guardamos la informacion de cada linea que leemos.
typedef struct {
	char codigo[10];
	char cedulaPaciente[20];
	char codigoMedico[20];
	char fecha[15];
	char hora[10];
	int estado; // Aqui usamos numeros: 0=Cancelada, 1=Activa, 2=Atendida
	char diagnostico[300]; 
} CitaTemp;

// --- SECCION DE VALIDACIONES (Para que no metan datos erroneos) ---

// Esta funcion revisa si la fecha que ingreso el usuario ya paso o es valida.
// Devuelve 1 si es una fecha futura (o de hoy), y 0 si es una fecha pasada.
int esFechaFutura(const char *fechaStr) {
	int dia, mes, anio;
	// Intentamos leer el dia, mes y anio del texto
	if (sscanf(fechaStr, "%d/%d/%d", &dia, &mes, &anio) != 3) return 0;
	
	// Obtenemos la fecha actual del reloj de la computadora
	time_t t = time(NULL);
	struct tm *fechaActual = localtime(&t);
	
	// Ajustamos los valores porque la computadora cuenta los años desde 1900
	int anioActual = fechaActual->tm_year + 1900;
	int mesActual = fechaActual->tm_mon + 1;
	int diaActual = fechaActual->tm_mday;
	
	// Comparamos el anio: si es menor al actual, es fecha pasada (invalida)
	if (anio < anioActual) return 0;
	
	// Si es el mismo anio pero el mes ya paso, es invalida
	if (anio == anioActual && mes < mesActual) return 0;
	
	// Si es el mismo anio y mes, pero el dia ya paso, es invalida
	if (anio == anioActual && mes == mesActual && dia < diaActual) return 0;
	
	// Si paso todas las pruebas, la fecha es valida
	return 1;
}

// Verifica que la hora tenga formato correcto (ej: 14:30) y sentido comun.
int esHoraValida(const char *horaStr) {
	int h, m;
	// Leemos dos numeros separados por dos puntos
	if (sscanf(horaStr, "%d:%d", &h, &m) != 2) return 0; 
	// Revisamos que las horas sean de 0-23 y los minutos de 0-59
	if (h < 0 || h > 23 || m < 0 || m > 59) return 0; 
	return 1;
}

// Esta funcion es importante: revisa si la hora de la cita cae dentro del turno del medico.
// Por ejemplo, si el medico trabaja de 08:00 a 12:00 y la cita es a las 15:00, dira que NO.
int estaEnHorarioDeAtencion(const char *rangoMedico, const char *horaCita) {
	int hInicio, mInicio, hFin, mFin, hCita, mCita;
	
	// Leemos el horario del medico (ej: "08:00-16:00")
	sscanf(rangoMedico, "%d:%d-%d:%d", &hInicio, &mInicio, &hFin, &mFin);
	// Leemos la hora de la cita deseada
	sscanf(horaCita, "%d:%d", &hCita, &mCita);
	
	// Truco matematico: Convertimos todo a minutos desde el inicio del dia.
	// Asi es mas facil comparar numeros enteros simples.
	int minInicio = hInicio * 60 + mInicio;
	int minFin = hFin * 60 + mFin;
	int minCita = hCita * 60 + mCita;
	
	// Verificamos si los minutos de la cita estan entre el inicio y el fin del turno
	return (minCita >= minInicio && minCita <= minFin);
}

// ESTA ES LA FUNCION CLAVE PARA EVITAR ERRORES:
// Revisa todo el archivo para ver si el medico ya tiene ocupada esa hora exacta.
int existeChoqueHorario(const char *codMed, const char *fecha, const char *hora) {
	// Abrimos el archivo en modo lectura
	FILE *f = fopen("citas.txt", "r");
	
	// Si no existe el archivo (ej. es la primera cita de todas), no hay choque
	if (!f) return 0; 
	
	char linea[500];
	CitaTemp c;
	
	// Leemos el archivo linea por linea
	while (fgets(linea, sizeof(linea), f)) {
		// Extraemos los datos de la linea actual
		if (sscanf(linea, "%[^;];%[^;];%[^;];%[^;];%[^;];%d;%[^\n]", 
				   c.codigo, c.cedulaPaciente, c.codigoMedico, c.fecha, c.hora, &c.estado, c.diagnostico) == 7) {
			
			// Limpiamos caracteres invisibles que a veces quedan al final (como el Enter)
			c.codigoMedico[strcspn(c.codigoMedico, "\r")] = '\0';
			c.codigoMedico[strcspn(c.codigoMedico, "\n")] = '\0';
			
			// Comparamos: ¿Es el mismo medico? Y ¿Es la misma fecha? Y ¿Es la misma hora?
			if (strcmp(c.codigoMedico, codMed) == 0 && 
				strcmp(c.fecha, fecha) == 0 && 
				strcmp(c.hora, hora) == 0 && 
				c.estado == 1) { // Importante: Solo nos importa si la cita esta ACTIVA (1)
				
				// Si todo coincide, encontramos un choque. Cerramos y devolvemos "Verdadero" (1)
				fclose(f);
				return 1; 
			}
		}
	}
	// Si terminamos de leer todo y no encontramos nada igual, no hay choque
	fclose(f);
	return 0; 
}

// Busca en el archivo de medicos cual es el horario de un doctor especifico
void obtenerHorarioMedico(const char *codMedico, char *bufferHorario) {
	FILE *f = fopen("medicos.txt", "r");
	char cod[20], nom[50], esp[40], hor[40];
	
	// Ponemos un valor por defecto por si no encontramos al medico
	strcpy(bufferHorario, "00:00-23:59"); 
	
	if(!f) return;
	
	// Buscamos al medico por su codigo
	while(fscanf(f, "%[^;];%[^;];%[^;];%[^\n]\n", cod, nom, esp, hor) == 4) {
		if(strcmp(cod, codMedico) == 0) {
			// Si lo encontramos, copiamos su horario al buffer y terminamos
			strcpy(bufferHorario, hor);
			break;
		}
	}
	fclose(f);
}

// Convierte el numero de estado (ej: 1) a una palabra legible (ej: "ACTIVA")
void obtenerEstadoStr(int estado, char *buffer) {
	if(estado == 1) strcpy(buffer, "ACTIVA");
	else if(estado == 2) strcpy(buffer, "ATENDIDA");
	else strcpy(buffer, "CANCELADA");
}

// --- FUNCIONES PARA MOSTRAR DATOS EN PANTALLA ---

// Muestra la lista de citas. Sirve para el Paciente (ve solo las suyas) o el Admin (ve todas).
// Si 'filtroPaciente' tiene una cedula, filtra. Si es NULL, muestra todo.
int listarCitasGeneral(const char *filtroPaciente) {
	FILE *f = fopen("citas.txt", "r");
	CitaTemp c;
	char estadoStr[20];
	char linea[500];
	int encontrados = 0;
	
	imprimirEncabezado("HISTORIAL DE CITAS");
	printf("  %-8s | %-12s | %-12s | %-8s | %-10s\n", "CODIGO", "MEDICO", "FECHA", "HORA", "ESTADO");
	imprimirLinea();
	
	if (f) {
		// Leemos linea por linea
		while (fgets(linea, sizeof(linea), f)) {
			// Parseamos la linea
			if (sscanf(linea, "%[^;];%[^;];%[^;];%[^;];%[^;];%d;%[^\n]", 
					   c.codigo, c.cedulaPaciente, c.codigoMedico, c.fecha, c.hora, &c.estado, c.diagnostico) == 7) {
				
				// Limpiamos basura del string
				c.cedulaPaciente[strcspn(c.cedulaPaciente, "\r")] = '\0';
				c.cedulaPaciente[strcspn(c.cedulaPaciente, "\n")] = '\0';
				
				// APLICAMOS EL FILTRO:
				// Mostramos la cita SI (no hay filtro) O SI (la cedula coincide con la del filtro)
				if (filtroPaciente == NULL || strcmp(c.cedulaPaciente, filtroPaciente) == 0) {
					obtenerEstadoStr(c.estado, estadoStr);
					printf("  %-8s | %-12s | %-12s | %-8s | %-10s\n", 
						   c.codigo, c.codigoMedico, c.fecha, c.hora, estadoStr);
					printf("    [DIAGNOSTICO]: %s\n", c.diagnostico);
					printf("  ------------------------------------------------------------\n");
					encontrados++;
				}
			}
		}
		fclose(f);
	}
	if (encontrados == 0) printf("\n    [INFO] No hay registros para mostrar.\n");
	return encontrados;
}

// Muestra solo las citas que le tocan a un medico especifico
int mostrarTablaMedico(const char *codMed) {
	FILE *f = fopen("citas.txt", "r");
	CitaTemp c;
	char linea[500];
	char estadoStr[20];
	int encontrados = 0;
	
	imprimirEncabezado("MI AGENDA MEDICA");
	printf("  %-8s | %-12s | %-10s | %-8s | %-10s\n", "CODIGO", "PACIENTE", "FECHA", "HORA", "ESTADO");
	imprimirLinea();
	
	if(f){
		while(fgets(linea, sizeof(linea), f)){
			// Leemos y descomponemos la linea
			if(sscanf(linea, "%[^;];%[^;];%[^;];%[^;];%[^;];%d;%[^\n]", c.codigo, c.cedulaPaciente, c.codigoMedico, c.fecha, c.hora, &c.estado, c.diagnostico) == 7){
				c.codigoMedico[strcspn(c.codigoMedico, "\r")] = '\0';
				c.codigoMedico[strcspn(c.codigoMedico, "\n")] = '\0';
				
				// Verificamos si esta cita le pertenece a este medico
				if(strcmp(c.codigoMedico, codMed)==0){
					obtenerEstadoStr(c.estado, estadoStr);
					printf("  %-8s | %-12s | %-10s | %-8s | %s\n", c.codigo, c.cedulaPaciente, c.fecha, c.hora, estadoStr);
					encontrados++;
				}
			}
		}
		fclose(f);
	}
	if(encontrados == 0) printf("\n    [INFO] No tiene citas asignadas.\n");
	imprimirLinea();
	return encontrados;
}

void verCitasDeMedico(const char *codMed) {
	mostrarTablaMedico(codMed);
	pausa();
}

// Esta funcion permite al medico "Atender" al paciente y escribir su diagnostico
void atenderCitaMedico(const char *codMed) {
	// Primero mostramos la lista para que vea los codigos
	if (mostrarTablaMedico(codMed) == 0) {
		pausa();
		return;
	}
	
	// Abrimos el archivo original y creamos uno temporal para guardar los cambios
	FILE *f = fopen("citas.txt", "r");
	FILE *temp = fopen("temp.txt", "w");
	CitaTemp c; 
	char linea[500]; 
	char diag[300]; 
	char codCita[20];
	int ok = 0;
	int foundInFile = 0;
	
	if(!f || !temp) { printf("Error archivo.\n"); return; }
	
	printf("\n  >> Ingrese Codigo de cita a atender (0 para volver): "); 
	scanf(" %[^\n]", codCita);
	while(getchar()!='\n'); 
	
	// Si el usuario pone 0, cancelamos todo y borramos el temporal
	if (strcmp(codCita, "0") == 0) {
		fclose(f); fclose(temp); remove("temp.txt");
		return;
	}
	
	// Recorremos el archivo original
	while(fgets(linea, sizeof(linea), f)){
		sscanf(linea, "%[^;];%[^;];%[^;];%[^;];%[^;];%d;%[^\n]", c.codigo, c.cedulaPaciente, c.codigoMedico, c.fecha, c.hora, &c.estado, c.diagnostico);
		c.codigoMedico[strcspn(c.codigoMedico, "\r")] = '\0';
		c.codigoMedico[strcspn(c.codigoMedico, "\n")] = '\0';
		
		// Si encontramos la cita que indico el medico
		if(strcmp(c.codigo, codCita) == 0) {
			foundInFile = 1;
			// Validamos seguridad: ¿Esta cita es realmente de este medico?
			if(strcmp(c.codigoMedico, codMed) == 0) {
				
				// Si la cita esta ACTIVA (Pendiente), procedemos a atender
				if (c.estado == 1) {
					printf("\n    --- ATENDIENDO PACIENTE %s ---\n", c.cedulaPaciente);
					printf("    Escriba Diagnostico [0 para cancelar]: ");
					scanf(" %[^\n]", diag);
					while(getchar()!='\n'); 
					
					if (strcmp(diag, "0") != 0) {
						// Actualizamos el diagnostico y cambiamos el estado a ATENDIDA (2)
						strcpy(c.diagnostico, diag);
						c.estado = 2; 
						ok = 1;
						printf("\n    [EXITO] Cita atendida y diagnostico guardado.\n");
					} else {
						printf("\n    [INFO] Operacion cancelada.\n");
					}
				} 
				// Si ya fue atendida, damos opcion de corregir el diagnostico
				else if (c.estado == 2) {
					printf("\n    [INFO] Esta cita YA FUE ATENDIDA.\n");
					printf("    Diagnostico actual: %s\n", c.diagnostico);
					printf("    >> Desea editar el diagnostico? (1: Si / 0: No): ");
					int opEdit = 0;
					if(scanf("%d", &opEdit)!=1) opEdit=0;
					while(getchar()!='\n'); 
					
					if(opEdit == 1) {
						printf("    Nuevo Diagnostico [0 para cancelar]: ");
						scanf(" %[^\n]", diag);
						while(getchar()!='\n'); 
						if (strcmp(diag, "0") != 0) {
							strcpy(c.diagnostico, diag);
							ok = 1;
							printf("\n    [EXITO] Diagnostico actualizado.\n");
						} else {
							printf("\n    [INFO] Edicion cancelada.\n");
						}
					}
				}
				else { printf("\n    [ERROR] No puede atender una cita cancelada.\n"); }
			} 
		}
		// Escribimos la linea (sea la modificada o la original) en el archivo temporal
		fprintf(temp, "%s;%s;%s;%s;%s;%d;%s\n", c.codigo, c.cedulaPaciente, c.codigoMedico, c.fecha, c.hora, c.estado, c.diagnostico);
	}
	fclose(f); fclose(temp);
	
	// Si todo salio bien, el archivo temporal se convierte en el oficial
	if(ok){ remove("citas.txt"); rename("temp.txt", "citas.txt"); }
	else { 
		// Si fallo o cancelo, borramos el temporal
		remove("temp.txt"); 
		if (!foundInFile) printf("\n    [ERROR] El codigo '%s' no existe en la base de datos.\n", codCita);
		else if (foundInFile && !ok) printf("\n    [INFO] No se realizaron cambios.\n");
	}
	pausa();
}

// Esta funcion es la mas compleja: Sirve tanto para CANCELAR (accion 1) como para REAGENDAR (accion 2)
void modificarCita(const char *codigoCita, int accion, const char *usuarioSolicitante, int esAdmin) {
	if (strcmp(codigoCita, "0") == 0) return;
	
	// Tecnica de "Archivo Temporal" para editar
	FILE *f = fopen("citas.txt", "r");
	FILE *temp = fopen("temp_citas.txt", "w");
	CitaTemp c;
	char linea[500];
	
	// Variables para controlar que paso durante el proceso
	int codigoExiste = 0;
	int tienePermiso = 0;
	int operacionExitosa = 0;
	int operacionAbortada = 0;
	int yaEstabaCancelada = 0;
	int yaFueAtendida = 0;
	
	if (!f || !temp) { printf("\n    [ERROR] Error de archivo.\n"); pausa(); return; }
	
	// Leemos linea por linea
	while (fgets(linea, sizeof(linea), f)) {
		if (sscanf(linea, "%[^;];%[^;];%[^;];%[^;];%[^;];%d;%[^\n]", 
				   c.codigo, c.cedulaPaciente, c.codigoMedico, c.fecha, c.hora, &c.estado, c.diagnostico) == 7) {
			
			// Limpiamos basura
			c.cedulaPaciente[strcspn(c.cedulaPaciente, "\r")] = '\0'; 
			c.cedulaPaciente[strcspn(c.cedulaPaciente, "\n")] = '\0';
			c.codigoMedico[strcspn(c.codigoMedico, "\r")] = '\0'; 
			c.codigoMedico[strcspn(c.codigoMedico, "\n")] = '\0';
			
			// Si encontramos la cita por su codigo
			if (strcmp(c.codigo, codigoCita) == 0) {
				codigoExiste = 1;
				// Seguridad: Solo el dueño de la cita o el Admin pueden tocarla
				if (esAdmin || strcmp(c.cedulaPaciente, usuarioSolicitante) == 0) {
					tienePermiso = 1;
					
					// CASO 1: CANCELAR
					if (accion == 1) { 
						if(c.estado == 0) yaEstabaCancelada = 1; // Ya estaba cancelada
						else if (c.estado == 2) yaFueAtendida = 1; // Ya la atendieron, no se puede cancelar
						else { 
							c.estado = 0; // Cambiamos estado a 0 (Cancelada)
							operacionExitosa = 1;
						}
					} 
					// CASO 2: REAGENDAR (Cambiar fecha/hora)
					else if (accion == 2) { 
						if(c.estado == 0) yaEstabaCancelada = 1;
						else if (c.estado == 2) yaFueAtendida = 1;
						else {
							// Buscamos el horario del medico de esta cita para validar
							char horarioDoc[40];
							obtenerHorarioMedico(c.codigoMedico, horarioDoc);
							
							imprimirEncabezado("REAGENDAR CITA");
							printf("    Cita Actual: %s %s\n", c.fecha, c.hora);
							printf("    Horario del medico: %s\n", horarioDoc);
							imprimirLinea();
							
							// --- Pedimos NUEVA FECHA ---
							int valid=0;
							char nuevaFecha[15];
							do {
								printf("    Nueva Fecha (DD/MM/AAAA) [0 para volver]: "); 
								scanf(" %[^\n]", nuevaFecha);
								if (strcmp(nuevaFecha, "0") == 0) { operacionAbortada = 1; break; }
								
								// Validamos que sea fecha futura
								if(esFechaFutura(nuevaFecha)) {
									valid=1;
									strcpy(c.fecha, nuevaFecha); // Actualizamos la fecha en memoria
								} else printf("    [!] Fecha invalida o pasada.\n");
							} while(!valid);
							
							// --- Pedimos NUEVA HORA (Solo si no cancelo antes) ---
							if (!operacionAbortada) {
								valid=0;
								char nuevaHora[10];
								do {
									printf("    Nueva Hora (HH:MM) [0 para volver]: "); 
									scanf(" %[^\n]", nuevaHora);
									if (strcmp(nuevaHora, "0") == 0) { operacionAbortada = 1; break; }
									
									// Validamos 3 cosas: Formato, Turno del Medico y Si ya esta ocupado
									if(!esHoraValida(nuevaHora)) {
										printf("    [!] Formato de hora invalido.\n");
									} else if (!estaEnHorarioDeAtencion(horarioDoc, nuevaHora)) {
										printf("    [!] Hora fuera del turno del medico (%s).\n", horarioDoc);
									} else if (existeChoqueHorario(c.codigoMedico, c.fecha, nuevaHora)) {
										printf("    [!] EL MEDICO YA TIENE UNA CITA A ESA HORA.\n");
									} else {
										valid=1;
										strcpy(c.hora, nuevaHora); // Actualizamos la hora en memoria
									}
								} while(!valid);
							}
							
							if (!operacionAbortada) {
								while(getchar()!='\n'); 
								operacionExitosa = 1;
							}
						}
					}
				}
			}
			// Guardamos la cita (modificada o no) en el archivo nuevo
			fprintf(temp, "%s;%s;%s;%s;%s;%d;%s\n", c.codigo, c.cedulaPaciente, c.codigoMedico, c.fecha, c.hora, c.estado, c.diagnostico);
		}
	}
	fclose(f); fclose(temp);
	
	// Finalizamos el proceso de reemplazo de archivos
	if (operacionExitosa) {
		remove("citas.txt"); 
		rename("temp_citas.txt", "citas.txt"); 
		if (accion == 1) printf("\n    [EXITO] La cita ha sido CANCELADA correctamente.\n");
		if (accion == 2) printf("\n    [EXITO] La cita ha sido REAGENDADA correctamente.\n");
	} 
	else { 
		// Si algo fallo, borramos el temporal
		remove("temp_citas.txt"); 
		if (operacionAbortada) printf("\n    [INFO] Operacion cancelada por el usuario.\n");
		else if (!codigoExiste) printf("\n    [ERROR] El codigo '%s' NO EXISTE.\n", codigoCita);
		else if (!tienePermiso) printf("\n    [ERROR] No tiene permiso para modificar esta cita.\n");
		else if (yaEstabaCancelada) {
			if (accion == 1) printf("\n    [AVISO] Esta cita YA ESTABA cancelada.\n");
			else printf("\n    [ERROR] No se puede reagendar una cita cancelada.\n");
		}
		else if (yaFueAtendida) printf("\n    [ERROR] No se puede cancelar/reagendar una cita ATENDIDA.\n");
	}
	pausa(); 
}

// Proceso principal donde el paciente crea una cita desde cero
void agendarCitaPaciente(const char *cedula) {
	FILE *fm = fopen("medicos.txt", "r");
	// Si no hay medicos, no podemos agendar nada
	if (!fm) {
		printf("\n    [INFO] No hay medicos registrados.\n");
		pausa(); return;
	}
	
	// Abrimos citas en modo 'a' (append) para agregar al final sin borrar lo anterior
	FILE *fc = fopen("citas.txt", "a"); 
	if (!fc) {
		printf("\n    [ERROR] Error al abrir citas.txt.\n");
		fclose(fm); pausa(); return;
	}
	
	char cod[20], nom[50], esp[40], hor[40];
	char selCod[20], selHor[40], nombreMedicoSel[50] = "Desconocido"; 
	int op, i=0, found=0;
	CitaTemp nc;
	
	imprimirEncabezado("SELECCION DE MEDICO");
	printf("  %-3s | %-25s | %-15s | %-12s\n", "#", "NOMBRE", "ESPECIALIDAD", "HORARIO");
	imprimirLinea();
	
	// Paso 1: Mostrar lista numerada de medicos para elegir
	while (fscanf(fm, "%[^;];%[^;];%[^;];%[^\n]\n", cod, nom, esp, hor) == 4) {
		printf("  %-3d | %-25s | %-15s | %-12s\n", ++i, nom, esp, hor);
	}
	rewind(fm); imprimirLinea(); // Regresamos el lector al inicio del archivo
	
	if (i == 0) { printf("\n    [INFO] No hay medicos disponibles.\n"); fclose(fm); fclose(fc); pausa(); return; }
	
	printf("\n  >> Ingrese # medico (0 para volver): "); 
	
	// Validamos que ingrese un numero valido
	if(scanf("%d", &op)!=1){
		while(getchar()!='\n'); 
		printf("\n    [ERROR] Opcion invalida.\n");
		fclose(fm); fclose(fc); pausa(); return;
	} 
	while(getchar()!='\n');
	
	if (op == 0) { fclose(fm); fclose(fc); return; }
	
	// Paso 2: Buscar cual medico corresponde al numero elegido
	i=0;
	while (fscanf(fm, "%[^;];%[^;];%[^;];%[^\n]\n", cod, nom, esp, hor) == 4) {
		if (++i == op) { 
			// Guardamos los datos del medico seleccionado
			strcpy(selCod, cod); strcpy(selHor, hor); strcpy(nombreMedicoSel, nom); 
			found=1; break; 
		}
	}
	fclose(fm);
	
	if (found) {
		// Paso 3: Crear los datos de la cita
		// Generamos un codigo aleatorio (Ej: C-5821)
		sprintf(nc.codigo, "C-%d", rand()%9000+1000);
		strcpy(nc.cedulaPaciente, cedula);
		strcpy(nc.codigoMedico, selCod);
		
		int valid=0;
		int abortar=0;
		
		// Pedir Fecha
		do {
			printf("    Fecha (DD/MM/AAAA) [0 para volver]: "); 
			scanf(" %[^\n]", nc.fecha);
			if(strcmp(nc.fecha, "0") == 0) { abortar=1; break; }
			
			if(esFechaFutura(nc.fecha)) valid=1;
			else printf("    [!] Fecha invalida o pasada.\n");
		} while(!valid);
		
		if(!abortar) {
			valid=0;
			// Pedir Hora
			do {
				printf("    Hora (HH:MM) [0 para volver]: "); 
				scanf(" %[^\n]", nc.hora);
				if(strcmp(nc.hora, "0") == 0) { abortar=1; break; }
				
				// Validaciones fuertes: Formato, Turno y Disponibilidad
				if(!esHoraValida(nc.hora)) {
					printf("    [!] Formato de hora invalido.\n");
				} else if(!estaEnHorarioDeAtencion(selHor, nc.hora)) {
					printf("    [!] Hora fuera del turno del medico (%s).\n", selHor);
				} else if(existeChoqueHorario(selCod, nc.fecha, nc.hora)) {
					printf("    [!] EL MEDICO YA TIENE UNA CITA A ESA HORA.\n");
					printf("        Por favor elija otra hora.\n");
				} else {
					valid=1;
				}
			} while(!valid);
		}
		
		if (abortar) {
			printf("\n    [INFO] Operacion cancelada por el usuario.\n");
		} else {
			while(getchar()!='\n'); 
			nc.estado = 1; strcpy(nc.diagnostico, "Pendiente");
			
			// Guardamos la nueva cita en el archivo
			fprintf(fc, "%s;%s;%s;%s;%s;%d;%s\n", nc.codigo, nc.cedulaPaciente, nc.codigoMedico, nc.fecha, nc.hora, nc.estado, nc.diagnostico);
			
			printf("\n    ============================================================");
			printf("\n    [EXITO] TU CITA SE HA AGENDADO EXITOSAMENTE");
			printf("\n    MEDICO: %s", nombreMedicoSel);
			printf("\n    CODIGO DE CITA: %s", nc.codigo);
			printf("\n    ============================================================");
		}
	} else {
		printf("\n    [ERROR] Medico no encontrado.\n");
	}
	fclose(fc);
	pausa();
}

// --- MENUS CON PROTECCION ANTI-BUCLE INFINITO ---
// Usamos scanf con validacion para evitar que si escriben letras, el menu se repita sin parar.

void menuPaciente(char idRef[]) {
	int op; char cod[20];
	do {
		imprimirEncabezado("PANEL PACIENTE");
		printf("    [1]  Ver Historial / Citas\n");
		printf("    [2]  Agendar Nueva Cita\n");
		printf("    [3]  Cancelar Cita\n");
		printf("    [4]  Reagendar Cita\n");
		printf("    [5]  Cerrar Sesion\n");
		imprimirLinea();
		printf("   >> Seleccione una opcion: ");
		
		// Validamos que sea numero
		if(scanf("%d", &op)!=1) { 
			while(getchar()!='\n'); // Limpiar basura
			printf("\n    [ERROR] Opcion invalida. Ingrese solo numeros.\n");
			pausa(); 
			op = -1; 
			continue; 
		}
		while(getchar()!='\n');
		
		switch(op){
		case 1: listarCitasGeneral(idRef); pausa(); break;
		case 2: agendarCitaPaciente(idRef); break;
		case 3: 
			// Solo mostramos la opcion de cancelar si tiene citas
			if (listarCitasGeneral(idRef) > 0) {
				printf("\n  >> Codigo a cancelar (0 para volver): "); 
				scanf(" %[^\n]", cod);
				while(getchar()!='\n'); 
				modificarCita(cod, 1, idRef, 0); 
			} else { pausa(); }
			break;
		case 4:
			if (listarCitasGeneral(idRef) > 0) {
				printf("\n  >> Codigo a reagendar (0 para volver): "); 
				scanf(" %[^\n]", cod);
				while(getchar()!='\n'); 
				modificarCita(cod, 2, idRef, 0); 
			} else { pausa(); }
			break;
		}
	} while(op!=5);
}

void menuMedico(char idRef[]) {
	int op;
	do {
		imprimirEncabezado("PANEL MEDICO");
		printf("    [1]  Ver mi Agenda\n");
		printf("    [2]  Atender Paciente / Editar Diagnostico\n");
		printf("    [3]  Modificar Mi Horario\n"); 
		printf("    [4]  Cerrar Sesion\n");
		imprimirLinea();
		printf("   >> Seleccione una opcion: ");
		
		// Validamos que sea numero
		if(scanf("%d", &op)!=1) { 
			while(getchar()!='\n'); 
			printf("\n    [ERROR] Opcion invalida. Ingrese solo numeros.\n");
			pausa(); 
			op = -1; 
			continue; 
		}
		while(getchar()!='\n');
		
		switch(op){
		case 1: verCitasDeMedico(idRef); break;
		case 2: atenderCitaMedico(idRef); break;
		case 3: modificarHorarioMedico(idRef); break;
		case 4: printf("\n    Cerrando sesion...\n"); break;
		default: printf("\n    [ERROR] Opcion invalida.\n"); pausa(); break;
		}
	} while(op!=4);
}

void menuAdminCitas(void) {
	int op; char cod[20];
	do {
		imprimirEncabezado("ADMINISTRACION DE CITAS");
		printf("    [1]  Ver Todas las Citas\n");
		printf("    [2]  Cancelar Cita (Admin)\n");
		printf("    [3]  Reagendar Cita (Admin)\n");
		printf("    [4]  Volver\n");
		imprimirLinea();
		printf("   >> Seleccione una opcion: ");
		
		// Validamos que sea numero
		if(scanf("%d", &op)!=1) { 
			while(getchar()!='\n'); 
			printf("\n    [ERROR] Opcion invalida. Ingrese solo numeros.\n");
			pausa(); 
			op = -1; 
			continue; 
		}
		while(getchar()!='\n');
		
		switch(op){
		case 1: listarCitasGeneral(NULL); pausa(); break; // NULL muestra todas
		case 2: 
			if (listarCitasGeneral(NULL) > 0) {
				printf("\n  >> Codigo (0 volver): "); 
				scanf(" %[^\n]", cod);
				while(getchar()!='\n'); 
				// Llamamos con permisos de Admin (ultimo parametro 1)
				modificarCita(cod, 1, "", 1); 
			} else { pausa(); }
			break;
		case 3: 
			if (listarCitasGeneral(NULL) > 0) {
				printf("\n  >> Codigo (0 volver): "); 
				scanf(" %[^\n]", cod);
				while(getchar()!='\n'); 
				modificarCita(cod, 2, "", 1);
			} else { pausa(); }
			break;
		}
	} while(op!=4);
}

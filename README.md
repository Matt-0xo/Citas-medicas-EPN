# Citas-medicas-EPN
Actualización Final: Validación Robusta, Lógica de Negocio y Persistencia de Datos
Se ha refactorizado el sistema completo para garantizar la integridad de los datos y mejorar la experiencia de usuario. Principales cambios por módulo:
•	main.c / admin.c: Implementación de bucles protegidos contra entradas inválidas (sanitización de scanf) para evitar ciclos infinitos. Enrutamiento dinámico de usuarios según su rol (ADMIN, MEDICO, PACIENTE).
•	citas.c / .h: Lógica central blindada. Se agregaron validaciones de fechas futuras (time.h), verificación de rangos horarios del médico y detección de colisiones (evitar doble agendamiento). Implementación completa de máquinas de estado para las citas (Activa, Cancelada, Atendida/Diagnosticada) y edición mediante archivos temporales.
•	usuarios.c / .h: Seguridad mejorada en el registro. Validación de cadenas alfanuméricas (ctype.h) para evitar inyección de caracteres corruptos. Vinculación automática entre la cuenta de usuario y la entidad correspondiente (Cédula Paciente / Código Médico.
•	medicos.c / .h: Añadida la funcionalidad de modificación de horarios y validación estricta de formato de hora (HH:MM-HH:MM).
•	pacientes.c / .h: Expansión del acceso para administradores, permitiendo registro manual y eliminación física de registros mediante manejo de archivos auxiliares.
•	(Nuevo)interfaz.c / .h: Modularización de la capa de presentación. Estandarización de limpieza de pantalla, pausas del sistema y encabezados con fecha/hora en tiempo real.

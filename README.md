# Sistema de Gestión de Citas Médicas

Sistema transaccional (TPS) desarrollado en lenguaje C para la administración de un centro hospitalario, permitiendo gestionar usuarios, pacientes, médicos y citas médicas mediante una arquitectura modular y almacenamiento persistente en archivos.

---

# Descripción del Proyecto

Este proyecto consiste en el desarrollo de un sistema de información hospitalario orientado a la gestión de citas médicas.

El sistema permite controlar el acceso según el rol del usuario (Administrador, Médico o Paciente), gestionar registros médicos y de pacientes, asignar citas y mantener un historial de atención médica.

Fue desarrollado como proyecto académico aplicando conceptos de:

- Programación estructurada
- Modularización
- Manejo de archivos
- Validación de datos
- Sistemas TPS (Transaction Processing System)
- CBIS (Computer-Based Information System)

---

# Objetivos

## Objetivo general

Desarrollar un sistema hospitalario que permita automatizar el proceso de gestión de citas médicas mediante el uso de programación en C.

## Objetivos específicos

- Gestionar usuarios con distintos niveles de acceso.
- Registrar pacientes y médicos con validaciones.
- Asignar citas médicas según disponibilidad.
- Controlar conflictos de horarios.
- Registrar diagnósticos médicos.
- Mantener persistencia de datos mediante archivos.

---

# Funcionalidades del Sistema

## Administrador

El administrador tiene acceso completo al sistema y puede:

- Registrar pacientes.
- Registrar médicos.
- Consultar registros.
- Gestionar citas médicas.
- Supervisar información general del sistema.

---

## Médico

El médico puede:

- Consultar sus citas asignadas.
- Visualizar pacientes atendidos.
- Registrar diagnósticos médicos.
- Consultar estado de citas.

---

## Paciente

El paciente puede:

- Consultar sus citas.
- Agendar citas médicas.
- Cancelar citas.
- Reprogramar citas.

---

# Validaciones Implementadas

El sistema incluye validaciones para garantizar integridad de datos:

## Pacientes

- Cédula única.
- Cédula de 10 dígitos.
- Solo números en cédula.
- Edad positiva.
- Teléfono numérico.

## Médicos

- Código único.
- Especialidad obligatoria.
- Horario con formato válido.

## Citas

- No se permiten citas duplicadas para un mismo médico en la misma fecha y hora.
- Verificación de disponibilidad médica.

---

# Estructura del Proyecto

```bash
Proyecto-Citas-Medicas/
│
├── main.c
├── admin.c
├── usuarios.c
├── pacientes.c
├── medicos.c
├── citas.c
│
├── admin.h
├── usuarios.h
├── pacientes.h
├── medicos.h
├── citas.h
│
├── usuarios.txt
├── pacientes.txt
├── medicos.txt
├── citas.txt
│
└── README.md

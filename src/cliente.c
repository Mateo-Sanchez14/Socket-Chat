/* Copyright 2023,
 * Facultad de Ciencias Exactas y Tecnología
 * Universidad Nacional de Tucuman
 * Copyright 2023, Mateo Sanchez <mateo-sanchez@hotmail.com>
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *
 * 1. Redistributions of source code must retain the above copyright notice,
 *    this list of conditions and the following disclaimer.
 *
 * 2. Redistributions in binary form must reproduce the above copyright notice,
 *    this list of conditions and the following disclaimer in the documentation
 *    and/or other materials provided with the distribution.
 *
 * 3. Neither the name of the copyright holder nor the names of its
 *    contributors may be used to endorse or promote products derived from this
 *    software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE
 * LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
 * CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
 * SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
 * INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
 * CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 * ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 * POSSIBILITY OF SUCH DAMAGE.
 */

/* === Inclusiones de cabeceras ================================================================ */
#include <stdio.h>
#include <ctype.h>
#include <string.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <time.h>
#include <arpa/inet.h>
#include <servidor.h>

/* === Definicion y Macros ===================================================================== */
#ifdef _WIN32
#define limpiarPantalla() system("cls")
#else
#define limpiarPantalla() system("clear")

#endif

// Consigna:
/*
Se desea crear una aplicación distribuida de mensajería la cual consta de un servidor el cual envía en
simultáneo mensajes a todas las aplicaciones cliente instaladas en una determinada subred.
Las aplicaciones tienen que hacer lo siguiente:
• Servidor:
Al iniciarse la aplicación debe mostrar por pantalla un mensaje de bienvenida. A continuación debe abrir
un puerto efímero aleatorio entre el 50.000 y el 65.000 (TCP o UDP según su criterio de diseño). Luego, la
aplicación debe solicitar el ingreso de un mensaje vía teclado (de hasta 200 caracteres), enviar el mismo a
todos los hosts de la subred y quedar a la espera de la escritura de un nuevo mensaje.
• Cliente:
Al iniciarse la aplicación debe mostrar por pantalla un mensaje de bienvenida. A continuación debe abrir
el puerto registrado 2.500 (TCP o UDP según su criterio de diseño) y quedar a la espera de la recepción
de un mensaje por parte del servidor. Una vez llegado un mensaje, la aplicación debe mostrar por
pantalla la siguiente información:
o Fecha y hora de la PC cliente.
o IP y puerto desde la que llega el mensaje.
o Mensaje.
En ambos casos debe realizar el control de error de los ingresos de datos, funciones y las llamadas a sistema
utilizadas, mostrando mensajes informativos en caso de falla.
*/

/* === Declaraciones de tipos de datos internos ================================================ */
#ifndef MSG_CONFIRM
#define MSG_CONFIRM 0
#endif
/* === Definiciones de variables internas ====================================================== */

/* === Declaraciones de funciones internas ==================================================== */

/* === Definiciones de funciones internas ====================================================== */

/*Aquí se escriben la implementación de las funciones */

void client() {
    int clientSocket = 0;
    int port = 0;
    char ip[16];
    char message[200];
    char buffer[200];
    struct sockaddr_in sourceAddress;
    socklen_t addressLength = sizeof(sourceAddress);

    printf("Bienvenido a la aplicacion de mensajeria como cliente\n");

    // We need to create the client socket to listen to the server
    clientSocket = socket(AF_INET, SOCK_DGRAM, 0);
    if (clientSocket < 0) {
        perror("Error al crear el socket del cliente");
        exit(EXIT_FAILURE);
    }

    // We need to set the client address
    struct sockaddr_in clientAddress;
    clientAddress.sin_family = AF_INET;
    clientAddress.sin_addr.s_addr = INADDR_ANY;
    clientAddress.sin_port = htons(2500);
    bzero(&(clientAddress.sin_zero), 8);

    // bind the socket to the port
    if (bind(clientSocket, (struct sockaddr *)&clientAddress, sizeof(clientAddress)) < 0) {
        perror("Error al bindear el socket del cliente");
        exit(EXIT_FAILURE);
    }

    while (1) {
        // Receive the message from the server
        if (recvfrom(clientSocket, buffer, sizeof(buffer), 0, (struct sockaddr *)&sourceAddress, &addressLength) < 0) {
            perror("Error al recibir el mensaje del servidor");
            exit(EXIT_FAILURE);
        }
        // Get the source IP address
        char sourceIP[INET_ADDRSTRLEN];
        inet_ntop(AF_INET, &(sourceAddress.sin_addr), sourceIP, INET_ADDRSTRLEN);

        printf("El mensaje recibido es: %s\n", buffer);

        // Show the date and time of the client
        time_t t = time(NULL);
        struct tm tm = *localtime(&t);
        printf("La fecha y hora del cliente es: %d-%d-%d %d:%d:%d\n", tm.tm_year + 1900, tm.tm_mon + 1,
               tm.tm_mday, tm.tm_hour, tm.tm_min, tm.tm_sec);

        // Show the ip and port from where the message came
        printf("La ip y el puerto desde donde llego el mensaje es: %s:%d\n", sourceIP, ntohs(sourceAddress.sin_port));
        bzero(&sourceIP, sizeof(sourceIP));
    }
}

/* === Definiciones de funciones externas ====================================================== */

/* === Cierre de archivo ======================================================================== */
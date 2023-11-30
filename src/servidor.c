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

void servidor() {
    int serverSocket = 0;
    int clientSocket = 0;
    int clientLength = 0;
    int messageLength = 0;
    int messageSent = 0;
    int messageReceived = 0;
    char message[200];
    char buffer[200];
    struct sockaddr_in serverAddress;
    struct sockaddr_in clientAddress;

    // We need to create the socket
    serverSocket = socket(AF_INET, SOCK_DGRAM, 0);
    if (serverSocket < 0) {
        perror("Error al crear el socket");
        exit(1);
    }

    // Asignate a random value between 50000 and 65000 to the port variable
    srand(time(NULL));
    int port = rand() % 15000 + 50000;

    // We need to set the server address
    serverAddress.sin_family = AF_INET;         // IPv4
    serverAddress.sin_addr.s_addr = INADDR_ANY; // Any address
    serverAddress.sin_port = htons(port);       // Port
    bzero(&(serverAddress.sin_zero), 8);        // Zero the rest of the struct

    // We need to bind the socket
    if (bind(serverSocket, (struct sockaddr *)&serverAddress, sizeof(serverAddress)) < 0) {
        perror("Error al bindear el socket");
        exit(1);
    }

    // Set the client parameters
    clientAddress.sin_family = AF_INET;               // IPv4
    clientAddress.sin_addr.s_addr = INADDR_BROADCAST; // Broadcast address
    clientAddress.sin_port = htons(2500);             // Registered port
    bzero(&(clientAddress.sin_zero), 8);              // Zero the rest of the struct

    // We need to set the socket to broadcast
    int broadcastPermission = 1;
    if (setsockopt(serverSocket, SOL_SOCKET, SO_BROADCAST, (void *)&broadcastPermission, sizeof(broadcastPermission)) < 0) {
        perror("Error al setear el socket para broadcast");
        exit(1);
    }

    // We do a loop to send the message to all the hosts in the subnet
    while (1) {
        // Read from console the message to send cleaning the buffer
        printf("Ingrese el mensaje que desea enviar\n");
        fgets(message, sizeof(message), stdin);
        message[strlen(message) - 1] = '\0';
        messageLength = strlen(message);

        // Send the message
        messageSent = sendto(serverSocket, message, messageLength, MSG_CONFIRM, (struct sockaddr *)&clientAddress, sizeof(clientAddress));
        if (messageSent < 0) {
            perror("Error al enviar el mensaje");
            exit(1);
        }

        // Success message
        printf("Mensaje enviado con exito por el mejor servidor boquense\n");
    }
}

/* === Definiciones de funciones externas ====================================================== */

/* === Cierre de archivo ======================================================================== */
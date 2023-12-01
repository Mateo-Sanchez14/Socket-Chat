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
    int serverSocket = createSocket();
    int port = getRandomPort();
    struct sockaddr_in serverAddress = createServerAddress(port);
    struct sockaddr_in clientAddress = createClientAddress();
    setBroadcastOption(serverSocket);
    bindSocket(serverSocket, serverAddress);

    while (1) {
        char message[201];
        getMessageFromConsole(message);
        sendMessageToSubnet(serverSocket, message, clientAddress);
        printSuccessMessage();
    }
}

/**
 * Creates a socket for the server.
 *
 * @return The server socket.
 */
int createSocket() {
    int serverSocket = socket(AF_INET, SOCK_DGRAM, 0);
    if (serverSocket < 0) {
        perror("Error al crear el socket");
        exit(1);
    }
    return serverSocket;
}

/**
 * Generates a random port number between 50000 and 64999.
 *
 * @return The randomly generated port number.
 */
int getRandomPort() {
    srand(time(NULL));
    return rand() % 15000 + 50000;
}

/**
 * Creates a server address structure with the specified port.
 *
 * @param port The port number for the server address.
 * @return The created server address structure.
 */
struct sockaddr_in createServerAddress(int port) {
    struct sockaddr_in serverAddress;
    serverAddress.sin_family = AF_INET;
    serverAddress.sin_addr.s_addr = INADDR_ANY;
    serverAddress.sin_port = htons(port);
    bzero(&(serverAddress.sin_zero), 8);
    return serverAddress;
}

/**
 * Creates a client address structure.
 *
 * @return The created client address structure.
 */
struct sockaddr_in createClientAddress() {
    struct sockaddr_in clientAddress;
    clientAddress.sin_family = AF_INET;
    clientAddress.sin_addr.s_addr = INADDR_BROADCAST;
    clientAddress.sin_port = htons(2500);
    bzero(&(clientAddress.sin_zero), 8);
    return clientAddress;
}

/**
 * Sets the broadcast option for a given server socket.
 * This allows the server socket to send broadcast messages.
 *
 * @param serverSocket The server socket to set the broadcast option for.
 */
void setBroadcastOption(int serverSocket) {
    int broadcastPermission = 1;
    if (setsockopt(serverSocket, SOL_SOCKET, SO_BROADCAST, (void *)&broadcastPermission, sizeof(broadcastPermission)) < 0) {
        perror("Error al setear el socket para broadcast");
        exit(1);
    }
}

/**
 * Binds the server socket to the specified server address.
 *
 * @param serverSocket The server socket to bind.
 * @param serverAddress The server address to bind the socket to.
 */
void bindSocket(int serverSocket, struct sockaddr_in serverAddress) {
    if (bind(serverSocket, (struct sockaddr *)&serverAddress, sizeof(serverAddress)) < 0) {
        perror("Error al bindear el socket");
        exit(1);
    }
}

/**
 * Function to get a message from the console.
 *
 * @param message The buffer to store the message.
 */
void getMessageFromConsole(char *message) {
    char deleted, flag = 0;
    printf("Ingrese el mensaje que desea enviar\n");

    if (fgets(message, 201, stdin) != NULL) {
        // Eliminamos el salto de línea si está presente
        size_t len = strlen(message);
        if (len > 0 && message[len - 1] == '\n') {
            message[len - 1] = '\0'; // Eliminamos el salto de línea
            len = strlen(message);
        } else {
            // Limpiamos el buffer de entrada en caso de que la entrada sea demasiado larga
            int c;
            while ((c = getchar()) != '\n' && c != EOF)
                ;
        }
    }
}

/**
 * Sends a message to a client in the subnet.
 *
 * @param serverSocket The server socket.
 * @param message The message to send.
 * @param clientAddress The client's address.
 */
void sendMessageToSubnet(int serverSocket, const char *message, struct sockaddr_in clientAddress) {
    int messageLength = strlen(message);
    int messageSent = sendto(serverSocket, message, messageLength + 1, MSG_CONFIRM, (struct sockaddr *)&clientAddress, sizeof(clientAddress));
    if (messageSent < 0) {
        perror("Error al enviar el mensaje");
        exit(1);
    }
}

char *codeSnippetSuccess = "    .___  ___.  _______ .__   __.      _______.     ___            __   _______     _______ .__   __. ____    ____  __       ___       _______   ______\n"
                           "    |   \\/   | |   ____||  \\ |  |     /       |    /   \\          |  | |   ____|   |   ____||  \\ |  | \\   \\  /   / |  |     /   \\     |       \\ /  __  \\\n"
                           "    |  \\  /  | |  |__   |   \\|  |    |   (----`   /  ^  \\         |  | |  |__      |  |__   |   \\|  |  \\   \\/   /  |  |    /  ^  \\    |  .--.  |  |  |  |\n"
                           "    |  |\\/|  | |   __|  |  . `  |     \\   \\      /  /_\\  \\  .--.  |  | |   __|     |   __|  |  . `  |   \\      /   |  |   /  /_\\  \\   |  |  |  |  |  |  |\n"
                           "    |  |  |  | |  |____ |  |\\   | .----)   |    /  _____  \\ |  `--'  | |  |____    |  |____ |  |\\   |    \\    /    |  |  /  _____  \\  |  '--'  |  `--'  |\n"
                           "    |__|  |__| |_______||__| \\__| |_______/    /__/     \\__\\ \\______/  |_______|   |_______||__| \\__|     \\__/     |__| /__/     \\__\\ |_______/ \\______/\n"
                           "\n"
                           "      ______   ______   .__   __.     __________   ___  __  .___________.  ______\n"
                           "     /      | /  __  \\  |  \\ |  |    |   ____\\  \\ /  / |  | |           | /  __  \\\n"
                           "    |  ,----'|  |  |  | |   \\|  |    |  |__   \\  V  /  |  | `---|  |----`|  |  |  |\n"
                           "    |  |     |  |  |  | |  . `  |    |   __|   >   <   |  |     |  |     |  |  |  |\n"
                           "    |  `----.|  `--'  | |  |\\   |    |  |____ /  .  \\  |  |     |  |     |  `--'  |\n"
                           "     \\______| \\______/  |__| \\__|    |_______/__/ \\__\\ |__|     |__|      \\______/\n"
                           "\n"
                           "    .______     ______     ______      __    __   _______ .__   __.      _______. _______  __\n"
                           "    |   _  \\   /  __  \\   /  __  \\    |  |  |  | |   ____||  \\ |  |     /       ||   ____||  |\n"
                           "    |  |_)  | |  |  |  | |  |  |  |   |  |  |  | |  |__   |   \\|  |    |   (----`|  |__   |  |\n"
                           "    |   _  <  |  |  |  | |  |  |  |   |  |  |  | |   __|  |  . `  |     \\   \\    |   __|  |  |\n"
                           "    |  |_)  | |  `--'  | |  `--'  '--.|  `--'  | |  |____ |  |\\   | .----)   |   |  |____ |__|\n"
                           "    |______/   \\______/   \\_____\\_____\\______/  |_______||__| \\__| |_______/    |_______|(__)\n";

/**
 * Prints a success message to the console.
 */
void printSuccessMessage() {
    printf("%s\n", codeSnippetSuccess);
}

/* === Definiciones de funciones externas ====================================================== */

/* === Cierre de archivo ======================================================================== */
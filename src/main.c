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
/* Agregar aquí los .h necesarios
Por ejemplo
#include "funciones.h"
#include "persona.h"
*/
#include <stdio.h>
#include <ctype.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>

#include <stdlib.h>

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

int main(int argc, char const *argv[]) {
    // First we need to chose if we are going to be a server or a client
    int option = 0;
    int port = 0;
    char ip[16];
    char message[200];
    char buffer[200];

    printf("Bienvenido a la aplicacion de mensajeria\n");
    printf("Por favor, ingrese la opcion que desea realizar\n");
    printf("1. Servidor\n");
    printf("2. Cliente\n");
    scanf("%d", &option);
    while (option != 1 && option != 2) {
        printf("Opcion invalida, por favor ingrese una opcion valida\n");
        scanf("%d", &option);
    }

    if (option == 1) {
        printf("Usted eligio la opcion servidor\n");
        printf("Por favor, ingrese el mensaje que desea enviar\n");
        scanf("%s", message);
        printf("El mensaje que desea enviar es: %s\n", message);
        sendMessage(message);
    } else {
        printf("Usted eligio la opcion cliente\n");
        printf("Por favor, ingrese el puerto que desea utilizar\n");
        scanf("%d", &port);
        while (port < 50000 || port > 65000) {
            printf("Puerto invalido, por favor ingrese un puerto valido\n");
            scanf("%d", &port);
        }
        printf("Por favor, ingrese la ip a la que desea enviar el mensaje\n");
        scanf("%s", ip);
        printf("La ip a la que desea enviar el mensaje es: %s\n", ip);
        printf("Por favor, ingrese el puerto al que desea enviar el mensaje\n");
        scanf("%d", &port);
        while (port < 50000 || port > 65000) {
            printf("Puerto invalido, por favor ingrese un puerto valido\n");
            scanf("%d", &port);
        }
        printf("El puerto al que desea enviar el mensaje es: %d\n", port);
        // Aqui se recibe el mensaje
    }

    return 0;
}

/*Aquí se escriben la implementación de las funciones */

void sendMessage(char *message) {
    // This function will send the message to all the hosts in the subnet
    // First we need to get the subnet
    char subnet[16] = "192.168.1.0";
    char ip[16];
    int sockfd;
    struct sockaddr_in servaddr;

    // Creating socket file descriptor
    if ((sockfd = socket(AF_INET, SOCK_DGRAM, 0)) < 0) {
        perror("socket creation failed");
        exit(EXIT_FAILURE);
    }

    memset(&servaddr, 0, sizeof(servaddr));

    // Filling server information
    servaddr.sin_family = AF_INET;
    servaddr.sin_port = htons(8080);

    // Iterate over all possible hosts in the subnet
    for (int i = 1; i <= 254; i++) {
        sprintf(ip, "%s.%d", subnet, i);
        servaddr.sin_addr.s_addr = inet_addr(ip);

        // Send the message
        sendto(sockfd, (const char *)message, strlen(message),
               MSG_CONFIRM, (const struct sockaddr *)&servaddr,
               sizeof(servaddr));
    }

    close(sockfd);
}

/* === Definiciones de funciones externas ====================================================== */

/* === Cierre de archivo ======================================================================== */
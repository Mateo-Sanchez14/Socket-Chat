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
#include <servidor.h>
#include <cliente.h>

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
void mensajeBienvenida();
/* === Definiciones de funciones internas ====================================================== */

int main(int argc, char const *argv[]) {
    // First we need to chose if we are going to be a server or a client
    char option[2];
    int port = 0;
    char ip[16];
    char message[200];
    char buffer[200];

    printf("Bienvenido a la aplicacion de mensajeria\n");
    mensajeBienvenida();
    printf("Por favor, ingrese la opcion que desea realizar\n");
    printf("1. Servidor\n");
    printf("2. Cliente\n");

    while (1) {
        fgets(option, sizeof(option), stdin);
        fgetc(stdin);
        option[strcspn(option, "\n")] = '\0';

        if (strlen(option) == 1 && (option[0] == '1' || option[0] == '2')) {
            break;
        } else {
            printf("Opcion invalida, por favor ingrese una opcion valida\n");
        }
    }

    limpiarPantalla();
    if (option[0] == '1') {
        servidor();
    } else {
        client();
    }

    return 0;
}

/*Aquí se escriben la implementación de las funciones */
const char *codeSnippet = "    _________  ________  ________                           ___  ________\n"
                          "   |\\___   ___\\\\   ____\\|\\   __  \\                         |\\  \\|\\   __  \\\n"
                          "   \\|___ \\  \\_\\ \\  \\___|\\ \\  \\|\\  \\      ____________      \\ \\  \\ \\  \\|\\  \\\n"
                          "        \\ \\  \\ \\ \\  \\    \\ \\   ____\\    |\\____________\\     \\ \\  \\ \\   ____\\\n"
                          "         \\ \\  \\ \\ \\  \\____\\ \\  \\___|    \\|____________|      \\ \\  \\ \\  \\___|\n"
                          "          \\ \\__\\ \\ \\_______\\ \\__\\                             \\ \\__\\ \\__\\\n"
                          "           \\|__|  \\|_______|\\|__|                              \\|__|\\|__|\n"
                          "\n"
                          "\n"
                          "\n"
                          "    ________  ________  ___  ___  ________  ________           _____\n"
                          "   |\\   ____\\|\\   __  \\|\\  \\|\\  \\|\\   __  \\|\\   __  \\         / __  \\\n"
                          "   \\ \\  \\___|\\ \\  \\|\\  \\ \\  \\\\\\  \\ \\  \\|\\  \\ \\  \\|\\  \\       |\\/_|\\  \\\n"
                          "    \\ \\  \\  __\\ \\   _  _\\ \\  \\\\\\  \\ \\   ____\\ \\  \\\\\\  \\      \\|/ \\ \\  \\\n"
                          "     \\ \\  \\|\\  \\ \\  \\\\  \\\\ \\  \\\\\\  \\ \\  \\___|\\ \\  \\\\\\  \\          \\ \\  \\\n"
                          "      \\ \\_______\\ \\__\\\\ _\\\\ \\_______\\ \\__\\    \\ \\_______\\          \\ \\__\\\n"
                          "       \\|_______|\\|__|\\|__|\\|_______|\\|__|     \\|_______|           \\|__|\n";

void mensajeBienvenida() {
    printf("%s\n", codeSnippet);
}

/* === Definiciones de funciones externas ====================================================== */

/* === Cierre de archivo ======================================================================== */
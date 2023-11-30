#ifndef SERVIDOR_H
#define SERVIDOR_H

// Include any necessary headers

// Declare any classes, structs, or functions
/**
 * Main function for the server application.
 */
void servidor();

/**
 * Creates a socket for the server.
 *
 * @return The server socket.
 */
int createSocket();

/**
 * Generates a random port number between 50000 and 64999.
 *
 * @return The randomly generated port number.
 */
int getRandomPort();

/**
 * Creates a server address structure with the specified port.
 *
 * @param port The port number for the server address.
 * @return The created server address structure.
 */
struct sockaddr_in createServerAddress(int port);

/**
 * Creates a client address structure.
 *
 * @return The created client address structure.
 */
struct sockaddr_in createClientAddress();

/**
 * Sets the broadcast option for a given server socket.
 * This allows the server socket to send broadcast messages.
 *
 * @param serverSocket The server socket to set the broadcast option for.
 */
void setBroadcastOption(int serverSocket);

/**
 * Binds the server socket to the specified server address.
 *
 * @param serverSocket The server socket to bind.
 * @param serverAddress The server address to bind the socket to.
 */
void bindSocket(int serverSocket, struct sockaddr_in serverAddress);

/**
 * Function to get a message from the console.
 *
 * @param message The buffer to store the message.
 */
void getMessageFromConsole(char *message);

/**
 * Sends a message to a client in the subnet.
 *
 * @param serverSocket The server socket.
 * @param message The message to send.
 * @param clientAddress The client's address.
 */
void sendMessageToSubnet(int serverSocket, const char *message, struct sockaddr_in clientAddress);

/**
 * Prints a success message to the console.
 */
void printSuccessMessage();

#endif // SERVIDOR_H

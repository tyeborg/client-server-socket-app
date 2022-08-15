#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <sys/socket.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <stdbool.h>
#include <regex.h> 

#define REGEX "[\\.0-9]"
#define BASE 10

/* Create a function to remove all occurences in a string */
void removeOccurrences(char* str, char c) 
{
    char *pr = str, *pw = str;
    while (*pr) {
        *pw = *pr++;
        pw += (*pw != c);
    }
    *pw = '\0';
}

/* Remove all of the remaining whitespaces in the input */
void removeTrailingWhiteSpaces(char* input)
{
    for(int i = strlen(input); i > 0; i--)
    {
        if(input[i] == '\n')
        {
            input[i] = '\0';
            break;
        }
    }
}

/* Create a IPv4 Rule Speicication display for the user */
void ipv4Specifications()
{
    printf("\n------IPv4 Address Specifications------\n");
    printf("*- Written in decimal digits.\n");
    printf("*- Digits formatted in four sections.\n");
    printf("*- Each section is separated by a period.\n");
    printf("*- Each digit range from 0 to 255.\n");
}

/* Create a function for sending a message to the server */
int sendToServer(int status, int sock, const char message[1000])
{
    /* Send whatever message to the given socket */
    status = send(sock, message, strlen(message), 0);

    if(status < 0)
    {
        /* If unsuccessful, notify the user on the matter */
        printf("Failed to send to server...\n");
        return 1;
    }
    return 0;
}

/* Create a function to receive a message from the server */
int receiveFromServer(int status, int sock, char message[1000])
{
    /* Receive any message from the given socket */
    status = recv(sock, message, strlen(message), 0);
    
    if (status < 0)
    {
        /* If unsuccessful, notify the user that the receive failed */
        printf("Receive failed.\n");
        return 1;
    }
    return 0;
}

int main(int argc, char **argv)
{
    /* Declare variables */
    char ip[1000];
    int port;
    int socket_desc;
    struct sockaddr_in server;
    int status;
    
    /* Create variables to use for validating prompts */
    bool loop = true;
    bool loop2 = true;
    bool loop3 = true;

    int rc;
    regex_t regex;
    char placeholder[1000];

    while(loop == true)
    {
        /* Ask the user for an IP address to connect to */
        printf("\nPlease enter an IP address to connect to: ");
        fgets(ip, sizeof(ip), stdin);

        /* Remove the trailing whitespaces of the IP input */
        removeTrailingWhiteSpaces(ip);

        /* Have a copy of the input */
        strcpy(placeholder, ip);

        rc = regcomp(&regex, REGEX, REG_EXTENDED);
        if(rc != 0)
        {
            fprintf(stderr, "Could not compile regex...\n");
            exit(1);
        }

        rc = regexec(&regex, ip, 0, NULL, 0);
        /* Determine if the input IP address matches with the regular expression */
        if(rc == 0)
        {
            char *endptr;
            int number;
            
            /* Utilize removeOccurrences function to remove all periods in ip */
            removeOccurrences(ip, '.');
            number = strtol(ip, &endptr, BASE);
            
            if (*endptr != '\0' || endptr == ip) 
            {
                /* Notify the user that IP addresses should 
                only contain numbers (and periods) */
                ipv4Specifications();
                printf("\n*- IP addresses must only contain numbers and periods.\n");
                /* Set loop to true */
                loop = true;
            }
            else
            {
                /* Set ip to the original input of the user */
                strcpy(ip, placeholder);
                /* Set loop to false */
                loop = false;
            }

            /* Determine whether loop was set to true or false 
            within the previous for loop */
            if(loop == true)
            {
                /* Loop throughout the main while loop once more */
                loop = true;
                continue;
            }
            
            /* Set varaibles for separating ip with the delimeter of '.' */
            char *token = strtok(ip, ".");
            char *ptr;
            int substring;
            int counter = 0;
            
            /* Construct a while loop to loop until the token is empty */
            while(token != NULL)
            {
                /* Set variable to the substring part of ip spearated by period */
                substring = strtol(token, &ptr, 10);

                /* Determine if the substring is in range between 0 and 255 */
                if(substring <= 255 && substring >= 0)
                {
                    /* Set the next instance for token */
                    token = strtok(NULL, ".");
                    /* Add one to counter */
                    counter += 1;
                }
                else if(substring < 0 || substring > 255)
                {
                    /* Notify the user that each section needs to be between 0 and 255 */
                    printf("*- '%d' is not in range between 0 and 255.\n", substring);
                    /* Set loop to true */
                    loop = true;

                    /* Set the next instance for token */
                    token = strtok(NULL, ".");
                    /* Add one to counter */
                    counter += 1;
                }
            } 
        
            /* Determine if there were four sections in the ip input */
            if(counter == 4 && loop == false)
            {
                /* Set ip to the original input set by user */
                strcpy(ip, placeholder);
                /* Notify the user that their input has been accepted */
                printf("'%s' has been accepted...\n", ip);
                /* Exit the main while loop */
                break;
            }
            else if(counter != 4 || counter != 4 && loop == true)
            {
                /* Let the user know that IP addresses contain four sections */
                printf("*- IPv4 addresses are 4 blocks of digits separated by a period.\n");
                /* Continue to loop once more throughout the main while loop */
                ipv4Specifications();
                loop = true;
                continue;
            }
        }
        /* Determine if the IP input does not match with the regular expression */
        else if(rc == REG_NOMATCH)
        {
            if(strcmp(ip, "\n") == 0)
            {
                printf("*- Empty entries are not allowed...\n");
            }
            else
            {
                /* Let the user know that the input is not considered to be an IP address */
                printf("*- '%s' is not considered to be an IPv4 address.\n", ip);
                ipv4Specifications();
            }
            /* Continue to loop throughout until a proper IP address is inputted */
            loop = true;   
            continue;         
        }
        else
        {
            /* If the input failed all of the conditions, exit the program */
            perror("*- Error...\n");
            exit(1);
        }
    }
    
    /* Construct a loop to validate for port number */
    while(loop2 == true)
    {
        /* Declare variables */
        char subport[1000];
        char *ptr;
        char *endptr;

        /* Ask the user for a port to connect to */
        printf("\nPlease enter a port to connect to: ");
        fgets(subport, sizeof(subport), stdin);

        /* Remove trailing white spaces from the user input */
        removeTrailingWhiteSpaces(subport);

        if(strcmp(subport, "\n") == 0)
        {
            printf("Empty entries are not allowed...\n");
            loop2 = true;
            continue;
        }
        
        port = strtol(subport, &endptr, BASE);
        if (*endptr != '\0' || endptr == subport)
        {
            /* Exit the for loop if any character is not an integer */
            /* Set loop2 to true */
            /* Notify the user that their input is not an integer */
            printf("*- '%s' is not an integer.\n", subport);
            loop2 = true;
        }
        else
        {
            if(port > 65353 || port < 0)
            {
                printf("*- Port numbers must range from 0 to 65353...\n");
                loop2 = true;
            }
            else
            {
                loop2 = false;
            }
        }
        
        /* Set condition if loop2 is true */
        if(loop2 == true)
        {
            /* Continue to loop throughout the while loop */
            continue;
        }
        /* Set condition if loop2 is false */
        else if(loop2 == false)
        {
            /* Notify the user that the input has beeen accepted */
            printf("'%d' has been accepted...\n", port);
            /* Exit the while loop */
            break;
        }
    } 

    /* Create the socket */
    socket_desc = socket(AF_INET, SOCK_STREAM, 0);

    if (socket_desc == -1)
    {
        /* Notify the user that the socket could not be created */
        printf("Could not create socket\n");
    }

    /* Notify the user that the socket has been created */
    printf("\nSocket created\n");

    /* Set the family, address and port of the socket */
    server.sin_addr.s_addr = inet_addr(ip);
    server.sin_family = AF_INET;
    server.sin_port = htons(port);

    /* Connect to the web server with the three values defined above */
    status = connect(socket_desc, (struct sockaddr*)&server, sizeof(server));

    if (status < 0)
    {
        printf("Connection error...\n");
        return 1;
    }
    
    printf("Connected...\n");

    /* Send the IP address to the server */
    sendToServer(status, socket_desc, ip);
    printf("\nIP address successfully sent to server.\n");

    while(loop3 == true)
    {
        /* Declare a buffer to store and receive info on */
        char buffer[1000];
        char filename[1000];

        int exists;
        char *endptr;

        /* Clear all of the buffers */
        memset(filename, 0, 1000);
        memset(buffer, 0, 1000);

        /* Ask the user to enter a filename */
        printf("\nPlease enter a file ('exit' to exit): ");
        fgets(filename, sizeof(filename), stdin);

        /* Remove all of the remaining whitespaces in the filename input */
        removeTrailingWhiteSpaces(filename);
        
        /* Determine if an empty entry was entered */
        if(strcmp(filename, "\n") == 0)
        {
            printf("No empty entries allowed...\n");
            loop3 = true;
            continue;
        }

        /* Set buffer to the same value of filename */
        strcpy(buffer, filename);

        /* Convert all letters in filename to lowercase */
        for(int i = 0; filename[i]; i++)
        {
            filename[i] = tolower(filename[i]);
        }
        
        /* Loop until the user enters 'exit' */
        if(strcmp(filename, "exit") == 0)
        {
            /* Send the exit message to the server */
            sendToServer(status, socket_desc, filename);

            /* Notify the user that they will be exiting the loop */
            printf("Exiting the client program...\n");
            loop3 = false;
            break;
        }
        
        /* Set the filename to the original input */
        strcpy(filename, buffer);
        /* Clear the buffer */
        memset(buffer, 0, 1000);

        /* Send the filename to the server */
        sendToServer(status, socket_desc, filename);
        printf("%s sent to server...\n", filename);

        /* Receive confirmation for the existence of the file */
        receiveFromServer(status, socket_desc, buffer);

        /* Convert the input into a integer stored in 'exists' variable */    
        exists = strtol(buffer, &endptr, BASE);
        /* Clear the buffer */
        memset(buffer, 0, 1000);

        /* If file does not exist within the server */
        if(exists == 1)
        {
            /* Receive a message stating that the filename does not exist */
            receiveFromServer(status, socket_desc, buffer);

            /* Notify the user that the server reply has been received */
            printf("\nServer reply received.\n");
            /* Display the message stating that the file does not exist */
            printf("%s\n", buffer);

            /* Continue iterating throughout the loop */
            loop3 = true;
            continue;
        }
        else if(exists == 0)
        {
            /* Notify the user that the server reply has been received */
            printf("\nFile size message received.\n");

            /* Receive filename size in bytes message */
            receiveFromServer(status, socket_desc, buffer);
            printf("%s is %s bytes\n", filename, buffer);

            /* Clear the buffer */
            memset(buffer, 0, 1000);

            /* Receive the file content of the filename */
            receiveFromServer(status, socket_desc, buffer);
            printf("\n File content message received.\n");
            printf("%s\n", buffer);

            /* Continue iterating throughout the loop */
            loop3 = true;
            continue;
        }
        else
        {
            /* Apply a safeguard if 0 or 1 is not received by the client */
            printf("\nAn error has occurred.\n");
            loop3 = true;
            continue;
        }
    }
    /* Close the server socket */
    close(socket_desc);

    return 0;
}
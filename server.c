#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/kthread.h>
#include <linux/init.h>
#include <linux/fs.h>
#include <linux/slab.h>
#include <linux/net.h>
#include <linux/in.h>
#include <linux/moduleparam.h>

#include <linux/socket.h>
#include <linux/string.h>
#include <linux/delay.h>
#include <asm/uaccess.h>

static struct task_struct *thread;

/* Set the default of port to 5000 */
static int port = 5000;
/* Make port a module parameter */
module_param(port, int, 0);
/* Declare port in the module information */
MODULE_PARM_DESC(port, "An integer parameter");

int receiveCheck(int status)
{
    if (status < 0)
    {
        /* If unsuccessful, notify the user that the receive failed */
        printk(KERN_ERR "Receive failed.\n");
        return 1;
    }
    return 0;
}

int sendCheck(int status)
{
    if(status < 0)
    {
        /* If unsuccessful, notify the user that the message failed to send */
        printk(KERN_ERR "Failed to send error message to client...\n");
        return 1;
    }
    return 0;
}

char* read_file(const char *filepath, loff_t *size)
{
    /* Declare the buffer we will read into */
    char *buffer;
    /* Declare the file */
    struct file *file = NULL;
    int err = 0;

    /* Open the file in read only */
    file = filp_open(filepath, O_RDONLY, S_IRWXU | S_IRWXG | S_IRWXO);

    /* Check that the file exists */
    if (IS_ERR(file)) 
    {
        err = PTR_ERR(file);
        printk(KERN_ERR "Error opening file");
        return NULL;
    }
    /* Get size of the file */
    *size = vfs_llseek(file, 0, SEEK_END);

    /* Rewind to start of file again */
    vfs_llseek(file, 0, SEEK_SET);
    /* Allocate space to read file into */
    buffer = kmalloc(*size, GFP_KERNEL);
    /* Read file into buffer */
    kernel_read(file, buffer, *size, size);

    /* Close file */
    filp_close(file, NULL);
    /* Return buffer with the size that has also been set */
    return buffer;
}

/* Define a thread function for a thread to utilize */
int kthread_function(void *args)
{
    /* The socket that will listen for incoming connections */
    struct socket *sock;
    /* The client that has connected */
    struct socket *client;
    /* The address data structure defining where we listen */
    struct sockaddr_in server;
    /* Kernel sockets send UDP messages */
    struct msghdr msg;
    /* Kvec is the data we will send - it has buffer & size */
    struct kvec iov;
    int status;
    /* The buffer we will send/receive messages on */
    char buffer[1000];
    char client_ip[1000];
    char filename[1000];

    /* Declare a variable for while loop construction */
    int loop = 0;

    /* Create a socket */
    status = sock_create(PF_INET, SOCK_STREAM, IPPROTO_TCP, &sock);
    if (status < 0)
    {
        /* Let the user know that the socket could not be created */
        printk(KERN_ERR "Could not create socket\n");
        return -1;
    }
    /* State in the log that the socket has been created */
    printk(KERN_INFO "Socket created\n");

    /* Set the family, address and port of the socket */
    server.sin_family = PF_INET;
    server.sin_addr.s_addr = INADDR_ANY;
    server.sin_port = htons(port);

    /* Kernel bind the socket */
    status = kernel_bind(sock, (struct sockaddr*)&server, sizeof(struct sockaddr_in));
    if (status < 0)
    {
        /* Notify the user (in log) that the server socket failed to be created */
        printk(KERN_ERR "Failed to create server socket.\n");
        return -1;
    }
    /* Display in the log that the server socket has been created */
    printk(KERN_INFO "Server socket created.\n");

    /* Listen on the server socket */
    kernel_listen(sock, 5);
    printk(KERN_INFO "Server listening on port %d\n", port);

    /* Accept a new connection */
    status = kernel_accept(sock, &client, 0);
    if (status < 0)
    {
        /* Display in the log that there was an error on accepting a connection */
        printk(KERN_ERR "Error accepting incoming connection.\n");
        return -1;
    }
    /* Log a message stating that the connection was successful */
    printk(KERN_INFO "Connection successful.\n");

    /* Clear the buffer */
    memset(buffer, 0, 1000);
    memset(&msg, 0, sizeof(msg));

    /* Create the data packet to send */
    iov.iov_base = buffer;
    iov.iov_len = 1000;

    /* Receive the client IP address from the client */
    status = kernel_recvmsg(client, &msg, &iov, 1, 1000, 0);
    receiveCheck(status);

    /* Print the received message - now in iov_base of packet */
    printk(KERN_INFO "Client IP Address (received): %s\n", (char*)iov.iov_base);
    strcpy(client_ip, (char*)iov.iov_base);

    /* Construct a while loop */
    while(loop == 0)
    {
        /* Clear all the buffers again */
        memset(buffer, 0, 1000);
        memset(&msg, 0, sizeof(msg));
        /* Create the data packet to send */
        iov.iov_base = buffer;
        iov.iov_len = 1000;

        memset(filename, 0, 1000);

        /* Receive the filename from the client */
        status = kernel_recvmsg(client, &msg, &iov, 1, 1000, 0);
        receiveCheck(status);

        /* Copy the string value into a new variable */
        strcpy(filename, (char*)iov.iov_base);

        /* Clear all the buffers again */
        memset(buffer, 0, 1000);
        memset(&msg, 0, sizeof(msg));
        /* Create the data packet to send */
        iov.iov_base = buffer;
        iov.iov_len = 1000;

        /* Determine if the server received was an 'exit' message */
        if(strcmp(filename, "exit") == 0)
        {
            /* Notify the user that the server is exiting */
            printk(KERN_INFO "Exiting the server...\n");
            /* Exit the current loop */
            loop = 1;
            break;
        }

        /* Declare variables */
        loff_t size;
        /* Read the file and store it inside a pointer */
        char *file_content = read_file(filename, &size);

        /* Determine if the file exists or not */
        if(file_content == NULL)
        {
            /* Send over "1" to the client which will indicate that the file 
            does not exist */
            strcpy(buffer, "1");
            status = kernel_sendmsg(client, &msg, &iov, 1, strlen(buffer));
            sendCheck(status);

            /* Clear all the buffers again */
            memset(buffer, 0, 1000);
            memset(&msg, 0, sizeof(msg));
            /* Create the data packet to send */
            iov.iov_base = buffer;
            iov.iov_len = 1000;

            msleep(1500);

            /* Send a message to the client stating that their input is not a 
            file exisiting within the server */
            snprintf(buffer, sizeof(buffer), "'%s' does not exist on the server", filename);
            status = kernel_sendmsg(client, &msg, &iov, 1, strlen(buffer));
            if(status < 0)
            {
                /* If unsuccessful, notify the user that the message failed to send */
                printk(KERN_ERR "Failed to send error message to client...\n");
                return 1;
            }
            /* Print a kernel error message to the log about the non-existent file */
            printk(KERN_INFO "Error '%s' does not exist.\n", filename);

            /* Continue to iterate through the while loop */
            loop = 0;
            continue;
        }
        else
        {
            /* Send over "0" to the client which will indicate that the file 
            does not exist */
            strcpy(buffer, "0");
            status = kernel_sendmsg(client, &msg, &iov, 1, strlen(buffer));
            sendCheck(status);

            /* Clear all the buffers again */
            memset(buffer, 0, 1000);
            memset(&msg, 0, sizeof(msg));
            /* Create the data packet to send */
            iov.iov_base = buffer;
            iov.iov_len = 1000;
            
            /* Pause the program temporarily to avoid splicing of messages */
            msleep(1500);

            /* Print a kernel information message to the log stating that the file is being read */
            printk(KERN_INFO "Reading file %s\n", filename);
            /* Print a message to the log clarifying that the file has been sent to the ip */
            printk(KERN_INFO "%s sent to %s\n", filename, client_ip);

            sprintf(buffer, "%lld", size);
            /* Send the filesize to the client */
            status = kernel_sendmsg(client, &msg, &iov, 1, strlen(buffer));
            sendCheck(status);
            
            /* Clear all the buffers again */
            memset(buffer, 0, 1000);
            memset(&msg, 0, sizeof(msg));
            /* Create the data packet to send */
            iov.iov_base = buffer;
            iov.iov_len = 1000;

            msleep(1500);

            if(strcmp(file_content, "") == 0)
            {
                sprintf(buffer, "'%s' exists, but its content could not be displayed.", filename);
                printk(KERN_INFO "%s content could not be displayed to the client.\n", filename);
            }
            else
            {
                /* Place the file content into the buffer */
                sprintf(buffer, "%s", file_content);
                printk(KERN_INFO "%s content has been displayed to the client.\n", filename);
            }

            /* Send the buffer (file content) to the client */
            status = kernel_sendmsg(client, &msg, &iov, 1, strlen(buffer));
            sendCheck(status);

            /* Free the allocated memory */
            kfree(file_content);

            /* Continue to iterate throughout the while loop */
            loop = 0;
            continue;
        }
    }
    /* Shutdown the client socket */
    status = kernel_sock_shutdown(client, SHUT_RDWR);

    /* Shutdown the server socket */
    status = kernel_sock_shutdown(sock, SHUT_RDWR);

    /* Release the server socket */
    sock_release(sock);

    return 0;
}

static int __init init_mod(void)
{
    /* Create the thread */
    thread = kthread_run(kthread_function, NULL, "Connection Thread");
    if (thread)
    {
        /* Display to the log that the thread has been created */
        printk(KERN_INFO "Thread created.\n");
    }
    else 
    {
        /* Display to the log that thread creation failed */
        printk(KERN_INFO "Thread creation failed.\n");
        return 1;
    }
    return 0;
}

static void __exit exit_mod(void)
{
    /* In module unloading, the kthread must be stopped */
    kthread_stop(thread);
    /* Display an exit kernel information message to the log */
    printk(KERN_INFO "Exiting the kernel thread...\n");
}

/* Set module initialisation */
module_init(init_mod);
/* Set module exit */
module_exit(exit_mod);
/* Set module license to GPL */
MODULE_LICENSE("GPL");
/* Set module author to my name and email */
MODULE_AUTHOR("Tyler Supersad <supersat@roehampton.ac.uk>");
/* Set module description to "Operating Systems Coursework 2" */
MODULE_DESCRIPTION("Operating Systems Coursework 1");
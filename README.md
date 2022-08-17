# Listening Linux Kernel Module Project

https://img.shields.io/github/languages/code-size/tyeborg/client-server-socket-app
https://img.shields.io/badge/C-00599C?style=for-the-badge&logo=c&logoColor=white

This is a fully operational project (written in C) demonstrating how to develop a Linux Kernel Module that is able to listen to incoming connections.

Every component of this project evaluates the following:

* Construction of a simple program using methods of layering, error detection and recovery, and reflection of error status across layers
* Implementation of a simple client-server socket-based application

## Installation

Clone this repository by `git clone https://github.com/tyeborg/client-server-socket-app.git`

## Usage

1. To build the Linux Kernel Module, you will need to run the following command line in your Linux Machine to install Linux Kernel Headers:

```bash
sudo apt install linux-headers-$(uname –r)
```
2. Open a terminal to commence initialization of the Kernel module. **(To build the Kernel module, the Makefile is needed)**

The content within the Makefile is displayed below:

```Makefile
obj-m += server.o

all:
	make -C /lib/modules/$(shell uname -r)/build M=$(PWD) modules

clean:
	make -C /lib/modules/$(shell uname -r)/build M=$(PWD) clean
```

3. With the existence of the Makefile, enter <ins>*make*</ins> into the command line to build the Kernel module. 

4. Once the Kernel module is built, install it by using the following insmod terminal command:

```bash
sudo insmod server.ko
```

*Viola! At this point, the Kernel module launched a kernel thread which will listen to incoming connections on the defined port of 5000.*

5. Open a separate terminal to build the Client module
    * Build the Client module using `gcc client.c -o client` on the terminal.
    * Run the Client module using `./client` on the terminal.

*Fantastic, the Client module and the Kernel module are running simultaneously! Now it is time to establish a connection between them...*

6. As the Kernel module is listening to incoming connections, the client [you] can fulfill them if and only if both modules are running on the same port. Therefore in the Client module, ensure <ins>*5000*</ins> is entered when prompted with which port to connect to. 

7. When a connection is received, enter the absolute filename that is desired to be read 

## Additional Information

To access kernel messages that are printed onto the log, use the following command in the terminal: `dmesg`

## Project Evaluation

*"Kernel module builds and runs, but it doesn't seem to always get a file, and cannot reconnect. So, generally working and well put together with a few bugs in place."* - Kevin Chalmers 

**Grade:** 78.00 / 100.00

# Listening Linux Kernel Module Project

![GitHub code size in bytes](https://img.shields.io/github/languages/code-size/tyeborg/client-server-socket-app)
![GitHub top language](https://img.shields.io/github/languages/top/tyeborg/client-server-socket-app)

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

5. Open a separate terminal to build the Client module.
    * Build the Client module using `gcc client.c -o client` on the terminal.
    * Run the Client module using `./client` on the terminal.

*Fantastic, the Client module and the Kernel module are running simultaneously! Now it is time to establish a connection between them...*

6. As the Kernel module is listening to incoming connections, the client [you] can fulfill them if and only if both modules are running on the same port. Therefore in the Client module, ensure <ins>*5000*</ins> is entered when prompted with which port to connect to. 

7. When a connection is received, enter the absolute filename that is desired to be read. 

## Additional Information

To access kernel messages that are printed onto the log, use the following command in the terminal: `dmesg`

## Languages & Tools Utilized

<img src="https://cdn.jsdelivr.net/gh/devicons/devicon/icons/c/c-original.svg" height="50" width="50" />
<img src="https://cdn.jsdelivr.net/gh/devicons/devicon/icons/linux/linux-original.svg" height="50" width="50" />

## Project Evaluation

*"Kernel module builds and runs, but it doesn't seem to always get a file, and cannot reconnect. So, generally working and well put together with a few bugs in place."* - Kevin Chalmers 

* **Solution**: `Excellent` (A running solution meeting all the specification)
* **Correctness**: `Good` (Program produces correct answers or appropriate results for the majority of inputs)
* **Logic**: `Excellent` (Program logic is correct with no known boundary errors, and no redundant or contradictory conditions)
* **Robutness**: `Good` (Program handles almost all erroneous or unexpected input gracefully without surprising the user)
* **Error Freedom**: `Good` (All obvious errors are removed but there are some smaller issues not rectified -- Program will compile and run)
* **Clarity**: `Excellent` (Program contains appropriate documentation for all major functions, variables, or non-trivial algorithms -- Formatting indentation, and other white space aids readability)

**Grade:** 78.00 / 100.00

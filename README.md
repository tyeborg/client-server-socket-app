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

### <svg viewBox="0 0 128 128">
<path fill="#659AD3" d="M115.4 30.7L67.1 2.9c-.8-.5-1.9-.7-3.1-.7-1.2 0-2.3.3-3.1.7l-48 27.9c-1.7 1-2.9 3.5-2.9 5.4v55.7c0 1.1.2 2.4 1 3.5l106.8-62c-.6-1.2-1.5-2.1-2.4-2.7z"></path><path fill="#03599C" d="M10.7 95.3c.5.8 1.2 1.5 1.9 1.9l48.2 27.9c.8.5 1.9.7 3.1.7 1.2 0 2.3-.3 3.1-.7l48-27.9c1.7-1 2.9-3.5 2.9-5.4V36.1c0-.9-.1-1.9-.6-2.8l-106.6 62z"></path><path fill="#fff" d="M85.3 76.1C81.1 83.5 73.1 88.5 64 88.5c-13.5 0-24.5-11-24.5-24.5s11-24.5 24.5-24.5c9.1 0 17.1 5 21.3 12.5l13-7.5c-6.8-11.9-19.6-20-34.3-20-21.8 0-39.5 17.7-39.5 39.5s17.7 39.5 39.5 39.5c14.6 0 27.4-8 34.2-19.8l-12.9-7.6z"></path>
</svg>

## Project Evaluation

*"Kernel module builds and runs, but it doesn't seem to always get a file, and cannot reconnect. So, generally working and well put together with a few bugs in place."* - Kevin Chalmers 

* **Solution**: `Excellent` (A running solution meeting all the specification)
* **Correctness**: `Good` (Program produces correct answers or appropriate results for the majority of inputs)
* **Logic**: `Excellent` (Program logic is correct with no known boundary errors, and no redundant or contradictory conditions)
* **Robutness**: `Good` (Program handles almost all erroneous or unexpected input gracefully without surprising the user)
* **Error Freedom**: `Good` (All obvious errors are removed but there are some smaller issues not rectified -- Program will compile and run)
* **Clarity**: `Excellent` (Program contains appropriate documentation for all major functions, variables, or non-trivial algorithms -- Formatting indentation, and other white space aids readability)

**Grade:** 78.00 / 100.00

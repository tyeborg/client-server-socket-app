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

<svg xmlns="http://www.w3.org/2000/svg" viewBox="0 0 128 128"><g fill="#181616"><path fill-rule="evenodd" clip-rule="evenodd" d="M64 5.103c-33.347 0-60.388 27.035-60.388 60.388 0 26.682 17.303 49.317 41.297 57.303 3.017.56 4.125-1.31 4.125-2.905 0-1.44-.056-6.197-.082-11.243-16.8 3.653-20.345-7.125-20.345-7.125-2.747-6.98-6.705-8.836-6.705-8.836-5.48-3.748.413-3.67.413-3.67 6.063.425 9.257 6.223 9.257 6.223 5.386 9.23 14.127 6.562 17.573 5.02.542-3.903 2.107-6.568 3.834-8.076-13.413-1.525-27.514-6.704-27.514-29.843 0-6.593 2.36-11.98 6.223-16.21-.628-1.52-2.695-7.662.584-15.98 0 0 5.07-1.623 16.61 6.19C53.7 35 58.867 34.327 64 34.304c5.13.023 10.3.694 15.127 2.033 11.526-7.813 16.59-6.19 16.59-6.19 3.287 8.317 1.22 14.46.593 15.98 3.872 4.23 6.215 9.617 6.215 16.21 0 23.194-14.127 28.3-27.574 29.796 2.167 1.874 4.097 5.55 4.097 11.183 0 8.08-.07 14.583-.07 16.572 0 1.607 1.088 3.49 4.148 2.897 23.98-7.994 41.263-30.622 41.263-57.294C124.388 32.14 97.35 5.104 64 5.104z"/><path d="M26.484 91.806c-.133.3-.605.39-1.035.185-.44-.196-.685-.605-.543-.906.13-.31.603-.395 1.04-.188.44.197.69.61.537.91zm2.446 2.729c-.287.267-.85.143-1.232-.28-.396-.42-.47-.983-.177-1.254.298-.266.844-.14 1.24.28.394.426.472.984.17 1.255zM31.312 98.012c-.37.258-.976.017-1.35-.52-.37-.538-.37-1.183.01-1.44.373-.258.97-.025 1.35.507.368.545.368 1.19-.01 1.452zm3.261 3.361c-.33.365-1.036.267-1.552-.23-.527-.487-.674-1.18-.343-1.544.336-.366 1.045-.264 1.564.23.527.486.686 1.18.333 1.543zm4.5 1.951c-.147.473-.825.688-1.51.486-.683-.207-1.13-.76-.99-1.238.14-.477.823-.7 1.512-.485.683.206 1.13.756.988 1.237zm4.943.361c.017.498-.563.91-1.28.92-.723.017-1.308-.387-1.315-.877 0-.503.568-.91 1.29-.924.717-.013 1.306.387 1.306.88zm4.598-.782c.086.485-.413.984-1.126 1.117-.7.13-1.35-.172-1.44-.653-.086-.498.422-.997 1.122-1.126.714-.123 1.354.17 1.444.663zm0 0"/></g></svg>

## Project Evaluation

*"Kernel module builds and runs, but it doesn't seem to always get a file, and cannot reconnect. So, generally working and well put together with a few bugs in place."* - Kevin Chalmers 

* **Solution**: `Excellent` (A running solution meeting all the specification)
* **Correctness**: `Good` (Program produces correct answers or appropriate results for the majority of inputs)
* **Logic**: `Excellent` (Program logic is correct with no known boundary errors, and no redundant or contradictory conditions)
* **Robutness**: `Good` (Program handles almost all erroneous or unexpected input gracefully without surprising the user)
* **Error Freedom**: `Good` (All obvious errors are removed but there are some smaller issues not rectified -- Program will compile and run)
* **Clarity**: `Excellent` (Program contains appropriate documentation for all major functions, variables, or non-trivial algorithms -- Formatting indentation, and other white space aids readability)

**Grade:** 78.00 / 100.00

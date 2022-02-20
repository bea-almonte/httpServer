# HTTP Server Project - Bea Almonte
## Description

This project implements an HTTP server to be run by a browser client.


**The submission includes the following files:**


|                      |                    |
| -------------------- | -------------------|
| httpServer.cpp       | Makefile           |
| front.html           | favicon.ico        |
| hello-there.html     | hello-there.jpg    |
| general-grevious.gif | bee-movie.html     |
| bee.jpg              | screenCaptures.pdf |
|                      |                    |


## Compiling the Program
- At the command line, issue the command `make httpServer` to compile it automatically
- The program produces an executable entitled `httpServer`
- The command `make clean` will delete the object files and executable

## Running the Program
- Ensure the HTML files and images are in the same directory as the executable
- Use the command `./httpServer [PORT NUMBER]`
- `[PORT NUMBER]` is an optional argument
- `[PORT NUMBER]` must be between  60001 - 60099
- Running it without an argument or invalid argument will set the port to `60001`
- To open the page on the browser go to: `localhost:PORTNUMBER`

## Expected Outputs
- The program will output on the console and send any requested HTML files to the client

### Binding Socket Error
- If the program can't bind socket to a specfic port, it will output an error.
- Run the program again with another valid port number

```
Error: cannot bind socket to port.
```

### Socket Successfully Binded to Port
- The program will output a success message and will be ready to accept connections

```
Server ready and listening.
```

### The Connection Is Established and an HTTP Request Is Sent
- The program will create a new thread to send the requested data
- The data will be displayed on the webpage appropirately.

```
Created thread.

REQUEST: GET /
Attempting to open /

Attempting to transmit 440 bytes.
Successfully transmitted 440 bytes.
```
### The Connection Is Established, But The File Requested Does Not Exist
- The program will attempt to open the file. When it is not succesful, it will send an error message to the console, as well as a 404 Error to the client

```
Created thread.

Attempting to transmit file.

REQUEST: GET /blank.html
Attempting to open blank.html
File blank.html doesn't exist. Sending error message.
````
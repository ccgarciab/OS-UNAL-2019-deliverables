# Manual of Use

## Building the project

Use the "make" command to compile and link sources into the executables _dogClient_ and _dogServer_. This command will also create the necessary files for the execution of the programs.

There are several make targets for your convenience:

* **make clean:** deletes all object and executable files, leaving just the sources and auxiliaries.
* **make tidy:** like **clean**, but spares the executables.
* **make program:** builds the project and then deletes object files.
* **make d:** builds the project with debug symbols for _gdb_ or similar debugers. Run **clean** first.
* **make random:** get ready to generate 10^6 random registers. After this, just run _randomStructs_. The **tidy** and **clean** targets also clean the results of this step.

## Run the project

 1. Run _dogServer_ from the terminal (e.g: ./dogServer).

 2. Run up to 32 instances of _dogClient_, each from it's own terminal window.
 
# Specifications

## Source Files

### Server

- We can find the "p2-dogServer.c" file which works as a server following the Client-Server Architecture

### Client

- We can find the "p2-dogClient.c" file which works as a client following the Client-Server Architecture

## Project Modules

There are nine modules inside the project: pet_file, pet_input, pet_output, pet_hashtable, error_handle, randomStructs, sock_aux, p2-dogServer and p2-dogClient:

- **pet_file:** Module with code that manages the interaction between the pets structs and dataBase (_dataDogs.dat_ file).

- **pet_input:** Module that contains all the functionality for getting input from stdin, including integers, strings and yes/no answers.

- **pet_output:** Module with the main functions to print to stdout the pet data and open the medical records with file editors.

- **pet_hashtable:** Module with all the functions that interact with the hash table.

- **randomStructs:** Module where the random data generator code can be found.

- **error_handle:** Module for abbreviating error handling of _clib_ functions, particularly if they make use of _errno_.

- **sock_aux:** Module for managing the sending and receiving of fixed lenght buffers, files and streams through sockets.

- **p2-dogServer:** Module with the Server logic of the application.

- **p2-dogClient:** Module with the Client logic of the application.

## Notes

* **The headers (Files with ".h" extension) can be found in the "include" folder**

* **Text files are really important, because without those the data generation of the "dataDogs.dat" file will fail. Using those files we randomly select data for each one that will compose the structures found in the said file**

* **The code used for the randomly generation of data can be found in the "generation" folder**

Made by: ccgarciab-jjpulidos

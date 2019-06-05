% dogClient(1) Version 1.0 | dataDog suite

NAME
====

**dogClient** — Client for remote pet database.

SYNOPSIS
========

| **./dogClient** 

DESCRIPTION
===========

Offers command line functionality for connecting with a server executed with
_dogServer(1)_. These 2 programs are intended to manage data for pets, for usage
in pet stores, veterinaries, and so on. The _dogClient_ program provides
support for basic CRUD operations and fetching of a batch of pet information
selected by name.

When executed _dogClient_ will prompt the user with five options as follows:

 1. **Insert a Register:** prompts the user for information. Builds and 
    sends a record.

 2. **Visualize a Register:** Fetches a record by the line number it currently 
    occupies in the database, then shows it. Optionally downloads and displays
    the medical record for the register, and allows remote edition of it.

 3. **Erase a Register:** Deletes a record identified by the line number it 
    currently occupies in the database.

 4. **Search a Register:** Search all registers with certain name, and prints
    them to screen.

 5. **Quit:** Closes safely the connection with the server.

FILES
=====

*./client/\[number\].temp*

:   Temporary files for medical record edition.

BUGS
====

Hopefully none :^)

AUTHOR
======

Cristian Camilo García Barrera. \
Juan Jesus Pulido Sanchez.

Contact at: {ccgarciab, jjpulidos}@unal.edu.co

SEE ALSO
========

**dogServer(1)**

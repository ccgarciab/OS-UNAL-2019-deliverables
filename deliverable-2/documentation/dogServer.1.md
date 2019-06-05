% dogServer(1) Version 1.0 | dataDog suite

NAME
====

**dogServer** — Server for pet information database.

SYNOPSIS
========

| **./dogServer** 

DESCRIPTION
===========

Offers command line functionality for connecting with up to 32 clients executed 
with _dogClient(1)_. These 2 programs are intended to manage data for pets, for 
usage in pet stores, veterinaries, and so on. The _dogServer_ program manages
the commands given to each _dogClient_ instance and logs the CRUD and selection
operations queried by them.

FILES
=====

*./server/\[number\].txt*

:   Permanent files storing the medical records.

*.dataDogs.dat*

:   Database file that stores the actual records.

*./serverDogs.log*

:   File for logging all activity

BUGS
====

Not thread safe by any measure.

AUTHOR
======

Cristian Camilo García Barrera. \
Juan Jesus Pulido Sanchez.

Contact at: {ccgarciab, jjpulidos}@unal.edu.co

SEE ALSO
========

**dogServer(1)**


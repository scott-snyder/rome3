This directory contains an example for showing how to borrow MIDASDAQ class from a ROME project and use it for a standalone program to read MIDAS files in offline.

Usage: (for bash)
$ export LIBROME=yes
$ make -C $ROMESYS
$ cd ..
$ $ROMESYS/bin/romebuilder.exe -i midas.xml -o .
$ make
$ cd -
$ make .
$ ./analyzer

In this example, a generated class MIDMidasDAQ (the source is in ../src/generated/) is used; therefore the bank structure is already defined in ../midas.xml.


If you prefer to use a lower level handle for MIDAS files, you may choose to use ROMEMidasFile class in the $ROMESYS/src directory instead of the method shown in this example.

This example shows how to use ROME to analyze midas data.

First of all, please download 'midas' (https://midas.triumf.ca) and set an environment variable MIDASSYS pointing to the directory where you have the midas package.
Go to examples/experiment in midas and compile it.
Then start 'frontend' there. You can start/stop a run from 'odbedit' program with start/stop command.
Midas banks created by midas (ADC, TDC etc.) are defined in frontend.c in the example.

Now you build the ROME based analysis framework in this directory (where this README.txt is located).
   $ROMESYS/bin/romebuilder.exe -i midas.xml -o . -midas
Midas banks are defined in midas.xml

After the compile is finished you can start the analyzer in online mode,
   ./midanalyzer.exe -i romeConfig.xml
The analyzer watches the status of the frontend. When you start a run (e.g. from odbedit), the analyzer will start analyzing the online data and ROOT output files will be created.

If you run mlogger (e.g. $MIDASSYS/linux/bin/mlogger), a midas file is created after each run, by default in $MIDASSYS.
You can analyze the midas files with using the same configuration file,
   ./midanalyzer.exe -i romeConfig.xml -pi $MIDASSYS -r 1 -m offline

Output ROOT files can be analyzed with using "./midanalyzer.exe -I" or plain root program.

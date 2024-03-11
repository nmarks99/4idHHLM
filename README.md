# 4id High Heat Load Mirrors

The soft IOC perl script `./iocBoot/ioc4idHHLM/softioc/4idHHLM.pl` can be used for starting/stopping the IOC.
Run this script without arguments to see available options.

To start the MEDM screens for the mirrors, run `./start_MEDM_mirrors`, or select "HHLM" under "Mirror"
in the 4idHHLM.adl which you can start by running `./start_MEDM_4idHHLM`. The MEDM screen has also been
autoconverted to caQtDM. To use the caQtDM screen, select the HHLM box under the Optics tab on the
4idHHLM.ui screen which you can start by running `./start_caQtDM_4idHHLM`. If you make a change to the MEDM
screen, you can autoconvert it to caQtDM by running `make` in the ./4idHHLMApp/op/adl/ directory.

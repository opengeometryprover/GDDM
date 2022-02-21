#!/bin/bash
EXE=gclc
URL=http://poincare.matf.bg.ac.rs/~janicic/gclc/linux-gclc.zip
FOLDER=linux-gclc
LONGEXE=$FOLDER/gclc
ZIPFILE=$FOLDER.zip
wget -c -N $URL -O $ZIPFILE
test -r $LONGEXE || unzip $ZIPFILE
test -r $EXE || ln -s $LONGEXE $EXE

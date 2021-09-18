#!/bin/bash
EXE=vampire
LONGEXE=vampire_z3_Release_static_master_4764
ZIPFILE=$LONGEXE.zip
URL=https://github.com/vprover/vampire/releases/download/4.5.1/$ZIPFILE
wget -c -N $URL -O $ZIPFILE
test -r $LONGEXE || unzip $ZIPFILE
test -r $EXE || ln -s $LONGEXE $EXE

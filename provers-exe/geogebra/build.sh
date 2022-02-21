#!/bin/bash
EXE=geogebra
VERSION=5.0.641.0-2021Sep03
FOLDER=GeoGebra-Discovery-$VERSION
LONGEXE=$FOLDER/GeoGebra-Discovery
ZIPFILE=$FOLDER-Linux64.zip
URL=https://github.com/kovzol/geogebra/releases/download/v$VERSION/$ZIPFILE
wget -c -N $URL -O $ZIPFILE
test -r $LONGEXE || unzip $ZIPFILE
test -r $EXE || ln -s $LONGEXE $EXE

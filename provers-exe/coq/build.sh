#!/bin/bash
VERSION=8.8.0
opam install coq
URL=https://github.com/coq-contribs/area-method/archive/refs/tags/v$VERSION.zip
ZIPFILE=$VERSION.zip
FOLDER=area-method-$VERSION
wget -c -N $URL -O $ZIPFILE
test -d $FOLDER || unzip $ZIPFILE
cd $FOLDER && make

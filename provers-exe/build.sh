#!/bin/bash
for d in coq gclc geogebra vampire; do
 cd $d
 ./build.sh
 cd ..
 done

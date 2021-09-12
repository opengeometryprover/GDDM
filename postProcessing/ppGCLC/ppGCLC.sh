#!/bin/sh

if grep -q 'The conjecture successfully proved' gclc.log
then
    echo Proved > "$2".result
else
    echo Unkown > "$2".result
fi

exit 0

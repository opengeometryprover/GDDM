#!/bin/sh

if grep -q 'The conjecture successfully proved' gclc.log
then
    echo Proved > "$1".result
else
    echo Unkown > "$1".result
fi

exit 0

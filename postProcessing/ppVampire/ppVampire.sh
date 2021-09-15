#!/bin/sh

if grep -q 'Termination reason: Refutation' "$1".out
then
    echo Proved > "$1".result
elif grep -q 'Termination reason: Satisfiable' "$1".out
then
    echo Proved > "$1".result
else
    echo Unkown > "$1".result
fi

exit 0

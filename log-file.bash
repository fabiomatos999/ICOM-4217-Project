#!/usr/bin/env bash

#"(stty raw; cat > ~/power.log) < #1 &"
#"(stty raw; cat > ~/error.log) < #2 &"

while true
do
    PREVNR="$(awk 'END{print NR}' ~/error.log)"
    sleep 20s
    DATE="$(date '+%Y %m %d %H:%M:%S')"
    POWERSTATUS="$(tail -n 1 ~/power.log)"
    ERROR="$(tail -n 1 ~/error.log)"
    NR="$(awk 'END{print NR}' ~/error.log)"
    if [ "$PREVNR" == "$NR" ]; then
        echo "$DATE $POWERSTATUS"
    else
        echo "$DATE $POWERSTATUS $ERROR"
    fi
done

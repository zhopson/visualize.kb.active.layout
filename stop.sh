#!/bin/bash
PID=`ps -ax | grep visual-kblang | grep -v grep | awk '{print $1}'`
if [ -z "$PID" ]
then 
    echo "Процесс не запущен"
    exit 1
else 
    echo $PID
    RESULT=`kill $PID`
    echo $RESULT
    exit 0
fi

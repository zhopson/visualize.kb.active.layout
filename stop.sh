#!/bin/bash
AP=/home/mikhailovan@Elektra.local/soft/v.color.lang
APPLICATION=visual-kblang
PID=`ps -ax | grep visual-kblang | grep -v grep | awk '{print $1}'`
NOW=`date '+%d-%m-%Y %H:%M:%S'`
if [ -z "$PID" ]
then
    echo "$NOW Процесс не запущен" >> $AP/$APPLICATION.log
    exit 1
else
    echo "$NOW Запущенный процесс: $PID" >> $AP/$APPLICATION.log
    RESULT=`kill $PID`
    if [ -n "$RESULT" ]; then
      echo "$NOW $RESULT" >> $AP/$APPLICATION.log
    fi
    echo "$NOW Программа остановлена" >> $AP/$APPLICATION.log
    exit 0
fi

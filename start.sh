#!/bin/bash
AP=/home/mikhailovan@Elektra.local/soft/v.color.lang
APPLICATION=visual-kblang
PID=`ps -ax | grep $APPLICATION | grep -v grep | awk '{print $1}'`
NOW=`date '+%d-%m-%Y %H:%M:%S'`
if [ -z "$PID" ] # если не запущен
then
    if [ -f /tmp/vlang.lock ] # файл блокировки существует
    then
	echo "$NOW Файл блокировки существует, удаляем..." >> $AP/$APPLICATION.log
	rm -rf /tmp/vlang.lock
    fi
    RESULT=`$AP/$APPLICATION > $AP/$APPLICATION-run.log 2>&1 &`
    #echo $RESULT > $AP/pid.txt
    echo "$NOW Запуск программы" >> $AP/$APPLICATION.log
    if [ -n "$RESULT" ]; then
        echo "$NOW $RESULT" >> $AP/$APPLICATION.log
    fi
    exit 0
else
    echo "$NOW Процесс уже запущен с PID: $PID" >> $AP/$APPLICATION.log
    exit 1
fi









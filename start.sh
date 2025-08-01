#!/bin/bash
AP=/home/mikhailovan@Elektra.local/soft/v.color.lang
APPLICATION=visual-kblang
PID=`ps -ax | grep $APPLICATION | grep -v grep | awk '{print $1}'`

if [ -z "$PID" ] # если не запущен
then
    if [ -f /tmp/vlang.lock ] # файл блокировки существует
    then
	echo "Файл блокировки существует, удаляем..."
	rm -rf /tmp/vlang.lock
    fi
    RESULT=`$AP/$APPLICATION > $AP/$APPLICATION.log 2>&1 &`
    #echo $RESULT > $AP/pid.txt
    echo "Запуск программы"
    echo $RESULT
    exit 0
else
    echo "Процесс уже запущен с PID: $PID"
    exit 1
fi









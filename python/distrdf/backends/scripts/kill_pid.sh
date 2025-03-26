#! /usr/bin/env bash

logfile=$1
if [ ! -f $logfile ]; then
    echo "log file not found, cannot retrieve PID!"
    exit 1
fi

PID=$(< $logfile)
kill -9 $PID

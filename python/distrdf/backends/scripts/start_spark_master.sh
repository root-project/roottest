#! /usr/bin/env bash

spark_class_cmd=$1
$spark_class_cmd org.apache.spark.deploy.master.Master -h 127.0.0.1 2>~/spark_master.err 1>~/spark_master.out 0</dev/null &
pid=$!
sleep 1 # spare time to launch the master process
echo $pid

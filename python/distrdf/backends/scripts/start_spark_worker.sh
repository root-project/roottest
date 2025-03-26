#! /usr/bin/env bash

spark_class_cmd=$1
work_dir=$2
$spark_class_cmd org.apache.spark.deploy.worker.Worker spark://127.0.0.1:7077 -c 12 -m 6G --work-dir $work_dir 2>~/spark_worker.err 1>~/spark_worker.out 0</dev/null &
pid=$!
echo $pid

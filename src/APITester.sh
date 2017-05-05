#!/bin/sh

url="http://localhost:3000"
method=$1
path=$2

if [ $# = 3 ];
then
    curl -X $method --header "Content-Type: application/json" --data $3 $url$path
else
    curl -X $method $url$path
fi

#!/bin/sh

#url="http://39.108.7.208:3000"
url="http://localhost:3000"
method=$1
path=$2

if [ $# = 3 ];
then
    curl -sX $method --header "Content-Type: application/json" --data $3 $url$path | python -mjson.tool
else
    curl -sX $method $url$path | python -mjson.tool
fi

#!/bin/bash

echo "Integration Test MultiThread ..."


echo "Starting Server ..."
../build/bin/main ../server_config_mult &

sleep 1

nc localhost 8080 &

curl -i -s http://localhost:8080/static/blah.txt -o static_reply.txt

echo "Shutting Down Server ..."
pkill -f main 

diff -s ./static_reply.txt ./blah.txt &>/dev/null
if [ $? -eq 0 ]
then
    echo "Integration Test MultiThread Passed."
fi

rm ./static_reply.txt

exit $?

#!/bin/bash

echo "Integration Test Meme ..."


echo "Starting Server ..."
../build/bin/main ../server_config_mult &

sleep 1

nc localhost 8080 &

curl http://localhost:8080/meme/list > ./originalList.html
m_id1=`curl -d memeselect=simply.jpg -d top=testing123 -d bot=testing987 http://localhost:8080/meme/create | grep -oP '(?<=">)[^"]*(?=</a>)'`
curl http://localhost:8080/meme/delete?id=${m_id1}
curl http://localhost:8080/meme/list > ./newList.html

diff -s ./originalList.html ./newList.html &>/dev/null
if [ $? != 0 ]
then
	echo "Meme Deletion Integreation Test Failed."
	exit 1
fi

echo "Shutting Down Server ..."
pkill -f main 

rm ./originalList.html
rm ./newList.html

echo "Meme Deletion Integration Test Passed!"

exit $?

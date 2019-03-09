#!/bin/bash

echo "Integration Test Meme ..."


echo "Starting Server ..."
../build/bin/main ../server_config_mult &

sleep 1

nc localhost 8080 &

m_id1=`curl -d memeselect=simply.jpg -d top=testing123 -d bot=testing987 http://localhost:8080/meme/create | grep -oP '(?<=">)[^"]*(?=</a>)'`
curl http://localhost:8080/meme/view?id=${m_id1} > ./test1.html

diff -s ./test1.html ./meme1.html &>/dev/null
if [ $? != 0 ]
then
	echo "Correct Test Failed."
	exit 1
fi

sleep 1

m_id2=`curl -d memeselect=simply.jpg -d top= -d bot= http://localhost:8080/meme/create | grep -oP '(?<=">)[^"]*(?=</a>)'`
curl http://localhost:8080/meme/view?id=${m_id2} > ./test2.html

diff -s ./test2.html ./404.html &>/dev/null
if [ $? != 0 ]
then
	echo "Blank Input Test Failed."
	exit 1
fi

sleep 1

m_id3=`curl -d memeselect=simply.jpg -d top=##Å% -d bot=¶asdf http://localhost:8080/meme/create | grep -oP '(?<=">)[^"]*(?=</a>)'`

curl http://localhost:8080/meme/view?id=${m_id3} > ./test3.html

diff -s ./test3.html ./404.html &>/dev/null
if [ $? != 0 ]
then
	echo "Special Character Test Failed."
	exit 1
fi

echo "Shutting Down Server ..."
pkill -f main 

rm ./test1.html
rm ./test2.html
rm ./test3.html

echo "Meme Integration Test Passed!"

exit $?

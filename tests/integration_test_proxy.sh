#!/bin/bash

echo "Proxy Integration Test ..."

echo "Starting Proxy Server ..."
./build/bin/main ./tests/proxy_integration_config_proxy &

echo "Fetching file through proxy..."
curl -i http://localhost:5000/proxy > ./tests/proxy.txt

echo "Fetching file directly from source..."
curl -i http://www.columbia.edu/~fdc/sample.html > ./tests/source.txt

echo "Shutting Down Server ..."
pkill -f main 

DIFF=$(diff -w -B ./tests/exp_proxy.txt ./tests/proxy.txt)
EXIT_STATUS=$? 

if [ "$EXIT_STATUS" -eq 0 ]
then
    echo "SUCCESS: Echo handler integration test passed"
else
    echo "FAILED: Echo handler integration test failed"
    echo $DIFF
fi 

rm -f ./tests/source.txt

exit $?

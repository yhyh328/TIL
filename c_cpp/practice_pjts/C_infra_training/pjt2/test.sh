#!/bin/bash

echo "=== Building pjt2 ==="
make clean
make

if [ $? -ne 0 ]; then
    echo "Build failed!"
    exit 1
fi

echo ""
echo "=== Starting server in background ==="
./server > server.log 2>&1 &
SERVER_PID=$!
sleep 2  # Wait for server to start

if ! kill -0 $SERVER_PID 2>/dev/null; then
    echo "ERROR: Server failed to start!"
    cat server.log
    exit 1
fi

echo "Server started (PID: $SERVER_PID)"
echo ""
echo "=== Testing client connection ==="
echo -e "test message\n" | ./client

echo ""
echo "=== Checking server output ==="
sleep 1
if [ -f server.log ]; then
    cat server.log
fi

echo ""
echo "=== Stopping server ==="
kill $SERVER_PID 2>/dev/null
wait $SERVER_PID 2>/dev/null
rm -f server.log

echo "Test completed!"


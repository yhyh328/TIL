#!/bin/sh
# wait-for-it.sh: wait for a host and port to be available before executing a command

set -e

host="$1"
shift
port="$1"
shift

until nc -z "$host" "$port"; do
  >&2 echo "Backend is unavailable - sleeping"
  sleep 1
done

>&2 echo "Backend is up - executing command"
exec "$@"

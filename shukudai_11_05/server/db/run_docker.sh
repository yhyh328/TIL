#!/usr/bin/env bash
set -euo pipefail

IMAGE_NAME="shukudai_pg:1.0"
BASE_PORT=5432
MAX_PORT=5500
DB_NAME="shukudai_11_06"
BOOT_USER="myuser"
BOOT_PASS="mypass"

echo "🔍 Searching for a free port between $BASE_PORT and $MAX_PORT..."

is_port_free() {
  local port="$1"
  if command -v ss >/dev/null 2>&1; then
    ! ss -ltn | awk '{print $4}' | grep -q ":$port$"
  elif command -v netstat >/dev/null 2>&1; then
    ! netstat -ltn | awk '{print $4}' | grep -q ":$port$"
  else
    # very dumb fallback (assume free)
    return 0
  fi
}

for PORT in $(seq "$BASE_PORT" "$MAX_PORT"); do
  if is_port_free "$PORT"; then
    echo "✅ Found free port: $PORT"
    CONTAINER_NAME="shukudai_pg_${PORT}"
    VOLUME_NAME="pgdata_${PORT}"

    echo "🚀 Starting Docker container: $CONTAINER_NAME"
    docker run -d \
      --name "$CONTAINER_NAME" \
      -e POSTGRES_USER="$BOOT_USER" \
      -e POSTGRES_PASSWORD="$BOOT_PASS" \
      -e POSTGRES_DB="$DB_NAME" \
      -p "${PORT}:5432" \
      -v "${VOLUME_NAME}:/var/lib/postgresql/data" \
      "$IMAGE_NAME" >/dev/null

    echo "✅ Container '$CONTAINER_NAME' is running on port $PORT"
    echo "   Volume: $VOLUME_NAME"
    echo
    echo "To connect (bootstrap superuser):"
    echo "  PGPASSWORD=${BOOT_PASS} psql -h 127.0.0.1 -p ${PORT} -U ${BOOT_USER} -d ${DB_NAME}"
    echo "To connect (app user from init.sql):"
    echo "  PGPASSWORD=password psql -h 127.0.0.1 -p ${PORT} -U appuser -d ${DB_NAME}"
    echo

    # write helper env file
    cat > .env.docker <<EOF
PGUSER=appuser
PGPASSWORD=password
PGHOST=127.0.0.1
PGPORT=${PORT}
PGDATABASE=${DB_NAME}
EOF
    echo "📝 Wrote .env.docker (use it in your Node app)."
    exit 0
  fi
done

echo "❌ No free port found between $BASE_PORT and $MAX_PORT."
exit 1

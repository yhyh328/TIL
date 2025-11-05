#!/usr/bin/env bash
set -euo pipefail

DBUSER_DEFAULT="myuser"
DBNAME_DEFAULT="shukudai_11_06"

usage() {
  cat <<'EOF'
Usage:
  pg_controller.sh --enter [PORT]          # psql into the container as myuser
  pg_controller.sh --psql [PORT] [USER] [DB]
  pg_controller.sh --logs [PORT]           # follow logs
  pg_controller.sh --remove [PORT]         # stop & remove the container

Notes:
  - Containers are named like: shukudai_pg_<PORT>
  - If PORT is omitted, the most recent running shukudai_pg_* is used.
EOF
}

# pick container by port, or most recent shukudai_pg_*
pick_container() {
  local port="${1:-}"
  if [[ -n "${port}" ]]; then
    echo "shukudai_pg_${port}"
    return
  fi
  docker ps --format '{{.Names}}' \
    | grep -E '^shukudai_pg_[0-9]+$' \
    | head -n1
}

cmd="${1:-}"; shift || true
case "$cmd" in
  --enter)
    port="${1:-}"; shift || true
    container="$(pick_container "$port")"
    [[ -z "$container" ]] && { echo "No shukudai_pg_* container running."; exit 1; }
    echo "🚀 Entering PostgreSQL in '$container'..."
    docker exec -it "$container" psql -U "${DBUSER_DEFAULT}" -d "${DBNAME_DEFAULT}"
    ;;

  --psql)
    port="${1:-}"; shift || true
    user="${1:-$DBUSER_DEFAULT}"; shift || true
    db="${1:-$DBNAME_DEFAULT}"; shift || true
    container="$(pick_container "$port")"
    [[ -z "$container" ]] && { echo "No shukudai_pg_* container running."; exit 1; }
    echo "🚀 psql -U ${user} -d ${db} inside '$container'..."
    docker exec -it "$container" psql -U "$user" -d "$db"
    ;;

  --logs)
    port="${1:-}"; shift || true
    container="$(pick_container "$port")"
    [[ -z "$container" ]] && { echo "No shukudai_pg_* container running."; exit 1; }
    echo "📜 Following logs for '$container'..."
    docker logs -f "$container"
    ;;

  --remove)
    port="${1:-}"; shift || true
    container="$(pick_container "$port")"
    [[ -z "$container" ]] && { echo "No shukudai_pg_* container running."; exit 1; }
    echo "🗑️ Removing container '$container'..."
    docker rm -f "$container"
    echo "✅ Removed."
    ;;

  *)
    usage; exit 1 ;;
esac

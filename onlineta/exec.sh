#!/usr/bin/env bash

USER=$(whoami)
DIR="measure"

MEMCG="$(mktemp -d -p "cgroup/memory/onlineta/" \
  "$USER-exec-XXXXXX")"

for src in "$DIR/memory/"* ; do
  dst="$MEMCG/$(basename "$src" | sed 's/max_usage/limit/')"
  /bin/echo "$(( $(cat "$src") + 100 ))" > "$dst"
done

cat "$MEMCG/memory.limit_in_bytes"
cat "$MEMCG/memory.memsw.limit_in_bytes"
#echo 1 > "$MEMCG/memory.use_hierarchy"
#echo 0 > "$MEMCG/memory.swappiness"

./libexec "$MEMCG/tasks" "$1"

cat "$MEMCG/memory.failcnt"
cat "$MEMCG/memory.stat"
cat "$MEMCG/memory.use_hierarchy"

rmdir "$MEMCG"

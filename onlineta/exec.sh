#!/usr/bin/env bash

USER=$(whoami)
DIR="measure"

MEMCG="$(mktemp -d -p "/sys/fs/cgroup/memory/onlineta/" \
  "$USER-exec-XXXXXX")"

for src in "$DIR/memory/"* ; do
  dst="$MEMCG/$(basename "$src" | sed 's/max_usage/limit/')"
  /bin/echo "$(( $(cat "$src") + 100 ))" > "$dst"
done

echo 1 > "$MEMCG/memory.use_hierarchy"
echo 0 > "$MEMCG/memory.swappiness"

/usr/bin/env sh -c "echo \$\$ >> \"$MEMCG/tasks\" && $1"

cat "$MEMCG/memory.failcnt"
#cat "$MEMCG/memory.stat"

rmdir "$MEMCG"

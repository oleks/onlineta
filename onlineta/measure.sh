#!/usr/bin/env sh

cp_if_exists()
{
  if [ -e "$1" ]; then
    cp "$1" "$2"
  fi
}

USER=$(whoami)
DIR="measure"

rm -rf "$DIR"
mkdir "$DIR"

MEMCG="$(mktemp -d -p "/sys/fs/cgroup/memory/onlineta/" \
  "$USER-measure-XXXXXX")"

/usr/bin/env sh -c "echo \$\$ >> \"$MEMCG/tasks\" && $1"

mkdir "$DIR/memory"

cp_if_exists "$MEMCG/memory.max_usage_in_bytes"          "$DIR/memory"
#cp_if_exists "$MEMCG/memory.memsw.max_usage_in_bytes"    "$DIR/memory"
#cp_if_exists "$MEMCG/memory.kmem.max_usage_in_bytes"     "$DIR/memory"
#cp_if_exists "$MEMCG/memory.kmem.tcp.max_usage_in_bytes" "$DIR/memory"

for file in "$DIR/memory"/*; do
  echo $(basename "$file")
  cat "$file"
done

#cat "$MEMCG/memory.stat"

rmdir "$MEMCG"

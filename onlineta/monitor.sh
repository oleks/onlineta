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

MEMCG="$(mktemp -d -p "cgroup/memory/onlineta/" \
  "$USER-monitor-XXXXXX")"
CPUCG="$(mktemp -d -p "cgroup/cpu/onlineta/" \
  "$USER-monitor-XXXXXX")"

./libexec -g "$MEMCG/tasks" -g "$CPUCG/tasks" "$@" || exit 1

#echo "echo \$\$ >> $MEMCG/tasks && ""$@"
#/usr/bin/env sh -c "echo \$\$ >> $MEMCG/tasks && echo \$\$ >> $CPUCG/tasks && ""$@"

mkdir "$DIR/memory"
mkdir "$DIR/cpu"

cp_if_exists "$MEMCG/memory.max_usage_in_bytes"          "$DIR/memory"
cp_if_exists "$MEMCG/memory.memsw.max_usage_in_bytes"    "$DIR/memory"
#cp_if_exists "$MEMCG/memory.kmem.max_usage_in_bytes"     "$DIR/memory"
#cp_if_exists "$MEMCG/memory.kmem.tcp.max_usage_in_bytes" "$DIR/memory"

for file in "$DIR/memory"/*; do
  echo $(basename "$file")
  cat "$file"
done

cp_if_exists "$CPUCG/cpuacct.usage" "$DIR/cpu"

rmdir "$MEMCG"

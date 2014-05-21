#!/usr/bin/env bash

# Demonstrating how to create a container with a scratchspace accessible from
# the host, such that the host can save the data left behind by the container
# once it finishes. See also ./jail-init.sh.

USER=$(whoami)
DIR="measure"

#MEMCG="$(mktemp -d -p "cgroup/memory/onlineta/" \
#  "$USER-exec-XXXXXX")"

#for src in "$DIR/memory/"* ; do
#  dst="$MEMCG/$(basename "$src" | sed 's/max_usage/limit/')"
#  /bin/echo "$(( $(cat "$src") * 2 ))" > "$dst"
#done

#CPU_TIME=$(cat "$DIR/cpu/cpuacct.usage")
#SECONDS=$(( $CPU_TIME / 1000000000 ))
#if [ "$SECONDS" -eq "0" ]; then # Support kernels < 2.6.17
#  SECONDS=1
#fi

mount -t tmpfs -o size=1M tmpfs output

unshare -minpuf -- \
  ./jail-init.sh
#  ./libexec -g "$MEMCG/tasks" -t $(( $SECONDS * 2 )) "$@"

# tar up output before umount!

umount output

#rmdir "$MEMCG"

#!/usr/bin/env sh

if [ $(id -u) != 0 ]; then
  echo "$0 must be run as root."
  exit 1
fi

./setup-cgroups.sh

TIMEOUT=$1

# Some useful variables.
CGROUPS_DIR="cgroups-target"
MEMCG="$CGROUPS_DIR/memory/onlineta"
CPUACCT="$CGROUPS_DIR/cpu,cpuacct/onlineta"

# A file to save time data into.
TIME_FILE=$(mktemp "tmp.onlineta.XXXXXXXXXXXX")

# Run the test.
# Timeout skews CPU time measurements.
if [ -z "$TIMEOUT" ]; then
  time -p --output="$TIME_FILE" ./priv-test.sh
else
  timeout "$TIMEOUT" time -p --output="$TIME_FILE" ./priv-test.sh
fi

# Report

TIME__WALL_CLOCK=$(cat "$TIME_FILE" | head -n 1 | cut -d ' ' -f 2)
if [ -z "$TIME__WALL_CLOCK" ]; then
  TIME__WALL_CLOCK="timeout"
fi
echo "Wall-clock time (seconds): $TIME__WALL_CLOCK"

MEMCG__MAX_USAGE_IN_BYTES=$(cat "$MEMCG/memory.max_usage_in_bytes")
echo "Max memory use (bytes): $MEMCG__MAX_USAGE_IN_BYTES"

CPUACCT__USAGE=$(cat "$CPUACCT/cpuacct.usage")
echo "Total CPU time (nanoseconds): $CPUACCT__USAGE"

CPUACCT__STAT=$(cat "$CPUACCT/cpuacct.stat")

CPUACCT__STAT_USER=$(echo "$CPUACCT__STAT" | head -n 1 | cut -d ' ' -f 2)
echo "Total CPU time in user mode (USER_HZ): $CPUACCT__STAT_USER"

CPUACCT__STAT_KERNEL=$(echo "$CPUACCT__STAT" | tail -n 1 | cut -d ' ' -f 2)
echo "Total CPU time in kernel mode (USER_HZ): $CPUACCT__STAT_KERNEL"

rm "$TIME_FILE"

./teardown-cgroups.sh

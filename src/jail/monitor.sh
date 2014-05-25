#!/usr/bin/env sh

if [ $(id -u) != 0 ]; then
  echo "$0 must be run as root."
  exit 1
fi

#############
# Arguments #
#############

while getopts "m:t:w:" arg
do
  case $arg in
  m)  LIMIT_MEMORY="$OPTARG";;
  t)  LIMIT_CPU_TIME="-t $OPTARG";;
  w)  LIMIT_WALL_CLOCK="timeout $OPTARG";;
      # OBS! timeout(1) skews CPU time measurements.
  ?)  echo "Usage: ./$0"
      echo "  [-w <timeout>]"
      echo "  [-m <memlimit-in-bytes>]"
      echo "  [-t <cpu-time-limit-in-seconds>]"
      exit 1
      ;;
  esac
done

#########
# Setup #
#########

./setup-cgroups.sh

# Some useful variables.
CGROUPS_DIR="cgroups-target"
MEMCG="$CGROUPS_DIR/memory/onlineta"
CPUACCT="$CGROUPS_DIR/cpu,cpuacct/onlineta"

# Apply the memory limit, if specified.
if [ -n "$LIMIT_MEMORY" ]; then
  /bin/echo "$LIMIT_MEMORY" > "$MEMCG/memory.limit_in_bytes"
fi

# A file to save time data into.
TIME_FILE=$(mktemp "tmp.onlineta.XXXXXXXXXXXX")

#######
# Run #
#######

$LIMIT_WALL_CLOCK time -p --output="$TIME_FILE" ./priv-test.sh $LIMIT_CPU_TIME

##########
# Report #
##########

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

############
# Teardown #
############

rm "$TIME_FILE"

./teardown-cgroups.sh

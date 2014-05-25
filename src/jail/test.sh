#!/usr/bin/env sh

#############
# Arguments #
#############

while getopts "t:" arg
do
  case $arg in
  t)  LIMIT_CPU_TIME="-t $OPTARG";;
  ?)  echo "Usage: ./$0"
      echo "  [-t <cpu-time-limit-in-seconds>]"
      exit 1
      ;;
  esac
done

#########
# Setup #
#########

mount -t tmpfs -o size=1M tmpfs "tmpfs-target" || exit 1
cp -r "input/"* "tmpfs-target/" || exit 1

#######
# Run #
#######

./mnt \
  ./mount-rootfs \
  ./minpuf \
  ./cgroups \
    -g ./cgroups-target/memory/onlineta/tasks \
    -g ./cgroups-target/cpu,cpuacct/onlineta/tasks \
    -g ./cgroups-target/devices/onlineta/tasks \
  ./pivot-root \
  /bin/.umount-oldroot \
  /bin/.setuid \
  /bin/.rlimits $LIMIT_CPU_TIME \
  /home/student/hej

############
# Teardown #
############

rm -rf "output/"* || exit 1
cp -r "tmpfs-target/"* "output/" || exit 1
umount "tmpfs-target" || exit 1

#!/bin/bash

# args:
# $1 - mem_limit (not used at the moment)
# $2 - core usage percentage (e.g. 0.1)
# $3 - container name
GROUP_ID=$3
GROUP_PATH="/sys/fs/cgroup"

mount -t tmpfs -o rw,nosuid,nodev,noexec,relatime,size=0k cgroup_root $GROUP_PATH
mount -t cgroup2 none $GROUP_PATH
if [ $? != "0" ]; then
    mkdir "$GROUP_PATH/$GROUP_ID"
    chmod 777 "$GROUP_PATH/$GROUP_ID" -R
    mount -t cgroup -o cpu,cpuset,cpuacct,memory cgroup_cpu "$GROUP_PATH/$GROUP_ID"
    echo 100000 > "$GROUP_PATH/$GROUP_ID/cpu.cfs_quota_us"
    echo $((100000/100*$2)) > "$GROUP_PATH/$GROUP_ID/cpu.cpu.cfs_period_us"
    cpus_count=$(grep -c processor /proc/cpuinfo)
    rand_cpu=$(( $RANDOM % $cpus_count ))
    echo "$rand_cpu" > "$GROUP_PATH/$GROUP_ID/cpuset.cpus"
    echo $(($1 * 1024)) > "$GROUP_PATH/$GROUP_ID/memory.limit_in_bytes"
    echo $(($1 * 1024)) > "$GROUP_PATH/$GROUP_ID/memory.memsw.limit_in_bytes"
else 
    {
    echo "+cpu" 
    echo "+cpuset" 
    echo "+memory"
    } >> "$GROUP_PATH/cgroup.subtree_control"
    mkdir "$GROUP_PATH/$GROUP_ID"
    chmod 777 "$GROUP_PATH/$GROUP_ID" -R
    cpu_count=$(grep -c processor /proc/cpuinfo)
    rand_cpu=$(( $RANDOM % $cpu_count ))
    echo "$rand_cpu" > "$GROUP_PATH/$GROUP_ID/cpuset.cpus"
    echo "$((1000000/100*$2)) 1000000" > "$GROUP_PATH/$GROUP_ID/cpu.max"
    echo 0 > "$GROUP_PATH/$GROUP_ID/memory.swap.max"
    echo $(($1 * 1024)) > "$GROUP_PATH/$GROUP_ID/memory.max"


fi
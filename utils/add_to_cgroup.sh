#!/bin/bash
pidt=$1
GROUP_ID=$2
GROUP_PATH="/sys/fs/cgroup"
echo "$pidt" > "$GROUP_PATH/$GROUP_ID/cgroup.procs"
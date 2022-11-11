#!/bin/bash

GROUP_ID=$1
GROUP_PATH="/sys/fs/cgroup"
umount "$GROUP_PATH/$GROUP_ID/"
rmdir "$GROUP_PATH/$GROUP_ID/"
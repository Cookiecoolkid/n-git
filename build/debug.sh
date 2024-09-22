#!/bin/bash

# 检查是否安装了 inotify-tools
if ! command -v inotifywait &> /dev/null
then
    echo "inotifywait could not be found, please install inotify-tools."
    exit 1
fi

# 检查是否提供了目录参数
if [ -z "$1" ]
then
    echo "Usage: $0 <directory>"
    exit 1
fi

DIRECTORY=$1

# 监控目录中的文件变化
inotifywait -m -r -e modify,create,delete --format '%w%f' "$DIRECTORY" | while read FILE
do
    if [ -f "$FILE" ]; then
        echo -e "File path: $FILE\n"
        echo -e "File content:\n$(cat "$FILE")\n"
    else
        echo -e "File path: $FILE\n"
        echo -e "File content: [File does not exist or is a directory]\n"
    fi
done

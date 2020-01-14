#!/usr/bin/env bash
${mysql[@]} < /docker-entrypoint-initdb.d/prot/prot.sql
find /docker-entrypoint-initdb.d -mindepth 2 -type f | while read -e f; do
    echo $f
    if [[ $f == *"prot/prot.sql"* ]]; then
        continue
    fi
    case "$f" in
        *.sh)
        if [ -x "$f" ]; then
            echo "$0: running $f"
            "$f"
        else
            echo "$0: sourcing $f"
            . "$f"
        fi
        ;;
        *.sql)    echo "$0: running $f"; "${mysql[@]}" < "$f"; echo ;;
        *.sql.gz) echo "$0: running $f"; gunzip -c "$f" | "${mysql[@]}"; echo ;;
        *)        echo "$0: ignoring $f" ;;
    esac
    echo
done
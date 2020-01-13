#!/usr/bin/env bash
${mysql[@]} < /docker-entrypoint-initdb.d/prot/prot.sql
${mysql[@]} < /docker-entrypoint-initdb.d/services_prot/bprot.sql
# find /docker-entrypoint-initdb.d -mindepth 1 -type f -print0 | while read -d $'\0' f; do
#     echo $f
#   case "$f" in
#     *.sh)
#       if [ -x "$f" ]; then
#         echo "$0: running $f"
#         "$f"
#       else
#         echo "$0: sourcing $f"
#         . "$f"
#       fi
#       ;;
#     *.sql)    echo "$0: running $f"; "${mysql[@]}" < "$f"; echo ;;
#     *.sql.gz) echo "$0: running $f"; gunzip -c "$f" | "${mysql[@]}"; echo ;;
#     *)        echo "$0: ignoring $f" ;;
#   esac
#   echo
# done
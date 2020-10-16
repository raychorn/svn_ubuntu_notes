ps -u root -o rss,command | grep -v peruser | awk '{sum+=$1} END {print sum/1024}'

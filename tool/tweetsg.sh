#!/bin/bash

rss=$1
url="http://api.twitter.com/1/statuses/user_timeline/metagoto.rss?count=5"
curlc="curl -o $rss $url"
tpl=$2
parser="$3 $rss $tpl"
oldsum=""
notify="curl -s $4"
sump=$5

echo "start $rss" >> /Users/runpac/cron/log.txt

if [ -f $rss ] 
then
  echo $($sump $rss) >> /Users/runpac/cron/log.txt
  oldsum=$($sump $rss)
fi

if ! $curlc
then
  exit 1
elif [ ! -f $rss ]
then
  exit 1
fi

sum=$($sump $rss)

echo "2nd sum $sum" >> /Users/runpac/cron/log.txt

if [ "$sum" != "$oldsum" ]
then
  if ! $parser
  then
    echo "!= " >> /Users/runpac/cron/log.txt
    exit 1
  else
	echo "notify " >> /Users/runpac/cron/log.txt
	$notify
  fi
else 
  echo "== " >> /Users/runpac/cron/log.txt
fi

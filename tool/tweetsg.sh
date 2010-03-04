#!/bin/bash

rss=$1
url="http://api.twitter.com/1/statuses/user_timeline/metagoto.rss?count=5"
curlc="curl -o $rss $url"
tpl=$2
parser="$3 $rss $tpl"
oldsum=""
notify="curl -s $4"
sump=$5

if [ -f $rss ] 
then
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

if [ "$sum" != "$oldsum" ]
then
  if ! $parser
  then
    exit 1
  else
    $notify
  fi
fi

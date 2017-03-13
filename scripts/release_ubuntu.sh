#!/bin/sh

set -eu

ROOT="$( cd "$( dirname "${BASH_SOURCE[0]}" )" && pwd )"/..

VERSION=`head -1 $ROOT/debian/changelog | awk '{print $2}'`
VERSION=${VERSION:1:-1}

cd $ROOT/..

for NAME in yakkety xenial trusty
do
  sed -i "1s/.*/rclone-browser (${VERSION}~${NAME}) ${NAME}; urgency=medium/" $ROOT/debian/changelog
  mv rclone-browser-${VERSION} rclone-browser-${VERSION}~${NAME}
  tar cJf rclone-browser_${VERSION}~${NAME}.orig.tar.xz rclone-browser-${VERSION}~${NAME}
  docker run -ti --rm -v `pwd`:/mnt -v ~/.gnupg:/home/mmozeiko/.gnupg u \
    /bin/bash -c "cd /mnt/rclone-browser-${VERSION}~${NAME} && debuild -S -k3A8ECC35 && dput ppa:mmozeiko/rclone-browser ../rclone-browser_${VERSION}~${NAME}_source.changes"
  mv rclone-browser-${VERSION}~${NAME} rclone-browser-${VERSION}
done

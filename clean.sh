#!/bin/sh
rm -rf build autom4te.cache config.h config.h.in config.guess config.sub config.log config.status configure install-sh ltmain.sh missing Makefile.in aclocal.m4 model.so libtool stamp-h1
for dir in .libs .deps; do
    find . -iname "$dir" -a -type d | xargs rm -rf
done
for file in "*.lo" "*.loT" "Makefile.in" "Makefile" "*.pyc" "*~" "*.tmp"; do
    find . -iname "$file" | xargs rm -f
done

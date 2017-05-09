#!/bin/sh

new_version_number=$1

# Inline substitution - works on both macOS and GNU
sed -i.bak "
s/Version [0-9][0-9]*\.[0-9][0-9]*\.[0-9][0-9]*/Version $1/g
" ./README.md

rm ./README.md.bak

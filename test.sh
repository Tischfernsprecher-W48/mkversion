#!/bin/bash

# Release festlegen
# 0 für Benutzer fragen
release_num=0

RELEASE=( "" "major release" "minor release" "patch level" "build" )
typeset -i i=1 max=${#RELEASE[*]}
if [ "${release_num}" -lt 1 ]
then
[ -f .last_version.number ] && echo "Alte Version: $(cat .last_version.number)" || mkversion >.last_version.number
echo ""
while (( i < max ))
do
   echo "${i}: New - ${RELEASE[${i}]}"
   i=i+1
done
echo ""
    read -n 1 -p "Bitte wählen sie eine Release Version aus: " release_num >/dev/null && echo ""
fi
[[ "${release_num}" =~ ^-?[1-$((max-1))]+$ ]] || { echo "ERROR: Aborting."; exit 1; }

# Version updaten
CUSTOM_VERSION=`mkversion "$(cat .last_version.number)" $release_num >.last_version.number && cat .last_version.number`
echo "Neue Version: ${CUSTOM_VERSION}"
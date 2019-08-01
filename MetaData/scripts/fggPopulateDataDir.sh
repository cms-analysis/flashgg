#!/bin/bash

TEMP=`getopt -o d:h --long directory:,help -n 'fggPopulateDataDir.sh' -- "$@"`

eval set -- "$TEMP"

directory=""

while true; do
    case "$1" in
        -d | --directory) directory="$2"; shift 2;;
        -h | --help ) help=1; shift;;
        -- ) shift; break ;;
        * ) break ;;
    esac
done

if [[ $help == 1 ]] || [[ $directory == "" ]]; then
    echo "Please specify the data directory (using the -d option) you want to populate (MicroAOD/data, Taggers/data or Systematics/data)"
fi

directory=${directory%/}
if [[ -L "$directory" && -d "$directory" ]]; then
    echo "Populating ${directory} ..."
    unlink $directory
    xrdcp -r root://eoscms.cern.ch//eos/cms/store/group/phys_higgs/cmshgg/flashgg-data/$directory ${directory%data}
fi

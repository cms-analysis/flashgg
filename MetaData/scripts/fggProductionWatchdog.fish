#!/usr/bin/env fish

argparse --name=crab-monitor 'l/loop' 's/sleep=' 'd/dir=' 'c=' -- $argv

#---Set defaults
#-working directory (current dir)
if not test $_flag_d
    set _flag_d './'
end
#-sleep time (1 minute)
if not test $_flag_s
    set _flag_s 60
end

#---Setup GRID proxy.
#-check if proxy validity is > 24h, if not get new proxy.
voms-proxy-info -e --valid 24:00
if test $status -eq 1
    voms-proxy-init -voms cms --valid 168:00
end
#-copy proxy to home directory and set variable accordingly.
set proxy (voms-proxy-info -p)
cp $proxy ~/
set -x X509_USER_PROXY ~/x509up_u68758 

bass source /cvmfs/cms.cern.ch/crab3/crab.sh
bass eval `scramv1 runtime -sh`

#---Moving into specified directory to prevent multiple instances of this script
#   to write into the same .fgg/, which is used by fggManageSamples.py as temporary directory.
cd $_flag_d

#---Infine loop
while true
    #---Tasks loop.
    for task in (ls -d crab_*/)
        echo $task
    
        #-get output dataset name and jobs state from crab status.
        set dataset ""    
        set crab_status (crab status $task)
        for line in $crab_status
            string match -e -r "^Output dataset:" $line
            if test $status -eq 0
                set dataset (string split ':' $line)[-1]
                set dataset (string trim $dataset)
                echo $dataset
            end
            string match -e -r "failed.*\%" $line
            if test $status -eq 0
                echo "Resubmitting ..."
                crab resubmit --maxjobruntime=1315 $task
            end
        end
        #-Import dataset into campaign catalogue and check files.
        if [ $dataset != "" ]
            string match -e -r $_flag_c $dataset
            if test $status -eq 0
                fggManageSamples.py -C $_flag_c import -S $dataset
                fggManageSamples.py -C $_flag_c check -q espresso --no-copy-proxy -S $dataset
            else
                echo "Dataset name does not match specified campaign, please check the -c option"
            end
        end
    end 

    #-exit if loop option is not set
    if not test $_flag_l
        exit 0
    else
        sleep $_flag_s
    end
end

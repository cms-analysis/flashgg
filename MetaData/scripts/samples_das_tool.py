#!/usr/bin/env python
import subprocess

def das_query(pattern,options):
    command_datasets = "dasgoclient --query 'dataset dataset={patt}'".format(patt=pattern)
    datasets = subprocess.check_output(command_datasets, shell=True)
    print "Found datasets:\n",datasets
    print "Now querying the number of events..."
    for ds in datasets.splitlines():
        das_query = "dasgoclient --query 'dataset dataset={ds} | sum(dataset.nevents)'".format(ds=ds)
        evts = subprocess.check_output(das_query, shell=True)
        print "{ds}: {evts} events".format(ds=ds,evts=evts.rsplit()[-1])

if __name__ == '__main__':

    import sys
    from optparse import OptionParser

    parser = OptionParser()
    parser.usage = """
    %prog <dataset_pattern>
    Given a dataset pattern, find datasets and number of events.
    For example:
    /VBFHiggs*ToGG_M125_TuneCP5_13TeV-JHUGenV7011-pythia8*/RunIISummer19UL17RECO-*/AODSIM
    """
    (options,args) = parser.parse_args()
    
    if len(args)!=1:
        print 'provide exactly one dataset pattern in argument.'
        sys.exit(1)

    pattern = args[0]

    das_query(pattern,options)
    ## N.B. useful example queries:
    # dasgoclient --examples


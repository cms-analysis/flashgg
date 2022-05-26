import os
import glob
import re

def run(cmd):
  print "%s\n\n"%cmd
  os.system(cmd)

def writePreamble(srcfile,treedir):
    srcfile.write("#!/bin/bash\n")
    srcfile.write("ulimit -s unlimited\n")
    srcfile.write("set -e\n")
    srcfile.write("cd %s/src\n"%os.environ['CMSSW_BASE'])
    srcfile.write("export SCRAM_ARCH=%s\n"%os.environ['SCRAM_ARCH'])
    srcfile.write("source /cvmfs/cms.cern.ch/cmsset_default.sh\n")
    srcfile.write("eval `scramv1 runtime -sh`\n")
    srcfile.write("cd %s\n"%treedir)

def writeCondorSub(srcfile,_exec,_queue,_nJobs):
    srcfile.write("executable = %s.sh\n"%_exec)
    srcfile.write("arguments  = $(ProcId)\n")
    srcfile.write("output     = %s.$(ClusterId).$(ProcId).out\n"%_exec)
    srcfile.write("error      = %s.$(ClusterId).$(ProcId).err\n\n"%_exec)
    srcfile.write("+JobFlavour = \"%s\"\n"%_queue)
    srcfile.write("queue %g\n"%_nJobs)

# ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
def writeSubFiles(treedir,outdir,options):
    # Make directory to store sub files
    if not os.path.isdir(outdir): os.system("mkdir %s"%(outdir))
    if not os.path.isdir("%s/jobs"%(outdir)): os.system("mkdir %s/jobs"%(outdir))
                       
    _jobdir = "%s/jobs"%outdir
    # Remove current job files
    if len(glob.glob("%s/*"%_jobdir)): os.system("rm %s/*"%_jobdir)
  
    _executable = "condor"
    _f = open("%s/%s.sh"%(_jobdir,_executable),"w") # single .sh script split into separate jobs
    writePreamble(_f,treedir)

    # Write details depending on mode
    subdirs = glob.glob("%s/*"%treedir)
    for idx,idir in enumerate(subdirs):
        _f.write("if [ $1 -eq %g ]; then\n"%idx)
        _f.write("cd %s \n"%idir)
        _f.write("hadd_all.py \n")
        _f.write("fi\n")

    # Close .sh file
    _f.close()
    os.system("chmod 775 %s/%s.sh"%(_jobdir,_executable))

    # Condor submission file
    _fsub = open("%s/%s.sub"%(_jobdir,_executable),"w")
    writeCondorSub(_fsub,_executable,options.queue,len(subdirs))
    _fsub.close()
    
# ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
# Function for submitting files to batch system
def submitFiles(outdir):
    _jobdir = "%s/jobs"%outdir
    _executable = "condor"
    cmdLine = "cd %s; condor_submit %s.sub; cd %s"%(_jobdir,_executable,os.getcwd())
    run(cmdLine)
    print "  --> Finished submitting files"

if __name__ == "__main__":
    from optparse import OptionParser
    parser = OptionParser(usage="%prog [options] treedir jobdir")
    parser.add_option("-d", "--dryRun",  dest="dryRun",  action="store_true", default=False, help="do not submit jobs, just write the config files");
    parser.add_option("-q", "--queue",   dest="queue",  type="string", default="espresso", help="condor queue to be used");
    (options, args) = parser.parse_args()
    
    if len(args)<2:
        raise RuntimeError, "Need to specify at least treedir jobdir"

    jobdir = args[1]
    writeSubFiles(args[0],jobdir,options)
    if options.dryRun:
        print "Wrote condor files in ",jobdir," but not submitting it. If everything ok, run w/o the --dryRun option. "
    else:
        print "Submitting... "
        submitFiles(jobdir)

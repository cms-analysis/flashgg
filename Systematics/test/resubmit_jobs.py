import os
import subprocess
from optparse import OptionParser
import fileinput
import ROOT
from ROOT import TFile, TTree

def files_to_remove(files,dir):
  filelist_to_remove = []
  for file in files:
    tfile = TFile.Open(dir+'/'+file);
    if (tfile.IsZombie()):
      filelist_to_remove.append(file)
  return filelist_to_remove

def list_files(file_name):
  with open(file_name) as f:
    datafile = f.readlines()
    file_list = []
    for line in datafile:
      if "USER_" in line:
        start_pt = line.find("output")
        end_pt = line.find(".root", start_pt + 1)
        res = line[start_pt:end_pt+5]
        if "/" in res : 
            res = res[res.find("/")+1:]
        file_list.append(res)
  return file_list


def find_runJobs(missing,dir):
  runJobs_dict = {}
  for item in missing:
    jobId = item[item.find("USER_")+5:item.find(".root")]
    file = item[:item.find("USER_")+4]
    bashCommand = "grep -r -H %s %s/runJobs*.sh "%(file,dir)
    out = subprocess.check_output(bashCommand,shell=True)
    start = out.find("runJobs")
    end = out.find(".sh")
    which_runJobs = ('runJobs')+out[start+7:end]
    if which_runJobs in runJobs_dict.keys() :
       runJobs_dict[which_runJobs].append(jobId)
    else  : runJobs_dict[which_runJobs] = [jobId]
  return runJobs_dict    


def submit_missing(runJobs_dict,dir,resubmit=True):
  for cluster in runJobs_dict.keys():
    bashCommand = "condor_submit %s/%s_mis.sub"%(dir,cluster)
    if resubmit : 
       print 'Resubmitting now!'
       os.system(bashCommand)
    else : 
      print 'Ready to resubmit, please set resubmit to True if you are ready : '
      print bashCommand


def prepare_runJobs_missing(runJobs_dict,dir):
  for cluster in runJobs_dict.keys():
    bashCommand = "cp %s/%s.sub %s/%s_mis.sub"%(dir,cluster,dir,cluster)
    os.system(bashCommand)
    bashCommand = "cp %s/%s.sh %s/original_%s.sh"%(dir,cluster,dir,cluster)
    os.system(bashCommand)
    jobs_to_run=''
    for jobId in runJobs_dict[cluster]:
       jobs_to_run+='%s '%jobId
    jobs_to_run = jobs_to_run[:-1] #remove the comma or space from the last
    for line in fileinput.input("%s/%s_mis.sub"%(dir,cluster), inplace=True):
      if  "queue" in line : print ("queue %d "%(len(runJobs_dict[cluster]))),
      else : print line,
    for line in fileinput.input("%s/%s.sh"%(dir,cluster), inplace=True):
      if  "declare -a jobIdsMap" in line : print ("declare -a jobIdsMap=(%s)\n"%(jobs_to_run)),
      else : print line,





def main():

  parser = OptionParser()
  parser.add_option("-d", "--dir", dest="dir",default="task_config.json",
                  help="directory")
  parser.add_option("-i", "--input", dest="input",default="all_root.txt",
                  help="input file with all root files present")
  parser.add_option("-r", "--resubmit", action="store_false",  dest="resubmit",default=True,
                  help="resubmit")

  (options, args) = parser.parse_args()

  bashCommand = "ls %s/*.root  > %s/all_root.txt"%(options.dir,options.dir)
  os.system(bashCommand)

  full_output =  list_files(options.dir+"/task_config.json")
  present_output =  list_files(options.dir+"/"+options.input)
  corrupted_files = files_to_remove(present_output,options.dir)
  not_finished = list(set(full_output) - set(present_output) - set(corrupted_files))
  print 'Number of missing files : ',len(not_finished)
  #print 'Missing the following files : ' not_finished
  runJobs_dict =   find_runJobs(not_finished,options.dir)
  print 'runJobs to be resubmitted : ',runJobs_dict
  prepare_runJobs_missing(runJobs_dict,options.dir)
  print 'Submitting missing jobs : '
  submit_missing(runJobs_dict,options.dir,options.resubmit)



if __name__ == "__main__":
    main()


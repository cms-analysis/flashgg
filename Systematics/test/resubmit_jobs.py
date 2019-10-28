import os
import subprocess
from optparse import OptionParser
import fileinput


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
    bashCommand = "grep -r %s %s/runJobs*.sh "%(file,dir)
    out = subprocess.check_output(bashCommand,shell=True)
    start = out.find("runJobs")
    end = out.find(".sh")
    which_runJobs = ('runJobs')+out[start+7:end]
    if which_runJobs in runJobs_dict.keys() :
       runJobs_dict[which_runJobs].append(jobId)
    else  : runJobs_dict[which_runJobs] = [jobId]
  return runJobs_dict    


def submit_missing(runJobs_dict,dir):
  for cluster in runJobs_dict.keys():
    bashCommand = "condor_submit %s/%s_mis.sub"%(dir,cluster,dir,cluster)
    os.system(bashCommand)


def prepare_runJobs_missing(runJobs_dict,dir):
  for cluster in runJobs_dict.keys():
    bashCommand = "cp %s/%s.sub %s/%s_mis.sub"%(dir,cluster,dir,cluster)
    os.system(bashCommand)
    jobs_to_run=''
    for jobId in runJobs_dict[cluster]:
       jobs_to_run+='%s,'%jobId
    jobs_to_run = jobs_to_run[:-1]
    for line in fileinput.input("%s/%s_mis.sub"%(dir,cluster), inplace=True):
      if "= $(ProcId)" in line : print line.replace("= $(ProcId)", "= $(JobId)"),
      elif  "queue" in line : print (line.strip() + " JobId in %s"%jobs_to_run),
      else : print line,




def main():

  parser = OptionParser()
  parser.add_option("-d", "--dir", dest="dir",default="task_config.json",
                  help="directory")
  parser.add_option("-i", "--input", dest="input",default="all_root.txt",
                  help="input file with all root files present")

  (options, args) = parser.parse_args()

  bashCommand = "ls %s/*.root  > %s/all_root.txt"%(options.dir,options.dir)
  os.system(bashCommand)

  full_output =  list_files(options.dir+"/task_config.json")
  present_output =  list_files(options.dir+"/"+options.input)
  not_finished = list(set(full_output) - set(present_output))
  print "Missing files : ",len(not_finished)
#  print not_finished
  runJobs_dict =   find_runJobs(not_finished,options.dir)
  prepare_runJobs_missing(runJobs_dict,options.dir)
  print 'Submitting missing jobs : '
  submit_missing(runJobs_dict,options.dir)



if __name__ == "__main__":
    main()


# Example Usage:
#
# cd flashgg
# python Systematics/scripts/resubmit_jobs.py --dir HHWWgg_2018_Data_Workspaces -s /eos/user/a/atishelm/ntuples/HHWWgg/HHWWgg_2018_Data_Workspaces/

import os
import subprocess
from optparse import OptionParser
import fileinput
import ROOT
import json
from ROOT import TFile, TTree

def files_to_remove(files,dir):
  filelist_to_remove = []
  for file in files:
    try:
      tfile = TFile.Open(dir+'/'+file);
    except:
      pass
    if tfile:
      if (tfile.IsZombie()):
        filelist_to_remove.append(file)
    else:
      print('File could not be opened, adding it to missing files')
      filelist_to_remove.append(file)
      
  print(filelist_to_remove)
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

def get_files_from_json(path_json):
  f = open(path_json)
  jf = json.load(f)
  flist = []
  for i in range(len(jf['jobs'])):
    flist.extend(jf['jobs'][i][2])
    
  for j in range(len(flist)):
    flist[j] = flist[j].split('/')[-1]
  
  return flist

def list_root(directory):
  flist = os.listdir(directory)
  return [fname for fname in flist if '.root' in fname]

def find_runJobs(missing,dir):
  runJobs_dict = {}
  for item in missing:
    # jobId = item[item.find("USER_")+5:item.find(".root")]
    # file = item[:item.find("USER_")+4]
    jobId = item.split('_')[-1].split('.')[0]
    file = item[:item.find('_{}.root'.format(jobId))] + '.root'
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


def prepare_runJobs_missing(runJobs_dict,dir, parentDataset=False, negR9Filter=False):
  for cluster in runJobs_dict.keys():
    bashCommand = "cp %s/%s.sub %s/%s_mis.sub"%(dir,cluster,dir,cluster)
    os.system(bashCommand)
    bashCommand = "cp %s/%s.sh %s/original_%s.sh"%(dir,cluster,dir,cluster)
    os.system(bashCommand)
    jobs_to_run=''
    for jobId in runJobs_dict[cluster]:
       jobs_to_run+='%s '%jobId
    jobs_to_run = jobs_to_run[:-1] #remove the comma or space from the last
    reqCpus = any(["RequestCpus" in line for line in fileinput.input("%s/%s_mis.sub"%(dir,cluster))])
    for line in fileinput.input("%s/%s_mis.sub"%(dir,cluster), inplace=True):
      if  "queue" in line and reqCpus: print ("queue %d "%(len(runJobs_dict[cluster]))),
      elif "queue" in line and not reqCpus: print ("RequestCpus = 2\nqueue %d "%(len(runJobs_dict[cluster]))),
      elif "max_retries" in line: print("max_retries = 2\n"),
      elif "RequestCpus" in line:
        print("RequestCpus = 2\n")
      else : print line,
    for line in fileinput.input("%s/%s.sh"%(dir,cluster), inplace=True):
      if  "declare -a jobIdsMap" in line : print ("declare -a jobIdsMap=(%s)\n"%(jobs_to_run)),
      elif "cmsRun" in line and parentDataset and "recalculatePDFWeights=True" not in line:
        ind = line.index("copyInputMicroAOD")
        line0 = line[:ind]
        line1 = line[ind:]
        line_ins = "recalculatePDFWeights=True "
        print (line0 + line_ins + line1),
      elif "cmsRun" in line and negR9Filter and "filterNegR9" not in line:
        ind = line.index("doBJetsAndMET")
        line0 = line[:ind]
        line1 = line[ind:]
        line_ins = "filterNegR9=True "
        print (line0 + line_ins + line1),
      else : print line,



def main():

  parser = OptionParser()
  parser.add_option("-d", "--dir", dest="dir",default="task_config.json",
                  help="directory")
  parser.add_option("-s", "--stage-dest", dest="stage_dest",
                  help="directory output files were staged to")
  # parser.add_option("-i", "--input", dest="input",default="all_root.txt",
  #                 help="input file with all root files present")
  parser.add_option("-r", "--resubmit", action="store_false",  dest="resubmit",default=True,
                  help="resubmit")
  parser.add_option("-p", "--parentDataset", action="store_true",  dest="parentDataset",default=False,
                  help="use parent dataset")
  parser.add_option("-f", "--filterNegR9", action="store_true",  dest="filterNegR9",default=False, help="use parent dataset")

  (options, args) = parser.parse_args()
  dir = os.path.dirname(os.path.abspath('%s/task_config.json'%options.dir))

  if options.stage_dest is not None:
    stageDir = os.path.abspath(options.stage_dest)
  else:
    stageDir = dir

  # bashCommand = "ls %s/*.root  > %s/all_root.txt"%(stageDir,dir)
  # os.system(bashCommand)

  full_output =  get_files_from_json(dir+"/task_config.json")
  present_output =  list_root(stageDir)
  corrupted_files = files_to_remove(present_output,stageDir)
  not_finished = list(set(full_output) - set(present_output))
  not_finished += corrupted_files
  print(not_finished)
  print 'Number of missing files : ',len(not_finished)
  #print 'Missing the following files : ' not_finished
  runJobs_dict = find_runJobs(not_finished,dir)
  print 'runJobs to be resubmitted : ',runJobs_dict
  prepare_runJobs_missing(runJobs_dict,dir,options.parentDataset, options.filterNegR9)
  print 'Submitting missing jobs : '
  submit_missing(runJobs_dict,dir,options.resubmit)



if __name__ == "__main__":
    main()


import fnmatch
import os

if not os.environ.has_key("CMSSW_BASE"):
  raise Exception,"flashgg not set up correctly? no CMSSW_BASE"  

flashggdir = os.environ["CMSSW_BASE"] + "/src/flashgg"

if not os.access(flashggdir,os.F_OK):
  raise Exception,"flashgg not set up correctly? no directory %s" % flashggdir  

with open("%s/Validation/scripts/endconfig.txt" % flashggdir,'r') as f:
  endtext = f.read()

exts = ["cc","h"]

matches = []
for root, dirnames, filenames in os.walk(flashggdir):
  for ext in exts:
    for filename in fnmatch.filter(filenames, '*.%s' % ext):
      matches.append(os.path.join(root, filename))

for match in matches:
  with open(match,'r') as f:
    text = f.read()
    if not text.count(endtext):
      answer = ""
      while answer != "y" and answer != "n":
        querytext = "File %s is missing the end formatting. Fix it? [y/n] " % match
        answer = raw_input(querytext).lower()[0]
        if answer == "y":
          with open(match,'a') as f:
            f.write("\n"+endtext)


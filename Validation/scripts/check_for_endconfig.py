import fnmatch
import os

if not os.environ.has_key("CMSSW_BASE"):
  raise Exception,"flashgg not set up correctly? no CMSSW_BASE"  

flashggdir = os.environ["CMSSW_BASE"] + "/src/flashgg"

if not os.access(flashggdir,os.F_OK):
  raise Exception,"flashgg not set up correctly? no directory %s" % flashggdir  

endtext = open("%s/Validation/scripts/endconfig.txt" % flashggdir,'r').read()

exts = ["cc","h"]

matches = []
for root, dirnames, filenames in os.walk(flashggdir):
  for ext in exts:
    for filename in fnmatch.filter(filenames, '*.%s' % ext):
      matches.append(os.path.join(root, filename))

for match in matches:
  text = open(match,'r').read()
  if not text.count(endtext):
    print match


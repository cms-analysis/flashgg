from PFCHS_0thVertex import *

stringsToFind = ["PFPileUp"]

result = {}

for item in dir(process):
  exec "result['%s'] = repr(process.%s)" % (item,item)

for item in result.keys():
	for astr in stringsToFind:
		if result[item].count(astr):
			print result[item]

#!/usr/bin/env python

import os
import fnmatch
import sys
import time
import numpy
from optparse import OptionParser

parser = OptionParser()
parser.add_option("-t","--sortByTime",action="store_true",dest="sortbytime",default=False,help="Sort plots by date modified")
parser.add_option("-k","--sortByType",action="store_true",dest="sortbytype",default=False,help="Sort plots by file type")
(options,args)=parser.parse_args()

for m in numpy.arange(110,151,1):
  directory = sys.argv[1]
  htmlFile = open(directory+'/'+str(m)+'.html','w')
  types=['png','gif','txt']

  sortedFiles=[]
  for root,dirs,files in os.walk(directory):
    for fType in types:
      for filename in fnmatch.filter(files,"*%d*.%s"%(m,fType)):
        if filename=='default.html': continue
        pathToFile = directory+'/'+filename
        modTime = "%s" % time.ctime(os.path.getmtime(pathToFile))
        sortedFiles.append([filename,modTime,fType])

  sortedFiles.sort(key=lambda x: x[0])
  if options.sortbytime:
    sortedFiles.sort(key=lambda x: x[1], reverse=True)
  if options.sortbytype:
    sortedFiles.sort(key=lambda x: x[2])

  for element in sortedFiles:
    filename = element[0]
    pathToFile = directory+'/'+filename
    htmlFile.write('<a href='+filename+'>'+filename+'</a> <br>\n')
    htmlFile.write('Last Modified (%s)<br>\n' % time.ctime(os.path.getmtime(pathToFile)))
    if '.txt' not in filename: htmlFile.write('<a href='+filename+'><img height=\"400\" src=\"'+filename+'\"></a><br>\n')

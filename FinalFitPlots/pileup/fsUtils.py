#!/bin/env python

import subprocess as sp
import re

def canonicalizeEOS(eospath):
    cmd = """cmsPfn %s | sed 's/\?.*$//'""" % eospath
    return sp.Popen(cmd, stdout=sp.PIPE, shell=True).communicate()[0].rstrip('\n')

#def canonicalizeCASTOR(castorpath):
#    return 
    ##CASTOR bash script
    #ESCAPED=`echo $DIR | sed -e 's/[\\/&]/\\\\&/g'`
    #FILES=`nsls ${DIR} | sed "s/^/rfio:${ESCAPED}\//" | paste -s`
    #hadd -T ${OUT} ${FILES}    

fsInfo = {
    re.compile('/store/') : {'function':canonicalizeEOS},
#    re.compile('/castor/') : {'function':canonicalizeCASTOR}
    }

def canonicalize(path):
    for pattern, props in fsInfo.iteritems():
        if pattern.search(path):
            return props['function'](path)
    else:
        return path






    
if __name__ == '__main__':
    testStrings = [
    "./foo.root",
    "/store/cms/foo.root",
    "/castor/cms/foo.root"
    ]

    def testIt (path):
        canonicalized = canonicalize(path)
        print "[%s]:[%s]" % (path, canonicalized)

    map( testIt, testStrings)
    pass

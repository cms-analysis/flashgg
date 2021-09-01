#!/usr/bin/env python
import os
import pickle
import shutil
import subprocess
from collections import defaultdict


def hadd(file, odir, idirs, appx=''):
    MAX_ARG_STRLEN = 131072
    if file.endswith('.pck'):
        try:
            haddPck( file, odir, idirs)
        except ImportError:
            pass
        return
    elif not file.endswith('.root'):
        return
    haddCmd = ['hadd']
    haddCmd.append(file.replace(idirs[0], odir).replace('.root', appx+'.root'))
    for dir in idirs:
        haddCmd.append( file.replace( idirs[0], dir ) )
    # import pdb; pdb.set_trace()
    cmd = ' '.join(haddCmd)
    print cmd
    if len(cmd) > MAX_ARG_STRLEN:
        print 'Command longer than maximum unix string length; dividing into 2'
        hadd(file, odir, idirs[:len(idirs)/2], '1')
        hadd(file.replace(idirs[0], idirs[len(idirs)/2]), odir, idirs[len(idirs)/2:], '2')
        haddCmd = ['hadd']
        haddCmd.append(file.replace(idirs[0], odir).replace('.root', appx+'.root'))
        haddCmd.append(file.replace(idirs[0], odir).replace('.root', '1.root'))
        haddCmd.append(file.replace(idirs[0], odir).replace('.root', '2.root'))
        cmd = ' '.join(haddCmd)
        print 'Running merge cmd:', cmd
        os.system(cmd)
    else:
        os.system(cmd)

def haddTrees(odir, idirs, firstTime=True):
    print 'adding', idirs
    print 'to', odir

    if os.path.exists(odir):
        raise RuntimeError("Error, %s exists already." % odir)
    elif os.path.dirname(odir):
        if not os.path.isdir(os.path.dirname(odir)): 
            os.makedirs(os.path.dirname(odir))

    if firstTime:
        files = []
        for chunk in idirs:
            if os.path.isdir(chunk):
                found = False
                for fname in os.listdir(chunk):
                    if fname.endswith(".root") and os.path.isfile(os.path.join(chunk, fname)):
                        files.append(os.path.join(chunk, fname))
                        found = True
                if not found: 
                    raise RuntimeError("Error, chunk %s doesn't contain any root file" % chunk)
            elif chunk.endswith(".root"):
                files.append(chunk)
    else:
        files = idirs[:]

    if len(files) == 0:
        raise RuntimeError("Error, no files for target %s" % odir)
    elif len(files) > 200:
        newlist = []; sublist = []
        for f in files:
            sublist.append(f)
            if len(sublist) == 200:
                haddTrees(odir+"_sub%d" % len(newlist), sublist, firstTime=False)
                newlist.append(odir+"_sub%d.root" % len(newlist))
                sublist = []
        if sublist:
            haddTrees(odir+"_sub%d" % len(newlist), sublist, firstTime=False)
            newlist.append(odir+"_sub%d.root" % len(newlist))
        haddTrees(odir, newlist, firstTime=False)
        return

    try:
        if len(files) == 1:
            shutil.move(files[0], odir+".root")
        else:
            subprocess.call(["hadd", odir+".root" ] + files)
    except OSError:
        print 
        print 'ERROR: directory in the way. Maybe you ran hadd already in this directory? Remove it and try again'
        print 
        raise

def haddChunks(idir, removeDestDir, cleanUp=False, maxSize=None):

    chunks = {}
    compsToSpare = set()

    for file in sorted(os.listdir(idir)):
        filepath = '/'.join( [idir, file] )
        #print filepath
        if filepath.endswith(".root"):
            compdir = file.rstrip(".root")
            try:
                prefix,num = compdir.rsplit('_',1)
                if not num.isdigit(): continue
            except ValueError:
                # ok, not a chunk
                continue
            #print prefix, num
            chunks.setdefault( prefix, list() ).append(filepath)
    if len(chunks)==0:
        print 'warning: no chunk found.'
        return
    if cleanUp:
        chunkDir = 'Chunks'
        if not os.path.isdir('Chunks'):
            os.mkdir(chunkDir)
    for comp, cchunks in chunks.iteritems():
        odir = '/'.join( [idir, comp] )
        tasks = [ (odir,cchunks) ]
        if maxSize:
            threshold = maxSize*(1024.**3)
            #print odir, cchunks
            running = [ dict(files=[], size=0.) ]
            for ch in cchunks:
                if os.path.isfile(ch+".root"):
                    size = os.path.getsize(ch+".root")
                if running[-1]['size'] + size > threshold:
                    running.append(dict(files=[], size=0.))
                running[-1]['files'].append(ch)
                running[-1]['size'] += size
            if len(running) > 1:
                tasks = []
                for i,task in enumerate(running):
                    tasks.append( ("%s_part%d" % (odir,i+1), task['files'][:]) )
                    print "Part %s: %d files, %.3f Gb" % (tasks[-1][0], len(task['files']), task['size']/(1024.**3))
            else:
                print "Entire chunk %.3f Gb, below threshold" % (running[-1]['size']/(1024.**3))
        for odir, cchunks in tasks:
            #print odir, cchunks
            if removeDestDir:
                if os.path.isfile(odir + ".root"):
                    os.unlink(odir + ".root")
            haddTrees(odir, cchunks)
            if cleanUp and (comp not in compsToSpare):
                for chunk in cchunks:
                    if not os.path.exists(chunk): continue
                    shutil.move(chunk, chunkDir)



if __name__ == '__main__':

    import sys
    from optparse import OptionParser

    parser = OptionParser()
    parser.usage = """
    %prog <dir>
    Find chunks in dir, and run recursive hadd to group all chunks.
    For example:
    DYJets_Chunk0/, DYJets_Chunk1/ ... -> hadd -> DYJets/
    WJets_Chunk0/, WJets_Chunk1/ ... -> hadd -> WJets/
    """
    parser.add_option("-r","--remove", dest="remove",
                      default=False,action="store_true",
                      help="remove existing destination directories.")
    parser.add_option("-c","--clean", dest="clean",
                      default=False,action="store_true",
                      help="move chunks to Chunks/ after processing.")
    parser.add_option("--max-size", dest="maxSize",
                      default=None,type="float",
                      help="max size of a chunk (in Gb)")

    (options,args) = parser.parse_args()

    if len(args)!=1:
        print 'provide exactly one directory in argument.'
        sys.exit(1)

    dir = args[0]

    haddChunks(dir, options.remove, options.clean, maxSize=options.maxSize)

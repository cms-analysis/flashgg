import FWCore.ParameterSet.Config as cms

# -----------------------------------------------------------------------
def addCategories(pset,cats,variables,histograms,mvas=None):
    
    for cat in cats:
        addCategory(pset,*cat,variables=variables,histograms=histograms,mvas=mvas)

# -----------------------------------------------------------------------
def addCategory(pset,label,cutbased=None,subcats=0,variables=[],histograms=[],mvas=None,classname=None,binnedOnly=None,
                dumpPdfWeights=None,nPdfWeights=None,nAlphaSWeights=None,nScaleWeights=None,splitPdfByStage0Bin=None,splitPdfByStage1Bin=None,dumpGenWeight=False, unbinnedSystematics=None):
    
    if subcats >= 0:
        catDef = cms.PSet(label=cms.string(label),
                          subcats=cms.int32(subcats),
                          variables=cms.VPSet(),
                          histograms=cms.VPSet(),
                          )
        if classname: catDef.className=cms.string(classname)
        if binnedOnly: catDef.binnedOnly=cms.bool(binnedOnly)
        if unbinnedSystematics: catDef.unbinnedSystematics=cms.bool(unbinnedSystematics)
        if dumpPdfWeights: catDef.dumpPdfWeights=cms.bool(dumpPdfWeights)
        if dumpGenWeight: catDef.dumpGenWeight=cms.bool(dumpGenWeight)
        if nPdfWeights: catDef.nPdfWeights=cms.int32(nPdfWeights)
        if nAlphaSWeights: catDef.nAlphaSWeights=cms.int32(nAlphaSWeights)
        if nScaleWeights: catDef.nScaleWeights=cms.int32(nScaleWeights)
        if splitPdfByStage0Bin is not None: catDef.splitPdfByStage0Bin=cms.bool(splitPdfByStage0Bin)
        if splitPdfByStage1Bin is not None: catDef.splitPdfByStage1Bin=cms.bool(splitPdfByStage1Bin)
        

        addVariables( catDef.variables, variables )
        addHistograms( catDef.histograms, histograms  )
        if mvas: 
            catDef.mvas = cms.VPSet()
            addMVAs( catDef.mvas, mvas  )
        
        pset.categories.append(catDef)

    if cutbased:
        # print'It is cut based'
        cb = cms.PSet( cut=cms.string(cutbased) )
        if( label != "" or classname):
            cb.name = cms.untracked.string(label)
        # print'cb = ',cb 
        pset.classifierCfg.categories.append(cb)


# -----------------------------------------------------------------------
def getNameExpr(expr,name=None):
    
    if ":=" in expr:
        toks=[ v.rstrip(" ").lstrip(" ") for v in expr.split(":=") ]
        ##print toks
        if len(toks)!=2:
            raise Exception("Could not parse variable definition %s" %expr)
        name,expr=toks
        
    if not name:
        name = expr.replace(".","_").replace("get","")
    
    return name,expr
   

#def parseVariable(expr, name):
#    name,expr = getNameExpr(expr,name)
#    if name.endswith("]"):
#        name,rng = name.replace("]","").split("[")
#        rng = rng.split(",")
#        nbins = int(rng[0])
#        vmin  = float(rng[1])
#        vmax  = float(rng[2])
#        return name, [expr], nbins, vmin, vmax
#        
#    if "map(" in  expr:
#        var, bins, vals = expr.lstrip("map(").rstrip(")").split("::")
#        bins = [ float(b) for b in bins.split(",") ]
#        vals = [ float(v) for v in vals.split(",") ]
#        return name, [var,bins, vals], nbins, vmin, vmax
#    else:
#        return name, [expr], None, None, None

def parseVariable(expr, name):
    name,expr = getNameExpr(expr,name)
    if name.endswith("]"):
        name,rng = name.replace("]","").split("[")
        rng = rng.split(",")
        nbins = int(rng[0])
        if rng[1].startswith("("):
            binning = [float(b) for b in rng[1].lstrip("(").rstrip(")").split(":") ]
#            print "binning in dumperconfigtools is "
#            print binning
            return name, [expr], nbins, None, None, binning
        else:
            vmin  = float(rng[1])
            vmax  = float(rng[2])
            return name, [expr], nbins, vmin, vmax, None
    
    if "map(" in  expr:
        var, bins, vals = expr.lstrip("map(").rstrip(")").split("::")
        bins = [ float(b) for b in bins.split(",") ]
        vals = [ float(v) for v in vals.split(",") ]
        return name, [var,bins, vals], None, None, None
    else:
        return name, [expr], None, None, None, None
    

# -----------------------------------------------------------------------
def addVariable(vpset,expr,name=None,nbins=None,vmin=None,vmax=None):



#    name, expr1, nbins, vmin, vmax = parseVariable(expr, name)
    name, expr1, nbins, vmin, vmax, binning = parseVariable(expr, name)
    
    # print'[dumperConfigTools] - expr1 = ',expr1
    # print'nbins = ',nbins 
    # print'[dumperConfigTools] - expr1[2] = ',expr1[2]

    if len(expr1)>1:
        pset = cms.PSet(
            expr  = cms.PSet(
                var = cms.string(expr1[0]), bins = cms.vdouble(expr1[1]), vals = cms.vdouble(expr1[2])
                ),
            name  = cms.untracked.string(name),
            )        
    else:
        pset = cms.PSet(
            expr  = cms.string(expr1[0]),
            name  = cms.untracked.string(name),
            )
    if nbins:
        pset.nbins = cms.untracked.int32(nbins)
        if vmin is not None and vmax is not None:
            pset.vmin = cms.untracked.double(vmin)
            pset.vmax = cms.untracked.double(vmax)
        elif binning:
            pset.binning = cms.untracked.vdouble(binning)
    vpset.append(pset)


#    name,expr = getNameExpr(expr,name)
#    
#    if name.endswith("]"):
#        name,rng = name.replace("]","").split("[")
#        rng = rng.split(",")
#        nbins = int(rng[0])
#        vmin  = float(rng[1])
#        vmax  = float(rng[2])
#    
#    if "map(" in  expr:
#        var, bins, vals = expr.lstrip("map(").rstrip(")").split("::")
#        bins = [ float(b) for b in bins.split(",") ]
#        vals = [ float(v) for v in vals.split(",") ]
#        pset = cms.PSet(
#            expr  = cms.PSet(
#                var = cms.string(var), bins = cms.vdouble(bins), vals = cms.vdouble(vals)
#                ),
#            name  = cms.untracked.string(name),
#            )        
#    else:
#        pset = cms.PSet(
#            expr  = cms.string(expr),
#            name  = cms.untracked.string(name),
#            )
#    if nbins:
#        pset.nbins = cms.untracked.int32(nbins)
#        pset.vmin = cms.untracked.double(vmin)
#        pset.vmax = cms.untracked.double(vmax)
#    vpset.append(pset)
    
    
# -----------------------------------------------------------------------
def addVariables(vpset,variables):
    if type(variables) == str:
        variables = mkVarList(variables)

    for var in variables:
        # print'var = ',var 
        if type(var) == str:
            # print'var is string'
            addVariable(vpset,var)
        else:
            # print'var is not string'
            addVariable(vpset,*var)

# -----------------------------------------------------------------------
def parseHistoDef(histo):
    
    hvars,hdef = histo.split(">>")
    if "[" in hdef:
        seps = "[]"
    else:
        seps = "()"
        
    name, hdef = hdef.split(seps[0])
    if name == "":
        raise Exception("Empty name in histogram declaration %s." % histo)
    
    if not hdef.endswith(seps[1]):
        raise Exception("Invalid histogram definition %s.\nBinning should be specified as either [xb1,xb2..xbN:yb1,yb2..ynB] or (nbx,x1,x2:nby,y1,y2)" % histo)
    hdef = hdef.rstrip(seps[1])
    
    nxbins,nybins = 0,0
    xvar,yvar = None,None
    xbins,ybins = [],[]
    
    if ":" in hvars:
        xvar, yvar = hvars.split(":")
        xbins, ybins = hdef.lstrip("[").rstrip("]").lstrip("(").rstrip(")").split(":")
    else:
        xvar, yvar = hvars,None
        xbins, ybins = hdef,None
        
    xbins = [ float(b) for b in xbins.lstrip(" ").rstrip(" ").split(",")  if b != ""]
    if ybins:
        ybins = [ float(b) for b in ybins.lstrip(" ").rstrip(" ").split(",")  if b != ""]
    if seps == "[]":
        nxbins  = 0
        nybins  = 0
    else:
        if len(xbins) != 3:
            raise Exception("Invalid histogram definition %s. Expecting only three items for x binning definition"% histo)
        if ybins and len(ybins) != 3:
            raise Exception("Invalid histogram definition %s. Expecting only three items for y binning definition"% histo)
        nxbins = int(xbins[0]) 
        xbins = xbins[1:3]
        if ybins:
            nybins = int(ybins[0]) 
            ybins = ybins[1:3]
            
    return name, xvar, nxbins, xbins, yvar, nybins, yvar

# -----------------------------------------------------------------------
def addHistogram(vpset,histo):
    
    hvars,hdef = histo.split(">>")
    if "[" in hdef:
        seps = "[]"
    else:
        seps = "()"
        
    name, hdef = hdef.split(seps[0])
    if name == "":
        raise Exception("Empty name in histogram declaration %s." % histo)
    
    if not hdef.endswith(seps[1]):
        raise Exception("Invalid histogram definition %s.\nBinning should be specified as either [xb1,xb2..xbN:yb1,yb2..ynB] or (nbx,x1,x2:nby,y1,y2)" % histo)
    hdef = hdef.rstrip(seps[1])
    
    if ":" in hvars:
        xvar, yvar = hvars.split(":")
        xbins, ybins = hdef.lstrip("[").rstrip("]").lstrip("(").rstrip(")").split(":")
    else:
        xvar, yvar = hvars,None
        xbins, ybins = hdef,None
        
    xbins = [ float(b) for b in xbins.lstrip(" ").rstrip(" ").split(",")  if b != ""]
    if ybins:
        ybins = [ float(b) for b in ybins.lstrip(" ").rstrip(" ").split(",")  if b != ""]
    if seps == "[]":
        nxbins  = 0
        nybins  = 0
    else:
        if len(xbins) != 3:
            raise Exception("Invalid histogram definition %s. Expecting only three items for x binning definition"% histo)
        if ybins and len(ybins) != 3:
            raise Exception("Invalid histogram definition %s. Expecting only three items for y binning definition"% histo)
        nxbins = int(xbins[0]) 
        xbins = xbins[1:3]
        if ybins:
            nybins = int(ybins[0]) 
            ybins = ybins[1:3]
    
    pset = cms.PSet( name=cms.untracked.string(name),
                     x=cms.untracked.string(xvar),
                     nxbins=cms.untracked.int32(nxbins),
                     )
    if nxbins == 0:
        pset.xbins=cms.untracked.vdouble(*xbins)
    else:
        pset.xmin=cms.untracked.double(xbins[0])
        pset.xmax=cms.untracked.double(xbins[1])

    if yvar:
        pset.y=cms.untracked.string(yvar)
        pset.nybins=cms.untracked.int32(nybins)
        pset.ybins=cms.untracked.vdouble(*ybins)
        if nybins == 0:
            pset.ybins=cms.untracked.vdouble(*ybins)
        else:
            pset.ymin=cms.untracked.double(ybins[0])
            pset.ymax=cms.untracked.double(ybins[1])

    vpset.append( pset )
               

# -----------------------------------------------------------------------
def addHistograms(vpset,histograms):
    
    for histo in histograms:
        addHistogram( vpset, histo )

# -----------------------------------------------------------------------
def addMVA(vpset,name,variables,classifier,weights):
    
    addVariable(vpset,name)
    pset=vpset[-1]
    pset.variables=cms.VPSet()
    pset.classifier=cms.string(classifier)
    pset.weights=cms.FileInPath(weights)
    addVariables(pset.variables,variables)
    
# -----------------------------------------------------------------------
def addMVAs(vpset,mvas):
    for mva in mvas:
        addMVA(vpset,*mva)

# -----------------------------------------------------------------------
def dumpOnly(pset,varList):
    for cat in pset.categories:
        cat.dumpOnly = cms.vstring(varList)
    

# -----------------------------------------------------------------------
def mkVarList(inp):
    
    ret = []
    for var in inp.replace(" ","").replace("\n","").replace("\t","").split(";"):
        print'var = ',var
        ret.append( var )
            
    return ret


# -----------------------------------------------------------------------
def addGlobalFloats(process,globalVariables,src,variables,tagSequence):
    
    ntproducer = "%sNtpProducer" % src
    if not hasattr(process,ntproducer):
        setattr(process,ntproducer,cms.EDProducer(
                "CandViewNtpProducer", 
                src = cms.InputTag(src), lazyParser = cms.untracked.bool(True),
                variables = cms.VPSet()
                )
            )
        if hasattr(process,src):
            tagSequence.insert(tagSequence.index(getattr(process,src))+1,getattr(process,ntproducer))
        else:
            dummySequence=cms.Sequence(getattr(process,ntproducer))
            tagSequence+=dummySequence
            
            
    varlist = {}
    for var in variables:
#        name,expr = getNameExpr(var)
        name0,expr0 = getNameExpr(var)
        name,expr,nbins,vmin,vmax,binning = parseVariable(expr0,name0)
#        print name,expr,nbins,vmin,vmax,binning
        varlist[name] = expr[0]
        setattr(globalVariables.extraFloats,name,cms.PSet(src=cms.InputTag(ntproducer,name)))
        if nbins:
            if vmin is not None and vmax is not None:
                setattr(globalVariables.extraFloats,name,cms.PSet(src=cms.InputTag(ntproducer,name),nbins=cms.int32(nbins),vmin=cms.double(vmin),vmax=cms.double(vmax)))
            elif binning:
                setattr(globalVariables.extraFloats,name,cms.PSet(src=cms.InputTag(ntproducer,name),nbins=cms.int32(nbins),binning=cms.vdouble(binning) ) )
            else:
                raise Exception("Global float variable %s format does not match name[nbins,vmin,vmax] nor name[-1,(boundary1: ... :boundaryN)] " %var)
            
#            setattr(globalVariables.extraFloats,"nbins",nbins)
#            setattr(globalVariables.extraFloats,"vmin",vmin)
#            setattr(globalVariables.extraFloats,"vmax",vmax)
    
    bookedVars = []
    for ivar in getattr(process,ntproducer).variables:
        bookedName = ivar.tag.value() 
        bookedExpr = ivar.quantity.value()
        if bookedName in varlist:
            assert( bookedExpr == varlist[bookedName] )
            bookedVars.append( bookedName )

    for name,expr in filter(lambda x: x[0] not in bookedVars, varlist.iteritems()):
        getattr(process,ntproducer).variables.append( cms.PSet(tag=cms.untracked.string(name),quantity=cms.untracked.string(expr)) )

import FWCore.ParameterSet.Config as cms

# -----------------------------------------------------------------------
def addCategories(pset,cats,variables,histograms,mvas=None):
    
    for cat in cats:
        addCategory(pset,*cat,variables=variables,histograms=histograms,mvas=mvas)

# -----------------------------------------------------------------------
def addCategory(pset,label,cutbased=None,subcats=0,variables=[],histograms=[],mvas=None):
    
    if subcats >= 0:
        catDef = cms.PSet(label=cms.string(label),
                          subcats=cms.int32(subcats),
                          variables=cms.VPSet(),
                          histograms=cms.VPSet()
                          )
        
        addVariables( catDef.variables, variables )
        addHistograms( catDef.histograms, histograms  )
        if mvas: 
            catDef.mvas = cms.VPSet()
            addMVAs( catDef.mvas, mvas  )
        
        pset.categories.append(catDef)

    if cutbased:
        cb = cms.PSet( cut=cms.string(cutbased) )
        if( label != "" ):
            cb.name = cms.untracked.string(label)
        pset.classifierCfg.categories.append(cb)

    
# -----------------------------------------------------------------------
def addVariable(vpset,expr,name=None,nbins=None,vmin=None,vmax=None):
    if ":=" in expr:
        toks=[ v.rstrip(" ").lstrip(" ") for v in expr.split(":=") ]
        ## print toks
        name,expr=toks
        
    if not name:
        name = expr.replace(".","_").replace("get","")
    if name.endswith("]"):
        name,rng = name.replace("]","").split("[")
        rng = rng.split(",")
        nbins = int(rng[0])
        vmin  = float(rng[1])
        vmax  = float(rng[2])

    pset = cms.PSet(
            expr  = cms.string(expr),
            name  = cms.untracked.string(name),
            )
    if nbins:
        pset.nbins = cms.untracked.int32(nbins)
        pset.vmin = cms.untracked.double(vmin)
        pset.vmax = cms.untracked.double(vmax)
    vpset.append(pset)
    
    
# -----------------------------------------------------------------------
def addVariables(vpset,variables):
    
    if type(variables) == str:
        variables = mkVarList(variables)

    for var in variables:
        if type(var) == str:
            addVariable(vpset,var)
        else:
            addVariable(vpset,*var)
        
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
            raise Exception("Invalid histogram definition %s. Expecting only three items for x binning definition")
        if ybins and len(ybins) != 3:
            raise Exception("Invalid histogram definition %s. Expecting only three items for y binning definition")
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
def mkVarList(inp):
    
    ret = []
    for var in inp.replace(" ","").replace("\n","").replace("\t","").split(";"):
        ret.append( var )
            
    return ret


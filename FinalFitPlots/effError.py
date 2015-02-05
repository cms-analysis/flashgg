import sys

errFile = open(sys.argv[1])
if len(sys.argv) == 3:
    nCats=int(sys.argv[2])
else:
    nCats=6

totEffs={}

allEff={}

types=["presel","id","elveto"]

for line in errFile.readlines():
  eff={}
  effErr={}
  if line.startswith('#'): continue
  if len(line.split())==0: continue
  cat = line.split()[0]
  for i,el in enumerate(line.split()):
    if i==0: continue
    
    if i<3:
      type=types[0]
    elif i<5:
      type=types[1]
    elif i<7:
      type=types[2]
    else:
      print "what is this?", cat,i,el

    if (i%2==0): effErr[type]=float(el)
    else: eff[type]=float(el)

  #print cat, 'eff:', eff
  #print cat, 'err:', effErr

  allEff[cat]=eff,effErr

  if len(eff)!=len(effErr): sys.exit('ERROR: arrays not equal length')
  
catnames=allEff.keys()

effstr=""
errstr=""

for newcat in range(0,nCats):
    toteff = 1.;
    toteffErrSq = 0.;
    for type in types:
        if nCats==6:  # 3 r9 (>0.94,0.9-0.94,<0.9) * 2 eta
            if type == "presel":
              if newcat%3!=2: # high and mid r9 both >0.9
                cat4ind=(newcat)/3*2
              else:
                cat4ind=((newcat)/3)*2+1
            else:
              if newcat%3==0: # only high r9 >0.94
                cat4ind=(newcat)/3*2
              else:
                cat4ind=((newcat)/3)*2+1
        elif nCats==4:  # 2 r9 (>0.94,<0.94) * 2 eta
            cat4ind=newcat
        else:
            print "nCats",nCats,"is undefined"
      
        thiscat=allEff[catnames[int(cat4ind)]]
        thiseff=thiscat[0][type]
        thiserr=thiscat[1][type]
     
        #print newcat,type,cat4ind,thiseff,
        toteff*=thiseff
        toteffErrSq+=((thiserr/thiseff)**2)
    
    toterr = toteff*(toteffErrSq**0.5)
    
    totEffs[newcat]=toteff,toterr
    effstr=effstr+"%1.3f"%totEffs[newcat][0]
    errstr=errstr+"%1.3f"%totEffs[newcat][1]
    if newcat is not (nCats-1):
        effstr=effstr+","
        errstr=errstr+","
    #print 'Total eff %s: %1.3f +/- %1.3f'%(cat,toteff,toterr)

#print totEffs
print '\nFor mergeGlobeSystematics.C :'
print 'Double_t ratioTP_[nphocats]            = {'+effstr+'};'
print 'Double_t ratioTP_low_err_[nphocats]    = {'+errstr+'};'
print 'Double_t ratioTP_high_err_[nphocats]   = {'+errstr+'};'

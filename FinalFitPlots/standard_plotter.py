import ROOT
from standard_plotter_cfg import *
ROOT.gROOT.SetStyle('Plain')
ROOT.gROOT.SetBatch(True)
ROOT.gStyle.SetOptStat(0)
#ROOT.gStyle.SetLineWidth(2)
ROOT.gStyle.SetPalette(1)
import sys,os,getopt,math

random		 = ROOT.TRandom3()
def printHelp():

  print """ Options for standard_plotter --
	    -signal	include signal in the plots, these are defined by indexes in standard_plotter_cfg
	    -norm	normalise histograms to Data.
	    -logy 	use a log scale for Y
	    -nostack    histograms will not be stacked, plotted as lines normed to 1. default is to stack
	    -data	include data in the plots
	    -gif	plot as gif rather than pdf
	"""
 
# ----------------------------------------------------------------
def expo_int(N,alph,a,b):
  return N*(1./alph)*(math.exp(b*alph) - math.exp(a*alph))
# ----------------------------------------------------------------
# -- User Imput Options -------------------------------------------
optlist  = ['-signal','-norm','-logy','-nostack','-data','-gif']
tmp,opts = getopt.getopt(sys.argv[0:],'', \
			 longopts=optlist)
args = sys.argv[1:]

include_signal 		= False
scale_montecarlo	= False
set_logy		= False
dont_stack		= False	
plot_data		= False
plot_as_gif		= False

for o in opts[1:]:
  if   o=='-sig':
    include_signal = True
    args.remove(o)
  elif o=='-norm':
    scale_montecarlo = True
    args.remove(o)
  elif  o=='-logy':
    set_logy = True
    args.remove(o)
  elif o=='-nostack':
    dont_stack = True
    args.remove(o)
  elif o=='-data':
    plot_data = True
    args.remove(o)
  elif o=='-gif':
    plot_as_gif = True
    args.remove(o)
  else :
	 printHelp()
	 sys.exit('No Option Available: %s'%o)
# ----------------------------------------------------------------
print "Higgs Analysis - Standard Plotter"
print "Plotting All histograms in", file 
# -- Check Sanity -------------------------------------------------
if not plot_data and scale_montecarlo:
  scale_montecarlo = False
if plot_data and not dont_stack:
  plot_ratio = True
else: plot_ratio = False
# ----------------------------------------------------------------

#remove signals from file list if no signal option
if not include_signal:
  remove_me=[]
  for si in signal_index: j = titles_colors.pop(str(si))
  for lnk in file_names:
    if  file_names[lnk][0] in signal_index: remove_me.append(lnk)
  for rm in remove_me:  file_names.pop(rm)
#print linked_files 
#if not include_signal: file_list = filter(lambda x: not x[1], file_list)
 
if os.path.isfile(file):  	
  data_file = ROOT.TFile(file)
else:
  sys.exit('No file found names %s'%data_name)

keys_list = data_file.GetListOfKeys()
hist_list = [k.ReadObj() for k in keys_list]
data_hist_list = []
for i in range(len(hist_list)):
  for ff in file_names:
    if ff == (hist_list[i].GetName().split("_"))[-1]:
	file_names[ff].append(hist_list[i].Clone())

for i in range(len(hist_list)):
  if data_name in hist_list[i].GetName():
	data_hist_list.append(hist_list[i].Clone())

rm_files = filter(lambda x: len(file_names[x])<=2,file_names)
for rm in rm_files: file_names.pop(rm)

f0 = file_names.keys()[0]


if len(file_names)!= 0: nhists = len(file_names[f0])-2
elif len(file_names)==0 and len(data_hist_list)!=0 : nhists = len(data_hist_list)
else: sys.exit("No Histograms anywhere to be plotted!")

types = [file_names[x][0] for x in file_names]
types= set(types)
linked_names = [filter(lambda x: file_names[x][0] ==i ,file_names) \
		for i in (types)]
linked_names = filter(lambda x: len(x)>0,linked_names)

for l in linked_names: print l

print nhists, "Histograms to be plotted in", plottingdir
tmp=[0 for kk in range(nhists)]

Nam = ''
for i in range(2,nhists+2):

        ind = i-2
	leg=ROOT.TLegend(0.55,0.6,0.85,0.85)
	leg.SetFillColor(0)
	leg.SetBorderSize(0)
	
	stack = ROOT.THStack()
        # ------------------------------------------------------
	c = ROOT.TCanvas()
	if plot_ratio:
          u_pad = ROOT.TPad(str(i)+'u',str(i)+'u'\
			 ,0.01,0.25,0.99,0.99)
          d_pad = ROOT.TPad(str(i)+'d',str(i)+'d'\
			 ,0.01,0.01,0.99,0.25)
	  u_pad.SetNumber(1)
	  d_pad.SetNumber(2)
          u_pad.Draw()
	  d_pad.Draw()

	  u_pad.SetGrid(True)	
	# ------------------------------------------------------

	to_be_stacked = []
	to_be_summed = []

	for l in range(len(linked_names)):
	  for j in range(len(linked_names[l])): 

		f = file_names[linked_names[l][j]]
		h = f[i].Clone()
		w = f[1]
		cc = str(f[0])

	  	Nam = f[i].GetName()
		h.Scale(w)
		if j == 0: 
			tmp[ind] = h.Clone()
		else:  tmp[ind].Add(h)

   
	  if dont_stack:	     
	      tmp[ind].SetLineColor(titles_colors[cc][1])
	      tmp[ind].SetLineWidth(2)	
	      if  tmp[ind].Integral() != 0: tmp[ind].Scale(1./tmp[ind].Integral())
	      leg.AddEntry(tmp[ind],titles_colors[cc][0],'L')
	  else:
	      tmp[ind].SetFillColor(titles_colors[cc][1])
	      leg.AddEntry(tmp[ind],titles_colors[cc][0],'F')

	  to_be_stacked.append(tmp[ind])
	  if not f[0] in signal_index: to_be_summed.append(tmp[ind])
	  #print "DEBUG -- Made Stacks"
	  # -----------------------------------------------------
	

	stacked_sum = 0
	
	for l,h in enumerate(to_be_summed):
	  stacked_sum+=h.Integral()

	if plot_data:
	   data_hist = data_hist_list[i-2]
           print data_hist.GetName()
 	   n_data	= data_hist.Integral()
	else: n_data = stacked_sum

	for h in to_be_stacked:
          if stacked_sum != 0 and scale_montecarlo:
	   if h in to_be_summed:
	    h.Scale(n_data/stacked_sum)
	  stack.Add(h) 
        

	if set_logy and plot_ratio: u_pad.SetLogy()
        elif set_logy: c.SetLogy()
	else: 
	 if plot_data: data_hist.SetMinimum(0)
        # -- Data Styles ------------------------
	if plot_data:
	  data_hist.SetMarkerStyle(21)
	  data_hist.SetMarkerSize(0.7)
	  data_hist.Sumw2()
	# ---------------------------------------	

        c.cd(1)
	if plot_data:
	  leg.AddEntry(data_hist,'Data L - '+str(current_lumi)+' pb^{-1}','PLE')
	  data_hist.Draw()

	
        if dont_stack:
	 to_be_stacked[0].SetMaximum(1.0)	 
	 to_be_stacked[0].Draw()
         for hist in to_be_stacked[1:]: hist.Draw("same")

	else: 
	  if plot_data: stack.Draw('same')
	  else:  stack.Draw()
	
	
	# Plot data again to ensure overlay
	if plot_data: data_hist.Draw('same')

	leg.Draw()

	if plot_ratio:
	  c.cd(2)
	  hist_rat = data_hist.Clone() 
	  hist_tot = data_hist.Clone() 
	  hist_1 = data_hist.Clone()

	  for b in range(hist_1.GetNbinsX()+1):
	    hist_1.SetBinContent(b,1) 
	    hist_tot.SetBinContent(b, \
		  sum([hh.GetBinContent(b)   \
		  for hh in to_be_summed]))

          hist_rat.Divide(hist_tot)

	  hist_1.SetLineColor(4)
	  hist_1.SetLineWidth(3)
	  hist_1.SetMaximum(2)
	  hist_1.SetMinimum(0)
	  hist_1.SetTitle('')
	  hist_1.GetYaxis().SetLabelSize(0.12)
	  hist_1.GetYaxis().SetNdivisions(3)
	  hist_1.GetXaxis().SetLabelSize(0.12)
	  hist_1.GetXaxis().SetTitle('')
	  hist_1.GetYaxis().SetTitle('')

	  hist_1.Draw("hist")
	  hist_rat.Draw('same')

	if plot_data: Nam = data_hist.GetName()
        # ----------------------------------------------------
	if plot_as_gif: c.SaveAs(plottingdir+'/'+Nam+'.gif')
	else: c.SaveAs(plottingdir+'/'+Nam+'.pdf')

#EOF



  


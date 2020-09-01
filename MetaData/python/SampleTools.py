#####################################################################################################
# Abe Tishelman-Charny
# 8 May 2020 
# The purpose of this module is to perform multiple tasks with samples to be processed by flashgg
#
# Example Usage:
#
# ##-- Create submit jobs json - only add sample if there is an entry in MetaData/data/cross_sections.json
# python SampleTools.py --samplesFile HHWWgg_bkgk.txt --createJobsJson --HHWWgg_bkg_campaigns --OnlyIfExists --inputJson MetaData/data/cross_sections.json
# python SampleTools.py --samplesFile HHWWgg_bkgk.txt --createJobsJson --HHWWgg_bkg_campaigns --OnlyIfExists --inputJson MetaData/data/cross_sections.json --short
#
# python SampleTools.py --samplesFile HHWWgg_Backgrounds.txt --createXSjson --XS_outName testXS.json --XSec_File mc_infos_DiJet.dat --HHWWgg_bkg_campaigns --inputJson updatedCrossSections_IthinkwithGenAnalyzeHighEvents.json 
#


# ##-- Merge jsons
# python SampleTools.py --mergeXSFiles --js1 MetaData/data/cross_sections.json --js2 updatedCrossSections_IthinkwithGenAnalyzeHighEvents.json --outJson test.json
#####################################################################################################

import os 
import glob 
import argparse 
import json 

parser = argparse.ArgumentParser()

##-- List of samples in txt file you would like to use SampleTools on 
parser.add_argument("--samplesFile", type=str, default="input.txt", help="Input file with sample names", required=False)
parser.add_argument("--HHWWgg_bkg_campaigns", action="store_true", default=False, help="Expected campaigns in samplesFile: HHWWgg_bkg_campaigns", required=False)
parser.add_argument("--fggCampaign", action="store_true", default=False, help="Expected campaigns in samplesFile: Era2017_RR-31Mar2018_v2", required=False)
parser.add_argument("--OnlyIfExists", action="store_true", default=False, help="Only add a sample to the output json (fggrunjobs input or XSEC json) if it exists in --inputJson", required=False)

##-- Create Cross Sections json file 
parser.add_argument("--createXSjson", action="store_true", default=False, help="Create XS json. Default used by flashgg is Metadata/data/cross_sections.json", required=False)
parser.add_argument("--XS_outName", type=str, default="updatedCrossSections.json", help="Name of output json for XS json", required=False)
parser.add_argument("--inputJson", type=str, default="MetaData/data/cross_sections.json", help="Name of input json to use as base", required=False)
parser.add_argument("--nXsecEvents", type=int, default=1000, help="Number of events to use to compute XSection of dataset", required=False)
parser.add_argument("--XSec_File", type=str, default="", help="Text file with cross section values to use instead of gen analyzer", required=False)

##-- Merge Cross Section jsons 
parser.add_argument("--mergeXSFiles", action="store_true", default=False, help="Merge cross section jsons", required=False)
parser.add_argument("--js1", type=str, default="", help="Json 1", required=False)
parser.add_argument("--js2", type=str, default="", help="Json 2", required=False)
parser.add_argument("--outJson", type=str, default="outJson.json", help="Output json of merged jsons", required=False)

##-- Create Json file for fggrunjobs 
parser.add_argument("--createJobsJson", action="store_true", default=False, help="Create input json for fggrunjobs", required=False)
parser.add_argument("--short", action="store_true", default=False, help="One sample per campaign", required=False)

##-- Misc 
parser.add_argument("--dryRun", action="store_true", default=False, help="Do not save any outputs", required=False)

args = parser.parse_args()

def XSecExists(sampleName_,json_):
	with open(json_, 'r+') as f:
		data = json.load(f)
		exists = sampleName_ in data 
	return exists 

def GetXSfromFile(XSec_File,samplename_):
	print"getting xs from file"
	XS = 0. 
	string_to_search = samplename_
	with open(XSec_File, 'r') as read_obj:
		# Read all lines in the file one by one		
		for line in read_obj:
			# For each line, check if line contains the string
			if string_to_search in line:
				# print"Line:",line 
				XS = float(line.split()[1])
				# for item in line.split(): # split by arbitrary amount of white space 
					# print'item:',item
				# exit(1)
				# print"Line containing XSec:"
				# print line 
				# XSecLine = line # form: "After filter: final cross section %s +- %s pb"	

	return XS 

# https://twiki.cern.ch/twiki/bin/viewauth/CMS/HowToGenXSecAnalyzer
def GetXSfromGenAnalyzer(parentsFileName_,nEvents_,dataTier_,campaign_):
	os.system("cp %s ../genproductions/test/calculateXSectionAndFilterEfficiency"%(parentsFileName_))
	os.chdir("../genproductions/test/calculateXSectionAndFilterEfficiency")
	os.system("./calculateXSectionAndFilterEfficiency.sh -f %s -d %s -n %s -c %s"%(parentsFileName_,dataTier_,nEvents_,campaign_))
	list_of_files = glob.glob('./*') # * means all if need specific format then *.csv
	latest_file = max(list_of_files, key=os.path.getctime)

	string_to_search = "After filter: final cross section"
	XSecLine = ""
	with open(latest_file, 'r') as read_obj:
		# Read all lines in the file one by one		
		for line in read_obj:
			# For each line, check if line contains the string
			if string_to_search in line:
				print"Line containing XSec:"
				print line 
				XSecLine = line # form: "After filter: final cross section %s +- %s pb"
	XSec = XSecLine.split('After filter: final cross section = ')[1]
	XSec = XSec.split('+-')[0]
	# example: XSec = "1.305e-01"
	XSecVal = XSec.split('e')[0]
	XSecFactorSign = XSec.split('e')[1][0]
	XSecFactor = XSec.split('e')[1][1:]
	# XSecFactorSign = XSecFactor[0]

	if(XSecFactorSign == "-"): XSecFactorSign = -1 
	if(XSecFactorSign == "+"): XSecFactorSign = 1
	XSecFactorNumber = float(10)**(float(XSecFactorSign)*float(XSecFactor))

	finalXSec = float(XSecVal)*float(XSecFactorNumber)

	os.chdir("flashgg")
	# os.chdir("../../../")
	return finalXSec 

def GetParentDataset(sample_):
	command = "dasgoclient --query='parent dataset=%s instance=prod/phys03'"%(sample_)
	parentDataset_ = os.popen(command).read()
	parentDataset_ = parentDataset_.replace('\n',"")
	parentDataset_ = parentDataset_.replace(' ',"")
	return parentDataset_

# Could add cool study of cross section and uncertainty vs. number of events used 
def CreateXSecJson(sampleNames_,datasets_,nXsecEvents_,XS_outName_,inputJson_,XSec_File_,dryRun_):
	useGenAnalyzer = 1 
	if(XSec_File_ != ""): useGenAnalyzer = 0
	if(useGenAnalyzer): print"Going to use GEN XSEC ANALYZER to compute cross sections"
	else: print"Going to obtain XSEC values from file:",XSec_File_
	outputName = XS_outName_
	with open(inputJson_, 'r+') as f:
		data = json.load(f)
		for iSamp,sampleName in enumerate(sampleNames_):
			dset = datasets_[iSamp]
			dataTier = "MINIAODSIM"
			campaign = "RunIIFall17MiniAODv2"
			# sampleName = sampleNames_[id]
			print"sample Name:",sampleName
			# print"dset:",datasets_[i]
			# Get dataset XSec 
			parentsFileName = "parentDatasets.txt"
			outParentsFile = open(parentsFileName, 'w+')
			parentDataset = GetParentDataset(dset)

			# if("LHEW" in sampleName): 
			# 	print"getting parent of parent"
			# 	parentDataset2 = GetParentDataset(parentDataset)
			# 	# parentDataset = GetParentDataset(parentDataset)
			# 	parentDataset=parentDataset2
			# 	dataTier = "AODSIM"
			# 	# campaign = "RunIIFall17MiniAODv2"
			# 	campaign = "RunIIFall17DRPremix"

			if(not dryRun_): outParentsFile.write("%s\n"%(parentDataset))

			outParentsFile.close()
			if(useGenAnalyzer): 
				XSec = GetXSfromGenAnalyzer(parentsFileName,nXsecEvents_,dataTier,campaign) # 2nd arg is number of events to use in computing cross section
				# cross section values: GenLumiInfoProduct_generator__SIM.obj.lheXSec_.value()
				# found this with: https://github.com/cms-sw/cmssw/blob/master/SimDataFormats/GeneratorProducts/interface/GenLumiInfoProduct.h
			else: 
				XSec = GetXSfromFile(XSec_File_,sampleName)
				if(XSec == 0): print"Could not find sample",sampleName,"in",XSec_File_

			print"XSec: %s pb"%(XSec) 			
			entry = "data['%s'] = {'xs' : %s}"%(sampleName,XSec)
			exec(entry)

	if(not dryRun_):
		with open(outputName, "w") as output:
			json.dump(data, output, indent=1)
	# else:
		# print"Dry run so not saving output json, but 

def MergeXSFiles(js1, js2, outJson):
	with open(js1, 'r+') as js1_file:
		js1_data = json.load(js1_file)
		# exists = sampleName_ in data 

	with open(js2, 'r+') as js2_file:
		js2_data = json.load(js2_file)
		# exists = sampleName_ in data 	

	# for obj in js1_data:
		# print"obj:",obj

	for obj in js2_data:
		# print obj
		if not obj in js1_data:
			shortObj = obj.split('_bmarzocc')[0]
			print"%s not in js1"%(shortObj)
			XS = js2_data[obj]['xs']
			print"XS:",XS
			entry = "js1_data['%s'] = {'xs' : %s}"%(shortObj,XS)
			exec(entry)

	# exit(1)

	with open(outJson, 'w') as outputFile:
		json.dump(js1_data, outputFile, indent=1)

if __name__ == '__main__':

	if(args.mergeXSFiles):
		MergeXSFiles(args.js1,args.js2,args.outJson)
		print"Merged %s and %s into %s"%(args.js1, args.js2, args.outJson)
		exit(1) 

	##-- Get dataset names 
	if(args.samplesFile==""):
		print"Need to provide input file"
		print"Exiting"
		exit(1)
	samplesFile = open(args.samplesFile,"r") 
	lines = samplesFile.readlines()

	possibleCampaigns = []

	if(args.HHWWgg_bkg_campaigns):
		for i in range(0,6):
			c = "HHWWgg_bkg_v%s\n"%(i)
			possibleCampaigns.append(c)
	if(args.fggCampaign):
		possibleCampaigns.append("Era2017_RR-31Mar2018_v2\n")

	campaigns = []
	allCampaignSamples = []
	campaignSamples = []
	campaign = ""
	for l in lines:
		# print"line:",l 
		# print"campaignSamples:",campaignSamples
		# if(l == "END"):
		if("END" in l):
			allCampaignSamples.append(campaignSamples)
			# print"campaignSamples",campaignSamples
			# print"allCampaignSamples",allCampaignSamples
			break 
		campaignLine = 0
		for pC in possibleCampaigns:
			if l == pC:
				campaignLine = 1
				allCampaignSamples.append(campaignSamples)
				# print"campaignSamples",campaignSamples
				# print"allCampaignSamples",allCampaignSamples
				campaigns.append(pC[:-1])
				campaignSamples = []

		if not campaignLine:
			if l != '\n':
				if l.split(' ')[0] is not "\n": 
					sample = l.split(' ')[0].replace(" ","")
					sample = l.split(' ')[0].replace("\n","")
					# print'sample:',sample
					campaignSamples.append(sample)

	samplesFile.close()
	allCampaignSamples.pop(0)
	# print"allCampaignSamples:",allCampaignSamples
	# print"campaigns:",campaigns
	# print"allCampaignSamples[0]",allCampaignSamples[0]
	# print"allCampaignSamples[1]",allCampaignSamples[1]

	sampleNames, samples = [], [] # collect all datasets and names for updating XSection json 
	for ic,cSamples in enumerate(allCampaignSamples):
		campaign = campaigns[ic]
		# print
		# print'campaign:',campaign
		# print
		if(args.short): outputName = "Taggers/test/%s_short.json"%(campaign)
		else: outputName = "Taggers/test/%s.json"%(campaign)
		
		# Begin writing file 
		Sample_JSON = '{\n' 
		Sample_JSON += '   "processes" : {\n'
		# for im,mass in enumerate(masses):
		for iS,sample in enumerate(cSamples):
			if(args.short and iS > 0): continue 
			sampleName = sample[1:]
			sampleName = sampleName.split('/')[0]
			sampleName = sampleName.replace('/','_')

			# check if sample name is defined in cross sections json
			exists = XSecExists(sampleName,args.inputJson)
			# print"exists: ",exists 
			if(args.OnlyIfExists):
				if not exists: 
					print"Need to add:",sampleName," to args.inputJson"
					existsInCurrent = XSecExists(sampleName,'MetaData/data/cross_sections.json')
					if existsInCurrent:
						print"exists in current:",existsInCurrent
					continue 

			sampleNames.append(sampleName)
			samples.append(sample)

			# print"sample name:",sampleName
			# print"dataset:",sample

			Sample_JSON += '		"{sampleName}" : ['
			Sample_JSON += ' "{sample}" '
			Sample_JSON += ' ]'
			Sample_JSON = Sample_JSON.replace("{sampleName}",str(sampleName))
			Sample_JSON = Sample_JSON.replace("{sample}",str(sample))
			if (iS is not len(cSamples)-1) and (not args.short): 
				Sample_JSON += ',\n' # need comma separation 
			else: continue # no comma at end of last object 
			# print'sample:',sample

		Sample_JSON += ' \n},\n'
		Sample_JSON += '	"cmdLine" : "campaign={campaign} targetLumi=1e+3 useAAA=1 useEOS=0 puTarget=6.245e-06,2.63e-05,4.92e-05,9.084e-05,9.854e-05,0.0001426,0.0001557,0.0001656,0.0002269,0.0005395,0.001076,0.002034,0.003219,0.004616,0.006528,0.009201,0.01283,0.01707,0.02125,0.0251,0.02847,0.03118,0.03325,0.03486,0.03626,0.03758,0.0387,0.03937,0.03946,0.03892,0.03782,0.03627,0.03435,0.03211,0.02967,0.02719,0.02482,0.02264,0.0207,0.01907,0.01784,0.01709,0.01685,0.0171,0.01771,0.01849,0.01916,0.01945,0.01911,0.01804,0.01627,0.01399,0.01147,0.008976,0.006728,0.004848,0.003375,0.002281,0.001504,0.0009715,0.0006178,0.0003882,0.0002419,0.0001501,9.294e-05,5.768e-05,3.598e-05,2.263e-05,1.437e-05,9.233e-06,5.996e-06,3.933e-06,2.601e-06,1.731e-06,1.157e-06,7.743e-07,5.184e-07,3.466e-07,2.311e-07,1.535e-07,1.015e-07,6.676e-08,4.365e-08,2.836e-08,1.829e-08,1.171e-08,7.437e-09,4.685e-09,2.926e-09,1.812e-09,1.111e-09,6.754e-10,4.066e-10,2.424e-10,1.431e-10,8.363e-11,4.839e-11,2.771e-11,1.571e-11,8.814e-12"'
		# ^^ 2017 PU 
		Sample_JSON = Sample_JSON.replace("{campaign}",campaign)
		Sample_JSON += '\n}'

		##-- Output json file for fggrunjobs if requested and not a dry run 
		if(not args.dryRun):
			if(args.createJobsJson):
				with open(outputName, "w") as output:
						output.write(Sample_JSON) # write json file 

			if(args.createJobsJson):
				print 
				print'[SampleTools.py] - ' + outputName + ' created'
				print'[SampleTools.py] - Make sure ' + outputName + ' looks good before submitting !' 
				print 

	# Create XS json if requested 
	if(args.createXSjson): CreateXSecJson(sampleNames,samples,args.nXsecEvents,args.XS_outName,args.inputJson,args.XSec_File,args.dryRun)

	# Close samples file 
	samplesFile.close() 

import os,sys,fnmatch

toydir = sys.argv[2:]
webdir = sys.argv[1]

if os.path.isdir("PlotsForWeb"):
	os.system("rm PlotsForWeb/*")
else:
	os.makedirs("PlotsForWeb")

htmlFiles = ["limit.html","pval.html"]
htmlNames = ["Limits","Pvalues"]
fileSpec = ["limit","pvals"]

sigFlag = ["B","Sx1","Sx3"]
sigNames = ["Background only","Signal (x1 SM)","Signal (x3 SM)"]

for toy in toydir:
	os.system("cp %s/ResultPlots/*.png PlotsForWeb"%toy)

os.system("cp 2Dplots/*.png PlotsForWeb/")


for k, sig in enumerate(sigFlag):
	for j,file in enumerate(htmlFiles):
		page = open("PlotsForWeb/"+sig+file,"w")
		page.write('Author: Matthew Kenzie <br>\n')
		page.write('<script language="Javascript"> \n document.write("Last modified: " + document.lastModified + ""); \n </SCRIPT> <br> \n ')
		page.write('<p> \n <h1 align=\'center\'>Global Toy Comparison</h1> \n </p> \n')
	
		page.write('<center>\n')
		page.write('Currently looking at: <br>\n')
		page.write('<b>'+sigNames[k]+' '+htmlNames[j]+'</b> <br>\n <br> \n')
		page.write('<table>\n')
		page.write('\t<tr>\n')
		for l, slink in enumerate(sigFlag):
			page.write('\t\t<td align=center><font size=\'4\'> <a href=\"'+slink+file+'\">'+sigNames[l]+'</a> &nbsp &nbsp </font></td>\n')
		page.write('\t</tr>\n')
		page.write('\t<tr>\n')
		for l, slink in enumerate(sigFlag):
			page.write('\t\t<td align=center><font size=\'4\'> <a href=\"'+slink+'2D.html\">'+slink+' 2D plots</a></td>\n')
		page.write('\t</tr>\n')
		page.write('</table>\n')
		page.write('<br> \n')
		for i,link in enumerate(htmlFiles):
			page.write('<font size=\'4\'> <a href=\"'+sig+link+'\">'+htmlNames[i]+'</a> &nbsp </font>\n')
		page.write('<br> \n')
		
		page.write('<table>\n')
		page.write('\t<tr>\n')
		page.write('\t\t<td width=\'10%\'></td>\n')
		page.write('\t\t<td width=\'45%\'><h2 align=\'center\'>Sideband Method</h2></td>\n')
		page.write('\t\t<td width=\'45%\'><h2 align=\'center\'>Background Fit Method</h2></td>\n')
		page.write('\t</tr>\n')
		listoffiles=[]
		for root,dirs,files in os.walk('PlotsForWeb'):
			for filename in fnmatch.filter(files,"%s*%s.png"%(sig,fileSpec[j])):
				listoffiles.append(filename)
		
		listoffiles.sort()
		for filename in listoffiles:
			toyname = (filename.split('_'))[0]
			if 'IC' in filename:
				page.write('\t<tr>\n')
				page.write('\t\t<td width=\'10%\'><h3 align=\'center\'>'+toyname+'</h3></td>\n')
				page.write('\t\t<td width=\'45%\'><a href='+filename+'><img height=\"400\" src=\"'+filename+'\"></a></td>\n')
			elif 'MIT' in filename:
				page.write('\t\t<td width=\'45%\'><a href='+filename+'><img height=\"400\" src=\"'+filename+'\"></a></td>\n')
				page.write('\t</tr>\n')
		page.write('</table>')
		page.write('</center>\n')
		page.close()
  
	files2D=[]
	page = open("PlotsForWeb/"+sig+"2D.html","w")
	for root, dirs, files in os.walk('2Dplots'):
		for filename in fnmatch.filter(files,'*%s*.png'%sig):
			files2D.append(filename)  
	files2D.sort()
	page.write('Author: Matthew Kenzie <br>\n')
	page.write('<script language="Javascript"> \n document.write("Last modified: " + document.lastModified + ""); \n </SCRIPT> <br> \n ')
	page.write('<p> \n <h1 align=\'center\'>Global Toy Comparison</h1> \n </p> \n')

	page.write('<center>\n')
	page.write('Currently looking at: <br>\n')
	page.write('<b> %s 2D plots </b> <br>\n<br> \n'%sig)
	page.write('<table>\n')
	page.write('\t<tr>\n')
	for l, slink in enumerate(sigFlag):
		page.write('\t\t<td align=center><font size=\'4\'> <a href=\"'+slink+file+'\">'+sigNames[l]+'</a> &nbsp &nbsp </font></td>\n')
	page.write('\t</tr>\n')
	page.write('\t<tr>\n')
	for l, slink in enumerate(sigFlag):
		page.write('\t\t<td align=center><font size=\'4\'> <a href=\"'+slink+'2D.html\">'+slink+' 2D plots</a></td>\n')
	page.write('\t</tr>\n')
	page.write('</table>\n')


	page.write('<table>\n')
	page.write('\t<tr>\n')
	switch=False
	for file in files2D:
		if 'diff' in file:
			page.write('\t\t<td width=\'25%\'><a href=\"'+file+'\"><img width=\"300\" src=\"'+file+'\"></a></td>\n')
		elif 'mH_' in file:
			if not switch:
				page.write('\t</tr> \n</table>\n<table>\n')
				switch=True
			page.write('\t<td width=\'33%\'><a href=\"'+file+'\"><img width=\"400\" src=\"'+file+'\"></a></td>\n')
		elif 'max_sig' in file:
			if switch:
				page.write('\t</tr> \n</table>\n<table>\n\t<tr>\n')
				switch=False
			page.write('\t\t<td width=\'50%\'><a href=\"'+file+'\"><img width=\"600\" src=\"'+file+'\"></a></td>\n')
	page.write('\t</tr>\n')
	page.write('</table>\n')
	page.close()

if not os.path.isdir(webdir):
  os.makedirs(webdir)
else:
  os.system("rm -r "+webdir)
  os.makedirs(webdir)
os.system("cp -r PlotsForWeb "+webdir)



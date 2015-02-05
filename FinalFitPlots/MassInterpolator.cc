#include "Macros/MassInterpolator.h"

#include "Macros/th1fmorph.C"

// ------------------------------------------------------------------------------------------------
void ChannelModel::addInputMass(TH1F * histo, float mass, int imass)
{
	masses_.resize(imass+1,0.);
	templates_.resize(imass+1);
	masses_[imass]   = mass;
	templates_[imass]= histo;
	
}

// ------------------------------------------------------------------------------------------------
void ChannelModel::addInterpolated(TH1F * histo, float mass)
{
	interpolatedMasses_.push_back(mass);
	interpolated_.push_back(histo);
}

// ------------------------------------------------------------------------------------------------
void ChannelModel::setup(AbsInterpolator * sigma, AbsInterpolator * br)
{
	sigma_ = sigma;
	br_ = br;
	assert( sigma_ != 0 && br_ != 0 );
	
	TGraph * effAcc = new TGraph(masses_.size());
	for(size_t imass=0; imass<masses_.size(); ++imass) {
		float mass = masses_[imass];
		effAcc->SetPoint( imass, masses_[imass], templates_[imass]->Integral() 
				  / ( (*sigma_)(mass) * (*br_)(mass) ) );
	}
	effAcc_ = new LinInterpolator("effAcc_"+process_+"_"+channel_,effAcc);
}


// ------------------------------------------------------------------------------------------------
MassInterpolator::MassInterpolator(size_t nmasses, double * outputMasses, bool doSmoothing, 
				   const char * th1pfx, const char * roohistpfx, const char * fmt,
				   int roundTo) :
	outputMasses_(outputMasses, outputMasses+nmasses), th1pfx_(th1pfx), roohistpfx_(roohistpfx), 
	fmt_(fmt), roundTo_(roundTo), 
	cacheChannelContaier_(0), doSmoothing_(doSmoothing), outws_(0), outfile_(0)
{
	roundMul = 1., roundDiv = 1.;
	for(int ii=0; ii<roundTo_; ++ii ) {
		roundMul *= 10.;
		roundDiv  *= 0.1;
	}
}

void MassInterpolator::printMap(){
  for (map<string,pair<double,vector<string> > >::iterator it=specProcNormMap_.begin(); it!=specProcNormMap_.end(); it++) {
    cout << it->first << " : ";
    cout << it->second.first << " : ";
    for (vector<string>::iterator vit=it->second.second.begin(); vit!=it->second.second.end(); vit++){
      cout << *vit << " ";
    }
  cout << endl;
  }
}

void MassInterpolator::addToMap(string key, double scaleFactor, vector<string> vec){
 
  specProcNormMap_.insert(pair<string,pair<double,vector<string> > >(key,make_pair(scaleFactor,vec)));
}

// ------------------------------------------------------------------------------------------------
void smoothMe(TH1F *hist);


// ------------------------------------------------------------------------------------------------
double round(double r) 
{
	return (r > 0.0) ? std::floor(r + 0.5) : std::ceil(r - 0.5);
}


// ------------------------------------------------------------------------------------------------
void MassInterpolator::addInput(float mass, const char * process, const char * channel, TH1F * histo)
{
	/// mass = round(mass*roundMul) * roundDiv;
	std::vector<float>::iterator itmass = find(inputMasses_.begin(), inputMasses_.end(), mass);
	int imass;
	if( itmass == inputMasses_.end() ) {		
		// make sure masses are added in increasing order
		if( ! inputMasses_.empty() ) {
			assert( inputMasses_.back() < mass );
		}
		inputMasses_.push_back(mass);
		imass = inputMasses_.size() - 1;
		std::cout << "Addding input mass " << mass << " as " << imass << std::endl;
	} else {
		imass = itmass - inputMasses_.begin();
	}
	
	if( doSmoothing_ ) {
		smoothMe(histo);
	}
	findContainer(process,channel)->addInputMass(histo, mass, imass);
}

// ------------------------------------------------------------------------------------------------
ChannelModel* MassInterpolator::findContainer(const char * process, const char * channel)
{
	if( cacheChannelContaier_ == 0 || std::string(cacheChannelContaier_->process()) != process 
	    || std::string(cacheChannelContaier_->channel()) != channel ) {
		processList_t::iterator iprocess = channelContainers_.find(process);
		channelsList_t::iterator ichannel;
		bool addChannel = false;
		if( iprocess == channelContainers_.end() ) {
			iprocess = channelContainers_.insert( std::make_pair(std::string(process), channelsList_t() ) ).first;
			addChannel = true;
		} else {
			ichannel = iprocess->second.find(channel);
			if( ichannel == iprocess->second.end() ) { addChannel = true; }
		}
		if( addChannel ) { 
			ichannel = iprocess->second.insert( std::make_pair(std::string(channel),ChannelModel(process, channel) ) )
				.first;
		}
		cacheChannelContaier_ = &(ichannel->second);
	}
	return cacheChannelContaier_;
}

// ------------------------------------------------------------------------------------------------
void MassInterpolator::runInterpolation(bool is7TeV)
{
	Normalization_8TeV normalizer;
	normalizer.Init(is7TeV?7:8);
	AbsInterpolator * br = new LinInterpolator("br",normalizer.GetBrGraph());
	for(processList_t::iterator iprocess = channelContainers_.begin(); iprocess != channelContainers_.end(); ++iprocess ) {
		AbsInterpolator * sigma = new LinInterpolator("sigma_"+iprocess->first,normalizer.GetSigmaGraph(iprocess->first));
		for(channelsList_t::iterator ichannel = iprocess->second.begin(); ichannel!=iprocess->second.end(); ++ichannel ) {
			ichannel->second.setup(sigma,br);
		}
	}
	
	std::vector<std::pair<int,int> > outputMassBoundaries;
	for(std::vector<float>::iterator itmass=outputMasses_.begin(); itmass!=outputMasses_.end(); ++itmass) {
		std::vector<float>::iterator bound =  lower_bound( inputMasses_.begin(), inputMasses_.end(), *itmass );		
		if( bound == inputMasses_.end() ) { 
			assert( *itmass == inputMasses_.back() ); 
			outputMassBoundaries.push_back( std::make_pair(inputMasses_.size()-1, inputMasses_.size()-1));
		} else {
			std::cout << *itmass << " " << *bound << std::endl;
			int low  = bound - inputMasses_.begin();
			if( low == 0 ) { assert( *itmass == *bound ); }
			int high = low;
			if( *bound != *itmass ) { low -= 1; }
			outputMassBoundaries.push_back( std::make_pair(low, high) );
		}
		std::cout << "Will interpolate " << *itmass << " using " << inputMasses_[outputMassBoundaries.back().first] 
			  << " and " << inputMasses_[outputMassBoundaries.back().second]  << std::endl;
	}

	for(processList_t::iterator iprocess = channelContainers_.begin(); iprocess != channelContainers_.end(); ++iprocess ) {
		std::cerr << "Interpolating " << iprocess->first << "(" << iprocess->second.size() 
			  << " channels) (" << outputMasses_.size() << " masses)"  << std::endl;
		for(channelsList_t::iterator ichannel = iprocess->second.begin(); ichannel!=iprocess->second.end(); ++ichannel ) {
			std::cerr << ".";
			ChannelModel * container = findContainer(iprocess->first.c_str(),ichannel->first.c_str());
			std::string fmt = iprocess->first+fmt_+ichannel->first;

			for(size_t imass=0; imass<outputMasses_.size(); ++imass ) {
				int low    = outputMassBoundaries[imass].first;
				int high   = outputMassBoundaries[imass].second;
				float mass = outputMasses_[imass];
				
				
				const TH1F * hlow  = container->histo(low);
				const TH1F * hhigh = container->histo(high);
				float mlow  = inputMasses_[low];
				float mhigh = inputMasses_[high];

				const TH1F * hinterp;
        float normalization;
        float norm_num;
        float norm_dem;
				if(low!=high) {
          normalization = container->normalization(mass);

          // if process is in special norm map adjust the normalization
          map<string,pair<double,vector<string> > >::iterator mapEntry = specProcNormMap_.find(iprocess->first);
          if (mapEntry!=specProcNormMap_.end()){
            double scaleFactor = mapEntry->second.first;
            vector<string> normChannels = mapEntry->second.second;
            // loop over channels to sum 
            for(channelsList_t::iterator ichannel = iprocess->second.begin(); ichannel!=iprocess->second.end(); ++ichannel ) {
              // only sum the categories not the systematics
              if (ichannel->first.find("sigma")==string::npos && ichannel->first.find("rv")==string::npos && ichannel->first.find("wv")==string::npos){
                // loop map item (i.e processes to scale to)
                for (vector<string>::iterator normCh=normChannels.begin(); normCh!=normChannels.end(); normCh++){
                  // DEBUG help
                  //cout << "Proc: " << iprocess->first << " Adding scale for " << *normCh << " " << ichannel->first << endl;
                  //cout << "    norm = " << normalization << endl;
                  norm_num += findContainer(normCh->c_str(),ichannel->first.c_str())->normalization(mass);
                }
                norm_dem += findContainer(iprocess->first.c_str(),ichannel->first.c_str())->normalization(mass);
              }
            }
            // multiply by scale factor
            normalization *= scaleFactor*(norm_num/norm_dem);
          }
					std::string hname = Form((th1pfx_+fmt).c_str(), mass);
					TH1F * hmorph = (TH1F*) th1fmorph(hname.c_str(),hname.c_str(),hlow,hhigh,mlow,mhigh,mass,normalization,0);
					hinterp = hmorph;
					container->addInterpolated(hmorph,mass);
				} else { 
          // if this is a special process also normalise the boundary histogram
          hinterp = hlow;
				}

				// 
				storeOutput( hinterp, fmt, mass, low == high );
			}
		}
		std::cerr << " Done" << std::endl;
	}

}

// ------------------------------------------------------------------------------------------------
void MassInterpolator::storeOutput(const TH1F * histo, const std::string & fmt, float mass, bool overwrite)
{
	TDirectory * pwd = gDirectory;
	if( outfile_ ) {
		if( overwrite ) {
			outfile_->cd();
			gDirectory->Delete(Form("%s;*", histo->GetName()));
			pwd->cd();
		}
		outfile_->WriteTObject(histo, histo->GetName());
	}
	if( outws_ ) {
		std::string name = Form((roohistpfx_+fmt).c_str(),mass);
		///// if( overwrite ) {
		///// 	outws_->cd();
		///// 	gDirectory->Delete((name+";*").c_str());
		///// 	pwd->cd();
		///// }
		RooDataHist rooDataHist( name.c_str(), name.c_str(), *x_, histo );
		if( outws_->import(rooDataHist) ) {
			RooDataHist & exisiting = *dynamic_cast<RooDataHist *>(outws_->data(name.c_str()));
			exisiting.reset();
			exisiting.add(rooDataHist);
		}
	}
}

// ------------------------------------------------------------------------------------------------
double getInterp(double C, double X1,double X2,double X3,double X4,double Y1,double Y2,double Y3,double Y4){


	TF1 func("f1","[0]*x*x*x+[1]*x*x+[2]*x+[3]",X1,X4);

	double entries[16];
	entries[0]=X1*X1*X1; entries[1]=X1*X1; entries[2]=X1; entries[3]=1;
	entries[4]=X2*X2*X2; entries[5]=X2*X2; entries[6]=X2; entries[7]=1;
	entries[8]=X3*X3*X3; entries[9]=X3*X3; entries[10]=X3; entries[11]=1;
	entries[12]=X4*X4*X4; entries[13]=X4*X4; entries[14]=X4; entries[15]=1;
	
	//create the Matrix;
	TMatrixD M(4,4);
	M.SetMatrixArray(entries);
	M.Invert();

	double a = M(0,0)*Y1+M(0,1)*Y2+M(0,2)*Y3+M(0,3)*Y4;
	double b = M(1,0)*Y1+M(1,1)*Y2+M(1,2)*Y3+M(1,3)*Y4;
	double c = M(2,0)*Y1+M(2,1)*Y2+M(2,2)*Y3+M(2,3)*Y4;
	double d = M(3,0)*Y1+M(3,1)*Y2+M(3,2)*Y3+M(3,3)*Y4;

	func.SetParameter(0,a);
	func.SetParameter(1,b);
	func.SetParameter(2,c);
	func.SetParameter(3,d);
	return func.Eval(C);
}

// ------------------------------------------------------------------------------------------------
void cdf2pdf(TH1F *h){

        TH1F *pdf = (TH1F*) h->Clone();

        for (int b=1;b<=pdf->GetNbinsX();b++){
		double y1 = pdf->GetBinContent(b-1);
		double y2 = pdf->GetBinContent(b);
                if (y2>y1) h->SetBinContent(b,y2-y1) ;
		else h->SetBinContent(b,0);

        }

	//return pdf;
}

// ------------------------------------------------------------------------------------------------
void pdf2cdf(TH1F *hb){

        // Make a CDF for this guy ..... 

        TH1F *cdf = (TH1F*) hb->Clone();

        for (int b=1;b<=cdf->GetNbinsX();b++){

                hb->SetBinContent(b,cdf->Integral(1,b));
        }

      //  return cdf;
}

// ------------------------------------------------------------------------------------------------
void smoothMe(TH1F *hist){

        int nEntriesPB = hist->GetEntries()/(hist->FindBin(hist->GetMean()+hist->GetRMS())-hist->FindBin(hist->GetMean()-hist->GetRMS()));

	int nBins = hist->GetNbinsX();
	// Make cdf histogram 	
	pdf2cdf(hist);

	if (nEntriesPB>100) {

  	  for (int b=1;b<=nBins;b++){
	
		if (b<3 || b>nBins-2) continue;
		else{

			double x1 = hist->GetBinCenter(b-2);
			double x2 = hist->GetBinCenter(b-1);
			double x3 = hist->GetBinCenter(b+1);
			double x4 = hist->GetBinCenter(b+2);

			double y1 = hist->GetBinContent(b-2);
			double y2 = hist->GetBinContent(b-1);
			double y3 = hist->GetBinContent(b+1);
			double y4 = hist->GetBinContent(b+2);

			hist->SetBinContent(b,getInterp(hist->GetBinCenter(b),x1,x2,x3,x4,y1,y2,y3,y4));
		
		}
	  }
	} else {
		hist->Smooth(20);
	}

	cdf2pdf(hist);
	
}

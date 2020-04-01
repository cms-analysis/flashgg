#include "flashgg/Systematics/interface/ObjectSystMethodBinnedByFunctor.h"
#include "DataFormats/Common/interface/Handle.h"
#include "DataFormats/Common/interface/PtrVector.h"
#include "flashgg/DataFormats/interface/Jet.h"
#include "CommonTools/Utils/interface/StringCutObjectSelector.h"

#include "JetMETCorrections/JetCorrector/interface/JetCorrector.h"
#include "CondFormats/JetMETObjects/interface/JetCorrectionUncertainty.h"
#include "FWCore/Framework/interface/ESHandle.h"
#include "CondFormats/JetMETObjects/interface/JetCorrectorParameters.h"
#include "JetMETCorrections/Objects/interface/JetCorrectionsRecord.h"


namespace flashgg {

    class JetEnergyCorrector: public BaseSystMethod<flashgg::Jet, int>
    {

    public:
        typedef StringCutObjectSelector<Jet, true> selector_type;

        JetEnergyCorrector( const edm::ParameterSet &conf, edm::ConsumesCollector && iC, const GlobalVariablesComputer * gv );
        void applyCorrection( flashgg::Jet &y, int syst_shift ) override;
        std::string shiftLabel( int ) const override;
        void eventInitialize( const edm::Event &iEvent, const edm::EventSetup & iSetup ) override;

    private:
        selector_type overall_range_;
        unique_ptr<JetCorrectionUncertainty> jec_unc_;
        const reco::JetCorrector* jec_cor_;
        bool debug_;
        bool uncertainties_set_;
        bool setup_uncertainties_;
        bool useTextFile_;
        edm::FileInPath textFileName_;
        std::string sourceName_;
        edm::EDGetTokenT<reco::JetCorrector> mJetCorrector;
    };


    JetEnergyCorrector::JetEnergyCorrector( const edm::ParameterSet &conf, edm::ConsumesCollector && iC, const GlobalVariablesComputer * gv ) :
        BaseSystMethod( conf, std::forward<edm::ConsumesCollector>(iC)  ),
        overall_range_( conf.getParameter<std::string>( "OverallRange" ) ),
        debug_( conf.getUntrackedParameter<bool>( "Debug", false ) ),
        setup_uncertainties_( conf.getParameter<bool>( "SetupUncertainties" ) ),
        useTextFile_( conf.getParameter<bool>( "UseTextFile" ) ),
        textFileName_( conf.getParameter<edm::FileInPath>( "TextFileName" ) ),
        sourceName_( conf.getParameter<std::string>( "SourceName" ) ),
        mJetCorrector( iC.consumes<reco::JetCorrector>(conf.getParameter<edm::InputTag>("JetCorrectorTag") ) )
    {
        uncertainties_set_ = false;
        if ( setup_uncertainties_ && useTextFile_ ) {
            throw cms::Exception( "JecSystematicConfig" ) << "You can't set both SetupUncertainties and UseTextFile to true - choose one or the other";
        }
    }

    void JetEnergyCorrector::eventInitialize( const edm::Event &iEvent, const edm::EventSetup & iSetup ) {
        if (applyCentralValue()) {
            edm::Handle<reco::JetCorrector>  corrector  ;
            iEvent.getByToken(mJetCorrector, corrector );
            jec_cor_ = corrector.product();
        }
        if (setup_uncertainties_ && !uncertainties_set_) {
            edm::ESHandle<JetCorrectorParametersCollection> JetCorParColl;
            iSetup.get<JetCorrectionsRecord>().get("AK4PFchs",JetCorParColl); 
            JetCorrectorParameters const & JetCorPar = (*JetCorParColl)["Uncertainty"];
            jec_unc_.reset( new JetCorrectionUncertainty(JetCorPar) );
            uncertainties_set_ = true;
        }
        if ( useTextFile_ ) {
            JetCorrectorParameters *corrParameters = new JetCorrectorParameters( textFileName_.fullPath(), sourceName_ );
            jec_unc_.reset( new JetCorrectionUncertainty( *corrParameters ) );
            uncertainties_set_ = true;
            delete corrParameters;
        }
    }

    std::string JetEnergyCorrector::shiftLabel( int syst_value ) const
    {
        std::string result;
        if( syst_value == 0 ) {
            result = Form( "%sCentral", label().c_str() );
        } else if( syst_value > 0 ) {
            result = Form( "%sUp%.2dsigma", label().c_str(), syst_value );
        } else {
            result = Form( "%sDown%.2dsigma", label().c_str(), -1 * syst_value );
        }
        return result;
    }

    void JetEnergyCorrector::applyCorrection( flashgg::Jet &y, int syst_shift )
    {
        if( overall_range_( y ) ) {
            double jec_adjust = 1.;
            if (applyCentralValue()) {
                double jec = jec_cor_->correction( y.correctedJet("Uncorrected") );
                double oldjec = (y.energy()/y.correctedJet("Uncorrected").energy());
                if ( debug_ ) {
                    std::cout << " DOING JEC! We get this jec from the corrector: " << jec << std::endl;
                    std::cout << "    ... previous jec was: " << oldjec << std::endl;
                }
                jec_adjust = jec/oldjec;
            }
            float unc = 0.;
            if (syst_shift != 0 && !uncertainties_set_) {
                throw cms::Exception( "JECUncertaintiesMissing" ) << " syst_shift=" << syst_shift << " but JEC Uncertanties were not set!";
            }
            if (uncertainties_set_) {
                jec_unc_->setJetEta(y.eta());
                jec_unc_->setJetPt(jec_adjust*y.pt()); 
                unc = jec_unc_->getUncertainty(true);
            }
            float scale = jec_adjust + syst_shift*unc;
            if( debug_ ) {
                std::cout << "  " << shiftLabel( syst_shift ) << ": Jet has pt= " << y.pt() << " eta=" << y.eta()
                          << " and we apply a multiplicative correction of " << scale << std::endl;
            }
            y.setP4( scale * y.p4() );
        }
    }
}

DEFINE_EDM_PLUGIN( FlashggSystematicJetMethodsFactory,
                   flashgg::JetEnergyCorrector,
                   "FlashggJetEnergyCorrector" );
// Local Variables:
// mode:c++
// indent-tabs-mode:nil
// tab-width:4
// c-basic-offset:4
// End:
// vim: tabstop=4 expandtab shiftwidth=4 softtabstop=4


#ifndef _FunctionHelpers_h_
#define _FunctionHelpers_h_

#include "Math/IFunction.h"
#include "TTree.h"
#include "TF1.h"
#include "TF2.h"
#include "TSpline.h"
#include "TH1.h"
#include "TH2.h"
#include "THnSparse.h"
#include "TList.h"

#include <list>
#include <set>
#include <algorithm>


// -----------------------------------------------------------------------------------------------
class HistoConverter   // : public ROOT::Math::IBaseFunctionMultiDim {
{
public:
    HistoConverter() : sp_( 0 ), hist_( 0 ), g_( 0 ) {};

    virtual ~HistoConverter() ;

    virtual double operator()( double *x, double *p ) = 0;
    double operator()( double x, double y = 0., double z = 0. ) { return eval( x, y, z ); };

    //// double DoEval(const double * x) const {
    //// 	return (*this)(x,0);
    //// }

    //// ROOT::Math::IBaseFunctionMultiDim * Clone() const { return clone(); }

    double eval( double x, double y = 0., double z = 0. )
    {
        double xv[3] = { x, y, z };
        return ( *this )( &xv[0], 0 );
    };
    virtual HistoConverter *clone() const = 0;

    TGraph * graph() { return g_; }

protected:
    TSpline *sp_;
    TH1   *hist_;
    TGraph *g_;


};

// -----------------------------------------------------------------------------------------------
class TF1ToFunctor : public ROOT::Math::IBaseFunctionMultiDim
{
public:
    TF1ToFunctor( TF1 *tf1 ) : tf1_( tf1 ) {};

    double DoEval( const double *x ) const
    {
        std::vector<double> xv( 3, 0. );
        for( size_t ii = 0; ii < NDim(); ++ii ) { xv[ii] = x[ii]; }
        return  tf1_->Eval( x[0], x[1], x[3] );
    };

    ROOT::Math::IBaseFunctionMultiDim *Clone() const { return new TF1ToFunctor( *this ); }

    unsigned int NDim() const { return tf1_->GetNdim(); };

private:
    TF1 *tf1_;
};



// -----------------------------------------------------------------------------------------------
template<class IntegratorT> class HistoIntegrator
{
public:
    HistoIntegrator( HistoConverter *integrand, double tolerance = 0.01 ) :
        integrand_( integrand )
    {
        integrator_.SetFunction( *integrand_ );
        integrator_.SetRelTolerance( tolerance );
    };

    double operator()( double *x, double *y )
    {
        return integrator_.Integral( x, y );
    };

    const HistoConverter *integrand() { return integrand_; };
    IntegratorT integrator() { return integrator_; };

private:
    IntegratorT integrator_;
    HistoConverter *integrand_;
};



// -----------------------------------------------------------------------------------------------
class  HistoToTF1 : public HistoConverter
{
public:
    HistoToTF1( TString name, TH1 *g ) { sp_ = new TSpline3( g, name ); hist_ = g; };
    double operator()( double *x, double *p )
    {
        return sp_->Eval( x[0] );
    };

    unsigned int NDim() const { return 1; }

    HistoConverter *clone() const { return new HistoToTF1( *this ); };
};

// -----------------------------------------------------------------------------------------------
class  GraphToTF1 : public HistoConverter
{
public:
    GraphToTF1( TString name, TGraph *g ) : capped_( false )
    { sp_ = new TSpline3( name, g ); };
    GraphToTF1( TString name, TGraph *g, double xmin, double valmin, double xmax, double valmax ) :
        capped_( true ), xmin_( xmin ), valmin_( valmin ), xmax_( xmax ), valmax_( valmax )
    {  g_ = ( TGraph * )g->Clone( name ); g_->Sort(); sp_ = new TSpline3( name, g_ ); };
    double operator()( double *x, double *p )
    {
        double val = sp_->Eval( x[0] );
        /// std::cout << x[0] << " " << val << std::endl;
        if( capped_ ) {
            if( x[0] <= xmin_ || val < valmin_ ) { return valmin_; }
            if( x[0] >= xmax_ || val > valmax_ ) { return valmax_; }
        }
        return val;
    };

    unsigned int NDim() const { return 1; }
    HistoConverter *clone() const { return new GraphToTF1( *this ); };
    TF1 *asTF1( TString name );

    TGraph *getGraph() { return g_; };

private:
    bool capped_;
    double xmin_, valmin_, xmax_, valmax_;

};


// -----------------------------------------------------------------------------------------------
class  LinGraphToTF1 : public HistoConverter
{
public:
    LinGraphToTF1( TString name, TGraph *g ) : capped_( false )
    { g_ = ( TGraph * )g->Clone( name ); };
    LinGraphToTF1( TString name, TGraph *g, double xmin, double valmin, double xmax, double valmax ) :
        capped_( true ), xmin_( xmin ), valmin_( valmin ), xmax_( xmax ), valmax_( valmax )
    { g_ = ( TGraph * )g->Clone( name ); };
    double operator()( double *x, double *p )
    {
        double val = g_->Eval( x[0] );
        if( capped_ ) {
            if( x[0] <= xmin_ || val < valmin_ ) { return valmin_; }
            if( x[0] >= xmax_ || val > valmax_ ) { return valmax_; }
        }
        return val;
    };

    unsigned int NDim() const { return 1; }
    HistoConverter *clone() const { return new LinGraphToTF1( *this ); };

private:
    bool capped_;
    double xmin_, valmin_, xmax_, valmax_;

};

// ------------------------------------------------------------------------------------------------
class  SimpleHistoToTF2 : public HistoConverter
{
public:
    SimpleHistoToTF2( TString name, TH2 *g ) { hist_ = g; };
    double operator()( double *x, double *p )
    {
        return ( ( TH2 * )hist_ )->GetBinContent( ( ( TH2 * )hist_ )->FindBin( x[0], x[1] ) );
    };

    unsigned int NDim() const { return 2; }
    HistoConverter *clone() const { return new SimpleHistoToTF2( *this ); };
};



// ------------------------------------------------------------------------------------------------
class FlatReweight : public HistoConverter
{
public:
    FlatReweight( double xmin, double xmax, double ymin, double ymax ) :
        xmin_( xmin ),
        xmax_( xmax ),
        ymin_( ymin ),
        ymax_( ymax )
    {}

    ~FlatReweight();
    double operator()( double *x, double *p ) ;

    unsigned int NDim() const { return 2; }
    HistoConverter *clone() const { return new FlatReweight( *this ); };

    void add( HistoConverter *h ) { components_.push_back( h ); }

    TF2 *asTF2( TString name );

private:
    double xmin_, xmax_, ymin_, ymax_;
    std::vector<HistoConverter *> components_;
};


// ------------------------------------------------------------------------------------------------
TH1 *integrate1D( TH1 *h, bool normalize = true );
TH2 *integrate2D( TH2 *h, bool normalize = true );

// ------------------------------------------------------------------------------------------------
template<class T = LinGraphToTF1>
HistoConverter * cdfInv( TH1 *h, double min, double max )
{
    TH1 *hi = integrate1D( h );
    TGraph g( hi );
    TGraph ginv;
    double last = 1.;
    int ilast = 0;
    double px = g.GetX()[0];
    double py = g.GetY()[0];
    for( int ip = 0; ip < g.GetN(); ++ip ) {
        double x = g.GetX()[ip];
        double y = g.GetY()[ip];
        /// std::cout << x << " " << y << std::endl;
        int jp = ginv.GetN();
        if( y != py ) {
            ginv.SetPoint( jp, 1. - py, px );
            if( ip == g.GetN() - 1 ) {
                ginv.SetPoint( jp + 1, 1. - y, x );
            }
        }
        py = y;
        px = x;
        if( y < last ) {
            last = y;
            ilast = ip;
            if( ilast == 0 && ip > 1 ) {
                min = x;
            }
        }
    }
    /// ginv.Print("all");
    max = g.GetX()[ilast];

    HistoConverter *invg = new T( Form( "%s_inv", hi->GetName() ), &ginv, 1.0e-06, min, 1., max );
    /// std::cout << min << " " << max << " " << invg->eval(max) << " " << invg->eval(g.GetX()[g.GetN()-1]) << " " << invg->eval(1.) << std::endl;
    delete hi;
    return invg;
}

// ------------------------------------------------------------------------------------------------
template<class T = LinGraphToTF1>
HistoConverter * cdf( TH1 *h, double min, double max )
{
    TH1 *hi = integrate1D( h );
    TGraph g( hi );
    TGraph ginv( g.GetN() );
    double last = 1.;
    int ilast = 0;
    for( int ip = 0; ip < g.GetN(); ++ip ) {
        double x = g.GetX()[ip];
        double y = g.GetY()[ip];
        if( y < last ) {
            last = y;
            ilast = ip;
            if( ilast == 0 && ip > 1 ) {
                min = x;
            }
        }
        ginv.SetPoint( ip, x, 1. - y );
    }
    max = g.GetX()[ilast];


    HistoConverter *invg = new T( Form( "%s_dir", hi->GetName() ), &ginv, min, 1.0e-06, max, 1. );
    /// std::cout << min << " " << max << " " << invg->eval(1.-last) << " " << g.GetY()[g.GetN()-1] << " " << invg->eval(g.GetY()[g.GetN()-1]) << " " << invg->eval(1.) << std::endl;
    delete hi;
    return invg;
}


////
//// HistoConverter * cdfInv(TH1 * h, double min, double max);
//// HistoConverter * cdf(TH1 * h, double min, double max);


// -----------------------------------------------------------------------------------------------
class IntegrationNode
{
public:
    IntegrationNode( int id, std::vector<double> coord, double w ) :
        id_( id ), coord_( coord ), weight_( w ), sum_( weight_ ), hasSum_( false ), linked_( false )
    {};

    class weakLess
    {
    public:
        bool operator()( const IntegrationNode *a, const IntegrationNode *b )
        {
            return  this->operator()( *a, *b );
        };

        bool operator()( const IntegrationNode &a, const IntegrationNode &b )
        {
            for( size_t icoord = 0; icoord < a.coord_.size(); ++icoord ) {
                if( a.coord_[icoord] < b.coord_[icoord] ) {
                    return true;
                } else if( a.coord_[icoord] > b.coord_[icoord] ) {
                    return false;
                }
            }
            return false;
        };

    };

    class strictLess
    {
    public:
        bool operator()( const IntegrationNode *a, const IntegrationNode *b )
        {
            return  this->operator()( *a, *b );
        };

        bool operator()( const IntegrationNode &a, const IntegrationNode &b )
        {
            for( size_t icoord = 0; icoord < a.coord_.size(); ++icoord ) {
                if( a.coord_[icoord] > b.coord_[icoord] ) {
                    return false;
                }
            }
            return true;
        };
    };

    void fill( double w ) { weight_ += w; sum_ += w; };
    bool isLinked() { return linked_; }
    void linked( bool x = true ) { linked_ = x; }

    void print( std::ostream &out ) const
    {
        out << "IntegrationNode " << id_ << " " << weight_;
        for( size_t ic = 0; ic < coord_.size(); ++ic ) {
            out << " " << coord_[ic];
        }
        out << "\n " << sum_ ;
        for( std::list<IntegrationNode *>::const_iterator ichild = children_.begin(); ichild != children_.end(); ++ichild ) {
            out << " " << ( *ichild )->id_ ;
            /// std::vector<double> & ccoord = (*ichild)->coord_;
            /// out << "(";
            /// for(int jc=0; jc<coord_.size(); ++jc) {
            /// 	out << " " << ccoord[jc];
            /// }
            /// out << ") ";
        }
        out << "\n";
    };

    void addChild( IntegrationNode *node )
    {
        //// children_.push_back(node);
        sum_ += node->weight_;
    };

    double sumEntries()
    {
        //// if( hasSum_ ) { return sum_; }
        //// for( std::list<IntegrationNode *>::iterator ichild=children_.begin(); ichild!=children_.end(); ++ichild) {
        //// 	sum_ += (*ichild)->sumEntries();
        //// }
        //// hasSum_ = true;
        return sum_;
    };

    int id() { return id_; };
    const std::vector<double> coord() { return coord_; };
private:
    int id_;
    std::vector<double> coord_;
    double weight_;
    double sum_;
    bool hasSum_, linked_;

    std::list<IntegrationNode *> children_;

};

// -----------------------------------------------------------------------------------------------
class IntegrationWeb : public std::set<IntegrationNode *, IntegrationNode::weakLess>
{
public:
    IntegrationWeb() : scale_( 1. ) {};

    ~IntegrationWeb()
    {
        for( iterator it = begin(); it != end(); ++it ) {
            delete *it;
        }
    };

    void link()
    {
        for( reverse_iterator it = rbegin(); it != rend(); ++it ) {
            reverse_iterator jt = it;
            ++jt;
            while( jt != rend() ) {
                if( grid_.size() == 1 || IntegrationNode::strictLess()( **jt, **it ) ) {
                    ( *jt )->addChild( *it );
                }
                ++jt;
            }
            ( *it )->linked( true );
        }
    };

    int fill( const double *coord, double w )
    {
        IntegrationNode *node = get( coord, false );
        node->fill( w );
        return node->id();
    };

    void integrate()
    {
        for( reverse_iterator it = rbegin(); it != rend(); ++it ) {
            ( *it )->sumEntries();
        }
    };

    double getIntegral( const double *coord )
    {
        return get( coord )->sumEntries() * scale_;
    };

    double getIntegral( const double *a, const double *b )
    {
        return getIntegral( b ) - getIntegral( a );
    };

    void print( std::ostream &out ) const
    {
        out << "Integration grid:\n";
        for( size_t d = 0; d < grid_.size(); ++d ) {
            out << "[" << d << "]: ";
            for( std::set<double>::iterator it = grid_[d].begin(); it != grid_[d].end(); ++it ) {
                out << *it << " ";
            }
            out << "\n";
        }


        for( iterator it = begin(); it != end(); ++it ) {
            ( *it )->print( out );
            out << std::endl;
        }
    };

    void scale( double x ) { scale_ = x; };

protected:
    double scale_;
    std::vector<std::set<double> > grid_;

    void volume_coordinates( std::vector<double> &point )
    {
        for( size_t idim = 0; idim < grid_.size(); ++idim ) {
            /// std::cout << idim << ":" << point[idim];
            std::set<double>::iterator ibound = grid_[idim].lower_bound( point[idim] );
            if( ibound == grid_[idim].end() ) {
                point[idim] = ( point[idim] <= *( grid_[idim].begin() ) ? * ( grid_[idim].begin() ) : * ( grid_[idim].rbegin() ) );
            } else if( point[idim] != *ibound ) {
                point[idim] = *( --ibound );
            }
            /// std::cout << "->" << point[idim] << " ";
        }
        /// std::cout << std::endl;
    };

    IntegrationNode *get( const double *coord, bool link = true )
    {
        std::vector<double> volume( coord, coord + grid_.size() );
        volume_coordinates( volume );
        IntegrationNode tmp( -1, volume, 0. );
        iterator inode = lower_bound( &tmp );
        if( inode == end() || key_comp()( **inode, tmp ) || key_comp()( tmp, **inode ) ) {
            inode = insert( inode, new IntegrationNode( size(), volume, 0. ) );
        }
        if( link && !( *inode )->isLinked() ) {
            iterator jnode = inode;
            ++jnode;
            while( jnode != end() ) {
                if( IntegrationNode::strictLess()( **inode, **jnode ) ) {
                    ( *inode )->addChild( *jnode );
                }
                ++jnode;
            }
            ( *inode )->linked( true );
        }
        //// (*inode)->print(std::cout);
        return *inode;
    };


};

// -----------------------------------------------------------------------------------------------
class SparseIntegrator : public IntegrationWeb, public HistoConverter
{
public:
    SparseIntegrator( THnSparse *integrand, double scale = 1. ) : hsp_( integrand )
    {
        /// reserve(integrand->GetNbins());
        scale_ = scale;
        Int_t dim = integrand->GetNdimensions();
        std::vector<int> bins( dim );
        std::vector<double> coord( dim );
        grid_.resize( dim );
        for( Int_t d = 0; d < dim; ++d ) {
            const TAxis *axis = integrand->GetAxis( d );
            for( int ibin = 0; ibin <= axis->GetNbins(); ++ibin ) {
                grid_[d].insert( axis->GetBinLowEdge( ibin ) );
            }
            //// for(std::set<double>::iterator it=grid_[d].begin(); it!=grid_[d].end(); ++it) {
            //// 	std::cout << *it << " ";
            //// }
            //// std::cout << std::endl;
        }
        for( Long64_t i = 0; i < integrand->GetNbins(); ++i ) {
            double weight = integrand->GetBinContent( i, &bins[0] );
            for( Int_t d = 0; d < dim; ++d ) {
                coord[d] = integrand->GetAxis( d )->GetBinLowEdge( bins[d] );
            }
            insert( new IntegrationNode( i, coord, weight ) );
        }
        link();
    };

    ~SparseIntegrator()
    {
        /// delete hsp_;
    };

    THnSparse *getIntegrand() { return hsp_; };

    double operator()( double *x, double *p )
    {
        return getIntegral( x );
    };

    unsigned int NDim() const { return grid_.size(); };

    HistoConverter *clone() const { return new SparseIntegrator( *this ); };

private:
    THnSparse *hsp_;

};


class DecorrTransform : public HistoConverter
{
public:
    DecorrTransform( TH2 *histo, float ref, bool doRef, bool doRatio = false, bool invert = false );

    virtual double operator()( double *x, double *p );
    virtual HistoConverter *clone() const;

    unsigned int NDim() const { return 2; };

    HistoConverter *getInvConverter() { return invtr_; };
    HistoConverter *getConverter( double x )
    {
        int ibin = hist_->GetXaxis()->FindBin( x );
        if(ibin==0){
            ibin=1;
        }
        return dirtr_[ibin];
    };

    double xmin() { return hist_->GetXaxis()->GetXmin(); };
    double xmax() { return hist_->GetXaxis()->GetXmax(); };
    double ymin() { return hist_->GetYaxis()->GetXmin(); };
    double ymax() { return hist_->GetYaxis()->GetXmax(); };

private:
    TH2 *hist_;
    int refbin_;
    double doRatio_;
    bool doRef_;

    HistoConverter *invtr_;
    std::vector<HistoConverter *> dirtr_;

};




#endif

// Local Variables:
// mode:c++
// indent-tabs-mode:nil
// tab-width:4
// c-basic-offset:4
// End:
// vim: tabstop=4 expandtab shiftwidth=4 softtabstop=4


#ifndef flashgg_CutAndClassBasedClassifier_h
#define flashgg_CutAndClassBasedClassifier_h

#include "CommonTools/Utils/interface/StringCutObjectSelector.h"

#include "CutBasedClassifier.h"

namespace flashgg {
    template <class T>
    class CutAndClassBasedClassifier : private CutBasedClassifier<T>, private ClassNameClassifier<T>
    {
    public:
        typedef StringCutObjectSelector<T, true> functor_type;

        CutAndClassBasedClassifier( const edm::ParameterSet &cfg ) : 
            CutBasedClassifier<T>(cfg),
            ClassNameClassifier<T>(cfg)
        {
        }

        std::pair<std::string, int> operator()( const T &obj ) const
        {
            auto cutbased = CutBasedClassifier<T>::operator()(obj);
            
            auto classbased = ClassNameClassifier<T>::operator()(obj);
            
            std::string cat = classbased.first;
            if( ! cutbased.first.empty() ) {
                cat += (cat.empty()?"":":")+cutbased.first; // FIXME: define ad-hoc method with dedicated + operator
            }
            return std::make_pair( cat, classbased.second  );
        }
    };
}

#endif // flashgg_CutAndClassBasedClassifier_h
// Local Variables:
// mode:c++
// indent-tabs-mode:nil
// tab-width:4
// c-basic-offset:4
// End:
// vim: tabstop=4 expandtab shiftwidth=4 softtabstop=4


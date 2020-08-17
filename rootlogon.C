{
    gSystem->Load( "libFWCoreFWLite.so" );
    FWLiteEnabler::enable();
    gSystem->Load( "libDataFormatsFWLite.so" );
    gSystem->Load( "libDataFormatsPatCandidates.so" );
}
// Local Variables:
// mode:c++
// indent-tabs-mode:nil
// tab-width:4
// c-basic-offset:4
// End:
// vim: tabstop=4 expandtab shiftwidth=4 softtabstop=4


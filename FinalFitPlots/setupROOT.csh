

source /afs/cern.ch/sw/lcg/external/gcc/4.6.2/x86_64-slc5-gcc46-opt/setup.csh

setenv ROOTSYS /afs/cern.ch/sw/lcg/app/releases/ROOT/5.32.00/x86_64-slc5-gcc46-opt/root/

setenv PATH "$ROOTSYS/bin:$PATH"


if ($?LD_LIBRARY_PATH) then
   setenv LD_LIBRARY_PATH "$ROOTSYS/lib:$LD_LIBRARY_PATH"      # Linux, ELF HP-UX
else
   setenv LD_LIBRARY_PATH $ROOTSYS/lib
endif


if ($?PYTHONPATH) then
   setenv PYTHONPATH $ROOTSYS/lib:$PYTHONPATH
else
   setenv PYTHONPATH $ROOTSYS/lib
endif

#if ($?MANPATH) then
#   setenv MANPATH `dirname $ROOTSYS/man/man1`:$MANPATH
#else
#   setenv MANPATH `dirname $ROOTSYS/man/man1`
#endif




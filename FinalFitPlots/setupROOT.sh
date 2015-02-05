
source /afs/cern.ch/sw/lcg/external/gcc/4.6.2/x86_64-slc5-gcc46-opt/setup.sh
export ROOTSYS=/afs/cern.ch/sw/lcg/app/releases/ROOT/5.32.00/x86_64-slc5-gcc46-opt/root/
export PATH="$ROOTSYS/bin:$PATH"

if [ -n "${LD_LIBRARY_PATH:+x}" ]
then
   export LD_LIBRARY_PATH="$ROOTSYS/lib:$LD_LIBRARY_PATH"      # Linux, ELF HP-UX
else
   export LD_LIBRARY_PATH=$ROOTSYS/lib
fi


if [ -n "${PYTHONPATH:+x}" ] 
then
   export PYTHONPATH=$ROOTSYS/lib:$PYTHONPATH
else
   export PYTHONPATH=$ROOTSYS/lib
fi

#if ($?MANPATH) then
#   setenv MANPATH `dirname $ROOTSYS/man/man1`:$MANPATH
#else
#   setenv MANPATH `dirname $ROOTSYS/man/man1`
#endif




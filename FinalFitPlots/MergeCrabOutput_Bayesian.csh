#!/bin/tcsh
foreach i ( `/bin/ls -d * | grep $1` )
	cd ${i}/res/
	echo $PWD
	/bin/ls outputToy_*.tgz | xargs -i tar -xvf {}
	cd outputToy/
	set mass = `echo ${i} | sed 's|\([0-9]*\).*|\1|'`
	set name = `/bin/ls *.root | awk -F. '{print $2}'`
	set name = `echo $name | awk '{print $1}'`
	hadd higgsCombineTest.${name}.mH${mass}.root higgsCombineTest.${name}.mH${mass}.*.root
	cp higgsCombineTest.${name}.mH${mass}.root ../../../expectedBayesian.mH${mass}.root
	cd ../../../
end

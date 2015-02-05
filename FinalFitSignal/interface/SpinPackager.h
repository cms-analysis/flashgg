#ifndef SpinPackager_h 
#define SpinPackager_h

#include <iostream>
#include <string>
#include <vector>

#include "RooWorkspace.h"

class SpinPackager {
	
	public:

		SpinPackager(RooWorkspace *ws, std::vector<std::string> smProcs, std::vector<std::string> gravProcs, int nCats, int mh, int sqrts);

		void package(std::vector<std::string> procs, std::string label);
		void packageOutput();

	private:
		RooWorkspace *ws_;
		std::vector<std::string> smProcs_;
		std::vector<std::string> gravProcs_;
		int nCats_;
		int mh_;
		int sqrts_;

};

#endif

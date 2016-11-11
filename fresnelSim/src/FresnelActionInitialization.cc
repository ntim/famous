/*
 * FresnelActionInitialization.cc
 *
 * @date 03.06.2016
 * @author Tim Niggemann, III Phys. Inst. A, RWTH Aachen University
 * @copyright GNU General Public License v3.0
 */

#include "FresnelActionInitialization.hh"

#include "FresnelRunAction.hh"

FresnelActionInitialization::FresnelActionInitialization(std::string path) :
		CoreActionInitialization(path, "fresnel") {
	//
}

CoreRunAction* FresnelActionInitialization::createRunAction() const {
	return new FresnelRunAction(getFilename());
}

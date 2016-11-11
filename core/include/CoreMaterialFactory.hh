/*
 * CoreMaterialFactory.hh
 *
 * @date Apr 23, 2012
 * @author Tim Niggemann, III Phys. Inst. A, RWTH Aachen University
 * @copyright GNU General Public License v3.0
 */

#ifndef COREMATERIALFACTORY_HH_
#define COREMATERIALFACTORY_HH_

#include <G4Material.hh>

class CoreMaterialFactory {
private:
	G4Material* aluminium;
	G4Material* silicon;
	G4Material* dummyMatter;
	G4Material* glass;
	G4Material* perfectMirror;
	G4Material* acrylic;
	G4Material* ug11;

	CoreMaterialFactory();

public:
	static const double LAMBDA_MIN;
	static const double LAMBDA_MAX;
	static const double ENERGY_MIN;
	static const double ENERGY_MAX;

	virtual ~CoreMaterialFactory();

	static CoreMaterialFactory* getInstance();

	G4Material* getAir();
	G4Material* getAluminum();
	G4Material* getSilicon();
	G4Material* getDummyMatter();
	G4Material* getGlass();
	G4Material* getPerfectMirror();
	G4Material* getAcrylic();
	G4Material* getUG11();
};

#endif /* COREMATERIALFACTORY_HH_ */

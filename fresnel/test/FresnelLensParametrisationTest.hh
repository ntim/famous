/**
 * FresnelLensParametrisationTest.hh
 *
 * @date Mar 7, 2012
 * @author Tim Niggemann, III Phys. Inst. A, RWTH Aachen University
 * @copyright GNU General Public License v3.0
 */

#ifndef FRESNELLENSPARAMETRISATIONTEST_HH_
#define FRESNELLENSPARAMETRISATIONTEST_HH_

#include <gtest/gtest.h>
#include <CLHEP/Units/SystemOfUnits.h>
#include <G4LogicalVolume.hh>
#include <G4Tubs.hh>
#include <G4PVParameterised.hh>

#include "FresnelLensParametrisation.hh"
#include "model/FresnelLensModel.hh"
#include "CoreMaterialFactory.hh"

using namespace CLHEP;

class FresnelLensParametrisationTest: public testing::Test {
protected:
	FresnelLensParametrisation* testedObject;
	FresnelLensModel* model;

	G4LogicalVolume* createMother() {
		G4VSolid* solid = new G4Tubs("motherSolid", 0, model->getR(), model->getTotalThickness(), 0, CLHEP::twopi);
		return new G4LogicalVolume(solid, CoreMaterialFactory::getInstance()->getAir(), "motherLv");
	}

	G4LogicalVolume* createGroovePrototype() {
		G4Cons* groove = new G4Cons("groove", 0, 0, 0, 0, 0, 0, CLHEP::twopi);
		return new G4LogicalVolume(groove, model->getMaterial(), "grooveLv");
	}

	virtual void SetUp() {
		model = new FresnelLensModel(10. * mm, 10. * mm);
		testedObject = new FresnelLensParametrisation(model);
	}
};

TEST_F(FresnelLensParametrisationTest, shouldNotFail) {
	new G4PVParameterised("testLens", createGroovePrototype(), createMother(), kZAxis, model->getTotalNumberOfGrooves(),
			testedObject, true);
}

#endif /* FRESNELLENSPARAMETRISATIONTEST_HH_ */

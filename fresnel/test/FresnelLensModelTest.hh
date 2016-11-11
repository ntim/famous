/**
 * FresnelLensModelTest.hh
 *
 * @date Mar 7, 2012
 * @author Tim Niggemann, III Phys. Inst. A, RWTH Aachen University
 * @copyright GNU General Public License v3.0
 */

#include <gtest/gtest.h>
#include <CLHEP/Units/SystemOfUnits.h>

#include "model/FresnelLensModel.hh"

using namespace CLHEP;

class FresnelLensModelTest: public testing::Test {
protected:
	FresnelLensModel* testedObject;
	double rIndex;
	double rIndexAir;
	double c;
	int totalNumberOfGrooves;
	double heighestGroove;

	virtual void SetUp() {
		testedObject = new FresnelLensModel(1. * m, 1. * m);
		rIndexAir = 1.0001720000000001;
		rIndex = 1.515333325;
		c = 0.001941139506;
		totalNumberOfGrooves = 2000;
		heighestGroove = 0.9703270 * mm;
	}
};

TEST_F(FresnelLensModelTest, getRIndex) {
	ASSERT_NEAR(rIndex, testedObject->getRIndex(), rIndex * 1e-5);
}

TEST_F(FresnelLensModelTest, getRIndexAir) {
	ASSERT_NEAR(rIndexAir, testedObject->getRIndexAir(), rIndexAir * 1e-5);
}

TEST_F(FresnelLensModelTest, getC) {
	ASSERT_NEAR(c, testedObject->getC(), c * 1e-5);
}

TEST_F(FresnelLensModelTest, getTotalNumberOfGrooves) {
	ASSERT_EQ(totalNumberOfGrooves, testedObject->getTotalNumberOfGrooves());
}

TEST_F(FresnelLensModelTest, getSagitta) {
	// Test simple case.
	ASSERT_NEAR(0, testedObject->getSagitta(0), 1e-5);
	// Set conic constant to -2 in order to check the calculation of the denominator.
	testedObject->setK(-1.5);
	// Values obtained with maple.
	double x[] = { 0, 100, 200, 300, 400, 500, 600, 700, 800, 900, 1000 };
	double s[] = { 0., 9.660409045, 38.11769139, 83.93260041, 145.0771812, 219.3031992, 304.4300473, 398.5107073,
			499.8945776, 607.2257501, 719.4101189, };
	for (unsigned int i = 0; i < 11; ++i) {
		ASSERT_NEAR(s[i], testedObject->getSagitta(x[i]), s[i]*1e-5);
	}
}

TEST_F(FresnelLensModelTest, getTotalThickness) {
	ASSERT_NEAR(heighestGroove + testedObject->getThickness(), testedObject->getTotalThickness(), 1e-5);
}


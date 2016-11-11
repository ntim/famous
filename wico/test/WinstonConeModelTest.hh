/**
 * WinstonConeModelTest.hh
 *
 * @date Mar 5, 2012
 * @author Tim Niggemann, III Phys. Inst. A, RWTH Aachen University
 * @copyright GNU General Public License v3.0
 */

#ifndef WINSTONCONEMODELTEST_HH_
#define WINSTONCONEMODELTEST_HH_

#include <gtest/gtest.h>

#include <CLHEP/Units/SystemOfUnits.h>
#include <CLHEP/Units/PhysicalConstants.h>

#include "VectorUtil.hh"
#include "model/WinstonConeModelFactory.hh"

class WinstonConeModelTest: public testing::Test {
protected:
	WinstonConeModel* testedObject;

	std::vector<double> theta, z, r;
	double length;

	virtual void SetUp() {
		length = 2.59808;
		// Initialize arrays.
		double thetaArray[] = { 30.000000, 32.307700, 34.615400, 36.923100, 39.230800, 41.538500, 43.846200, 46.153800,
				48.461500, 50.769200, 53.076900, 55.384600, 57.692300, 60.000000, 62.307700, 64.615400, 66.923100,
				69.230800, 71.538500, 73.846200, 76.153800, 78.461500, 80.769200, 83.076900, 85.384600, 87.692300, };
		double zArray[] = { 2.598080, 2.368470, 2.160790, 1.972200, 1.800300, 1.643080, 1.498790, 1.365990, 1.243390,
				1.129920, 1.024620, 0.926662, 0.835334, 0.750000, 0.670102, 0.595149, 0.524704, 0.458378, 0.395827,
				0.336739, 0.280839, 0.227877, 0.177629, 0.129892, 0.084483, 0.041236, };
		double rArray[] = { 1.000000, 0.997727, 0.991486, 0.982013, 0.969903, 0.955638, 0.939612, 0.922145, 0.903503,
				0.883901, 0.863518, 0.842502, 0.820975, 0.799038, 0.776774, 0.754250, 0.731525, 0.708644, 0.685645,
				0.662559, 0.639409, 0.616216, 0.592994, 0.569755, 0.546506, 0.523254, };
		theta = VectorUtil::times(VectorUtil::adapt(thetaArray, 26), CLHEP::deg);
		z = VectorUtil::adapt(zArray, 26);
		r = VectorUtil::adapt(rArray, 26);
		//
		testedObject = WinstonConeModelFactory::createRound(1, 0.5);
	}
};

TEST_F(WinstonConeModelTest, getLength) {
	ASSERT_NEAR(length, testedObject->getLength(), 1e-4);
}

TEST_F(WinstonConeModelTest, getThetaMax) {
	ASSERT_DOUBLE_EQ(30.0 * CLHEP::deg, testedObject->getThetaMax());
}

TEST_F(WinstonConeModelTest, getF) {
	ASSERT_DOUBLE_EQ(0.75, testedObject->getF());
}

TEST_F(WinstonConeModelTest, getR1) {
	ASSERT_DOUBLE_EQ(1.0, testedObject->getR1());
}

TEST_F(WinstonConeModelTest, getR2) {
	ASSERT_DOUBLE_EQ(0.5, testedObject->getR2());
}

TEST_F(WinstonConeModelTest, getNumberOfSides) {
	ASSERT_EQ(0, testedObject->getNumberOfSides());
}

TEST_F(WinstonConeModelTest, getZ) {
	for (unsigned int i = 0; i < z.size(); ++i) {
		ASSERT_NEAR(z[i] - length/2., testedObject->getZ(theta[i]), 1e-3);
	}
}

TEST_F(WinstonConeModelTest, shouldFail) {
	EXPECT_ANY_THROW({ testedObject->getZ(0); });
	EXPECT_ANY_THROW({ testedObject->getZ(CLHEP::pi); });
	EXPECT_ANY_THROW({ testedObject->getR(0); });
	EXPECT_ANY_THROW({ testedObject->getR(CLHEP::pi); });
}

TEST_F(WinstonConeModelTest, getR) {
	for (unsigned int i = 0; i < r.size(); ++i) {
		ASSERT_NEAR(r[i], testedObject->getR(theta[i]), 1e-3);
	}
}

#endif /* WINSTONCONEMODELTEST_HH_ */

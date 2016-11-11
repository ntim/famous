/*
 * GeneralParticleSourceMessengerTest.hh
 *
 * @date Aug 12, 2014
 * @author Tim Niggemann, III Phys. Inst. A, RWTH Aachen University
 * @copyright GNU General Public License v3.0
 */

#ifndef GENERALPARTICLESOURCEMESSENGERTEST_HH_
#define GENERALPARTICLESOURCEMESSENGERTEST_HH_

#include <gtest/gtest.h>

#include "GeneralParticleSourceMessenger.hh"

class GeneralParticleSourceMessengerTest: public testing::Test {
protected:
	virtual void SetUp() {
		//
	}
};

TEST_F(GeneralParticleSourceMessengerTest, getInstance) {
	GeneralParticleSourceMessenger::getInstance();
}

#endif /* GENERALPARTICLESOURCEMESSENGERTEST_HH_ */

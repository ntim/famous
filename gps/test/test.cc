/*
 * test.cc
 *
 * @date Jul 29, 2014
 * @author Tim Niggemann, III Phys. Inst. A, RWTH Aachen University
 * @copyright GNU General Public License v3.0
 */

#include <gtest/gtest.h>

#include "GeneralParticleSourceMessengerTest.hh"

int main(int argc, char **argv) {
	testing::InitGoogleTest(&argc, argv);
	// Run all tests.
	return RUN_ALL_TESTS();
}

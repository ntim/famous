/*
 * HexTest.hh
 *
 * @date Jul 29, 2014
 * @author Tim Niggemann, III Phys. Inst. A, RWTH Aachen University
 * @copyright GNU General Public License v3.0
 */

#ifndef HEXTEST_HH_
#define HEXTEST_HH_

#include <gtest/gtest.h>

#include <cmath>

#include "Hex.hh"

class HexTest: public testing::Test {
protected:
	virtual void SetUp() {
		//
	}
};

TEST_F(HexTest, round) {
	hex::Point p = hex::round( { 0.3, -0.3, 0 });
	ASSERT_EQ(p.toAxial().q, 0.0);
	ASSERT_EQ(p.toAxial().r, 0.0);
}

TEST_F(HexTest, grid) {
	size_t num = 64;
	std::vector<hex::Point> points = hex::grid(num);
	ASSERT_EQ(num, points.size());
}

#endif /* HEXTEST_HH_ */

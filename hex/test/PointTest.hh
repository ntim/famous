/*
 * PointTest.hh
 *
 * @date Jul 29, 2014
 * @author Tim Niggemann, III Phys. Inst. A, RWTH Aachen University
 * @copyright GNU General Public License v3.0
 */

#ifndef POINTTEST_HH_
#define POINTTEST_HH_

#include <gtest/gtest.h>

#include <cmath>

#include "Point.hh"

class PointTest: public testing::Test {
protected:
	virtual void SetUp() {
		//
	}
};

TEST_F(PointTest, toAxial) {
	hex::Point p = hex::Point::fromAxial( { 1, 0 });
	ASSERT_EQ(p.toAxial().q, 1);
	ASSERT_EQ(p.toAxial().r, 0);
	p = hex::Point::fromCube( { 1, -1, 0 });
	ASSERT_EQ(p.toAxial().q, 1);
	ASSERT_EQ(p.toAxial().r, 0);
}

TEST_F(PointTest, toCube) {
	hex::Point p = hex::Point::fromAxial( { 1, 0 });
	ASSERT_EQ(p.toCube().x, 1);
	ASSERT_EQ(p.toCube().y, -1);
	ASSERT_EQ(p.toCube().z, 0);
}

TEST_F(PointTest, toCartesian) {
	hex::Point p = hex::Point::fromAxial( { 1, 0 }, 2.0);
	ASSERT_EQ(p.toCartesian().x, 2.0 * sqrt(3.0) / 2.0);
	ASSERT_EQ(p.toCartesian().y, 2.0 * 3.0 / 2.0);
}

#endif /* POINTTEST_HH_ */

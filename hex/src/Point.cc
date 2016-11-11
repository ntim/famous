/*
 * Point.cc
 *
 * @date Jul 29, 2014
 * @author Tim Niggemann, III Phys. Inst. A, RWTH Aachen University
 * @copyright GNU General Public License v3.0
 */

#include "Point.hh"

#include <cmath>
#include <algorithm>

namespace hex {

Point::Point(double q, double r, double s) {
	c = {q, r};
	size = s;
}

Point Point::fromAxial(AxialCoordinate c, double size) {
	return Point(c.q, c.r, size);
}

Point Point::fromCube(CubeCoordinate c, double size) {
	return Point(c.x, c.z, size);
}

AxialCoordinate Point::toAxial() const {
	return c;
}

CubeCoordinate Point::toCube() const {
	return {c.q, -c.q - c.r, c.r};
}

CartesianCoordinate Point::toCartesian() const {
	return {size * sqrt(3.0) * (c.r + c.q / 2.0), size * 3.0 / 2.0 * c.q};
}

CartesianCoordinate Point::toCartesian(Point offset) const {
	CartesianCoordinate o = offset.toCartesian();
	return {size * sqrt(3.0) * (c.r + c.q / 2.0) - o.x, size * 3.0 / 2.0 * c.q - o.y};
}

double Point::distance(Point h) const {
	CubeCoordinate p1 = toCube();
	CubeCoordinate p2 = h.toCube();
	return (fabs(p1.x - p2.x) + fabs(p1.y - p2.y) + fabs(p1.z - p2.z)) / 2.0;
}

double Point::cartesianDistance(Point h) const {
	return cartesianDistance(h, Point::fromAxial( { 0, 0 }));
}

double Point::cartesianDistance(Point h, Point offset) const {
	CartesianCoordinate p1 = toCartesian(offset);
	CartesianCoordinate p2 = h.toCartesian(offset);
	return sqrt((p1.x - p2.x) * (p1.x - p2.x) + (p1.y - p2.y) * (p1.y - p2.y));
}

double Point::getSize() const {
	return size;
}

} /* namespace hex */


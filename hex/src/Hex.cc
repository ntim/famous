/*
 * Hex.cc
 *
 * @date Jul 29, 2014
 * @author Tim Niggemann, III Phys. Inst. A, RWTH Aachen University
 * @copyright GNU General Public License v3.0
 */

#include "Hex.hh"

#include <cmath>
#include <algorithm>

namespace hex {

Point round(CubeCoordinate c) {
	CubeCoordinate r = { roundf((double) c.x), roundf((double) c.y), roundf((double) c.z) };
	CubeCoordinate diff = { fabs(r.x - c.x), fabs(r.y - c.y), fabs(r.z - c.z) };
	if (diff.x > diff.y and diff.x > diff.z) {
		r.x = -r.y - r.z;
	} else if (diff.y > diff.z) {
		r.y = -r.x - r.z;
	} else {
		r.z = -r.x - r.y;
	}
	return Point::fromCube(r);
}

struct CartesianDistanceToCenterComparator {
	Point center;

	bool operator()(Point h1, Point h2) {
		return h1.cartesianDistance(center) < h2.cartesianDistance(center);
	}
};

std::vector<Point> grid(std::size_t num, double size) {
	return grid(num, size, Point::fromAxial( { 0.0, 0.5 }, size));
}

std::vector<Point> grid(std::size_t num, double size, Point center) {
	// Iterate over all possible x, y, z.
	std::vector<Point> points;
	int max = (int) ceil(sqrt(num) / 2.0);
	for (int x = -max; x < max + 1; x++) {
		for (int y = -max - 1; y < max + 1; y++) {
			for (int z = -max; z < max + 2; z++) {
				if (x + y + z == 0) {
					points.push_back(Point::fromCube( { (double) x, (double) y, (double) z }, size));
				}
			}
		}
	}
	// Sort pixels according to distance to the center.
	CartesianDistanceToCenterComparator comparator = { center };
	std::sort(points.begin(), points.end(), comparator);
	// Truncate vector to num.
	if (points.size() > num) {
		points.erase(points.begin() + num, points.end());
	}
	return points;
}

void polygon(Point p, std::vector<double>& x, std::vector<double>& y) {
	polygon(p, Point::fromAxial( { 0, 0 }), x, y);
}

void polygon(Point p, Point offset, std::vector<double>& x, std::vector<double>& y) {
	CartesianCoordinate c = p.toCartesian();
	CartesianCoordinate o = offset.toCartesian();
	for (size_t i = 0; i < 7; i++) {
		x.push_back(cos(2.0 * M_PI / 6.0 * (i + 0.5)) * p.getSize() + c.x - o.x);
		y.push_back(sin(2.0 * M_PI / 6.0 * (i + 0.5)) * p.getSize() + c.y - o.y);
	}
}

} /* namespace hex */

/*
 * Point.hh
 *
 * @date Jul 29, 2014
 * @author Tim Niggemann, III Phys. Inst. A, RWTH Aachen University
 * @copyright GNU General Public License v3.0
 */

#ifndef POINT_HH_
#define POINT_HH_

namespace hex {

struct AxialCoordinate {
	double q, r;
};

struct CubeCoordinate {
	double x, y, z;
};

struct CartesianCoordinate {
	double x, y;
};

class Point {
private:
	AxialCoordinate c;
	double size;

public:
	Point(double q, double r, double size);
	static Point fromAxial(AxialCoordinate c, double size = 1.0);
	static Point fromCube(CubeCoordinate c, double size = 1.0);

	AxialCoordinate toAxial() const;
	CubeCoordinate toCube() const;
	CartesianCoordinate toCartesian() const;
	CartesianCoordinate toCartesian(Point offset) const;
	double distance(Point h) const;
	double cartesianDistance(Point h) const;
	double cartesianDistance(Point h, Point offset) const;

	double getSize() const;
};

} /* namespace hex */

#endif /* POINT_HH_ */

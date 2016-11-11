/*
 * Hex.hh
 *
 * @date Jul 29, 2014
 * @author Tim Niggemann, III Phys. Inst. A, RWTH Aachen University
 * @copyright GNU General Public License v3.0
 */

#ifndef HEX_HH_
#define HEX_HH_

#include <vector>

#include "Point.hh"

namespace hex {

Point round(CubeCoordinate c);
std::vector<Point> grid(std::size_t num, double size = 1.0);
std::vector<Point> grid(std::size_t num, double size, Point center);
void polygon(Point p, std::vector<double>& x, std::vector<double>& y);
void polygon(Point p, Point offset, std::vector<double>& x, std::vector<double>& y);

} /* namespace hex */

#endif /* HEX_HH_ */

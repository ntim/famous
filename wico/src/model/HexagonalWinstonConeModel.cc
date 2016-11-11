/*
 * HexagonalWinstonConeModel.cc
 *
 * @date Jul 30, 2014
 * @author Tim Niggemann, III Phys. Inst. A, RWTH Aachen University
 * @copyright GNU General Public License v3.0
 */

#include "model/HexagonalWinstonConeModel.hh"

HexagonalWinstonConeModel::HexagonalWinstonConeModel(double _r1, double _r2) :
		WinstonConeModel(_r1, _r2) {
	//
}

HexagonalWinstonConeModel::~HexagonalWinstonConeModel() {
	//
}

int HexagonalWinstonConeModel::getNumberOfSides() const {
	return 6;
}

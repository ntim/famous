/**
 * ParabolicConcentratorModel.hh
 *
 * @date Mar 5, 2012
 * @author Tim Niggemann, III Phys. Inst. A, RWTH Aachen University
 * @copyright GNU General Public License v3.0
 */

#ifndef PARABOLICCONCENTRATORMODEL_HH_
#define PARABOLICCONCENTRATORMODEL_HH_

#include "WinstonConeModel.hh"

class ParabolicConcentratorModel: public WinstonConeModel {
private:

	double parabola(double x) const;

public:
	ParabolicConcentratorModel(double r1, double r2);
	virtual ~ParabolicConcentratorModel();

	virtual double getF() const;
	virtual double getLength() const;
	virtual double getThetaMax() const;
	virtual double getR(double theta) const;
	virtual double getZ(double theta) const;
};

#endif /* PARABOLICCONCENTRATORMODEL_HH_ */

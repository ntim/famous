/*
 * FamousPersistVisitor.hh
 *
 * @date Oct 9, 2014
 * @author Tim Niggemann, III Phys. Inst. A, RWTH Aachen University
 * @copyright GNU General Public License v3.0
 */

#ifndef FAMOUSPERSISTVISITOR_HH_
#define FAMOUSPERSISTVISITOR_HH_

#include "CoreRootPersistVisitor.hh"

#include "model/FamousModel.hh"
#include "model/FresnelLensModel.hh"
#include "model/WinstonConeModel.hh"

class FamousPersistVisitor: public CoreRootPersistVisitor {
public:
	using CoreRootPersistVisitor::operator ();

	virtual void operator ()(FamousModel* m);
	virtual void operator ()(FresnelLensModel* m);
	virtual void operator ()(WinstonConeModel* m);
};

#endif /* FAMOUSPERSISTVISITOR_HH_ */

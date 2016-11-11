/*
 * FresnelPersistVisitor.hh
 *
 * @date 03.06.2016
 * @author Tim Niggemann, III Phys. Inst. A, RWTH Aachen University
 * @copyright GNU General Public License v3.0
 */

#ifndef FRESNELSIM_INCLUDE_FRESNELPERSISTVISITOR_HH_
#define FRESNELSIM_INCLUDE_FRESNELPERSISTVISITOR_HH_

#include "CoreRootPersistVisitor.hh"

#include "model/FresnelLensModel.hh"

class FresnelPersistVisitor: public CoreRootPersistVisitor {
public:
	using CoreRootPersistVisitor::operator ();

	virtual void operator ()(FresnelLensModel* m);
};

#endif /* FRESNELSIM_INCLUDE_FRESNELPERSISTVISITOR_HH_ */

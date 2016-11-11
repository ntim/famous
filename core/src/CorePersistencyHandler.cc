/*
 * CorePersistencyHandler.cc
 *
 * @date Jul 31, 2014
 * @author Tim Niggemann, III Phys. Inst. A, RWTH Aachen University
 * @copyright GNU General Public License v3.0
 */

#include "CorePersistencyHandler.hh"

CorePersistencyHandler::CorePersistencyHandler(CoreRootPersistVisitor* _visitor) :
		PersistencyHandler(_visitor), visitor(_visitor) {
}

void CorePersistencyHandler::persist(CorePersistable p) {
	boost::apply_visitor(*visitor, p);
}

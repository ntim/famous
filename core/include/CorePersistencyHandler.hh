/*
 * CorePersistencyHandler.hh
 *
 * @date Jul 31, 2014
 * @author Tim Niggemann, III Phys. Inst. A, RWTH Aachen University
 * @copyright GNU General Public License v3.0
 */

#ifndef COREPERSISTENCYHANDLER_HH_
#define COREPERSISTENCYHANDLER_HH_

#include <persistency/PersistencyHandler.hh>

#include "CoreHit.hh"
#include "CoreRootPersistVisitor.hh"
#include "GeneralParticleSourceMessenger.hh"

/**
 * Variant for all supported, persistable types.
 */
typedef boost::variant<G4SipmHitsCollection*, G4SipmDigiCollection*, G4SipmVoltageTraceDigiCollection*,
		ParticleSourceMessenger*, G4SipmModel*, G4SipmVoltageTraceModel*, CoreHitsCollection*, GeneralParticleSourceMessenger*> CorePersistable;

class CorePersistencyHandler: public PersistencyHandler {
private:
	CoreRootPersistVisitor* visitor;

public:
	CorePersistencyHandler(CoreRootPersistVisitor* visitor);

	void persist(CorePersistable p);
};

#endif /* COREPERSISTENCYHANDLER_HH_ */

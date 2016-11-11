/*
 * CoreRootPersistVisitor.hh
 *
 * @date Jul 31, 2014
 * @author Tim Niggemann, III Phys. Inst. A, RWTH Aachen University
 * @copyright GNU General Public License v3.0
 */

#ifndef COREROOTPERSISTVISITOR_HH_
#define COREROOTPERSISTVISITOR_HH_

#include <persistency/PersistencyHandler.hh>
#include <persistency/RootPersistVisitor.hh>

#include <CLHEP/Vector/ThreeVector.h>

#include "CoreHit.hh"
#include "GeneralParticleSourceMessenger.hh"

class TFile;

class CoreRootPersistVisitor: public RootPersistVisitor {
public:
	using RootPersistVisitor::operator ();

	virtual void operator ()(CoreHitsCollection* hc);

	// Delegate properly.
	virtual void operator ()(G4SipmHitsCollection* hc);
	virtual void operator ()(G4SipmDigiCollection* dc);
	virtual void operator ()(G4SipmVoltageTraceDigiCollection* dc);
	virtual void operator ()(ParticleSourceMessenger* m);
	virtual void operator ()(G4SipmModel* m);
	virtual void operator ()(G4SipmVoltageTraceModel* m);
	virtual void operator ()(GeneralParticleSourceMessenger* m);
};

#endif /* COREROOTPERSISTVISITOR_HH_ */

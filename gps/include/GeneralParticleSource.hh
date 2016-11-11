/*
 * GeneralParticleSource.hh
 *
 * @date Aug 7, 2014
 * @author Tim Niggemann, III Phys. Inst. A, RWTH Aachen University
 * @copyright GNU General Public License v3.0
 */

#ifndef GENERALPARTICLESOURCE_HH_
#define GENERALPARTICLESOURCE_HH_

#include <G4VUserPrimaryGeneratorAction.hh>
#include <G4ParticleGun.hh>
#include <G4Event.hh>

class GeneralParticleSource: public G4VUserPrimaryGeneratorAction {
private:
	struct Rand {
		Rand() {
			//
		}
		virtual void operator ()(double&, double&) {
			throw 1;
		}
	};
	struct RandRect: Rand {
		// Pitches.
		double a, b;
		RandRect(double _a, double _b) :
				Rand(), a(_a), b(_b) {
			//
		}
		virtual void operator ()(double& x, double& y);
	};
	struct RandCircle: Rand {
		// Minimal and maximal radius.
		double rMin, rMax;
		RandCircle(double _rMin, double _rMax) :
				Rand(), rMin(_rMin), rMax(_rMax) {
			//
		}
		virtual void operator ()(double& x, double& y);
	};
	struct RandHex: Rand {
		// Outer radius.
		double a;
		RandHex(double _a) :
				Rand(), a(_a) {
			//
		}
		virtual void operator ()(double& x, double& y);
	};

	G4ParticleGun* particleGun;
	G4ThreeVector getPolarizationVector(double angle);

public:
	GeneralParticleSource();

	virtual void GeneratePrimaries(G4Event* anEvent);
};

#endif /* GENERALPARTICLESOURCE_HH_ */

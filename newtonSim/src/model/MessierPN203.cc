/*
 * MessierPN203.cc
 *
 * @date 27.01.2011
 * @author Tim Niggemann, III Phys. Inst. A, RWTH Aachen University
 * @copyright GNU General Public License v3.0
 */

#include "model/MessierPN203.hh"

MessierPN203::MessierPN203(G4ThreeVector _placement, G4RotationMatrix* _rotation) :
		NewtonTelescope("messierPN203", _placement, _rotation, 101.5 * CLHEP::mm, 64.96 / 2. * CLHEP::mm, 800 * CLHEP::mm,
				700. * CLHEP::mm, 50.8 * CLHEP::mm / 2., 135. * CLHEP::mm) {
	setTubusRadius(117.5 * CLHEP::mm);
}

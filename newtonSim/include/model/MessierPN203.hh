/*
 * MessierPN203.hh
 *
 * @date 27.01.2011
 * @author Tim Niggemann, III Phys. Inst. A, RWTH Aachen University
 * @copyright GNU General Public License v3.0
 */

#ifndef MESSIERPN203_HH_
#define MESSIERPN203_HH_

#include "NewtonTelescope.hh"

#include <G4ThreeVector.hh>
#include <G4RotationMatrix.hh>

/**
 * Implementation of the Bresser Messier PN203 Newton reflector.
 */
class MessierPN203: public NewtonTelescope {
public:
	MessierPN203(G4ThreeVector placement, G4RotationMatrix* rotation);
};

#endif /* MESSIERPN203_HH_ */

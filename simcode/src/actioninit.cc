#include "actioninit.hh"

actioninit::actioninit() : G4VUserActionInitialization() {}

actioninit::~actioninit() {}

void actioninit::BuildForMaster() const {
  SetUserAction(new runAction());
}

void actioninit::Build() const {
  SetUserAction(new runAction());
  SetUserAction(new generator());
}

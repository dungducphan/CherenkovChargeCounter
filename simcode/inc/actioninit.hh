#include "G4VUserActionInitialization.hh"

#include "generator.hh"
#include "runaction.hh"

class actioninit : public G4VUserActionInitialization {
  public:
    actioninit();
    virtual ~actioninit();

    virtual void Build() const;
    virtual void BuildForMaster() const;
};

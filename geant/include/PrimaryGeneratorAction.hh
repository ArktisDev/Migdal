#ifndef PRIMARYGENERATORACTION_H
#define PRIMARYGENERATORACTION_H

#include "G4VUserPrimaryGeneratorAction.hh"

#include "G4GeneralParticleSource.hh"
#include "G4ParticleGun.hh"

class PrimaryGeneratorAction : public G4VUserPrimaryGeneratorAction
{
  public:
    PrimaryGeneratorAction();    
    ~PrimaryGeneratorAction() override;
    
    // Method from the base class
    void GeneratePrimaries(G4Event* event) override;         
  
    // Method to access particle gun
    const G4GeneralParticleSource* GetParticleSource() const { return particleSource; }
  
  private:
    G4GeneralParticleSource* particleSource;
};

#endif // PRIMARYGENERATORACTION_H

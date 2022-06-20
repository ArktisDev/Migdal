#include "PrimaryGeneratorAction.hh"
#include "G4SystemOfUnits.hh"
#include "CLHEP/Random/RandGauss.h"
#include "CLHEP/Random/RandExponential.h"

#include <iostream>

PrimaryGeneratorAction::PrimaryGeneratorAction()
{
    particleSource = new G4GeneralParticleSource;
}

PrimaryGeneratorAction::~PrimaryGeneratorAction()
{
    
}

void PrimaryGeneratorAction::GeneratePrimaries(G4Event* event)
{
    particleSource->GeneratePrimaryVertex(event);
}
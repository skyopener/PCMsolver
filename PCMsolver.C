/*---------------------------------------------------------------------------*\
  =========                 |
  \\      /  F ield         | OpenFOAM: The Open Source CFD Toolbox
   \\    /   O peration     |
    \\  /    A nd           | Copyright (C) 2011-2013 OpenFOAM Foundation
     \\/     M anipulation  |
-------------------------------------------------------------------------------
License
    This file is part of OpenFOAM.

    OpenFOAM is free software: you can redistribute it and/or modify it
    under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    OpenFOAM is distributed in the hope that it will be useful, but WITHOUT
    ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
    FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License
    for more details.

    You should have received a copy of the GNU General Public License
    along with OpenFOAM.  If not, see <http://www.gnu.org/licenses/>.

Application
    buoyantBoussinesqPimpleFoam

Description
    Transient solver for buoyant, turbulent flow of incompressible fluids

    Uses the Boussinesq approximation:
    \f[
        rho_{k} = 1 - beta(T - T_{ref})
    \f]

    where:
        \f$ rho_{k} \f$ = the effective (driving) kinematic density
        beta = thermal expansion coefficient [1/K]
        T = temperature [K]
        \f$ T_{ref} \f$ = reference temperature [K]

    Valid when:
    \f[
        \frac{beta(T - T_{ref})}{rho_{ref}} << 1
    \f]

\*---------------------------------------------------------------------------*/
#include "OFstream.H"
#include <fstream>
#include <sstream>
#include <string>
#include "fvCFD.H"
#include "pimpleControl.H"
#include "fixedFluxPressureFvPatchScalarField.H"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

int main(int argc, char *argv[])
{
    #include "setRootCase.H"
    #include "createTime.H"
    #include "createMesh.H"
    #include "readGravitationalAcceleration.H"
    #include "createFields.H"

pimpleControl pimple(mesh);

    #include "initContinuityErrs.H"
    #include "readMyControls.H"
    #include "CourantNo.H"
    #include "setInitialDeltaT.H"

    

    // * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

    Info<< "\nStarting time loop\n" << endl;
    #include "createMyFiles.H"
    
    while (runTime.loop())
    {
        Info<< "Time = " << runTime.timeName() << nl << endl;
	    #include "readMyControls.H"
        #include "CourantNo.H"
        #include "setDeltaT.H"
	if(onlyConduction==1){
        #include "TEqn_onlyconduction.H"
    }
    else{
        // --- Pressure-velocity PIMPLE corrector loop
        while (pimple.loop())
        {
            #include "UEqn.H"
            // --- Pressure corrector loop
            while (pimple.correct())
            {
                #include "pEqn.H"
            }
            #include "TEqn.H"
        }
	}   
	gradT=fvc::grad(T);
    runTime.write();
    #include "writeMyFiles.H"

    Info<< "Liquid fraction [-] = " << liquidFraction<< endl;
    Info<< "ExecutionTime = " << runTime.elapsedCpuTime() << " s"
            << "  ClockTime = " << runTime.elapsedClockTime() << " s"
            << nl << endl;
    }

    Info<< "End\n" << endl;

    return 0;
}


// ************************************************************************* //



    IOdictionary transportProperties
    (
        IOobject
        (
            "transportProperties",
            runTime.constant(),
            mesh,
            IOobject::MUST_READ,
            IOobject::NO_WRITE
        )
    );


    dimensionedScalar kappaSolid(transportProperties.lookup("kappaSolid"));
    dimensionedScalar kappaLiquid(transportProperties.lookup("kappaLiquid"));

    dimensionedScalar beta(transportProperties.lookup("beta"));
    // Reference temperature [K]
    dimensionedScalar Tref(transportProperties.lookup("Tref"));


    dimensionedScalar L(transportProperties.lookup("L"));


    dimensionedScalar rho(transportProperties.lookup("rho"));

    dimensionedScalar darcyTermEpsilon(transportProperties.lookup("darcyTermEpsilon"));
    dimensionedScalar darcyTermConstant(transportProperties.lookup("darcyTermConstant"));



    dimensionedScalar cpSolid(transportProperties.lookup("cpSolid"));
    dimensionedScalar cpLiquid(transportProperties.lookup("cpLiquid"));

    dimensionedScalar TSolid(transportProperties.lookup("TSolid"));
    dimensionedScalar TLiquid(transportProperties.lookup("TLiquid"));

    dimensionedScalar nu(transportProperties.lookup("nu"));



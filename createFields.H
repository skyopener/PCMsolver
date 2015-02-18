    Info<< "Reading thermophysical properties\n" << endl;

    Info<< "Reading field T\n" << endl;
    volScalarField T
    (
        IOobject
        (
            "T",
            runTime.timeName(),
            mesh,
            IOobject::MUST_READ,
            IOobject::AUTO_WRITE
        ),
        mesh
    );

    Info<< "Reading field p_rgh\n" << endl;
    volScalarField p_rgh
    (
        IOobject
        (
            "p_rgh",
            runTime.timeName(),
            mesh,
            IOobject::MUST_READ,
            IOobject::AUTO_WRITE
        ),
        mesh
    );
    volVectorField gradT
    (
        IOobject
        (
            "gradT",
            runTime.timeName(),
            mesh,
            IOobject::NO_READ,
            IOobject::AUTO_WRITE
        ),
        fvc::grad(T)
    );
    gradT.write();

    Info<< "Reading field U\n" << endl;
    volVectorField U
    (
        IOobject
        (
            "U",
            runTime.timeName(),
            mesh,
            IOobject::MUST_READ,
            IOobject::AUTO_WRITE
        ),
        mesh
    );

    #include "createPhi.H"

    #include "readTransportProperties.H"

    Info<< "Creating field darcyTermLiquidFraction\n" << endl;
    volScalarField darcyTermLiquidFraction
    (
	IOobject
	(
		"darcyTermLiquidFraction",
		runTime.timeName(),
		mesh,
		IOobject::NO_READ,
		IOobject::AUTO_WRITE
	),
	max(   min( (T-TSolid)/(TLiquid-TSolid),scalar(1) )  ,  scalar(0)   )
    );
    darcyTermLiquidFraction.write();

    Info<< "Creating field darcyTerm\n" << endl;
    volScalarField darcyTerm
    (
	IOobject
	(
		"darcyTerm",
		runTime.timeName(),
		mesh
	),
	darcyTermConstant*(pow(scalar(1)-darcyTermLiquidFraction,2))/(pow(darcyTermLiquidFraction,3)+darcyTermEpsilon)
    );


    // Kinematic density for buoyancy force
    Info<< "Creating field bouyancy term\n" << endl;
    volScalarField rhok
    (
        IOobject
        (
            "rhok",
            runTime.timeName(),
            mesh
        ),
        1.0 - beta*(T - Tref)
    );

    Info<< "Creating cp \n" << endl;
    volScalarField cp
    (
    	IOobject
    	(
    	    "cp",
    	    runTime.timeName(),
    	    mesh
    	),
    	cpLiquid*darcyTermLiquidFraction+cpSolid*(1-darcyTermLiquidFraction)
    );
    

    Info<< "Creating kappa \n" << endl;
    volScalarField kappa
    (
	IOobject
	(
	    "kappa",
	    runTime.timeName(),
	    mesh
	),
	kappaLiquid*darcyTermLiquidFraction+kappaSolid*(1-darcyTermLiquidFraction)
    );



    Info<< "Creating phicp term\n" << endl;
    surfaceScalarField phicp
    (
	IOobject
	(
   	    "phicp",
	     runTime.timeName(),
	     mesh
	),
	phi*fvc::interpolate(cp)
    );
    



    Info<< "Calculating field g.h\n" << endl;
    volScalarField gh("gh", g & mesh.C());
    surfaceScalarField ghf("ghf", g & mesh.Cf());

    volScalarField p
    (
        IOobject
        (
            "p",
            runTime.timeName(),
            mesh,
            IOobject::NO_READ,
            IOobject::AUTO_WRITE
        ),
        p_rgh + rhok*gh
    );

    label pRefCell = 0;
    scalar pRefValue = 0.0;
    setRefCell
    (
        p,
        p_rgh,
        mesh.solutionDict().subDict("PIMPLE"),
        pRefCell,
        pRefValue
    );

    if (p_rgh.needReference())
    {
        p += dimensionedScalar
        (
            "p",
            p.dimensions(),
            pRefValue - getRefCellValue(p, pRefCell)
        );
    }

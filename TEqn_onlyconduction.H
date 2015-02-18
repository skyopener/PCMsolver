    scalar residual=scalar(10);
    int iteration=0;
    scalar meanResidual=scalar(10);
    dimensionedScalar mydeltaT1("mydeltaT",dimensionSet(0, 0, 1, 0, 0, 0, 0),scalar(runTime.deltaTValue()));


while(iteration<=maxLiquidFractionIteration&&residual>=maxLiquidFractionResidual){

    
    volScalarField darcyTermLiquidFractionOldIteration=darcyTermLiquidFraction;
    volScalarField dFdT= (1/(TLiquid-TSolid))*pos(T-TSolid)*pos(TLiquid-T);
    volScalarField Finverse=darcyTermLiquidFraction*(TLiquid-TSolid)+TSolid;
    volScalarField ST=-((cpLiquid-cpSolid)*(T)+L)*dFdT/mydeltaT1;
    volScalarField SC=-ST*Finverse+((cpLiquid-cpSolid)*(T)+L)*(darcyTermLiquidFraction.oldTime()-darcyTermLiquidFraction)/mydeltaT1;

    fvScalarMatrix TEqn
    (
        fvm::ddt(cp,T)-
        fvm::laplacian(kappa/rho,T)-
        fvm::Sp(ST,T)
        ==SC
    );
    TEqn.solve();


    darcyTermLiquidFraction=darcyTermLiquidFraction+darcyTermLiquidFractionRelaxFactor*dFdT*(T-Finverse);
    darcyTermLiquidFraction=max(    min(darcyTermLiquidFraction,scalar(1))   ,scalar(0));
    Info<<"ValueTAverage: "<<T.weightedAverage(mesh.V()).value()<<endl;
    cp=cpLiquid*darcyTermLiquidFraction+cpSolid*(scalar(1)-darcyTermLiquidFraction);
    kappa=kappaLiquid*darcyTermLiquidFraction+kappaSolid*(scalar(1)-darcyTermLiquidFraction);

    residual=gMax (  mag( darcyTermLiquidFraction.internalField()-darcyTermLiquidFractionOldIteration.internalField() )    );
    meanResidual=gSum(   mag( darcyTermLiquidFraction.internalField()-darcyTermLiquidFractionOldIteration.internalField() ) * mesh.V()   )/gSum(mesh.V().field());
    iteration++;
}
    liquidFraction=darcyTermLiquidFraction.weightedAverage(mesh.V()).value();
    Info<<"TEqn residual: "<<residual<< " meanResidual: "<<meanResidual<<"\n"<<endl;
    darcyTerm=darcyTermConstant*(pow(scalar(1)-darcyTermLiquidFraction,2))/(pow(darcyTermLiquidFraction,3)+darcyTermEpsilon);	
    rhok = 1.0 - beta*(T - Tref);
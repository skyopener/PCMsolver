

if((scalar(runTime.value()))>=n*secondsToWriteMyData){
	Info<<"myInfo: "<<runTime.value()<<endl;
	Info<<"myInfo2: "<<myTime;
    if(writeUIntegral){
        scalar sumU=fvc::domainIntegrate(mag(U)).value();
        Info<<" Integral U: " << sumU << "  Time: " << runTime.timeName() << endl;
        *myStreamPtr << sumU  <<" "<< runTime.timeName() << endl;
        }
    if(writeLiquidFraction){
        *myStreamPtrLiquid << liquidFraction  <<" "<< runTime.timeName() << endl;
    }
    //myTime=scalar(runTime.value());
    n++;
    myTime=mesh.time().value();
}
 #include "readTimeControls.H"

int maxLiquidFractionIteration(readScalar(pimple.dict().lookup("maxLiquidFractionIteration")));
scalar maxLiquidFractionResidual(readScalar(pimple.dict().lookup("maxLiquidFractionResidual")));
scalar darcyTermLiquidFractionRelaxFactor(readScalar(pimple.dict().lookup("darcyTermLiquidFractionRelaxFactor")));
scalar liquidFraction;

int writeLiquidFraction(readScalar(pimple.dict().lookup("writeLiquidFraction")));
int writeUIntegral(readScalar(pimple.dict().lookup("writeUIntegral")));
int secondsToWriteMyData(readScalar(pimple.dict().lookup("secondsToWriteMyData")));
int onlyConduction(readScalar(pimple.dict().lookup("onlyConduction")));
scalar myTime=0;
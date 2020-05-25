// Read gravitational acceleration
Info<< "\nReading g" << endl;
uniformDimensionedVectorField g
(
    IOobject
    (
        "g",
        runTime.constant(),
        mesh,
        IOobject::READ_IF_PRESENT,
        IOobject::NO_WRITE
    ),
    dimensionedVector(dimAcceleration, Zero)
);

Info<< "Reading phaseProperties dictionary\n" << endl;
IOdictionary phaseProperties
(
     IOobject
     (
          "phaseProperties",
          runTime.constant(),
          mesh,
          IOobject::MUST_READ,
          IOobject::NO_WRITE
     )
);

coupledMultiphaseCompressibleSystem fluid(mesh, phaseProperties);
autoPtr<timeIntegrator> integrator(timeIntegrator::New(mesh));
integrator->addSystem(fluid);

basicThermoCloud parcels
(
    "reactingCloud1",
    fluid.rho(),
    fluid.U(),
    g,
    mesh.lookupObject<fluidThermoModel>("fluidThermo")
);

fluid.volumeFraction() = 1.0 - parcels.theta();


const volScalarField& p = fluid.p();
const volScalarField& T = fluid.T();
const surfaceScalarField& phi = fluid.phi();
fluid.update();

autoPtr<errorEstimator> error;
if (!isA<staticFvMesh>(mesh))
{
    error =
        errorEstimator::New
        (
            mesh,
            IOdictionary
            (
                IOobject
                (
                    "dynamicMeshDict",
                    runTime.constant(),
                    mesh,
                    IOobject::MUST_READ,
                    IOobject::NO_WRITE
                )
            )
        );
}
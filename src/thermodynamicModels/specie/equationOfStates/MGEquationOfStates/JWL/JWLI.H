/*---------------------------------------------------------------------------*\
  =========                 |
  \\      /  F ield         | OpenFOAM: The Open Source CFD Toolbox
   \\    /   O peration     | Website:  https://openfoam.org
    \\  /    A nd           | Copyright (C) 2011-2020 OpenFOAM Foundation
     \\/     M anipulation  |
-------------------------------------------------------------------------------
2020-04-02 Jeff Heylmun:    Modified class for a density based thermodynamic
                            class
-------------------------------------------------------------------------------
License
    This file is derivative work of OpenFOAM.

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

\*---------------------------------------------------------------------------*/

// * * * * * * * * * * * * * * * Member Functions  * * * * * * * * * * * * * //


template<class Specie>
Foam::scalar Foam::JWL<Specie>::Gamma
(
    const scalar& rho,
    const scalar& e
) const
{
    if (rho < rhoCutOff_)
    {
        return gammaIdeal_;
    }

    return omega_ + 1.0;
}


template<class Specie>
Foam::scalar Foam::JWL<Specie>::Pi
(
    const scalar& rho,
    const scalar& e
) const
{
    if (rho < rhoCutOff_)
    {
        return 0.0;
    }

    scalar V(rho0_/max(rho, 1e-10));
    return
        A_*(omega_/(R1_*V) - 1.0)*exp(-R1_*V)
      + B_*(omega_/(R2_*V) - 1.0)*exp(-R2_*V)
      - omega_*rho*e0_;

}


template<class Specie>
Foam::scalar Foam::JWL<Specie>::delta
(
    const scalar& p,
    const scalar& rho,
    const scalar& e
) const
{
    if (rho < rhoCutOff_)
    {
        return 0.0;
    }

    scalar rhos(max(rho, 1e-10));
    return
        (
            A_*exp(-R1_*rho0_/rhos)
           *(omega_*(1.0/(R1_*rho0_) + 1.0/rhos) - R1_*rho0_/sqr(rhos))
          + B_*exp(-R2_*rho0_/rhos)
           *(omega_*(1.0/(R2_*rho0_) + 1.0/rhos) - R2_*rho0_/sqr(rhos))
          - omega_*e0_
        )/omega_;
}


template<class Specie>
Foam::scalar Foam::JWL<Specie>::dpdv
(
    const scalar& rho,
    const scalar& e
) const
{
    if (rho < rhoCutOff_)
    {
        return (gammaIdeal_ - 1.0)*e*sqr(rho);
    }

    scalar rhos(max(rho, 1e-10));
    return
      - (
            A_*exp(-R1_*rho0_/rhos)
           *(R1_*rho0_ - omega_*rho*(1.0 + rho/R1_/rho0_))
          + B_*exp(-R2_*rho0_/rhos)
           *(R2_*rho0_ - omega_*rho*(1.0 + rho/R2_/rho0_))
          + omega_*(e + e0_)*sqr(rhos)
        );
}


template<class Specie>
Foam::scalar Foam::JWL<Specie>::dpde
(
    const scalar& rho,
    const scalar& e
) const
{
    if (rho < rhoCutOff_)
    {
        return (gammaIdeal_ - 1.0)*rho;
    }
    return omega_*rho;
}


template<class Specie>
Foam::scalar Foam::JWL<Specie>::E
(
    const scalar& rho,
    const scalar& e
) const
{
    if (rho < rhoCutOff_)
    {
        return 0.0;
    }

    scalar rhos = max(rho, 1e-10);
    scalar V(rho0_/rhos);
    return (A_/R1_*exp(-R1_*V) + B_/R2_*exp(-R2_*V))/rho0_ - e0_;
}


template<class Specie>
Foam::scalar Foam::JWL<Specie>::Cv
(
    const scalar& rho,
    const scalar& e
) const
{
    return 0;
}


template<class Specie>
Foam::scalar Foam::JWL<Specie>::Cp
(
    const scalar& rho,
    const scalar& e
) const
{
    return 0;
}


template<class Specie>
Foam::scalar Foam::JWL<Specie>::H
(
    const scalar& rho,
    const scalar& e
) const
{
    if (rho < rhoCutOff_)
    {
        return 0.0;
    }

    scalar rhos(max(rho, 1e-10));
    return
        A_*(1.0/rhos + 1.0/(R1_*rho0_))*exp(-R1_*rho0_/rhos)
      + B_*(1.0/rhos + 1.0/(R2_*rho0_))*exp(-R2_*rho0_/rhos)
      + e0_*omega_*log(rhos);
}

template<class Specie>
Foam::scalar Foam::JWL<Specie>::CpMCv
(
    const scalar& rho,
    const scalar& e
) const
{
    return this->R();
}


template<class Specie>
Foam::scalar Foam::JWL<Specie>::S
(
    const scalar& p,
    const scalar& rho,
    const scalar& T
) const
{
    return -this->R()*log(p/Foam::constant::thermodynamic::Pstd);
}

// ************************************************************************* //

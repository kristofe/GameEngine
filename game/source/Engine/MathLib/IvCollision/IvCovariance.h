//===============================================================================
// @ IvCovariance.h
// 
// Helper routines for computing properties of covariance matrices
// ------------------------------------------------------------------------------
// Copyright (C) 2004 by Elsevier, Inc. All rights reserved.
//
//
//
//===============================================================================

#ifndef __IvCovariance__h__
#define __IvCovariance__h__

//-------------------------------------------------------------------------------
//-- Dependencies ---------------------------------------------------------------
//-------------------------------------------------------------------------------

//-------------------------------------------------------------------------------
//-- Typedefs, Structs ----------------------------------------------------------
//-------------------------------------------------------------------------------

class IvVector3;
class IvMatrix33;

//-------------------------------------------------------------------------------
//-- Functions ------------------------------------------------------------------
//-------------------------------------------------------------------------------

void IvComputeCovarianceMatrix( IvMatrix33& C, IvVector3& mean,
                              const IvVector3* points, unsigned int numPoints );
void IvGetRealSymmetricEigenvectors( IvVector3& v1, IvVector3& v2, IvVector3& v3, 
                                   const IvMatrix33& A );

//-------------------------------------------------------------------------------
//-- Inlines --------------------------------------------------------------------
//-------------------------------------------------------------------------------

//-------------------------------------------------------------------------------
//-- Externs --------------------------------------------------------------------
//-------------------------------------------------------------------------------

#endif

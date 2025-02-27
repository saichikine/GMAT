//$Id$
//------------------------------------------------------------------------------
//                             SchurFactorization
//------------------------------------------------------------------------------
// GMAT: General Mission Analysis Tool
//
// Copyright (c) 2002 - 2018 United States Government as represented by the
// Administrator of the National Aeronautics and Space Administration.
// All Other Rights Reserved.
//
// Licensed under the Apache License, Version 2.0 (the "License"); 
// You may not use this file except in compliance with the License. 
// You may obtain a copy of the License at:
// http://www.apache.org/licenses/LICENSE-2.0. 
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either 
// express or implied.   See the License for the specific language
// governing permissions and limitations under the License.
//
// Developed jointly by NASA/GSFC and Thinking Systems, Inc. under the FDSS II
// contract, Task Order 08.
//
// Author: Joshua Raymond
// Created: 2017/07/27
//
/**
* Declares SchurFactorization class.
*/
//------------------------------------------------------------------------------

#include "SchurFactorization.hpp"
#include "QRFactorization.hpp"
#include "RealUtilities.hpp"
#include "UtilityException.hpp"
#include <iostream>

//------------------------------------------------------------------------------
// SchurFactorization()
//------------------------------------------------------------------------------
/**
* Constructor
*/
//------------------------------------------------------------------------------
SchurFactorization::SchurFactorization()
{
}

//------------------------------------------------------------------------------
// SchurFactorization(const SchurFactorization &schurfactorization)
//------------------------------------------------------------------------------
/**
* Copy Constructor
*/
//------------------------------------------------------------------------------
SchurFactorization::SchurFactorization(const SchurFactorization
                                       &schurfactorization)
{
}

//------------------------------------------------------------------------------
// ~SchurFactorization()
//------------------------------------------------------------------------------
/**
* Destructor
*/
//------------------------------------------------------------------------------
SchurFactorization::~SchurFactorization()
{
}

//------------------------------------------------------------------------------
// SchurFactorization& operator=(const SchurFactorization &schurfactorization)
//------------------------------------------------------------------------------
/**
* Assigment operator
*/
//------------------------------------------------------------------------------
SchurFactorization& SchurFactorization::operator=(const SchurFactorization
                                                  &schurfactorization)
{
   if (this != &schurfactorization)
   {
      //Call the base class method
      MatrixFactorization::operator=(schurfactorization);
   }

   return *this;
}

//------------------------------------------------------------------------------
// void Factor(const Rmatrix inputMatrix, Rmatrix &sdUnitary, Rmatrix &sdUpper)
//------------------------------------------------------------------------------
/**
* Matrix factorization routine using the Schur identity
*
* Factorization is completed by using QR factorization:
*     A->Q,R
*     A = R*Q
* This pattern is used until a tolerance is reached.  A will become sdUpper
* through this process and sdUnitary will come from multiplying all Q values
* calculated together.  Original equation by Padraic Bartlett at the end
* of the document "Lecture 5: The Schur Decomposition".
*
* @param inputMatrix The matrix to factor
* @param &sdUnitary Unitary matrix Q where Q'AQ = U
* @param &sdUpper Upper triangular matrix U
*
* @return 0 on success, anything else indicates a problem
*/
//------------------------------------------------------------------------------
void SchurFactorization::Factor(const Rmatrix inputMatrix, Rmatrix &sdUnitary,
                                Rmatrix &sdUpper)
{

   if (inputMatrix.GetNumRows() != inputMatrix.GetNumColumns())
   {
      std::string errMessage = 
         "Matrix must be square to use Schur factorization. \n";
      throw UtilityException(errMessage);
   }

   Rmatrix A = inputMatrix;
   Rmatrix Aold = A;
   Rmatrix sdUnitaryOld = sdUnitary;
   Rmatrix qrOrthog(A.GetNumRows(), A.GetNumRows());
   Rmatrix qrUpper(A.GetNumRows(), A.GetNumRows());
   Rmatrix qrOrthogOld(A.GetNumRows(), A.GetNumRows());
   Rmatrix qrUpperOld(A.GetNumRows(), A.GetNumRows());
   Rmatrix zeroMatrix(A.GetNumRows(), A.GetNumRows());

   Real tol = 0.000001;
   Real diff1 = 100;
   Real diff2 = 100;

   for (Integer i = 0; i < A.GetNumRows(); i++)
      sdUnitary(i, i) = 1;

   QRFactorization *qr = new QRFactorization();
   qr->Factor(A, qrUpper, qrOrthog);
   A = qrUpper * qrOrthog;
   sdUnitary = sdUnitary * qrOrthog;

   while ((diff1 > tol) || (diff2 > tol))
   {
      A = qrUpper * qrOrthog;
      qrUpperOld = qrUpper;
      qrOrthogOld = qrOrthog;
      qrUpper = zeroMatrix;
      qrOrthog = zeroMatrix;
      qr->Factor(A, qrUpper, qrOrthog);

      diff1 = 0;
      diff2 = 0;


      for (Integer i = 0; i < A.GetNumRows(); i++)
      {
         for (Integer j = 0; j < A.GetNumRows(); j++)
         {
            if (std::abs(A(i, j) - Aold(i, j)) > diff1)
               diff1 = std::abs(A(i, j) - Aold(i, j));
            if (std::abs(sdUnitary(i, j) - sdUnitaryOld(i, j)) > diff2)
               diff2 = std::abs(sdUnitary(i, j) - sdUnitaryOld(i, j));
         }
      }
      Aold = A;
      sdUnitaryOld = sdUnitary;
      sdUnitary = sdUnitary * qrOrthog;
   }

   sdUpper = A;
}

//------------------------------------------------------------------------------
// void Invert(Rmatrix &inputMatrix)
//------------------------------------------------------------------------------
/**
* Matrix inversion routine using the Schur identity
*
* This method is a port of the inversion code from GTDS, as ported by Angel
* Wang of Thinking Systems and then integrated into GMAT by D. Conway.
*
* @param inputMatrix The matrix to be inverted
*
* @return 0 on success, anything else indicates a problem
*/
//------------------------------------------------------------------------------
void SchurFactorization::Invert(Rmatrix &inputMatrix)
{
   Integer rowCount = inputMatrix.GetNumRows();
   Integer array_size = (Integer)(pow((2 * rowCount + 1), 2) - 1) / 8;
   Real* sum1 = new Real[array_size];
   Integer index = 0;
   for (Integer i = 0; i < inputMatrix.GetNumRows(); i++)
   {
      for (Integer j = i; j < inputMatrix.GetNumColumns(); j++)
      {
         sum1[index] = inputMatrix(i, j);
         ++index;
      }
   }

#ifdef DEBUG_SCHUR
   MessageInterface::ShowMessage("Performing Schur inversion\n   ");
   MessageInterface::ShowMessage("array_size = %d\n", array_size);
   MessageInterface::ShowMessage("Packed array:   [");
   for (UnsignedInt i = 0; i < array_size; ++i)
   {
      if (i > 0)
         MessageInterface::ShowMessage(", ");
      MessageInterface::ShowMessage("%.15le", sum1[i]);
   }
   MessageInterface::ShowMessage("]\n");
#endif

   Integer retval = -1;

   // Check to see if the upper left element is invertible
   if ((array_size > 0) && (sum1[0] != 0.0))
   {
      Real *delta = new Real[array_size];
      Integer ij = 0, now = ij + 1;
      sum1[0] = 1.0 / sum1[0];
      if (rowCount > 1)
      {
         Integer i, i1, j, j1, jl, jn, l, l1, lPlus1, n, nn, nMinus1;
         Integer rowCountMinus1 = rowCount - 1;

         // Recursively invert the n X n matrix knowing the inverse of the
         // (n-1) X (n-1) matrix until the inverted matrix is found
         for (n = 2; n <= rowCount; ++n)
         {
            nMinus1 = n - 1;
            l1 = 0;

            // Compute delta working arrays
            for (l = 1; l <= nMinus1; ++l)
            {
               j1 = 0;
               delta[l - 1] = 0.0;

               for (j = 1; j <= l; ++j)
               {
                  jl = j1 + l - 1;
                  jn = j1 + n - 1;
                  delta[l - 1] = delta[l - 1] + (sum1[jl] * sum1[jn]);
                  j1 += rowCount - j;
               }

               if (l != nMinus1)
               {
                  lPlus1 = l + 1;

                  for (j = lPlus1; j <= nMinus1; ++j)
                  {
                     jn = j1 + n - 1;
                     jl = l1 + j - 1;
                     delta[l - 1] += (sum1[jl] * sum1[jn]);
                     j1 += rowCount - j;
                  }
                  l1 += rowCount - l;
               }
            }
            j1 = n;
            nn = rowCountMinus1 + n;


            // Compute W
            for (j = 1; j <= nMinus1; ++j)
            {
               sum1[nn - 1] -= (delta[j - 1] * sum1[j1 - 1]);
               j1 += rowCount - j;
            }

            // Check if observation is '0'; if so, throw an exception
            now = n + ij;
            if (now > rowCount)
            if (ij != 0)
               break;

            if (sum1[nn - 1] == 0.0)
               continue;

            sum1[nn - 1] = 1.0 / sum1[nn - 1];
            j1 = n;

            // Compute Y
            for (j = 1; j <= nMinus1; ++j)
            {
               // Calculate [H12];   GTDS MatSpec  Eq 8-162b
               sum1[j1 - 1] = -delta[j - 1] * sum1[nn - 1];
               j1 += rowCount - j;
            }

            // Compute X
            i1 = n;
            for (i = 1; i <= nMinus1; ++i)
            {
               j1 = i;
               for (j = 1; j <= i; ++j)
               {
                  // Calculate [H22];   GTDS MatSpec Eq
                  sum1[j1 - 1] -= (sum1[i1 - 1] * delta[j - 1]);
                  j1 += rowCount - j;
               }
               i1 += rowCount - i;
            }
            rowCountMinus1 += rowCount - n;
         }
      }
      delete[] delta;
      retval = 0;
   }
   else
   {
      if (array_size == 0)
         throw UtilityException("Schur inversion cannot proceed; the size of "
         "the array being inverted is zero");

      if (sum1[0] == 0.0)
         throw UtilityException("Schur inversion cannot proceed; the upper "
         "left element of the array being inverted is zero");
   }

   index = 0;
   for (Integer i = 0; i < inputMatrix.GetNumRows(); i++)
   {
      for (Integer j = i; j < inputMatrix.GetNumColumns(); j++)
      {
         inputMatrix(i, j) = sum1[index];
         ++index;
      }
      
      for (Integer j = 0; j < i; j++)
         inputMatrix(i, j) = inputMatrix(j, i);
   }

   return;
}

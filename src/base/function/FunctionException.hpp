//$Id$
//------------------------------------------------------------------------------
//                               FunctionException
//------------------------------------------------------------------------------
// GMAT: General Mission Analysis Tool.
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
// Author: Allison Greene
// Created: 2004/9/22
//
// Developed jointly by NASA/GSFC and Thinking Systems, Inc. under contract
// number NNG04CC06P.
//
/**
 * Exception class used by the Function hierarchy.
 */
//------------------------------------------------------------------------------
#ifndef FUNCTIONEXCEPTION_H
#define FUNCTIONEXCEPTION_H

#include "gmatdefs.hpp"
#include "BaseException.hpp"

class GMAT_API FunctionException : public BaseException
{
public:
   FunctionException(std::string details = "");
   virtual ~FunctionException();
   FunctionException(const FunctionException &be);
};

#endif // FUNCTIONEXCEPTION_H

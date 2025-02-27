//$Id$
//------------------------------------------------------------------------------
//                            YukonOptimizerFactory
//------------------------------------------------------------------------------
// GMAT: General Mission Analysis Tool
//
// Copyright (c) 2002 - 2018 United States Government as represented by the
// Administrator of The National Aeronautics and Space Administration.
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
// contract, Task 08
//
// Author: Joshua Raymond, Thinking Systems
// Created: 2017/10/23
//
/**
 *  Declaration code for the YukonOptimizerFactory class.
 */
//------------------------------------------------------------------------------

#ifndef YukonOptimizerFactory_hpp
#define YukonOptimizerFactory_hpp


#include "yukon_defs.hpp"
#include "Factory.hpp"
// #include "Solver.hpp"

class YUKON_API YukonOptimizerFactory : public Factory
{
public:
   virtual GmatBase* CreateObject(const std::string &ofType,
                                  const std::string &withName);
   virtual Solver* CreateSolver(const std::string &ofType,
                                const std::string &withName);
   
   // default constructor
   YukonOptimizerFactory();
   // constructor
   YukonOptimizerFactory(StringArray createList);
   // copy constructor
   YukonOptimizerFactory(const YukonOptimizerFactory& fact);
   // assignment operator
   YukonOptimizerFactory& operator=(const YukonOptimizerFactory& fact);
   
   virtual ~YukonOptimizerFactory();

   virtual bool DoesObjectTypeMatchSubtype(const std::string &theType,
      const std::string &theSubtype);
};

#endif // YukonOptimizerFactory_hpp

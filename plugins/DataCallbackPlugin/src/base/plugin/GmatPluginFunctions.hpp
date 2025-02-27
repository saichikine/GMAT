//$Id: GmatPluginFunctions.hpp 9460 2011-04-21 22:03:28Z ravimathur $
//------------------------------------------------------------------------------
//                            GmatPluginFunctions
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
// Developed jointly by NASA/GSFC and Thinking Systems, Inc. under contract
// number NNG06CA54C
//
// Author: Ravi Mathur, Emergent Space Technologies, Inc.
// Created: December 16, 2014
//
/**
 * Definition for library code interfaces.
 * 
 * This is prototype code.
 */
//------------------------------------------------------------------------------
#ifndef GmatPluginFunctions_hpp
#define GmatPluginFunctions_hpp

#include "datacallback_defs.hpp"

class Factory;
class MessageReceiver;

extern "C"
{
   Integer    DATACALLBACK_API GetFactoryCount();
   Factory    DATACALLBACK_API *GetFactoryPointer(Integer index);
   void       DATACALLBACK_API SetMessageReceiver(MessageReceiver* mr);

   // Set the user-specified callback function
   int        DATACALLBACK_API SetCallback(const char* subscriberName, void (*CBFcn)(const double*, int, void*), void* userData);

   // Get last message from a CInterface function
   const char DATACALLBACK_API *getLastMessage();
};


#endif /*GmatPluginFunctions_hpp*/

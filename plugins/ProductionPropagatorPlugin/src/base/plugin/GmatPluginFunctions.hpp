//$Id: GmatPluginFunctions.hpp,v 1.1 2008/07/03 19:15:33 djc Exp $
//------------------------------------------------------------------------------
//                            GmatPluginFunctions
//------------------------------------------------------------------------------
// GMAT: General Mission Analysis Tool
//
// Copyright (c) 2002 - 2018 United States Government as represented by the
// Administrator of the National Aeronautics and Space Administration.
// All Other Rights Reserved.
//
// Developed jointly by NASA/GSFC and Thinking Systems, Inc. under contract
// number NNG06CA54C
//
// Author: Darrel J. Conway, Thinking Systems, Inc.
// Created: Nov 6, 2013
//
/**
 * Definition for library code interfaces.
 * 
 * This is sample code demonstrating GMAT's plug-in capabilities.
 */
//------------------------------------------------------------------------------

#ifndef GMATPLUGINFUNCTIONS_H_
#define GMATPLUGINFUNCTIONS_H_

#include "ProductionPropagatorDefs.hpp"
#include "Factory.hpp"

class MessageReceiver;


/**
 * This code defines the C interfaces GMAT uses to load a plug-in library.
 */
extern "C"
{
   Integer PRODUCTIONPROPAGATOR_API GetFactoryCount();
   Factory PRODUCTIONPROPAGATOR_API *GetFactoryPointer(Integer index);
   void    PRODUCTIONPROPAGATOR_API SetMessageReceiver(MessageReceiver* mr);
};


#endif /*GMATPLUGINFUNCTIONS_H_*/

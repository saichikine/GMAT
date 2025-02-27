# $Id$
# 
# GMAT: General Mission Analysis Tool.
# 
# CMAKE script file for GMAT Estimation Plugin.
# (Adapted from the CInterface plugin CMake script)
# This file must be installed in the plugins/EstimationPlugin/src directory
#
# Author: Ravi Mathur
#  
# DO NOT MODIFY THIS FILE UNLESS YOU KNOW WHAT YOU ARE DOING!
#

MESSAGE("==============================")
MESSAGE("GMAT Estimation Plugin setup " ${VERSION}) 

# ====================================================================
# Major error checks
if(NOT EXISTS ${F2C_DIR})
  MESSAGE(WARNING "Skipping due to missing F2C")
  RETURN()
endif()

if(NOT XercesC_FOUND)
  MESSAGE(WARNING "Skipping Estimation: Xerces missing")
  RETURN()
endif()

SET(TargetName GmatEstimation)

# Add Preproccessor Definitions
ADD_DEFINITIONS("-DESTIMATION_EXPORTS")
# ADD_DEFINITIONS("-DENABLE_UNFINISHED_FEATURES")

# ====================================================================
# list of directories containing source/header files
SET(PLUGIN_DIRS
  adapter
  command
  datafilter
  errormodel
  estimator
  event
  factory
  hardware
  include
  measurement
  measurement/Ionosphere
  measurement/Troposphere
  measurementfile
  measurementmodel
  plugin
  propagator
  reporter
  signal
  tdmReader
  trackingfile
  trackingsystem
  )

# ====================================================================
# source files
SET(PLUGIN_SRCS
    adapter/DopplerAdapter.cpp
    adapter/DSNRangeAdapter.cpp
    adapter/GNDopplerAdapter.cpp
    adapter/GNRangeAdapter.cpp
    adapter/GPSAdapter.cpp
    adapter/PointRangeRateAdapterKps.cpp
    adapter/RangeAdapterKm.cpp
    adapter/RangeRateAdapterKps.cpp
    adapter/TDRSDopplerAdapter.cpp
    adapter/TDRSRangeAdapter.cpp
    adapter/TrackingDataAdapter.cpp
    command/RunEstimator.cpp
    command/RunSimulator.cpp
    datafilter/DataFilter.cpp
    datafilter/AcceptFilter.cpp
    datafilter/RejectFilter.cpp
    datafilter/StatisticAcceptFilter.cpp
    datafilter/StatisticRejectFilter.cpp
    errormodel/ErrorModel.cpp
    estimator/BatchEstimator.cpp
    estimator/BatchEstimatorInv.cpp
    estimator/BatchEstimatorSVD.cpp
    estimator/EstimationStateManager.cpp
    estimator/Estimator.cpp
    estimator/EstimatorException.cpp
    estimator/ExtendedKalmanInv.cpp
    estimator/SequentialEstimator.cpp
    estimator/Simulator.cpp
    event/EstimationRootFinder.cpp
    event/Event.cpp
    event/EventData.cpp
    event/EventException.cpp
    event/EventManager.cpp
    event/LightTimeCorrection.cpp
    factory/AdapterFactory.cpp
    factory/DataFileFactory.cpp
    factory/ErrorModelFactory.cpp
    factory/EstimationCommandFactory.cpp
    factory/EstimationDataFilterFactory.cpp
    factory/EstimatorFactory.cpp
    factory/EstimatorHardwareFactory.cpp
    factory/EventFactory.cpp
    factory/MeasurementFactory.cpp
    factory/MeasurementModelFactory.cpp
    factory/NavPropagatorFactory.cpp
    factory/ObTypeFactory.cpp
    factory/SignalModelFactory.cpp
    factory/TrackingDataFactory.cpp
    factory/TrackingSystemFactory.cpp
    hardware/Antenna.cpp
    hardware/RFHardware.cpp
    hardware/Receiver.cpp
    hardware/Sensor.cpp
    hardware/Signal.cpp
    hardware/Transmitter.cpp
    hardware/Transponder.cpp
    measurement/AveragedDoppler.cpp
    measurement/CoreMeasurement.cpp
    measurement/DSNTwoWayDoppler.cpp
    measurement/DSNTwoWayRange.cpp
    measurement/MeasurementData.cpp
    measurement/MeasurementManager.cpp
    measurement/MeasurementModel.cpp
    measurement/MeasurementModelBase.cpp
    measurement/MediaCorrection.cpp
    measurement/OpticalAngles.cpp
    measurement/OpticalAzEl.cpp
    measurement/PhysicalMeasurement.cpp
    measurement/SnTwoWayRange.cpp
    measurement/TDRSSTwoWayRange.cpp
    measurement/TrackingData.cpp
    measurement/TwoWayRange.cpp
    measurement/USNTwoWayRange.cpp
    measurement/Troposphere/Troposphere.cpp
    measurementfile/B3_obtype.cpp
    measurementfile/DataFile.cpp
    measurementfile/DataFileAdapter.cpp
    measurementfile/GmatObType.cpp
    measurementfile/GmatData.cpp
    measurementfile/GmatObType.cpp
    measurementfile/GmatODDopplerType.cpp
    measurementfile/GmatODType.cpp
    measurementfile/ObservationData.cpp
    measurementfile/ObType.cpp
    measurementfile/RampTableData.cpp
    measurementfile/RampTableType.cpp
    measurementmodel/MeasureModel.cpp
    measurementmodel/GPSPointMeasureModel.cpp
    plugin/GmatPluginFunctions.cpp
    propagator/RungeKutta4.cpp
    reporter/ProgressReporter.cpp
    signal/PhysicalSignal.cpp
    signal/SignalBase.cpp
    signal/SignalData.cpp
    signal/SinglePointSignal.cpp
    tdmReader/TdmErrorHandler.cpp
    tdmReader/TdmObType.cpp
    tdmReader/TdmReadWriter.cpp
    trackingfile/TFSMagicNumbers.cpp
    trackingfile/TrackingFileSet.cpp
    trackingsystem/DSNTrackingSystem.cpp
    trackingsystem/OpticalTrackingSystem.cpp
    trackingsystem/TDRSSTrackingSystem.cpp
    trackingsystem/TrackingSystem.cpp
    trackingsystem/USNTrackingSystem.cpp
)

# Temporarily disable Ionosphere for Mac/Linux
#if(WIN32)
  SET(PLUGIN_SRCS ${PLUGIN_SRCS}
    measurement/Ionosphere/Ionosphere.cpp
    measurement/Ionosphere/iri2007.c
    )
#endif()

# ====================================================================
# Setup common plugin definitions, targets, etc.
_SETUPPLUGIN(${TargetName} "${PLUGIN_DIRS}" "${PLUGIN_SRCS}" plugins)


# ====================================================================
# Additional includes
TARGET_INCLUDE_DIRECTORIES(${TargetName} PRIVATE ${F2C_DIR})

# ====================================================================
# Additional link libraries
TARGET_LINK_LIBRARIES(${TargetName} PRIVATE XercesC::XercesC)
if(WIN32)
  TARGET_COMPILE_DEFINITIONS(${TargetName} PUBLIC -DXERCES_STATIC_LIBRARY)
  TARGET_LINK_LIBRARIES(${TargetName} PUBLIC Ws2_32)
endif()

# Set RPATH to find shared libraries in default locations on Mac/Linux
# Disabled since static Xerces is being used. Enable in future if GMAT
# switches to dynamic Xerces
if(UNIX AND false)
  if(APPLE)
    SET(MAC_BASEPATH "../Frameworks/") # GmatBase and Xerces in Frameworks
    SET_TARGET_PROPERTIES(${TargetName} PROPERTIES INSTALL_RPATH
      "@loader_path/${MAC_BASEPATH}"
      )
  else()
    SET_TARGET_PROPERTIES(${TargetName} PROPERTIES INSTALL_RPATH
      "\$ORIGIN/;\$ORIGIN/libxerces/"
      )
  endif()
endif()


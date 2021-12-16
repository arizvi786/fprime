// ======================================================================
// \title  RpiCam.hpp
// \author arizvi
// \brief  hpp file for RpiCam component implementation class
//
// \copyright
// Copyright 2009-2015, by the California Institute of Technology.
// ALL RIGHTS RESERVED.  United States Government Sponsorship
// acknowledged.
//
// ======================================================================

#ifndef RpiCam_HPP
#define RpiCam_HPP

#include "RPI/RpiCam/RpiCamComponentAc.hpp"

namespace RPI {

  class RpiCam :
    public RpiCamComponentBase
  {

    public:

      // ----------------------------------------------------------------------
      // Construction, initialization, and destruction
      // ----------------------------------------------------------------------

      //! Construct object RpiCam
      //!
      RpiCam(
          const char *const compName /*!< The component name*/
      );

      //! Initialize object RpiCam
      //!
      void init(
          const NATIVE_INT_TYPE queueDepth, /*!< The queue depth*/
          const NATIVE_INT_TYPE instance = 0 /*!< The instance number*/
      );

      //! Destroy object RpiCam
      //!
      ~RpiCam();

    PRIVATE:

      // ----------------------------------------------------------------------
      // Handler implementations for user-defined typed input ports
      // ----------------------------------------------------------------------

      //! Handler implementation for Run
      //!
      void Run_handler(
          const NATIVE_INT_TYPE portNum, /*!< The port number*/
          NATIVE_UINT_TYPE context /*!< 
      The call order
      */
      );

    PRIVATE:

      // ----------------------------------------------------------------------
      // Command handler implementations
      // ----------------------------------------------------------------------

      //! Implementation for CAM_TakeImage command handler
      //! Command to send a string to the UART
      void CAM_TakeImage_cmdHandler(
          const FwOpcodeType opCode, /*!< The opcode*/
          const U32 cmdSeq, /*!< The command sequence number*/
          const Fw::CmdStringArg& filename /*!< 
          Location to store the captured image
          */
      );

      //! Implementation for CAM_RemoveImageFile command handler
      //! Command to remove an image file stored on-board
      void CAM_RemoveImageFile_cmdHandler(
          const FwOpcodeType opCode, /*!< The opcode*/
          const U32 cmdSeq, /*!< The command sequence number*/
          const Fw::CmdStringArg& filename /*!<
          Location of image file to remove
          */
      );
  
      I32 removeFile(const char* filename);

      U32 imgSucc;
      U32 imgFail;

    };

} // end namespace RPI

#endif

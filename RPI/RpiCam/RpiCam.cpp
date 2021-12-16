// ======================================================================
// \title  RpiCam.cpp
// \author arizvi
// \brief  cpp file for RpiCam component implementation class
//
// \copyright
// Copyright 2009-2015, by the California Institute of Technology.
// ALL RIGHTS RESERVED.  United States Government Sponsorship
// acknowledged.
//
// ======================================================================


#include <RPI/RpiCam/RpiCam.hpp>
#include "Fw/Types/BasicTypes.hpp"
#include <iostream>
#include <Os/File.hpp>
#include <Os/FileSystem.hpp>

namespace RPI {

  // ----------------------------------------------------------------------
  // Construction, initialization, and destruction
  // ----------------------------------------------------------------------

  RpiCam ::
    RpiCam(
        const char *const compName
    ) : RpiCamComponentBase(compName)
  {
    this->imgSucc = 0;
    this->imgFail = 0;
  }

  void RpiCam ::
    init(
        const NATIVE_INT_TYPE queueDepth,
        const NATIVE_INT_TYPE instance
    )
  {
    RpiCamComponentBase::init(queueDepth, instance);
  }

  RpiCam ::
    ~RpiCam()
  {

  }

  // ----------------------------------------------------------------------
  // Handler implementations for user-defined typed input ports
  // ----------------------------------------------------------------------

  I32 RpiCam ::
    removeFile(const char* filename)
    {
      I32 status=0;
      Os::FileSystem::Status fileSysStatus;

      FW_ASSERT(filename != NULL);

      //Remove file created at location
      fileSysStatus = Os::FileSystem::removeFile(filename);
      if( Os::FileSystem::OP_OK != fileSysStatus) {
        // Failed to remove file
        this->log_WARNING_HI_CAM_ImageFileRemoveFailed(filename);
        status = -1;
      }

      return status;
    }

  void RpiCam ::
    Run_handler(
        const NATIVE_INT_TYPE portNum,
        NATIVE_UINT_TYPE context
    )
  {
    // TODO
  }

  // ----------------------------------------------------------------------
  // Command handler implementations
  // ----------------------------------------------------------------------

  void RpiCam ::
    CAM_TakeImage_cmdHandler(
        const FwOpcodeType opCode,
        const U32 cmdSeq,
        const Fw::CmdStringArg& filename
    )
  {
    I32 status=0;

    Os::File file;
    Os::File::Status fileStatus;

    fileStatus = file.open(filename.toChar(), Os::File::OPEN_READ);
    if( Os::File::OP_OK == fileStatus) {
      // File exists
      this->log_WARNING_HI_CAM_ImageFileAlreadyExists(filename);
      this->imgFail++;
      this->tlmWrite_CAM_NumImagesFail(imgFail);
      this->cmdResponse_out(opCode,cmdSeq,Fw::CmdResponse::VALIDATION_ERROR);
      return;
    }
    file.close();

    fileStatus = file.open(filename.toChar(), Os::File::OPEN_WRITE);
    if( Os::File::OP_OK != fileStatus) {
      // Failed to create file
      this->log_WARNING_HI_CAM_ImageFileOpenFailed(filename);
      this->imgFail++;
      this->tlmWrite_CAM_NumImagesFail(imgFail);
      this->cmdResponse_out(opCode,cmdSeq,Fw::CmdResponse::VALIDATION_ERROR);
      return;
    }
    file.close();

    //Remove file created at location
    status = this->removeFile(filename.toChar());
    if (status < 0) {
      this->imgFail++;
      this->tlmWrite_CAM_NumImagesFail(imgFail);
      this->cmdResponse_out(opCode,cmdSeq,Fw::CmdResponse::VALIDATION_ERROR);
      return;
    }

    char raspiCmd[Fw::String::STRING_SIZE];
    (void)snprintf(raspiCmd, Fw::String::STRING_SIZE, "raspistill -o %s", filename.toChar());
    this->log_ACTIVITY_HI_CAM_ExecutingShellCmd(raspiCmd);
    status = system(raspiCmd);

    if (status < 0) {
      this->log_WARNING_HI_CAM_ImageCaptureFailed(filename);
      this->imgFail++;
      this->tlmWrite_CAM_NumImagesFail(imgFail);
      this->cmdResponse_out(opCode,cmdSeq,Fw::CmdResponse::EXECUTION_ERROR);
    }
    else {
      this->log_ACTIVITY_HI_CAM_ImageCaptured(filename);
      this->imgSucc++;
      this->tlmWrite_CAM_NumImagesSucc(imgSucc);
      this->cmdResponse_out(opCode,cmdSeq,Fw::CmdResponse::OK);
    }
  }

  void RpiCam ::
    CAM_RemoveImageFile_cmdHandler(
        const FwOpcodeType opCode,
        const U32 cmdSeq,
        const Fw::CmdStringArg& filename
    )
  {
    I32 status;

    status = this->removeFile(filename.toChar());
    if (status < 0) {
      this->cmdResponse_out(opCode,cmdSeq,Fw::CmdResponse::EXECUTION_ERROR);
    }
    else {    
      this->log_ACTIVITY_HI_CAM_ImageFileRemoved(filename);
      this->cmdResponse_out(opCode,cmdSeq,Fw::CmdResponse::OK);
    }
  }


} // end namespace RPI

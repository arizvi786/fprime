module RPI {

  @ An RPI camera component
  active component RpiCam {

    # ----------------------------------------------------------------------
    # Types
    # ----------------------------------------------------------------------

    # ----------------------------------------------------------------------
    # General ports
    # ----------------------------------------------------------------------

    @ Input Scheduler port - called at 1Hz and 10Hz
    async input port Run: Svc.Sched

    # ----------------------------------------------------------------------
    # Special ports
    # ----------------------------------------------------------------------

    @ Command receive
    command recv port cmdIn

    @ Command registration
    command reg port cmdRegOut

    @ Command response
    command resp port cmdResponseOut

    @ Event
    event port eventOut

    @ Parameter get
    param get port prmGetOut

    @ Parameter set
    param set port prmSetOut

    @ Telemetry
    telemetry port tlmOut

    @ Text event
    text event port textEventOut

    @ Time get
    time get port timeGetOut

    # ----------------------------------------------------------------------
    # Commands
    # ----------------------------------------------------------------------

    @ Command to capture an image and store it to file on-board
    async command CAM_TakeImage(
                                 $filename: string size 80 @< Location to store the captured image
                               ) \
      opcode 0

    @ Command to remove an image file stored on-board
    async command CAM_RemoveImageFile(
                                 $filename: string size 80 @< Location of image file to remove
                               ) \
      opcode 1

    # ----------------------------------------------------------------------
    # Events
    # ----------------------------------------------------------------------

    @ Image successfully captured at location
    event CAM_ImageCaptured(
                         filename: string size 80 @< The message
                       ) \
      severity activity high \
      id 0 \
      format "Image captured and stored at location: {}"

    @ Failed to capture image at location
    event CAM_ImageCaptureFailed(
                         filename: string size 80 @< The message
                       ) \
      severity warning high \
      id 1 \
      format "Image capture failed to store image at location: {}"

    @ Executing shell command
    event CAM_ExecutingShellCmd(
                         cmd: string size 256 @< The command
                       ) \
      severity activity high \
      id 2 \
      format "Executing shell command: {}"

    @ Image file already exists
    event CAM_ImageFileAlreadyExists(
                         filename: string size 256 @< The filename
                       ) \
      severity warning high \
      id 3 \
      format "Image file already exists at location: {}"

    @ Image file open failure
    event CAM_ImageFileOpenFailed(
                         filename: string size 256 @< The filename
                       ) \
      severity warning high \
      id 4 \
      format "Image file failed to open at location: {}"

    @ Image file remove failure
    event CAM_ImageFileRemoveFailed(
                         filename: string size 256 @< The filename
                       ) \
      severity warning high \
      id 5 \
      format "Image file failed to be removed at location: {}"

    @ Image file removed
    event CAM_ImageFileRemoved(
                         filename: string size 256 @< The filename
                       ) \
      severity activity high \
      id 6 \
      format "Image file removed at location: {}"

    # ----------------------------------------------------------------------
    # Parameters
    # ----------------------------------------------------------------------

    # ----------------------------------------------------------------------
    # Telemetry
    # ----------------------------------------------------------------------

    @ Number of images captured successfully completed since FSW boot-up
    telemetry CAM_NumImagesSucc: U32 id 0

    @ Number of image captures failed since FSW boot-up
    telemetry CAM_NumImagesFail: U32 id 1

  }

}

from enum import *
# Defined some enums from C library that hopefully make your life easier.


# Note:
# Please use the latest version of python which have enum34 library

class MentalCommandActionEnum(IntEnum):
    MC_NONE                     = 0x0000
    MC_NEUTRAL                  = 0x0001
    MC_PUSH                     = 0x0002
    MC_PULL                     = 0x0004
    MC_LIFT                     = 0x0008
    MC_DROP                     = 0x0010
    MC_LEFT                     = 0x0020
    MC_RIGHT                    = 0x0040
    MC_ROTATE_LEFT              = 0x0080
    MC_ROTATE_RIGHT             = 0x0100
    MC_ROTATE_CLOCKWISE         = 0x0200
    MC_ROTATE_COUNTER_CLOCKWISE = 0x0400
    MC_ROTATE_FORWARDS          = 0x0800
    MC_ROTATE_REVERSE           = 0x1000
    MC_DISAPPEAR                = 0x2000


class MentalCommandTrainingControlEnum(IntEnum):
    MC_NONE 	= 0     # No action.
    MC_START 	= 1     # Start a new training.
    MC_ACCEPT 	= 2     # Accept current training.
    MC_REJECT 	= 3     # Reject current training.
    MC_ERASE 	= 4     # Erase training data for an action.
    MC_RESET 	= 5     # Reset current training.

class MentalCommandEventEnum(IntEnum):
    IEE_MentalCommandNoEvent                      = 0
    IEE_MentalCommandTrainingStarted              = 1
    IEE_MentalCommandTrainingSucceeded            = 2
    IEE_MentalCommandTrainingFailed               = 3
    IEE_MentalCommandTrainingCompleted            = 4
    IEE_MentalCommandTrainingDataErased           = 5
    IEE_MentalCommandTrainingRejected             = 6
    IEE_MentalCommandTrainingReset                = 7
    IEE_MentalCommandAutoSamplingNeutralCompleted = 8
    IEE_MentalCommandSignatureUpdated             = 9

# In the Iedk.h file, the last item doesn't have any pre-defined value, as in C/C++, last item's value will be implicitly increased by 1, so I do the same here
class EngineEventEnum(IntEnum):
    IEE_UnknownEvent          = 0x0000  # An unknown event.
    IEE_EmulatorError         = 0x0001  # Error event from emulator. Connection to EmoComposer could be lost.
    IEE_ReservedEvent         = 0x0002  # Reserved event.
    IEE_UserAdded             = 0x0010  # A headset is connected.
    IEE_UserRemoved           = 0x0020  # A headset has been disconnected.
    IEE_EmoStateUpdated       = 0x0040  # Detection results have been updated from EmoEngine.
    IEE_ProfileEvent          = 0x0080  # A profile has been returned from EmoEngine.
    IEE_MentalCommandEvent    = 0x0100  # A IEE_MentalCommandEvent_t has been returned from EmoEngine.
    IEE_FacialExpressionEvent = 0x0200  # A IEE_FacialExpressionEvent_t has been returned from EmoEngine.
    IEE_InternalStateChanged  = 0x0400  # Reserved for internal use.
    IEE_AllEvent              = IEE_InternalStateChanged + 1 # Bit-mask for all events except error types.


# Refer to IedkErrorCode.h for more information
class ErrorCodeEnum(IntEnum):
    EDK_ACCESS_DENIED               = 0x2031
    EDK_APP_INVALID_DATE            = 0x2017
    EDK_APP_QUOTA_EXCEEDED          = 0x2016
    EDK_AUTHENTICATION_ERROR        = 0x2101
    EDK_BUFFER_TOO_SMALL            = 0x0300
    EDK_CANNOT_ACQUIRE_DATA         = 0x0200
    EDK_CLOUD_PROFILE_EXISTS        = 0x1010
    EDK_CLOUD_USER_ID_DONT_LOGIN    = 0x1022
    EDK_COULDNT_CONNECT             = 0x1003
    EDK_COULDNT_RESOLVE_HOST        = 0x1002
    EDK_COULDNT_RESOLVE_PROXY       = 0x1001
    EDK_DAILY_DEBIT_LIMITED         = 0x2025
    EDK_DEVICE_CODE_ERROR           = 0x2004
    EDK_EMOENGINE_DISCONNECTED      = 0x0501
    EDK_EMOENGINE_PROXY_ERROR       = 0x0502
    EDK_EMOENGINE_UNINITIALIZED     = 0x0500
    EDK_EMOTIVCLOUD_UNINITIALIZED   = 0x1023
    EDK_FE_NO_SIG_AVAILABLE         = 0x0308
    EDK_FILESTREAM_ERROR            = 0x0504
    EDK_FILESYSTEM_ERROR            = 0x0309
    EDK_FILE_ERROR                  = 0x0506
    EDK_FILE_EXISTS                 = 0x2000
    EDK_FILE_NOT_FOUND              = 0x2030
    EDK_GYRO_NOT_CALIBRATED         = 0x0700
    EDK_HEADSET_IS_OFF              = 0x2002
    EDK_HEADSET_NOT_AVAILABLE       = 0x2001
    EDK_INVALID_CLOUD_USER_ID       = 0x1020
    EDK_INVALID_DEBIT_ERROR         = 0x2014
    EDK_INVALID_DEBIT_NUMBER        = 0x2024
    EDK_INVALID_DEV_ID_ERROR        = 0x0002
    EDK_INVALID_ENGINE_USER_ID      = 0x1021
    EDK_INVALID_PARAMETER           = 0x0302
    EDK_INVALID_PROFILE_ARCHIVE     = 0x0101
    EDK_INVALID_USER_ID             = 0x0400
    EDK_LICENSE_DEVICE_LIMITED      = 0x2019
    EDK_LICENSE_ERROR               = 0x2010
    EDK_LICENSE_EXPIRED             = 0x2011
    EDK_LICENSE_NOT_FOUND           = 0x2012
    EDK_LICENSE_NO_EEG              = 0x2022
    EDK_LICENSE_REGISTERED          = 0x2020
    EDK_LOGIN_ERROR                 = 0x2102
    EDK_MC_EXCESS_MAX_ACTIONS       = 0x0307
    EDK_MC_INVALID_ACTIVE_ACTION    = 0x0306
    EDK_MC_INVALID_TRAINING_ACTION  = 0x0304
    EDK_MC_INVALID_TRAINING_CONTROL = 0x0305
    EDK_NO_ACTIVE_LICENSE           = 0x2021
    EDK_NO_EVENT                    = 0x0600
    EDK_NO_INTERNET_CONNECTION      = 0x2100
    EDK_NO_USER_FOR_BASEPROFILE     = 0x0102
    EDK_OK                          = 0x0000
    EDK_OPERATION_TIMEDOUT          = 0x1004
    EDK_OPTIMIZATION_IS_ON          = 0x0800
    EDK_OUT_OF_RANGE                = 0x0301
    EDK_OVER_DEVICE_LIST            = 0x2015
    EDK_OVER_QUOTA                  = 0x2013
    EDK_PARAMETER_LOCKED            = 0x0303
    EDK_RESERVED1                   = 0x0900
    EDK_SAVING_IS_RUNNING           = 0x2003
    EDK_STREAM_NOT_SUPPORTED        = 0x0505
    EDK_STREAM_UNINITIALIZED        = 0x0503
    EDK_UNKNOWN_ERROR               = 0x0001
    EDK_UPDATE_LICENSE              = 0x2023
    EDK_UPLOAD_FAILED               = 0x1011

class DataChannelEnum(IntEnum):
    IED_COUNTER         = 0
    IED_INTERPOLATED    = 1
    IED_RAW_CQ          = 2
    IED_AF3             = 3
    IED_F7              = 4
    IED_F3              = 5
    IED_FC5             = 6
    IED_T7              = 7
    IED_P7              = 8
    IED_Pz              = 9
    IED_O1              = IED_Pz
    IED_O2              = 10
    IED_P8              = 11
    IED_T8              = 12
    IED_FC6             = 13
    IED_F4              = 14
    IED_F8              = 15
    IED_AF4             = 16
    IED_GYROX           = 17
    IED_GYROY           = 18
    IED_TIMESTAMP       = 19
    IED_MARKER_HARDWARE = 20
    IED_ES_TIMESTAMP    = 21
    IED_FUNC_ID         = 22
    IED_FUNC_VALUE      = 23
    IED_MARKER          = 24
    IED_SYNC_SIGNAL     = 25

class DetechtionEnum(IntEnum):
    DT_BlinkAndWink     = 0x0001  # Blink and Wink detection
    DT_FacialExpression = 0x0002  # Other facial expression detection
    DT_EyeMovement      = 0x0004  # Eye movement detection
    DT_Excitement       = 0x0008  # Excitement detection (deprecated)
    DT_Engagement       = 0x0010  # Engagement detection (deprecated)
    DT_Relaxation       = 0x0020  # Relaxation detection (deprecated)
    DT_Interest         = 0x0040  # Interest detection (deprecated)
    DT_Stress           = 0x0080  # Stress detection (deprecated)
    DT_Focus            = 0x0100  # Focus detection (deprecated)
    DT_MentalCommand    = 0x0200  # Mental command detection
    DT_FFTDetection     = 0x0400  # FFTDataDection
    DT_AllDetections    = (DT_BlinkAndWink | DT_FacialExpression | DT_EyeMovement |
                        DT_Excitement | DT_Engagement | DT_Relaxation |
                        DT_Interest | DT_Stress | DT_Focus |
                        DT_MentalCommand | DT_FFTDetection)

class MotionChannelEnum(IntEnum):
    IMD_COUNTER   = 0  # Sample counter
    IMD_GYROX     = 1  # Gyroscope X-axis
    IMD_GYROY     = 2  # Gyroscope Y-axis
    IMD_GYROZ     = 3  # Gyroscope Z-axis
    IMD_ACCX      = 4  # Accelerometer X-axis
    IMD_ACCY      = 5  # Accelerometer Y-axis
    IMD_ACCZ      = 6  # Accelerometer Z-axis
    IMD_MAGX      = 7  # Magnetometer X-axis
    IMD_MAGY      = 8  # Magnetometer Y-axis
    IMD_MAGZ      = 9  # Magnetometer Z-axis
    IMD_TIMESTAMP = 10 # Timestamp of the motion data stream

class WindowTypeEnum(IntEnum):
    IEE_HANNING   = 0  # Hanning Window
    IEE_HAMMING   = 1  # Hamming Window
    IEE_HANN      = 2  # Hann Window
    IEE_BLACKMAN  = 3  # Blackman-Harris Window
    IEE_RECTANGLE = 4  # Uniform/rectangular Window

class FacialExpressionAlgoEnum(IntEnum):
    FE_NEUTRAL     = 0x0001
    FE_BLINK       = 0x0002
    FE_WINK_LEFT   = 0x0004
    FE_WINK_RIGHT  = 0x0008
    FE_HORIEYE     = 0x0010
    FE_SURPRISE    = 0x0020
    FE_FROWN       = 0x0040
    FE_SMILE       = 0x0080
    FE_CLENCH      = 0x0100
    FE_LAUGH       = 0x0200
    FE_SMIRK_LEFT  = 0x0400
    FE_SMIRK_RIGHT = 0x0800

class FacialExpressionTrainingControlEnum(IntEnum):
    FE_NONE   = 0 # No action
    FE_START  = 1 # Start a new training
    FE_ACCEPT = 2 # Accept current training
    FE_REJECT = 3 # Reject current training
    FE_ERASE  = 4 # Erase training data for a facial expression
    FE_RESET  = 5 # Reset current training

# Input - enum class name
# Input - element val
# Output - element name
def parseEdkEnum(enum, val):
    for i in enum:
        if i.value == val:
            return i.name
			
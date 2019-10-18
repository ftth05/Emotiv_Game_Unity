import sys
import os
from threading import Thread
from ctypes import *

# REQUIRED
from utilities import *

# Note:
# $python: Python 2.7.14 32bit on win32
# The proper way to call these IEE_ functions is assigning them to an int value then check that returned value
# E.g.
# errorCode = libEDK.IEE_EngineConnect("Emotiv Systems-5")
# if errorCode is EDK_OK, meaning function IEE_EngineConnect was executed successfully, without error
# if errorCode is another value ( != EDK_OK), check what it means in utilities.py file - ErrorCode enum

try:
    if sys.platform.startswith('win32'):
        # If Your Python is 64-bit version, load edk.dll from win64 folder instead
        libEDK = cdll.LoadLibrary("../../bin/win32/edk.dll")
    elif sys.platform.startswith('linux'):
        srcDir = os.getcwd()
        if sys.platform.startswith('arm'):
            libPath = srcDir + "/../../bin/armhf/libedk.so"
        else:
            libPath = srcDir + "/../../bin/linux64/libedk.so"
        libEDK = CDLL(libPath)
    else:
        raise Exception('System not supported.')
except Exception as e:
    print 'Error: cannot load EDK lib:', e
    exit()


##-------------------------------------------------------------------------------##
##------------- EDIT THESE FIELDS BEFORE CONTINUING ----------------------------###
userName = "your_username"
password = "your_password"

profileName = "your_profile_name"

# Training profile was saved with some kind of version control, but no API is exposed
# Therefor you can only get the latest version of it
PROFILE_VERSION = -1

# Pre-define MC actions
# Max is 4 actions
MCAction1 = MentalCommandActionEnum.MC_PULL
MCAction2 = MentalCommandActionEnum.MC_PUSH
MCAction3 = MentalCommandActionEnum.MC_NONE
MCAction4 = MentalCommandActionEnum.MC_NONE
# Bitwise operator, use to create ACTIONLIST and verify valid action
ACTIONLIST = MCAction1 | MCAction2 | MCAction3 | MCAction4 | MentalCommandActionEnum.MC_NEUTRAL
ACTIONLIST_ARRAY = [MCAction1, MCAction2, MCAction3, MCAction4]
##----------------------------------------------------------------------------##
################################################################################

running = False

eEvent = libEDK.IEE_EmoEngineEventCreate()
eState = libEDK.IEE_EmoStateCreate()


IS_MentalCommandGetCurrentActionPower = libEDK.IS_MentalCommandGetCurrentActionPower
IS_MentalCommandGetCurrentActionPower.argtypes = [c_void_p]
IS_MentalCommandGetCurrentActionPower.restype = c_float

userID    = c_uint(0)
ptrUserID = pointer(userID)

profileID = c_uint(0)
ptrProfileID = pointer(profileID)

state           = c_int(0)
mental_state    = c_int(0)
power           = c_float(0.0)
EngineEventType = c_int(0)

ActionOut  = c_long(0)
pActionOut = pointer(ActionOut)

TrainingActionOut    = c_int(0)
ptrTrainingActionOut = pointer(TrainingActionOut)

chargeLevel    = c_int(0)
ptrChargeLevel = pointer(chargeLevel)

maxChargeLevel    = c_int(0)
ptrMaxChargeLevel = pointer(maxChargeLevel)

cloudUserID    = c_uint(0)
ptrCloudUserID = pointer(cloudUserID)

IEE_InputChannels_enum = ["IEE_CHAN_CMS",
                          "IEE_CHAN_DRL",
                          "IEE_CHAN_FP1",
                          "IEE_CHAN_AF3",
                          "IEE_CHAN_F7",
                          "IEE_CHAN_F3",
                          "IEE_CHAN_FC5",
                          "IEE_CHAN_T7",
                          "IEE_CHAN_P7",
                          "IEE_CHAN_O1", #="IEE_CHAN_Pz"
                          "IEE_CHAN_O2",
                          "IEE_CHAN_P8",
                          "IEE_CHAN_T8",
                          "IEE_CHAN_FC6",
                          "IEE_CHAN_F4",
                          "IEE_CHAN_F8",
                          "IEE_CHAN_AF4",
                          "IEE_CHAN_FP2"]

IEE_EEG_ContactQuality_enum = ["No Signal",
                               "Very Bad",
                               "Poor",
                               "Fair",
                               "Good"]

IEE_SignalStrength_enum = ["No Signal", "Bad Signal", "Good Signal"]

#------------------------------------------------------------------------------------------------------------------------------------------------------------

def handleMentalCommandEvent(cognitiveEvent):
    libEDK.IEE_EmoEngineEventGetUserId(cognitiveEvent, ptrUserID)
    eventType = libEDK.IEE_MentalCommandEventGetType(cognitiveEvent)

    if eventType == MentalCommandEventEnum.IEE_MentalCommandTrainingStarted:
        print "Mental Command Training for User", userID.value, " STARTED!"

    elif eventType == MentalCommandEventEnum.IEE_MentalCommandTrainingSucceeded:
        print "Mental Command Training for User", userID.value, "SUCCEEDED! Type \"accept\" or \"reject\""

    elif eventType == MentalCommandEventEnum.IEE_MentalCommandTrainingFailed:
        print "Mental Command Training for User", userID.value, " FAILED!"

    elif eventType == MentalCommandEventEnum.IEE_MentalCommandTrainingCompleted:
        print "Mental Command Training for User", userID.value, " COMPLETED!"

    elif eventType == MentalCommandEventEnum.IEE_MentalCommandTrainingDataErased:
        print "Mental Command Training Data for User", userID.value, " ERASED!"

    elif eventType == MentalCommandEventEnum.IEE_MentalCommandTrainingRejected:
        print "Mental Command Training for User", userID.value, " REJECTED!"

    elif eventType == MentalCommandEventEnum.IEE_MentalCommandTrainingReset:
        print "Mental Command Training for User", userID.value, " RESET!"

    elif eventType == MentalCommandEventEnum.IEE_MentalCommandAutoSamplingNeutralCompleted:
        print "Mental Command Auto Sampling Neutral for User", userID.value, " COMPLETED!"

    elif eventType == MentalCommandEventEnum.IEE_MentalCommandSignatureUpdated:
        print "Mental Command Signature for User", userID.value, " UPDATED!"

    elif eventType == MentalCommandEventEnum.IEE_MentalCommandNoEvent:
        print "No Mental Command Event"


def parsecommand(inputs):
    wrongArgument = False
    commands = inputs.split()

    if commands[0] == "status":
        libEDK.IS_GetBatteryChargeLevel(eState, ptrChargeLevel, ptrMaxChargeLevel)
        WirelessStrength = libEDK.IS_GetWirelessSignalStatus(eState)
        NumContact = libEDK.IS_GetNumContactQualityChannels(eState)
        print "Battery Level:", chargeLevel.value, "out of", maxChargeLevel.value
        print "Wireless Strength:", IEE_SignalStrength_enum[WirelessStrength]
        print "Number of Contact:", NumContact
        if NumContact != 0:
            for i in range(0, NumContact):
                SignalCondition = libEDK.IS_GetContactQuality(eState, i)
                print IEE_InputChannels_enum[i], "\t :", IEE_EEG_ContactQuality_enum[SignalCondition]
        else:
            print "Headset signal not available"

    elif commands[0] == "next":
        print "Moving onto Data Acquisition Mode!"
        wrongArgument = False

    elif commands[0] == "quit":
        print "BYE!"
        wrongArgument = False

    elif commands[0] == "help":
        print "Following step for training..."
        print "1. save (create/save profile to Cloud) or load "
        print "2. set"
        print "Train each action from \"train [action]\" > \"start\" > \"accept/reject\" step-by-step"
        print "3. train [neutral/push/pull/etc.] - Note: Always train neutral first, only train action \n"
        print "   that defined in ACTIONLIST"
        print "4. start" # Start action that just set with train command
        print "5. accept or reject"
        print "Optional: erase, status, report, running"
        wrongArgument = False

    elif commands[0] == "save":

        libEDK.EC_GetProfileId(cloudUserID, profileName, ptrProfileID)

        if profileID.value >= 0:
            if libEDK.EC_UpdateUserProfile(cloudUserID.value, userID.value, profileID.value, profileName) == ErrorCodeEnum.EDK_OK:
                print "Updating finished"
            else:
                print "Updating failed"
                # Note: profileType = 0 if you are using training-profile
                #       profileType = 1 if you are using EmoKey
                # Read the API Reference document (EmotivCloudClient.h) for more information
                profileType = 0
                #Create a new profile if not existed
                if libEDK.EC_SaveUserProfile(cloudUserID.value, userID.value, profileName, profileType) == ErrorCodeEnum.EDK_OK:
                    print "Saving finished"
                else:
                    print "Saving failed"
                    exit()

    elif commands[0] == "load":
        numberOfProfile = libEDK.EC_GetAllProfileName(cloudUserID.value)
        libEDK.EC_GetProfileId(cloudUserID, profileName, ptrProfileID)
        print "Profile ID: ", profileID.value
        print "Profile Name: ", profileName

        if numberOfProfile > 0:
            if libEDK.EC_LoadUserProfile(cloudUserID.value, userID.value, profileID.value, PROFILE_VERSION) == ErrorCodeEnum.EDK_OK:
                print "Loading finished"
            else:
                print "Loading failed"
                exit()

    elif commands[0] == "set":
        # The second argument is ACTIONLIST without MC_NEUTRAL
        if libEDK.IEE_MentalCommandSetActiveActions(userID, ACTIONLIST ^ MentalCommandActionEnum.MC_NEUTRAL) == ErrorCodeEnum.EDK_OK:
            print "Actions set active for user", userID.value, "."
        else:
            print "Activating actions FAILED!"

    # Set action that will be trained if call training_start
    elif commands[0] == "train":  # Input Ex.: training_action push
        if len(commands) == 2:
            for action in MentalCommandActionEnum:
                # Can only train action that defined in ACTIONLIST
                if (('MC_' + commands[1].upper()) == action.name) and (action.value | ACTIONLIST) == ACTIONLIST:
                    errorCode = libEDK.IEE_MentalCommandSetTrainingAction(userID, action.value)
                    if errorCode == ErrorCodeEnum.EDK_OK:
                        print 'Set action {} for training \n'.format(action.name)
                        wrongArgument = False
                    else:
                        print 'Failed to set action {} for training, error: {} \n'.format(action.name, errorCode)
        else:
            wrongArgument = True
            print 'Wrong command format. Should be \"training_action [action_name]\"'

    elif commands[0] == "start":
        if len(commands) == 1:
            if libEDK.IEE_MentalCommandSetTrainingControl(userID, MentalCommandTrainingControlEnum.MC_START) == ErrorCodeEnum.EDK_OK:
                err = libEDK.IEE_MentalCommandGetTrainingAction(userID, ptrTrainingActionOut)
                print "Training ", parseEdkEnum(MentalCommandActionEnum, TrainingActionOut.value), " start! "
            else:
                print "Start training ", [TrainingActionOut.value], " failed!"
        else:
            wrongArgument = True

    elif commands[0] == "accept":
        if len(commands) == 1:
            if libEDK.IEE_MentalCommandSetTrainingControl(userID, MentalCommandTrainingControlEnum.MC_ACCEPT) == ErrorCodeEnum.EDK_OK:
                libEDK.IEE_MentalCommandGetTrainingAction(userID, ptrTrainingActionOut)
                print "Accepting Training for", parseEdkEnum(MentalCommandActionEnum, TrainingActionOut.value)
            else:
                print "Training Acceptance Failed"
        else:
            wrongArgument = True

    elif commands[0] == "reject":
        if len(commands) == 1:
            if libEDK.IEE_MentalCommandSetTrainingControl(userID, MentalCommandTrainingControlEnum.MC_REJECT) == ErrorCodeEnum.EDK_OK:
                libEDK.IEE_MentalCommandGetTrainingAction(userID, ptrTrainingActionOut)
                print "Rejecting training for", parseEdkEnum(MentalCommandActionEnum, TrainingActionOut.value)
            else:
                print "Training Reject Failed"
        else:
            wrongArgument = True

    elif commands[0] == "erase":
        if len(commands) == 1:
            if libEDK.IEE_MentalCommandSetTrainingControl(userID, MentalCommandTrainingControlEnum.MC_ERASE) == ErrorCodeEnum.EDK_OK:
                if libEDK.IEE_MentalCommandGetTrainingAction(userID, ptrTrainingActionOut) != 0:
                    print "Erase training for", parseEdkEnum(MentalCommandActionEnum, TrainingActionOut.value)
                    wrongArgument = False
            else:
               print "Training Erase Failed"
        else:
            wrongArgument = True

    elif commands[0] == "running":
        global running
        running = True

    elif commands[0] == "report":
        print "Report:"
        if libEDK.IEE_MentalCommandGetTrainedSignatureActions(userID, pActionOut) == ErrorCodeEnum.EDK_OK:
            print "Trained Action:", ActionOut.value, "\n"
        else:
            print "Error in Retrieving Trained Signature Actions"
        for action in MentalCommandActionEnum:
            if action.value == MentalCommandActionEnum.MC_NONE or action.value == MentalCommandActionEnum.MC_NEUTRAL:
                continue
            # Get skill rating of each action in ACTIONLIST
            if action.value | ACTIONLIST == ACTIONLIST:
                skillRating = c_float(0)
                ptrSkillRating = pointer(skillRating)
                libEDK.IEE_MentalCommandGetActionSkillRating(userID, action.value, ptrSkillRating)
                print 'Action: {} - skill rating: {}'.format(action.name, skillRating.value)
    else:
        print "Unknown Command", commands[0], "."

    if wrongArgument:
        print "Wrong argument(s) for command ", commands, "."
    return

def getcommand():
    while 1:
        command = str(raw_input())
        command = command.lower()
        parsecommand(command)

#----------------------------------------------------------------------------
print "========================================================================"
print "Example to show how to train Mental Commands from EmoEngine/EmoComposer."
print "========================================================================"
#----------------------------------------------------------------------------
# print 'Action list:'
# for action in ACTIONLIST_ARRAY:
    # print '{}'.format(parseEdkEnum(MentalCommandActionEnum, TrainingActionOut.value))

if libEDK.IEE_EngineConnect("Emotiv Systems-5") != 0:
    print "Emotiv Engine start up failed."

if libEDK.EC_Connect() != 0:
    print "Cannot connect to Emotiv Cloud"
    exit()

if libEDK.EC_Login(userName, password) != 0:
    print "Your login attempt has failed. The username or password may be incorrect"
    exit()

print "Logged in as %s" % userName

if libEDK.EC_GetUserDetail(ptrCloudUserID) != 0:
    exit()

print "Enter your command. Enter \"help\" for commands\n"

thread1 = Thread(target = getcommand)
thread1.start()

while 1:
    state = libEDK.IEE_EngineGetNextEvent(eEvent)
    if state == ErrorCodeEnum.EDK_OK:
        EngineEventType = libEDK.IEE_EmoEngineEventGetType(eEvent)
        libEDK.IEE_EmoEngineEventGetUserId(eEvent, ptrUserID)
        if EngineEventType == EngineEventEnum.IEE_UserAdded:
            print "New User", userID.value, "added"

        if EngineEventType == EngineEventEnum.IEE_UserRemoved:
            print "User ", userID.value, " Removed"

        if EngineEventType == EngineEventEnum.IEE_EmoStateUpdated:
            libEDK.IEE_EmoEngineEventGetEmoState(eEvent, eState)
            mental_state = libEDK.IS_MentalCommandGetCurrentAction(eState)
            power = IS_MentalCommandGetCurrentActionPower(eState)

            if running:
                print 'Current command: {} - Power: {}'.format(parseEdkEnum(MentalCommandActionEnum, mental_state), power)

        if EngineEventType == EngineEventEnum.IEE_MentalCommandEvent:
            handleMentalCommandEvent(eEvent)

#---------------------------------------------------------------------------------------------------------------------------------------------------------------
libEDK.IEE_EmoStateFree(eState)
libEDK.IEE_EmoEngineEventFree(eEvent)
libEDK.IEE_EngineDisconnect()

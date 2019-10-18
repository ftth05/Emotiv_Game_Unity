/****************************************************************************
**
** Copyright 2015 by Emotiv. All rights reserved
** Example - EmoStateLogger
** This example demonstrates the use of the core Emotiv API functions.
** It logs all Emotiv detection results for the attached users after
** successfully establishing a connection to Emotiv EmoEngineTM or
** EmoComposerTM
****************************************************************************/

using System;
using System.Collections.Generic;
using Emotiv;
using System.IO;
using System.Threading;

namespace EmoStateLogger
{
    class EmoStateLogger
    {
        static int userID = -1;
        static System.IO.StreamWriter engineLog = new System.IO.StreamWriter("engineLog.csv");
        static System.IO.StreamWriter expLog = new System.IO.StreamWriter("FacialExpression.csv");

        static void engine_EmoEngineConnected(object sender, EmoEngineEventArgs e)
        {
            Console.WriteLine("Emoengine connected");
        }

        static void engine_EmoEngineDisconnected(object sender, EmoEngineEventArgs e)
        {
            Console.WriteLine("Emoengine disconnected");
        }
        static void engine_UserAdded(object sender, EmoEngineEventArgs e)
        {
            Console.WriteLine("user added ({0})", e.userId);
            userID = (int)e.userId;
        }
        static void engine_UserRemoved(object sender, EmoEngineEventArgs e)
        {
            Console.WriteLine("user removed");
        }

        static void engine_EmoStateUpdated(object sender, EmoStateUpdatedEventArgs e)
        {
            EmoState es = e.emoState;

            Single timeFromStart = es.GetTimeFromStart();
            //Console.WriteLine("new emostate {0}", timeFromStart);
        }

        static void engine_EmoEngineEmoStateUpdated(object sender, EmoStateUpdatedEventArgs e)
        {
            EmoState es = e.emoState;

            Single timeFromStart = es.GetTimeFromStart();

            Int32 headsetOn = es.GetHeadsetOn();

            EdkDll.IEE_SignalStrength_t signalStrength = es.GetWirelessSignalStatus();
            Int32 chargeLevel = 0;
            Int32 maxChargeLevel = 0;
            es.GetBatteryChargeLevel(out chargeLevel, out maxChargeLevel);

            engineLog.Write(
                "{0},{1},{2},{3},{4},",
                timeFromStart,
                headsetOn, signalStrength, chargeLevel, maxChargeLevel);
            
            engineLog.WriteLine("");
            engineLog.Flush();
        }  


        static void engine_FacialExpressionEmoStateUpdated(object sender, EmoStateUpdatedEventArgs e)
        {
            EmoState es = e.emoState;

            Single timeFromStart = es.GetTimeFromStart();

            EdkDll.IEE_FacialExpressionAlgo_t[] expAlgoList = { 
                                                      EdkDll.IEE_FacialExpressionAlgo_t.FE_BLINK, 
                                                      EdkDll.IEE_FacialExpressionAlgo_t.FE_CLENCH, 
                                                      EdkDll.IEE_FacialExpressionAlgo_t.FE_SUPRISE, 
                                                      EdkDll.IEE_FacialExpressionAlgo_t.FE_FROWN, 
                                                      EdkDll.IEE_FacialExpressionAlgo_t.FE_HORIEYE, 
                                                      EdkDll.IEE_FacialExpressionAlgo_t.FE_NEUTRAL, 
                                                      EdkDll.IEE_FacialExpressionAlgo_t.FE_SMILE, 
                                                      EdkDll.IEE_FacialExpressionAlgo_t.FE_WINK_LEFT, 
                                                      EdkDll.IEE_FacialExpressionAlgo_t.FE_WINK_RIGHT
                                                      };
            Boolean[] isExpActiveList = new Boolean[expAlgoList.Length];

            Boolean isBlink = es.FacialExpressionIsBlink();
            Boolean isLeftWink = es.FacialExpressionIsLeftWink();
            Boolean isRightWink = es.FacialExpressionIsRightWink();
            Boolean isEyesOpen = es.FacialExpressionIsEyesOpen();
            Boolean isLookingUp = es.FacialExpressionIsLookingUp();
            Boolean isLookingDown = es.FacialExpressionIsLookingDown();
            Single leftEye = 0.0F;
            Single rightEye = 0.0F;
            Single x = 0.0F;
            Single y = 0.0F;
            es.FacialExpressionGetEyelidState(out leftEye, out rightEye);
            es.FacialExpressionGetEyeLocation(out x, out y);
            Single eyebrowExtent = es.FacialExpressionGetEyebrowExtent();
            Single smileExtent = es.FacialExpressionGetSmileExtent();
            Single clenchExtent = es.FacialExpressionGetClenchExtent();
            EdkDll.IEE_FacialExpressionAlgo_t upperFaceAction = es.FacialExpressionGetUpperFaceAction();
            Single upperFacePower = es.FacialExpressionGetUpperFaceActionPower();
            EdkDll.IEE_FacialExpressionAlgo_t lowerFaceAction = es.FacialExpressionGetLowerFaceAction();
            Single lowerFacePower = es.FacialExpressionGetLowerFaceActionPower();
            //for (int i = 0; i < expAlgoList.Length; ++i)
            //{
            //    isExpActiveList[i] = es.FacialExpressionIsActive(expAlgoList[i]);
            //}

            expLog.Write(
                "{0},{1},{2},{3},{4},{5},{6},{7},{8},{9},{10},{11},{12},{13},{14},{15},{16},",
                timeFromStart,
                isBlink, isLeftWink, isRightWink, isEyesOpen, isLookingUp,
                isLookingDown, leftEye, rightEye,
                x, y, eyebrowExtent, smileExtent, upperFaceAction,
                upperFacePower, lowerFaceAction, lowerFacePower);
            //for (int i = 0; i < expAlgoList.Length; ++i)
            //{
            //    expLog.Write("{0},", isExpActiveList[i]);
            //}
            expLog.WriteLine("");
            expLog.Flush();
        }

        static void Main(string[] args)
        {
            EmoEngine engine = EmoEngine.Instance;

            engine.EmoEngineConnected +=
                new EmoEngine.EmoEngineConnectedEventHandler(engine_EmoEngineConnected);
            engine.EmoEngineDisconnected +=
                new EmoEngine.EmoEngineDisconnectedEventHandler(engine_EmoEngineDisconnected);
            engine.UserAdded +=
                new EmoEngine.UserAddedEventHandler(engine_UserAdded);
            engine.UserRemoved +=
                new EmoEngine.UserRemovedEventHandler(engine_UserRemoved);
            engine.EmoStateUpdated +=
                new EmoEngine.EmoStateUpdatedEventHandler(engine_EmoStateUpdated);
            engine.FacialExpressionEmoStateUpdated +=
                new EmoEngine.FacialExpressionEmoStateUpdatedEventHandler(engine_FacialExpressionEmoStateUpdated);        
            engine.EmoEngineEmoStateUpdated +=
                new EmoEngine.EmoEngineEmoStateUpdatedEventHandler(engine_EmoEngineEmoStateUpdated);


            //write header
            string headerEngine = "Time, HeadSetOn, Wireless Strength, Battery Level, Max Batery Level";
            engineLog.WriteLine(headerEngine);
            engineLog.WriteLine("");

            //header facial expression
            string headerExp = "Time, isBlink, isLeftWink, isRightWink, isEyesOpen, isLookingUp, isLookingDown, leftEye, rightEye, x, y, eyebrowExtent, smileExtent, upperFaceAction, upperFacePower, lowerFaceAction, lowerFacePower";
            expLog.WriteLine(headerExp);
            expLog.WriteLine("");


            engine.Connect();

            Console.WriteLine("===========================================================================");
            Console.WriteLine("Example to show how to log the EmoState from EmoEngine.");
          

            while (true)
            {
                try
                {
                    engine.ProcessEvents(5);
                }
                catch (EmoEngineException e)
                {
                    Console.WriteLine("{0}", e.ToString());
                }
                catch (Exception e)
                {
                    Console.WriteLine("{0}", e.ToString());
                }
            }
            engine.Disconnect();
        }
    }
}

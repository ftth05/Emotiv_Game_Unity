using Neurorehab.Scripts.Devices.Abstracts;
using Neurorehab.Scripts.Devices.Data;
using UnityEngine;
using UnityEngine.UI;
using emotiv = Neurorehab.Scripts.Enums.Emotiv;

namespace Neurorehab.Device_Emotiv.Scripts
{
    /// <summary>
    /// Responsible for updating the Unity Emotiv data according to its <see cref="EmotivData"/>.
    /// </summary>
    public class EmotivUnity : GenericDeviceUnity
    {
        [Header("Bars")]
        public Image BarLongTermExc;
        public Image BarLowerFacePower;
        public Image BarMeditation;
        public Image BarShortTermExc;
        public Image BarSmileValue;
        public Image BarUpperFacePower;
        public Image BarBoredom;
        public Image BarEyebrowExtent;
        public Image BarFrustration;

        [Header("Boolean Values")]
        public Text BlinkValue;
        public Text BoredomValue;

        [Header("EEG Values")]
        public Text CounterValue;
        public Text EyebrowExtentValue;
        public Text EyePosValue;
        public Text F3Value;
        public Text F4Value;
        public Text F7Value;
        public Text F8Value;
        public Text FC5Value;
        public Text FC6Value;
        public Text AF4Value;
        public Text FrustrationValue;

        [Header("Position Values")]
        public Text GyroPosValue;

        [Header("Bar Values")]
        public Text LongTermExcitementValue;
        public Text LookingDownValue;
        public Text LookingLeftValue;
        public Text LookingRightValue;
        public Text LookingUpValue;
        public Text LowerFacePowerValue;
        public Text MeditationValue;
        public Text O1Value;
        public Text O2Value;
        public Text P7Value;
        public Text P8Value;
        public Text ShortTermExcitementValue;
        public Text SmileValue;
        public Text T7Value;
        public Text T8Value;
        public GameObject TrackerPoint;
        public GameObject TrackerScreen;
        public Text UpperFacePowerValue;

        private void Update()
        {
            UpdateEyeTrackGuiPanel();
            UpdateGuiValues();
        }

        /// <summary>
        /// Updates The eyetracking panel in the GUI according to its <see cref="GenericDeviceUnity.GenericDeviceData"/> data
        /// </summary>
        private void UpdateEyeTrackGuiPanel()
        {
            var screenHeight = Screen.currentResolution.height;
            var screenWidth = Screen.currentResolution.width;

            var parentHeight = TrackerScreen.GetComponent<RectTransform>().rect.height;
            var parentWidth = TrackerScreen.GetComponent<RectTransform>().rect.width;

            var eyePosX = GenericDeviceData.GetFloat(emotiv.eyelocationx.ToString());
            var eyePosY = GenericDeviceData.GetFloat(emotiv.eyelocationy.ToString());

            var newHeightPos = parentHeight * eyePosY / screenHeight;
            var newWidthPos = parentWidth * eyePosX / screenWidth;

            TrackerPoint.transform.localPosition = new Vector3(newWidthPos, newHeightPos);
        }

        /// <summary>
        /// Updates the values of each emotiv signal according to its <see cref="GenericDeviceUnity.GenericDeviceData"/> data
        /// </summary>
        private void UpdateGuiValues()
        {
            BarLongTermExc.fillAmount = GenericDeviceData.GetFloat(emotiv.longtermexcitement.ToString()) /
                                        GenericDeviceData.GetFloat("max_" + emotiv.longtermexcitement);

            BarShortTermExc.fillAmount = GenericDeviceData.GetFloat(emotiv.shorttermexcitement.ToString()) /
                                         GenericDeviceData.GetFloat("max_" + emotiv.shorttermexcitement);
            BarMeditation.fillAmount = GenericDeviceData.GetFloat(emotiv.meditation.ToString()) /
                                       GenericDeviceData.GetFloat("max_" + emotiv.meditation);
            BarFrustration.fillAmount = GenericDeviceData.GetFloat(emotiv.frustration.ToString()) /
                                        GenericDeviceData.GetFloat("max_" + emotiv.frustration);
            BarBoredom.fillAmount = GenericDeviceData.GetFloat(emotiv.boredom.ToString()) /
                                    GenericDeviceData.GetFloat("max_" + emotiv.boredom);
            BarEyebrowExtent.fillAmount = GenericDeviceData.GetFloat(emotiv.eyebrowextent.ToString()) /
                                          GenericDeviceData.GetFloat("max_" + emotiv.eyebrowextent);
            BarSmileValue.fillAmount = GenericDeviceData.GetFloat(emotiv.smileextent.ToString()) /
                                       GenericDeviceData.GetFloat("max_" + emotiv.smileextent);
            BarUpperFacePower.fillAmount = GenericDeviceData.GetFloat(emotiv.upperfacepower.ToString()) /
                                           GenericDeviceData.GetFloat("max_" + emotiv.upperfacepower);
            BarLowerFacePower.fillAmount = GenericDeviceData.GetFloat(emotiv.lowerfacepower.ToString()) /
                                           GenericDeviceData.GetFloat("max_" + emotiv.lowerfacepower);

            LongTermExcitementValue.text =
                GenericDeviceData.GetFloat(emotiv.longtermexcitement.ToString()).ToString();
            ShortTermExcitementValue.text =
                GenericDeviceData.GetFloat(emotiv.shorttermexcitement.ToString()).ToString();
            MeditationValue.text =
                GenericDeviceData.GetFloat(emotiv.meditation.ToString()).ToString();
            FrustrationValue.text =
                GenericDeviceData.GetFloat(emotiv.frustration.ToString()).ToString();
            BoredomValue.text =
                GenericDeviceData.GetFloat(emotiv.boredom.ToString()).ToString();
            EyebrowExtentValue.text =
                GenericDeviceData.GetFloat(emotiv.eyebrowextent.ToString()).ToString();
            SmileValue.text =
                GenericDeviceData.GetFloat(emotiv.smileextent.ToString()).ToString();
            UpperFacePowerValue.text =
                GenericDeviceData.GetFloat(emotiv.upperfacepower.ToString()).ToString();
            LowerFacePowerValue.text =
                GenericDeviceData.GetFloat(emotiv.lowerfacepower.ToString()).ToString();


            CounterValue.text = GenericDeviceData.GetFloat(emotiv.counter.ToString()).ToString();
            AF4Value.text = GenericDeviceData.GetFloat(emotiv.af4.ToString()).ToString();
            F3Value.text = GenericDeviceData.GetFloat(emotiv.f3.ToString()).ToString();
            F4Value.text = GenericDeviceData.GetFloat(emotiv.f4.ToString()).ToString();
            F7Value.text = GenericDeviceData.GetFloat(emotiv.f7.ToString()).ToString();
            F8Value.text = GenericDeviceData.GetFloat(emotiv.f8.ToString()).ToString();
            FC5Value.text = GenericDeviceData.GetFloat(emotiv.fc5.ToString()).ToString();
            FC6Value.text = GenericDeviceData.GetFloat(emotiv.fc6.ToString()).ToString();
            T7Value.text = GenericDeviceData.GetFloat(emotiv.t7.ToString()).ToString();
            T8Value.text = GenericDeviceData.GetFloat(emotiv.t8.ToString()).ToString();
            P7Value.text = GenericDeviceData.GetFloat(emotiv.p7.ToString()).ToString();
            P8Value.text = GenericDeviceData.GetFloat(emotiv.p8.ToString()).ToString();
            O1Value.text = GenericDeviceData.GetFloat(emotiv.o1.ToString()).ToString();
            O2Value.text = GenericDeviceData.GetFloat(emotiv.o2.ToString()).ToString();

            var gyro =
                GenericDeviceData.GetRotation(emotiv.gyro.ToString());

            GyroPosValue.text = string.Format("X: {0}; Y: {1}; Z: {2}", gyro.x, gyro.y, gyro.z);

            EyePosValue.text = string.Format("X: {0}; Y: {1};", emotiv.eyelocationy, emotiv.eyelocationy);

            BlinkValue.text = GenericDeviceData.GetBoolean(emotiv.blink.ToString()) ? "TRUE" : "FALSE";
            LookingUpValue.text = GenericDeviceData.GetBoolean(emotiv.lookingup.ToString()) ? "TRUE" : "FALSE";
            LookingDownValue.text = GenericDeviceData.GetBoolean(emotiv.lookingdown.ToString()) ? "TRUE" : "FALSE";
            LookingLeftValue.text = GenericDeviceData.GetBoolean(emotiv.lookingleft.ToString()) ? "TRUE" : "FALSE";
            LookingRightValue.text = GenericDeviceData.GetBoolean(emotiv.lookingright.ToString()) ? "TRUE" : "FALSE";
        }
    }
}
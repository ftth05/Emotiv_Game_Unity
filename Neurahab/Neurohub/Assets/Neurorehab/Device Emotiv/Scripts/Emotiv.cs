using Neurorehab.Scripts.Devices.Abstracts;
using Neurorehab.Scripts.Devices.Data;
using Neurorehab.Scripts.Enums;

namespace Neurorehab.Device_Emotiv.Scripts
{
    /// <summary>
    /// The controller of all the <see cref="EmotivData"/>. Responsible for creating, deleting and updating all the <see cref="EmotivData"/> according to what is receiving by UDP.
    /// </summary>
    public class Emotiv : GenericDeviceController
    {
        protected override void Awake()
        {
            base.Awake();

            DeviceName = Devices.emotiv.ToString();
        }
    }
}
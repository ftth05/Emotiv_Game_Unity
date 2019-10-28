using UnityEngine;
using System.Collections;
using UnityEngine.UI;
using Emotiv;

public class CameraControl : MonoBehaviour
{
    public float m_DampTime = 0.2f;    // aproximate time             
    public float m_ScreenEdgeBuffer = 4f;    // so that the tanks always on the screen       
    public float m_MinSize = 6.5f;      // min zoom in of camera            
    /*[HideInInspector]*/ public Transform[] m_Targets;

    public GameObject player;
    public Text message;
    private Vector3 offset;
    public float rotationY;
    public float sensitivity;

    private Camera m_Camera;                        
    private float m_ZoomSpeed;                      
    private Vector3 m_MoveVelocity;                 
    private Vector3 m_DesiredPosition;    // the position the camera try to reach          


    private void Awake()
    {
        m_Camera = GetComponentInChildren<Camera>();
    }


    private void FixedUpdate()
    {
        Move();
        Zoom();
    }

    void LateUpdate()
    {
        int x = 0, y = 0;
        transform.position = player.transform.position + offset;
        try
        {
            EmotivCtrl.engine.HeadsetGetGyroDelta((uint)EmotivCtrl.engineUserID, out x, out y);
        }
        catch (Emotiv.EmoEngineException e)
        {
            message.text = e.Message;
        }
        transform.RotateAround(player.transform.position, Vector3.up, x * sensitivity);
        offset = transform.position - player.transform.position;
    }

    private void Move()
    {
        FindAveragePosition();

        transform.position = Vector3.SmoothDamp(transform.position, m_DesiredPosition, ref m_MoveVelocity, m_DampTime);
    }


    private void FindAveragePosition()
    {
        Vector3 averagePos = new Vector3();
        int numTargets = 0;

        for (int i = 0; i < m_Targets.Length; i++)
        {
            if (!m_Targets[i].gameObject.activeSelf)// [i] thank is not active continue to camera position. 
                continue;

            averagePos += m_Targets[i].position;
            numTargets++;
        }

        if (numTargets > 0)
            averagePos /= numTargets;  // divide 

        averagePos.y = transform.position.y;

        m_DesiredPosition = averagePos;
    }


    private void Zoom()
    {
        float requiredSize = FindRequiredSize();
        m_Camera.orthographicSize = Mathf.SmoothDamp(m_Camera.orthographicSize, requiredSize, ref m_ZoomSpeed, m_DampTime);
    }


    private float FindRequiredSize()
    {
        Vector3 desiredLocalPos = transform.InverseTransformPoint(m_DesiredPosition);

        float size = 0f;

        for (int i = 0; i < m_Targets.Length; i++)
        {
            if (!m_Targets[i].gameObject.activeSelf)
                continue;

            Vector3 targetLocalPos = transform.InverseTransformPoint(m_Targets[i].position);

            Vector3 desiredPosToTarget = targetLocalPos - desiredLocalPos;

            size = Mathf.Max (size, Mathf.Abs (desiredPosToTarget.y));

            size = Mathf.Max (size, Mathf.Abs (desiredPosToTarget.x) / m_Camera.aspect);
        }
        
        size += m_ScreenEdgeBuffer;

        size = Mathf.Max(size, m_MinSize);

        return size;
    }


    public void SetStartPositionAndSize()
    {
        FindAveragePosition();

        transform.position = m_DesiredPosition;

        m_Camera.orthographicSize = FindRequiredSize();
    }
}
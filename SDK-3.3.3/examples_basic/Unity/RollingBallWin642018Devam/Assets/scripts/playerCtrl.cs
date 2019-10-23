using UnityEngine;
using UnityEngine.UI;
using System.Collections;
using Emotiv;

public class playerCtrl : MonoBehaviour {

    /*
    public int m_PlayerNumber = 1;         
    public float m_Speed = 12f;            
    public float m_TurnSpeed = 180f;       
    public AudioSource m_MovementAudio;    
    public AudioClip m_EngineIdling;       
    public AudioClip m_EngineDriving;      
    public float m_PitchRange = 0.2f;


    private string m_MovementAxisName;     
    private string m_TurnAxisName;         
    private Rigidbody m_Rigidbody;         
    private float m_MovementInputValue;    
    private float m_TurnInputValue;        
    private float m_OriginalPitch;    
               
         */





    public Text countText;
    public float speed = 12f;
    public float jumpConstant;
	public Camera cam;
	private Rigidbody rb;
	private int count;
    private float moveZ;

    public Text logText;
	EmoEngine engine;

	/*
	 * This method handle the EmoEngine update event, 
	 * if the EmoState has the PUSH action, 
	 * a force is applied to the ball in the direction of the camara
	*/
	void engine_EmoStateUpdated(object sender, EmoStateUpdatedEventArgs e)
	{
		EmoState es = e.emoState;
		if (e.userId != 0) 
			return;
		Debug.Log ("Corrent action: " + es.MentalCommandGetCurrentAction().ToString());
		if (es.MentalCommandGetCurrentAction () == EdkDll.IEE_MentalCommandAction_t.MC_PUSH) {
            Vector3 movement = new Vector3(cam.transform.forward.x, cam.transform.forward.y, cam.transform.forward.z);
            
            rb.AddForce(movement * speed);
			Debug.Log ("Push");
		}
	}

	/*
	 * Set the handler for update event
	*/
    void Start () {
        rb = GetComponent<Rigidbody>();
		EmoEngine.Instance.EmoStateUpdated += new EmoEngine.EmoStateUpdatedEventHandler(engine_EmoStateUpdated);
	}

	/*
	 * You can also use the up arrow key to move the ball.
	*/
    void FixedUpdate() {
		moveZ = Input.GetAxis ("Vertical");
		Vector3 movement = new Vector3(cam.transform.forward.x, cam.transform.forward.y, cam.transform.forward.z);
		rb.AddForce(movement * speed * moveZ);
        
        Move(); // Bu tankin ileri gitme problemini cozdu

    }

    /*
	 * If the ball collides with an item, it will be disabled 
	 * and a counter will be increased
	*/

    //From TankMovement
    private void Move()
    {
        // Adjust the position of the tank based on the player's input.
        Vector3 movement = transform.forward * moveZ * speed * Time.deltaTime;

        rb.MovePosition(rb.position + movement);
    }




      void OnTriggerEnter(Collider other)
    {
		if (other.CompareTag ("pickup")) {
			other.gameObject.SetActive (false);
			count++;
		}
    }

	void SetCountText(){
		countText.text = "Count: " + count;
	}
}

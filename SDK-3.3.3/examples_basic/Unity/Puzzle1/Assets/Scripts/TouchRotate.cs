using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class TouchRotate : MonoBehaviour

{
    // with this code I can rotate the picture via mouse click when the status is neutral.
    public static int action = 0;


    private void OnMouseDown()
    {
        if (action == 0) // simdilik push ile calisacak.
        {
            if (!GameControl.youWin)
                transform.Rotate(0f, 0f, 90f);

        }

        else
        {
            void OnMouseEnter()
            {

                transform.localScale += new Vector3(-0.15f, -0.15f, 0);
            }
            void OnMouseExit()
            {
                transform.localScale += new Vector3(0.15f, 0.15f, 0);
            }

        }

    }

    /*
    public static int action = 0;
    public static float speed = 7.0f / 8.0f;
    // Use this for initialization
    void Start () {
    
    }
    
    // Update is called once per frame
    void Update () {
        if (action == 0)
        {
            if (!GameControl.youWin)
            //if (transform.localPosition.z > 0)
            {
                transform.Rotate(0f, 0f, 90f);
                //transform.Translate(0, 0, -speed * Time.deltaTime * 10);
            }
            else
            {
                transform.localPosition = new Vector3(0, 0, 0);
            }
        }
        else
        {
            if (transform.localPosition.z < 7)
            {
                transform.Translate(0, 0, speed * Time.deltaTime);
            }
            else
            {
                transform.localPosition = new Vector3(0, 0, 7);
            }
        }
    }

    */

}

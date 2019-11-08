using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class TouchRotate : MonoBehaviour
{
    private void OnMouseEnter()
    {
       
        transform.localScale += new Vector3(-0.15f, -0.15f,0);
    }
    private void OnMouseExit()
    {
        transform.localScale += new Vector3(0.15f, 0.15f, 0);
    }
    private void OnMouseDown()
    {
        if (!GameControl.youWin)
            transform.Rotate(0f, 0f, 90f);
    }
}

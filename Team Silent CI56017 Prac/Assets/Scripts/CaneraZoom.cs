using UnityEngine;

/// <summary>
/// Zooms the camera
/// Ben Thorpe
/// </summary>
public class CaneraZoom : MonoBehaviour
{

    private Camera mainCamera;
    //The default Z position, used for zoom value
    private float defaultY;
    //Informs the collision balancer of the change 
    private CollisionBalance collisionBalance;

    /// <summary>
    /// Used to inilizased varailbes
    /// </summary>    
    void Start()
    {
        mainCamera = Camera.main;
        collisionBalance = GameObject.FindGameObjectWithTag(GlobalTags.BALANCE)
            .GetComponent<CollisionBalance>();
        defaultY = mainCamera.transform.position.y;
    }

    /// <summary>
    /// Change the zoom of the camera
    /// </summary>
    /// <param name="a_value"></param>
    public void ChangeZoom(float a_value)
    {
        //Update position
        Vector3 pos = mainCamera.transform.position;
        pos.y = defaultY * a_value;
        mainCamera.transform.position = pos;
        //Update the colliders
        collisionBalance.AlterCollision(a_value);
    }
}

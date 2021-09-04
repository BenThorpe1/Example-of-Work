using UnityEngine;

/// <summary>
/// Controlls the camera
/// Ben Thorpe
/// </summary>
public class CamerFollow : MonoBehaviour
{

    /// <summary>
    /// Moves the camera
    /// </summary>
    /// <param name="a_pos"></param>
    public void CenterCamera(Vector3 a_pos)
    {
        Vector3 target = new Vector3(a_pos.x, transform.position.y, a_pos.z);
        transform.position = target;
    }
}

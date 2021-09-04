using UnityEngine;

/// <summary>
/// Testing the colliders with ray casting
/// Ben Thorpe
/// </summary>
public class RayTest : MonoBehaviour
{
    Vector3 point;
    bool hitted = false;

    // Update is called once per frame
    void Update()
    {
        //Check if something is within path
        Ray ray = new Ray(transform.position, Vector3.forward);
        RaycastHit hit;
        Debug.DrawRay(ray.origin, ray.direction * 1000, Color.cyan, 2f);
        if (Physics.Raycast(ray, out hit))
        {
            Debug.Log(hit.transform.name);
            point = hit.point;
            hitted = true;
        }
    }

    /// <summary>
    /// Draw a shpere at point of contact
    /// </summary>
    private void OnDrawGizmosSelected()
    {
        if (hitted)
            {
            Gizmos.color = Color.magenta;
            Gizmos.DrawSphere(point, 1);
        }
    }
}

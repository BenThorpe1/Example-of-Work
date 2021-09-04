using UnityEngine;

/// <summary>
/// Repersent the sight of AI units
/// Ben Thorpe
/// </summary>
public class AISight : MonoBehaviour
{
    private Enemy attachedAI;
    private const float RAY_Y_POS = 0.7f;
    private PlayerController player;


    /// <summary>
    /// Used to inilize
    /// </summary>
    private void Start()
    {
        attachedAI = transform.parent.GetComponent<Enemy>();
        player = GameObject.FindGameObjectWithTag(GlobalTags.PLAYER)
            .GetComponent<PlayerController>();
    }

    /// <summary>
    /// Check if player is in range of sight
    /// </summary>
    /// <param name="other"></param>
    private void OnTriggerStay(Collider other)
    {
        if (other.tag == GlobalTags.PLAYER)
        {
            //Check if player is actual visible
            Ray ray = new Ray(transform.position, (other.transform.position - transform.position).normalized);
            ray.origin = new Vector3(ray.origin.x, RAY_Y_POS, ray.origin.z);
            RaycastHit hit;
            if (Physics.Raycast(ray, out hit))
            {
                if (hit.transform.tag == GlobalTags.PLAYER)
                {
                    if (!player.CheckHidden())
                    {
                        attachedAI.SpotPlayer(other.transform.position);
                    }
                }
            }

        }
    }
}

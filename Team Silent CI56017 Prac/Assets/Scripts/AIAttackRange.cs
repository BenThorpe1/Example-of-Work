using System.Collections;
using System.Collections.Generic;
using UnityEngine;

/// <summary>
/// Repsents the attack range of the AI
/// Ben Thorpe
/// </summary>
public class AIAttackRange : MonoBehaviour
{
    private Enemy attachedAI;
    private PlayerController player;

    /// <summary>
    /// Used to inilize
    /// </summary>
    private void Start()
    {
        attachedAI = transform.parent.GetComponent<Enemy>();
        player = GameObject.FindGameObjectWithTag(GlobalTags.PLAYER).GetComponent<PlayerController>();
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
            RaycastHit hit;
            if (Physics.Raycast(ray, out hit))
            {

                if (hit.transform.tag == GlobalTags.PLAYER)
                {
                    if (!player.CheckHidden())
                    {
                        attachedAI.PlayerInRange();
                    }
                }
            }

        }
    }
}

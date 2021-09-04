using UnityEngine;
using System.Collections.Generic;

/// <summary>
/// Repersnt the range in which the player can attack
/// Ben Thorpe
/// </summary>
public class PlayerAttackRange : MonoBehaviour
{
    private List<Transform> collidingObjects;

    /// <summary>
    /// Used to inilize
    /// </summary>
    private void Start()
    {
        collidingObjects = new List<Transform>();
    }

    /// <summary>
    /// Check if enemies are still alive
    /// On trigger exit isn't called if an object is destored
    /// </summary>
    private void Update()
    {
        for (int i = 0; i < collidingObjects.Count; i++)
        {
            if (collidingObjects[i] == null)
            {
                collidingObjects.RemoveAt(i);
                i--;
            }
        }
    }
    /// <summary>
    /// Keep track of all colliding objects
    /// Checks if player is in bushes
    /// </summary>
    /// <param name="other"></param>
    private void OnTriggerEnter(Collider other)
    {
        if (other.tag == GlobalTags.ENEMY)
        {
            collidingObjects.Add(other.transform);
        }
    }

    /// <summary>
    ///  Keep track of all colliding objects
    /// Checks if player moved out of the bushes
    /// </summary>
    /// <param name="other"></param>
    private void OnTriggerExit(Collider other)
    {
        if (other.tag == GlobalTags.ENEMY)
        {
            collidingObjects.Remove(other.transform);
        }
    }

    /// <summary>
    /// Returns all enemies in range
    /// </summary>
    /// <returns></returns>
    public Transform[]GetEnemyInRange()
    {
        return collidingObjects.ToArray();
    }
}

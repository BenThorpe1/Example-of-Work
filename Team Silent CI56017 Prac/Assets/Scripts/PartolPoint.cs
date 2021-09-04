using System.Collections;
using System.Collections.Generic;
using UnityEngine;

/// <summary>
/// Draws a line around the partol point
/// Ben Thorpe
/// </summary>
public class PartolPoint : MonoBehaviour
{
    void OnDrawGizmosSelected()
    {
        transform.parent.GetComponent<Pathing>().DrawPath();
    }
}

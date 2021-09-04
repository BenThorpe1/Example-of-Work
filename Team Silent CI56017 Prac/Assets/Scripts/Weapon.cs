using System.Collections;
using System.Collections.Generic;
using UnityEngine;

/// <summary>
/// Repsernts a pickable item that the player can use to kill enemy
/// Ben Thorpe
/// </summary>
public class Weapon : Item
{
    /// <summary>
    /// Kill a nearby enemy
    /// </summary>
    /// <returns></returns>
    public override bool ItemEffect()
    {
        //Get Player
        PlayerController player = GameObject.FindGameObjectWithTag(GlobalTags.PLAYER)
            .GetComponent<PlayerController>();
        return player.Attack();
    }
}

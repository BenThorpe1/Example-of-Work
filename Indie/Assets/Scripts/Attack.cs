using UnityEngine;

/// <summary>
/// Repersent an attack action
/// </summary>
[System.Serializable]
[CreateAssetMenu(fileName = "Attack", menuName = "ScriptableObjects/Attack")]
public class Attack : ScriptableObject
{
    //Where to use str or int stats
    [SerializeField]
    private ATTACK_TYPE attackType;
    //Power of attack
    [SerializeField]
    private float might;
    //Base hit rate of atack
    [SerializeField]
    private float hit;
    //Range in tiles
    [SerializeField]
    private float range;

    //Getters
    public float GetMight() { return might; }
    public float GetHit() { return hit; }
    public float GetRange() { return range; }
    public ATTACK_TYPE GetAttackType() { return attackType; }

}

/// <summary>
/// Repsersents the types of attacks
/// </summary>
[System.Serializable]
public enum ATTACK_TYPE
{
    //Physic uses ATT and PRT
    PHY = 0,
    //Magic uses MAG and RSL
    MAG = 1,
    COUNT = 2
}


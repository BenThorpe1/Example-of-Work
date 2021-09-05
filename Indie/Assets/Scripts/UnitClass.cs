using UnityEngine;

/// <summary>
/// Repersents a unit class
/// </summary>
[System.Serializable]
[CreateAssetMenu(fileName = "Class", menuName = "ScriptableObjects/Class")]
/// <summary>
/// Repesents a base of class
/// </summary>
public class UnitClass : ScriptableObject
{
    [SerializeField]
    private string className;
    [SerializeField]
    private BaseStats baseStats;

    //Getters
    public string GetClassName() { return className; }
    public BaseStats GetBaseStats() { return baseStats; }
}

/// <summary>
/// Repersents class ID
/// </summary>
[System.Serializable]
public enum CLASS_ID
{
WARRIOR =0,
COUNT =1
};

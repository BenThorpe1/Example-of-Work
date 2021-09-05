using UnityEngine;

/// <summary>
/// Contians data for all classes
/// </summary>
[CreateAssetMenu(fileName = "ClassList", menuName = "ScriptableObjects/ClassList")]
public class ClassType : ScriptableObject
{
    //List of all avialbe classes
    [SerializeField]
    private UnitClass[] classData;

    //Getters
    public string GetClassName(int a_i) { return classData[a_i].GetClassName(); }
    public BaseStats GetClassBase(int a_i) { return classData[a_i].GetBaseStats(); }
}



using UnityEngine;

/// <summary>
/// Controls how AI acts //Not independed
/// </summary>
public class AICOntroller : MonoBehaviour
{
    [SerializeField]
    private UnitManager unitManager;

    /// <summary>
    /// Start the AI turn
    /// </summary>
    public void StartTurn()
    {
        unitManager.EndTurn();
    }
}

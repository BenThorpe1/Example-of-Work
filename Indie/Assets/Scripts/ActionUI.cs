using UnityEngine;
using UnityEngine.UI;

/// <summary>
/// Controlls the option when selecting what to do with a unit
/// </summary>
public class ActionUI : MonoBehaviour
{
    //Various buttons repersents options
    [SerializeField]
    private Button moveButton;
    [SerializeField]
    private Button attackButton;
    [SerializeField]
    private Button waitButton;

    /// <summary>
    /// Set the active of the object
    /// </summary>
    /// <param name="a_true"></param>
    public void SetActive(bool a_true)
    {
        gameObject.SetActive(a_true);
    }

    /// <summary>
    /// Allow options based on unit abilities
    /// </summary>
    /// <param name="a_moved"></param>
    /// <param name="a_acted"></param>
    public void SelectUnit(bool a_moved, bool a_acted)
    {
        moveButton.interactable = !a_moved;
        attackButton.interactable = !a_acted;
    }

}

using UnityEngine;
using UnityEngine.UI;

/// <summary>
/// Controls the display for unit when hovered over
/// </summary>
public class UnitHoverDisplay : MonoBehaviour
{
    #region Serialized Text fields
    [SerializeField]
    private Text unitHoverName;
    [SerializeField]
    private Text unitHoverCurrentHP;
    [SerializeField]
    private Text unitHoverMaxHP;
    [SerializeField]
    private Image hoverBox;
    [SerializeField]
    private Text unitHoverCurrentMP;
    [SerializeField]
    private Text unitHoverMaxMP;
    [SerializeField]
    private Text className;
    #endregion

    [SerializeField]
    private ClassType classTypes;

    //Colors for the display box
    [SerializeField]
    private Color playerColour;
    [SerializeField]
    private Color enemyColour;
    [SerializeField]
    private Color neutralColour;

    /// <summary>
    /// Display unit details
    /// </summary>
    public void DisplayUnit(string a_name, float a_currentHP,
        float a_maxHP, UNIT_ALIGMENT a_aligment, float a_currentMP,
        float a_maxMP, CLASS_ID a_class)
    {
        gameObject.SetActive(true);
        unitHoverName.text = a_name;
        unitHoverCurrentHP.text = a_currentHP.ToString();
        unitHoverMaxHP.text = a_maxHP.ToString();
        unitHoverCurrentMP.text = a_currentMP.ToString();
        unitHoverMaxMP.text = a_maxMP.ToString();
        className.text = classTypes.GetClassName((int)a_class);

        //Update box colour
        switch (a_aligment)
        {
            case UNIT_ALIGMENT.PLAYER:
                {
                    hoverBox.color = playerColour;
                    break;
                }
            case UNIT_ALIGMENT.ENEMY:
                {
                    hoverBox.color = enemyColour;
                    break;
                }
            case UNIT_ALIGMENT.NEUTRAL:
                {
                    hoverBox.color = neutralColour;
                    break;
                }
            default:
                {
                    Debug.Log("Default unit aligment");
                    break;
                }
        }

    }

    /// <summary>
    /// Toggle display
    /// </summary>
    public void Display(bool a_active)
    {
        gameObject.SetActive(a_active);
    }
}

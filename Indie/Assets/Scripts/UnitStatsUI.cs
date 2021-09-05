using UnityEngine;
using UnityEngine.UI;

/// <summary>
/// Displays units stats
/// </summary>
public class UnitStatsUI : MonoBehaviour
{

    #region Base info
    //SerializeField texts
    [SerializeField]
    private Text nameText;
    [SerializeField]
    private Text classText;
    [SerializeField]
    private Text levelText;
    [SerializeField]
    private Text currentHPText;
    [SerializeField]
    private Text maxHPText;
    [SerializeField]
    private Text currentMPText;
    [SerializeField]
    private Text maxMPText;
    [SerializeField]
    private Text expText;
    [SerializeField]
    private Text moveText;
    [SerializeField]
    private ClassType classTypes;

    #endregion

    #region Base stats
    [SerializeField]
    private Text strText;
    [SerializeField]
    private Text intText;
    [SerializeField]
    private Text dexText;
    [SerializeField]
    private Text defText;
    [SerializeField]
    private Text spdText;
    [SerializeField]
    private Text resText;
    #endregion

    #region CalulateStats
    [SerializeField]
    private Text atkText;
    [SerializeField]
    private Text magText;
    [SerializeField]
    private Text prtText;
    [SerializeField]
    private Text rslText;
    [SerializeField]
    private Text hitText;
    [SerializeField]
    private Text avdText;
    #endregion

    /// <summary>
    /// Display unit stats
    /// </summary>
    /// <param name="a_unit"></param>
    public void DisplayStats(Unit a_unit)
    {
        gameObject.SetActive(true);
        FillPlayerInfo(a_unit);
        FillBaseStats(a_unit);
        FillCalulcatedStats(a_unit);
    }

    /// <summary>
    /// Hides the ui;
    /// </summary>
    public void Close()
    {
        gameObject.SetActive(false);
    }

    /// <summary>
    /// Fill in players basic info
    /// </summary>
    /// <param name="a_unit"></param>
    private void FillPlayerInfo(Unit a_unit)
    {
        nameText.text = a_unit.GetName();
        classText.text = classTypes.GetClassName((int)a_unit.GetClassID());
        levelText.text = a_unit.GetLevel().ToString();
        currentHPText.text = a_unit.GetCurrentHP().ToString();
        maxHPText.text = a_unit.GetMaxHP().ToString();
        currentMPText.text = a_unit.GetCurrentMP().ToString();
        maxMPText.text = a_unit.GetMaxMP().ToString();
        expText.text = a_unit.GetEXP().ToString();
        moveText.text = a_unit.GetMovement().ToString();
    }

    /// <summary>
    /// Fills in players base stats
    /// </summary>
    /// <param name="unit"></param>
    private void FillBaseStats(Unit a_unit)
    {
        strText.text = a_unit.GetSTR().ToString();
        intText.text = a_unit.GetINT().ToString();
        dexText.text = a_unit.GetDEX().ToString();
        defText.text = a_unit.GetDEF().ToString();
        spdText.text = a_unit.GetSPD().ToString();
        resText.text = a_unit.GetRES().ToString();
    }

    /// <summary>
    /// Fill in players caluculated stats
    /// </summary>
    /// <param name="a_unit"></param>
    private void FillCalulcatedStats(Unit a_unit)
    {
        atkText.text = a_unit.GetATK().ToString();
        magText.text = a_unit.GetMAG().ToString();
        prtText.text = a_unit.GetPRT().ToString();
        rslText.text = a_unit.GetRSL().ToString();
        hitText.text = a_unit.GetHIT().ToString();
        avdText.text = a_unit.GetAVD().ToString();
    }
}

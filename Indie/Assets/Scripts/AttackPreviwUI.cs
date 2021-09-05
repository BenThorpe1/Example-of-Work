using System.Collections;
using UnityEngine;
using UnityEngine.UI;

/// <summary>
/// Shows preview of battle forcast
/// </summary>
public class AttackPreviwUI : MonoBehaviour
{
    //Text fields to display
    [SerializeField]
    private Text hitText;
    [SerializeField]
    private Text damageText;
    [SerializeField]
    private Text missText;
    //How long the miss text appears for
    private const float MISS_DELAY = 0.5f;

    /// <summary>
    /// Set the canvas to display or not
    /// </summary>
    /// <param name="a_true"></param>
    public void Display(bool a_true)
    {
        gameObject.SetActive(a_true);
    }

    /// <summary>
    /// Displays the preview
    /// </summary>
    /// <param name="a_hit"></param>
    /// <param name="a_damage"></param>
    public void DisplayStats(float a_hit, float a_damage)
    {
        Display(true);
        hitText.text = a_hit.ToString();
        damageText.text = a_damage.ToString();
    }

    /// <summary>
    /// Display the miss text
    /// </summary>
    public void Miss()
    {
        StartCoroutine(DisplayMiss());
    }

    /// <summary>
    /// Display miss text for set amount of time
    /// </summary>
    /// <returns></returns>
    private IEnumerator DisplayMiss()
    {
        missText.gameObject.SetActive(true);
        yield return new WaitForSeconds(MISS_DELAY);
        missText.gameObject.SetActive(false);
    }

}

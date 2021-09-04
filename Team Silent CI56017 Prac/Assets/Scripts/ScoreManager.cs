using UnityEngine;
using UnityEngine.UI;

/// <summary>
/// Managers the player's score
/// </summary>
public class ScoreManager : MonoBehaviour
{

    [SerializeField]
    private Text displayText;
    private float score = 0;

    /// <summary>
    /// Increase the player's score
    /// </summary>
    /// <param name="a_value"></param>
    public void AddScore(float a_value)
    {
        score += a_value;
        displayText.text = score.ToString();
    }
  
}

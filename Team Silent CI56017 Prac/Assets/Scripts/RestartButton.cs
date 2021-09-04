using UnityEngine;
using UnityEngine.SceneManagement;

/// <summary>
/// Restart the game
/// Ben Thorpe
/// </summary>
public class RestartButton : MonoBehaviour
{
    public void Restart()
    {
        SceneManager.LoadScene(0);
        Time.timeScale = 1;
    }

}

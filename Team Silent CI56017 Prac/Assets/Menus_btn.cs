using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using UnityEngine.SceneManagement;

/// <summary>
/// Khaled Gholami
/// </summary>
public class Menus_btn : MonoBehaviour
{
    public static bool GameisPaused = false; // If the game is pause or not.
    public GameObject PauseMenuUI; // Reference for Pause Menu UI in UNIT
    public GameObject ZoomScale; // Reference for hide scaler when pause is active.


    // Update is called once per frame
    void Update()
    {
        if (Input.GetKeyDown(KeyCode.Escape) || Input.GetKeyDown(KeyCode.P))
        {
            if (GameisPaused)
            {
                Resume(); // resume funtion
            } else
            {
                Pause(); // pause function
            }
        }
    }

    void Pause()
    {
        PauseMenuUI.SetActive(true); // turn Pausemenu UI on when pause is active.
        ZoomScale.SetActive(false); // This can be see and control while pause is active so this function will disable them
        Time.timeScale = 0f; // Freeze anything that moved.
        GameisPaused = true; // pause is active

    }

   public void Resume()
    {
        PauseMenuUI.SetActive(false); // turn Pausemenu UI off when pause is inactive.
        ZoomScale.SetActive(true); // When Unpause or resume, this is disable so this function will enable them.
        Time.timeScale = 1f; // Unfreeze anything that moved.
        GameisPaused = false; // pause is inactive
    }

    public void Menu()
    {
        Debug.Log("Open menu...");
        //SceneManager.LoadScene("WHATEVER THE SCENE NAME IS"); // Once Menu scene is created, then remove this comment and rename the scene.
        Time.timeScale = 1; // Unpause when move into new scene.
    }

    public void Quit()
    {
        Debug.Log("Closing Gam e...");
        Application.Quit();
    }

    /// <summary>
    /// Allowed pasuing to be done with a button
    /// Ben
    /// </summary>
    public void OnButtonClick()
    {
        if (GameisPaused)
        {
            Resume(); // resume funtion
        }
        else
        {
            Pause(); // pause function
        }
    }
}

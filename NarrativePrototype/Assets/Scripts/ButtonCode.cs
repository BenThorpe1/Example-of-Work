using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using UnityEngine.UI;
using UnityEngine.SceneManagement;

/// <summary>
/// Contains all code used by buttons
/// </summary>
public class ButtonCode : MonoBehaviour
{
    /// <summary>
    /// Changes the scene
    /// </summary>
    /// <param name="a_value"></param>
    public void ChangeScene(int a_value)
    {
        SceneManager.LoadScene(a_value);
    }

    /// <summary>
    /// Closes the application
    /// </summary>
    public void EndApp()
    {
    #if UNITY_EDITOR
        UnityEditor.EditorApplication.isPlaying = false;
    #else
            Application.Quit();
    #endif
    }
}

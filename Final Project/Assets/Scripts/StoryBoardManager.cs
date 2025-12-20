using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using UnityEngine.Playables;

public class StoryBoardManager : MonoBehaviour
{
    public List<PlayableDirector> directors;

    private void Update()
    {
        // Check for key presses to play specific storyboards
        if (Input.GetKeyDown(KeyCode.Alpha1))
        {
            PlayStoryBoard(0);
        }
        else if (Input.GetKeyDown(KeyCode.Alpha2))
        {
            PlayStoryBoard(1);
        }
        else if (Input.GetKeyDown(KeyCode.Alpha3))
        {
            PlayStoryBoard(2);
        }
        else if (Input.GetKeyDown(KeyCode.Alpha4))
        {
            PlayStoryBoard(3);
        }
        else if (Input.GetKeyDown(KeyCode.Alpha5))
        {
            PlayStoryBoard(4);
        }
        else if (Input.GetKeyDown(KeyCode.Alpha6))
        {
            PlayStoryBoard(5);
        }
    }

    private void PlayStoryBoard(int index)
    {
        if (index < 0 || index >= directors.Count)
        {
            Debug.LogError("Index out of range for PlayableDirector list.");
            return;
        }
        PlayableDirector director = directors[index];
        if (director != null)
        {
            // stop and reset all the other directors
            foreach (var dir in directors)
            {
                if (dir != null && dir != director)
                {
                    dir.Stop();
                }
            }

            director.Play();
        }
        else
        {
            Debug.LogError("PlayableDirector at index " + index + " is null.");
        }
    }
}

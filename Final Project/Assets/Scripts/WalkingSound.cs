using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class FootstepAudio : MonoBehaviour
{
    public AudioClip[] footstepClips; // 可丟進不同腳步聲
    public AudioSource audioSource;   // 推薦直接拖角色身上的 AudioSource

    // 這個 function 會被 Animation Event 叫用
    public void PlayFootstepLeft()
    {
        if (footstepClips.Length == 0) return;
        // 隨機選一個腳步聲
        //int index = Random.Range(0, footstepClips.Length);
        audioSource.PlayOneShot(footstepClips[0]);
    }
    public void PlayFootstepRight()
    {
        if (footstepClips.Length == 0) return;
        // 隨機選一個腳步聲
        //int index = Random.Range(0, footstepClips.Length);
        audioSource.PlayOneShot(footstepClips[1]);
    }

    public void PlayRunFootstepLeft()
    {
        if(footstepClips.Length == 0) return;
        audioSource.PlayOneShot(footstepClips[2]);
    }

    public void PlayRunFootstepRight()
    {
        if (footstepClips.Length == 0) return;
        audioSource.PlayOneShot(footstepClips[3]);
    }

    public void PlayShakeLegLeft()
    {
        if (footstepClips.Length == 0) return;
        audioSource.PlayOneShot(footstepClips[4]);
    }
}


using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class FogController : MonoBehaviour
{
    // Adjust the fog density in render settings
    public void SetFogDensity(float density)
    {
        RenderSettings.fogDensity = density;
    }

    public void SetFogDensity2ZeroGradually(float duration)
    {
        StartCoroutine(SetFogDensityOverTime(0f, duration));
    }

    private IEnumerator SetFogDensityOverTime(float targetDensity, float duration)
    {
        float startDensity = RenderSettings.fogDensity;
        float elapsed = 0f;
        while (elapsed < duration)
        {
            elapsed += Time.deltaTime;
            float t = elapsed / duration;
            RenderSettings.fogDensity = Mathf.Lerp(startDensity, targetDensity, t);
            yield return null;
        }
        // Ensure it ends at the target density
        RenderSettings.fogDensity = targetDensity;
    }
}

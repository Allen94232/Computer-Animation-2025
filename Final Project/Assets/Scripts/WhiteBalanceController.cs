using System.Collections;
using UnityEngine;
using UnityEngine.Rendering;
using UnityEngine.Rendering.Universal;

public class WhiteBalanceController : MonoBehaviour
{
    public float duration = 2f; // 預設持續時間
    public Volume volume;
    private WhiteBalance whiteBalance;

    private void Start()
    {
        if (volume != null && volume.profile.TryGet(out whiteBalance))
        {
        }
        else
        {
            Debug.LogWarning("White Balance override not found in volume profile.");
        }
    }

    public void ChangeTemperature(float targetTemperature)
    {
        if (whiteBalance != null)
        {
            StartCoroutine(LerpTemperature(whiteBalance.temperature.value, targetTemperature, duration));
        }
    }

    private IEnumerator LerpTemperature(float start, float end, float duration)
    {
        this.duration = duration; // 更新持續時間

        float elapsed = 0f;
        while (elapsed < duration)
        {
            elapsed += Time.deltaTime;
            float t = Mathf.Clamp01(elapsed / duration);
            whiteBalance.temperature.value = Mathf.Lerp(start, end, t);
            yield return null;
        }

        whiteBalance.temperature.value = end; // 保證最後是精確的目標值
    }
}

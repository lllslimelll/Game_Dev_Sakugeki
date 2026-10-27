// VS/PS共通
#include "../../Common/VertexToPixelHeader.hlsli"

// IN
#define PS_INPUT VertexToPixelLit

// PS
#include "../../Common/Pixel/PixelShader3DHeader.hlsli"

float4 main(PS_INPUT PSInput) : SV_TARGET0
{
	// テクスチャーの色を取得
    float4 color = diffuseMapTexture.Sample(diffuseMapSampler, PSInput.uv);
    if (color.a < 0.01f)
    {
        discard;
    }
    
    // デフューズを乗算合成
    color *= PSInput.diffuse;

    // ライティング設定
    // ランバート反射
    float lightPow = saturate(dot(PSInput.normal, -PSInput.lightDir));
    
    // 結果を乗算合成
    color.rgb *= lightPow;
    
    return color;
}

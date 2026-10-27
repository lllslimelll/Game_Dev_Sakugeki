// VS/PS共通
#include "../../Common/VertexToPixelHeader.hlsli"

// IN
#include "../../Common/Vertex/VertexInputType.hlsli"
#define VERTEX_INPUT DX_MV1_VERTEX_TYPE_NMAP_1FRAME

// OUT
#define VS_OUTPUT VertexToPixelLit
#include "../../Common/Vertex/VertexShader3DHeader.hlsli"

cbuffer cbParam : register(b7)
{
    float4 g_camera_pos; // カメラ座標

    float g_fog_start;
    float g_fog_end;
    float dummy2;
    float dummy3;
}

VS_OUTPUT main(VS_INPUT VSInput)
{
	VS_OUTPUT ret;

	// 頂点座標変換 +++++++++++++++++++++++++++++++++++++( 開始 )
    float4 lLocalPosition;
    float4 lWorldPosition;
    float4 lViewPosition;

	// float3 → float4
    lLocalPosition.xyz = VSInput.pos;
    lLocalPosition.w = 1.0f;

	// ローカル座標をワールド座標に変換(剛体)
    lWorldPosition.w = 1.0f;
    lWorldPosition.xyz = mul(lLocalPosition, g_base.localWorldMatrix);
    
	// ワールド座標をビュー座標に変換
    lViewPosition.w = 1.0f;
    lViewPosition.xyz = mul(lWorldPosition, g_base.viewMatrix);
    ret.vwPos.xyz = lViewPosition.xyz;

	// ビュー座標を射影座標に変換
    ret.svPos = mul(lViewPosition, g_base.projectionMatrix);

	// 頂点座標変換 +++++++++++++++++++++++++++++++++++++( 終了 )
	
	// ピクセルシェーダへ引継&初期化 ===============( 開始 )
	// UV座標
    ret.uv.x = VSInput.uv0.x;
    ret.uv.y = VSInput.uv0.y;
    ret.worldPos = lWorldPosition.xyz;
	// 法線をローカル空間からワールド空間へ変換
    ret.normal = normalize(mul(VSInput.norm, (float3x3) g_base.localWorldMatrix));
	// ディフューズカラー
    ret.diffuse = VSInput.diffuse;
	// ライト方向(ローカル)
    ret.lightDir = float3(0.3f, -0.7f, 0.8f);
    // ライトの強さ
    ret.lightPower;
    // フォグの強さ(0.0:フォグが濃い、1.0:フォグが薄い)
    ret.fogFactor =
		clamp((g_fog_end - length(lWorldPosition.xyz - g_camera_pos.xyz)) / (g_fog_end - g_fog_start), 0.0, 1.0);
	// =============================================( 終了 )

	// 出力パラメータを返す
    return ret;
}
#pragma once

// デバッグ機能を使うかどうか
#ifdef _DEBUG 
// デバッグビルド
#define USE_DEBUG_TOOLS 1
#else
// リリースビルド
#define USE_DEBUG_TOOLS 1
#endif
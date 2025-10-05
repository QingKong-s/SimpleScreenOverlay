#include "pch.h"


HRESULT CGrTextCache::CreateGeometryRealization(ID2D1DeviceContext1* pDC,
    const D2D1::Matrix3x2F* pTransform)
{
    float xDpi, yDpi;
    pDC->GetDpi(&xDpi, &yDpi);
    SafeRelease(pGr);
    ID2D1PathGeometry1* pGeo;
    auto hr = eck::GetTextLayoutPathGeometry(pTl, 0.f, 0.f, pGeo, xDpi);
    if (FAILED(hr))
        return hr;
    hr = pDC->CreateFilledGeometryRealization(pGeo,
        D2D1::ComputeFlatteningTolerance(
            pTransform ? *pTransform : D2D1::Matrix3x2F::Identity(),
            xDpi, yDpi),
        &pGr);
    pGeo->Release();
    return hr;
}

HRESULT CGrTextCacheWithBk::CreateGeometryRealization(ID2D1DeviceContext1* pDC,
    const D2D1::Matrix3x2F* pTransform)
{
    SafeRelease(pGrBk);
    auto hr = __super::CreateGeometryRealization(pDC, pTransform);
    if (FAILED(hr))
        return hr;
    DWRITE_TEXT_METRICS tm;
    pTl->GetMetrics(&tm);
    //---取行信息
    UINT32 cLine = tm.lineCount;
    std::vector<DWRITE_LINE_METRICS> vBuf(cLine);
    hr = pTl->GetLineMetrics(vBuf.data(), cLine, &cLine);
    if (FAILED(hr))
        return hr;
    //---计算HitTestTextRange缓冲区初始值
    const auto cHitTestBuf = tm.lineCount * tm.maxBidiReorderingDepth;
    std::vector<DWRITE_HIT_TEST_METRICS> vHitTestBuf(cHitTestBuf);
    //---取每一行矩形
    UINT32 idxChar{};
    UINT32 cActual{};
    ComPtr<ID2D1PathGeometry1> pGeo;
    eck::g_pD2dFactory->CreatePathGeometry(&pGeo);
    ComPtr<ID2D1GeometrySink> pSink;
    pGeo->Open(&pSink);
    D2D1_POINT_2F pt[4];
    float yLastBottom = FLT_MAX;
    for (const auto& lm : vBuf)
    {
    ReTry:
        hr = pTl->HitTestTextRange(idxChar, lm.length, 0.f, 0.f,
            vHitTestBuf.data(), cHitTestBuf, &cActual);
        if (FAILED(hr))
            if (hr != E_NOT_SUFFICIENT_BUFFER)
            {
                vHitTestBuf.resize(cActual);
                goto ReTry;
            }
            else
                return hr;
        const float x = vHitTestBuf[0].left,
            y = std::min(yLastBottom, vHitTestBuf[0].top);// 与上一行相接
        // 这两个范围应该用HitTestTextRange的结果来计算
        const float	r = vHitTestBuf[0].left + vHitTestBuf[0].width,
            b = vHitTestBuf[0].top + vHitTestBuf[0].height;
        yLastBottom = b;
        pSink->BeginFigure(D2D1::Point2F(x, y), D2D1_FIGURE_BEGIN_FILLED);

        pt[0].x = x;
        pt[0].y = y;
        pt[1].x = r;
        pt[1].y = y;
        pt[2].x = r;
        pt[2].y = b;
        pt[3].x = x;
        pt[3].y = b;
        pSink->AddLines(pt + 1, 3);
        pSink->EndFigure(D2D1_FIGURE_END_CLOSED);
        idxChar += lm.length;
    }
    hr = pSink->Close();
    if (FAILED(hr))
        return hr;
    float xDpi, yDpi;
    pDC->GetDpi(&xDpi, &yDpi);
    hr = pDC->CreateFilledGeometryRealization(pGeo.Get(),
        D2D1::ComputeFlatteningTolerance(
            pTransform ? *pTransform : D2D1::Matrix3x2F::Identity(),
            xDpi, yDpi),
        &pGrBk);
    return hr;
}
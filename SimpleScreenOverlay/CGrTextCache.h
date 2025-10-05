#pragma once
struct CGrTextCache
{
    ID2D1GeometryRealization* pGr;
    IDWriteTextLayout* pTl;

    EckInline HRESULT CreateTextLayout(PCWCH ps, int cch,
        IDWriteTextFormat* pTf, float cx, float cy)
    {
        SafeRelease(pTl);
        return eck::g_pDwFactory->CreateTextLayout(
            ps, (UINT32)cch, pTf, cx, cy, &pTl);
    }

    HRESULT CreateGeometryRealization(ID2D1DeviceContext1* pDC,
        const D2D1::Matrix3x2F* pTransform = nullptr);

    EckInline HRESULT Create(PCWCH ps, int cch,
        IDWriteTextFormat* pTf, float cx, float cy,
        ID2D1DeviceContext1* pDC, const D2D1::Matrix3x2F* pTransform = nullptr)
    {
        auto hr = CreateTextLayout(ps, cch, pTf, cx, cy);
        if (SUCCEEDED(hr))
            hr = CreateGeometryRealization(pDC, pTransform);
        return hr;
    }

    EckInlineNdCe auto GetTextLayout() const { return pTl; }
    EckInlineNdCe auto GetGeometryRealization() const { return pGr; }

    EckInlineNdCe BOOL IsValid() const { return !!pGr; }

    EckInline void Invalidate()
    {
        SafeRelease(pTl);
        SafeRelease(pGr);
    }
};

struct CGrTextCacheWithBk : public CGrTextCache
{
    ID2D1GeometryRealization* pGrBk;

    HRESULT CreateGeometryRealization(ID2D1DeviceContext1* pDC,
        const D2D1::Matrix3x2F* pTransform = nullptr);

    EckInline HRESULT Create(PCWCH ps, int cch,
        IDWriteTextFormat* pTf, float cx, float cy,
        ID2D1DeviceContext1* pDC, const D2D1::Matrix3x2F* pTransform = nullptr)
    {
        auto hr = CreateTextLayout(ps, cch, pTf, cx, cy);
        if (SUCCEEDED(hr))
            hr = CreateGeometryRealization(pDC, pTransform);
        return hr;
    }

    EckInlineNdCe auto GetGeometryRealizationBk() const { return pGrBk; }

    EckInlineNdCe BOOL IsValid() const { return !!pGr && !!pGrBk; }

    EckInline void Invalidate()
    {
        __super::Invalidate();
        SafeRelease(pGrBk);
    }
};
#include "pch.h"
#include "CVeFunctionMenu.h"

LRESULT CVeFunctionMenu::OnEvent(UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    switch (uMsg)
    {
    case WM_PAINT:
    {
        Dui::ELEMPAINTSTRU ps;
        BeginPaint(ps, wParam, lParam);

        D2D1_RECT_F rc{ GetViewRectF() };
        m_pBrush->SetColor(App->GetColor(CApp::CrDefFuncMenuTitle));
        m_pDC->FillRectangle(rc, m_pBrush);

        rc.left += (float)VeCxFuncMenuBorder;
        rc.top += (float)VeCyFuncMenuTitle;
        rc.right -= (float)VeCxFuncMenuBorder;
        rc.bottom -= float(VeCyFuncMenuFooter / 2);

        m_pBrush->SetColor(App->GetColor(CApp::CrDefFuncMenuBkg));
        const auto eOldBlend = m_pDC->GetPrimitiveBlend();
        m_pDC->SetPrimitiveBlend(D2D1_PRIMITIVE_BLEND_COPY);
        m_pDC->FillRectangle(rc, m_pBrush);
        m_pDC->SetPrimitiveBlend(eOldBlend);

        m_pBrush->SetColor(App->GetColor(CApp::CrText));
        if (!m_pTextLayout && !GetText().IsEmpty())
        {
            eck::g_pDwFactory->CreateTextLayout(GetText().Data(), GetText().Size(),
                GetTextFormat(), GetWidthF(), (float)VeCyFuncMenuTitle, &m_pTextLayout);
            m_pTextLayout->SetTextAlignment(DWRITE_TEXT_ALIGNMENT_CENTER);
            m_pTextLayout->SetParagraphAlignment(DWRITE_PARAGRAPH_ALIGNMENT_CENTER);
        }
        if (m_pTextLayout)
            m_pDC->DrawTextLayout({ 0,0 }, m_pTextLayout, m_pBrush,
                Dui::DrawTextLayoutFlags);

        EndPaint(ps);
    }
    return 0;

    case WM_SIZE:
    {
        D2D1_RECT_F rc;
        rc.left = VeCxFuncMenuBorder;
        rc.top = VeCyFuncMenuTitle;
        rc.right = GetWidthF() - VeCxFuncMenuBorder;
        rc.bottom = GetHeightF() - VeCyFuncMenuFooter;
        m_LV.SetRect(rc);
    }
    break;

    case WM_NOTIFY:
    {
        if ((WPARAM)&m_LV == wParam)
            return GenElemNotify((Dui::DUINMHDR*)lParam);
    }
    break;

    case WM_SETTEXT:
    {
        ECK_DUILOCK;
        SafeRelease(m_pTextLayout);
    }
    break;

    case WM_SETFONT:
    {
        ECK_DUILOCK;
        SafeRelease(m_pTextLayout);
    }
    break;
    case WM_CREATE:
    {
        m_pDC->CreateSolidColorBrush({}, &m_pBrush);
        m_LV.Create(nullptr, Dui::DES_VISIBLE, 0,
            0, 0, 10, 10, this);
        m_LV.SetItemHeight(VeCyFuncMenuItem);
        m_LV.SetItemPadding(VeCyFuncMenuItemPadding);

        m_LV.SetTextFormat(App->GetTextFormatCommon());
        SetTextFormat(App->GetTextFormatCommon());

        m_SizeBox.SetAllowSize(FALSE);
        m_SizeBox.Attach(this);
    }
    break;
    case WM_DESTROY:
    {
        SafeRelease(m_pBrush);
        m_SizeBox.Detach();
    }
    break;
    }
    return __super::OnEvent(uMsg, wParam, lParam);
}

void CVeFunctionMenu::ReCalcIdealSize()
{
    auto cyList = m_LV.GetItemCount() *
        (VeCyFuncMenuItem + VeCyFuncMenuItemPadding);
    cyList += (VeCyFuncMenuItemPadding + 1);
    m_LV.SetSize(GetWidthF(), cyList);
    SetSize(GetWidthF(), VeCyFuncMenuTitle + cyList + VeCyFuncMenuFooter);
}
﻿#include "pch.h"
#include "MessageUI.h"

#include "LogUI.h"

MessageUI::MessageUI()
    : UI("MessageUI", "##MessageUI")

{
    m_LogColor[(UINT)Log_Level::INFO] = { 1.f, 1.f, 1.f, 1.0f };
    m_LogColor[(UINT)Log_Level::ERR] = { 1.0f, 0.0f, 0.0f, 1.0f };
    m_LogColor[(UINT)Log_Level::WARN] = { 1.0f, 1.0f, 0.0f, 1.0f };

    m_bResize = false;
    m_iLogCount = 0;

    m_MaxTableSize = { 300.f, 500.f };

}

MessageUI::~MessageUI()
{
}

void MessageUI::tick()
{
    
}

void MessageUI::render_update()
{
    UpdateData();

    // 스크롤 얻어오기
    m_fLastScrollY = ImGui::GetScrollY();

    // 변경 여부 체크
    if (m_iLogCount < m_vectLog.size())
    {
        m_bResize = true;
    }

    // 사이즈 변경
    if (m_MaxTableSize.y < m_TableSize.y - 125.f && m_bResize == true)
    {
        m_MaxTableSize = m_TableSize;
        m_MaxTableSize.x -= 25.f;
        m_MaxTableSize.y -= 125.f;

    }

    // 필터 레벨 체크
    CheckLogLvFilter();

    // table 플래그 설정
    static ImGuiTableFlags flags = ImGuiTableFlags_Resizable |  ImGuiTableFlags_NoSavedSettings | ImGuiTableFlags_NoBordersInBodyUntilResize |
                                   ImGuiTableFlags_BordersOuter | ImGuiTableFlags_SizingStretchProp | ImGuiTableFlags_ScrollY | ImGuiTableFlags_NoHostExtendY;

    // table 생성
    if (ImGui::BeginTable("LogMessage", 1, flags, m_MaxTableSize))
    {
        ImGui::TableSetupColumn("Log");
        ImGui::TableHeadersRow();

        for (int row = 0; row < m_vectLog.size(); row++)
        {
            //색상 설정
            ImVec4 Color;
   
            bool SearchDisplay = false;
            bool ColorDisplay = false;

            CheckSearchDisplay(SearchDisplay, row);

            CheckColorDispaly(ColorDisplay, row);

            if (ColorDisplay && SearchDisplay)
            {
                string count = std::to_string(row);
                string temp = m_vectLog[row].m_strMsg.c_str() + count;

                ImGui::TableNextRow();
                ImGui::TableSetColumnIndex(0);
                ImGui::TextColored(SetLogColor(m_vectLog[row]), temp.c_str());

             }

            // 현재 스크롤 위치
            float currentScrollY = ImGui::GetScrollY();
            float maxScrollY = ImGui::GetScrollMaxY();

            // 만약 사용자가 스크롤을 움직이지 않았고 스크롤이 최하단에 있었다면, 스크롤을 최하단으로 유지
            if (m_fLastScrollY == maxScrollY && m_bResize)
            {
                ImGui::SetScrollHereY(1.0f);
            }

            // 스크롤 위치 업데이트
            m_fLastScrollY = ImGui::GetScrollY();

        }

        ImGui::EndTable();
    }

    m_iLogCount = m_vectLog.size();
    m_bResize = false;


}

ImVec4 MessageUI::SetLogColor(tLog Log)
{
    ImVec4 Color;

    switch (Log.m_LogLv)
    {
    case Log_Level::INFO:
        return Color = m_LogColor[(UINT)Log_Level::INFO];
        break;
    case Log_Level::WARN:
        return Color = m_LogColor[(UINT)Log_Level::ERR];
        break;
    case Log_Level::ERR:
        return Color = m_LogColor[(UINT)Log_Level::WARN];
        break;
    case Log_Level::END:
        break;
    default:
        break;
    }

    return Color;
}

void MessageUI::UpdateData()
{
    // 데이터 갱신
    m_vectLog = CLogMgr::GetInst()->GetLogvec();

    // 사이즈 측정
    m_Parent = dynamic_cast<LogUI*>(this->GetParentUI());

    //필터값 갱신
    m_charLogMsgFilter = m_Parent->GetLogTextFilter();


    // 부모 ui 사이즈 받아오기
    if (m_Parent != nullptr)
        m_TableSize = m_Parent->GetUISize();
}

void MessageUI::CheckLogLvFilter()
{
    for (int i = 0; i < 4; ++i)
    {
        bool* mask = m_Parent->GetLogLvFilter();

        if (mask[i] == true)
        {
            m_iLoglvMask |= (1 << i);
        }
        else
        {
            m_iLoglvMask &= ~(1 << i);
        }
    }
}

void MessageUI::CheckSearchDisplay(bool& check, int row)
{
    if (m_charLogMsgFilter.empty())
    {
        check = true;
    }
    else
    {
        for (int i = 0; i <= m_vectLog[row].m_strMsg.size() - m_charLogMsgFilter.size(); ++i)
        {
            if (m_vectLog[row].m_strMsg.substr(i, m_charLogMsgFilter.size()) == m_charLogMsgFilter)
            {
                check = true;
                break;
            }
        }
    }
}

void MessageUI::CheckColorDispaly(bool& check, int row)
{
    if (m_iLoglvMask & 1)
    {
        check = true;
    }
    else if (m_iLoglvMask & 2 && m_vectLog[row].m_LogLv == Log_Level::INFO)
    {
        check = true;

    }
    else if (m_iLoglvMask & 4 && m_vectLog[row].m_LogLv == Log_Level::WARN)
    {
        check = true;

    }
    else if (m_iLoglvMask & 8 && m_vectLog[row].m_LogLv == Log_Level::ERR)
    {
        check = true;

    }
}

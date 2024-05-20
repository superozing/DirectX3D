#include "pch.h"
#include "LogUI.h"

#include <Engine\CLogMgr.h>



LogUI::LogUI()
	: UI("##Log", "##LogUI")
    , m_vectLog()
    , m_AvailableSize(0.f, 0.f)
    , m_iLoglvMask(0)
    , m_iLogTimeMask(0)
    , m_fLastScrollY(0.f)
{
    m_LoglvCheckBox[0] = true;
    m_LoglvCheckBox[1] = false;
    m_LoglvCheckBox[2] = false;
    m_LoglvCheckBox[3] = false;

    m_LogTimePrint[0] = true;
    m_LogTimePrint[1] = false;
    m_LogTimePrint[2] = false;
    m_LogTimePrint[3] = false;

    m_LogColor[(UINT)Log_Level::INFO] = ImVec4(0.7f, 0.7f, 0.7f, 1.0f);
    m_LogColor[(UINT)Log_Level::WARN] = ImVec4(1.f, 1.f, 0.f, 1.0f);
    m_LogColor[(UINT)Log_Level::ERR] = ImVec4(1.f, 0.f, 0.f, 1.0f);

}

LogUI::~LogUI()
{
}

ImVec4 LogUI::GetLogColor(tLog Log)
{
    ImVec4 Color;

    switch (Log.m_LogLv)
    {
    case Log_Level::INFO:
        return Color = m_LogColor[(UINT)Log_Level::INFO];
        break;
    case Log_Level::WARN:
        return Color = m_LogColor[(UINT)Log_Level::WARN];
        break;
    case Log_Level::ERR:
        return Color = m_LogColor[(UINT)Log_Level::ERR];
        break;
    case Log_Level::END:
        break;
    default:
        break;
    }

    return Color;
}

void LogUI::CheckLevelMask()
{
    if (m_LoglvCheckBox[0])
    {
        m_iLoglvMask |= (UINT)Log_Level::INFO;
        m_iLoglvMask |= (UINT)Log_Level::WARN;
        m_iLoglvMask |= (UINT)Log_Level::ERR;
    }
    else
    {

        for (int i = 0; i < 3; ++i)
        {
            if (m_LoglvCheckBox[i+1])
                m_iLoglvMask |= (1 << i);
            else
                m_iLoglvMask &= ~(1 << i);
        }
    }
}

void LogUI::CheckTimeMask()
{

    m_iLogTimeMask = CLogMgr::GetInst()->GetTimeMask();

    if (m_LogTimePrint[0])
    {
        m_iLogTimeMask = 0;

    }
    else
    {
        for (int i = 0; i < 3; ++i)
        {
            if (m_LogTimePrint[i + 1])
                m_iLogTimeMask |= (1 << i);
            else
                m_iLogTimeMask &= ~(1 << i);
        }
    }

    CLogMgr::GetInst()->SetTimeMask(m_iLogTimeMask);
}

bool LogUI::CheckSearchDisplay(string msg)
{    
    return m_TextFilter.PassFilter(msg.c_str());
}


bool LogUI::CheckLogLvDisplay(Log_Level Loglv)
{
    return (UINT)Loglv & m_iLoglvMask;
}

void LogUI::tick()
{
}

void LogUI::render_update()
{
    m_vectLog = CLogMgr::GetInst()->GetLogs();

    // 복사 버튼
    ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0.3f, 0.3f, 0.3f, 1.0f)); // 기본 배경색
    ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4(1.f, 0.f, 0.f, 1.0f)); // 호버 배경색도 기본 배경색과 동일하게 설정
    ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4(1.f, 0.f, 0.f, 1.0f));
    if (ImGui::Button("CopyLog"))
    {
        CLogMgr::GetInst()->CopyLog();
    }

    ImGui::SameLine();

    // 삭제 버튼
    if (ImGui::Button("DeleteLog"))
    {
        CLogMgr::GetInst()->ClearLog();
    }
    ImGui::PopStyleColor(3);

    // 필터 체크박스
    
    if (ImGui::Checkbox("All", &m_LoglvCheckBox[0]))
    {
        if (m_LoglvCheckBox[0])
        {
            m_LoglvCheckBox[1] = false;
            m_LoglvCheckBox[2] = false;
            m_LoglvCheckBox[3] = false;
        }
    }
    ImGui::SameLine();

    if (ImGui::Checkbox("Info", &m_LoglvCheckBox[1]))
    {
        if (m_LoglvCheckBox[1])
            m_LoglvCheckBox[0] = false;
    }
    ImGui::SameLine();

    if (ImGui::Checkbox("Warn", &m_LoglvCheckBox[2]))
    {
        if (m_LoglvCheckBox[2])
            m_LoglvCheckBox[0] = false;
    }

    ImGui::SameLine();

    if (ImGui::Checkbox("Error", &m_LoglvCheckBox[3]))
    {
        if (m_LoglvCheckBox[3])
            m_LoglvCheckBox[0] = false;
    }

    CheckLevelMask();


    // 시간 관련 표기
    if (ImGui::Checkbox("No Time", &m_LogTimePrint[0]))
    {
        if (m_LogTimePrint[0])
        {
            for (int i = 1; i < 4; ++i)
            {
                m_LogTimePrint[i] = false;
            }
        }
    }
    ImGui::SameLine();
    
    if (ImGui::Checkbox("Sec", &m_LogTimePrint[1]))
    {
        if (m_LogTimePrint[1])
            m_LogTimePrint[0] = false;
    }
    ImGui::SameLine();

    if (ImGui::Checkbox("Min", &m_LogTimePrint[2]))
    {
        if (m_LogTimePrint[2])
            m_LogTimePrint[0] = false;
    }
    ImGui::SameLine();

    if (ImGui::Checkbox("Hour", &m_LogTimePrint[3]))
    {
        if (m_LogTimePrint[3])
            m_LogTimePrint[0] = false;
    }

    CheckTimeMask();

    // 검색 필터
    ImGui::Text("Search");
    ImGui::SameLine();

    ImGui::PushItemWidth(-FLT_MIN);
    m_TextFilter.Draw("##SearchBar");
    ImGui::PopItemWidth();



    // InvisibleButton으로 크기 조정
    ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2(0, 0));
    ImGui::InvisibleButton("hsplitter", ImVec2(-1, 8.0f));
    if (ImGui::IsItemActive())
    {
        m_AvailableSize.y += ImGui::GetIO().MouseDelta.y;
    }
    ImGui::PopStyleVar();
    
    // 테이블 빈 공간 추가
    ImGui::Dummy(ImVec2(0.0f, m_AvailableSize.y));


    // 크기 통제
    ImVec2 parentSize = ImGui::GetContentRegionAvail();
    
    float tableHeight = parentSize.y - m_AvailableSize.y;
    if (tableHeight < 0) tableHeight = 0;


    // table 플래그 설정
    static ImGuiTableFlags flags = ImGuiTableFlags_Resizable | ImGuiTableFlags_NoSavedSettings | ImGuiTableFlags_NoBordersInBodyUntilResize |
        ImGuiTableFlags_BordersOuter | ImGuiTableFlags_SizingStretchProp | ImGuiTableFlags_ScrollY | ImGuiTableFlags_NoHostExtendY;

    // table 생성
    if (ImGui::BeginTable("LogMessage", 1, flags, ImVec2(0.f, tableHeight)))
    {
        ImGui::TableSetupColumn("Log");
        ImGui::TableHeadersRow();

        for (int row = 0; row < m_vectLog.size(); row++)
        {
            //색상 설정
            ImVec4 Color;

            bool SearchDisplay = false;
            bool ColorDisplay = false;

            SearchDisplay =  CheckSearchDisplay(m_vectLog[row].m_strMsg);

            ColorDisplay = CheckLogLvDisplay(m_vectLog[row].m_LogLv);

           if (SearchDisplay && ColorDisplay)
           { 
                 string temp;

             if (m_iLogTimeMask > 0)
             {
                 

                 temp += CLogMgr::GetInst()->GetTimeMsg(m_vectLog[row].m_dTime, m_iLogTimeMask) + " ";

                 temp += m_vectLog[row].m_strMsg.c_str();
                 m_vectLog[row].m_strMsg = temp;
             }
             

                ImGui::TableNextRow();
                ImGui::TableSetColumnIndex(0);
                ImGui::TextColored(GetLogColor(m_vectLog[row]), m_vectLog[row].m_strMsg.c_str());
           }
            

            // 현재 스크롤 위치
            float currentScrollY = ImGui::GetScrollY();
            float maxScrollY = ImGui::GetScrollMaxY();

            // 만약 사용자가 스크롤을 움직이지 않았고 스크롤이 최하단에 있었다면, 스크롤을 최하단으로 유지
            if (m_fLastScrollY == maxScrollY)
            {
                ImGui::SetScrollHereY(1.0f);
            }

            // 스크롤 위치 업데이트
            m_fLastScrollY = ImGui::GetScrollY();

        }

        ImGui::EndTable();
    }
}
#include "pch.h"
#include "MessageUI.h"


MessageUI::MessageUI()
	: UI("MessageUI", "##MessageUI")
{
    m_LogColor[(UINT)Log_Level::INFO] = { 1.f, 1.f, 1.f, 1.0f };
    m_LogColor[(UINT)Log_Level::ERR] = { 1.0f, 0.0f, 0.0f, 1.0f };
    m_LogColor[(UINT)Log_Level::WARN] = { 1.0f, 1.0f, 0.0f, 1.0f };

}

MessageUI::~MessageUI()
{
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

void MessageUI::tick()
{
    
}

void MessageUI::render_update()
{
    m_vectLog = CLogMgr::GetInst()->GetLogvec();

    static ImGuiTableFlags flags = ImGuiTableFlags_Resizable | ImGuiTableFlags_Reorderable | ImGuiTableFlags_Hideable | ImGuiTableFlags_BordersOuter | ImGuiTableFlags_BordersV | ImGuiTableFlags_SizingStretchProp;

   

    if (ImGui::BeginTable("LogMessage", 1, flags))
    {
        // Submit columns name with TableSetupColumn() and call TableHeadersRow() to create a row with a header in each column.
        // (Later we will show how TableSetupColumn() has other uses, optional flags, sizing weight etc.)
        ImGui::TableSetupColumn("Log");
        ImGui::TableHeadersRow();


        for (int row = 0; row < m_vectLog.size(); row++)
        {
            ImGui::TableNextRow();
            ImGui::TableSetColumnIndex(0);


            //색상 설정
            ImVec4 Color = SetLogColor(m_vectLog[row]);

            ImGui::TextColored(Color, m_vectLog[row].m_strMsg.c_str());
            
           
            
        }
        ImGui::EndTable();
    }
}

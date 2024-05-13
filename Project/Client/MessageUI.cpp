#include "pch.h"
#include "MessageUI.h"


MessageUI::MessageUI()
	: UI("MessageUI", "##MessageUI")
{
    
}

MessageUI::~MessageUI()
{
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

            ImGui::TextColored(ImVec4(1.0f, 0.0f, 0.0f, 1.0f), "Warning: This is a warning message!");
            ImGui::Text("%d", row);

            ImGui::Text(m_vectLog[row].m_strMsg.c_str());
            
           
            
        }
        ImGui::EndTable();
    }
}

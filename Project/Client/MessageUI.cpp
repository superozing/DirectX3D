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
    static ImGuiTableFlags flags = ImGuiTableFlags_Resizable | ImGuiTableFlags_Reorderable | ImGuiTableFlags_Hideable | ImGuiTableFlags_BordersOuter | ImGuiTableFlags_BordersV | ImGuiTableFlags_SizingStretchProp;

    if (ImGui::BeginTable("LogMessage", 2, flags))
    {
        // Submit columns name with TableSetupColumn() and call TableHeadersRow() to create a row with a header in each column.
        // (Later we will show how TableSetupColumn() has other uses, optional flags, sizing weight etc.)
        ImGui::TableSetupColumn("Log Level");
        ImGui::TableSetupColumn("Message");
        ImGui::TableHeadersRow();
        for (int row = 0; row < 200; row++)
        {
            ImGui::TableNextRow();
            for (int column = 0; column < 2; column++)
            {
                ImGui::TableSetColumnIndex(column);
                ImGui::Text("Hello %d,%d", column, row);
            }
        }
        ImGui::EndTable();
    }
}

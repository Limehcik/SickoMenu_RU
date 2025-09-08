#include "pch-il2cpp.h"
#include "esp_tab.h"
#include "game.h"
#include "state.hpp"
#include "utility.h"
#include "gui-helpers.hpp"

namespace EspTab {

	void Render() {
		bool changed = false;
		ImGui::SameLine(100 * State.dpiScale);
		ImGui::BeginChild("###ESP", ImVec2(500 * State.dpiScale, 0), true, ImGuiWindowFlags_NoBackground);
		changed |= ToggleButton("Включить", &State.ShowEsp);

		changed |= ToggleButton("Показывать призраков", &State.ShowEsp_Ghosts);
		//dead bodies for v3.1
		changed |= ToggleButton("Скрыть во время голосования", &State.HideEsp_During_Meetings);

		changed |= ToggleButton("Показывать Боксы", &State.ShowEsp_Box);
		changed |= ToggleButton("Показывать Линии", &State.ShowEsp_Tracers);
		changed |= ToggleButton("Показывать Дистанцию", &State.ShowEsp_Distance);
		//better esp (from noobuild) coming v3.1
		changed |= ToggleButton("Относительно роли", &State.ShowEsp_RoleBased);

		if (State.ShowEsp_RoleBased) {
			ImGui::SameLine();
			changed |= ToggleButton("Члены экипажа", &State.ShowEsp_Crew);
			ImGui::SameLine();
			changed |= ToggleButton("Предатели", &State.ShowEsp_Imp);
		}

		ImGui::EndChild();
		if (changed) {
			State.Save();
		}
	}
}
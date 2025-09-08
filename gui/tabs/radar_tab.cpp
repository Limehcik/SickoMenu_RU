#include "pch-il2cpp.h"
#include "radar_tab.h"
#include "gui-helpers.hpp"
#include "state.hpp"
#include "utility.h"

namespace RadarTab {
	void Render() {
		ImGui::SameLine(100 * State.dpiScale);
		ImGui::BeginChild("###Radar", ImVec2(500 * State.dpiScale, 0), true, ImGuiWindowFlags_NoBackground);
		ImGui::Dummy(ImVec2(4, 4) * State.dpiScale);
		if (ToggleButton("Показать Радар", &State.ShowRadar)) {
			State.Save();
		}

		ImGui::Dummy(ImVec2(7, 7) * State.dpiScale);
		ImGui::Separator();
		ImGui::Dummy(ImVec2(7, 7) * State.dpiScale);

		if (ToggleButton("Показать Мертвые Тела", &State.ShowRadar_DeadBodies)) {
			State.Save();
		}
		if (ToggleButton("Показывать Призраков", &State.ShowRadar_Ghosts)) {
			State.Save();
		}
		if (ToggleButton("ПКМ для Телепорта", &State.ShowRadar_RightClickTP)) {
			State.Save();
		}

		ImGui::Dummy(ImVec2(7, 7) * State.dpiScale);
		ImGui::Separator();
		ImGui::Dummy(ImVec2(7, 7) * State.dpiScale);

		if (ToggleButton("Скрыть Радар на Голосовании", &State.HideRadar_During_Meetings)) {
			State.Save();
		}
		if (ToggleButton("Отрисовывать Иконки Игроков", &State.RadarDrawIcons)) {
			State.Save();
		}
		/*if (State.RadarDrawIcons && State.RevealRoles) {
			ImGui::SameLine();
			if (ToggleButton("Show Role Color on Visor", &State.RadarVisorRoleColor)) {
				State.Save();
			}
		}*/

		if (ToggleButton("Закрепить Позицию Радара", &State.LockRadar)) {
			State.Save();
		}
		if (ToggleButton("Показать границу", &State.RadarBorder)) {
			State.Save();
		}
		if (ImGui::ColorEdit4("Цвета Радара",
			(float*)&State.SelectedColor,
			ImGuiColorEditFlags__OptionsDefault
			| ImGuiColorEditFlags_NoInputs
			| ImGuiColorEditFlags_AlphaBar
			| ImGuiColorEditFlags_AlphaPreview)) {
			State.Save();
		}
		if (ImGui::InputInt("Дополнительная Ширина", &State.RadarExtraWidth)) {
			State.RadarExtraWidth = abs(State.RadarExtraWidth); //prevent negatives
			State.Save();
		}
		if (ImGui::InputInt("Дополнительная Высота", &State.RadarExtraHeight)) {
			State.RadarExtraHeight = abs(State.RadarExtraHeight); //prevent negatives
			State.Save();
		}

		ImGui::EndChild();
	}
}
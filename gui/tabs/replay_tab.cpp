#include "pch-il2cpp.h"
#include "replay_tab.h"
#include "gui-helpers.hpp"
#include "state.hpp"
#include <chrono>

namespace ReplayTab {
	void Render() {
		ImGui::SameLine(100 * State.dpiScale);
		ImGui::BeginChild("###Replay", ImVec2(500 * State.dpiScale, 0), true, ImGuiWindowFlags_NoBackground);
		ImGui::Dummy(ImVec2(4, 4) * State.dpiScale);
		if (ToggleButton("Показать Повтор", &State.ShowReplay)) {
			State.Save();
		}
		if (ToggleButton("Показывать Только Последние", &State.Replay_ShowOnlyLastSeconds))
		{
			State.Save();
		}
		ImGui::SameLine();
		if (SliderIntV2("Секунд", &State.Replay_LastSecondsValue, 1, 1200, "%d", ImGuiSliderFlags_AlwaysClamp))
		{
			State.Save();
		}

		if (ToggleButton("Очистить После Голосования", &State.Replay_ClearAfterMeeting))
		{
			State.Save();
		}

		if (ToggleButton("Отрисовывать Иконки Игроков", &State.Replay_DrawIcons))
		{
			State.Save();
		}

		if (ImGui::ColorEdit4("Цвета Карты Повтора",
			(float*)&State.SelectedReplayMapColor,
			ImGuiColorEditFlags__OptionsDefault
			| ImGuiColorEditFlags_NoInputs
			| ImGuiColorEditFlags_AlphaBar
			| ImGuiColorEditFlags_AlphaPreview)) {
			State.Save();
		}
		ImGui::EndChild();
	}
}
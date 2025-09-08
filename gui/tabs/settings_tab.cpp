#include "pch-il2cpp.h"
#include "settings_tab.h"
#include "utility.h"
#include "gui-helpers.hpp"
#include "state.hpp"
#include "game.h"
#include "achievements.hpp"
#include "DirectX.h"
#include "imgui/imgui_impl_win32.h" // ImGui_ImplWin32_GetDpiScaleForHwnd
#include "theme.hpp" // ApplyTheme

namespace SettingsTab {
	enum Groups {
		General,
		Spoofing,
		Customization,
		Keybinds
	};

	static bool openGeneral = true; //default to general tab group
	static bool openSpoofing = false;
	static bool openCustomization = false;
	static bool openKeybinds = false;

	void CloseOtherGroups(Groups group) {
		openGeneral = group == Groups::General;
		openSpoofing = group == Groups::Spoofing;
		openCustomization = group == Groups::Customization;
		openKeybinds = group == Groups::Keybinds;
	}

	void CheckKeybindEdit(bool hotKey) {
		State.KeybindsBeingEdited = State.KeybindsBeingEdited || hotKey;
	}

	void Render() {
		ImGui::SameLine(100 * State.dpiScale);
		ImGui::BeginChild("###Settings", ImVec2(500 * State.dpiScale, 0), true, ImGuiWindowFlags_NoBackground);
		if (TabGroup("Главная", openGeneral)) {
			CloseOtherGroups(Groups::General);
		}
		ImGui::SameLine();
		if (TabGroup("Подделывание", openSpoofing)) {
			CloseOtherGroups(Groups::Spoofing);
		}
		ImGui::SameLine();
		if (TabGroup("Кастомизация", openCustomization)) {
			CloseOtherGroups(Groups::Customization);
		}
		ImGui::SameLine();
		if (TabGroup("Клавиши", openKeybinds)) {
			CloseOtherGroups(Groups::Keybinds);
		}
		if (openGeneral) {
			ImGui::Dummy(ImVec2(4, 4) * State.dpiScale);
			if (ToggleButton("Показывать Клавиши", &State.ShowKeybinds)) {
				State.Save();
			}
			ImGui::SameLine();
			if (ToggleButton("Включить Активацию Клавишь в Чате", &State.KeybindsWhileChatting)) {
				State.Save();
			}
			if (ToggleButton("Показывать Меню при Запуске", &State.ShowMenuOnStartup)) {
				State.Save();
			}
			ImGui::SameLine();
			if (ToggleButton("Подтверждение для Отключения", &State.PanicWarning)) {
				State.Save();
			}

			if (ToggleButton("Дополнительные Команды", &State.ExtraCommands)) {
				State.Save();
			}

			if (ImGui::IsItemHovered()) {
				ImGui::SetTooltip("Напиши \"/help\" что бы увидеть все команды.");
			}
			ImGui::Dummy(ImVec2(7, 7) * State.dpiScale);
			ImGui::Separator();
			ImGui::Dummy(ImVec2(7, 7) * State.dpiScale);

			// sorry to anyone trying to read this code it is pretty messy
#pragma region New config menu, needs fixing
			/*
			std::vector<std::string> CONFIGS = GetAllConfigs();
			CONFIGS.push_back("[New]");
			CONFIGS.push_back("[Delete]");

			std::vector<const char*> CONFIGS_CHAR;

			for (const std::string& str : CONFIGS) {
				char* ch = new char[str.size() + 1];
				std::copy(str.begin(), str.end(), ch);
				ch[str.size()] = '\0';
				CONFIGS_CHAR.push_back(ch);
			}

			bool isNewConfig = CONFIGS.size() == 1;
			bool isDelete = false;

			int& selectedConfigInt = State.selectedConfigInt;
			std::string selectedConfig = CONFIGS[selectedConfigInt];

			if (CustomListBoxInt("Configs", &selectedConfigInt, CONFIGS_CHAR), 100 * State.dpiScale, ImVec4(0,0,0,0), ImGuiComboFlags_NoArrowButton) {
				isNewConfig = selectedConfigInt == CONFIGS.size() - 2;
				isDelete = selectedConfigInt == CONFIGS.size() - 1;
				if (!isNewConfig && !isDelete) State.selectedConfig = CONFIGS[selectedConfigInt];
				State.Save();
				State.Load();
			}

			if (isNewConfig || isDelete) {
				InputString("Name", &State.selectedConfig);
				if (isNewConfig && (AnimatedButton(CheckConfigExists(State.selectedConfig) ? "Overwrite" : "Save"))) {
					State.Save();
					CONFIGS = GetAllConfigs();

					selectedConfigInt = std::distance(CONFIGS.begin(), std::find(CONFIGS.begin(), CONFIGS.end(), State.selectedConfig));
				}

				if (isDelete && CheckConfigExists(State.selectedConfig)) {
					if (AnimatedButton("Delete")) {
						selectedConfigInt--;
						State.Delete();
						CONFIGS = GetAllConfigs();
						if (selectedConfigInt < 0) selectedConfigInt = 0;
					}
				}
			}*/
#pragma endregion

			InputString("Название Конфига", &State.selectedConfig);

			if (CheckConfigExists(State.selectedConfig) && AnimatedButton("Загрузить Конфиг"))
			{
				State.SaveConfig();
				State.Load();
				State.Save(); //actually save the selected config
			}
			if (CheckConfigExists(State.selectedConfig)) ImGui::SameLine();
			if (AnimatedButton("Сохранить Конфиг"))
			{
				State.Save();
			}
			if (!CheckConfigExists(State.selectedConfig)) {
				ImGui::Text("Имя Конфига не Найдено!");
				ImGui::SameLine();
			}

			/*if (ToggleButton("Adjust by DPI", &State.AdjustByDPI)) {
				if (!State.AdjustByDPI) {
					State.dpiScale = 1.0f;
				}
				else {
					State.dpiScale = ImGui_ImplWin32_GetDpiScaleForHwnd(DirectX::window);
				}
				State.dpiChanged = true;
				State.Save();
			}*/

			/*static const std::vector<const char*> DPI_SCALING_LEVEL = {"50%", "60%", "70%", "80%", "90%", "100%", "110%", "120%", "130%", "140%", "150%", "160%", "170%", "180%", "190%", "200%", "210%", "220%", "230%", "240%", "250%", "260%", "270%", "280%", "290%", "300%"};
			
			int scaleIndex = (int(std::clamp(State.dpiScale, 0.5f, 3.0f) * 100.0f) - 50) / 5;
			if (CustomListBoxInt("Menu Scale", &scaleIndex, DPI_SCALING_LEVEL, 100 * State.dpiScale)) {
				State.dpiScale = (scaleIndex * 10 + 50) / 100.0f;
				State.dpiChanged = true;
			}*/

			ImGui::Dummy(ImVec2(4, 4) * State.dpiScale);

			if (ImGui::InputInt("FPS", &State.GameFPS)) {
				State.GameFPS = std::clamp(State.GameFPS, 1, 2147483647);
				State.Save();
			}

			ImGui::Dummy(ImVec2(1, 1) * State.dpiScale);

			if (ToggleButton("Авто Выход, если FPS меньше", &State.LeaveDueLFPS)) {
				State.Save();
			}
			ImGui::SameLine();
			ImGui::PushItemWidth(80);
			ImGui::InputInt("Минимальный FPS", &State.minFpsThreshold);
			if (State.minFpsThreshold < 0)
				State.minFpsThreshold = 0;
			ImGui::PopItemWidth();

			ImGui::Dummy(ImVec2(5, 5) * State.dpiScale);

#ifdef _DEBUG
			if (ToggleButton("Показать Debug Вкладку", &State.showDebugTab)) {
				State.Save();
			}
			ImGui::Dummy(ImVec2(4, 4) * State.dpiScale);
#endif
			if (!IsHost() && !State.SafeMode && !IsNameValid(State.userName)) {
				ImGui::PushStyleColor(ImGuiCol_FrameBg, ImVec4(0.5f, 0.f, 0.f, State.MenuThemeColor.w));
				if (InputString("Никнейм", &State.userName)) State.Save();
				ImGui::PopStyleColor();
			}
			else if (InputString("Никйнем", &State.userName)) State.Save();

			if (!IsNameValid(State.userName) && !IsHost() && State.SafeMode) {
				if (State.userName == "")
					ImGui::TextColored(ImVec4(1.0f, 0.0f, 0.0f, 1.0f), "Пустой никнейм обнаружается Античитом. Никнейм будет проигнорен.");
				if (State.userName.length() >= (size_t)13)
					ImGui::TextColored(ImVec4(1.0f, 0.0f, 0.0f, 1.0f), "Слишком длинный никнейм обнаружается Античитом. Никнейм будет проигнорен.");
				else if (!IsNameValid(State.userName))
					ImGui::TextColored(ImVec4(1.0f, 0.0f, 0.0f, 1.0f), "Никнейм содержит символы, обнаружаемые античитом. Никнейм будет проигнорен.");
				else
					ImGui::TextColored(ImVec4(1.0f, 0.0f, 0.0f, 1.0f), "Никнейм не нравится Античиту. Никнейм будет проигнорен.");
			}

			if (IsNameValid(State.userName) || IsHost() || !State.SafeMode) {
				if ((IsInGame() || IsInLobby()) && AnimatedButton("Установить Никнейм")) {
					if (IsInGame())
						State.rpcQueue.push(new RpcSetName(State.userName));
					else if (IsInLobby())
						State.lobbyRpcQueue.push(new RpcSetName(State.userName));
					LOG_INFO("Successfully set in-game name to \"" + State.userName + "\"");
				}
				if (IsNameValid(State.userName)) {
					if ((IsInGame() || IsInLobby())) ImGui::SameLine();
					if (AnimatedButton("Установить Никнейм Аккаунту")) {
						SetPlayerName(State.userName);
						LOG_INFO("Successfully set account name to \"" + State.userName + "\"");
					}
				}
				ImGui::SameLine();
			}
			if (ToggleButton("Авто Установка Никнейма", &State.SetName)) {
				State.Save();
			}

			if (InputString("Кастомный Код", &State.customCode)) {
				State.Save();
			}
			if (ToggleButton("Заменить Код Лобби в Режима Стримера", &State.HideCode)) {
				State.Save();
			}
			ImGui::SameLine();
			if (ToggleButton("RGB Код Лобби", &State.RgbLobbyCode)) {
				State.Save();
			}

			ImGui::Dummy(ImVec2(4, 4) * State.dpiScale);

			ImGui::Dummy(ImVec2(7, 7) * State.dpiScale);
			ImGui::Separator();
			ImGui::Dummy(ImVec2(7, 7) * State.dpiScale);

			static float timer = 0.0f;
			static bool CosmeticsNotification = false;

			if (ToggleButton("Открыть Косметику", &State.UnlockCosmetics)) {
				State.Save();
				CosmeticsNotification = true;
				timer = static_cast<float>(ImGui::GetTime());
			}

			if (CosmeticsNotification) {
				float currentTime = static_cast<float>(ImGui::GetTime());
				if (currentTime - timer < 5.0f) {
					ImGui::SameLine();
					if (State.UnlockCosmetics)
						ImGui::TextColored(ImVec4(0.0f, 1.0f, 0.0f, 1.0f), "Косметика открыта!");
					else
						ImGui::TextColored(ImVec4(1.0f, 0.0f, 0.0f, 1.0f), "Косметика Закрыта!");
				}
				else {
					CosmeticsNotification = false;
				}
			}

			if (Achievements::IsSupported())
			{
				ImGui::SameLine();
				if (AnimatedButton("Открыть Все Достижения"))
					Achievements::UnlockAll();
			}

			if (ToggleButton("Другие пользователи видят что ты используешь", &State.ModDetection)) State.Save();
			ImGui::SameLine();
			if (CustomListBoxInt(" ", &State.BroadcastedMod, MODS, 100.f * State.dpiScale)) State.Save();
		}
		if (openSpoofing) {
			/*if (ToggleButton("Spoof Guest Account", &State.SpoofGuestAccount)) {
				State.Save();
			}
			if (State.SpoofGuestAccount) {
				ImGui::SameLine();
				if (ToggleButton("Use Custom Guest Friend Code", &State.UseNewFriendCode)) {
					State.Save();
				}
				if (State.UseNewFriendCode) {
					if (InputString("Guest Friend Code", &State.NewFriendCode)) {
						State.Save();
					}
					ImGui::Text("Guest friend code should be <= 10 characters long and cannot have a hashtag.");
				}
				ImGui::TextColored(ImVec4(0.f, 1.f, 0.f, 1.f), "Pro Tip: You can bypass the free chat restriction using a space after your custom friend");
				ImGui::TextColored(ImVec4(0.f, 1.f, 0.f, 1.f), "code!");
			}*/
			/*if (AnimatedButton("Force Login as Guest")) {
				State.ForceLoginAsGuest = true;
			}*/
			if (ToggleButton("Притвориться Гостевым Аккаунтом (ТОЛЬКО Быстрый Чат)", &State.SpoofGuestAccount)) {
				State.Save();
			}
			if (ToggleButton("Использовать Кастомный Код (ТОЛЬКО для Новых или Гостевый Аккаунтов)", &State.UseNewFriendCode)) {
				State.Save();
			}
			if (State.UseNewFriendCode) {
				ImGui::SetNextItemWidth(150 * State.dpiScale); // Adjust the width of the input box
				if (InputString("Код друга (ТОЛЬКО для Новых или Гостевый Аккаунтов)", &State.NewFriendCode)) {
					State.Save();
				}
				ImGui::Text("Новый Код должен быть <= 10 сивмолов длинной и быть без пробелов.");
			}
			if (ToggleButton("Подменить Уровень", &State.SpoofLevel)) {
				State.Save();
			}
			if (State.SpoofLevel) {
				ImGui::SameLine();
				if (ImGui::InputInt("", &State.FakeLevel, 0, 1)) {
					State.Save();
				}

				if (State.SafeMode && (State.FakeLevel <= 0 || State.FakeLevel > 100001))
					ImGui::TextColored(ImVec4(1.0f, 0.0f, 0.0f, 1.0f), "Уровень обнаружается Античитом, уровень должен быть от 0 до 100001.");
			}

			if (ToggleButton("Подменить Платформу", &State.SpoofPlatform)) {
				State.Save();
			}
			if (State.SpoofPlatform) {
				ImGui::SameLine();
				if (CustomListBoxInt("", &State.FakePlatform, PLATFORMS))
					State.Save();
			}

			if (ToggleButton("Отключить Античит пока ты Хост (+25 Режим)", &State.DisableHostAnticheat)) {
				if (!State.DisableHostAnticheat && State.BattleRoyale) {
					State.BattleRoyale = false;
					State.GameMode = 0;
				}
				State.Save();
			}
			/*if (State.DisableHostAnticheat) {
				BoldText("Warning (+25 Mode)", ImVec4(1.f, 0.f, 0.f, 1.f));
				BoldText("With this option enabled, you can only find public lobbies with +25 enabled.");
				BoldText("You may not find any public lobbies in the game listing due to this.");
				BoldText("This is intended behaviour, do NOT report it as a bug.");
			}*/
			if (ToggleButton("Подменить Among Us Версию", &State.SpoofAUVersion))
				State.Save();
			if (State.SpoofAUVersion) {
				ImGui::SameLine();
				if (CustomListBoxInt("", &State.FakeAUVersion, AUVERSIONS))
					State.Save();
			}
		}

		if (openCustomization) {
			if (ToggleButton("Скрыть Водяную Марку", &State.HideWatermark)) {
				State.Save();
			}

			ImGui::Dummy(ImVec2(4, 4) * State.dpiScale);

			if (ToggleButton("Светлый Режим", &State.LightMode)) State.Save();
			ImGui::SameLine();
			if (!State.GradientMenuTheme) {
				if (ImGui::ColorEdit3("Цвет Темы меню", (float*)&State.MenuThemeColor, ImGuiColorEditFlags__OptionsDefault | ImGuiColorEditFlags_NoInputs | ImGuiColorEditFlags_AlphaBar | ImGuiColorEditFlags_AlphaPreview)) {
					State.Save();
				}
			}
			else {
				if (ImGui::ColorEdit3("Градиент 1", (float*)&State.MenuGradientColor1, ImGuiColorEditFlags__OptionsDefault | ImGuiColorEditFlags_NoInputs | ImGuiColorEditFlags_AlphaBar | ImGuiColorEditFlags_AlphaPreview)) {
					State.Save();
				}
				ImGui::SameLine();
				if (ImGui::ColorEdit3("Градиент 2", (float*)&State.MenuGradientColor2, ImGuiColorEditFlags__OptionsDefault | ImGuiColorEditFlags_NoInputs | ImGuiColorEditFlags_AlphaBar | ImGuiColorEditFlags_AlphaPreview)) {
					State.Save();
				}
			}
			ImGui::SameLine();
			if (ToggleButton("Градиентная Тема", &State.GradientMenuTheme))
				State.Save();

			if (ToggleButton("Цвет Фона Исходя от Темы", &State.MatchBackgroundWithTheme)) {
				State.Save();
			}
			ImGui::SameLine();
			if (ToggleButton("RGB Тема Меню", &State.RgbMenuTheme)) {
				State.Save();
			}
			ImGui::SameLine();
			if (AnimatedButton("Сбросить Тему Меню"))
			{
				State.MenuThemeColor = ImVec4(1.f, 0.f, 0.424f, 1.f);
			}

			SteppedSliderFloat("Непрозрачность", (float*)&State.MenuThemeColor.w, 0.1f, 1.f, 0.01f, "%.2f", ImGuiSliderFlags_Logarithmic | ImGuiSliderFlags_NoInput);

			ImGui::Dummy(ImVec2(4, 4) * State.dpiScale);

			if (ToggleButton("Темная Тема Игры", &State.DarkMode)) State.Save();
			ImGui::SameLine();
			if (ToggleButton("Кастомная Тема Игры", &State.CustomGameTheme)) State.Save();

			if (State.CustomGameTheme) {
				if (ImGui::ColorEdit3("Цвет Фона", (float*)&State.GameBgColor, ImGuiColorEditFlags__OptionsDefault | ImGuiColorEditFlags_NoInputs | ImGuiColorEditFlags_AlphaBar | ImGuiColorEditFlags_AlphaPreview))
					State.Save();
				ImGui::SameLine();
				if (ImGui::ColorEdit3("Цвет Текста", (float*)&State.GameTextColor, ImGuiColorEditFlags__OptionsDefault | ImGuiColorEditFlags_NoInputs | ImGuiColorEditFlags_AlphaBar | ImGuiColorEditFlags_AlphaPreview))
					State.Save();
			}
			if (ToggleButton("Изменить Шрифт Чата", &State.ChatFont)) {
				State.Save();
			}
			if (State.ChatFont) {
				ImGui::SameLine();
				if (CustomListBoxInt(" ", &State.ChatFontType, FONTS, 160.f * State.dpiScale)) {
					State.Save();
				}
			}

			ImGui::Dummy(ImVec2(4, 4)* State.dpiScale);

			if (ImGui::CollapsingHeader("Интерфейс")) {
				ImGui::SetNextItemWidth(50 * State.dpiScale);
				if (ImGui::InputFloat("Размер Меню", &State.dpiScale)) {
					State.dpiScale = std::clamp(State.dpiScale, 0.5f, 3.f);
					State.dpiChanged = true;
					State.Save();
				}
				if (ToggleButton("Отключить Анимации", &State.DisableAnimations))
					State.Save();
				if (ImGui::InputFloat("Скорость Анимаций", &State.AnimationSpeed)) {
					if (State.AnimationSpeed <= 0) State.AnimationSpeed = 1.f;
					State.Save();
				}
				SteppedSliderFloat("Радиус Скругления", &State.RoundingRadiusMultiplier, 0.f, 2.f, 0.01f, "%.2f", ImGuiSliderFlags_Logarithmic | ImGuiSliderFlags_NoInput);
			}

			if (ImGui::CollapsingHeader("Цвета ролей")) {
				ImGui::ColorEdit4("Призраки Экипажа", (float*)&State.CrewmateGhostColor, ImGuiColorEditFlags__OptionsDefault | ImGuiColorEditFlags_NoInputs | ImGuiColorEditFlags_AlphaBar | ImGuiColorEditFlags_AlphaPreview);
				ImGui::SameLine(150.f * State.dpiScale);
				ImGui::ColorEdit4("Член Экипажа", (float*)&State.CrewmateColor, ImGuiColorEditFlags__OptionsDefault | ImGuiColorEditFlags_NoInputs | ImGuiColorEditFlags_AlphaBar | ImGuiColorEditFlags_AlphaPreview);
				ImGui::SameLine(300.f * State.dpiScale);
				ImGui::ColorEdit4("Инжинер", (float*)&State.EngineerColor, ImGuiColorEditFlags__OptionsDefault | ImGuiColorEditFlags_NoInputs | ImGuiColorEditFlags_AlphaBar | ImGuiColorEditFlags_AlphaPreview);

				ImGui::ColorEdit4("Ангел Хранитель", (float*)&State.GuardianAngelColor, ImGuiColorEditFlags__OptionsDefault | ImGuiColorEditFlags_NoInputs | ImGuiColorEditFlags_AlphaBar | ImGuiColorEditFlags_AlphaPreview);
				ImGui::SameLine(150.f * State.dpiScale);
				ImGui::ColorEdit4("Ученый", (float*)&State.ScientistColor, ImGuiColorEditFlags__OptionsDefault | ImGuiColorEditFlags_NoInputs | ImGuiColorEditFlags_AlphaBar | ImGuiColorEditFlags_AlphaPreview);
				ImGui::SameLine(300.f * State.dpiScale);
				ImGui::ColorEdit4("Предатель", (float*)&State.ImpostorColor, ImGuiColorEditFlags__OptionsDefault | ImGuiColorEditFlags_NoInputs | ImGuiColorEditFlags_AlphaBar | ImGuiColorEditFlags_AlphaPreview);
				
				ImGui::ColorEdit4("Оборотень", (float*)&State.ShapeshifterColor, ImGuiColorEditFlags__OptionsDefault | ImGuiColorEditFlags_NoInputs | ImGuiColorEditFlags_AlphaBar | ImGuiColorEditFlags_AlphaPreview);
				ImGui::SameLine(150.f * State.dpiScale);
				ImGui::ColorEdit4("Призрак Предателя", (float*)&State.ImpostorGhostColor, ImGuiColorEditFlags__OptionsDefault | ImGuiColorEditFlags_NoInputs | ImGuiColorEditFlags_AlphaBar | ImGuiColorEditFlags_AlphaPreview);
				ImGui::SameLine(300.f * State.dpiScale);
				ImGui::ColorEdit4("Паникер", (float*)&State.NoisemakerColor, ImGuiColorEditFlags__OptionsDefault | ImGuiColorEditFlags_NoInputs | ImGuiColorEditFlags_AlphaBar | ImGuiColorEditFlags_AlphaPreview);
				
				ImGui::ColorEdit4("Следопыт", (float*)&State.TrackerColor, ImGuiColorEditFlags__OptionsDefault | ImGuiColorEditFlags_NoInputs | ImGuiColorEditFlags_AlphaBar | ImGuiColorEditFlags_AlphaPreview);
				ImGui::SameLine(150.f * State.dpiScale);
				ImGui::ColorEdit4("Фантом", (float*)&State.PhantomColor, ImGuiColorEditFlags__OptionsDefault | ImGuiColorEditFlags_NoInputs | ImGuiColorEditFlags_AlphaBar | ImGuiColorEditFlags_AlphaPreview);

				if (AnimatedButton("Сбросить Цвета Ролей")) {
					State.CrewmateGhostColor = ImVec4(1.f, 1.f, 1.f, 0.5f);
					State.CrewmateColor = ImVec4(1.f, 1.f, 1.f, 1.f);
					State.EngineerColor = ImVec4(0.f, 1.f, 1.f, 1.f);
					State.GuardianAngelColor = ImVec4(0.5f, 0.5f, 0.5f, 0.5f);
					State.ScientistColor = ImVec4(0.2f, 0.2f, 1.f, 1.f);
					State.ImpostorColor = ImVec4(1.f, 0.1f, 0.1f, 1.f);
					State.ShapeshifterColor = ImVec4(1.f, 0.67f, 0.f, 1.f);
					State.ImpostorGhostColor = ImVec4(0.25f, 0.25f, 0.25f, 0.5f);
					State.NoisemakerColor = ImVec4(0.f, 1.f, 0.47f, 1.f);
					State.TrackerColor = ImVec4(0.65f, 0.36f, 1.f, 1.f);
					State.PhantomColor = ImVec4(0.53f, 0.f, 0.f, 1.f);
					State.Save();
				}
			}

			if (ImGui::CollapsingHeader("Другие Цвета")) {
				ImGui::ColorEdit4("Хост Лобби", (float*)&State.HostColor, ImGuiColorEditFlags__OptionsDefault | ImGuiColorEditFlags_NoInputs | ImGuiColorEditFlags_AlphaBar | ImGuiColorEditFlags_AlphaPreview);
				ImGui::SameLine(150.f * State.dpiScale);
				ImGui::ColorEdit4("ID Игрока", (float*)&State.PlayerIdColor, ImGuiColorEditFlags__OptionsDefault | ImGuiColorEditFlags_NoInputs | ImGuiColorEditFlags_AlphaBar | ImGuiColorEditFlags_AlphaPreview);
				ImGui::SameLine(300.f * State.dpiScale);
				ImGui::ColorEdit4("Уровень Игрока", (float*)&State.LevelColor, ImGuiColorEditFlags__OptionsDefault | ImGuiColorEditFlags_NoInputs | ImGuiColorEditFlags_AlphaBar | ImGuiColorEditFlags_AlphaPreview);

				ImGui::ColorEdit4("Платформа", (float*)&State.PlatformColor, ImGuiColorEditFlags__OptionsDefault | ImGuiColorEditFlags_NoInputs | ImGuiColorEditFlags_AlphaBar | ImGuiColorEditFlags_AlphaPreview);
				ImGui::SameLine(150.f * State.dpiScale);
				ImGui::ColorEdit4("Используемый Мод", (float*)&State.ModUsageColor, ImGuiColorEditFlags__OptionsDefault | ImGuiColorEditFlags_NoInputs | ImGuiColorEditFlags_AlphaBar | ImGuiColorEditFlags_AlphaPreview);
				ImGui::SameLine(300.f * State.dpiScale);
				ImGui::ColorEdit4("Проверка Ников", (float*)&State.NameCheckerColor, ImGuiColorEditFlags__OptionsDefault | ImGuiColorEditFlags_NoInputs | ImGuiColorEditFlags_AlphaBar | ImGuiColorEditFlags_AlphaPreview);

				ImGui::ColorEdit4("Код Друга", (float*)&State.FriendCodeColor, ImGuiColorEditFlags__OptionsDefault | ImGuiColorEditFlags_NoInputs | ImGuiColorEditFlags_AlphaBar | ImGuiColorEditFlags_AlphaPreview);
				ImGui::SameLine(150.f * State.dpiScale);
				ImGui::ColorEdit4("Dater Names", (float*)&State.DaterNamesColor, ImGuiColorEditFlags__OptionsDefault | ImGuiColorEditFlags_NoInputs | ImGuiColorEditFlags_AlphaBar | ImGuiColorEditFlags_AlphaPreview);
				ImGui::SameLine(300.f * State.dpiScale);
				ImGui::ColorEdit4("Код Лобби", (float*)&State.LobbyCodeColor, ImGuiColorEditFlags__OptionsDefault | ImGuiColorEditFlags_NoInputs | ImGuiColorEditFlags_AlphaBar | ImGuiColorEditFlags_AlphaPreview);
				
				ImGui::ColorEdit4("Возраст Лобби", (float*)&State.AgeColor, ImGuiColorEditFlags__OptionsDefault | ImGuiColorEditFlags_NoInputs | ImGuiColorEditFlags_AlphaBar | ImGuiColorEditFlags_AlphaPreview);

				if (AnimatedButton("Сбросить Другие Цвета")) {
					State.HostColor = ImVec4(1.f, 0.73f, 0.f, 1.f);
					State.PlayerIdColor = ImVec4(1.f, 0.f, 0.f, 1.f);
					State.LevelColor = ImVec4(0.f, 1.f, 0.f, 1.f);
					State.PlatformColor = ImVec4(0.73f, 0.f, 1.f, 1.f);
					State.ModUsageColor = ImVec4(1.f, 0.73f, 0.f, 1.f);
					State.NameCheckerColor = ImVec4(1.f, 0.67f, 0.f, 1.f);
					State.FriendCodeColor = ImVec4(0.2f, 0.6f, 1.f, 1.f);
					State.DaterNamesColor = ImVec4(1.f, 0.f, 0.f, 1.f);
					State.LobbyCodeColor = ImVec4(1.f, 0.73f, 0.f, 1.f);
					State.AgeColor = ImVec4(0.f, 1.f, 0.f, 1.f);
					State.Save();
				}
			}
		}

		if (openKeybinds) {
			State.KeybindsBeingEdited = false; // This should not stay on permanently

			CheckKeybindEdit(HotKey(State.KeyBinds.Toggle_Menu));
			ImGui::SameLine(100 * State.dpiScale);
			ImGui::Text("Показать/Спрятать Меню");

			ImGui::Dummy(ImVec2(4, 4) * State.dpiScale);

			CheckKeybindEdit(HotKey(State.KeyBinds.Toggle_Console));
			ImGui::SameLine(100 * State.dpiScale);
			ImGui::Text("Показать/Спрятать Консоль");

			ImGui::Dummy(ImVec2(4, 4) * State.dpiScale);

			CheckKeybindEdit(HotKey(State.KeyBinds.Toggle_Radar));
			ImGui::SameLine(100 * State.dpiScale);
			ImGui::Text("Показать/Спрятать Радар");

			ImGui::Dummy(ImVec2(4, 4) * State.dpiScale);

			CheckKeybindEdit(HotKey(State.KeyBinds.Toggle_Replay));
			ImGui::SameLine(100 * State.dpiScale);
			ImGui::Text("Показать/Спрятать Повтор");

			ImGui::Dummy(ImVec2(4, 4)* State.dpiScale);

			CheckKeybindEdit(HotKey(State.KeyBinds.Toggle_ChatAlwaysActive));
			ImGui::SameLine(100 * State.dpiScale);
			ImGui::Text("Показать/Спрятать Чат");

			ImGui::Dummy(ImVec2(4, 4) * State.dpiScale);

			CheckKeybindEdit(HotKey(State.KeyBinds.Toggle_ReadGhostMessages));
			ImGui::SameLine(100 * State.dpiScale);
			ImGui::Text("Читать Сообщения Призраков");

			ImGui::Dummy(ImVec2(4, 4) * State.dpiScale);

			CheckKeybindEdit(HotKey(State.KeyBinds.Toggle_Sicko));
			ImGui::SameLine(100 * State.dpiScale);
			ImGui::Text("ПАНИКА");

			ImGui::Dummy(ImVec2(4, 4) * State.dpiScale);

			CheckKeybindEdit(HotKey(State.KeyBinds.Toggle_Hud));
			ImGui::SameLine(100 * State.dpiScale);
			ImGui::Text("Включить/Выключить HUD");

			ImGui::Dummy(ImVec2(4, 4) * State.dpiScale);

			CheckKeybindEdit(HotKey(State.KeyBinds.Toggle_Freecam));
			ImGui::SameLine(100 * State.dpiScale);
			ImGui::Text("Свободная Камера");

			ImGui::Dummy(ImVec2(4, 4) * State.dpiScale);

			CheckKeybindEdit(HotKey(State.KeyBinds.Toggle_Zoom));
			ImGui::SameLine(100 * State.dpiScale);
			ImGui::Text("Приближение");

			ImGui::Dummy(ImVec2(4, 4) * State.dpiScale);

			CheckKeybindEdit(HotKey(State.KeyBinds.Toggle_Noclip));
			ImGui::SameLine(100 * State.dpiScale);
			ImGui::Text("Ноуклип");

			ImGui::Dummy(ImVec2(4, 4) * State.dpiScale);

			CheckKeybindEdit(HotKey(State.KeyBinds.Toggle_Autokill));
			ImGui::SameLine(100 * State.dpiScale);
			ImGui::Text("Авто Убийство");

			ImGui::Dummy(ImVec2(4, 4) * State.dpiScale);

			CheckKeybindEdit(HotKey(State.KeyBinds.Reset_Appearance));
			ImGui::SameLine(100 * State.dpiScale);
			ImGui::Text("Сбросить Внешний Вид");

			ImGui::Dummy(ImVec2(4, 4) * State.dpiScale);

			CheckKeybindEdit(HotKey(State.KeyBinds.Randomize_Appearance));
			ImGui::SameLine(100 * State.dpiScale);
			ImGui::Text("Запутать Вид");

			ImGui::Dummy(ImVec2(4, 4) * State.dpiScale);

			CheckKeybindEdit(HotKey(State.KeyBinds.Repair_Sabotage));
			ImGui::SameLine(100 * State.dpiScale);
			ImGui::Text("Починить Саботажи");

			ImGui::Dummy(ImVec2(4, 4) * State.dpiScale);

			CheckKeybindEdit(HotKey(State.KeyBinds.Close_All_Doors));
			ImGui::SameLine(100 * State.dpiScale);
			ImGui::Text("Закрыть Все Двери");

			ImGui::Dummy(ImVec2(4, 4) * State.dpiScale);

			CheckKeybindEdit(HotKey(State.KeyBinds.Close_Current_Room_Door));
			ImGui::SameLine(100 * State.dpiScale);
			ImGui::Text("Закрыть Дверь Текущей Комнаты");

			ImGui::Dummy(ImVec2(4, 4) * State.dpiScale);

			CheckKeybindEdit(HotKey(State.KeyBinds.Complete_Tasks));
			ImGui::SameLine(100 * State.dpiScale);
			ImGui::Text("Завершить Все Задания");

			ImGui::Dummy(ImVec2(4, 4) * State.dpiScale);

			CheckKeybindEdit(HotKey(State.KeyBinds.Cancel_Start));
			ImGui::SameLine(100 * State.dpiScale);
			ImGui::Text("Отменить Старт Игры");
		}
		ImGui::EndChild();
	}
}

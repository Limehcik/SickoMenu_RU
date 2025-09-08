#include "pch-il2cpp.h"
#include "game_tab.h"
#include "game.h"
#include "gui-helpers.hpp"
#include "utility.h"
#include "state.hpp"
#include "logger.h"
/*#include <hunspell/hunspell.hxx>
#include <sstream>
#include <string>
#include <vector>
#include "imgui.h"

class SpellChecker {
public:
    SpellChecker(const std::string& affPath, const std::string& dicPath) {
        if (!Hunspell::isAvailable()) {
            throw std::runtime_error("Hunspell is not available.");
        }
        spell = new Hunspell(affPath.c_str(), dicPath.c_str());
        if (!spell->load()) {
            delete spell;
            throw std::runtime_error("Failed to load Hunspell dictionary.");
        }
    }

    ~SpellChecker() {
        delete spell;
    }

    bool isCorrect(const std::string& word) const {
        return spell->spell(word.c_str());
    }

private:
    Hunspell* spell;
};

void HighlightMisspelledWords(SpellChecker& checker, const std::string& text) {
    std::istringstream iss(text);
    std::string word;

    while (iss >> word) {

        bool isCorrect = checker.isCorrect(word);

        if (!isCorrect) {

            ImGui::TextColored(ImVec4(1.0f, 0.0f, 0.0f, 1.0f), "%s", word.c_str());
        } else {

            ImGui::Text("%s ", word.c_str());
        }
    }
}

void RenderMenu() {
    try {
        SpellChecker spellChecker("en_US.aff", "en_US.dic");

        std::string chatMessage = "Ths is a smaple text with sme misspelled wrds.";

        if (ToggleButton("Blocked Words", &State.SMAC_CheckBadWords)) State.Save();
        if (State.SMAC_CheckBadWords) {
            HighlightMisspelledWords(spellChecker, chatMessage);

            static std::string newWord = "";
            InputString("New Word", &newWord, ImGuiInputTextFlags_EnterReturnsTrue);
            ImGui::SameLine();
            if (AnimatedButton("Add Word")) {
                State.SMAC_BadWords.push_back(newWord);
                State.Save();
                newWord = "";
            }


        }
    } catch (const std::exception& e) {

        ImGui::TextColored(ImVec4(1.0f, 0.0f, 0.0f, 1.0f), "Error: %s", e.what());
    }
}


bool ToggleButton(const char* label, bool* p_value) {
    return ImGui::Checkbox(label, p_value);
}

void InputString(const char* label, std::string* str, int flags = 0) {
    ImGui::InputText(label, &(*str)[0], str->capacity() + 1, flags);
}


struct State {
    bool SMAC_CheckBadWords;
    void Save() {}
    static std::vector<std::string> SMAC_BadWords;
};

std::vector<std::string> State::SMAC_BadWords;

int main() {


    while (true) {
        RenderMenu();


    }

    return 0;
}

*/

static std::string strToLower(std::string str) {
    std::string new_str = "";
    for (auto i : str) {
        new_str += char(std::tolower(i));
    }
    return new_str;
}

namespace GameTab {
    enum Groups {
        General,
        Chat,
        Anticheat,
        Utils,
        Options
    };

    static bool openGeneral = true;
    static bool openChat = false;
    static bool openAnticheat = false;
    static bool openUtils = false;
    static bool openOptions = false;

    void CloseOtherGroups(Groups group) {
        openGeneral = group == Groups::General;
        openChat = group == Groups::Chat;
        openAnticheat = group == Groups::Anticheat;
        openUtils = group == Groups::Utils;
        openOptions = group == Groups::Options;
    }

    void Render() {
        ImGui::SameLine(100 * State.dpiScale);
        ImGui::BeginChild("###Game", ImVec2(500 * State.dpiScale, 0), true, ImGuiWindowFlags_NoBackground);
        if (TabGroup("Главная", openGeneral)) {
            CloseOtherGroups(Groups::General);
        }
        ImGui::SameLine();
        if (TabGroup("Чат", openChat)) {
            CloseOtherGroups(Groups::Chat);
        }
        ImGui::SameLine();
        if (TabGroup("Античит", openAnticheat)) {
            CloseOtherGroups(Groups::Anticheat);
        }
        ImGui::SameLine();
        if (TabGroup("Утилиты", openUtils)) {
            CloseOtherGroups(Groups::Utils);
        }

        if (GameOptions().HasOptions() && (IsInGame() || IsInLobby())) {
            ImGui::SameLine();
            if (TabGroup("Настройки", openOptions)) {
                CloseOtherGroups(Groups::Options);
            }
        }

        enum WarnViewType {
            WarnView_List = 0,
            WarnView_Manual,
            WarnView_COUNT
        };

        static int selectedWarnView = 0;
        const char* warnViewModes[WarnView_COUNT] = {
            "List View",
            "Manual Warn"
        };

        if (openGeneral) {
            ImGui::Dummy(ImVec2(2, 2) * State.dpiScale);
            if (SteppedSliderFloat("Множитель Скорости Игрока", &State.PlayerSpeed, 0.f, 10.f, 0.05f, "%.2fx", ImGuiSliderFlags_Logarithmic | ImGuiSliderFlags_NoInput)) {
                State.PrevPlayerSpeed = State.PlayerSpeed;
            }
            if (SteppedSliderFloat("Дистанция Убийства", &State.KillDistance, 0.f, 20.f, 0.1f, "%.1f m", ImGuiSliderFlags_Logarithmic | ImGuiSliderFlags_NoInput)) {
                State.PrevKillDistance = State.KillDistance;
            }
            /*if (GameOptions().GetGameMode() == GameModes__Enum::Normal) {
                if (CustomListBoxInt("Task Bar Updates", &State.TaskBarUpdates, TASKBARUPDATES, 225 * State.dpiScale))
                    State.PrevTaskBarUpdates = State.TaskBarUpdates;
            }*/
            if (ToggleButton("Без Задержки Способностей", &State.NoAbilityCD)) {
                State.Save();
            }
            ImGui::SameLine();
            if (ToggleButton("Умножать Скорость", &State.MultiplySpeed)) {
                State.Save();
            }
            ImGui::SameLine();
            if (ToggleButton("Изменять Дистанцию Убийства", &State.ModifyKillDistance)) {
                State.Save();
            }

            ImGui::Dummy(ImVec2(7, 7) * State.dpiScale);
            ImGui::Separator();
            ImGui::Dummy(ImVec2(7, 7) * State.dpiScale);

            if (IsHost() || !State.SafeMode) {
                CustomListBoxInt(" ", &State.SelectedColorId, HOSTCOLORS, 85.0f * State.dpiScale);
            }
            else {
                if (State.SelectedColorId >= (int)COLORS.size()) State.SelectedColorId = 0;
                CustomListBoxInt(" ", &State.SelectedColorId, COLORS, 85.0f * State.dpiScale);
            }
            ImGui::SameLine();
            if (AnimatedButton("Случайный Цвет"))
            {
                State.SelectedColorId = GetRandomColorId();
            }

            if (IsInGame() || IsInLobby()) {
                ImGui::SameLine();
                if (AnimatedButton("Установить Цвет"))
                {
                    if (IsHost() || !State.SafeMode) {
                        if (IsInGame())
                            State.rpcQueue.push(new RpcForceColor(*Game::pLocalPlayer, State.SelectedColorId));
                        else if (IsInLobby())
                            State.lobbyRpcQueue.push(new RpcForceColor(*Game::pLocalPlayer, State.SelectedColorId));
                    }
                    else if (IsColorAvailable(State.SelectedColorId)) {
                        if (IsInGame())
                            State.rpcQueue.push(new RpcSetColor(State.SelectedColorId));
                        else if (IsInLobby())
                            State.lobbyRpcQueue.push(new RpcSetColor(State.SelectedColorId));
                    }
                }
            }
            ImGui::SameLine();
            if (ToggleButton("Snipe Color", &State.SnipeColor)) {
                State.Save();
            }

            if (ToggleButton("Консоль", &State.ShowConsole)) {
                State.Save();
            }

            /*if (ToggleButton("Auto-Join", &State.AutoJoinLobby))
                State.Save();
            ImGui::SameLine();
            if (InputString("Lobby Code", &State.AutoJoinLobbyCode))
                State.Save();

            if (AnimatedButton("Join Lobby")) {
                AmongUsClient_CoJoinOnlineGameFromCode(*Game::pAmongUsClient,
                    GameCode_GameNameToInt(convert_to_string(State.AutoJoinLobbyCode), NULL),
                    NULL);
            }*/

            if (IsInGame() || IsInLobby()) ImGui::SameLine();
            if ((IsInGame() || IsInLobby()) && AnimatedButton("Сбросить Внешний Вид"))
            {
                ControlAppearance(false);
            }


            if (IsInGame() && (IsHost() || !State.SafeMode) && AnimatedButton("Убить Всех")) {
                for (auto player : GetAllPlayerControl()) {
                    if (IsInGame() && (IsHost() || !State.SafeMode)) {
                        if (IsInGame())
                            State.rpcQueue.push(new RpcMurderPlayer(*Game::pLocalPlayer, player));
                        else if (IsInLobby())
                            State.lobbyRpcQueue.push(new RpcMurderPlayer(*Game::pLocalPlayer, player));
                    }
                    else {
                        if (IsInGame())
                            State.rpcQueue.push(new FakeMurderPlayer(*Game::pLocalPlayer, player));
                        else if (IsInLobby())
                            State.lobbyRpcQueue.push(new FakeMurderPlayer(*Game::pLocalPlayer, player));
                    }
                }
            }
            if (IsInLobby() && !State.SafeMode) ImGui::SameLine();
            if (IsInLobby() && !State.SafeMode && AnimatedButton("Дать Всем Ноуклип")) {
                for (auto p : GetAllPlayerControl()) {
                    if (p != *Game::pLocalPlayer) State.lobbyRpcQueue.push(new RpcMurderLoop(*Game::pLocalPlayer, p, 1, true));
                }
                State.NoClip = true;
                ShowHudNotification("Всем Выдан Ноуклип!");
            }
            /*if (IsHost() && (IsInGame() || IsInLobby()) && AnimatedButton("Spawn Dummy")) {
                auto outfit = GetPlayerOutfit(GetPlayerData(*Game::pLocalPlayer));
                if (IsInGame()) State.rpcQueue.push(new RpcSpawnDummy(outfit->fields.ColorId, convert_from_string(outfit->fields.PlayerName)));
                if (IsInLobby()) State.lobbyRpcQueue.push(new RpcSpawnDummy(outfit->fields.ColorId, convert_from_string(outfit->fields.PlayerName)));
            }*/
            if ((IsInGame() || IsInLobby()) && ((IsHost() && IsInGame()) || !State.SafeMode)) {
                ImGui::SameLine();
                if (AnimatedButton(IsHost() ? "Защитить Всех" : "Защитить Всех(Визуально)")) {
                    for (auto player : GetAllPlayerControl()) {
                        uint8_t colorId = GetPlayerOutfit(GetPlayerData(player))->fields.ColorId;
                        if (IsInGame())
                            State.rpcQueue.push(new RpcProtectPlayer(*Game::pLocalPlayer, PlayerSelection(player), colorId));
                        else if (IsInLobby())
                            State.lobbyRpcQueue.push(new RpcProtectPlayer(*Game::pLocalPlayer, PlayerSelection(player), colorId));
                    }
                }
            }

            if (IsInGame() && ToggleButton("Отключить Вентиляции", &State.DisableVents)) {
                State.Save();
            }
            if (IsInGame() && (IsHost() || !State.SafeMode)) ImGui::SameLine();
            if (IsInGame() && (IsHost() || !State.SafeMode) && ToggleButton("Спам репортами", &State.SpamReport)) {
                State.Save();
            }

            if ((IsInGame() || (IsInLobby() && State.KillInLobbies)) && (IsHost() || !State.SafeMode)) {
                if (AnimatedButton("Убить Всех Членов Экипажа")) {
                    for (auto player : GetAllPlayerControl()) {
                        if (!PlayerIsImpostor(GetPlayerData(player))) {
                            if (IsInGame())
                                State.rpcQueue.push(new RpcMurderPlayer(*Game::pLocalPlayer, player));
                            else if (IsInLobby())
                                State.lobbyRpcQueue.push(new RpcMurderPlayer(*Game::pLocalPlayer, player));
                        }
                    }
                }
                ImGui::SameLine();
                if (AnimatedButton("Убить Всех Предателей")) {
                    for (auto player : GetAllPlayerControl()) {
                        if (PlayerIsImpostor(GetPlayerData(player))) {
                            if (IsInGame())
                                State.rpcQueue.push(new RpcMurderPlayer(*Game::pLocalPlayer, player,
                                    player->fields.protectedByGuardianId < 0 || State.BypassAngelProt));
                            else if (IsInLobby())
                                State.lobbyRpcQueue.push(new RpcMurderPlayer(*Game::pLocalPlayer, player,
                                    player->fields.protectedByGuardianId < 0 || State.BypassAngelProt));
                        }
                    }
                }
                if (!State.SafeMode) {
                    ImGui::SameLine();
                    if (AnimatedButton("Самоубийство Членов экипажа")) {
                        for (auto player : GetAllPlayerControl()) {
                            if (!PlayerIsImpostor(GetPlayerData(player))) {
                                if (IsInGame())
                                    State.rpcQueue.push(new RpcMurderPlayer(player, player,
                                        player->fields.protectedByGuardianId < 0 || State.BypassAngelProt));
                                else if (IsInLobby())
                                    State.lobbyRpcQueue.push(new RpcMurderPlayer(player, player,
                                        player->fields.protectedByGuardianId < 0 || State.BypassAngelProt));
                            }
                        }
                    }
                    ImGui::SameLine();
                    if (AnimatedButton("Самоубийство Предателей")) {
                        for (auto player : GetAllPlayerControl()) {
                            if (PlayerIsImpostor(GetPlayerData(player))) {
                                if (IsInGame())
                                    State.rpcQueue.push(new RpcMurderPlayer(player, player,
                                        player->fields.protectedByGuardianId < 0 || State.BypassAngelProt));
                                else if (IsInLobby())
                                    State.lobbyRpcQueue.push(new RpcMurderPlayer(player, player,
                                        player->fields.protectedByGuardianId < 0 || State.BypassAngelProt));
                            }
                        }
                    }
                }

                static int ventId = 0;
                if (IsInGame() && (IsHost() || !State.SafeMode)) {
                    std::vector<const char*> allVents;
                    switch (State.mapType) {
                    case Settings::MapType::Ship:
                        allVents = SHIPVENTS;
                        break;
                    case Settings::MapType::Hq:
                        allVents = HQVENTS;
                        break;
                    case Settings::MapType::Pb:
                        allVents = PBVENTS;
                        break;
                    case Settings::MapType::Airship:
                        allVents = AIRSHIPVENTS;
                        break;
                    case Settings::MapType::Fungle:
                        allVents = FUNGLEVENTS;
                        break;
                    }
                    ventId = std::clamp(ventId, 0, (int)allVents.size() - 1);

                    ImGui::SetNextItemWidth(100 * State.dpiScale);
                    CustomListBoxInt("Вентиляция", &ventId, allVents);
                    ImGui::SameLine();
                    if (AnimatedButton("Телепортировать Всех к Вентиляции")) {
                        for (auto p : GetAllPlayerControl()) {
                            State.rpcQueue.push(new RpcBootFromVent(p, (State.mapType == Settings::MapType::Hq) ? ventId + 1 : ventId)); //MiraHQ vents start from 1 instead of 0
                        }
                    }
                }
            }

            if (IsInGame() || IsInLobby()) {
                if (!State.SafeMode && GameOptions().GetBool(BoolOptionNames__Enum::VisualTasks) && AnimatedButton("Сканировать Всех")) {
                    for (auto p : GetAllPlayerControl()) {
                        if (IsInGame()) State.rpcQueue.push(new RpcForceScanner(p, true));
                        else State.lobbyRpcQueue.push(new RpcForceScanner(p, true));
                    }
                }
                if (!State.SafeMode && GameOptions().GetBool(BoolOptionNames__Enum::VisualTasks)) ImGui::SameLine();
                if (!State.SafeMode && GameOptions().GetBool(BoolOptionNames__Enum::VisualTasks) && AnimatedButton("Остановить Сканирование Всех")) {
                    for (auto p : GetAllPlayerControl()) {
                        if (IsInGame()) State.rpcQueue.push(new RpcForceScanner(p, false));
                        else State.lobbyRpcQueue.push(new RpcForceScanner(p, false));
                    }
                }
                if (IsInGame() && !State.InMeeting && !State.SafeMode && GameOptions().GetBool(BoolOptionNames__Enum::VisualTasks)) ImGui::SameLine();
                if (IsInGame() && !State.InMeeting && AnimatedButton("Выкинуть Всех из Вентиляции")) {
                    State.rpcQueue.push(new RpcBootAllVents());
                }
                if ((IsHost() || !State.SafeMode) && State.InMeeting) ImGui::SameLine();
                if ((IsHost() || !State.SafeMode) && State.InMeeting && AnimatedButton("Закончить Голосование")) {
                    State.rpcQueue.push(new RpcEndMeeting());
                    State.InMeeting = false;
                }

                if (!State.SafeMode && !IsHost()) {
                    if (AnimatedButton("Установить Всем Никнейм")) {
                        for (auto p : GetAllPlayerControl()) {
                            if (IsInGame()) State.rpcQueue.push(new RpcForceName(p, std::format("{}<size=0><{}></size>", State.hostUserName, p->fields.PlayerId)));
                            if (IsInLobby()) State.lobbyRpcQueue.push(new RpcForceName(p, std::format("{}<size=0><{}></size>", State.hostUserName, p->fields.PlayerId)));
                        }
                    }
                    ImGui::SameLine();
                    if (ToggleButton("Принудительно Устанавливать Всем Никнейм", &State.ForceNameForEveryone)) {
                        State.Save();
                    }

                    if (InputString("Никнейм", &State.hostUserName)) {
                        State.Save();
                    }

                    if (AnimatedButton("Установить Всем Цвет")) {
                        for (auto p : GetAllPlayerControl()) {
                            if (IsInGame()) State.rpcQueue.push(new RpcForceColor(p, State.HostSelectedColorId));
                            if (IsInLobby()) State.lobbyRpcQueue.push(new RpcForceColor(p, State.HostSelectedColorId));
                        }
                    }
                    ImGui::SameLine();
                    if (ToggleButton("Принудительно Устанавливать Всем цвет", &State.ForceColorForEveryone)) {
                        State.Save();
                    }

                    if (CustomListBoxInt(" ­", &State.HostSelectedColorId, HOSTCOLORS, 85.0f * State.dpiScale)) State.Save();
                }
            }
        }

        if (openChat) {
            bool msgAllowed = IsChatValid(State.chatMessage);
            if (!msgAllowed) {
                ImGui::PushStyleColor(ImGuiCol_FrameBg, ImVec4(0.5f, 0.f, 0.f, State.MenuThemeColor.w));
                if (InputStringMultiline("\n\n\n\n\nПоле для Сообщения", &State.chatMessage)) State.Save();
                ImGui::PopStyleColor();
            }
            else if (InputStringMultiline("\n\n\n\n\nПоле для Сообщения", &State.chatMessage)) State.Save();
            if ((IsInGame() || IsInLobby()) && State.ChatCooldown >= 3.f && IsChatValid(State.chatMessage)) {
                ImGui::SameLine();
                if (AnimatedButton("Отправить"))
                {
                    auto player = (!State.SafeMode && State.playerToChatAs.has_value()) ?
                        State.playerToChatAs.validate().get_PlayerControl() : *Game::pLocalPlayer;
                    if (IsInGame()) State.rpcQueue.push(new RpcSendChat(player, State.chatMessage));
                    else if (IsInLobby()) State.lobbyRpcQueue.push(new RpcSendChat(player, State.chatMessage));
                    State.MessageSent = true;
                }
            }
            if ((IsInGame() || IsInLobby()) && State.ReadAndSendSickoChat) ImGui::SameLine();
            if (State.ReadAndSendSickoChat && (IsInGame() || IsInLobby()) && AnimatedButton("Отправить в AUM"))
            {
                auto player = (!State.SafeMode && State.playerToChatAs.has_value()) ?
                    State.playerToChatAs.validate().get_PlayerControl() : *Game::pLocalPlayer;
                if (IsInGame()) {
                    State.rpcQueue.push(new RpcForceSickoChat(PlayerSelection(player), State.chatMessage, true));
                }
                else if (IsInLobby()) {
                    State.lobbyRpcQueue.push(new RpcForceSickoChat(PlayerSelection(player), State.chatMessage, true));
                }
            }

            if (ToggleButton("Спам", &State.ChatSpam))
            {
                if (State.BrainrotEveryone) State.BrainrotEveryone = false;
                if (State.RizzUpEveryone) State.RizzUpEveryone = false;
                State.Save();
            }
            if (((IsHost() && IsInGame()) || !State.SafeMode) && State.ChatSpamMode) ImGui::SameLine();
            if ((IsHost() || !State.SafeMode) && State.ChatSpamMode && ToggleButton("Спамить от Всех", &State.ChatSpamEveryone))
            {
                State.Save();
            }
            if (IsHost() || !State.SafeMode) {
                if (CustomListBoxInt("Режим Спама в чат", &State.ChatSpamMode,
                    { State.SafeMode ? "С сообщением (ТОЛЬКО для самоспама)" : "С Сообщением", "Пустые Сообщения", State.SafeMode ? "Самосообщение + Пустые Сообщения" : "Сообщение + Пустой Чат" })) State.Save();
            }

            if (std::find(State.ChatPresets.begin(), State.ChatPresets.end(), State.chatMessage) == State.ChatPresets.end() && AnimatedButton("Сохранить сообщение")) {
                State.ChatPresets.push_back(State.chatMessage);
                State.Save();
            }
            if (!(IsHost() || !State.SafeMode) && State.chatMessage.size() > 120) {
                ImGui::TextColored(ImVec4(1.0f, 0.0f, 0.0f, 1.0f), "Сообщение обнаружится Античитом.");
            }
            if (!State.ChatPresets.empty()) {
                static int selectedPresetIndex = 0;
                selectedPresetIndex = std::clamp(selectedPresetIndex, 0, (int)State.ChatPresets.size() - 1);
                std::vector<const char*> presetVector(State.ChatPresets.size(), nullptr);
                for (size_t i = 0; i < State.ChatPresets.size(); i++) {
                    presetVector[i] = State.ChatPresets[i].c_str();
                }
                CustomListBoxInt("Сохраненные сообщения", &selectedPresetIndex, presetVector);
                auto msg = State.ChatPresets[selectedPresetIndex];
                if (AnimatedButton("Вставить в Поле для Сообщения"))
                {
                    State.chatMessage = msg;
                }
                ImGui::SameLine();
                if (AnimatedButton("Удалить"))
                    State.ChatPresets.erase(State.ChatPresets.begin() + selectedPresetIndex);
            }
        }

        if (openAnticheat) {
            if (ToggleButton("Включить Античит (SMAC)", &State.Enable_SMAC)) State.Save();
            if (IsHost()) CustomListBoxInt("Наказание хозяина ", &State.SMAC_HostPunishment, SMAC_HOST_PUNISHMENTS, 85.0f * State.dpiScale);
            else CustomListBoxInt("Обычное наказание", &State.SMAC_Punishment, SMAC_PUNISHMENTS, 85.0f * State.dpiScale);

            if (ToggleButton("Добавлять Читеров в Черный Список", &State.SMAC_AddToBlacklist)) State.Save();
            ImGui::SameLine();
            if (ToggleButton("Наказание Черного списка", &State.SMAC_PunishBlacklist)) State.Save();
            if (ToggleButton("Игнорировать Белый Список", &State.SMAC_IgnoreWhitelist)) State.Save();
            if (State.SMAC_PunishBlacklist) {
                ImGui::Text("Черный Список");
                if (State.BlacklistFriendCodes.empty())
                    ImGui::TextColored(ImVec4(1.0f, 0.0f, 0.0f, 1.0f), "Нет игроков в черном списке!");
                else {
                    ImGui::SameLine(0.f, 0.f);
                    ImGui::Text(" (%d Игроков в Черном списке)", State.BlacklistFriendCodes.size());
                }
                static std::string newBFriendCode = "";
				bool isInBlacklistAlready = std::find(State.BlacklistFriendCodes.begin(), State.BlacklistFriendCodes.end(), newBFriendCode) != State.BlacklistFriendCodes.end();
                InputString("Новый Код Друга", &newBFriendCode, ImGuiInputTextFlags_EnterReturnsTrue);
                if (isInBlacklistAlready)
                    ImGui::TextColored(ImVec4(1.0f, 0.0f, 0.0f, 1.0f), "Этот человек уже в черном листе!");
                if (newBFriendCode != "" && !isInBlacklistAlready) ImGui::SameLine();
                if (newBFriendCode != "" && !isInBlacklistAlready && AnimatedButton("Добавить")) {
                    State.BlacklistFriendCodes.push_back(newBFriendCode);
                    State.Save();
                    newBFriendCode = "";
                }

                if (!State.BlacklistFriendCodes.empty()) {
                    static int selectedBCodeIndex = 0;
                    selectedBCodeIndex = std::clamp(selectedBCodeIndex, 0, (int)State.BlacklistFriendCodes.size() - 1);
                    std::vector<const char*> bCodeVector(State.BlacklistFriendCodes.size(), nullptr);
                    for (size_t i = 0; i < State.BlacklistFriendCodes.size(); i++) {
                        bCodeVector[i] = State.BlacklistFriendCodes[i].c_str();
                    }
                    CustomListBoxInt("Игрок для удаления", &selectedBCodeIndex, bCodeVector);
                    ImGui::SameLine();
                    if (AnimatedButton("Удалить"))
                        State.BlacklistFriendCodes.erase(State.BlacklistFriendCodes.begin() + selectedBCodeIndex);
                }
            }
            if (State.SMAC_IgnoreWhitelist) {
                ImGui::Text("Белый Список");
                if (State.WhitelistFriendCodes.empty())
                    ImGui::TextColored(ImVec4(1.0f, 0.0f, 0.0f, 1.0f), "Нет игроков в белом списке!");
                else {
                    ImGui::SameLine(0.f, 0.f);
                    ImGui::Text(" (%d Игроков в Белом Списке)", State.WhitelistFriendCodes.size());
                }
                static std::string newWFriendCode = "";
                static bool isInWhitelistAlready = std::find(State.WhitelistFriendCodes.begin(), State.WhitelistFriendCodes.end(), newWFriendCode) != State.WhitelistFriendCodes.end();
                InputString("Новый Код Друга\n", &newWFriendCode, ImGuiInputTextFlags_EnterReturnsTrue);
                if (isInWhitelistAlready)
					ImGui::TextColored(ImVec4(1.0f, 0.0f, 0.0f, 1.0f), "Игрок уже в белом списке!");
                if (newWFriendCode != "" && !isInWhitelistAlready) ImGui::SameLine();
                if (newWFriendCode != "" && !isInWhitelistAlready && AnimatedButton("Добавить\n")) {
                    State.WhitelistFriendCodes.push_back(newWFriendCode);
                    State.Save();
                    newWFriendCode = "";
                }

                if (!State.WhitelistFriendCodes.empty()) {
                    static int selectedWCodeIndex = 0;
                    selectedWCodeIndex = std::clamp(selectedWCodeIndex, 0, (int)State.WhitelistFriendCodes.size() - 1);
                    std::vector<const char*> wCodeVector(State.WhitelistFriendCodes.size(), nullptr);
                    for (size_t i = 0; i < State.WhitelistFriendCodes.size(); i++) {
                        wCodeVector[i] = State.WhitelistFriendCodes[i].c_str();
                    }
                    CustomListBoxInt("Игрок для удаления\n", &selectedWCodeIndex, wCodeVector);
                    ImGui::SameLine();
                    if (AnimatedButton("Удалить\n"))
                        State.WhitelistFriendCodes.erase(State.WhitelistFriendCodes.begin() + selectedWCodeIndex);
                }
            }
            ImGui::Text("Обнаружаемые Действия:(пока не переведено из за сложностей вмещения)");
            if (ToggleButton(" UsageAUM/KillNetwork", &State.SMAC_CheckAUM)) State.Save();
            ImGui::SameLine();
            if (ToggleButton("SickoMenu Usage", &State.SMAC_CheckSicko)) State.Save();
            ImGui::SameLine();
            if (ToggleButton("Abnormal Names", &State.SMAC_CheckBadNames)) State.Save();

            if (ToggleButton("Abnormal Set Color", &State.SMAC_CheckColor)) State.Save();
            ImGui::SameLine();
            if (ToggleButton("Abnormal Set Cosmetics", &State.SMAC_CheckCosmetics)) State.Save();
            ImGui::SameLine();
            if (ToggleButton("Abnormal Chat Note", &State.SMAC_CheckChatNote)) State.Save();

            if (ToggleButton("Abnormal Scanner", &State.SMAC_CheckScanner)) State.Save();
            ImGui::SameLine();
            if (ToggleButton("Abnormal Animation", &State.SMAC_CheckAnimation)) State.Save();
            ImGui::SameLine();
            if (ToggleButton("Setting Tasks", &State.SMAC_CheckTasks)) State.Save();

            if (ToggleButton("Abnormal Murders", &State.SMAC_CheckMurder)) State.Save();
            ImGui::SameLine();
            if (ToggleButton("Abnormal Shapeshift", &State.SMAC_CheckShapeshift)) State.Save();
            ImGui::SameLine();
            if (ToggleButton("Abnormal Vanish", &State.SMAC_CheckVanish)) State.Save();


            if (ToggleButton("Abnormal Meetings/Body Reports", &State.SMAC_CheckReport)) State.Save();
            ImGui::SameLine();
            if (ToggleButton("Abnormal Venting", &State.SMAC_CheckVent)) State.Save();
            ImGui::SameLine();
           
            if (ToggleButton("Abnormal Chat", &State.SMAC_CheckChat)) State.Save();

            if (ToggleButton("Abnormal Task Completion", &State.SMAC_CheckTaskCompletion)) State.Save();
            ImGui::SameLine();
            if (ToggleButton("Abnormal Sabotages", &State.SMAC_CheckSabotage)) State.Save();
            if (ToggleButton("Abnormal Player Levels (0 to ignore)", &State.SMAC_CheckLevel)) State.Save();
            if (State.SMAC_CheckLevel && ImGui::InputInt("Level >=", &State.SMAC_HighLevel)) {
                State.Save();
            }
            if (State.SMAC_CheckLevel && ImGui::InputInt("Level <=", &State.SMAC_LowLevel)) {
                State.Save();
            }
            if (ToggleButton("Заблокированные Слова", &State.SMAC_CheckBadWords)) State.Save();
            if (State.SMAC_CheckBadWords) {
                if (State.SMAC_BadWords.empty())
                    ImGui::TextColored(ImVec4(1.0f, 0.0f, 0.0f, 1.0f), "Не добавлено слов!");
                static std::string newWord = "";
                InputString("Новое Слово", &newWord, ImGuiInputTextFlags_EnterReturnsTrue);
                ImGui::SameLine();
                if (AnimatedButton("Добавить Слово")) {
                    State.SMAC_BadWords.push_back(newWord);
                    State.Save();
                    newWord = "";
                }
                if (!State.SMAC_BadWords.empty()) {
                    static int selectedWordIndex = 0;
                    selectedWordIndex = std::clamp(selectedWordIndex, 0, (int)State.SMAC_BadWords.size() - 1);
                    std::vector<const char*> wordVector(State.SMAC_BadWords.size(), nullptr);
                    for (size_t i = 0; i < State.SMAC_BadWords.size(); i++) {
                        wordVector[i] = State.SMAC_BadWords[i].c_str();
                    }
                    CustomListBoxInt("Слово для Удаления", &selectedWordIndex, wordVector);
                    ImGui::SameLine();
                    if (AnimatedButton("Удалить"))
                        State.SMAC_BadWords.erase(State.SMAC_BadWords.begin() + selectedWordIndex);
                }
            }
        }

        if (openUtils) {
            /*if (ToggleButton("Ignore Whitelisted Players [Exploits]", &State.Destruct_IgnoreWhitelist)) {
                State.Save();
            }*/
            if (ToggleButton("Игнорировать Игроков из Белого Списка [Бан/Кик]", &State.Ban_IgnoreWhitelist)) {
                State.Save();
            }
            if (IsInLobby() && ToggleButton("Попытка Краша Лобби(При Старте Игры)", &State.CrashSpamReport)) {
                State.Save();
            }
            if (State.CrashSpamReport) ImGui::TextColored(ImVec4(1.0f, 1.0f, 1.0f, 1.0f), ("Когда Игра в Лобби Запустится, Лобби Будет Уничтожено"));
            if (State.AprilFoolsMode) {
                ImGui::TextColored(ImVec4(0.79f, 0.03f, 1.f, 1.f), State.DiddyPartyMode ? "Режим вечеринки Дидди" : (IsChatCensored() || IsStreamerMode() ? "Х***в Режим" : "Хуев Режим"));
                if (ToggleButton("Mog Everyone [Sigma]", &State.BrainrotEveryone)) {
                    if (State.ChatSpam) State.ChatSpam = false;
                    if (State.RizzUpEveryone) State.RizzUpEveryone = false;
                    State.Save();
                }
                if (State.DiddyPartyMode && ToggleButton("Rizz Up Everyone [Skibidi]", &State.RizzUpEveryone)) {
                    if (State.ChatSpam) State.ChatSpam = false;
                    if (State.BrainrotEveryone) State.BrainrotEveryone = false;
                    State.Save();
                }
            }
            if (IsHost()) {
                ImGui::Dummy(ImVec2(5, 5) * State.dpiScale);
                if (((IsInGame() && Object_1_IsNotNull((Object_1*)*Game::pShipStatus)) || (IsInLobby() && Object_1_IsNotNull((Object_1*)*Game::pLobbyBehaviour)))
                    && AnimatedButton(IsInLobby() ? "Удалить Лобби" : "Удалить Карту")) {
                    State.taskRpcQueue.push(new DestroyMap());
                }
                ImGui::Dummy(ImVec2(7, 7) * State.dpiScale);
                if (ToggleButton("Забанить Всех", &State.BanEveryone)) {
                    State.Save();
                }
                if (ToggleButton("Кикнуть Всех", &State.KickEveryone)) {
                    State.Save();
                }
                if (SteppedSliderFloat("Задержка Кика/Бана", &State.AutoPunishDelay, 0.f, 10.f, 0.1f, "%.1f", ImGuiSliderFlags_NoInput)) {
                    State.Save();
                }
                ImGui::Dummy(ImVec2(7, 7) * State.dpiScale);
                const char* buttonLabel = IsInGame() ? "Кикать AFK Игроков" : "Кикнуть AFK Игроков [Только в игре]";
                if (ToggleButton(buttonLabel, &State.KickAFK)) {
                    State.Save();
                }
                if (State.KickAFK) ImGui::SameLine();
                if (State.KickAFK && ToggleButton("Включить уведомления Warn-AFK", &State.NotificationsAFK)) {
                    State.Save();
                }
                if (State.KickAFK && ToggleButton("AFK - Второй шанс", &State.SecondChance)) {
                    State.Save();
                }
                std::string header = "Анти-AFK ~ Расширенные настройки";
                if (!IsInGame()) {
                    header += " [GAME-MATCH]";
                }
                ImGui::Dummy(ImVec2(5, 5) * State.dpiScale);
                if (State.KickAFK && ImGui::CollapsingHeader(header.c_str()))
                {
                    if (SteppedSliderFloat("Время до Кика", &State.TimerAFK, 40.f, 350.f, 1.f, "%.0f", ImGuiSliderFlags_NoInput)) {
                        State.Save();
                    }
                    if (State.SecondChance && SteppedSliderFloat("Дополнительное Время", &State.AddExtraTime, 15.f, 120.f, 1.f, "%.0f", ImGuiSliderFlags_NoInput)) {
                        State.Save();
                    }
                    if (State.SecondChance && SteppedSliderFloat("Мин. время перед добавлением", &State.ExtraTimeThreshold, 5.f, 60.f, 1.f, "%.0f", ImGuiSliderFlags_NoInput)) {
                        State.Save();
                    }
                    if (State.NotificationsAFK && SteppedSliderFloat("Время уведомлений Warn-AFK", &State.NotificationTimeWarn, 5.f, 60.f, 1.f, "%.0f", ImGuiSliderFlags_NoInput)) {
                        State.Save();
                    }
                }
                ImGui::Dummy(ImVec2(3, 3) * State.dpiScale);
                ImGui::Separator();
                ImGui::Dummy(ImVec2(3, 3) * State.dpiScale);
                if (ToggleButton("Только Игроки из Белого Списка", &State.KickByWhitelist)) {
                    State.Save();
                }
                if (State.KickByWhitelist) ImGui::SameLine();
                if (State.KickByWhitelist && ToggleButton("Включить уведомления WL", &State.WhitelistNotifications)) {
                    State.Save();
                }
                ImGui::Dummy(ImVec2(15, 15) * State.dpiScale);
                if (ToggleButton("Банить игроков за Пере Заход", &State.BanLeavers)) {
                    State.Save();
                }
                ImGui::Dummy(ImVec2(5, 5) * State.dpiScale);
                if (ImGui::CollapsingHeader("БАн-ПЗ ~ Дополнительные Настройки"))
                {
                    if (SteppedSliderFloat("Максимум Пере Заходов", &State.LeaveCount, 1.f, 15.f, 1.f, "%.0f", ImGuiSliderFlags_NoInput)) {
                        State.Save();
                    }
                    ImGui::Dummy(ImVec2(5, 5) * State.dpiScale);
                    if (ToggleButton("Черный Список Игроков с Авто Пере Заходом", &State.BL_AutoLeavers)) {
                        State.Save();
                    }
                }
                ImGui::Dummy(ImVec2(3, 3) * State.dpiScale);
                ImGui::Separator();
                ImGui::Dummy(ImVec2(3, 3) * State.dpiScale);
                if (ToggleButton("Предупреждение/Кик от Проверки Никнеймов", &State.KickByLockedName)) {
                    State.Save();
                }
                if (State.KickByLockedName) ImGui::SameLine();
                if (State.KickByLockedName && ToggleButton("Показывать Уведомления о Данных Игрока", &State.ShowPDataByNC)) {
                    State.Save();
                }
                if (State.KickByLockedName) {
                    ImGui::Text("Заблокированные Никнеймы");
                    if (State.LockedNames.empty())
                        ImGui::TextColored(ImVec4(1.0f, 0.0f, 0.0f, 1.0f), "Нет заблокированых никнеймов!");
                    static std::string newName = "";
                    InputString("Новый Никнейм", &newName, ImGuiInputTextFlags_EnterReturnsTrue);
                    if (newName != "") ImGui::SameLine();
                    if (newName != "" && AnimatedButton("Добавить")) {
						newName = strToLower(newName);
                        State.LockedNames.push_back(newName);
                        State.Save();
                        newName = "";
                    }

                    if (!State.LockedNames.empty()) {
                        static int selectedName = 0;
                        selectedName = std::clamp(selectedName, 0, (int)State.LockedNames.size() - 1);
                        std::vector<const char*> bNameVector(State.LockedNames.size(), nullptr);
                        for (size_t i = 0; i < State.LockedNames.size(); i++) {
                            bNameVector[i] = State.LockedNames[i].c_str();
                        }
                        CustomListBoxInt("Никнейм для Удаления", &selectedName, bNameVector);
                        ImGui::SameLine();
                        if (AnimatedButton("Удалить"))
                            State.LockedNames.erase(State.LockedNames.begin() + selectedName);
                    }
                }
                ImGui::Dummy(ImVec2(15, 15) * State.dpiScale);
                ImGui::BeginGroup();
                if (ToggleButton("Кик Игроков с Предупреждением", &State.KickWarned)) {
                    State.Save();
                }
                if (ToggleButton("Забанить Игроков с Предупреждением", &State.BanWarned)) {
                    State.Save();
                }
                if (ToggleButton("Оповестить Игроков с Предупреждением", &State.NotifyWarned)) {
                    State.Save();
                }

                ImGui::Dummy(ImVec2(5, 5) * State.dpiScale);

                ImGui::PushItemWidth(80);
                ImGui::InputInt("Максимальные Предупреждения", &State.MaxWarns);
                if (State.MaxWarns < 1)
                    State.MaxWarns = 1;
                ImGui::PopItemWidth();
                ImGui::EndGroup();
            }
            if (IsHost()) ImGui::SameLine();
            ImGui::BeginGroup();
            ImGui::PushItemWidth(150);
            if (!IsHost()) ImGui::Dummy(ImVec2(5, 5) * State.dpiScale);
            ImGui::Combo("Просмотр Прежупреждений", &selectedWarnView, warnViewModes, WarnView_COUNT);
            ImGui::PopItemWidth();


            if (selectedWarnView == WarnView_List) {
                if (!State.WarnedFriendCodes.empty()) {
                    ImGui::Text("Предупрежденые Игроки");

                    std::string localFC = "";
                    if (Game::pLocalPlayer && *Game::pLocalPlayer) {
                        localFC = convert_from_string((*Game::pLocalPlayer)->fields.FriendCode);
                    }

                    std::vector<std::string> warnedList;
                    std::vector<std::string> fcKeys;

                    for (const auto& [fc, count] : State.WarnedFriendCodes) {
                        if (count <= 0 || fc == localFC)
                            continue;

                        warnedList.push_back(std::format("{} ({} предупреждений{})", fc, count, count == 1 ? "" : "s"));
                        fcKeys.push_back(fc);
                    }

                    if (!warnedList.empty()) {
                        static int selectedWarned = 0;
                        selectedWarned = std::clamp(selectedWarned, 0, (int)warnedList.size() - 1);

                        std::vector<const char*> warnedCStrs;
                        for (const auto& entry : warnedList) warnedCStrs.push_back(entry.c_str());

                        ImGui::PushItemWidth(200);
                        CustomListBoxInt("Предупрежденые по Коду", &selectedWarned, warnedCStrs);
                        ImGui::PopItemWidth();

                        ImGui::SameLine();
                        if (ImGui::Button("Удалить")) {
                            if (selectedWarned >= 0 && selectedWarned < (int)fcKeys.size()) {
                                std::string fc = fcKeys[selectedWarned];
                                State.WarnedFriendCodes.erase(fc);
                                State.WarnReasons.erase(fc);
                                selectedWarned = 0;
                                State.Save();
                            }
                        }

                        std::string selectedFc = fcKeys[selectedWarned];
                        auto& warnReasons = State.WarnReasons[selectedFc];

                        if (!warnReasons.empty()) {
                            ImGui::Text("Максимальные Причины:");

                            static int selectedReason = 0;
                            selectedReason = std::clamp(selectedReason, 0, (int)warnReasons.size() - 1);

                            std::vector<std::string> numberedReasons;
                            numberedReasons.reserve(warnReasons.size());
                            for (size_t i = 0; i < warnReasons.size(); ++i) {
                                numberedReasons.push_back(std::format("[{}] {}", i + 1, warnReasons[i]));
                            }

                            std::vector<const char*> reasonCStrs;
                            for (const auto& str : numberedReasons) reasonCStrs.push_back(str.c_str());

                            ImGui::PushItemWidth(200);
                            ImGui::ListBox("##Лист Причин Предупреждений", &selectedReason, reasonCStrs.data(), (int)reasonCStrs.size());
                            ImGui::PopItemWidth();

                            ImGui::SameLine();
                            if (ImGui::Button("Удалить")) {
                                if (selectedReason >= 0 && selectedReason < (int)warnReasons.size()) {
                                    warnReasons.erase(warnReasons.begin() + selectedReason);
                                    selectedReason = 0;

                                    if (--State.WarnedFriendCodes[selectedFc] <= 0) {
                                        State.WarnedFriendCodes.erase(selectedFc);
                                        State.WarnReasons.erase(selectedFc);
                                        selectedWarned = 0;
                                    }

                                    State.Save();
                                }
                            }
                        }
                    }
                    else {
                        ImGui::TextColored(ImVec4(1.f, 0.f, 0.f, 1.f), "Нет предупрежденых игроков.");
                    }
                }
            }
            else if (selectedWarnView == WarnView_Manual) {
                static std::string friendCodeToWarn;
                static std::string warnReason;

                ImGui::PushItemWidth(200);
                InputString("FriendCode##warn", &friendCodeToWarn);
                InputString("Reason", &warnReason);
                ImGui::PopItemWidth();

                if (ImGui::Button("Отправить Предупреждение") && !friendCodeToWarn.empty() && !warnReason.empty()) {
                    State.WarnedFriendCodes[friendCodeToWarn]++;
                    State.WarnReasons[friendCodeToWarn].push_back(warnReason);
                    State.Save();

                    friendCodeToWarn.clear();
                    warnReason.clear();
                }
            }

            ImGui::EndGroup();

            ImGui::Dummy(ImVec2(3, 3) * State.dpiScale);
            ImGui::Separator();
            ImGui::Dummy(ImVec2(3, 3) * State.dpiScale);

            /*if (ToggleButton("Enable Temp-Ban System", &State.EnableTempBan)) {
                State.Save();
			}
            if (State.EnableTempBan && ImGui::CollapsingHeader("Temp-Ban System"))
            {
				static std::string friendCodeToTempBan;
                static int banDays = 0;
                static int banHours = 0;
                static int banMinutes = 0;
                static int banSeconds = 0;
                ImGui::BeginGroup();

                ImGui::PushItemWidth(150);
                InputString("Friend Code", &friendCodeToTempBan);

                if (ImGui::InputInt("Days", &banDays) && banDays < 0) banDays = 0;
                if (ImGui::InputInt("Hours", &banHours) && banHours < 0) banHours = std::clamp(banHours, 0, 23);
                if (ImGui::InputInt("Minutes", &banMinutes) && banMinutes < 0) banMinutes = std::clamp(banMinutes, 0, 59);
                if (ImGui::InputInt("Seconds", &banSeconds) && banSeconds < 0) banSeconds = std::clamp(banSeconds, 0, 59);
                ImGui::PopItemWidth();

                if (!friendCodeToTempBan.empty() && ImGui::Button("Submit Temp-Ban")) {
                    int totalSeconds = 0;
                    totalSeconds += SafeMax(0, banDays) * 86400;
                    totalSeconds += SafeMax(0, banHours) * 3600;
                    totalSeconds += SafeMax(0, banMinutes) * 60;
                    totalSeconds += SafeMax(0, banSeconds);

                    if (totalSeconds > 0) {
                        auto now = std::chrono::system_clock::now();
                        auto banEndTime = now + std::chrono::seconds(totalSeconds);
						int banEndSaveTime = static_cast<int>(banEndTime.time_since_epoch().count());
                        //State.TempBannedFriendCodes[State.TBanFC] = {banEndTime, std::to_string(now.time_since_epoch().count())};
                        //State.TBanFC.clear();
                        //State.BanDays = State.BanHours = State.BanMinutes = State.BanSeconds = 0;
                        State.TempBannedFCs[friendCodeToTempBan] = banEndSaveTime;
                        if (IsInGame() || IsInLobby()) {
                            for (auto p : GetAllPlayerControl()) {
                                if (convert_from_string(p->fields.FriendCode) != friendCodeToTempBan) continue;
                                if (IsInGame()) {
                                    State.rpcQueue.push(new PunishPlayer(p, false));
                                }
                                if (IsInLobby()) {
                                    State.lobbyRpcQueue.push(new PunishPlayer(p, false));
                                }
                            }
                        }
                        State.Save();
                    }
                }
                ImGui::EndGroup();
                ImGui::SameLine();

                ImGui::BeginGroup();
                ImGui::Text("Temp-Banned Players:");

                auto now = std::chrono::system_clock::now();
                for (auto it = State.TempBannedFCs.begin(); it != State.TempBannedFCs.end(); ) {
                    if (now.time_since_epoch().count() >= it->second) {
                        State.TempBannedFCs.erase(it);
                    }
                    else {
						++it;
                    }
                }

                if (State.TempBannedFCs.empty()) {
                    ImGui::TextColored(ImVec4(1.0f, 0.0f, 0.0f, 1.0f), "No players are temporarily banned.");
                }
                else {
                    static int selectedTempBanIndex = 0;
                    std::vector<std::string> displayList;
                    std::vector<std::string> friendCodeList;

                    for (const auto& [fc, banTime] : State.TempBannedFCs) {
                        auto timeLeft = banTime - now.time_since_epoch().count();
                        if (timeLeft < 0) timeLeft = 0;

                        int d = static_cast<int>(timeLeft / 86400);
                        int h = static_cast<int>((timeLeft % 86400) / 3600);
                        int m = static_cast<int>((timeLeft % 3600) / 60);
                        int s = static_cast<int>(timeLeft % 60);

                        char buffer[128];
                        snprintf(buffer, sizeof(buffer), "%s | %02dd:%02dh:%02dm:%02ds", fc.c_str(), d, h, m, s);

                        displayList.push_back(buffer);
                        friendCodeList.push_back(fc);
                    }

                    std::vector<const char*> displayCStrs;
                    for (auto& s : displayList) displayCStrs.push_back(s.c_str());

                    selectedTempBanIndex = std::clamp(selectedTempBanIndex, 0, (int)displayCStrs.size() - 1);
                    CustomListBoxInt("Select TempBan", &selectedTempBanIndex, displayCStrs);

                    if (ImGui::Button("Unban")) {
                        if (selectedTempBanIndex >= 0 && selectedTempBanIndex < (int)friendCodeList.size()) {
                            std::string targetFC = friendCodeList[selectedTempBanIndex];
                            State.TempBannedFCs.erase(targetFC);
                            State.Save();
                        }
                    }
                }
                ImGui::Dummy(ImVec2(10, 10) * State.dpiScale);
                ImGui::TextColored(ImVec4(1.0f, 0.0f, 0.0f, 1.0f), "Note: Temporary Ban Features\nWorks for Host Only!");
                ImGui::EndGroup();
            }*/
        }

        if (openOptions) {
            if ((IsInGame() || IsInLobby()) && GameOptions().HasOptions()) {
                GameOptions options;
                /*std::string hostText = std::format("Host: {}", RemoveHtmlTags(GetHostUsername()));
                ImGui::Text(const_cast<char*>(hostText.c_str()));*/

                if (options.GetGameMode() == GameModes__Enum::Normal)
                {
                    auto allPlayers = GetAllPlayerControl();
                    RoleRates roleRates = RoleRates(options, (int)allPlayers.size());
                    // this should be all the major ones. if people want more they're simple enough to add.
                    ImGui::Text("Визуальные Задания: %s", (options.GetBool(app::BoolOptionNames__Enum::VisualTasks) ? "Включены" : "Выключены"));
                    switch (options.GetInt(app::Int32OptionNames__Enum::TaskBarMode)) {
                    case 0:
                        ImGui::Text("Обновление Шкалы: Постоянно");
                        break;
                    case 1:
                        ImGui::Text("Обновление Шкалы: На голосовании");
                        break;
                    case 2:
                        ImGui::Text("Обновление Шкалы: Никогда");
                        break;
                    default:
                        ImGui::Text("Обновление Шкалы: Другое");
                        break;
                    }
                    ImGui::Text("Показ Ролей: %s", (options.GetBool(app::BoolOptionNames__Enum::ConfirmImpostor) ? "Включено" : "Выключено"));
                    switch (options.GetInt(app::Int32OptionNames__Enum::KillDistance)) {
                    case 0:
                        ImGui::Text("Дистанция Убийства: Короткая");
                        break;
                    case 1:
                        ImGui::Text("Дистанция Убийства: Средняя");
                        break;
                    case 2:
                        ImGui::Text("Дистанция Убийства: Большая");
                        break;
                    default:
                        ImGui::Text("Дистанция Убийства: Другая");
                        break;
                    }

                    ImGui::Dummy(ImVec2(3, 3) * State.dpiScale);
                    ImGui::Separator();
                    ImGui::Dummy(ImVec2(3, 3) * State.dpiScale);

                    ImGui::Text("Макс. Инжинеров: %d", roleRates.GetRoleCount(app::RoleTypes__Enum::Engineer));
                    ImGui::Text("Шанс Инжинера: %d%", options.GetRoleOptions().GetChancePerGame(RoleTypes__Enum::Engineer));
                    ImGui::Text("КД Люка: %.2f s", options.GetFloat(app::FloatOptionNames__Enum::EngineerCooldown, 1.0F));
                    ImGui::Text("Продолжительность Нахождения в Люке: %.2f s", options.GetFloat(app::FloatOptionNames__Enum::EngineerInVentMaxTime, 1.0F));

                    ImGui::Dummy(ImVec2(3, 3) * State.dpiScale);
                    ImGui::Separator();
                    ImGui::Dummy(ImVec2(3, 3) * State.dpiScale);

                    ImGui::Text("Макс. Ученых: %d", roleRates.GetRoleCount(app::RoleTypes__Enum::Scientist));
                    ImGui::Text("Шанс Ученого: %d%", options.GetRoleOptions().GetChancePerGame(RoleTypes__Enum::Scientist));
                    ImGui::Text("КД Планшета: %.2f s", options.GetFloat(app::FloatOptionNames__Enum::ScientistCooldown, 1.0F));
                    ImGui::Text("Длительность Работы Планшета: %.2f s", options.GetFloat(app::FloatOptionNames__Enum::ScientistBatteryCharge, 1.0F));

                    ImGui::Dummy(ImVec2(3, 3) * State.dpiScale);
                    ImGui::Separator();
                    ImGui::Dummy(ImVec2(3, 3) * State.dpiScale);

                    ImGui::Text("Макс. Ангелов Хранителей: %d", roleRates.GetRoleCount(app::RoleTypes__Enum::GuardianAngel));
                    ImGui::Text("Шанс Ангелав Хранителей: %d%", options.GetRoleOptions().GetChancePerGame(RoleTypes__Enum::GuardianAngel));
                    ImGui::Text("КД Защиты: %.2f s", options.GetFloat(app::FloatOptionNames__Enum::GuardianAngelCooldown, 1.0F));
                    ImGui::Text("Длительность Защиты: %.2f s", options.GetFloat(app::FloatOptionNames__Enum::ProtectionDurationSeconds, 1.0F));

                    ImGui::Dummy(ImVec2(3, 3) * State.dpiScale);
                    ImGui::Separator();
                    ImGui::Dummy(ImVec2(3, 3) * State.dpiScale);

                    ImGui::Text("Макс. Оборотней: %d", roleRates.GetRoleCount(app::RoleTypes__Enum::Shapeshifter));
                    ImGui::Text("Шанс Оборотня: %d%", options.GetRoleOptions().GetChancePerGame(RoleTypes__Enum::Shapeshifter));
                    ImGui::Text("КД Превращения: %.2f s", options.GetFloat(app::FloatOptionNames__Enum::ShapeshifterCooldown, 1.0F));
                    ImGui::Text("Длительность Превращения: %.2f s", options.GetFloat(app::FloatOptionNames__Enum::ShapeshifterDuration, 1.0F));

                    ImGui::Dummy(ImVec2(3, 3) * State.dpiScale);
                    ImGui::Separator();
                    ImGui::Dummy(ImVec2(3, 3) * State.dpiScale);

                    ImGui::Text("Макс. Паникеров: %d", roleRates.GetRoleCount(app::RoleTypes__Enum::Noisemaker));
                    ImGui::Text("Шанс Паникера: %d%", options.GetRoleOptions().GetChancePerGame(RoleTypes__Enum::Noisemaker));
                    ImGui::Text("Длительность Паникера: %.2f s", options.GetFloat(app::FloatOptionNames__Enum::NoisemakerAlertDuration, 1.0F));

                    ImGui::Dummy(ImVec2(3, 3) * State.dpiScale);
                    ImGui::Separator();
                    ImGui::Dummy(ImVec2(3, 3) * State.dpiScale);

                    ImGui::Text("Макс. Следопытов: %d", roleRates.GetRoleCount(app::RoleTypes__Enum::Tracker));
                    ImGui::Text("Шанс Следопыта: %d%", options.GetRoleOptions().GetChancePerGame(RoleTypes__Enum::Tracker));
                    ImGui::Text("КД Слежки: %.2f s", options.GetFloat(app::FloatOptionNames__Enum::TrackerDuration, 1.0F));
                    ImGui::Text("Задержка Слежки: %.2f s", options.GetFloat(app::FloatOptionNames__Enum::TrackerCooldown, 1.0F));
                    ImGui::Text("Длительность Слежки: %.2f s", options.GetFloat(app::FloatOptionNames__Enum::TrackerDelay, 1.0F));

                    ImGui::Dummy(ImVec2(3, 3) * State.dpiScale);
                    ImGui::Separator();
                    ImGui::Dummy(ImVec2(3, 3) * State.dpiScale);

                    ImGui::Text("Макс. Фантомов: %d", roleRates.GetRoleCount(app::RoleTypes__Enum::Phantom));
                    ImGui::Text("Шанс Фантома: %d%", options.GetRoleOptions().GetChancePerGame(RoleTypes__Enum::Phantom));
                    ImGui::Text("КД Невидимости: %.2f s", options.GetFloat(app::FloatOptionNames__Enum::PhantomCooldown, 1.0F));
                    ImGui::Text("Длительность Невидимости: %.2f s", options.GetFloat(app::FloatOptionNames__Enum::PhantomDuration, 1.0F));
                }
                else if (options.GetGameMode() == GameModes__Enum::HideNSeek) {

                    int ImpostorId = options.GetInt(app::Int32OptionNames__Enum::ImpostorPlayerID);
                    if (ImpostorId < 0) {
                        ImGui::Text("Предатель: Круговой");
                    }
                    else {
                        std::string ImpostorName = std::format("Выбрано Предателей: {}", convert_from_string(NetworkedPlayerInfo_get_PlayerName(GetPlayerDataById(ImpostorId), nullptr)));
                        ImGui::Text(const_cast<char*>(ImpostorName.c_str()));
                    }
                    ImGui::Text("Режим Фонарика: %s", (options.GetBool(app::BoolOptionNames__Enum::UseFlashlight) ? "On" : "Off"));

                    ImGui::Dummy(ImVec2(3, 3) * State.dpiScale);
                    ImGui::Separator();
                    ImGui::Dummy(ImVec2(3, 3) * State.dpiScale);

                    ImGui::Text("Использований Люка: %d", options.GetInt(app::Int32OptionNames__Enum::CrewmateVentUses));
                    ImGui::Text("Длительность Находления в Люке: %.2f s", options.GetFloat(app::FloatOptionNames__Enum::CrewmateTimeInVent, 1.0F));

                    ImGui::Dummy(ImVec2(3, 3) * State.dpiScale);
                    ImGui::Separator();
                    ImGui::Dummy(ImVec2(3, 3) * State.dpiScale);

                    ImGui::Text("Время Чтобы Спрятаться: %.2f s", options.GetFloat(app::FloatOptionNames__Enum::EscapeTime, 1.0F));
                    ImGui::Text("Время Финальных Пряток: %.2f s", options.GetFloat(app::FloatOptionNames__Enum::FinalEscapeTime, 1.0F));
                    ImGui::Text("Финальная Скорость Предателя: %.2f s", options.GetFloat(app::FloatOptionNames__Enum::SeekerFinalSpeed, 1.0F));
                }
            }
            else CloseOtherGroups(Groups::General);
        }
        ImGui::EndChild();
    }
}

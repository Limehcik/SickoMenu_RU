#include "pch-il2cpp.h"
#include "self_tab.h"
#include "game.h"
#include "gui-helpers.hpp"
#include "utility.h"
#include "state.hpp"
#include "logger.h"

extern void RevealAnonymousVotes(); // in MeetingHud.cpp

namespace SelfTab {
    enum Groups {
        Visuals,
        Utils,
        Randomizers,
        TextEditor
    };

    static bool openVisuals = true; //default to visual tab group
    static bool openUtils = false;
    static bool openRandomizers = false;
    static bool openTextEditor = false;

    static std::string originalText = "";
    static std::string editedText = "";

    static bool italicName = false;
    static bool underlineName = false;
    static bool strikethroughName = false;
    static bool boldName = false;
    static bool nobrName = false;
    static ImVec4 nameColor1 = ImVec4(1.f, 1.f, 1.f, 1.f);
    static ImVec4 nameColor2 = ImVec4(1.f, 1.f, 1.f, 1.f);
    static bool coloredName = false;
    static bool font = false;
    static int fontType = 0;
    static bool resizeName = false;
    static float nameSize = 0.f;
    static bool indentName = false;
    static float indentLevel = 0.f;
    static bool cspaceName = false;
    static float cspaceLevel = 0.f;
    static bool mspaceName = false;
    static float mspaceLevel = 0.f;
    static bool voffsetName = false;
    static float voffsetLevel = 0.f;
    static bool rotateName = false;
    static float rotateAngle = 0.f;

    void CloseOtherGroups(Groups group) {
        openVisuals = group == Groups::Visuals;
        openUtils = group == Groups::Utils;
        openRandomizers = group == Groups::Randomizers;
        openTextEditor = group == Groups::TextEditor;
    }

    std::string GetTextEditorName(std::string str) {
        str = RemoveHtmlTags(str);

        std::string opener = "", closer = "";

        if (coloredName) {
            str = GetGradientUsername(str, nameColor1, nameColor2);
        }

        if (italicName) {
            opener += "<i>";
            closer += "</i>";
        }

        if (underlineName && !coloredName) {
            opener += "<u>";
            closer += "</u>";
        }

        if (strikethroughName && !coloredName) {
            opener += "<s>";
            closer += "</s>";
        }

        if (boldName) {
            opener += "<b>";
            closer += "</b>";
        }

        if (nobrName) {
            opener += "<nobr>";
            closer += "</nobr>";
        }

        if (font) {
            switch (fontType) {
            case 0: {
                opener += "<font=\"Barlow-Italic SDF\">";
                break;
            }
            case 1: {
                opener += "<font=\"Barlow-Medium SDF\">";
                break;
            }
            case 2: {
                opener += "<font=\"Barlow-Bold SDF\">";
                break;
            }
            case 3: {
                opener += "<font=\"Barlow-SemiBold SDF\">";
                break;
            }
            case 4: {
                opener += "<font=\"Barlow-SemiBold Masked\">";
                break;
            }
            case 5: {
                opener += "<font=\"Barlow-ExtraBold SDF\">";
                break;
            }
            case 6: {
                opener += "<font=\"Barlow-BoldItalic SDF\">";
                break;
            }
            case 7: {
                opener += "<font=\"Barlow-BoldItalic Masked\">";
                break;
            }
            case 8: {
                opener += "<font=\"Barlow-Black SDF\">";
                break;
            }
            case 9: {
                opener += "<font=\"Barlow-Light SDF\">";
                break;
            }
            case 10: {
                opener += "<font=\"Barlow-Regular SDF\">";
                break;
            }
            case 11: {
                opener += "<font=\"Barlow-Regular Masked\">";
                break;
            }
            case 12: {
                opener += "<font=\"Barlow-Regular Outline\">";
                break;
            }
            case 13: {
                opener += "<font=\"Brook SDF\">";
                break;
            }
            case 14: {
                opener += "<font=\"LiberationSans SDF\">";
                break;
            }
            case 15: {
                opener += "<font=\"NotoSansJP-Regular SDF\">";
                break;
            }
            case 16: {
                opener += "<font=\"VCR SDF\">";
                break;
            }
            case 17: {
                opener += "<font=\"CONSOLA SDF\">";
                break;
            }
            case 18: {
                opener += "<font=\"digital-7 SDF\">";
                break;
            }
            case 19: {
                opener += "<font=\"OCRAEXT SDF\">";
                break;
            }
            case 20: {
                opener += "<font=\"DIN_Pro_Bold_700 SDF\">";
                break;
            }
            }
            closer += "</font>";
        }

        /*if (State.Material) {
            switch (State.MaterialType) {
            case 0: {
                opener += "<material=\"Barlow-Italic SDF Outline\">";
                break;
            }
            case 1: {
                opener += "<material=\"Barlow-BoldItalic SDF Outline\">";
                break;
            }
            case 2: {
                opener += "<material=\"Barlow-SemiBold SDF Outline\">";
                break;
            }
                    closer += "</material>";
            }
        }*/

        if (resizeName) {
            opener += std::format("<size={}%>", nameSize * 100);
            closer += "</size>";
        }

        if (indentName) {
            opener += std::format("<line-indent={}>", indentLevel);
            closer += "</line-indent>";
        }

        if (cspaceName) {
            opener += std::format("<cspace={}>", cspaceLevel);
            closer += "</cspace>";
        }

        if (mspaceName) {
            opener += std::format("<mspace={}>", mspaceLevel);
            closer += "</mspace>";
        }

        if (voffsetName) {
            opener += std::format("<voffset={}>", voffsetLevel);
            closer += "</voffset>";
        }

        if (rotateName) {
            opener += std::format("<rotate={}>", rotateAngle);
            closer += "<rotate=0>";
        }

        return opener + str + closer;
    }

    void Render() {
        ImGui::SameLine(100 * State.dpiScale);
        ImGui::BeginChild("###Self", ImVec2(500 * State.dpiScale, 0), true, ImGuiWindowFlags_NoBackground);
        if (TabGroup("Визуалы", openVisuals)) {
            CloseOtherGroups(Groups::Visuals);
        }
        ImGui::SameLine();
        if (TabGroup("Утилиты", openUtils)) {
            CloseOtherGroups(Groups::Utils);
        }
        ImGui::SameLine();
        if (TabGroup("Рандомайзер", openRandomizers)) {
            CloseOtherGroups(Groups::Randomizers);
        }
        ImGui::SameLine();
        if (TabGroup("Редактор Текста", openTextEditor)) {
            CloseOtherGroups(Groups::TextEditor);
        }

        if (openVisuals) {
            ImGui::Dummy(ImVec2(4, 4) * State.dpiScale);
            if (ToggleButton("Максимальный Обзор", &State.MaxVision)) {
                State.Save();
            }
            ImGui::SameLine();
            if (ToggleButton("Валлхак", &State.Wallhack)) {
                State.Save();
            }
            ImGui::SameLine();
            ToggleButton("Отключить HUD", &State.DisableHud);

            if (ToggleButton("Свобод. Камера", &State.FreeCam)) {
                State.playerToFollow = {};
                State.Save();
            }

            ImGui::SameLine(130.f * State.dpiScale);
            SteppedSliderFloat("Скорость", &State.FreeCamSpeed, 0.f, 10.f, 0.05f, "%.2fx", ImGuiSliderFlags_Logarithmic | ImGuiSliderFlags_NoInput);

            if (ToggleButton("Отдалить", &State.EnableZoom)) {
                State.Save();
                if (!State.EnableZoom) RefreshChat();
            }

            ImGui::SameLine(130.f * State.dpiScale);
            SteppedSliderFloat("Размер", &State.CameraHeight, 0.5f, 10.0f, 0.1f, "%.2fx", ImGuiSliderFlags_Logarithmic | ImGuiSliderFlags_NoInput);

            ImGui::Dummy(ImVec2(7, 7) * State.dpiScale);
            ImGui::Separator();
            ImGui::Dummy(ImVec2(7, 7) * State.dpiScale);

            if (ToggleButton("Показывать Кнопку Чата", &State.ChatAlwaysActive)) {
                State.Save();
            }
            ImGui::SameLine();
            if (ToggleButton("Разрешить Ctrl+(C/V)", &State.ChatPaste)) { //add copying later
                State.Save();
            }

            if (ToggleButton("Видеть Сообщения Призраков", &State.ReadGhostMessages)) {
                State.Save();
            }
            ImGui::SameLine();
            if (ToggleButton("Читать и Писать в SickoЧат", &State.ReadAndSendSickoChat)) {
                State.Save();
            }
            if (State.ReadAndSendSickoChat) ImGui::Text("Чтобы писать в SickoЧат Пишите \"/sc [сообщение]\"!");
            /*static int framesPassed = 0;
            if (AnimatedButton("Refresh Chat Button")) {
                State.RefreshChatButton = true;
                framesPassed = 100;
            }

            if (framesPassed == 0) State.RefreshChatButton = false;
            else framesPassed--;*/

            if (!IsHost() && State.SafeMode) {
                ImGui::Text("Кастомные Ники Видны Только Вам, Если Вы не Хост");
            }

            if (!IsHost() && State.SafeMode) {
                ImGui::Text("Или у Вас не Выключен Безопасный Режим!");
            }

            if (ToggleButton("Кастомный Ник", &State.CustomName)) {
                State.Save();
            }
            ImGui::SameLine();
            if (ToggleButton("Кастомный Ник Всем", &State.CustomNameForEveryone)) {
                State.Save();
            }

            if ((IsHost() || !State.SafeMode)) {
                if (ToggleButton("Кастомный Ник Виден Всем", &State.ServerSideCustomName)) {
                    State.Save();
                }
            }

            if (State.CustomName && ImGui::CollapsingHeader("Настройки Кастомного Ника"))
            {
                if (ToggleButton("Курсив", &State.ItalicName)) {
                    State.Save();
                }
                ImGui::SameLine();
                if (ToggleButton("Подчеркнутый", &State.UnderlineName)) {
                    State.Save();
                }
                ImGui::SameLine();
                if (ToggleButton("Зачеркнутый", &State.StrikethroughName)) {
                    State.Save();
                }
                ImGui::SameLine();
                if (ToggleButton("Толстый", &State.BoldName)) {
                    State.Save();
                }

                if (ToggleButton("Без переноса", &State.NobrName)) {
                    State.Save();
                }

                if (ImGui::ColorEdit4("Начало Градиента", (float*)&State.NameColor1, ImGuiColorEditFlags__OptionsDefault | ImGuiColorEditFlags_NoInputs | ImGuiColorEditFlags_AlphaBar | ImGuiColorEditFlags_AlphaPreview)) {
                    State.Save();
                }
                ImGui::SameLine();
                if (ImGui::ColorEdit4("Конец Градиента", (float*)&State.NameColor2, ImGuiColorEditFlags__OptionsDefault | ImGuiColorEditFlags_NoInputs | ImGuiColorEditFlags_AlphaBar | ImGuiColorEditFlags_AlphaPreview)) {
                    State.Save();
                }
                ImGui::SameLine();
                if (ToggleButton("Цветной", &State.ColoredName)) {
                    State.Save();
                }

                if (ToggleButton("RGB", &State.RgbName)) {
                    State.Save();
                }

                if (CustomListBoxInt("Режим Градиента", &State.ColorMethod, { "Статичный", "Слева-на-Право" }, 80.f * State.dpiScale))
                    State.Save();
                ImGui::SameLine();
                if (CustomListBoxInt("RGB Режим", &State.RgbMethod, { "Все-сразу", "Слева-на-Право" }, 80.f * State.dpiScale))
                    State.Save();

                if (ToggleButton("Вкл Префикс и Суффикс", &State.UsePrefixAndSuffix)) State.Save();
                if (ToggleButton("Новые Строки для Префикса и Суффикса", &State.PrefixAndSuffixNewLines)) State.Save();

                if (InputString("Префикс", &State.NamePrefix)) State.Save();
                if (InputString("Суффикс", &State.NameSuffix)) State.Save();
                if (State.UsePrefixAndSuffix) ImGui::TextColored(ImVec4(1.0f, 1.0f, 1.0f, 1.0f), ("Примечание: Префикс и/или Суффикс Будут Удалены из Концов Имени, если Они в нем Содержатся."));
                if (State.UsePrefixAndSuffix) ImGui::TextColored(ImVec4(1.0f, 1.0f, 1.0f, 1.0f), ("Это Делается для Предотвращения Переполнения Имен."));

                if (ToggleButton("Шрифт", &State.Font)) {
                    State.Save();
                }
                if (State.Font) {
                    ImGui::SameLine();
                    if (CustomListBoxInt(" ", &State.FontType, FONTS, 160.f * State.dpiScale)) {
                        State.Save();
                    }
                }
                if (State.Font) ImGui::TextColored(ImVec4(1.0f, 1.0f, 1.0f, 1.0f), ("Примечание: Белый Ник не Будет Виден в Чате."));

                /*if (ToggleButton("Material", &State.Material)) {
                    State.Save();
                }
                ImGui::SameLine();
                if (CustomListBoxInt(" Some materials are not supported", &State.MaterialType, MATERIALS, 160.f * State.dpiScale)) {
                    State.Save();
                }*/

                if (ToggleButton("Размер", &State.ResizeName)) {
                    State.Save();
                }

                ImGui::SameLine();
                if (ImGui::InputFloat("Размер Ника", &State.NameSize)) {
                    State.Save();
                }

                if (ToggleButton("Отступ", &State.IndentName)) {
                    State.Save();
                }

                ImGui::SameLine();
                if (ImGui::InputFloat("Отступ Ника", &State.NameIndent)) {
                    State.Save();
                }

                if (ToggleButton("Интервал", &State.CspaceName)) {
                    State.Save();
                }

                ImGui::SameLine();
                if (ImGui::InputFloat("Интервал Символов", &State.NameCspace)) {
                    State.Save();
                }

                if (ToggleButton("Межстроч.", &State.MspaceName)) {
                    State.Save();
                }

                ImGui::SameLine();
                if (ImGui::InputFloat("Межстроч. Растоян.", &State.NameMspace)) {
                    State.Save();
                }

                if (ToggleButton("Верт. Сдвиг", &State.VoffsetName)) {
                    State.Save();
                }

                ImGui::SameLine();
                if (ImGui::InputFloat("Верт. Сдвиг Ника", &State.NameVoffset)) {
                    State.Save();
                }
                if (ToggleButton("Поворот", &State.RotateName)) {
                    State.Save();
                }

                ImGui::SameLine();
                if (ImGui::InputFloat("Угол Поворота", &State.NameRotate)) {
                    State.Save();
                }
                ImGui::Dummy(ImVec2(5, 5) * State.dpiScale);
            }

            if (ToggleButton("Показ. роли", &State.RevealRoles)) {
                State.Save();
            }
            ImGui::SameLine(120.0f * State.dpiScale);
            if (ToggleButton("Абрев. Ролей", &State.AbbreviatedRoleNames))
            {
                State.Save();
            }
            ImGui::SameLine(240.0f * State.dpiScale);
            if (ToggleButton("Цветные Точки Игроков у Имен", &State.PlayerColoredDots))
            {
                State.Save();
            }

            if (ToggleButton("Показ Инфо Игроков в Лобби", &State.ShowPlayerInfo))
            {
                State.Save();
            }
            ImGui::SameLine();
            if (ToggleButton("Показ Инфо Лобби", &State.ShowLobbyInfo))
            {
                State.Save();
            }

            if (ToggleButton("Видеть Голоса", &State.RevealVotes)) {
                State.Save();
            }
            ImGui::SameLine();
            if (ToggleButton("Видеть Тайные Голоса", &State.RevealAnonymousVotes)) {
                State.Save();
                RevealAnonymousVotes();
            }

            if (ToggleButton("Видеть Призраков", &State.ShowGhosts)) {
                State.Save();
            }
            ImGui::SameLine();
            if (ToggleButton("Видеть Фантомов", &State.ShowPhantoms)) {
                State.Save();
            }
            ImGui::SameLine();
            if (ToggleButton("Видеть Игроков в Люках", &State.ShowPlayersInVents)) {
                State.Save();
            }

            if (ToggleButton("Видеть Щиты", &State.ShowProtections))
            {
                State.Save();
            }
            ImGui::SameLine();
            if (ToggleButton("Видеть КД Убийства", &State.ShowKillCD)) {
                State.Save();
            }

            if (ToggleButton("Откл Аним. Убийства", &State.DisableKillAnimation)) {
                State.Save();
            }
            ImGui::SameLine();
            if (ToggleButton("Откл Музыку в Лобби", &State.DisableLobbyMusic)) {
                State.Save();
            }
            ImGui::SameLine();
            if (ToggleButton("Старый Текст Пинга", &State.OldStylePingText)) State.Save();

            if (ToggleButton("Показ Хоста", &State.ShowHost)) {
                State.Save();
            }
            ImGui::SameLine();
            if (ToggleButton("Показ Результаты Голос.", &State.ShowVoteKicks)) {
                State.Save();
            }
            ImGui::SameLine();
            if (ToggleButton("Показ FPS", &State.ShowFps)) {
                State.Save();
            }

            if (ToggleButton("Показ КД Чата", &State.ShowChatTimer)) {
                State.Save();
            }
            ImGui::SameLine();
            if (ToggleButton("Увеличить Лимит Символов в Чате", &State.ExtendChatLimit)) {
                State.Save();
            }

            if (ToggleButton("Увеличить Историю Чата", &State.ExtendChatHistory)) {
                State.Save();
            }

            /*if (ToggleButton("Change Body Type", &State.ChangeBodyType)) {
                State.Save();
            }
            if (State.ChangeBodyType) {
                ImGui::SameLine();
                if (CustomListBoxInt("Type", &State.BodyType, BODYTYPES, 75.f * State.dpiScale))
                    State.Save();
            }*/

            if (State.InMeeting && AnimatedButton("Выйти из Голосования"))
            {
                if (IsHost()) State.rpcQueue.push(new RpcEndMeeting());
                else State.rpcQueue.push(new EndMeeting());
                State.InMeeting = false;
            }
        }

        if (openUtils) {
            if (ToggleButton("Открыть Люки", &State.UnlockVents)) {
                State.Save();
            }
            ImGui::SameLine();
            if (ToggleButton("Двигаться в Люке и при Превращении", &State.MoveInVentAndShapeshift)) {
                if (*Game::pLocalPlayer == NULL) State.Save();
                else if (!State.MoveInVentAndShapeshift && (State.InMeeting || (*Game::pLocalPlayer)->fields.inVent)) {
                    (*Game::pLocalPlayer)->fields.moveable = false;
                    State.Save();
                }
            }

            if (ToggleButton("Всегда Двигаться", &State.AlwaysMove)) {
                State.Save();
            }

            if (ToggleButton("Нет Анимации Превращения", &State.AnimationlessShapeshift)) {
                State.Save();
            }
            ImGui::SameLine();
            if (ToggleButton("Копир. Код Лобби при Отключ.", &State.AutoCopyLobbyCode)) {
                State.Save();
            }

            if (ToggleButton("Ноуклип", &State.NoClip)) {
                State.Save();
            }
            ImGui::SameLine();
            if (ToggleButton("Нет Анимации Искателя", &State.NoSeekerAnim)) State.Save();

            if (ToggleButton("Убивать Предателей", &State.KillImpostors)) {
                State.Save();
            }
            ImGui::SameLine();
            if (ToggleButton("Беск. Дистанция Убийств", &State.InfiniteKillRange)) {
                State.Save();
            }

            if (ToggleButton("Лучше Уведомления Чата", &State.BetterChatNotifications)) {
                State.Save();
            }
            ImGui::SameLine();
            if (ToggleButton("Лучше Ввод Кода Лобби", &State.BetterLobbyCodeInput)) {
                State.Save();
            }

            if (ToggleButton("Лучше Звук Сообщения", &State.BetterMessageSounds)) {
                State.Save();
            }

            /*if (ToggleButton("Bypass Guardian Angel Protections", &State.BypassAngelProt)) {
                State.Save();
            }*/
            ImGui::SameLine();
            if (ToggleButton("Авто Убийство", &State.AutoKill)) {
                State.Save();
            }

            if (ToggleButton("Задания за Предателя", &State.DoTasksAsImpostor)) {
                State.Save();
            }
            ImGui::SameLine();
            if (ToggleButton("Репортить при Убийстве", &State.ReportOnMurder)) {
                State.Save();
            }
            if (State.ReportOnMurder) {
                ImGui::SameLine();
                if (ToggleButton("Без Саморепорта", &State.PreventSelfReport)) {
                    State.Save();
                }
            }
            /*ImGui::SameLine();
            if (ToggleButton("Always Use Kill Exploit", &State.AlwaysUseKillExploit)) {
                State.Save();
            }*/

            if (ToggleButton("Визуально Ожить", &State.FakeAlive)) {
                State.Save();
            }
            ImGui::SameLine();
            if (((IsHost() && IsInGame()) || !State.SafeMode) && ToggleButton(IsHost() ? "Режим Бога" : "Визуал Щит", &State.GodMode))
                State.Save();

            if (ToggleButton("(Shift/Ctrl + ПКМ) Для ТП", &State.ShiftRightClickTP)) {
                State.Save();
            }
            if (!State.SafeMode) ImGui::SameLine();
            if (!State.SafeMode && ToggleButton("Зажмите ALT для ТП Всех", &State.TeleportEveryone)) {
                State.Save();
            }
            if (ToggleButton((State.SafeMode ? "Повернуть всех (Только для Вас)" : "Повернуть всех"), &State.RotateEveryone)) {
                State.Save();
            }
            if (!State.SafeMode && State.RotateEveryone && ToggleButton("Поворот Виден Всем", &State.RotateServerSide)) {
                State.Save();
            }
            if (ImGui::InputFloat("Радус Поворота", &State.RotateRadius, 0.0f, 0.0f, "%.2f m")) {
                State.Save();
            }

            if (ImGui::InputFloat("X Координата", &State.xCoordinate, 0.0f, 0.0f, "%.4f X")) {
                State.Save();
            }

            if (ImGui::InputFloat("Y Координата", &State.yCoordinate, 0.0f, 0.0f, "%.4f Y")) {
                State.Save();
            }

            if (ToggleButton("Относительный ТП", &State.RelativeTeleport)) {
                State.Save();
            }
            if (IsInGame() || IsInLobby())
                ImGui::SameLine();
            if ((IsInGame() || IsInLobby()) && AnimatedButton("Получить Позицию"))
            {
                Vector2 position = GetTrueAdjustedPosition(*Game::pLocalPlayer);
                State.xCoordinate = position.x;
                State.yCoordinate = position.y;
            }
            if (IsInGame() || IsInLobby())
                ImGui::SameLine();

            if ((IsInGame() || IsInLobby()) && AnimatedButton("ТП к"))
            {
                Vector2 position = GetTrueAdjustedPosition(*Game::pLocalPlayer);
                Vector2 target = { (State.RelativeTeleport ? position.x : 0.f) + State.xCoordinate, (State.RelativeTeleport ? position.y : 0.f) + State.yCoordinate };
                if (IsInGame()) {
                    State.rpcQueue.push(new RpcSnapTo(target));
                }
                else if (IsInLobby()) {
                    State.lobbyRpcQueue.push(new RpcSnapTo(target));
                }
            }
            if (!State.SafeMode && (IsInGame() || IsInLobby())) {
                ImGui::SameLine();
                if (AnimatedButton("ТП Всех к"))
                {
                    Vector2 position = GetTrueAdjustedPosition(*Game::pLocalPlayer);
                    Vector2 target = { (State.RelativeTeleport ? position.x : 0.f) + State.xCoordinate, (State.RelativeTeleport ? position.y : 0.f) + State.yCoordinate };
                    std::queue<RPCInterface*>* queue = nullptr;
                    if (IsInGame())
                        queue = &State.rpcQueue;
                    else if (IsInLobby())
                        queue = &State.lobbyRpcQueue;
                    for (auto player : GetAllPlayerControl()) {
                        queue->push(new RpcForceSnapTo(player, target));
                    }
                }
            }

            if (CustomListBoxInt("Выбрать Роль", &State.FakeRole, FAKEROLES, 100.0f * State.dpiScale))
                State.Save();
            ImGui::SameLine();
            if ((IsHost() || !State.SafeMode) && (IsInGame() || IsInLobby()) && AnimatedButton("Установить")) {
                State.FakeRole = std::clamp(State.FakeRole, 0, 10);
                if (IsInGame())
                    State.rpcQueue.push(new RpcSetRole(*Game::pLocalPlayer, RoleTypes__Enum(State.FakeRole)));
                else if (IsInLobby())
                    State.lobbyRpcQueue.push(new RpcSetRole(*Game::pLocalPlayer, RoleTypes__Enum(State.FakeRole)));
            }
            if (IsHost() || !State.SafeMode) ImGui::SameLine();
            if ((IsHost() || !State.SafeMode) && (IsInGame() || IsInLobby()) && AnimatedButton("Установить Всем")) {
                State.FakeRole = std::clamp(State.FakeRole, 0, 10);
                if (IsInGame()) {
                    for (auto player : GetAllPlayerControl())
                        State.rpcQueue.push(new RpcSetRole(player, RoleTypes__Enum(State.FakeRole)));
                }
                else if (IsInLobby()) {
                    for (auto player : GetAllPlayerControl())
                        State.lobbyRpcQueue.push(new RpcSetRole(player, RoleTypes__Enum(State.FakeRole)));
                }
            }
            bool roleAllowed = false;
            switch (State.FakeRole) {
            case (int)RoleTypes__Enum::Crewmate:
            case (int)RoleTypes__Enum::Engineer:
            case (int)RoleTypes__Enum::Scientist:
            case (int)RoleTypes__Enum::Noisemaker:
            case (int)RoleTypes__Enum::Tracker:
            case (int)RoleTypes__Enum::CrewmateGhost:
            case (int)RoleTypes__Enum::ImpostorGhost:
            case (int)RoleTypes__Enum::GuardianAngel:
                roleAllowed = true;
                break;
            case (int)RoleTypes__Enum::Impostor:
                if ((!IsHost() && State.SafeMode) || State.RealRole != RoleTypes__Enum::Impostor || State.RealRole != RoleTypes__Enum::Shapeshifter || State.RealRole != RoleTypes__Enum::Phantom) {
                    roleAllowed = false;
                    break;
                }
                roleAllowed = true;
                break;
            case (int)RoleTypes__Enum::Shapeshifter:
                if (State.RealRole != RoleTypes__Enum::Shapeshifter) {
                    roleAllowed = false;
                    break;
                }
                roleAllowed = true;
                break;
            case (int)RoleTypes__Enum::Phantom:
                if (State.RealRole != RoleTypes__Enum::Phantom) {
                    roleAllowed = false;
                    break;
                }
                roleAllowed = true;
                break;
            default:
                roleAllowed = false;
                break;
            }
            if ((IsInGame() || IsInLobby()) && (roleAllowed || (IsHost() || !State.SafeMode)) && AnimatedButton("Установить Фейк")) {
                if (IsInGame())
                    State.rpcQueue.push(new SetRole(RoleTypes__Enum(State.FakeRole)));
                else if (IsInLobby())
                    State.lobbyRpcQueue.push(new SetRole(RoleTypes__Enum(State.FakeRole)));
            }
            ImGui::SameLine();
            if (ToggleButton("Авто Ставить Фейк Роль", &State.AutoFakeRole)) {
                State.Save();
            }
            if (IsInLobby() || IsInGame()) {
                ImGui::SameLine();
                std::string roleText = FAKEROLES[int(State.RealRole)];
                ImGui::Text(("Реал. Роль: " + roleText).c_str());
            }

            if (!State.SafeMode) {
                if (ToggleButton("Разбл. кнопку убийства", &State.UnlockKillButton)) {
                    State.Save();
                }
                ImGui::SameLine();
                if (ToggleButton("Убивать в невидимости", &State.KillInVanish)) {
                    State.Save();
                }
                /*if (ToggleButton("Bypass Guardian Angel Protections", &State.BypassAngelProt)) {
                    State.Save();
                }*/
            }
        }

        if (openRandomizers) {
            if (ToggleButton("Циклер", &State.Cycler)) {
                State.Save();
            }
            ImGui::SameLine();
            if (ToggleButton("Цикл на голосовании", &State.CycleInMeeting)) {
                State.Save();
            }
            ImGui::SameLine();
            if (ToggleButton("Цикл Между Игроками", &State.CycleBetweenPlayers)) {
                State.Save();
            }

            if (SteppedSliderFloat("Интервал", &State.CycleTimer, 0.2f, 1.f, 0.02f, "%.2fs", ImGuiSliderFlags_Logarithmic | ImGuiSliderFlags_NoInput)) {
                State.PrevCycleTimer = State.CycleTimer;
                State.CycleDuration = State.CycleTimer * 50;
            }

            ImGui::Dummy(ImVec2(4, 4) * State.dpiScale);
            if (ImGui::CollapsingHeader("Настройки Циклера")) {
                if (ToggleButton("Цикл Ника", &State.CycleName)) {
                    State.Save();
                }


                ImGui::SameLine(120.0f * State.dpiScale);
                if (ToggleButton("Цикл Цвета", &State.RandomColor)) {
                    State.Save();
                }

                ImGui::SameLine(240.0f * State.dpiScale);
                if (ToggleButton("Цикл Шляпы", &State.RandomHat)) {
                    State.Save();
                }

                if (ToggleButton("Цикл Визора", &State.RandomVisor)) {
                    State.Save();
                }

                ImGui::SameLine(120.0f * State.dpiScale);
                if (ToggleButton("Цикл Одежды", &State.RandomSkin)) {
                    State.Save();
                }

                ImGui::SameLine(240.0f * State.dpiScale);
                if (ToggleButton("Цикл Питом.", &State.RandomPet)) {
                    State.Save();
                }

                if (ToggleButton("Цикл Фона Ника", &State.RandomNamePlate)) {
                    State.Save();
                }

                if (IsHost() || !State.SafeMode) {
                    ImGui::SameLine();
                    if (ToggleButton(IsHost() ? "Циклер на Всех (Только Ник и Цвет)" : "Циклер на Всех", &State.CycleForEveryone)) {
                        State.Save();
                    }
                }
            }


            if (ImGui::CollapsingHeader("Настройки Цикла Ника")) {
                if (CustomListBoxInt("Генерация Ника", &State.cyclerNameGeneration, NAMEGENERATION, 75 * State.dpiScale)) {
                    State.Save();
                }
                if (State.cyclerNameGeneration == 2) {
                    if (State.cyclerUserNames.empty())
                        ImGui::TextColored(ImVec4(1.0f, 0.0f, 0.0f, 1.0f), "Генерация Ника Будет Работать Через Комбинации Слов, Потому что в Цикле нет Ников.");
                    static std::string newName = "";
                    InputString("Новый Ник", &newName, ImGuiInputTextFlags_EnterReturnsTrue);
                    ImGui::SameLine();
                    if (AnimatedButton("Добавить Ник")) {
                        State.cyclerUserNames.push_back(newName);
                        State.Save();
                        newName = "";
                    }
                    if (!(IsHost() || !State.SafeMode) && !IsNameValid(newName)) {
                        ImGui::TextColored(ImVec4(1.0f, 0.0f, 0.0f, 1.0f), "Ник Обнаружается Античитом. Ник будет Проигнорен.");
                    }
                    if (!State.cyclerUserNames.empty()) {
                        static int selectedNameIndex = 0;
                        selectedNameIndex = std::clamp(selectedNameIndex, 0, (int)State.cyclerUserNames.size() - 1);
                        std::vector<const char*> nameVector(State.cyclerUserNames.size(), nullptr);
                        for (size_t i = 0; i < State.cyclerUserNames.size(); i++) {
                            nameVector[i] = State.cyclerUserNames[i].c_str();
                        }
                        CustomListBoxInt("Ник для Удаления", &selectedNameIndex, nameVector);
                        ImGui::SameLine();
                        if (AnimatedButton("Удалить"))
                            State.cyclerUserNames.erase(State.cyclerUserNames.begin() + selectedNameIndex);
                    }
                }
            }

            if (ToggleButton("Путание Вида", &State.confuser)) {
                State.Save();
            }

            if (ImGui::CollapsingHeader("Настройки Запутания")) {
                if ((IsInGame() || IsInLobby()) && AnimatedButton("Запутать Вид")) {
                    ControlAppearance(true);
                }
                if (IsInGame() || IsInLobby()) {
                    if (IsHost() || !State.SafeMode)
                        ImGui::SameLine();
                }
                if ((IsInGame() || IsInLobby()) && !State.SafeMode && AnimatedButton("Рандомизировать Всех")) {
                    std::queue<RPCInterface*>* queue = nullptr;
                    if (IsInGame())
                        queue = &State.rpcQueue;
                    else if (IsInLobby())
                        queue = &State.lobbyRpcQueue;
                    std::vector availableHats = { "hat_NoHat", "hat_AbominalHat", "hat_anchor", "hat_antenna", "hat_Antenna_Black", "hat_arrowhead", "hat_Astronaut-Blue", "hat_Astronaut-Cyan", "hat_Astronaut-Orange", "hat_astronaut", "hat_axe", "hat_babybean", "hat_Baguette", "hat_BananaGreen", "hat_BananaPurple", "hat_bandanaWBY", "hat_Bandana_Blue", "hat_Bandana_Green", "hat_Bandana_Pink", "hat_Bandana_Red", "hat_Bandana_White", "hat_Bandana_Yellow", "hat_baseball_Black", "hat_baseball_Green", "hat_baseball_Lightblue", "hat_baseball_LightGreen", "hat_baseball_Lilac", "hat_baseball_Orange", "hat_baseball_Pink", "hat_baseball_Purple", "hat_baseball_Red", "hat_baseball_White", "hat_baseball_Yellow", "hat_Basketball", "hat_bat_crewcolor", "hat_bat_green", "hat_bat_ice", "hat_beachball", "hat_Beanie_Black", "hat_Beanie_Blue", "hat_Beanie_Green", "hat_Beanie_Lightblue", "hat_Beanie_LightGreen", "hat_Beanie_LightPurple", "hat_Beanie_Pink", "hat_Beanie_Purple", "hat_Beanie_White", "hat_Beanie_Yellow", "hat_bearyCold", "hat_bone", "hat_Bowlingball", "hat_brainslug", "hat_BreadLoaf", "hat_bucket", "hat_bucketHat", "hat_bushhat", "hat_Butter", "hat_caiatl", "hat_caitlin", "hat_candycorn", "hat_captain", "hat_cashHat", "hat_cat_grey", "hat_cat_orange", "hat_cat_pink", "hat_cat_snow", "hat_chalice", "hat_cheeseBleu", "hat_cheeseMoldy", "hat_cheeseSwiss", "hat_ChefWhiteBlue", "hat_cherryOrange", "hat_cherryPink", "hat_Chocolate", "hat_chocolateCandy", "hat_chocolateMatcha", "hat_chocolateVanillaStrawb", "hat_clagger", "hat_clown_purple", "hat_comper", "hat_croissant", "hat_crownBean", "hat_crownDouble", "hat_crownTall", "hat_CuppaJoe", "hat_Deitied", "hat_devilhorns_black", "hat_devilhorns_crewcolor", "hat_devilhorns_green", "hat_devilhorns_murky", "hat_devilhorns_white", "hat_devilhorns_yellow", "hat_Doc_black", "hat_Doc_Orange", "hat_Doc_Purple", "hat_Doc_Red", "hat_Doc_White", "hat_Dodgeball", "hat_Dorag_Black", "hat_Dorag_Desert", "hat_Dorag_Jungle", "hat_Dorag_Purple", "hat_Dorag_Sky", "hat_Dorag_Snow", "hat_Dorag_Yellow", "hat_doubletophat", "hat_DrillMetal", "hat_DrillStone", "hat_DrillWood", "hat_EarmuffGreen", "hat_EarmuffsPink", "hat_EarmuffsYellow", "hat_EarnmuffBlue", "hat_eggGreen", "hat_eggYellow", "hat_enforcer", "hat_erisMorn", "hat_fairywings", "hat_fishCap", "hat_fishhed", "hat_fishingHat", "hat_flowerpot", "hat_frankenbolts", "hat_frankenbride", "hat_fungleFlower", "hat_geoff", "hat_glowstick", "hat_glowstickCyan", "hat_glowstickOrange", "hat_glowstickPink", "hat_glowstickPurple", "hat_glowstickYellow", "hat_goggles", "hat_Goggles_Black", "hat_Goggles_Chrome", "hat_GovtDesert", "hat_GovtHeadset", "hat_halospartan", "hat_hardhat", "hat_Hardhat_black", "hat_Hardhat_Blue", "hat_Hardhat_Green", "hat_Hardhat_Orange", "hat_Hardhat_Pink", "hat_Hardhat_Purple", "hat_Hardhat_Red", "hat_Hardhat_White", "hat_HardtopHat", "hat_headslug_Purple", "hat_headslug_Red", "hat_headslug_White", "hat_headslug_Yellow", "hat_Heart", "hat_heim", "hat_Herohood_Black", "hat_Herohood_Blue", "hat_Herohood_Pink", "hat_Herohood_Purple", "hat_Herohood_Red", "hat_Herohood_Yellow", "hat_hl_fubuki", "hat_hl_gura", "hat_hl_korone", "hat_hl_marine", "hat_hl_mio", "hat_hl_moona", "hat_hl_okayu", "hat_hl_pekora", "hat_hl_risu", "hat_hl_watson", "hat_hunter", "hat_IceCreamMatcha", "hat_IceCreamMint", "hat_IceCreamNeo", "hat_IceCreamStrawberry", "hat_IceCreamUbe", "hat_IceCreamVanilla", "hat_Igloo", "hat_Janitor", "hat_jayce", "hat_jinx", "hat_killerplant", "hat_lilShroom", "hat_maraSov", "hat_mareLwyd", "hat_military", "hat_MilitaryWinter", "hat_MinerBlack", "hat_MinerYellow", "hat_mira_bush", "hat_mira_case", "hat_mira_cloud", "hat_mira_flower", "hat_mira_flower_red", "hat_mira_gem", "hat_mira_headset_blue", "hat_mira_headset_pink", "hat_mira_headset_yellow", "hat_mira_leaf", "hat_mira_milk", "hat_mira_sign_blue", "hat_mohawk_bubblegum", "hat_mohawk_bumblebee", "hat_mohawk_purple_green", "hat_mohawk_rainbow", "hat_mummy", "hat_mushbuns", "hat_mushroomBeret", "hat_mysteryBones", "hat_NewYear2023", "hat_OrangeHat", "hat_osiris", "hat_pack01_Astronaut0001", "hat_pack02_Tengallon0001", "hat_pack02_Tengallon0002", "hat_pack03_Stickynote0004", "hat_pack04_Geoffmask0001", "hat_pack06holiday_candycane0001", "hat_PancakeStack", "hat_paperhat", "hat_Paperhat_Black", "hat_Paperhat_Blue", "hat_Paperhat_Cyan", "hat_Paperhat_Lightblue", "hat_Paperhat_Pink", "hat_Paperhat_Yellow", "hat_papermask", "hat_partyhat", "hat_pickaxe", "hat_Pineapple", "hat_PizzaSliceHat", "hat_pk01_BaseballCap", "hat_pk02_Crown", "hat_pk02_Eyebrows", "hat_pk02_HaloHat", "hat_pk02_HeroCap", "hat_pk02_PipCap", "hat_pk02_PlungerHat", "hat_pk02_ScubaHat", "hat_pk02_StickminHat", "hat_pk02_StrawHat", "hat_pk02_TenGallonHat", "hat_pk02_ThirdEyeHat", "hat_pk02_ToiletPaperHat", "hat_pk02_Toppat", "hat_pk03_Fedora", "hat_pk03_Goggles", "hat_pk03_Headphones", "hat_pk03_Security1", "hat_pk03_StrapHat", "hat_pk03_Traffic", "hat_pk04_Antenna", "hat_pk04_Archae", "hat_pk04_Balloon", "hat_pk04_Banana", "hat_pk04_Bandana", "hat_pk04_Beanie", "hat_pk04_Bear", "hat_pk04_BirdNest", "hat_pk04_CCC", "hat_pk04_Chef", "hat_pk04_DoRag", "hat_pk04_Fez", "hat_pk04_GeneralHat", "hat_pk04_HunterCap", "hat_pk04_JungleHat", "hat_pk04_MinerCap", "hat_pk04_MiniCrewmate", "hat_pk04_Pompadour", "hat_pk04_RamHorns", "hat_pk04_Slippery", "hat_pk04_Snowman", "hat_pk04_Vagabond", "hat_pk04_WinterHat", "hat_pk05_Burthat", "hat_pk05_Cheese", "hat_pk05_cheesetoppat", "hat_pk05_Cherry", "hat_pk05_davehat", "hat_pk05_Egg", "hat_pk05_Ellie", "hat_pk05_EllieToppat", "hat_pk05_Ellryhat", "hat_pk05_Fedora", "hat_pk05_Flamingo", "hat_pk05_FlowerPin", "hat_pk05_GeoffreyToppat", "hat_pk05_Helmet", "hat_pk05_HenryToppat", "hat_pk05_Macbethhat", "hat_pk05_Plant", "hat_pk05_RHM", "hat_pk05_Svenhat", "hat_pk05_Wizardhat", "hat_pk06_Candycanes", "hat_pk06_ElfHat", "hat_pk06_Lights", "hat_pk06_Present", "hat_pk06_Reindeer", "hat_pk06_Santa", "hat_pk06_Snowman", "hat_pk06_tree", "hat_pkHW01_BatWings", "hat_pkHW01_CatEyes", "hat_pkHW01_Horns", "hat_pkHW01_Machete", "hat_pkHW01_Mohawk", "hat_pkHW01_Pirate", "hat_pkHW01_PlagueHat", "hat_pkHW01_Pumpkin", "hat_pkHW01_ScaryBag", "hat_pkHW01_Witch", "hat_pkHW01_Wolf", "hat_Plunger_Blue", "hat_Plunger_Yellow", "hat_police", "hat_Ponytail", "hat_Pot", "hat_Present", "hat_Prototype", "hat_pusheenGreyHat", "hat_PusheenicornHat", "hat_pusheenMintHat", "hat_pusheenPinkHat", "hat_pusheenPurpleHat", "hat_pusheenSitHat", "hat_pusheenSleepHat", "hat_pyramid", "hat_rabbitEars", "hat_Ramhorn_Black", "hat_Ramhorn_Red", "hat_Ramhorn_White", "hat_ratchet", "hat_Records", "hat_RockIce", "hat_RockLava", "hat_Rubberglove", "hat_Rupert", "hat_russian", "hat_saint14", "hat_sausage", "hat_savathun", "hat_schnapp", "hat_screamghostface", "hat_Scrudge", "hat_sharkfin", "hat_shaxx", "hat_shovel", "hat_SlothHat", "hat_SnowbeanieGreen", "hat_SnowbeanieOrange", "hat_SnowBeaniePurple", "hat_SnowbeanieRed", "hat_Snowman", "hat_Soccer", "hat_Sorry", "hat_starBalloon", "hat_starhorse", "hat_Starless", "hat_StarTopper", "hat_stethescope", "hat_StrawberryLeavesHat", "hat_TenGallon_Black", "hat_TenGallon_White", "hat_ThomasC", "hat_tinFoil", "hat_titan", "hat_ToastButterHat", "hat_tombstone", "hat_tophat", "hat_ToppatHair", "hat_towelwizard", "hat_Traffic_Blue", "hat_traffic_purple", "hat_Traffic_Red", "hat_Traffic_Yellow", "hat_Unicorn", "hat_vi", "hat_viking", "hat_Visor", "hat_Voleyball", "hat_w21_candycane_blue", "hat_w21_candycane_bubble", "hat_w21_candycane_chocolate", "hat_w21_candycane_mint", "hat_w21_elf_pink", "hat_w21_elf_swe", "hat_w21_gingerbread", "hat_w21_holly", "hat_w21_krampus", "hat_w21_lights_white", "hat_w21_lights_yellow", "hat_w21_log", "hat_w21_mistletoe", "hat_w21_mittens", "hat_w21_nutcracker", "hat_w21_pinecone", "hat_w21_present_evil", "hat_w21_present_greenyellow", "hat_w21_present_redwhite", "hat_w21_present_whiteblue", "hat_w21_santa_evil", "hat_w21_santa_green", "hat_w21_santa_mint", "hat_w21_santa_pink", "hat_w21_santa_white", "hat_w21_santa_yellow", "hat_w21_snowflake", "hat_w21_snowman", "hat_w21_snowman_evil", "hat_w21_snowman_greenred", "hat_w21_snowman_redgreen", "hat_w21_snowman_swe", "hat_w21_winterpuff", "hat_wallcap", "hat_warlock", "hat_whitetophat", "hat_wigJudge", "hat_wigTall", "hat_WilfordIV", "hat_Winston", "hat_WinterGreen", "hat_WinterHelmet", "hat_WinterRed", "hat_WinterYellow", "hat_witch_green", "hat_witch_murky", "hat_witch_pink", "hat_witch_white", "hat_wolf_grey", "hat_wolf_murky", "hat_Zipper" };
                    std::vector availableSkins = { "skin_None", "skin_Abominalskin", "skin_ApronGreen", "skin_Archae", "skin_Astro", "skin_Astronaut-Blueskin", "skin_Astronaut-Cyanskin", "skin_Astronaut-Orangeskin", "skin_Bananaskin", "skin_benoit", "skin_Bling", "skin_BlueApronskin", "skin_BlueSuspskin", "skin_Box1skin", "skin_BubbleWrapskin", "skin_Burlapskin", "skin_BushSign1skin", "skin_Bushskin", "skin_BusinessFem-Aquaskin", "skin_BusinessFem-Tanskin", "skin_BusinessFemskin", "skin_caitlin", "skin_Capt", "skin_CCC", "skin_ChefBlackskin", "skin_ChefBlue", "skin_ChefRed", "skin_clown", "skin_D2Cskin", "skin_D2Hunter", "skin_D2Osiris", "skin_D2Saint14", "skin_D2Shaxx", "skin_D2Titan", "skin_D2Warlock", "skin_enforcer", "skin_fairy", "skin_FishingSkinskin", "skin_fishmonger", "skin_FishSkinskin", "skin_General", "skin_greedygrampaskin", "skin_halospartan", "skin_Hazmat-Blackskin", "skin_Hazmat-Blueskin", "skin_Hazmat-Greenskin", "skin_Hazmat-Pinkskin", "skin_Hazmat-Redskin", "skin_Hazmat-Whiteskin", "skin_Hazmat", "skin_heim", "skin_hl_fubuki", "skin_hl_gura", "skin_hl_korone", "skin_hl_marine", "skin_hl_mio", "skin_hl_moona", "skin_hl_okayu", "skin_hl_pekora", "skin_hl_risu", "skin_hl_watson", "skin_Horse1skin", "skin_Hotdogskin", "skin_InnerTubeSkinskin", "skin_JacketGreenskin", "skin_JacketPurpleskin", "skin_JacketYellowskin", "skin_Janitorskin", "skin_jayce", "skin_jinx", "skin_LifeVestSkinskin", "skin_Mech", "skin_MechanicRed", "skin_Military", "skin_MilitaryDesert", "skin_MilitarySnowskin", "skin_Miner", "skin_MinerBlackskin", "skin_mummy", "skin_OrangeSuspskin", "skin_PinkApronskin", "skin_PinkSuspskin", "skin_Police", "skin_presentskin", "skin_prisoner", "skin_PrisonerBlue", "skin_PrisonerTanskin", "skin_pumpkin", "skin_PusheenGreyskin", "skin_Pusheenicornskin", "skin_PusheenMintskin", "skin_PusheenPinkskin", "skin_PusheenPurpleskin", "skin_ratchet", "skin_rhm", "skin_RockIceskin", "skin_RockLavaskin", "skin_Sack1skin", "skin_scarfskin", "skin_Science", "skin_Scientist-Blueskin", "skin_Scientist-Darkskin", "skin_screamghostface", "skin_Security", "skin_Skin_SuitRedskin", "skin_Slothskin", "skin_SportsBlueskin", "skin_SportsRedskin", "skin_SuitB", "skin_SuitW", "skin_SweaterBlueskin", "skin_SweaterPinkskin", "skin_Sweaterskin", "skin_SweaterYellowskin", "skin_Tarmac", "skin_ToppatSuitFem", "skin_ToppatVest", "skin_uglysweaterskin", "skin_vampire", "skin_vi", "skin_w21_deer", "skin_w21_elf", "skin_w21_msclaus", "skin_w21_nutcracker", "skin_w21_santa", "skin_w21_snowmate", "skin_w21_tree", "skin_Wall", "skin_Winter", "skin_witch", "skin_YellowApronskin", "skin_YellowSuspskin" };
                    std::vector availableVisors = { "visor_EmptyVisor", "visor_anime", "visor_BaconVisor", "visor_BananaVisor", "visor_beautyMark", "visor_BillyG", "visor_Blush", "visor_Bomba", "visor_BubbleBumVisor", "visor_Candycane", "visor_Carrot", "visor_chimkin", "visor_clownnose", "visor_Crack", "visor_CucumberVisor", "visor_D2CGoggles", "visor_Dirty", "visor_Dotdot", "visor_doubleeyepatch", "visor_eliksni", "visor_erisBandage", "visor_eyeball", "visor_EyepatchL", "visor_EyepatchR", "visor_fishhook", "visor_Galeforce", "visor_heim", "visor_hl_ah", "visor_hl_bored", "visor_hl_hmph", "visor_hl_marine", "visor_hl_nothoughts", "visor_hl_nudge", "visor_hl_smug", "visor_hl_sweepy", "visor_hl_teehee", "visor_hl_wrong", "visor_IceBeard", "visor_IceCreamChocolateVisor", "visor_IceCreamMintVisor", "visor_IceCreamStrawberryVisor", "visor_IceCreamUbeVisor", "visor_is_beard", "visor_JanitorStache", "visor_jinx", "visor_Krieghaus", "visor_Lava", "visor_LolliBlue", "visor_LolliBrown", "visor_LolliOrange", "visor_lollipopCrew", "visor_lollipopLemon", "visor_lollipopLime", "visor_LolliRed", "visor_marshmallow", "visor_masque_blue", "visor_masque_green", "visor_masque_red", "visor_masque_white", "visor_mira_card_blue", "visor_mira_card_red", "visor_mira_glasses", "visor_mira_mask_black", "visor_mira_mask_blue", "visor_mira_mask_green", "visor_mira_mask_purple", "visor_mira_mask_red", "visor_mira_mask_white", "visor_Mouth", "visor_mummy", "visor_PiercingL", "visor_PiercingR", "visor_PizzaVisor", "visor_pk01_AngeryVisor", "visor_pk01_DumStickerVisor", "visor_pk01_FredVisor", "visor_pk01_HazmatVisor", "visor_pk01_MonoclesVisor", "visor_pk01_PaperMaskVisor", "visor_pk01_PlagueVisor", "visor_pk01_RHMVisor", "visor_pk01_Security1Visor", "visor_Plsno", "visor_polus_ice", "visor_pusheenGorgeousVisor", "visor_pusheenKissyVisor", "visor_pusheenKoolKatVisor", "visor_pusheenOmNomNomVisor", "visor_pusheenSmileVisor", "visor_pusheenYaaaaaayVisor", "visor_Reginald", "visor_Rudolph", "visor_savathun", "visor_Scar", "visor_SciGoggles", "visor_shopglasses", "visor_shuttershadesBlue", "visor_shuttershadesLime", "visor_shuttershadesPink", "visor_shuttershadesPurple", "visor_shuttershadesWhite", "visor_shuttershadesYellow", "visor_SkiGoggleBlack", "visor_SKiGogglesOrange", "visor_SkiGogglesWhite", "visor_SmallGlasses", "visor_SmallGlassesBlue", "visor_SmallGlassesRed", "visor_starfish", "visor_Stealthgoggles", "visor_Stickynote_Cyan", "visor_Stickynote_Green", "visor_Stickynote_Orange", "visor_Stickynote_Pink", "visor_Stickynote_Purple", "visor_Straw", "visor_sunscreenv", "visor_teary", "visor_ToastVisor", "visor_tvColorTest", "visor_vr_Vr-Black", "visor_vr_Vr-White", "visor_w21_carrot", "visor_w21_nutstache", "visor_w21_nye", "visor_w21_santabeard", "visor_wash", "visor_WinstonStache" };
                    std::vector availablePets = { "pet_EmptyPet", "pet_Alien", "pet_Bedcrab", "pet_BredPet", "pet_Bush", "pet_Charles", "pet_Charles_Red", "pet_ChewiePet", "pet_clank", "pet_coaltonpet", "pet_Creb", "pet_Crewmate", "pet_Cube", "pet_D2GhostPet", "pet_D2PoukaPet", "pet_D2WormPet", "pet_Doggy", "pet_Ellie", "pet_frankendog", "pet_GuiltySpark", "pet_HamPet", "pet_Hamster", "pet_HolidayHamPet", "pet_Lava", "pet_nuggetPet", "pet_Pip", "pet_poro", "pet_Pusheen", "pet_Robot", "pet_Snow", "pet_Squig", "pet_Stickmin", "pet_Stormy", "pet_test", "pet_UFO", "pet_YuleGoatPet" };
                    std::vector availableNamePlates = { "nameplate_NoPlate", "nameplate_cliffs", "nameplate_grill", "nameplate_plant", "nameplate_sandcastle", "nameplate_zipline", "nameplate_pusheen_01", "nameplate_pusheen_02", "nameplate_pusheen_03", "nameplate_pusheen_04", "nameplate_flagAro", "nameplate_flagMlm", "nameplate_hunter", "nameplate_Polus_DVD", "nameplate_Polus_Ground", "nameplate_Polus_Lava", "nameplate_Polus_Planet", "nameplate_Polus_Snow", "nameplate_Polus_SpecimenBlue", "nameplate_Polus_SpecimenGreen", "nameplate_Polus_SpecimenPurple", "nameplate_is_yard", "nameplate_is_dig", "nameplate_is_game", "nameplate_is_ghost", "nameplate_is_green", "nameplate_is_sand", "nameplate_is_trees", "nameplate_Mira_Cafeteria", "nameplate_Mira_Glass", "nameplate_Mira_Tiles", "nameplate_Mira_Vines", "nameplate_Mira_Wood", "nameplate_hw_candy", "nameplate_hw_woods", "nameplate_hw_pumpkin" };
                    //help me out with the nameplates, couldn't find them in the game assets
                    for (auto player : GetAllPlayerControl()) {
                        std::string name = "";
                        if (State.confuserNameGeneration == 0 || (State.confuserNameGeneration == 2 && State.cyclerUserNames.empty()))
                            name = GenerateRandomString();
                        else if (State.confuserNameGeneration == 1)
                            name = GenerateRandomString(true);
                        else if (State.confuserNameGeneration == 2) {
                            if (!State.cyclerUserNames.empty())
                                name = State.cyclerUserNames[randi(0, State.cyclerUserNames.size() - 1)] + "<size=0>" + std::to_string(player->fields.PlayerId) + "</size>";
                        }
                        else
                            name = GenerateRandomString();
                        queue->push(new RpcForceName(player, name));
                        queue->push(new RpcForceColor(player, randi(0, 17)));
                        queue->push(new RpcForceHat(player, convert_to_string(availableHats[randi(0, availableHats.size() - 1)])));
                        queue->push(new RpcForceSkin(player, convert_to_string(availableSkins[randi(0, availableSkins.size() - 1)])));
                        queue->push(new RpcForceVisor(player, convert_to_string(availableVisors[randi(0, availableVisors.size() - 1)])));
                        queue->push(new RpcForcePet(player, convert_to_string(availablePets[randi(0, availablePets.size() - 1)])));
                        queue->push(new RpcForceNamePlate(player, convert_to_string(availableNamePlates[randi(0, availableNamePlates.size() - 1)])));
                    }
                }

                ImGui::Text("Путаться при:");
                if (ToggleButton("Заходе в Лобби", &State.confuseOnJoin)) {
                    State.Save();
                }
                ImGui::SameLine();
                if (ToggleButton("Начале Игры", &State.confuseOnStart)) {
                    State.Save();
                }
                ImGui::SameLine();
                if (ToggleButton("Убийстве", &State.confuseOnKill)) {
                    State.Save();
                }
                ImGui::SameLine();
                if (ToggleButton("В Люке", &State.confuseOnVent)) {
                    State.Save();
                }

                if (ToggleButton("На Голосовании", &State.confuseOnMeeting)) {
                    State.Save();
                }
            }
            if (ImGui::CollapsingHeader("Настройки Путания Ника")) {
                if (CustomListBoxInt("Генерация Ника", &State.confuserNameGeneration, NAMEGENERATION, 75 * State.dpiScale)) {
                    State.Save();
                }
                if (State.confuserNameGeneration == 2) {
                    if (State.cyclerUserNames.empty())
                        ImGui::TextColored(ImVec4(1.0f, 0.0f, 0.0f, 1.0f), "Генерация Ника Будет Работать Через Комбинации Слов, Потому что в Цикле нет Ников.");
                    static std::string newName = "";
                    InputString("Новый Ник ", &newName, ImGuiInputTextFlags_EnterReturnsTrue);
                    ImGui::SameLine();
                    if (AnimatedButton("Добавить Ник ")) {
                        State.cyclerUserNames.push_back(newName);
                        State.Save();
                        newName = "";
                    }
                    if (!(IsHost() || !State.SafeMode) && !IsNameValid(newName)) {
                        ImGui::TextColored(ImVec4(1.0f, 0.0f, 0.0f, 1.0f), "Ник Обнаружается Античитом. Ник будет Проигнорен.");
                    }
                    if (!State.cyclerUserNames.empty()) {
                        static int selectedNameIndex = 0;
                        selectedNameIndex = std::clamp(selectedNameIndex, 0, (int)State.cyclerUserNames.size() - 1);
                        std::vector<const char*> nameVector(State.cyclerUserNames.size(), nullptr);
                        for (size_t i = 0; i < State.cyclerUserNames.size(); i++) {
                            nameVector[i] = State.cyclerUserNames[i].c_str();
                        }
                        CustomListBoxInt("Ник для Удаления", &selectedNameIndex, nameVector);
                        ImGui::SameLine();
                        if (AnimatedButton("Удалить "))
                            State.cyclerUserNames.erase(State.cyclerUserNames.begin() + selectedNameIndex);
                    }
                }
            }
        }
        if (openTextEditor) {
            InputString("Вход", &originalText);
            editedText = GetTextEditorName(originalText);
            InputString("Выход", &editedText);
            ImGui::SameLine();
            if (AnimatedButton("Скопировать")) ClipboardHelper_PutClipboardString(convert_to_string(editedText), NULL);

            if (ToggleButton("Курсив", &italicName)) {
                State.Save();
            }
            ImGui::SameLine();
            if (ToggleButton("Подчеркнутый", &underlineName)) {
                State.Save();
            }
            ImGui::SameLine();
            if (ToggleButton("Зачеркнутый", &strikethroughName)) {
                State.Save();
            }
            ImGui::SameLine();
            if (ToggleButton("Толстый", &boldName)) {
                State.Save();
            }

            if (ToggleButton("Без переноса", &nobrName)) {
                State.Save();
            }

            if (ImGui::ColorEdit4("Начало Градиента", (float*)&nameColor1, ImGuiColorEditFlags__OptionsDefault | ImGuiColorEditFlags_NoInputs | ImGuiColorEditFlags_AlphaBar | ImGuiColorEditFlags_AlphaPreview)) {
                State.Save();
            }
            ImGui::SameLine();
            if (ImGui::ColorEdit4("Конец Градиента", (float*)&nameColor2, ImGuiColorEditFlags__OptionsDefault | ImGuiColorEditFlags_NoInputs | ImGuiColorEditFlags_AlphaBar | ImGuiColorEditFlags_AlphaPreview)) {
                State.Save();
            }
            ImGui::SameLine();
            if (ToggleButton("Цветной", &coloredName)) {
                State.Save();
            }

            ImGui::Dummy(ImVec2(2, 2) * State.dpiScale);

            if (ToggleButton("Шрифт", &font)) {
                State.Save();
            }
            ImGui::SameLine();
            if (CustomListBoxInt(" ", &fontType, FONTS, 160.f * State.dpiScale)) {
                State.Save();
            }
            ImGui::Dummy(ImVec2(-5, -5) * State.dpiScale);
            if (State.Font) ImGui::TextColored(ImVec4(1.0f, 1.0f, 1.0f, 1.0f), ("Примечание: Белый Ник не Будет Виден в Чате"));

            ImGui::Dummy(ImVec2(2, 2) * State.dpiScale);

            /*if (ToggleButton("Material", &State.Material)) {
                State.Save();
            }
            ImGui::SameLine();
            if (CustomListBoxInt(" Some materials are not supported", &State.MaterialType, MATERIALS, 160.f * State.dpiScale)) {
                State.Save();
            }*/

            ImGui::Dummy(ImVec2(10, 10) * State.dpiScale);
            if (ToggleButton("Размер", &resizeName)) {
                State.Save();
            }

            ImGui::SameLine();
            if (ImGui::InputFloat("", &nameSize)) {
                State.Save();
            }

            ImGui::Dummy(ImVec2(5, 5) * State.dpiScale);
            if (ToggleButton("Отступ", &indentName)) {
                State.Save();
            }

            ImGui::SameLine();
            if (ImGui::InputFloat("", &indentLevel)) {
                State.Save();
            }

            ImGui::Dummy(ImVec2(5, 5) * State.dpiScale);
            if (ToggleButton("Интервал", &cspaceName)) {
                State.Save();
            }

            ImGui::SameLine();
            if (ImGui::InputFloat("Символов", &cspaceLevel)) {
                State.Save();
            }

            ImGui::Dummy(ImVec2(5, 5) * State.dpiScale);
            if (ToggleButton("Межстроч.", &mspaceName)) {
                State.Save();
            }

            ImGui::SameLine();
            if (ImGui::InputFloat("Растоян.", &mspaceLevel)) {
                State.Save();
            }

            ImGui::Dummy(ImVec2(5, 5) * State.dpiScale);
            if (ToggleButton("Верт. Сдвиг", &voffsetName)) {
                State.Save();
            }

            ImGui::SameLine();
            if (ImGui::InputFloat("", &voffsetLevel)) {
                State.Save();
            }

            ImGui::Dummy(ImVec2(5, 5) * State.dpiScale);
            if (ToggleButton("Поворот", &rotateName)) {
                State.Save();
            }

            ImGui::SameLine();
            if (ImGui::InputFloat("Угол", &rotateAngle)) {
                State.Save();
            }
        }
        ImGui::EndChild();
    }
}
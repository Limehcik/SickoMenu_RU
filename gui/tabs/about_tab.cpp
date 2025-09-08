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
#include <cstdlib>

namespace AboutTab {
    enum Groups {
        Welcome,
        Credits
    };

    static bool openWelcome = true; //default to welcome tab group
    static bool openCredits = false;

    void CloseOtherGroups(Groups group) {
        openWelcome = group == Groups::Welcome;
        openCredits = group == Groups::Credits;
    }

    const ImVec4 SickoCol = ImVec4(1.f, 0.f, 0.424f, 1.f);
    const ImVec4 RedCol = ImVec4(1.f, 0.f, 0.f, 1.0f);
    const ImVec4 AumCol = ImVec4(1.f, 0.3333f, 0.3333f, 1.0f);
    const ImVec4 GoldCol = ImVec4(1.f, 0.7333f, 0.f, 1.0f);
    const ImVec4 GoatCol = ImVec4(0.937f, 0.004f, 0.263f, 1.0f);
    const ImVec4 DevCol = ImVec4(0.102f, 0.7373f, 0.6118f, 1.0f);
    const ImVec4 ContributorCol = ImVec4(0.3804f, 0.4314f, 0.7961f, 1.0f);
    const ImVec4 LimeCol = ImVec4(0.0f, 1.0f, 0.0f, 1.0f);

    void Render() {
        ImGui::SameLine(100 * State.dpiScale);
        ImGui::BeginChild("###Информация", ImVec2(500 * State.dpiScale, 0), true, ImGuiWindowFlags_NoBackground);
        if (TabGroup("Приветствие", openWelcome)) {
            CloseOtherGroups(Groups::Welcome);
        }
        ImGui::SameLine();
        if (TabGroup("Авторы", openCredits)) {
            CloseOtherGroups(Groups::Credits);
        }

        if (openWelcome) {
            ImGui::Text(std::format("Приветсвуем {}в ", State.HasOpenedMenuBefore ? "" : "").c_str());
            ImGui::SameLine(0.0f, 0.0f);
            ImGui::TextColored(SickoCol, "SickoMenu");
            ImGui::SameLine(0.0f, 0.0f);
            ImGui::TextColored(GoldCol, std::format(" {}", State.SickoVersion).c_str());
            ImGui::SameLine(0.0f, 0.0f);
            ImGui::Text(" от ");
            ImGui::SameLine(0.0f, 0.0f);
            ImGui::TextColored(GoatCol, "g0aty");
            ImGui::SameLine(0.0f, 0.0f);
            ImGui::Text("!");

            ImGui::TextColored(SickoCol, "SickoMenu");
            ImGui::SameLine(0.0f, 0.0f);
            ImGui::Text(" это мощная утилита для Among Us.");
            ImGui::Text("Её цель - улучшить игровой опыт для всех игроков!");
            ImGui::Text("Используйте кнопку \"Проверить обновления\" что бы скачать последний релиз!");
            if (ColoredButton(DevCol, "GitHub")) {
                OpenLink("https://github.com/g0aty/SickoMenu");
            }
            ImGui::SameLine();
            if (ColoredButton(GoldCol, "Проверить обновления")) {
                OpenLink("https://github.com/g0aty/SickoMenu/releases/latest");
            }
            ImGui::SameLine();
            if (ColoredButton(State.RgbColor, "Донат")) {
                OpenLink("https://ko-fi.com/g0aty");
            }
            ImGui::SameLine();
            if (ColoredButton(State.RgbColor, "Донат переводчику")) {
                OpenLink("");
            }

            ImGui::Text("Заходи в Discord сервер для поддержки, сообщения о багах, и сникпиках!");
            if (ColoredButton(ContributorCol, "Заходи в Discord!")) {
                OpenLink("https://dsc.gg/sickos"); //SickoMenu discord invite
            }

            ImGui::TextColored(SickoCol, "SickoMenu");
            ImGui::SameLine(0.0f, 0.0f);
            ImGui::Text(" это бесплатная программа с открытым кодом.");

            if (State.SickoVersion.find("pr") != std::string::npos || State.SickoVersion.find("rc") != std::string::npos) {
                if (State.SickoVersion.find("pr") != std::string::npos) ImGui::TextColored(State.RgbColor, "Вы имеете доступ к пре-релизу, наслаждайтесь!");
                else ImGui::TextColored(State.RgbColor, "Вы имеете доступ к релиз-кандиданту, наслаждайтесь!");
                BoldText("Если у вас нет доступа к каналу пре-релизов на нашем Discord и вы не", ImVec4(0.f, 1.f, 0.f, 1.f));
                BoldText("собирали самостоятельно, сообщите об этом нашей службе поддержки,", ImVec4(0.f, 1.f, 0.f, 1.f));
                BoldText("создав тикет на нашем сервере Discord!", ImVec4(0.f, 1.f, 0.f, 1.f));
            }
            else {
                ImGui::TextColored(ImVec4(1.f, 0.f, 0.f, 1.f), "Если вы заплатили за это меню, немедленно требуйте возврата денег.");
                BoldText("Убедитесь, что вы загрузили последнюю версию SickoMenu с GitHub, сайта", ImVec4(0.f, 1.f, 0.f, 1.f));
                BoldText("или официального Discord сервера!", ImVec4(0.f, 1.f, 0.f, 1.f));
            }
            //hopefully stop people from reselling a foss menu for actual money

            if (State.AprilFoolsMode) {
                ImGui::Dummy(ImVec2(7, 7) * State.dpiScale);
                auto DiddyCol = ImVec4(0.79f, 0.03f, 1.f, 1.f);
                ImGui::TextColored(DiddyCol, std::format("Теперь у вас есть доступ к совершенно новому режиму: {} Режим!", State.DiddyPartyMode ? "Дидди Вечеринка" :
                (IsChatCensored() || IsStreamerMode() ? "Х**в" : "Хуев")).c_str());
                ImGui::TextColored(DiddyCol, "Откройте для себя все новые функции и наслаждайтесь!");
                if (ToggleButton("Режим вечеринки Дидди", &State.DiddyPartyMode)) {
                    if (State.RizzUpEveryone) State.RizzUpEveryone = false;
                    State.Save();
                }
            }
        }

        if (openCredits) {
            ImGui::TextColored(SickoCol, "SickoMenu");
            ImGui::SameLine(0.0f, 0.0f);
            ImGui::Text(" это форк");
            ImGui::SameLine(0.0f, 0.0f);
            ImGui::TextColored(AumCol, " AmongUsMenu");
            ImGui::SameLine(0.0f, 0.0f);
            ImGui::TextColored(RedCol, " (в архиве)");
            ImGui::SameLine(0.0f, 0.0f);
            ImGui::Text(", иди и проверь это!");

            if (ColoredButton(AumCol, "AmongUsMenu")) {
                OpenLink("https://github.com/BitCrackers/AmongUsMenu");
            }
            BoldText("Главный разработчик", GoldCol);
            if (ColoredButton(GoatCol, "g0aty")) {
                OpenLink("https://github.com/g0aty");
            }

            BoldText("Переводчик на русский", GoldCol);
            if (ColoredButton(LimeCol, "Limehcik")) {
                OpenLink("https://github.com/Limehcik");
            }

            BoldText("Разработчики", DevCol);
            if (ColoredButton(DevCol, "GDjkhp")) {
                OpenLink("https://github.com/GDjkhp");
            }
            ImGui::SameLine(100.f * State.dpiScale);
            if (ColoredButton(DevCol, "Reycko")) {
                OpenLink("https://github.com/Reycko");
            }
            ImGui::SameLine(200.f * State.dpiScale);
            if (ColoredButton(DevCol, "astra1dev")) {
                OpenLink("https://github.com/astra1dev");
            }

            if (ColoredButton(DevCol, "Luckyheat")) {
                OpenLink("https://github.com/Luckyheat");
            }
            ImGui::SameLine(100.f * State.dpiScale);
            if (ColoredButton(DevCol, "UN83991956")) {
                OpenLink("https://github.com/UN83991956");
            }
            ImGui::SameLine(200.f * State.dpiScale);
            if (ColoredButton(DevCol, "HarithGamerPk")) {
                OpenLink("https://github.com/HarithGamerPk");
            }

            if (ColoredButton(DevCol, "dark-lord333")) {
                OpenLink("https://github.com/dark-lord333");
            }
            ImGui::SameLine(100.f * State.dpiScale);
            if (ColoredButton(DevCol, "WhoAboutYT")) {
                OpenLink("https://github.com/WhoAboutYT");
            }
            ImGui::SameLine(200.f * State.dpiScale);
            if (ColoredButton(DevCol, "M4-sicko")) {
                OpenLink("https://github.com/M4-sicko");
            }

            BoldText("Авторы", ContributorCol);
            if (ColoredButton(ContributorCol, "acer51-doctom")) {
                OpenLink("https://github.com/acer51-doctom");
            }
            ImGui::SameLine(100.f * State.dpiScale);
            if (ColoredButton(ContributorCol, "ZamTDS")) {
                OpenLink("https://github.com/ZamTDS");
            }

            BoldText("Некоторые люди, которые внесли свой вклад в AUM", AumCol);
            if (ColoredButton(AumCol, "KulaGGin")) {
                OpenLink("https://github.com/KulaGGin");
            }
            ImGui::SameLine();
            ImGui::Text("(Помог с кодом ImGui для системы Повтора)");

            if (ColoredButton(AumCol, "tomsa000")) {
                OpenLink("https://github.com/tomsa000");
            }
            ImGui::SameLine();
            ImGui::Text("(Помог с устранением утечек памяти и умных указателей)");

            if (ColoredButton(AumCol, "cddjr")) {
                OpenLink("https://github.com/cddjr");
            }
            ImGui::SameLine();
            ImGui::Text("(Помог в обновлении до версии с картой Fungle)");

            ImGui::Text("Благодарность");
            ImGui::SameLine(0.0f, 0.0f);
            ImGui::TextColored(AumCol, " v0idp");
            ImGui::SameLine(0.0f, 0.0f);
            ImGui::Text(" за создание");
            ImGui::SameLine(0.0f, 0.0f);
            ImGui::TextColored(AumCol, " AmongUsMenu");
            ImGui::SameLine(0.0f, 0.0f);
            ImGui::Text("!");
            if (ColoredButton(AumCol, "v0idp")) {
                OpenLink("https://github.com/v0idp");
            }

            ImGui::Text("Все остальные, кто внес свой вклад в");
            ImGui::SameLine(0.0f, 0.0f);
            ImGui::TextColored(AumCol, " AUM,");
            ImGui::SameLine(0.0f, 0.0f);
            ImGui::Text(" которых я не смог перечислить здесь.");

            ImGui::Text("Спасибо, что сделали ");
            ImGui::SameLine(0.0f, 0.0f);
            ImGui::TextColored(SickoCol, "SickoMenu");
            ImGui::SameLine(0.0f, 0.0f);
            ImGui::Text(" возможным!");
        }
        ImGui::EndChild();
    }
}
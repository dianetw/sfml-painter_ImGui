#include <imgui.h>
#include <imgui-SFML.h>
#include <SFML/Graphics.hpp>

int main() {
    // �]�w�����j�p�P�W��
    sf::VideoMode mode(900, 650);
    sf::RenderWindow window(mode, L"�p�e�a");
    ImGui::SFML::Init(window);
    ImGui::StyleColorsLight();
    // �]�w�I����
    sf::Texture background;
    if (!background.loadFromFile("background.png")) {
        return EXIT_FAILURE;
    }
    sf::Sprite backgroundSprite(background);
    backgroundSprite.setPosition(0, 50);

    // �]�w�e��
    sf::RenderTexture renderTexture;
    renderTexture.create(600, 560);
    renderTexture.clear(sf::Color::White);
    renderTexture.display();

    sf::Sprite renderSprite(renderTexture.getTexture());
    sf::Vector2f offset(20, 70);
    renderSprite.setPosition(offset);

    // �]�w�e��
    sf::CircleShape cursor(10.0f);
    cursor.setOrigin(10.f, 10.f);
    cursor.setFillColor(sf::Color::Black);

    enum class State {
        Idle,
        Drawing,
    };

    // �]�w�ثe���A
    State state = State::Idle;

    sf::Clock deltaClock;
    auto& io = ImGui::GetIO();
    io.Fonts->Clear();
    io.Fonts->AddFontFromFileTTF("arial.ttf", 18.f);
    ImGui::SFML::UpdateFontTexture();

    while (window.isOpen()) {
        // ���o�ƹ��۹��������m
        auto pos = sf::Vector2f(sf::Mouse::getPosition(window));

        sf::Event evt;
        if (window.pollEvent(evt)) {
            ImGui::SFML::ProcessEvent(window, evt);
            switch (evt.type) {
            case sf::Event::Closed:
                window.close();
                break;
            case sf::Event::MouseButtonPressed:
                if (io.WantCaptureMouse) break;
                // ����U�ƹ�����B�ƹ��b�e�����ɡA�N���A�ഫ��ø�s��
                if (evt.mouseButton.button == sf::Mouse::Button::Left) {
                    if (renderSprite.getGlobalBounds().contains(pos)) {
                        state = State::Drawing;
                    }
                }
                break;
            case sf::Event::MouseButtonReleased:
                // ���}�ƹ�����ɡA�N���A�ഫ��o�b��
                if (evt.mouseButton.button == sf::Mouse::Button::Left) {
                    state = State::Idle;
                }
            }
        }

        ImGui::SFML::Update(window, deltaClock.restart());

        if (ImGui::BeginMainMenuBar()) {
            if (ImGui::BeginMenu("File")) {
                if (ImGui::MenuItem("New")) {
                    renderTexture.clear(sf::Color(255, 255, 255));
                }
                if (ImGui::MenuItem("Save")) {
                    renderTexture.getTexture().copyToImage().saveToFile("newfile.png");
                }
                ImGui::EndMenu();
            }
            ImGui::EndMainMenuBar();
        }
        ImGui::SetNextWindowPos(ImVec2(20, 25));
        ImGui::Begin("Swatch", 0,
            ImGuiWindowFlags_NoDecoration |
            ImGuiWindowFlags_NoBackground |
            ImGuiWindowFlags_AlwaysAutoResize |
            ImGuiWindowFlags_NoMove);
        if (ImGui::ColorButton("Black", ImVec4(0.0, 0.0, 0.0, 1.0))) {
            cursor.setFillColor(sf::Color(0, 0, 0));
        }
        ImGui::SameLine();
        if (ImGui::ColorButton("White", ImVec4(1.0, 1.0, 1.0, 1.0))) {
            cursor.setFillColor(sf::Color(255, 255, 255));
        }
        ImGui::SameLine();
        if (ImGui::ColorButton("Red", ImVec4(1.0, 0.0, 0.0, 1.0))) {
            cursor.setFillColor(sf::Color(255, 0, 0));
        }
        ImGui::SameLine();
        if (ImGui::ColorButton("Orange", ImVec4(1.0, (float)0.647, 0.0, 1.0))) {
            cursor.setFillColor(sf::Color(255, 165, 0));
        }
        ImGui::SameLine();
        if (ImGui::ColorButton("Yellow", ImVec4(1.0, 1.0, 0.0, 1.0))) {
            cursor.setFillColor(sf::Color(255, 255, 0));
        }
        ImGui::SameLine();
        if (ImGui::ColorButton("Green", ImVec4(0.0, 1.0, 0.0, 1.0))) {
            cursor.setFillColor(sf::Color(0, 255, 0));
        }
        ImGui::SameLine();
        if (ImGui::ColorButton("Blue", ImVec4(0.0, 0.0, 1.0, 1.0))) {
            cursor.setFillColor(sf::Color(0, 0, 255));
        }
        ImGui::SameLine();
        float cursorSize = cursor.getRadius();
        if (ImGui::SliderFloat("Cursor Size", &cursorSize, 1.0f, 50.f)) {
            cursor.setRadius(cursorSize);
        }
        ImGui::End();

        ImGui::SetNextWindowPos(ImVec2(float(window.getSize().x - 270), 30));
        ImGui::SetNextWindowSize(ImVec2(250, 250));
        ImGui::Begin("Color Picker Window", 0, ImGuiWindowFlags_NoResize);
        sf::Color cursorColor = cursor.getFillColor();
        float col[3] = { cursorColor.r/255.0f, cursorColor.g/255.0f, cursorColor.b/255.0f};
        if (ImGui::ColorPicker3("Colors", col)) {
            cursor.setFillColor(sf::Color(col[0] * 255, col[1] * 255, col[2] * 255));
        }
        ImGui::End();

        // �p�G���A�Oø�s�����ܡA�N�e���e��e���W
        if (state == State::Drawing) {
            cursor.setPosition(pos - offset);
            renderTexture.draw(cursor);
            renderTexture.display();
        }

        // �N�����M�Ũȩ̀ǵe�W�I����e��
        window.clear(sf::Color(230, 230, 230));
        window.draw(backgroundSprite);
        window.draw(renderSprite);

        if (!io.WantCaptureMouse) {
            // ��ƹ��b�e�����ɡA�N�e���e�X
            if (renderSprite.getGlobalBounds().contains(pos)) {
                cursor.setPosition(pos);
                window.draw(cursor);
            }
        }

        ImGui::SFML::Render(window);  // draw ui

        // ��s�e��
        window.display();
    }
    ImGui::SFML::Shutdown();
}
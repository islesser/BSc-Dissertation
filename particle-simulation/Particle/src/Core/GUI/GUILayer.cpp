#include "PCHeader.h"
#include "GUILayer.h"

namespace PT
{
GUILayer::GUILayer()
{
}

GUILayer::~GUILayer()
{
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();
}

bool GUILayer::init(GLFWwindow *_window)
{
    window = _window;
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    io = &ImGui::GetIO();
    (void)io;
    io->ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard; // Enable Keyboard Controls
    io->ConfigFlags |= ImGuiConfigFlags_DockingEnable;   // Enable Docking
    io->ConfigFlags |= ImGuiConfigFlags_ViewportsEnable; // Enable Multi-Viewport / Platform Windows

    ImGui::StyleColorsDark();

    ImGuiStyle &style = ImGui::GetStyle();
    if (io->ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
    {
        style.WindowRounding = 0.0f;
        style.Colors[ImGuiCol_WindowBg].w = 1.0f;
    }

    ImGui_ImplGlfw_InitForOpenGL(window, true);
    ImGui_ImplOpenGL3_Init("#version 410");

    return true;
}

void GUILayer::addElement(std::function<void()> window)
{
    windows.push_back(window);
}

void GUILayer::render(Simulation *sim)
{
    PROFILE("GUILayer::render");

    bool hov = ImGui::IsWindowHovered(ImGuiFocusedFlags_AnyWindow);
    GC::get()->updateBool("GUI_HOVER", hov);

    {
        ImGui::Begin("Debug Variables");
        {
            // Framerate / time
            ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);

            // Fill
            ImGui::Text("Fill: %d / %d",
                        GC::get()->getInt("CURR_NO_PARTICLES"),
                        GC::get()->getInt("MAX_PARTICLES"));

            bool debug = sim->shouldDrawDebug();
            ImGui::Checkbox("Debug Draw", &debug);
            sim->setDrawDebug(debug);

            // Speed.
            glm::vec2 speed = sim->getSpeed();
            ImGui::SliderFloat("Min Speed", &speed.x, -10.0f, 0.0f, "%.2f");
            ImGui::SliderFloat("Max Speed", &speed.y, 0.0f, 10.0f, "%.2f");
            sim->setSpeed(speed);

            // Drag coefficient
            float dragCoeff = sim->getDragCoeff();
            ImGui::SliderFloat("Drag Coefficient", &dragCoeff, 0.0f, 1.0f, "%.2f");
            sim->setDragCoeff(dragCoeff);

            // Gravity
            float gravity = sim->getGravity();
            ImGui::SliderFloat("Gravity", &gravity, 0.0f, 3.0f, "%.3f");
            sim->setGravity(gravity);

            // Samples
            int samples = sim->getSamples();
            ImGui::SliderInt("Samples", &samples, 0.0f, 10.0f, "%d");
            sim->setSamples(samples);

            // Sample Strength
            float ss = sim->getSampleStrength();
            ImGui::SliderFloat("Sample Str.", &ss, 0.0f, 2.0f, "%.2f");
            sim->setSampleStrength(ss);

            // Sample Strength Degredation
            float ssd = sim->getSampleStengthDegradation();
            ImGui::SliderFloat("Sample Str Deg.", &ssd, 0.0f, 1.0f, "%.2f");
            sim->setSampleStengthDegradation(ssd);

            // ------------ FORCE GRID 0 -------------------------------------
            // ---------------------------------------------------------------
            ImGui::Text("Forcegrid 0");
            auto forcegrid0 = sim->Force(0);

            int octaves = forcegrid0->getOctaves();
            float per = forcegrid0->getPersistance();
            ImGui::SliderInt("Octaves", &octaves, 1.0f, 8.0f, "%d");
            ImGui::SliderFloat("Persistance", &per, 0.0f, 1.0f, "%.2f");
            forcegrid0->setOctaves(octaves);
            forcegrid0->setPersistance(per);

            bool actualMag = forcegrid0->isShowingActualMagnitude();
		  	ImGui::Checkbox("Actual Magnitude", &actualMag);
		  	forcegrid0->setShowActualMagnitude(actualMag);

		  	float mag = forcegrid0->getMaxMagnitude();
		  	ImGui::SliderFloat("Maximum Magnitude", &mag, 0.0f, 20.0f, "%.2f");
		  	forcegrid0->setMaxMagnitude(mag);
        }
        ImGui::End();
    }

    for (auto w : windows)
    {
        {
            w();
        }
    }
}

void GUILayer::begin()
{
    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplGlfw_NewFrame();
    ImGui::NewFrame();
}

void GUILayer::end()
{
    // io->DisplaySize = ImVec2((float)window->getWidth(), (float)window->getHeight());

    // Rendering
    ImGui::Render();
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

    if (io->ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
    {
        GLFWwindow *backup_current_context = glfwGetCurrentContext();
        ImGui::UpdatePlatformWindows();
        ImGui::RenderPlatformWindowsDefault();
        glfwMakeContextCurrent(backup_current_context);
    }
}

void GUILayer::constantElements()
{

    static bool opt_fullscreen_persistant = true;
    bool opt_fullscreen = opt_fullscreen_persistant;
    static ImGuiDockNodeFlags dockspace_flags = ImGuiDockNodeFlags_PassthruCentralNode;

    // We are using the ImGuiWindowFlags_NoDocking flag to make the parent window not dockable into,
    // because it would be confusing to have two docking targets within each others.
    ImGuiWindowFlags window_flags = ImGuiWindowFlags_MenuBar | ImGuiWindowFlags_NoDocking;
    if (opt_fullscreen)
    {
        ImGuiViewport *viewport = ImGui::GetMainViewport();
        ImGui::SetNextWindowPos(viewport->Pos);
        ImGui::SetNextWindowSize(viewport->Size);
        ImGui::SetNextWindowViewport(viewport->ID);
        ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.0f);
        ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f);
        window_flags |= ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove;
        window_flags |= ImGuiWindowFlags_NoBringToFrontOnFocus | ImGuiWindowFlags_NoNavFocus;
    }

    // When using ImGuiDockNodeFlags_PassthruCentralNode, DockSpace() will render our background and handle the pass-thru hole, so we ask Begin() to not render a background.
    if (dockspace_flags & ImGuiDockNodeFlags_PassthruCentralNode)
        window_flags |= ImGuiWindowFlags_NoBackground;

    // Important: note that we proceed even if Begin() returns false (aka window is collapsed).
    // This is because we want to keep our DockSpace() active. If a DockSpace() is inactive,
    // all active windows docked into it will lose their parent and become undocked.
    // We cannot preserve the docking relationship between an active window and an inactive docking, otherwise
    // any change of dockspace/settings would lead to windows being stuck in limbo and never being visible.
    bool a = true;
    ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 0.0f));
    ImGui::Begin("DockSpace Demo", &a, window_flags);
    ImGui::PopStyleVar();

    if (opt_fullscreen)
        ImGui::PopStyleVar(2);

    // DockSpace
    if (io->ConfigFlags & ImGuiConfigFlags_DockingEnable)
    {
        ImGuiID dockspace_id = ImGui::GetID("MyDockSpace");
        ImGui::DockSpace(dockspace_id, ImVec2(0.0f, 0.0f), dockspace_flags);
    }

    if (ImGui::BeginMenuBar())
    {
        if (ImGui::BeginMenu("Docking"))
        {
            // Disabling fullscreen would allow the window to be moved to the front of other windows,
            // which we can't undo at the moment without finer window depth/z control.
            //ImGui::MenuItem("Fullscreen", NULL, &opt_fullscreen_persistant);

            if (ImGui::MenuItem("Flag: NoSplit", "", (dockspace_flags & ImGuiDockNodeFlags_NoSplit) != 0))
                dockspace_flags ^= ImGuiDockNodeFlags_NoSplit;
            if (ImGui::MenuItem("Flag: NoResize", "", (dockspace_flags & ImGuiDockNodeFlags_NoResize) != 0))
                dockspace_flags ^= ImGuiDockNodeFlags_NoResize;
            if (ImGui::MenuItem("Flag: NoDockingInCentralNode", "", (dockspace_flags & ImGuiDockNodeFlags_NoDockingInCentralNode) != 0))
                dockspace_flags ^= ImGuiDockNodeFlags_NoDockingInCentralNode;
            if (ImGui::MenuItem("Flag: PassthruCentralNode", "", (dockspace_flags & ImGuiDockNodeFlags_PassthruCentralNode) != 0))
                dockspace_flags ^= ImGuiDockNodeFlags_PassthruCentralNode;
            if (ImGui::MenuItem("Flag: AutoHideTabBar", "", (dockspace_flags & ImGuiDockNodeFlags_AutoHideTabBar) != 0))
                dockspace_flags ^= ImGuiDockNodeFlags_AutoHideTabBar;
            ImGui::Separator();
            ImGui::EndMenu();
        }

        ImGui::EndMenuBar();
    }
    ImGui::End();
}

} // namespace PT

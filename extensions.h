#pragma once

#include <imgui.h>
#include <imgui_impl_glut.h>
#include <imgui_impl_opengl2.h>

class j_imgui {
public:
    void init() {
        // Setup Dear ImGui context
        IMGUI_CHECKVERSION();
        ImGui::CreateContext();
        ImGuiIO& io = ImGui::GetIO(); (void)io;
        //io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;     // Enable Keyboard Controls

        // Setup Dear ImGui style
        ImGui::StyleColorsDark();
//        ImGui::StyleColorsLight();
//        ImGui::StyleColorsClassic();

        // Setup Platform/Renderer backends
        ImGui_ImplGLUT_Init();
        ImGui_ImplGLUT_InstallFuncs();
        ImGui_ImplOpenGL2_Init();
    }
    void draw() {
        beforeDraw();
//        bool open = true;
//        ImGui::ShowDemoWindow(&open);
        static float f = 0.0f;
        ImGui::Begin("Hello, world!");
        ImGui::Text("This is some useful text.");
        ImGui::SliderFloat("float", &f, 0.0f, 1.0f);
        ImGui::End();
//        std::cout << "win pos: " << ImGui::GetWindowPos().x << "; " << ImGui::GetWindowPos().y << std::endl;
        afterDraw();
    }

    void close() {
        // Cleanup
        ImGui_ImplOpenGL2_Shutdown();
        ImGui_ImplGLUT_Shutdown();
        ImGui::DestroyContext();
    }

    void beforeDraw() {
        // Start the Dear ImGui frame
        ImGui_ImplOpenGL2_NewFrame();
        ImGui_ImplGLUT_NewFrame();
    }
    void afterDraw() {
        // Rendering
        ImGui::Render();
        ImGuiIO& io = ImGui::GetIO();
        //        glViewport(0, 0, (GLsizei)io.DisplaySize.x, (GLsizei)io.DisplaySize.y);
        //        glClearColor(clear_color.x * clear_color.w, clear_color.y * clear_color.w, clear_color.z * clear_color.w, clear_color.w);
//        glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
//        glClear(GL_COLOR_BUFFER_BIT);
//        glUseProgram(0); // You may want this if using this code in an OpenGL 3+ context where shaders may be bound, but prefer using the GL3+ code.
        ImGui_ImplOpenGL2_RenderDrawData(ImGui::GetDrawData());
    }
};
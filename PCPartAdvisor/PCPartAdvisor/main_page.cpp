// INSTRUCTIONS
// THE PROGRAM CAN DO THE FOLLOWING:
// 1. Read GPU options from a file called "gpudb.txt"
// 2. Display a dropdown for GPU, CPU, and PC case
// 3. Display the total price of the selected components
// 4. Display additional information based on the selected components
// 5. Display a button to optimize the selections to lower price
// 6. Display a prompt for the user's budget
// 7. Display a note if the selected CPU is i7
// 8. Display an upgrade option if the selected GPU is 1080
// 9. Display advice if the selected PC case is Luxury and the user's budget is low




#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <imgui.h>
#include <imgui_impl_glfw.h>
#include <imgui_impl_opengl3.h>

// Callback function to handle window resizing
void framebuffer_size_callback(GLFWwindow* window, int width, int height) {
    glViewport(0, 0, width, height);
}

int main() {
    // Initialize GLFW
    if (!glfwInit()) {
        std::cerr << "Failed to initialize GLFW" << std::endl;
        return -1;
    }

    // Set GLFW to use OpenGL version 3.3
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    // Create a GLFW window
    GLFWwindow* window = glfwCreateWindow(800, 600, "PC Parts advisor", NULL, NULL);
    if (window == NULL) {
        std::cerr << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);

    // Initialize GLEW
    if (glewInit() != GLEW_OK) {
        std::cerr << "Failed to initialize GLEW" << std::endl;
        return -1;
    }

    // Set the viewport size
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
    glViewport(0, 0, 800, 600);

    // Setup ImGui context
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO(); (void)io;
    ImGui_ImplGlfw_InitForOpenGL(window, true);
    ImGui_ImplOpenGL3_Init("#version 330");

    // Read options from gpudb.txt
    std::ifstream gpuFile("gpudb.txt");
    std::vector<std::string> gpuOptions;
    if (gpuFile.is_open()) {
        std::string line;
        while (std::getline(gpuFile, line)) {
            gpuOptions.push_back(line);
        }
        gpuFile.close();
    }

    // CPU options
    std::vector<const char*> cpuOptions = { "i5 10th gen", "i7 8th gen" };
    float cpuPrices[] = { 100.0f, 120.0f }; // Prices for CPU options (i5, i7)

    // PC case options
    std::vector<const char*> caseOptions = { "Lian Li Luxury", "Basic" };
    float casePrices[] = { 200.0f, 35.0f };

    // Prices
    float gpuPrices[] = { 150.0f, 170.0f }; // Prices for GPU options (1080, 2070)

    // Selected components
    int selectedGpu = 0, selectedCpu = 0, selectedCase = 0;

    // User budget
    float userBudget = 0;

    // Main loop
    while (!glfwWindowShouldClose(window)) {
        // Poll and handle events
        glfwPollEvents();

        // Start the ImGui frame
        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();

        // Create a simple ImGui window
        ImGui::Begin("Parts selector");

        // Prompt for user budget
        ImGui::InputFloat("Budget", &userBudget);

        // GPU dropdown
        ImGui::Text("GPU Dropdown:");
        if (ImGui::BeginCombo("gpu_dd", gpuOptions[selectedGpu].c_str())) {
            for (int i = 0; i < gpuOptions.size(); ++i) {
                bool isSelected = (selectedGpu == i);
                if (ImGui::Selectable(gpuOptions[i].c_str(), isSelected))
                    selectedGpu = i;
                if (isSelected)
                    ImGui::SetItemDefaultFocus();
            }
            ImGui::EndCombo();
        }

        // CPU dropdown
        ImGui::Text("CPU Dropdown:");
        if (ImGui::BeginCombo("cpu_dd", cpuOptions[selectedCpu])) {
            for (int i = 0; i < cpuOptions.size(); ++i) {
                bool isSelected = (selectedCpu == i);
                if (ImGui::Selectable(cpuOptions[i], isSelected))
                    selectedCpu = i;
                if (isSelected)
                    ImGui::SetItemDefaultFocus();
            }
            ImGui::EndCombo();
        }

        // PC case dropdown
        ImGui::Text("PC Case Dropdown:");
        if (ImGui::BeginCombo("case_dd", caseOptions[selectedCase])) {
            for (int i = 0; i < caseOptions.size(); ++i) {
                bool isSelected = (selectedCase == i);
                if (ImGui::Selectable(caseOptions[i], isSelected))
                    selectedCase = i;
                if (isSelected)
                    ImGui::SetItemDefaultFocus();
            }
            ImGui::EndCombo();
        }

        // Calculate total price
        float totalPrice = gpuPrices[selectedGpu] + cpuPrices[selectedCpu] + casePrices[selectedCase];
        ImGui::Text("Total Price: %.2f", totalPrice);

        // Additional information based on selection
        if (selectedCpu == 1) { // If i7 is chosen
            ImGui::Text("Note: No room for upgrade. Expensive to upgrade in future.");
        }
        if (selectedGpu == 0) { // If 1080 is chosen
            ImGui::Text("Upgrade Option: For just 20 pounds, get up to 20%% more performance and new ray tracing technology.");
        }
        if (selectedCase == 0 && userBudget < 500) { // If Luxury case is chosen and budget is low
            ImGui::Text("Advice: On a low budget, we advise choosing a cheaper PC case like 'Basic'. PC Cases do not affect PC performance other than having better quality fans in some cases.");
        }

        // Button to optimize selections
        if (ImGui::Button("Optimize my selections")) {
            // Find the cheapest GPU
            float minGpuPrice = gpuPrices[0];
            int cheapestGpuIndex = 0;
            for (int i = 1; i < sizeof(gpuPrices) / sizeof(float); ++i) {
                if (gpuPrices[i] < minGpuPrice) {
                    minGpuPrice = gpuPrices[i];
                    cheapestGpuIndex = i;
                }
            }
            selectedGpu = cheapestGpuIndex;

            // Find the cheapest CPU
            float minCpuPrice = cpuPrices[0];
            int cheapestCpuIndex = 0;
            for (int i = 1; i < sizeof(cpuPrices) / sizeof(float); ++i) {
                if (cpuPrices[i] < minCpuPrice) {
                    minCpuPrice = cpuPrices[i];
                    cheapestCpuIndex = i;
                }
            }
            selectedCpu = cheapestCpuIndex;

            // Find the cheapest PC case
            float minCasePrice = casePrices[0];
            int cheapestCaseIndex = 0;
            for (int i = 1; i < sizeof(casePrices) / sizeof(float); ++i) {
                if (casePrices[i] < minCasePrice) {
                    minCasePrice = casePrices[i];
                    cheapestCaseIndex = i;
                }
            }
            selectedCase = cheapestCaseIndex;
        }

        ImGui::End();

        // Rendering
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        ImGui::Render();
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

        // Swap buffers
        glfwSwapBuffers(window);
    }

    // Cleanup
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();

    glfwDestroyWindow(window);
    glfwTerminate();

    return 0;
}

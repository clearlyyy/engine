#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#define STB_IMAGE_IMPLEMENTATION
#include <imgui/imgui.h>
#include <imgui/imgui_impl_glfw.h>
#include <imgui/imgui_impl_opengl3.h>
#include "shader.h"
#include "camera.h"
#include "model.h"
//#include "cube.h"
#include "chunk.h"

#include <glm/gtx/string_cast.hpp>

#include <iostream>
#include <string>
#include <sstream>
#include <vector>

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void processInput(GLFWwindow* window);
void mouse_callback(GLFWwindow* window, double xpos, double ypos);
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
unsigned int loadTexture(const char* path);
unsigned int loadCubemap(vector<std::string> faces);
// settings
const unsigned int SCR_WIDTH = 1500;
const unsigned int SCR_HEIGHT = 720;

float deltaTime = 0.0f;
float lastFrame = 0.0f;

bool cursorMode = false;

float lastX = SCR_WIDTH / 2.0f;
float lastY = SCR_HEIGHT / 2.0f;
bool firstMouse = true;

glm::vec3 lightPos(1.2f, 1.0f, 2.0f);

//Chunk chunk2(glm::vec3(48.0f, 0.0f, 0.0f));
//Chunk chunk(glm::vec3(0.0f, 0.0f, 0.0f));
Chunk* newChunk = new Chunk(glm::vec3(0.0f, 0.0f, 0.0f));
float origSpeed;
float boostSpeed;

int main()
{
    // glfw: initialize and configure
    // ------------------------------
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_REFRESH_RATE, 140);


#ifdef __APPLE__
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif

    // glfw window creation
    // --------------------
    GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "shitcraft", NULL, NULL);
    if (window == NULL)
    {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }

    glfwMakeContextCurrent(window);
    glfwSwapInterval(0);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
    glfwSetCursorPosCallback(window, mouse_callback);
    glfwSetScrollCallback(window, scroll_callback);

    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
    // glad: load all OpenGL function pointers
    // ---------------------------------------
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return -1;
    }

    glEnable(GL_DEPTH_TEST);

    //Shader lightingShader("shaders/cube.glsl", "shaders/light.glsl");
    //Shader ourShader("shaders/cube.glsl", "shaders/light.glsl");
    //Shader lightCubeShader("shaders/suntex.glsl", "shaders/sun.glsl");
    Shader skyboxShader("shaders/skyboxV.glsl", "shaders/skyboxF.glsl");

    camera.MouseSensitivity = 0.1f;

    stbi_set_flip_vertically_on_load(true);

    unsigned int diffuseMap = loadTexture("resources/backpack/diffuse.jpg");
    unsigned int specularMap = loadTexture("resources/backpack/specular.jpg");

    std::string backpackModel = "resources/backpack/backpack.obj";
    //Model ourModel("resources/backpack/backpack.obj");
    //skybox init
    unsigned int skyboxVAO, skyboxVBO;
    glGenVertexArrays(1, &skyboxVAO);
    glGenBuffers(1, &skyboxVBO);
    glBindVertexArray(skyboxVAO);
    glBindBuffer(GL_ARRAY_BUFFER, skyboxVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(skyboxVertices), &skyboxVertices, GL_STATIC_DRAW);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);

    // load textures
    // -------------
    vector<std::string> faces
    {
        "textures/skybox2/right.png",
        "textures/skybox2/left.png",
        "textures/skybox2/top.png",
        "textures/skybox2/bottom.png",
        "textures/skybox2/front.png",
        "textures/skybox2/back.png",
    };
    unsigned int cubemapTexture = loadCubemap(faces);

    // shader configuration
    // --------------------
    skyboxShader.use();
    skyboxShader.setInt("skybox", 0);

    camera.MovementSpeed = 4.0f;
    origSpeed = camera.MovementSpeed;
    boostSpeed = camera.MovementSpeed += 16.0f;

    camera.Zoom = 90.0f;
    camera.Pitch = 45.0f / 2;

    //IMGUI SETUP 
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO(); (void)io;
    ImGui::StyleColorsDark();
    ImGui_ImplGlfw_InitForOpenGL(window, true);
    ImGui_ImplOpenGL3_Init("#version 330");

    // render loop
    // -----------

    //glm::vec3 lightColor;
    glm::vec3 backpackPos = glm::vec3(0.0f, 0.0f, 0.0f);
    glm::vec3 backpackRot = glm::vec3(0.0f, 0.0f, 0.0f);

    glm::vec3 cubePos = glm::vec3(0.0f, 0.0f, 0.0f);
    glm::vec3 cubeRot = glm::vec3(0.0f, 0.0f, 0.0f);

    glm::vec3 chunkSize = glm::vec3(48.0f, 128.0f, 48.0f);
    int chunkSeed = 1;
    float Freq = 0.03f;

    bool wireframe = false;

    //Cube tCube(cubePos, "textures/dirt.jpg", "textures/dirt.jpg");

    //Chunk* chunk = new Chunk(glm::vec3(0.0f, 0.0f, 0.0f));
    //Chunk* newChunk = new Chunk(glm::vec3(48.0f, 0.0f, 0.0f));
    //Chunk* newChunk2 = new Chunk(glm::vec3(0.0f, 0.0f, 48.0f));
    //Chunk* newChunk3 = new Chunk(glm::vec3(48.0f, 0.0f, 48.0f));

    std::vector<Chunk> world;

    int renderDistance = 10;

    for (int i = -renderDistance; i < renderDistance; i++) {
        for (int j = -renderDistance; j < renderDistance; j++) {
            world.push_back(Chunk(glm::vec3(i*48, 0.0f, j*48)));
        }
    }
    
    //glEnable(GL_CULL_FACE);
    //glCullFace(GL_BACK);

    //glm::vec3 chunkSize2 = chunk->getChunkSize();

    

    for (int i = 0; i < world.size(); i++)
        world[i].initChunk2();


        

    

   //chunk->initChunk2();
   //
   //
   //newChunk->initChunk2();
   //
   //
   //newChunk2->initChunk2();
   //
   //
   //newChunk3->initChunk2();


    int selectedBlock[4] = {0,0,0,0};

    float frameRate;
    double timer = 1.0f;
    glm::vec3 lightDirection = glm::vec3(-0.2f, -1.0f, 0.208f);
    glm::vec3 ambient = glm::vec3(0.7f, 0.7f, 0.7f);
    glm::vec3 diffuse = glm::vec3(0.8f, 0.8f, 0.8f);
    std::string frameRateStr;

    bool cinemaMode = false;
    glm::vec3 tempPosition = glm::vec3(-169.496063, 321.925934, -168.110580);
    float tempZoom = 18.0f;
    glm::vec3 tempFront = glm::vec3(0.500839, -0.710798, 0.493890);
    float tempPitch = -45.299854;
    float tempYaw = -43;
    float bias = -.1f;

    //chunk.updateLight(lightDirection, ambient, diffuse);

    while (!glfwWindowShouldClose(window))
    {

        glm::vec3 tempChunkSize = chunkSize;
        int tempSeed = chunkSeed;
        float tempFreq = Freq;
        float tempbias = bias;
        glm::vec3 templightDirection = lightDirection;
        glm::vec3 tempAmbient = ambient;
        glm::vec3 tempDiffuse = diffuse;
        bool tempCinemaMode = cinemaMode;

        float currentFrame = static_cast<float>(glfwGetTime());
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;

        frameRate = 1.0f / deltaTime;

        //std::cout << frameRate << "\n;
        timer += timer * deltaTime;
        if (timer > 1.5) {
            frameRateStr = std::to_string(frameRate);
            timer = 1.0;
        }

        processInput(window);
        glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();


        if (wireframe)
            glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
        else
            glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

        
        projection = glm::perspective(glm::radians(camera.Zoom), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.01f, 1000.0f);
        view = camera.GetViewMatrix();
        
        //newChunk->DrawChunk(glm::vec3(48.0f, 0, 0));
        //newChunk2->DrawChunk(glm::vec3(48.0f, 0, 0));
        //newChunk3->DrawChunk(glm::vec3(48.0f, 0, 0));
        //chunk->DrawChunk(glm::vec3(48.0f,0,0));

        for (int i = 0; i < world.size(); i++)
            world[i].DrawChunk();
        

        
  
        glDepthFunc(GL_LEQUAL);  // change depth function so depth test passes when values are equal to depth buffer's content
        skyboxShader.use();
        view = glm::mat4(glm::mat3(camera.GetViewMatrix())); // remove translation from the view matrix
        skyboxShader.setMat4("view", view);
        skyboxShader.setMat4("projection", projection);
        
        // skybox cube
        glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
        glBindVertexArray(skyboxVAO);
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_CUBE_MAP, cubemapTexture);
        glDrawArrays(GL_TRIANGLES, 0, 36);
        glBindVertexArray(0);
        glDepthFunc(GL_LESS);

        const char* camStr = "camera.Position: ";
        

        ImGui::Begin("SHITCRAFT_DEBUG");
        ImGui::Text("SHITCRAFT_DEBUG_MENU");
        ImGui::Text("FPS: ");
        ImGui::SameLine();
        ImGui::Text(frameRateStr.c_str());
        ImGui::Checkbox("Wireframe?", &wireframe);
        ImGui::SameLine();
        ImGui::Checkbox("Cinematic Mode", &cinemaMode);
        ImGui::Text("------------------------------------");
        ImGui::Text("camera.Position");
        ImGui::SameLine();
        ImGui::Text(glm::to_string(camera.Position).c_str());
        ImGui::Text(std::to_string(camera.Zoom).c_str());
        ImGui::Text(glm::to_string(camera.Front).c_str());
        ImGui::Text(std::to_string(camera.Pitch).c_str());
        ImGui::Text(std::to_string(camera.Yaw).c_str());
        ImGui::Text("------------------------------------");
        ImGui::Text("CHUNK_PROPERTIES");
        //ImGui::SliderFloat3("Chunk Size", &chunkSize.x, 1.0f, chunkSize2.x);
        ImGui::Text("BLOCK_PROPERTIES");
        ImGui::InputInt4("addBlock", selectedBlock);
        if (ImGui::Button("Add")) {
          // chunk->addBlock(selectedBlock[0], selectedBlock[1], selectedBlock[2], selectedBlock[3]);
           //chunk->genMesh();
           //chunk->initChunk2();
        }
        ImGui::Text("PERLIN_NOISE_PROPERTIES");
        ImGui::SliderInt("Seed", &chunkSeed, 1, 200);
        ImGui::SliderFloat("Freq", &Freq, 0.0, 1.0);
        ImGui::SliderFloat("Bias", &bias, -1.0, 1.0);
        ImGui::Text("LIGHTING_PROPERTIES");
        ImGui::SliderFloat3("LightDir", &lightDirection.x, -1.0f, 1.0f);
        ImGui::SliderFloat("Diffuse", &diffuse.x, -1.0f, 1.0f);
        ImGui::SliderFloat("Ambient", &ambient.x,  0.0f, 1.0f);

        if (tempChunkSize != chunkSize)
        {
            //chunk->setSize(chunkSize);
            //chunk->updateSize();
        }

        if (tempSeed != chunkSeed)
        {
            //chunk->genChunkWorld(chunkSeed, Freq, bias);
            //chunk->setSize(chunkSize);
            //chunk->updateSize();
        }
        if (tempbias != bias)
        {
            //chunk->genChunkWorld(chunkSeed, Freq, bias);
            //chunk->setSize(chunkSize);
            //chunk->updateSize();
        }
        if (tempFreq != Freq)
        {
            //chunk->genChunkWorld(chunkSeed, Freq, bias);
            //chunk->setSize(chunkSize);
            //chunk->updateSize();
        }
        if (templightDirection != lightDirection)
        {
            //chunk->updateLight(lightDirection, glm::vec3(ambient.x, ambient.x, ambient.x), glm::vec3(diffuse.x, diffuse.x, diffuse.x));
        }
        if (tempAmbient != ambient)
        {
            //chunk->updateLight(lightDirection, glm::vec3(ambient.x, ambient.x, ambient.x), glm::vec3(diffuse.x, diffuse.x, diffuse.x));
        }
        if (tempDiffuse != diffuse)
        {
            //chunk->updateLight(lightDirection, glm::vec3(ambient.x, ambient.x, ambient.x), glm::vec3(diffuse.x, diffuse.x, diffuse.x));
        }
        if (tempCinemaMode != cinemaMode) {
            if (cinemaMode) {
                tempPosition = camera.Position;
                tempZoom = camera.Zoom;
                
                tempPitch = camera.Pitch;
                tempYaw = camera.Yaw;
                camera.Position = glm::vec3(-169.496063, 321.925934, -168.110580);
                camera.Zoom = 18.0f;
                camera.Front = glm::vec3(0.500839, -0.710798, 0.493890);
                camera.Pitch = -45.299854;
                camera.Yaw = -763;
            }
            else {      
                camera.Zoom = tempZoom;
                camera.Position = tempPosition;
                camera.Yaw = tempYaw;
                camera.Pitch = tempPitch;
            }
        }
        ImGui::End();

        ImGui::Render();
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

        // glfw: swap buffers and poll IO events (keys pressed/released, mouse moved etc.)
        // -------------------------------------------------------------------------------
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();

    // glfw: terminate, clearing all previously allocated GLFW resources.
    // ------------------------------------------------------------------
    glfwTerminate();
    return 0;
}




double seconds = 1.0;
// process all input: query GLFW whether relevant keys are pressed/released this frame and react accordingly
// ---------------------------------------------------------------------------------------------------------
void processInput(GLFWwindow* window)
{
    seconds += seconds * deltaTime;
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
        if (seconds > 1.5) {
            if (!cursorMode) {
                cursorMode = true;
                glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
                seconds = 1.0;
            }
            else {
                glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
                cursorMode = false;
                seconds = 1.0;
            }
        }
    }
    if (glfwGetKey(window, GLFW_KEY_F) == GLFW_PRESS) {
        if (seconds > 1.5) {
            //chunk.genMesh();
            //chunk.initChunk2();
            seconds = 1.0;
        }
    }
    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
        camera.ProcessKeyboard(FORWARD, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
        camera.ProcessKeyboard(BACKWARD, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
        camera.ProcessKeyboard(LEFT, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
        camera.ProcessKeyboard(RIGHT, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS)
        camera.ProcessKeyboard(UP, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_LEFT_CONTROL) == GLFW_PRESS || glfwGetKey(window, GLFW_KEY_C) == GLFW_PRESS)
        camera.ProcessKeyboard(DOWN, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS)
        camera.MovementSpeed = boostSpeed;
    //if (glfwGetKey(window, GLFW_KEY_F) == GLFW_PRESS)
    //    chunk.initChunk2();
    else
        camera.MovementSpeed = origSpeed;


}

void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    glViewport(0, 0, width, height);
}

void mouse_callback(GLFWwindow* window, double xposIn, double yposIn)
{

    float xpos = static_cast<float>(xposIn);
    float ypos = static_cast<float>(yposIn);

    if (firstMouse)
    {
        lastX = xpos;
        lastY = ypos;
        firstMouse = false;
    }

    float xoffset = xpos - lastX;
    float yoffset = lastY - ypos; // reversed since y-coordinates go from bottom to top

    lastX = xpos;
    lastY = ypos;

    if (!cursorMode) {
        camera.ProcessMouseMovement(xoffset, yoffset);
    }
}

void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
    camera.ProcessMouseScroll(static_cast<float>(yoffset));
}

unsigned int loadTexture(char const* path) {
    unsigned int textureID;
    glGenTextures(1, &textureID);

    int width, height, nrComponents;
    unsigned char* data = stbi_load(path, &width, &height, &nrComponents, 0);
    if (data) {
        GLenum format;
        if (nrComponents == 1)
            format = GL_RED;
        else if (nrComponents == 3)
            format = GL_RGB;
        else if (nrComponents == 4)
            format = GL_RGBA;

        glBindTexture(GL_TEXTURE_2D, textureID);
        glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

        stbi_image_free(data);
    }
    else
    {
        std::cout << "Texture failed to load at path: " << path << std::endl;
        stbi_image_free(data);
    }
    return textureID;
}

unsigned int loadCubemap(vector<std::string> faces)
{
    unsigned int textureID;
    glGenTextures(1, &textureID);
    glBindTexture(GL_TEXTURE_CUBE_MAP, textureID);

    int width, height, nrComponents;
    for (unsigned int i = 0; i < faces.size(); i++)
    {
        unsigned char* data = stbi_load(faces[i].c_str(), &width, &height, &nrComponents, 0);
        if (data)
        {
            glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
            stbi_image_free(data);
        }
        else
        {
            std::cout << "Cubemap texture failed to load at path: " << faces[i] << std::endl;
            stbi_image_free(data);
        }
    }
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);

    return textureID;
}

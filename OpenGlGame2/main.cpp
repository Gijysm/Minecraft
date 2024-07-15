#include "lib.h"
#include "Window.h"
#include "Camera.h"
#include "Mesh.h"
#include "Voxel.h"
#include "Chunk.h"
#include "Chunks.h"
#include "Event.h"
#include "VoxelRender.h"
#include "Shader.h"
#include "Png_loading.h"
#include "Texture.h"

// Variables
int16_t width = 1020, height = 800;
float r = static_cast<float>(rand() % 256) / 255.0f;
float g = static_cast<float>(rand() % 256) / 255.0f;
float b = static_cast<float>(rand() % 256) / 255.0f;
float a = 1.0f;
float vertices[] = {
    -1.0f, -1.0f, 0.0f, 0.0f, 1.0f, // bottom left
     1.0f, -1.0f, 0.0f, 1.0f, 1.0f,  // bottom right
    -1.0f,  1.0f, 0.0f, 0.0f, 0.0f,  // top left

     1.0f, -1.0f, 0.0f, 1.0f, 1.0f,  // bottom right
     1.0f,  1.0f, 0.0f, 1.0f, 0.0f,  // top right
    -1.0f,  1.0f, 0.0f, 0.0f, 0.0f   // top left
};


int artts[] = { 3, 2, 0 };



int main()
{
    // Initialize Window and Event
    Window::instalization(width, height, "OpenGL");
    Event::instalization();


    // Load Shader
    Shader* shader = load_shader("C:\\Users\\popka\\source\\repos\\OpenGlGame2\\Shaders\\main.glslf",
        "C:\\Users\\popka\\source\\repos\\OpenGlGame2\\Shaders\\main.glslv");
    if (shader == nullptr)
    {
        std::cerr << "ERROR::SHADER::PROGRAM_LINKING::COMPILATION_FAILED\n" << std::endl;
        Window::Terminate();
        return 0;
    }

    // Load Texture
    Texture* texture = load_texture("C:\\Users\\popka\\source\\repos\\OpenGlGame2\\ATLAS\\14w25a_textures_0.png");
    if (texture == nullptr)
    {
        std::cerr << "Failed to load texture" << std::endl;
        delete shader;
        Window::Terminate();
        return 0;
    }
    Chunks* chunks = new Chunks(8, 1, 8);
    Mesh** meshes = new Mesh * [chunks->Getvolume()];
    for (size_t i = 0; i < chunks->Getvolume(); i++)
        meshes[i] = nullptr;
    VoxelRender renderer(1024 * 1024 * 8);

    glEnable(GL_DEPTH_TEST);
    glEnable(GL_CULL_FACE);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    Camera* camera = new Camera(vec3(0,0,20), radians(90.0f));

     float lastTime = glfwGetTime();
    float currentTime = 0;
    float delta = 0;
    float CamX = 0, CamY = 0;
    // Main loop
    glClearColor(0.6, 0.62, 0.65, 1);
    while (!Window::shouldClose())
    {
        currentTime = glfwGetTime();
        delta = currentTime - lastTime;
        lastTime = currentTime;
        if (Event::justClicked(GLFW_MOUSE_BUTTON_1))
        {
            r = static_cast<float>(rand() % 256) / 255.0f;
            g = static_cast<float>(rand() % 256) / 255.0f;
            b = static_cast<float>(rand() % 256) / 255.0f;
            glClearColor(r, g, b, a);
        }
        if (Event::isKeyPressed(GLFW_KEY_TAB))
        {
            Event::toogleCursor();
        }
        if (Event::isKeyPressed(GLFW_KEY_W))
        {
            camera->getPosition() += camera->getFont() * 4.6f * delta;
        }
        if(Event::isKeyPressed(GLFW_KEY_S))
		{
			camera->getPosition() -=  camera->getFont() * 4.6f * delta;
		}
        if (Event::isKeyPressed(GLFW_KEY_A))
        {
            camera->getPosition() -= camera->getRight() * 4.6f * delta;
        }
        if (Event::isKeyPressed(GLFW_KEY_D))
        {
            camera->getPosition() += camera->getRight() * 4.6f * delta;
        }
        if (Event::isKeyPressed(GLFW_KEY_ESCAPE))
        {
            Window::shouldClose(true);
        }
        if(Event::getCursorLocked())
        {
            CamX += -(Event::getDeltaY() / Window::getHeight());
            CamY += -(Event::getDeltaX() / Window::getWidth());
        }
        camera->getRotation() = mat4(1.0f);
        camera->rotate(CamX,CamY, 0);
        Chunk* closes[27];

        for (size_t i = 0; i < chunks->Getvolume(); i++)
        {
            Chunk* chunk = chunks->GetChunk(i);
            if (meshes[i] != nullptr)
                delete meshes[i];
            for (int j = 0; j < 27; j++)
            {
                closes[j] = 0;
            }
            for (size_t j = 0; j < chunks->Getvolume(); j++)
            {
                Chunk* other = chunks->GetChunk(j);
                int dx = other->x - chunk->x;
                int dy = other->y - chunk->y;
                int dz = other->z - chunk->z;
                if (abs(dx) > 1 || abs(dy) > 1 || abs(dz) > 1)
                {
                    continue;
                }
                dx += 1;
                dy += 1;
                dz += 1;
                closes[(dy * 3 + dz) * 3 + dx] = other;
            }
            Mesh* mesh = renderer.render(chunk, (const Chunk**)closes);
            meshes[i] = mesh;
            

        }
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        shader->use();
        shader->uniform_mat4("projview", camera->getProjection() * camera->getView());
        texture->bind();
        mat4 model;
        for (size_t i = 0; i < chunks->Getvolume(); i++)
        {
            Chunk* chunk = chunks->GetChunk(i);
            Mesh* mesh = meshes[i];
            model = glm::translate(mat4(1.0f), vec3(chunk->x * _CHUNK_W, chunk->y * _CHUNK_H, chunk->z * _CHUNK_D));
            shader->uniform_mat4("model", model);
            mesh->draw(GL_TRIANGLES);
        }
        Window::swapBuffers();
        Event::PullEvents();
    }
    for (size_t i = 0; i < chunks->Getvolume(); i++)
    {
        delete meshes[i];
    }
    delete [] meshes;
    delete shader;
    delete texture;
    delete chunks;
    Window::Terminate();

}

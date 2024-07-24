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
#include "LightSolver.h"
#include "Lighting.h"
#include "LightMap.h"
#include "Files.h"
#include "Block.h"
#include "LineBranch.h"
#include "Png_loading.h"
#include "Texture.h"

// Variables
int16_t width = 1020, height = 800;
float vertices[] = {
    -0.01f, -0.01f, // bottom left
     0.01f,  0.01f,
    -0.01f,  0.01f,  // bottom right
     0.01f, -0.01f,
};

int artts[] = { 2, 0 };

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
    Shader* CrossShairshader = load_shader("C:\\Users\\popka\\source\\repos\\OpenGlGame2\\Shaders\\CrossShair.glslf",
        "C:\\Users\\popka\\source\\repos\\OpenGlGame2\\Shaders\\CrossShair.glslv");
    if (CrossShairshader == nullptr)
    {
        std::cerr << "ERROR::SHADER::PROGRAM_LINKING::COMPILATION_FAILED\n" << std::endl;
        Window::Terminate();
        return 0;
    }
    Shader* Lineshader = load_shader("C:\\Users\\popka\\source\\repos\\OpenGlGame2\\Shaders\\Lineshader.glslf",
        "C:\\Users\\popka\\source\\repos\\OpenGlGame2\\Shaders\\Lineshader.glslv");
    if (Lineshader == nullptr)
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
    {
        Block* block = new Block(1,1);
        Block::blocks[block->id] = block;

        block = new Block(2, 2);
        Block::blocks[block->id] = block;

        block = new Block(3, 4);
        block->textureFaces[2] = 2;
        block->textureFaces[3] = 1;
        Block::blocks[block->id] = block;

        block = new Block(4, 30);
        block->emission[0] = 10;
        block->emission[1] = 10;
        Block::blocks[block->id] = block;

    }
    Chunks* chunks = new Chunks(5, 5, 5);
    Mesh** meshes = new Mesh* [chunks->Getvolume()];
    for (size_t i = 0; i < chunks->Getvolume(); i++)
    {
        meshes[i] = nullptr;
    }
    VoxelRender renderer(1024 * 1024 * 8);
    LineBranch* branch = new LineBranch(4096);

    glEnable(GL_DEPTH_TEST);
    glEnable(GL_CULL_FACE);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    Camera* camera = new Camera(vec3(96, 96, 96), radians(70.0f));
    Mesh* CrossShair = new Mesh(vertices, 4, artts);

    float lastTime = glfwGetTime();
    float currentTime = 0;
    float delta = 0;
    float CamX = 0, CamY = 0;
    int Choosen_block = 4;
    Lighting::instalize(chunks);
    Lighting::onWorldLoaded();
    // Main loop
    glClearColor(0.6, 0.62, 0.65, 1);
    while (!Window::shouldClose())
    {
        currentTime = glfwGetTime();
        delta = currentTime - lastTime;
        lastTime = currentTime;
        if (Event::isKeyPressed(GLFW_KEY_TAB))
        {
            Event::toogleCursor();
        }
        if (Event::isKeyPressed(GLFW_KEY_F1))
        {
            unsigned char* buffer = new unsigned char[chunks->Getvolume() * _CHUNK_SIZE];
            chunks->write(buffer);
            write_to_file("C:\\Users\\popka\\source\\repos\\OpenGlGame2\\save.bin", (const char*) buffer, chunks->Getvolume() * _CHUNK_SIZE);
            cout << "Saved in: " << (chunks->Getvolume() * _CHUNK_SIZE ) << endl;

            delete[] buffer;
        }
        if (Event::isKeyPressed(GLFW_KEY_F2))
        {
            unsigned char* buffer = new unsigned char[chunks->Getvolume() * _CHUNK_SIZE];
            chunks->read(buffer);
            read_from_file("C:\\Users\\popka\\source\\repos\\OpenGlGame2\\save.bin", (char*)buffer, chunks->Getvolume() * _CHUNK_SIZE);

            delete[] buffer;
        }
        if (Event::isKeyPressed(GLFW_KEY_W))
        {
            camera->getPosition() += camera->getFont() * 7.6f * delta;
        }
        if (Event::isKeyPressed(GLFW_KEY_S))
        {
            camera->getPosition() -= camera->getFont() * 7.6f * delta;
        }
        if (Event::isKeyPressed(GLFW_KEY_A))
        {
            camera->getPosition() -= camera->getRight() * 7.6f * delta;
        }
        if (Event::isKeyPressed(GLFW_KEY_D))
        {
            camera->getPosition() += camera->getRight() * 7.6f * delta;
        }
        if (Event::isKeyPressed(GLFW_KEY_ESCAPE))
        {
            Window::shouldClose(true);
        }
        if (Event::getCursorLocked())
        {
            CamX += -(Event::getDeltaY() / Window::getHeight());
            CamY += -(Event::getDeltaX() / Window::getWidth());

            camera->getRotation() = mat4(1.0f);
            camera->rotate(CamX, CamY, 0);
        }
        {
            vec3 end;
            vec3 norm;
            vec3 iend;
            voxel* vox = chunks->rayCast(camera->getPosition(), camera->getFont(), 10, end, norm, iend);
            if (vox != nullptr)
            {
                branch->Cube(iend.x + 0.5f, iend.y + 0.5f, iend.z + 0.5f, 1.005f, 1.005f, 1.005f, 0, 0, 0, 0.5f);
                if (Event::justClicked(GLFW_MOUSE_BUTTON_1))
                {
                    int x = (int)(iend.x);
                    int y = (int)(iend.y);
                    int z = (int)(iend.z);
                    chunks->Set(x, y, z, 0);
                    Lighting::onBlockSet(x, y, z, 0);
                }
                if (Event::justClicked(GLFW_MOUSE_BUTTON_2))
                {
                    int x = (int)(iend.x) + (int) (norm.x);
                    int y = (int)(iend.y) + (int)(norm.y);
                    int z = (int)(iend.z) + (int)(norm.z);
                    chunks->Set(x, y, z, Choosen_block);
                    Lighting::onBlockSet(x, y, z, Choosen_block);
                }
            }
        }
        Chunk* closes[27];
        for (size_t i = 0; i < chunks->Getvolume(); i++) {
            if (i >= chunks->Getvolume()) {
                std::cerr << "Index out of bounds: " << i << std::endl;
                continue;
            }
            Chunk* chunk = chunks->chunks[i];
            if (!chunk->modifier)
                continue;
            chunk->modifier = false;
            if (meshes[i] != nullptr)
                delete meshes[i];
            for (int j = 0; j < 27; j++)
            {
                closes[j] = nullptr;
            }
            for (size_t j = 0; j < chunks->Getvolume(); j++) {
                Chunk* other = chunks->chunks[j];

                int ox = other->x - chunk->x;
                int oy = other->y - chunk->y;
                int oz = other->z - chunk->z;

                if (abs(ox) > 1 || abs(oy) > 1 || abs(oz) > 1)
                    continue;

                ox += 1;
                oy += 1;
                oz += 1;
                closes[(oy * 3 + oz) * 3 + ox] = other;
            }
            Mesh* mesh = renderer.render(chunk, (const Chunk**)closes);
            meshes[i] = mesh;
            
        }

        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        shader->use();
        shader->uniform_mat4("projview", camera->getProjection() * camera->getView());
        texture->bind();
        mat4 model(1.0f);
        for (size_t i = 0; i < chunks->Getvolume(); i++)
        {
            Chunk* chunk = chunks->chunks[i];
            Mesh* mesh = meshes[i];
            model = glm::translate(mat4(1.0f), vec3(chunk->x * _CHUNK_W + 0.5, chunk->y * _CHUNK_H + 0.5, chunk->z * _CHUNK_D + 0.5));
            shader->uniform_mat4("model", model);
            mesh->draw(GL_TRIANGLES);
        }
        CrossShairshader->use();
        CrossShair->draw(GL_LINES);
        Lineshader->use();
        Lineshader->uniform_mat4("projview", camera->getProjection() * camera->getView());
        glLineWidth(2);
        branch->render();
        Window::swapBuffers();
        Event::PullEvents();
    }
    Lighting::finalize();

    delete shader;
    delete texture;
    delete CrossShairshader;
    delete CrossShair;
    delete Lineshader;
    delete chunks;
    delete branch;
    Window::Terminate();
}

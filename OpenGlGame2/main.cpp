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
#include "LightMap.h"
#include "Files.h"
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
    Chunks* chunks = new Chunks(16, 16, 16);
    size_t chunkVolume = chunks->Getvolume();
    Mesh** meshes = new Mesh * [chunkVolume];
    for (size_t i = 0; i < chunkVolume; i++)
    {
        meshes[i] = nullptr;
    }
    VoxelRender renderer(1024 * 1024 * 8);
    LineBranch* branch = new LineBranch(4096);

    glEnable(GL_DEPTH_TEST);
    glEnable(GL_CULL_FACE);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    Camera* camera = new Camera(vec3(0, 0, 20), radians(70.0f));
    Mesh* CrossShair = new Mesh(vertices, 4, artts);

    float lastTime = glfwGetTime();
    float currentTime = 0;
    float delta = 0;
    float CamX = 0, CamY = 0;
    int Choosen_block = 3;
    LightSolver* SolverR = new LightSolver(chunks, 0);
    LightSolver* SolverG = new LightSolver(chunks, 1);
    LightSolver* SolverB = new LightSolver(chunks, 2);
    LightSolver* SolverS = new LightSolver(chunks, 3);
    for (size_t y = 0; y < chunks->h * _CHUNK_H; y++)
    {
        for (size_t z = 0; z < chunks->d * _CHUNK_D; z++)
        {
            for (size_t x = 0; x < chunks->w * _CHUNK_W; x++)
            {
                voxel* vox = chunks->Get(x, y, z);
                if (vox->id == 3)
                {
                    SolverR->Add(x, y, 15);
                    SolverG->Add(x, y, 15);
                    SolverB->Add(x, y, 15);
                }
            }
        }
    }
    for (size_t z = 0; z < chunks->d * _CHUNK_D; z++)
    {
        for (size_t x = 0; x < chunks->w * _CHUNK_W; x++)
        {
            for (size_t y = chunks->h * _CHUNK_H - 1; y >= 0; y--)
            {
                voxel* vox = chunks->Get(x, y, z);
                if (vox->id != 0) {
                    break;
                }
                chunks->GetChunkByVoxel(x, y, z)->lightmap->SetS(x % _CHUNK_W, y % _CHUNK_H, z % _CHUNK_D, 0xF);
            }

        }
    }

    for (int z = 0; z < chunks->d * _CHUNK_D; z++) {
        for (int x = 0; x < chunks->w * _CHUNK_W; x++) {
            for (int y = chunks->h * _CHUNK_H - 1; y >= 0; y--) {
                voxel* vox = chunks->Get(x, y, z);
                if (vox->id != 0) {
                    break;
                }
                if (chunks->getLight(x - 1, y, z, 3) == 0 ||
                    chunks->getLight(x + 1, y, z, 3) == 0 ||
                    chunks->getLight(x, y - 1, z, 3) == 0 ||
                    chunks->getLight(x, y + 1, z, 3) == 0 ||
                    chunks->getLight(x, y, z - 1, 3) == 0 ||
                    chunks->getLight(x, y, z + 1, 3) == 0) {
                    SolverS->Add(x, y, z);
                }
                Chunk* chunk = chunks->GetChunkByVoxel(x, y, z);
                if (!chunk) {
                    std::cerr << "Null chunk at voxel (" << x << ", " << y << ", " << z << ")" << std::endl;
                    continue;
                }

                if (!chunk->lightmap) {
                    std::cerr << "Null lightmap at chunk (" << chunk->x << ", " << chunk->y << ", " << chunk->z << ")" << std::endl;
                    continue;
                }

                chunk->lightmap->SetS(x % _CHUNK_W, y % _CHUNK_H, z % _CHUNK_D, 0xF);
            }

        }
    }
    SolverR->solve();
    SolverG->solve();
    SolverB->solve();
    SolverS->solve();
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
            unsigned char* buffer = new unsigned char[chunkVolume * _CHUNK_SIZE];
            chunks->write(buffer);
            write_to_file("C:\\Users\\popka\\source\\repos\\OpenGlGame2\\save.bin", (const char*) buffer, chunkVolume * _CHUNK_SIZE);
            cout << "Saved in: " << (chunkVolume * _CHUNK_SIZE ) << endl;

            delete[] buffer;
        }
        if (Event::isKeyPressed(GLFW_KEY_F2))
        {
            unsigned char* buffer = new unsigned char[chunkVolume * _CHUNK_SIZE];
            chunks->read(buffer);
            read_from_file("C:\\Users\\popka\\source\\repos\\OpenGlGame2\\save.bin", (char*)buffer, chunkVolume * _CHUNK_SIZE);

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

                    SolverR->remove(x, y, z);
                    SolverG->remove(x, y, z);
                    SolverB->remove(x, y, z);
                    SolverS->remove(x, y, z);

                    SolverR->solve();
                    SolverG->solve();
                    SolverB->solve();
                    if (chunks->getLight(x, y + 1, z, 3) == 0xF)
                    {
                        for (int i = y - 1; i >= 0; i--)
                        {
                            if (chunks->Get(x, i, z)->id != 0)
                                break;
                            SolverS->Add(x, i, z, 0xF);
                        }
                     }
                    SolverR->Add(x, y + 1, z); SolverG->Add(x, y + 1, z); SolverB->Add(x, y + 1, z); SolverS->Add(x, y + 1, z);
                    SolverR->Add(x, y - 1, z); SolverG->Add(x, y - 1, z); SolverB->Add(x, y - 1, z); SolverS->Add(x, y - 1, z);
                    SolverR->Add(x + 1, y, z); SolverG->Add(x + 1, y, z); SolverB->Add(x + 1, y, z); SolverS->Add(x + 1, y, z);
                    SolverR->Add(x - 1, y, z); SolverG->Add(x - 1, y, z); SolverB->Add(x - 1, y, z); SolverS->Add(x - 1, y, z);
                    SolverR->Add(x, y, z + 1); SolverG->Add(x, y, z + 1); SolverB->Add(x, y, z + 1); SolverS->Add(x, y, z + 1);
                    SolverR->Add(x, y, z - 1); SolverG->Add(x, y, z - 1); SolverB->Add(x, y, z - 1); SolverS->Add(x, y, z - 1);

                    SolverR->solve();
                    SolverG->solve();
                    SolverB->solve();
                    SolverS->solve();
                }
                if (Event::justClicked(GLFW_MOUSE_BUTTON_2))
                {
                    int x = (int)(iend.x) + (int) (norm.x);
                    int y = (int)(iend.y) + (int)(norm.y);
                    int z = (int)(iend.z) + (int)(norm.z);
                    chunks->Set(x, y, z, Choosen_block);
                    SolverR->Add(x, y, z);
                    SolverG->Add(x, y, z);
                    SolverB->Add(x, y, z);
                    SolverS->Add(x, y, z);
                    for (int i = y - 1; i >= 0; i--)
                    {
                        SolverS->remove(x, i, z);
                        if(i ==0 || chunks->Get(x, i, z)->id != 0)
							break;
                    }
                    SolverR->solve();
                    SolverG->solve();
                    SolverB->solve();
                    SolverS->solve();
                    if (Choosen_block == 3)
                    {
                        SolverR->Add(x, y, z, 10);
                        SolverG->Add(x, y, z, 10);
                        SolverB->Add(x, y, z, 0);
                        SolverR->solve();
                        SolverG->solve();
                        SolverB->solve();
                    }
                    
                }
            }
        }
        Chunk* closes[27];
        for (size_t i = 0; i < chunkVolume; i++) {
            Chunk* chunk = chunks->chunks[i];
            if (!chunk->modifier)
                continue;
            chunk->modifier = false;
            if (meshes[i] != nullptr)
                delete meshes[i];
            for (int j = 0; j < 27; j++)
                closes[j] = nullptr;
            for (size_t j = 0; j < chunkVolume; j++) {
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
        mat4 model;
        for (size_t i = 0; i < chunkVolume; i++)
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
    delete SolverR;
    delete SolverG;
    delete SolverB;
    delete SolverS;

    delete shader;
    delete texture;
    delete CrossShairshader;
    delete CrossShair;
    delete Lineshader;
    delete chunks;
    delete branch;
    Window::Terminate();
}

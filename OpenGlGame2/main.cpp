#include "lib.h"
#include "Window.h"
#include "Camera.h"
#include "Event.h"
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
    Texture* texture = load_texture("C:\\Users\\popka\\Downloads\\857b4fb07782360778fceb455a39fff2.jpg");
    if (texture == nullptr)
    {
        std::cerr << "Failed to load texture" << std::endl;
        delete shader;
        Window::Terminate();
        return 0;
    }

    // Set up vertex data and buffers
    GLuint VAO, VBO;
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);

    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), (GLvoid*)(0 * sizeof(GLfloat)));
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), (GLvoid*)(3 * sizeof(GLfloat)));
    glEnableVertexAttribArray(1);

    glBindVertexArray(0);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    Camera* camera = new Camera(vec3(0,0,1), radians(90.0f));

    mat4 model(1.0f);

    model = translate(model, vec3(0.5, 0, 0));
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
            camera->getPosition() += camera->getFont() * 1.6f * delta;
        }
        if(Event::isKeyPressed(GLFW_KEY_S))
		{
			camera->getPosition() -=  camera->getFont() * 1.6f * delta;
		}
        if (Event::isKeyPressed(GLFW_KEY_A))
        {
            camera->getPosition() -= camera->getRight() * 1.6f * delta;
        }
        if (Event::isKeyPressed(GLFW_KEY_D))
        {
            camera->getPosition() += camera->getRight() * 1.6f * delta;
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
        glClear(GL_COLOR_BUFFER_BIT);
        shader->use();
        shader->uniform_mat4("model", model);
        shader->uniform_mat4("projview", camera->getProjection() * camera->getView());
        texture->bind();
        glBindVertexArray(VAO);
        glDrawArrays(GL_TRIANGLES, 0, 6); // змінено з 3 на 6
        glBindVertexArray(0);
        Window::swapBuffers();
        Event::PullEvents();
    }

    delete shader;
    delete texture;
    glDeleteBuffers(1, &VBO);
    glDeleteVertexArrays(1, &VAO);
    Window::Terminate();

}

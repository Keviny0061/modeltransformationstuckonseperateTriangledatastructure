#include <GL/glew.h>
#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>

#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <vector>

struct Vertex {
    float x, y, z;
};

using namespace std;
void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void processInput(GLFWwindow* window);

// settings
const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 600;


const char* vertexShaderSource = "#version 330 core\n"
"layout (location = 0) in vec3 aPos;\n"
"layout (location = 1) in vec4 aColor;\n"
"out vec4 vertexColor;\n"
"void main()\n"
"{\n"
"   gl_Position = vec4(aPos, 1.0);\n"
"   vertexColor = aColor;\n"
"}\0";

const char* fragmentShaderSource = "#version 330 core\n"
"out vec4 FragColor;\n"
"in vec4 vertexColor;\n"
"void main()\n"
"{\n"
"   FragColor = vertexColor;\n"
"}\n\0";


int main()
{
    std::string filePath = "source.vs";
    std::ifstream fileStream(filePath);
    std::string fileContent;

    if (fileStream) {
        // Use stringstream to read the file's contents into a string
        std::stringstream buffer;
        buffer << fileStream.rdbuf();
        fileContent = buffer.str();

        // Close the file after reading
        fileStream.close();
    }
    else {
        std::cerr << "Could not open file: " << filePath << std::endl;
        return -1;
    }

    // Convert the read content into a const char*
    const char* vertexShaderSource = fileContent.c_str();

    std::string filePath2 = "source.fs";
    std::ifstream fileStream2(filePath2);
    std::string fileContent2;

    if (fileStream) {
        // Use stringstream to read the file's contents into a string
        std::stringstream buffer;
        buffer << fileStream2.rdbuf();
        fileContent2 = buffer.str();

        // Close the file after reading
        fileStream2.close();
    }
    else {
        std::cerr << "Could not open file: " << filePath2 << std::endl;
        return -1;
    }

    // Convert the read content into a const char*
    const char* fragmentShaderSource = fileContent2.c_str();

    //below is seperate triangle data structure
    /*
    std::string filePath4 = "cube.obj";
    std::ifstream fileStream4(filePath4);

    std::vector<Vertex> vertices; // Store vertices read from the file
    std::vector<int> faceIndices; // Store face indices, assuming triangles
    std::vector<Vertex> separateTriangles; // Final structure for OpenGL

    if (!fileStream4.is_open()) {
        std::cerr << "Failed to open file: " << filePath4 << std::endl;
        return -1;
    }

    std::string line;
    while (std::getline(fileStream4, line)) {
        std::istringstream lineStream(line);
        std::string lineType;
        lineStream >> lineType;

        if (lineType == "v") {
            // Read vertex
            Vertex vertex;
            lineStream >> vertex.x >> vertex.y >> vertex.z;
            vertices.push_back(vertex);
        }
        else if (lineType == "f") {
            // Read face indices
            int index;
            for (int i = 0; i < 3; ++i) { // Assuming all faces are triangles
                lineStream >> index;
                faceIndices.push_back(index - 1); // Convert to 0-indexed
            }
        }
    }

    // Close the file
    fileStream4.close();

    // Generate separate triangles data structure
    for (int index : faceIndices) {
        separateTriangles.push_back(vertices[index]);
    }

    // Example: print out the vertices of the separate triangles
    for (const Vertex& vertex : separateTriangles) {
        std::cout << "v " << vertex.x << " " << vertex.y << " " << vertex.z << std::endl;
    }
    */


    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

#ifdef __APPLE__
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif

    GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "viewGL", NULL, NULL);
    if (window == NULL)
    {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

    glewInit();

    unsigned int vertexShader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
    glCompileShader(vertexShader);
    int success;
    char infoLog[512];
    glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
    if (!success)
    {
        glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << std::endl;
    }

    unsigned int fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
    glCompileShader(fragmentShader);
    glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
    if (!success)
    {
        glGetShaderInfoLog(fragmentShader, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n" << infoLog << std::endl;
    }

    unsigned int shaderProgram = glCreateProgram();
    glAttachShader(shaderProgram, vertexShader);
    glAttachShader(shaderProgram, fragmentShader);
    glLinkProgram(shaderProgram);
    glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
    if (!success) {
        glGetProgramInfoLog(shaderProgram, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n" << infoLog << std::endl;
    }
    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);

    string line;
    string v, valuesX[8], valuesY[8], valuesZ[8];
    int n = 0;

    ifstream myfile("cube.obj");
    if (!myfile.is_open()) {
        cout << "Unable to open file";
        return 1;
    }

    while (getline(myfile, line)) {
        if (line[0] == 'v' && line[1] != 'n') {
            istringstream iss(line);
            char v;
            double x, y, z;
            if (iss >> v >> x >> y >> z) {
                valuesX[n] = to_string(x);
                valuesY[n] = to_string(y);
                valuesZ[n] = to_string(z);
                cout << valuesX[n] << "\t" << valuesY[n] << "\t" << valuesZ[n] << endl;
                n++;
                if (n > 8) {
                    cout << "Array size exceeded." << endl;
                    break;
                }
            }
            else {
                cout << "Failed to parse line: " << line << endl;
            }
        }
    }

    myfile.close();
    
    float vertices[] = {
        // Position         // Color
      -0.5f, -0.25f, 0.0f,  1.0f, 0.0f, 0.0f, 1.0f, // left  - Red
         0.5f, -0.75f, 0.0f,  0.0f, 1.0f, 0.0f, 1.0f, // right - Green
         0.0f,  0.5f, 0.0f,   0.0f, 0.0f, 1.0f, 1.0f, // top   - Blue

        -0.5f, -0.5f, 0.0f,   1.0f, 1.0f, 0.0f, 1.0f, // left  - Yellow
         0.5f, -0.5f, 0.0f,   0.0f, 1.0f, 1.0f, 1.0f, // right - Cyan
         0.0f, -1.0f, 0.0f,   1.0f, 0.0f, 1.0f, 1.0f,  // bottom- Magenta

         - 1.000000f, -1.000000f, 1.000000f,1.0f, 0.0f, 0.0f, 1.0f,
1.000000f, 1.000000f, 1.000000f, 0.0f, 1.0f, 0.0f, 1.0f,
1.000000f, -1.000000f, 1.00000f,  0.0f, 0.0f, 1.0f, 1.0f
    };
    /*float vertices[] = {

    }*/

    unsigned int VBO, VAO;
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    //glBufferData(GL_ARRAY_BUFFER, sizeof(Vertex)* vertices.size(), vertices.data(), GL_STATIC_DRAW);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    // Position attribute
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 7 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    // Color attribute
    glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, 7 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glColor3f(1.0f, 1.0f, 1.0f); // Set wireframe color
    // Disable wireframe rendering
    

    while (!glfwWindowShouldClose(window))
    {
        glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
        processInput(window);

        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        glUseProgram(shaderProgram);
        glBindVertexArray(VAO);
        glDrawArrays(GL_TRIANGLES, 0, 9); //this 9 says how many vertices

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    glDeleteProgram(shaderProgram);

    glfwTerminate();
    return 0;
}

void processInput(GLFWwindow* window)
{
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);
}

void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    glViewport(0, 0, width, height);
}

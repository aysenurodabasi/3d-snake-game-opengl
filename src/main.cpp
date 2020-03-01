#include <iostream>
#include <thread>         // std::this_thread::sleep_for
#include <chrono>         // std::chrono::seconds
#include<glad/glad.h>
#include<glm/vec3.hpp>
#include<glm/glm.hpp>
#include<GLFW/glfw3.h>
#include<vector>
#include<cmath>
#include<glm/gtc/matrix_transform.hpp>
#include "shaderprogram.hpp"
#include "Cube.hpp"
#include "texturemanager.hpp"

std::vector<Vertex>      vertices;
std::vector<unsigned int>   indices;
std::vector<Cube*>    snakeList;


glm::vec3 position;
int scale;
float length=0.25;
int randmo;
 std::map<std::string,unsigned int>  textureArray;

unsigned int VBO;
unsigned int VAO;
unsigned int EBO;  
unsigned int texture;


void moveSnake()
{
    for(auto next:snakeList)
    {
        next->move();
    }
    for(int i=snakeList.size()-1;i>0;i--)
    {
        snakeList[i]->setDirection(snakeList[i-1]->getDirection());
    }
}
 
void drawSnake(ShaderProgram& program)
{
    for(auto next:snakeList)
    {
        // program.setVec4("uColor",next->getColor());
        // program.setMat3("uMtxTransform",&rot);
         float angle=0.0f;
        glm::mat4 rot = next->getTransform();
        program.setMat4("mtxTransform",&rot);
        glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, 0);
    }
}


void addToSnakeTail()
{
    int elementCount = snakeList.size();
    if(elementCount==0)
    {
        snakeList.push_back(new Cube(0.0f,0.0f,length,vertices,indices));
    }
    else
    {
        Cube* lastCube = snakeList[elementCount-1];
        glm::vec3 pos = lastCube->getPosition();
        Cube::DIRECTION dir = lastCube->getDirection();
        switch(dir)
        {
            case Cube::DIR_RIGHT:
                pos-=glm::vec3(length,0.0f,0.0f);
            break;
            case  Cube::DIR_LEFT:
                pos+=glm::vec3(length,0.0f,0.0f);
                break;
            case  Cube::DIR_UP:
                pos-=glm::vec3(0.0f,length,0.0f);
                break;    
            case  Cube::DIR_DOWN:
                pos+=glm::vec3(0.0f,length,0.0f);
                break;              
        }
    
        Cube* newCube = new Cube(pos.x,pos.y,length,vertices,indices);
        newCube->setDirection(dir);
        snakeList.push_back(newCube);
    }
}


void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
  if (key == GLFW_KEY_ESCAPE)
        glfwTerminate();   
    if(action==GLFW_PRESS)
    {
        if(snakeList.size()!=0)
        {
            Cube* first = snakeList[0];

            if(key==GLFW_KEY_LEFT)  
            {
                first->setDirection(Cube::DIR_LEFT);
            }
            if(key==GLFW_KEY_RIGHT) 
            {
                first->setDirection(Cube::DIR_RIGHT);
            }    
            if(key==GLFW_KEY_UP)  
            {
                first->setDirection(Cube::DIR_UP);
            }
            if(key==GLFW_KEY_DOWN) 
            {
                first->setDirection(Cube::DIR_DOWN);
            }  

            if(key==GLFW_KEY_Q)
            {
                scale-=2.0;
            }    
            
            if(key==GLFW_KEY_W)
            {
                scale+=2.0;
            } 
        }
        
        if(key==GLFW_KEY_SPACE)
            addToSnakeTail();
    }                    
}

int main(int argc,char** argv)
{   if(!glfwInit())
        return -1;
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    
    GLFWwindow* window = glfwCreateWindow(800,800,"Snake",NULL,NULL);

    if(window==NULL)
    {   std::cout<<"Pencere Olusturulamadi"<<std::endl;
        glfwTerminate();
        return -1;
    }    
    glfwSetKeyCallback(window, key_callback);
    glfwMakeContextCurrent(window);
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return -1;
    } 
    
    // texture = TextureManager::getInstance()->loadTexture("./images/img.jpg");
     std::string textureNames[]={"./images/desiign.jpg","./images/img.jpg","./images/snake.jpg","./images/texture.jpg"};
     TextureManager* textureManager = TextureManager::getInstance(); 
    
    scale = 1.0f;
    ShaderProgram program;

    addToSnakeTail();
    addToSnakeTail();
    addToSnakeTail();
    addToSnakeTail();
    addToSnakeTail();
    addToSnakeTail();

  
    program.attachShader("./shaders/simplevs.glsl",GL_VERTEX_SHADER);
    program.attachShader("./shaders/simplefs.glsl",GL_FRAGMENT_SHADER);
    program.link();
    program.addUniform("MtxTransform");
   
    glGenVertexArrays(1, &VAO); 
    glGenBuffers(1,&VBO);
    glGenBuffers(1, &EBO);
    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER,VBO);
    glBufferData(GL_ARRAY_BUFFER,sizeof(Vertex)*vertices.size(),&vertices[0],GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);  
    glEnableVertexAttribArray(0); 
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3*sizeof(float)));
    glEnableVertexAttribArray(1);    
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(unsigned int)*indices.size(), &indices[0], GL_STATIC_DRAW); 
   
   
    textureManager->loadTexture(textureNames[0]); 
    textureManager->loadTexture(textureNames[1]);
    textureManager->loadTexture(textureNames[2]);
    textureManager->loadTexture(textureNames[3]);
    

    glEnable(GL_DEPTH_TEST);
    while(!glfwWindowShouldClose(window))
    {
        //oluşturulacak resim başlangıç rengine boyanıyor
        glClearColor(0.0f, 0.4f, 0.7f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
        
        // angle+=1.0f;
        //çizimde kullanılacak olan program nesnesi aktif ediliyor
        program.use();
       
        randmo = std::rand()%4;
        TextureManager::getInstance()->activateTexture(textureNames[randmo]);
        
        glBindVertexArray(VAO);
        
        
        drawSnake(program);
        
        moveSnake();

        // program.setVec4("uColor",glm::vec4(1.0f,0.0f,0.0f,1.0f));
        // program.setMat4("MtxTransform",&mtxTransform);
        
        //daire index buffer kullanılarak kopyalanıyor.
        glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, 0);
        
        std::this_thread::sleep_for (std::chrono::milliseconds(200));

        glfwSwapBuffers(window);

        glfwPollEvents();
    }

}
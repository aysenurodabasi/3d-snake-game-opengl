#include "Cube.hpp"
#include "shaderprogram.hpp"
#include<cstdlib>
#include<glm/glm.hpp>
#include<GLFW/glfw3.h>
#include<cmath>
#include <fstream>
#include <iostream>


float angle;
Cube::Cube(float x,float y,float length,std::vector<Vertex> &vertices,std::vector<unsigned int>  &indices)
{
    m_Position  = glm::vec3(x,y,0.0f);

    // m_Color.r   = (10+std::rand()%15)/25.0;
    // m_Color.g   = (10+std::rand()%15)/25.0;  
    m_Length    = length;
    m_Direction = DIR_RIGHT;  
    
    

     glm::vec3 v[8];
    v[0] = glm::vec3(-length/2,-length/2,-length/2);
    v[1] = glm::vec3(length/2,-length/2,-length/2);
    v[2] = glm::vec3(length/2,-length/2,length/2);
    v[3] = glm::vec3(-length/2,-length/2,length/2);

    v[4] = glm::vec3(-length/2,length/2,-length/2);
    v[5] = glm::vec3(length/2,length/2,-length/2);
    v[6] = glm::vec3(length/2,length/2,length/2);
    v[7] = glm::vec3(-length/2,length/2,length/2);
    //Ust Yuzey

    Vertex tempVertices[24];
    tempVertices[0].pos = v[7];
    tempVertices[1].pos = v[3];
    tempVertices[2].pos = v[2];
    tempVertices[3].pos = v[6];
    tempVertices[0].texCoord = glm::vec2(0.0f,1.0f);
    tempVertices[1].texCoord = glm::vec2(0.0f,0.0f);
    tempVertices[2].texCoord = glm::vec2(1.0f,0.0f);
    tempVertices[3].texCoord = glm::vec2(1.0f,1.0f);
    //Sag Yuzey
    tempVertices[4].pos = v[6];
    tempVertices[5].pos = v[2];
    tempVertices[6].pos = v[1];
    tempVertices[7].pos = v[5];
    tempVertices[4].texCoord = glm::vec2(0.0f,1.0f);
    tempVertices[5].texCoord = glm::vec2(0.0f,0.0f);
    tempVertices[6].texCoord = glm::vec2(1.0f,0.0f);
    tempVertices[7].texCoord = glm::vec2(1.0f,1.0f);
//    //Ust Yuzey
    tempVertices[8 ].pos = v[4];
    tempVertices[9 ].pos = v[7];
    tempVertices[10].pos = v[6];
    tempVertices[11].pos = v[5];
    tempVertices[8 ].texCoord = glm::vec2(0.0f,1.0f);
    tempVertices[9 ].texCoord = glm::vec2(0.0f,0.0f);
    tempVertices[10].texCoord = glm::vec2(1.0f,0.0f);
    tempVertices[11].texCoord = glm::vec2(1.0f,1.0f);

    //Sol Yüzey
    tempVertices[12].pos = v[0];
    tempVertices[13].pos = v[3];
    tempVertices[14].pos = v[2];
    tempVertices[15].pos = v[1];
    tempVertices[12].texCoord = glm::vec2(0.0f,1.0f);
    tempVertices[13].texCoord = glm::vec2(0.0f,0.0f);
    tempVertices[14].texCoord = glm::vec2(1.0f,0.0f);
    tempVertices[15].texCoord = glm::vec2(1.0f,1.0f);
    //Sag Yuzey
    tempVertices[16].pos = v[4];
    tempVertices[17].pos = v[0];
    tempVertices[18].pos = v[1];
    tempVertices[19].pos = v[5];
    tempVertices[16].texCoord = glm::vec2(0.0f,1.0f);
    tempVertices[17].texCoord = glm::vec2(0.0f,0.0f);
    tempVertices[18].texCoord = glm::vec2(1.0f,0.0f);
    tempVertices[19].texCoord = glm::vec2(1.0f,1.0f);
//    //Alt Yuzey
    tempVertices[20].pos = v[7];
    tempVertices[21].pos = v[3];
    tempVertices[22].pos = v[0];
    tempVertices[23].pos = v[4];
    tempVertices[20].texCoord = glm::vec2(0.0f,1.0f);
    tempVertices[21].texCoord = glm::vec2(0.0f,0.0f);
    tempVertices[22].texCoord = glm::vec2(1.0f,0.0f);
    tempVertices[23].texCoord = glm::vec2(1.0f,1.0f);


    for(int i=0;i<24;i++)
        vertices.push_back(tempVertices[i]);

    for(int i=0;i<6;i++)
    {
        int startIndex = 4*i;
        indices.push_back(startIndex);
        indices.push_back(startIndex+1);
        indices.push_back(startIndex+2);
        
        indices.push_back(startIndex);
        indices.push_back(startIndex+2);
        indices.push_back(startIndex+3);
    }
}


glm::vec3 Cube::getPosition()
{
    return m_Position;
}


Cube::DIRECTION Cube::getDirection()
{
    return m_Direction;
}
void Cube::setDirection(Cube::DIRECTION dir)
{
    m_Direction = dir;
}




glm::mat4 Cube::getTransform()
 {
    
    glm::vec3 camPosition(0.0f,0.0f,2.0f);
    glm::vec3 camLookAt(0.0f,0.0f,0.0f);
    glm::vec3 camUp(0.0f,1.0f,0.0f);

    glm::mat4 mtxCam = glm::lookAt(camPosition,camLookAt,camUp);
    glm::mat4 mtxProj = glm::perspective(glm::radians(90.0f),(800.0f/800.0f),1.0f,100.0f);
    
    glm::mat4 txTransform(1);
    glm::mat4 txTranslate = glm::translate(glm::mat4(1),m_Position);
    
    glm::mat4 mtxTransform(1); 
    glm::mat4 mtxRotation = glm::rotate(glm::mat4(1),glm::radians(angle),m_Position);
    mtxTransform = mtxProj*mtxCam*txTranslate*mtxRotation;
    angle+=1.0f;
    return mtxTransform;
 }



void Cube::move()
{
    switch(m_Direction)
    {
        case DIR_LEFT:
            m_Position-= glm::vec3(m_Length,0.0f,0.0f);
        break;
        case DIR_RIGHT:
            m_Position+= glm::vec3(m_Length,0.0f,0.0f);
        break;
        case DIR_UP:
            m_Position+= glm::vec3(0.0f,m_Length,0.0f);
        break;
        case DIR_DOWN:
            m_Position-= glm::vec3(0.0f,m_Length,0.0f);
        break;                        
    }
}
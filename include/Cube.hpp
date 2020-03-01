#ifndef QUBE_HPP
#define  QUBE_HPP
#include<glm/vec3.hpp>
#include<glm/vec2.hpp>
#include<glm/mat3x3.hpp>
#include<cmath>
#include<glm/gtx/matrix_transform_2d.hpp>
#include<glm/gtc/matrix_transform.hpp>
#include<vector>


    struct Vertex
    {
        glm::vec3 pos;
        glm::vec2 texCoord;
    };


class Cube
{
public:

    enum DIRECTION
    {
        DIR_RIGHT   =  1,
        DIR_LEFT    = -1,
        DIR_UP      =  2,
        DIR_DOWN    = -2

    };


    Cube(float  x,float y,float length, std::vector<Vertex> &vertices,std::vector<unsigned int>  &indices);

    glm::vec3 getPosition();
    // glm::vec4 getColor();
    glm::mat4 getTransform();
    DIRECTION getDirection();
 
    void setDirection(DIRECTION dir);

    void move();                

    void Trotate(glm::mat4& mtxTransform);
private:

    // glm::vec4   m_Color;
    glm::vec3   m_Position;
    float       m_Length;

    DIRECTION   m_Direction;
};


#endif
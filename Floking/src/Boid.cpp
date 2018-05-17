#include "Boid.h"
#include <ngl/VAOPrimitives.h>
#include <math.h>

///Boid constructor

Boid::Boid( ngl::Vec3 _pos, ngl::Vec3 _dir, float _velocity, GLfloat _rad, GLfloat _separateCoef, GLfloat _alignCoef, GLfloat _seekCoef)
{
    // set values from params
    m_pos=_pos;
    m_dir=_dir;
    m_velocity=_velocity;
    m_radius=_rad;
//    m_colour=_colour;
    m_colour=(0.5f,0.1f,0.8f, 1.0f);

    //setting the behavious coeficients
    m_separateCoef=_separateCoef;
    m_alignCoef=_alignCoef;
    m_seekCoef=_seekCoef;


    m_RSeparate =_rad*m_separateCoef;
    m_RAlign = _rad*m_alignCoef;
    m_RSeek = _rad*m_seekCoef;

    m_hit=false;

    _dir.normalize();
    RotateBoid();

}

Boid::Boid()
{
    m_hit=false;
}

/// draw method ///


void Boid::loadMatricesToShader( ngl::Transformation &_tx, const ngl::Mat4 &_globalMat, ngl::Camera *_cam  ) const
{
    ngl::ShaderLib *shader=ngl::ShaderLib::instance();

    ngl::Mat4 MV;
    ngl::Mat4 MVP;
    ngl::Mat3 normalMatrix;
    MV=_cam->getViewMatrix()  *_globalMat* _tx.getMatrix();
    MVP=_cam->getProjectionMatrix()*MV;
    normalMatrix=MV;
    normalMatrix.inverse().transpose();
    shader->setUniform("MVP",MVP);
    shader->setUniform("normalMatrix",normalMatrix);
}


void Boid::draw( const std::string &_shaderName, const ngl::Mat4 &_globalMat,  ngl::Camera *_cam )const
{
    ngl::ShaderLib *shader=ngl::ShaderLib::instance();
    shader->use(_shaderName);


    shader->setUniform("Colour",0.4f,0.1f,0.6f, 1.0f);


    // grab an instance of the primitives for drawing
    ngl::VAOPrimitives *prim=ngl::VAOPrimitives::instance();
    ngl::Transformation t;

    //    shader->setUniform();
    t.reset();
    t.setPosition(m_pos);
    t.setScale(m_radius,m_radius,m_radius);
    t.setRotation(m_rotation);

    loadMatricesToShader(t,_globalMat,_cam);
    prim->draw("cone");

    glPolygonMode(GL_FRONT_AND_BACK,GL_FILL);
}

/// movement methods  ///

void Boid::move()
{
    // store the last position
    m_lastPos=m_pos;
    // update the current position
    if (m_dir.length()<=1)
    {
        m_dir*=2;
    }
    m_pos+=m_dir*m_velocity;
    // get the next position
    m_nextPos=(m_pos+m_dir);
    m_hit=false;
}

void Boid::Steer(ngl::Vec3 desiredDir)
{
    /* This function assignes the new direction to the boid and calls the
rotate function*/
    //    desiredDir+=m_dir;
    desiredDir.normalize();
    m_dir=desiredDir;
    RotateBoid();
}


void Boid::RotateBoid()
{
    /* This function rotates the boid depending on the boids direction*/
    double pitch, yaw;
    pitch = -atan(m_dir.m_y/sqrt(m_dir.m_x*m_dir.m_x + m_dir.m_z*m_dir.m_z))*57.2958;

    // Yaw
    yaw = atan2(m_dir.m_x, m_dir.m_z)*57.2958;
    m_rotation.set(pitch, yaw, 0.0f);


}

/// behavior methods ///

void Boid :: setBehavor(GLfloat _seekCoef, GLfloat _alignCoef, GLfloat _separateCoef )
{
    /* This function sets the new values for the befavior coeficients
and then based on this new values it creates new values for the radius of
influence for each behavior

This function is called from the UI each time one of the values UI for the  is changed


*/

    m_seekCoef=_seekCoef;
    m_alignCoef=_alignCoef;
    m_separateCoef=_separateCoef;

    ///reseting the radius for each behavior
    m_RAlign=m_radius*m_alignCoef;
    m_RSeek =m_radius*m_seekCoef;
    m_RSeparate=m_radius*m_separateCoef;
    std::cout<<"Behavior \n"<<m_RAlign;


}
ngl::Vec3 Boid::TargetBoidDirection(ngl::Vec3 m_TargetPos)
{
    /* This function returns a vec3 that holds the new direction between the boid and a
target. After it calculated the direction, this vector has to be normalised to not
increase the speed of the boid. */

    ngl::Vec3 newDir= m_TargetPos-m_pos;
    newDir.normalize();
    return newDir;

}
double Boid::TargetBoidDistance(ngl::Vec3 m_TargetPos)
{
    /* This function returns a double variable that holds the magnitude of the
direction vector, from the boid to the target. */
    ngl::Vec3 newDir= m_TargetPos-m_pos;
    double distance = newDir.length();
    return distance;

}
void Boid::seekTarget(ngl::Vec3 m_TargetPos)
{
    /* The SEEEK
This function calculates the direction between the boid and the target
and the distance between the two.
Then if the target is within the seeking area of influence ( distance <= seek Rad),
the STEER function will be called, that assignes a new direction and
orientation to the boid
*/
    ngl::Vec3 newDir=TargetBoidDirection(m_TargetPos);
    double distance=TargetBoidDistance(m_TargetPos);
    //checking if th public Behaviore boid is in the radius of the target
    if (distance <= m_RSeek)
    {
        Steer(newDir);//2 represents the speed
    }
}

void Boid::Separate(ngl::Vec3 m_TargetPos)
{
    /* The SEPARATE
This function calculates the direction between the boid and the target
and the distance between the two.
Then if the target is within the separation area of influence ( distance <= seek Rad),
the STEER function will be called, that assignes a new direction and
orientation to the boid
*/

    ngl::Vec3 newDir=TargetBoidDirection(m_TargetPos-m_RSeparate*m_TargetPos);
    double distance=TargetBoidDistance(m_TargetPos);
    if (distance <= m_RSeparate)
    {
        Steer( newDir*(-1));
    }
}

ngl::Vec3 Boid::align(ngl::Vec3 direction, uint numBoid)
{
    /* The ALIGNMENT
This function gets the direction variable, which is the sum of the target and boid
direction. It will return the averages direction of the two objects
*/

    //averaging the direction
    ngl::Vec3 AVGirection = direction/numBoid;
    return AVGirection;
}



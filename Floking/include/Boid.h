#ifndef Boid_H_
#define Boid_H_


#include <ngl/Camera.h>
#include <ngl/ShaderLib.h>
#include <ngl/Transformation.h>
#include <ngl/Vec3.h>
#include <ngl/Obj.h>


/*! \brief a simple Boid class */
class Boid
{
public :
    ///Boid constructor
    Boid(ngl::Vec3 _pos,  ngl::Vec3 _dir,float _velocity, GLfloat _rad,  GLfloat _separateCoef, GLfloat _alignCoef, GLfloat _seekCoef);
    Boid();
    /// draw method ///
    void draw(const std::string &_shaderName,const ngl::Mat4 &_globalMat, ngl::Camera *_cam )const ;
    void loadMatricesToShader(ngl::Transformation &_tx, const ngl::Mat4 &_globalMat,ngl::Camera *_cam )const;
    inline void setColour (ngl::Vec3 _colour){m_colour=_colour;}

    /// movement methods  ///
    inline void reverse(){m_dir=m_dir*-1.0;}
    void Steer(ngl::Vec3 desiredDir);
    void move();
    void RotateBoid();

    /// behavior methods ///
    double TargetBoidDistance(ngl::Vec3 m_TargetPos);
    ngl::Vec3 TargetBoidDirection(ngl::Vec3 m_TargetPos);
    void seekTarget(ngl::Vec3 m_TargetPos);
    void Separate(ngl::Vec3 m_TargetPos);
    ngl::Vec3 align(ngl::Vec3 direction, uint numBoid);

    ///collision methods ///
    inline void setHit(){m_hit=true;}
    inline void setNotHit(){m_hit=false;}
    inline bool isHit()const {return m_hit;}

    ///set parameters methods ///
    inline void setRadius(GLfloat _radius){m_radius=_radius;}
    inline void setRadSeparate(GLfloat _radius){m_RSeparate=_radius;}
    inline void setRadAlign(GLfloat _radius){m_RAlign=_radius;}
    inline void setRadSeek(GLfloat _radius){m_RSeek=_radius;}
    ////////  functions to set behavior param   ////////
    void setBehavor(GLfloat _seekCoef, GLfloat _alignCoef, GLfloat _separateCoef );
    inline void SetSeparationAmount(GLfloat _separateCoef){m_separateCoef=_separateCoef;}
    inline void SetSeekAmount(GLfloat _seekCoef){m_separateCoef=_seekCoef;}
    inline void SetAlignAmount(GLfloat _alignCoef){m_separateCoef=_alignCoef;}
    inline void SetVelocity(float _velocity){m_velocity=_velocity;}


    ///get parameters methods ///
    inline ngl::Vec3 getPos() const {return m_pos;}
    inline ngl::Vec3 getNextPos() const {return m_nextPos;}
    inline GLfloat getRadius() const {return m_radius;}
    inline float getVelocity() const {return m_velocity;}
    inline GLfloat getRAlign() const {return m_RAlign;}
    inline void setDirection(ngl::Vec3 _d){m_dir=_d;}
    inline ngl::Vec3 getDirection() const { return m_dir;}
    inline ngl::Vec4 getColour (){return m_colour;}


protected :


    /*! the radius of the Boid */
    GLfloat m_radius;
    /*! flag to indicate if the Boid has been hit by ray */
    bool m_hit;
    // the direction of the Boid
    ngl::Vec3 m_dir;
    // Position of Boid
    ngl::Vec3 m_pos;
    // Boid seek/seporation/ Radious
    GLfloat m_RSeek, m_RSeparate, m_RAlign;
    // the last position of the Boid
    ngl::Vec3 m_lastPos;
    // the next position of the Boid
    ngl::Vec3 m_nextPos;
    // Boid Velocity
    float m_velocity = 5.0f;
    std::unique_ptr<ngl::Obj> m_mesh;
    ngl::Vec3 m_rotation;
    ngl::Vec4 m_colour;

    //    /////////////////////   Controlling Behavior amount     /////////////////////
    GLfloat m_separateCoef, m_alignCoef, m_seekCoef;


};


#endif

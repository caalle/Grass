#include "grass.h"
#include "segment.h"

Grass::Grass()
{

    // Random nummer mellan -10 och 10
    float xpos = 20.0*(rand()/float(RAND_MAX)) - 10.0;
    float zpos = 20.0*(rand()/float(RAND_MAX)) - 10.0;

    init(xpos, 0.0f, zpos);
}


Grass::Grass(float x, float y, float z)
{
    init(x, y, z);
}

void Grass::init(float x, float y, float z)
{
    // Startposition och vinkel
    base.x = x;
    base.y = y;
    base.z = z;
    initialAngleXY = 80.0f + 20.0*(rand()/float(RAND_MAX)); // 80-100 degrees
    initialAngleZX = 360.0*(rand()/float(RAND_MAX)); // 0-360 degrees

    // gräsets längd, skapa segment
	float lengthMultiplier = 1.75f + 0.5*(rand()/float(RAND_MAX)); // 0.75-1 * length
    segments[0] = Segment(base, initialAngleXY, initialAngleZX, lengthMultiplier * 0.4f);
    for (int i=1; i < NUM_SEGMENTS; i++)
        segments[i] = Segment(segments[i-1].getPosition(), segments[i-1].getAngleXY(), initialAngleZX, lengthMultiplier * (0.3f/(0.5f*(i+1))));

    // Sätt gräsets "ljushet"
    colorShade = 0.2*rand()/float(RAND_MAX) + 0.65;


}

Grass::~Grass()
{
    delete segments;
}

void Grass::calculate(float windAngle,float windMagnitude, double timestep)
{

        segments[0].calculatePosition(windAngle, windMagnitude, base, initialAngleXY, timestep);
        for (int i=1; i < NUM_SEGMENTS; i++)
            segments[i].calculatePosition(windAngle, windMagnitude, segments[i-1].getPosition(), segments[i-1].getAngleXY(), timestep);

}

void Grass::draw()
{
    glBegin(GL_QUAD_STRIP);

        //Ritar ut första segmentet
        glColor3f(0.4*colorShade, colorShade, 0.3*colorShade);


        float sinVal = BASE_WIDTH * sin(DEG2RAD(initialAngleZX));
        float cosVal = BASE_WIDTH * cos(DEG2RAD(initialAngleZX));

        glTexCoord2f(0.0f, 0.0f);
        glVertex3f(base.x - 0.5*cosVal, base.y, base.z - 0.5*sinVal);
        glTexCoord2f(1.0f, 0.0f);
        glVertex3f(base.x + 0.5*cosVal, base.y, base.z + 0.5*sinVal);


        //Ritar ut resten segmenten
        for (int i=0; i < NUM_SEGMENTS; i++)
        {

            float sinVal2 = 0.5f*sinVal + 0.5f*BASE_WIDTH * cos(DEG2RAD(segments[i].getAngleZX()));
            float cosVal2 = 0.5f*cosVal + 0.5f*BASE_WIDTH * sin(DEG2RAD(segments[i].getAngleZX()));

            Vector3f point = segments[i].getPosition();

            glTexCoord2f(0.0f, (0.98f/NUM_SEGMENTS) * (i+1));
            glVertex3f(point.x - 0.5*cosVal2, point.y, point.z - 0.5*sinVal2);

            glTexCoord2f(1.0f, (0.98f/NUM_SEGMENTS) * (i+1));
            glVertex3f(point.x + 0.5*cosVal2, point.y, point.z + 0.5*sinVal2);
        }


    glEnd();

}



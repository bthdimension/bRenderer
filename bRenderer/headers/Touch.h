#ifndef B_TOUCH_H
#define B_TOUCH_H

#include <map>

/** @brief An object that holds the characteristics of a touch on a touch screen.
*	@author Benjamin Buergisser
*/
struct Touch
{
    GLfloat startPositionX;
    GLfloat startPositionY;
    GLfloat lastPositionX;
    GLfloat lastPositionY;
    GLfloat currentPositionX;
    GLfloat currentPositionY;
};

/* Typedefs */
typedef std::map< int, Touch > TouchMap;

#endif /* defined(B_TOUCH_H) */

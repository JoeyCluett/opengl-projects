#ifndef __JJC__PROJ__TOOL__H__
#define __JJC__PROJ__TOOL__H__

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/transform.hpp>
#include <glm/gtx/intersect.hpp>

namespace Proj_Tools {

glm::vec3 getIntersect(glm::vec3 pos, glm::vec3 dir) {
    float f;
    if(glm::intersectRayPlane(pos, dir, glm::vec3(0, 0, 0), glm::vec3(0, 1, 0), f)) {
        return pos + (dir * f);
    } else if(glm::intersectRayPlane(pos, -dir, glm::vec3(0, 0, 0), glm::vec3(0, 1, 0), f)) {
        return pos + (dir * f);
    }

    return pos;
}

} // namespace Proj_Tools

#endif // __JJC__PROJ__TOOL__H__

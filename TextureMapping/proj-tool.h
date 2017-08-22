#ifndef __JJC__PROJ__TOOL__H__
#define __JJC__PROJ__TOOL__H__

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/transform.hpp>
#include <glm/gtx/intersect.hpp>

namespace Proj_Tools {

GLfloat getIntersect(glm::vec3 pos, glm::vec3 dir, glm::vec3 A, glm::vec3 B, glm::vec3 C) {
    glm::vec3 e1 = B - A;
    glm::vec3 e2 = C - A;

    // calculate the planes normal vector
    glm::vec3 pvec = glm::cross(dir, e2);
    GLfloat det = glm::dot(e1, pvec);

    // ray is parallel to plane
    if(det < 1e-8 && det > -1e-8)
        return 0;

    GLfloat inv_det = 1 / det;
    glm::vec3 tvec = pos - A;
    GLfloat u = glm::dot(tvec, pvec) * inv_det;
    if(u < 0.0 || u > 1.0)
        return 0;

    glm::vec3 qvec = glm::cross(tvec, e1);
    GLfloat v = glm::dot(dir, qvec) * inv_det;
    if(v < 0.0 || u+v > 1)
        return 0;

    return glm::dot(e2, qvec) * inv_det;
}

} // namespace Proj_Tools

#endif // __JJC__PROJ__TOOL__H__

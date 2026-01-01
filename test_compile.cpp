// Simple test to check if our new headers compile
#include<hgl/math/Vector.h>
#include<hgl/math/geometry/AABB.h>
#include<hgl/math/geometry/primitives/GeometryBase.h>
#include<hgl/math/geometry/primitives/Sphere.h>
#include<hgl/math/geometry/primitives/Capsule.h>
#include<hgl/math/geometry/primitives/Cylinder.h>
#include<hgl/math/geometry/primitives/Cone.h>
#include<hgl/math/geometry/primitives/Torus.h>
#include<hgl/math/geometry/queries/CollisionDetector.h>
#include<hgl/math/geometry/queries/RaycastQuery.h>
#include<hgl/math/geometry/queries/DistanceQuery.h>
#include<hgl/math/geometry/queries/ContainmentQuery.h>
#include<hgl/math/Geometry.h>

int main() {
    // Test that our classes can be instantiated
    hgl::math::Sphere sphere(hgl::math::Vector3f(0, 0, 0), 1.0f);
    hgl::math::Capsule capsule;
    hgl::math::Cylinder cylinder;
    hgl::math::Cone cone;
    hgl::math::Torus torus;
    
    // Test basic queries
    bool hit = hgl::math::CollisionDetector::Intersects(sphere, capsule);
    
    return 0;
}

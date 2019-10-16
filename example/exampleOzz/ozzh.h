
#ifndef _OZZH_H_2F87C1877DE740CE73F81CFCF58F03A1_
#define _OZZH_H_2F87C1877DE740CE73F81CFCF58F03A1_

#include <ozz/animation/runtime/animation.h>
#include <ozz/animation/runtime/local_to_model_job.h>
#include <ozz/animation/runtime/sampling_job.h>
#include <ozz/animation/runtime/skeleton.h>

#include <ozz/base/log.h>

#include <ozz/base/maths/box.h>
#include <ozz/base/maths/simd_math.h>
#include <ozz/base/maths/soa_transform.h>
#include <ozz/base/maths/vec_float.h>

#include <ozz/base/containers/vector.h>

#include <ozz/options/options.h>

#include <ozz/base/io/stream.h>
#include <ozz/base/io/archive.h>
#include <ozz/base/io/archive_traits.h>

// Skeleton archive can be specified as an option.
OZZ_OPTIONS_DECLARE_STRING(skeleton,
                           "Path to the skeleton (ozz archive format).",
                           "media/skeleton.ozz", false)

// Animation archive can be specified as an option.
OZZ_OPTIONS_DECLARE_STRING(animation,
                           "Path to the animation (ozz archive format).",
                           "media/animation.ozz", false)

class ozzh
{
public:
    ozzh(){}
    ~ozzh(){}
    
public:
    bool Initialize();
    bool Update(float dt);
    
private:
    ozz::animation::Skeleton m_skeleton;
    ozz::animation::Animation m_animation;
    ozz::animation::SamplingCache m_cache;
    ozz::Vector<ozz::math::SoaTransform>::Std m_locals;
    ozz::Vector<ozz::math::Float4x4>::Std m_models;
};

#endif // _OZZH_H_2F87C1877DE740CE73F81CFCF58F03A1_

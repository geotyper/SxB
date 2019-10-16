
#include "ozzh.h"

bool LoadSkeleton(const char* _filename, ozz::animation::Skeleton* _skeleton) {
    assert(_filename && _skeleton);
    ozz::log::Out() << "Loading skeleton archive " << _filename << "."
    << std::endl;
    ozz::io::File file(_filename, "rb");
    if (!file.opened()) {
        ozz::log::Err() << "Failed to open skeleton file " << _filename << "."
        << std::endl;
        return false;
    }
    ozz::io::IArchive archive(&file);
    if (!archive.TestTag<ozz::animation::Skeleton>()) {
        ozz::log::Err() << "Failed to load skeleton instance from file "
        << _filename << "." << std::endl;
        return false;
    }
    
    // Once the tag is validated, reading cannot fail.
    archive >> *_skeleton;
    
    return true;
}

bool LoadAnimation(const char* _filename,
                   ozz::animation::Animation* _animation) {
    assert(_filename && _animation);
    ozz::log::Out() << "Loading animation archive: " << _filename << "."
    << std::endl;
    ozz::io::File file(_filename, "rb");
    if (!file.opened()) {
        ozz::log::Err() << "Failed to open animation file " << _filename << "."
        << std::endl;
        return false;
    }
    ozz::io::IArchive archive(&file);
    if (!archive.TestTag<ozz::animation::Animation>()) {
        ozz::log::Err() << "Failed to load animation instance from file "
        << _filename << "." << std::endl;
        return false;
    }
    
    // Once the tag is validated, reading cannot fail.
    archive >> *_animation;
    
    return true;
}


bool ozzh::Initialize()
{
    // Reading skeleton.
    if (!LoadSkeleton(OPTIONS_skeleton, &m_skeleton)) {
        return false;
    }
    
    // Reading animation.
    if (!LoadAnimation(OPTIONS_animation, &m_animation)) {
        return false;
    }
    return true;
}

bool ozzh::Update(float dt)
{
    // Samples optimized animation at t = animation_time_.
    ozz::animation::SamplingJob sampling_job;
    sampling_job.animation = &m_animation;
    sampling_job.cache = &m_cache;
    sampling_job.ratio = dt;
    sampling_job.output = make_range(m_locals);
    if (!sampling_job.Run()) {
        return false;
    }
    
    // Converts from local space to model space matrices.
    ozz::animation::LocalToModelJob ltm_job;
    ltm_job.skeleton = &m_skeleton;
    ltm_job.input = make_range(m_locals);
    ltm_job.output = make_range(m_models);
    if (!ltm_job.Run()) {
        return false;
    }
    
    return true;
}

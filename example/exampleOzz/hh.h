
#ifndef _HH_H_A1FCC1D896CEAC3BDD605DF8C8187CDC_
#define _HH_H_A1FCC1D896CEAC3BDD605DF8C8187CDC_

#pragma mark SFML
#include <SFML/Config.hpp>
#include <SFML/Window/WindowBase.hpp>
#include <SFML/Window/Event.hpp>

#pragma mark bgfx
#include <bgfx/bgfx.h>
#include <bgfx/platform.h>
#include <bgfx/examples/common/bgfx_utils.h>

#pragma mark bx
#include <bx/bx.h>
#include <bx/allocator.h>
#include <bx/readerwriter.h>
#include <bx/file.h>
#include <bx/timer.h>
#include <bx/math.h>
#include <bx/commandline.h>
#include <bx/endian.h>
#include <bx/string.h>
#include <bx/macros.h>
#include <bx/uint32_t.h>

#pragma mark bimg
#include <bimg/bimg.h>
#include <bimg/decode.h>

#pragma mark ozz animation
#include <ozz/animation/runtime/animation.h>
#include <ozz/animation/runtime/local_to_model_job.h>
#include <ozz/animation/runtime/sampling_job.h>
#include <ozz/animation/runtime/skeleton.h>

#pragma mark ozz base log
#include <ozz/base/log.h>

#pragma mark ozz base math
#include <ozz/base/maths/box.h>
#include <ozz/base/maths/simd_math.h>
#include <ozz/base/maths/soa_transform.h>
#include <ozz/base/maths/vec_float.h>

#pragma mark ozz base io
#include <ozz/base/io/stream.h>
#include <ozz/base/io/archive.h>
#include <ozz/base/io/archive_traits.h>

#pragma mark ozz base container
#include <ozz/base/containers/vector.h>

#pragma mark ozz options
#include <ozz/options/options.h>

#pragma mark crt
#include <iostream>
#include <stdint.h>  // int32_t
#include <string>

#pragma mark tinystl
#include <tinystl/allocator.h>
#include <tinystl/vector.h>
#include <tinystl/string.h>
namespace stl = tinystl;

#pragma mark sxbCommon
#include <sxbCommon/utils.h>
#include <sxbCommon/Mesh.h>
#include <sxbCommon/Camera.h>
#include <sxbCommon/DataStruct.h>
#include <sxbCommon/Cursor.h>

#endif // _HH_H_A1FCC1D896CEAC3BDD605DF8C8187CDC_

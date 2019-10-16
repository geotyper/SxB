
#include "sxbCommon/utils.h"

SXB_NAMESPACE_BEGIN

std::string Utils::s_runtimeDirectory = "../../../3rd/bgfx/examples/runtime/";

const std::string & Utils::getRuntimeDirectory()
{
    return s_runtimeDirectory;
}

bool Utils::getMem(double &residentMem_, double &virtualMem_)
{
    return false;
}

SXB_NAMESPACE_END

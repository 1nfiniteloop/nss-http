#pragma once

#include <memory>

namespace unix_account
{
template <typename T>
std::unique_ptr<T> create();
} // namespace unix_account
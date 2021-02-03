# wrapped_var
![CMake build](https://github.com/mik90/wrapped_var/workflows/CMake/badge.svg)

Header only mutex variable wrappers like Rust's `std::sync::Mutex` but for C++

### Setup
- Requires C++11
- Requires conan
- Install with conan (not uploaded anywhere yet) or just copy the single header
- Run unit tests:
```sh
git clone git@github.com:mik90/wrapped_var.git
mkdir build && cd build
cmake --build . --target UnitTest
ctest
```

### Usage
Create a `std::string` and protect it underneath a mutex
```cpp
WrappedMutex<std::string> wrapped_string{"hello there"};
```

Create a `std::vector` and protect it underneath a mutex.
```cpp
using vector_type = std::vector<int>;
constexpr vector_type::size_type size = 5;
constexpr vector_type::value_type value = 12;
WrappedMutex<vector_type> wrapped_string(size, value);
```

Modify a mutex-protected `std::string`
```cpp
WrappedMutex<std::string> wrapped_string{"modify me"};
{
  auto string_accessor = wrapped_string.get();
  string_accessor.get_ref() = "you are modified";
}

const auto const_string_accessor = wrapped_string.get();
ASSERT_EQ(const_string_accessor.get_ref(), "you are modified");
```
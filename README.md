# wrapped_var
![CMake build](https://github.com/mik90/wrapped_var/workflows/CMake/badge.svg)

Header only mutex variable wrappers like Rust's `std::sync::Mutex` but for C++

### Requirements
- Requires C++11 or newer
- Header-only so conan isn't strictly required to pull this in as a dependency. You could just pull down the header.  
  I haven't uploaded the recipe to conan-center or anywhere so that's kind of the only option.

### Running unit tests
- Requires conan
```sh
git clone git@github.com:mik90/wrapped_var.git
mkdir build && cd build
cmake --build . --target UnitTest
ctest
```

### Usage
Create a `std::string` and protect it underneath a mutex
```cpp
wrapped_var<std::string> wrapped_string{"hello there"};
```

Create a `std::vector` and protect it underneath a mutex.
```cpp
using vector_type = std::vector<int>;
constexpr vector_type::size_type size = 5;
constexpr vector_type::value_type value = 12;
wrapped_var<vector_type> wrapped_string(size, value);
```

Modify a mutex-protected `std::string`
```cpp
wrapped_var<std::string> wrapped_string{"modify me"};
{
  auto string_accessor = wrapped_string.get();
  string_accessor.get_ref() = "you are modified";
}

const auto const_string_accessor = wrapped_string.get();
ASSERT_EQ(const_string_accessor.get_cref(), "you are modified");
```

### Notes
Not all lock types will compile. `std::lock_guard` and `std::scoped_lock` both hit compilation errors.
So far only `std::unique_lock` works.
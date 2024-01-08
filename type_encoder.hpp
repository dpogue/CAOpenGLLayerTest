#include <objc/runtime.h>
#include <array>
#include <string_view>
#include <tuple>
#include <type_traits>
#include <utility>

template <std::string_view const&... Strs>
struct join
{
    // Join all strings into a single std::array of chars
    static constexpr auto impl() noexcept
    {
        constexpr std::size_t len = (Strs.size() + ... + 0);
        std::array<char, len + 1> arr{};
        auto append = [i = 0, &arr](auto const& s) mutable {
            for (auto c : s) arr[i++] = c;
        };
        (append(Strs), ...);
        arr[len] = 0;
        return arr;
    }
    // Give the joined string static storage
    static constexpr auto arr = impl();
    // View as a std::string_view
    static constexpr std::string_view value {arr.data(), arr.size() - 1};
};

// Helper to get the value out
template <std::string_view const&... Strs>
inline constexpr auto join_v = join<Strs...>::value;

// Handle special pointers as their own types, not as pointers
template<typename T> struct is_non_objc_pointer : std::is_pointer<T> {};
template<> struct is_non_objc_pointer<void> : std::false_type {};
template<> struct is_non_objc_pointer<id> : std::false_type {};
template<> struct is_non_objc_pointer<SEL> : std::false_type {};
template<> struct is_non_objc_pointer<Class> : std::false_type {};
template<> struct is_non_objc_pointer<char*> : std::false_type {};
template<> struct is_non_objc_pointer<const char*> : std::false_type {};

template<typename T, typename Enable = void>
struct encoded_type { };

template<typename T>
inline constexpr auto encoded_type_v = encoded_type<T>::value;

template<typename T>
struct encoded_type<T, std::enable_if_t<is_non_objc_pointer<T>::value && !std::is_class_v<std::remove_pointer_t<T>>>> {
    static constexpr std::string_view _pointer = "^";
    static constexpr std::string_view value = join_v<_pointer, encoded_type_v<std::remove_pointer_t<T>>>;
};

template<typename T>
struct encoded_type<T, std::enable_if_t<is_non_objc_pointer<T>::value && std::is_class_v<std::remove_pointer_t<T>> && !std::is_const_v<std::remove_pointer_t<T>>>> {
    // Cheat a bit here and treat pointers to structures/classes as void*, so
    // that we don't have to encode the structure
    // Clang does this for C++ Templates, so it should be fine...
    static constexpr std::string_view value = "^v";
};

template<typename T>
struct encoded_type<T, std::enable_if_t<is_non_objc_pointer<T>::value && std::is_class_v<std::remove_pointer_t<T>> && std::is_const_v<std::remove_pointer_t<T>>>> {
    // Same cheat, but for const pointers to structures/classes
    static constexpr std::string_view value = "^rv";
};

template<typename T>
struct encoded_type<T, std::enable_if_t<std::is_const_v<T>>> {
    static constexpr std::string_view _const = "r";
    static constexpr std::string_view value = join_v<_const, encoded_type_v<std::remove_cv_t<T>>>;
};

template<typename T>
struct encoded_type<T, std::enable_if_t<std::is_void_v<T>>>  {
    static constexpr std::string_view value = "v";
};

template<typename T>
struct encoded_type<T, std::enable_if_t<std::is_same_v<T, id>>> {
    static constexpr std::string_view value = "@";
};

template<typename T>
struct encoded_type<T, std::enable_if_t<std::is_same_v<T, SEL>>> {
    static constexpr std::string_view value = ":";
};

template<typename T>
struct encoded_type<T, std::enable_if_t<std::is_same_v<T, Class>>> {
    static constexpr std::string_view value = "#";
};

template<typename T>
struct encoded_type<T, std::enable_if_t<std::is_same_v<T, char>>> {
    static constexpr std::string_view value = "c";
};

template<typename T>
struct encoded_type<T, std::enable_if_t<std::is_same_v<T, signed char>>> {
    static constexpr std::string_view value = "c";
};

template<typename T>
struct encoded_type<T, std::enable_if_t<std::is_same_v<T, unsigned char>>> {
    static constexpr std::string_view value = "C";
};

template<typename T>
struct encoded_type<T, std::enable_if_t<std::is_same_v<T, char*>>> {
    static constexpr std::string_view value = "*";
};

template<typename T>
struct encoded_type<T, std::enable_if_t<std::is_same_v<T, const char*>>> {
    static constexpr std::string_view value = "r*";
};

template<typename T>
struct encoded_type<T, std::enable_if_t<std::is_same_v<T, short>>> {
    static constexpr std::string_view value = "s";
};

template<typename T>
struct encoded_type<T, std::enable_if_t<std::is_same_v<T, unsigned short>>> {
    static constexpr std::string_view value = "S";
};

template<typename T>
struct encoded_type<T, std::enable_if_t<std::is_same_v<T, int>>> {
    static constexpr std::string_view value = "i";
};

template<typename T>
struct encoded_type<T, std::enable_if_t<std::is_same_v<T, unsigned int>>> {
    static constexpr std::string_view value = "I";
};

template<typename T>
struct encoded_type<T, std::enable_if_t<std::is_same_v<T, long>>> {
    static constexpr std::string_view value = "l";
};

template<typename T>
struct encoded_type<T, std::enable_if_t<std::is_same_v<T, unsigned long>>> {
    static constexpr std::string_view value = "L";
};

template<typename T>
struct encoded_type<T, std::enable_if_t<std::is_same_v<T, long long>>> {
    static constexpr std::string_view value = "q";
};

template<typename T>
struct encoded_type<T, std::enable_if_t<std::is_same_v<T, unsigned long long>>> {
    static constexpr std::string_view value = "Q";
};

template<typename T>
struct encoded_type<T, std::enable_if_t<std::is_same_v<T, float>>> {
    static constexpr std::string_view value = "f";
};

template<typename T>
struct encoded_type<T, std::enable_if_t<std::is_same_v<T, double>>> {
    static constexpr std::string_view value = "d";
};

template<typename T>
struct encoded_type<T, std::enable_if_t<std::is_same_v<T, long double>>> {
    static constexpr std::string_view value = "D";
};

template<typename T>
struct encoded_type<T, std::enable_if_t<std::is_function_v<T>>> {
    static constexpr std::string_view value = "?";
};

template<typename T>
struct encoded_type<T, std::enable_if_t<std::is_class_v<T> && !std::is_const_v<T>>> {
    static constexpr std::string_view value = "{?=}";
};

template<typename T>
struct encoded_type<T, std::enable_if_t<std::is_union_v<T>>> {
    static constexpr std::string_view value = "(?=)";
};


template<typename T, std::size_t I = 0, typename Enable = void>
struct encoded_arguments { };

template<typename T, std::size_t I = 0>
inline constexpr auto encoded_arguments_v = encoded_arguments<T, I>::value;

template<typename T, std::size_t I>
struct encoded_arguments<T, I, std::enable_if_t<I < std::tuple_size_v<T>>> {
    static constexpr std::string_view value = join_v<encoded_type_v<std::tuple_element_t<I, T>>, encoded_arguments_v<T, I + 1>>;
};

template<typename T, std::size_t I>
struct encoded_arguments<T, I, std::enable_if_t<I == std::tuple_size_v<T>>> {
    static constexpr std::string_view value = "";
};

template<typename R, typename... Args>
inline constexpr const char* encode_method(R(*)(Args...)) {
    return join_v<encoded_type_v<R>, encoded_arguments_v<typename std::tuple<Args...>>>.data();
}


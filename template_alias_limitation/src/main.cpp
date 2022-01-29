#include <concepts>

#define ALLOW_COMPILATION_FAILURE_IN_MSVC_AND_CLANG_BUT_NOT_GCC 0

template <typename a_p, typename b_p>
class template_t            {};

template <typename a_p, typename b_p>
using template_alias_t      = template_t<a_p, b_p>;

template <template <typename ...> typename template_p, typename ...parameters_p>
using template_template_t   = template_p<parameters_p...>;

template <typename type_a_p, typename type_b_p>
concept is_any_tr =
    std::same_as<std::remove_cv_t<type_a_p>, std::remove_cv_t<type_b_p>>;

template <
    template <typename ...> typename template_a_p,
    template <typename ...> typename template_b_p,
    typename ...parameters_p
> concept is_any_ttr =
    std::same_as<template_a_p<parameters_p...>, template_b_p<parameters_p...>>;

template <typename ...parameters_p>
using should_compile_t      = template_t<parameters_p...>;

#if (ALLOW_COMPILATION_FAILURE_IN_MSVC_AND_CLANG_BUT_NOT_GCC)
template <typename ...parameters_p>
using fails_to_compile_t    = template_alias_t<parameters_p...>;
#endif

int main(void)
{
    static_assert(is_any_tr<template_t<int, int>, template_alias_t<int, int>>);
    static_assert(is_any_ttr<template_t, template_alias_t, int, int>);

    static_assert(is_any_tr<template_template_t<template_t, int, int>, template_template_t<template_alias_t, int, int>>);
    static_assert(is_any_tr<template_t<int, int>, template_template_t<template_alias_t, int, int>>);
    static_assert(is_any_tr<template_template_t<template_t, int, int>, template_alias_t<int, int>>);

    // Pretty unintuitive that the one compiles but not the other. The only difference is the extra indirection.
    // However as seen above, the extra indirection works when an alias of a template is passed as a template template
    // parameter taking a parameter pack, so there is a reasonable work-around that works on all three major compilers.
    static_assert(is_any_tr<template_t<int, int>, should_compile_t<int, int>>);
#if (ALLOW_COMPILATION_FAILURE_IN_MSVC_AND_CLANG_BUT_NOT_GCC)
    static_assert(is_any_tr<template_t<int, int>, fails_to_compile_t<int, int>>);
#endif

    return 0;
}

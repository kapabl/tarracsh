#ifndef NODE_TRAITS_H
#define NODE_TRAITS_H


namespace kapa::tarracsh::domain::classfile::signature {

template <typename T>
struct is_or_single_capture {
    static constexpr bool value = std::false_type{};
};


template <>
struct is_or_single_capture<std::wstring> {
    static constexpr bool value = std::true_type{};
};


template <typename T>
struct is_list {
    static constexpr bool value = std::false_type{};
};

template <typename T>
struct is_list<std::vector<T>> {
    static constexpr bool value = std::true_type{};
};

}


#endif

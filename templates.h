#pragma once

template<typename... Args> struct Types;
template<typename T, typename... Args> struct Types<T, Args...> { static int size() { return sizeof(T) + Types<Args...>::size(); } };
template<typename T> struct Types<T> { static int size() { return sizeof(T); } };

template<typename... Args> struct TypeSize;
template<typename T, typename... Args> struct TypeSize<T, Args...> { enum {value = TypeSize<T>::value + TypeSize<Args...>::value}; };
template<typename T> struct TypeSize<T> { enum {value = 1}; };
#pragma once

template<typename type>
const type Constants<type>::Pi = type(3.1415926);

template<typename type>
const type Constants<type>::HalfPi = Constants<type>::Pi * type(0.5);

template<typename type>
const type Constants<type>::TwoPi = Constants<type>::Pi * type(2.0);

template<typename type>
const type Constants<type>::InvPi = type(1) / Constants<type>::Pi;
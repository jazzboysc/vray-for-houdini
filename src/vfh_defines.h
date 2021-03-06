//
// Copyright (c) 2015-2016, Chaos Software Ltd
//
// V-Ray For Houdini
//
// ACCESSIBLE SOURCE CODE WITHOUT DISTRIBUTION OF MODIFICATION LICENSE
//
// Full license text: https://github.com/ChaosGroup/vray-for-houdini/blob/master/LICENSE
//

#ifndef VRAY_FOR_HOUDINI_UTIL_DEFINES_H
#define VRAY_FOR_HOUDINI_UTIL_DEFINES_H

// QT defines macro "foreach" to be the same as QT_FOREACH which is highly likely to collide
// openvdb/util/NodeMask has "foreach" method
// define this macro to stop QT from defining it
#ifndef QT_NO_KEYWORDS
#  define QT_NO_KEYWORDS
#endif

#define STRINGIZE_NX(A) #A
#define STRINGIZE(A) STRINGIZE_NX(A)
#define NOT(x) !(x)

#define StrEq(nameA, nameB) (vutils_strcmp(nameA, nameB) == 0)

#define FreePtr(p)    if (p) { delete    p; p = nullptr; }
#define FreePtrArr(p) if (p) { delete [] p; p = nullptr; }

template <typename T, int N>
char (&ArraySizeHelper(T (&array)[N]))[N];
#define CountOf(array) (sizeof(ArraySizeHelper(array)))

#define IsFloatEq(a, b) VUtils::isZero(a - b, 1e-6f)

#define MemberEq(member) (member == other.member)
#define MemberFloatEq(member) (IsFloatEq(member, other.member))
#define MemberNotEq(member) (!(member == other.member))

/// Disables copy-constructor and assignment operator
#define VfhDisableCopy(cls) \
private: \
	cls(const cls&); \
	cls& operator=(const cls&);

#endif // VRAY_FOR_HOUDINI_UTIL_DEFINES_H

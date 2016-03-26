#ifndef _LINUX_SWAB_H
#define _LINUX_SWAB_H

// #include <linux/types.h>
#ifndef _LINUX_TYPES_H
#define _LINUX_TYPES_H

// #include <asm/types.h>
#ifndef _ASM_X86_TYPES_H
#define _ASM_X86_TYPES_H

// #include <asm-generic/types.h>
#ifndef _ASM_GENERIC_TYPES_H
#define _ASM_GENERIC_TYPES_H
/*
 * int-ll64 is used practically everywhere now,
 * so use it as a reasonable default.
 */
// #include <asm-generic/int-ll64.h>
/*
 * asm-generic/int-ll64.h
 *
 * Integer declarations for architectures which use "long long"
 * for 64-bit types.
 */

#ifndef _ASM_GENERIC_INT_LL64_H
#define _ASM_GENERIC_INT_LL64_H

// #include <asm/bitsperlong.h>
#ifndef __ASM_X86_BITSPERLONG_H
#define __ASM_X86_BITSPERLONG_H

#ifdef __x86_64__
# define __BITS_PER_LONG 64
#else
# define __BITS_PER_LONG 32
#endif

// #include <asm-generic/bitsperlong.h>
#ifndef __ASM_GENERIC_BITS_PER_LONG
#define __ASM_GENERIC_BITS_PER_LONG

/*
 * There seems to be no way of detecting this automatically from user
 * space, so 64 bit architectures should override this in their
 * bitsperlong.h. In particular, an architecture that supports
 * both 32 and 64 bit user space must not rely on CONFIG_64BIT
 * to decide it, but rather check a compiler provided macro.
 */
#ifndef __BITS_PER_LONG
#define __BITS_PER_LONG 32
#endif

#endif /* __ASM_GENERIC_BITS_PER_LONG */


#endif /* __ASM_X86_BITSPERLONG_H */

#ifndef __ASSEMBLY__
/*
 * __xx is ok: it doesn't pollute the POSIX namespace. Use these in the
 * header files exported to user space
 */

typedef __signed__ char __s8;
typedef unsigned char __u8;

typedef __signed__ short __s16;
typedef unsigned short __u16;

typedef __signed__ int __s32;
typedef unsigned int __u32;

#ifdef __GNUC__
__extension__ typedef __signed__ long long __s64;
__extension__ typedef unsigned long long __u64;
#else
typedef __signed__ long long __s64;
typedef unsigned long long __u64;
#endif

#endif /* __ASSEMBLY__ */


#endif /* _ASM_GENERIC_INT_LL64_H */


#endif /* _ASM_GENERIC_TYPES_H */

#endif /* _ASM_X86_TYPES_H */

#ifndef __ASSEMBLY__

//#include <linux/posix_types.h>


/*
 * Below are truly Linux-specific types that should never collide with
 * any application/library that wants linux/types.h.
 */

#ifdef __CHECKER__
#define __bitwise__ __attribute__((bitwise))
#else
#define __bitwise__
#endif
#ifdef __CHECK_ENDIAN__
#define __bitwise __bitwise__
#else
#define __bitwise
#endif

typedef __u16 __bitwise __le16;
typedef __u16 __bitwise __be16;
typedef __u32 __bitwise __le32;
typedef __u32 __bitwise __be32;
typedef __u64 __bitwise __le64;
typedef __u64 __bitwise __be64;

typedef __u16 __bitwise __sum16;
typedef __u32 __bitwise __wsum;

/*
 * aligned_u64 should be used in defining kernel<->userspace ABIs to avoid
 * common 32/64-bit compat problems.
 * 64-bit values align to 4-byte boundaries on x86_32 (and possibly other
 * architectures) and to 8-byte boundaries on 64-bit architectures.  The new
 * aligned_64 type enforces 8-byte alignment so that structs containing
 * aligned_64 values have the same alignment on 32-bit and 64-bit architectures.
 * No conversions are necessary between 32-bit user-space and a 64-bit kernel.
 */
#define __aligned_u64 __u64 __attribute__((aligned(8)))
#define __aligned_be64 __be64 __attribute__((aligned(8)))
#define __aligned_le64 __le64 __attribute__((aligned(8)))

#endif /*  __ASSEMBLY__ */
#endif /* _LINUX_TYPES_H */


// #include <asm/swab.h>
#ifndef _ASM_X86_SWAB_H
#define _ASM_X86_SWAB_H

//#include <linux/types.h>


static __inline__  __u32 __arch_swab32(__u32 val)
{
	__asm__("bswapl %0" : "=r" (val) : "0" (val));
	return val;
}
#define __arch_swab32 __arch_swab32

static __inline__  __u64 __arch_swab64(__u64 val)
{
#ifdef __i386__
	union {
		struct {
			__u32 a;
			__u32 b;
		} s;
		__u64 u;
	} v;
	v.u = val;
	__asm__("bswapl %0 ; bswapl %1 ; xchgl %0,%1"
	    : "=r" (v.s.a), "=r" (v.s.b)
	    : "0" (v.s.a), "1" (v.s.b));
	return v.u;
#else /* __i386__ */
	__asm__("bswapq %0" : "=r" (val) : "0" (val));
	return val;
#endif
}
// #define __arch_swab64 __arch_swab64

#endif /* _ASM_X86_SWAB_H */

/*
 * casts are necessary for constants, because we never know how for sure
 * how U/UL/ULL map to __u16, __u32, __u64. At least not in a portable way.
 */
#define ___constant_swab16(x) ((__u16)(				\
	(((__u16)(x) & (__u16)0x00ffU) << 8) |			\
	(((__u16)(x) & (__u16)0xff00U) >> 8)))

#define ___constant_swab32(x) ((__u32)(				\
	(((__u32)(x) & (__u32)0x000000ffUL) << 24) |		\
	(((__u32)(x) & (__u32)0x0000ff00UL) <<  8) |		\
	(((__u32)(x) & (__u32)0x00ff0000UL) >>  8) |		\
	(((__u32)(x) & (__u32)0xff000000UL) >> 24)))

#define ___constant_swab64(x) ((__u64)(				\
	(((__u64)(x) & (__u64)0x00000000000000ffULL) << 56) |	\
	(((__u64)(x) & (__u64)0x000000000000ff00ULL) << 40) |	\
	(((__u64)(x) & (__u64)0x0000000000ff0000ULL) << 24) |	\
	(((__u64)(x) & (__u64)0x00000000ff000000ULL) <<  8) |	\
	(((__u64)(x) & (__u64)0x000000ff00000000ULL) >>  8) |	\
	(((__u64)(x) & (__u64)0x0000ff0000000000ULL) >> 24) |	\
	(((__u64)(x) & (__u64)0x00ff000000000000ULL) >> 40) |	\
	(((__u64)(x) & (__u64)0xff00000000000000ULL) >> 56)))

#define ___constant_swahw32(x) ((__u32)(			\
	(((__u32)(x) & (__u32)0x0000ffffUL) << 16) |		\
	(((__u32)(x) & (__u32)0xffff0000UL) >> 16)))

#define ___constant_swahb32(x) ((__u32)(			\
	(((__u32)(x) & (__u32)0x00ff00ffUL) << 8) |		\
	(((__u32)(x) & (__u32)0xff00ff00UL) >> 8)))

/*
 * Implement the following as inlines, but define the interface using
 * macros to allow constant folding when possible:
 * ___swab16, ___swab32, ___swab64, ___swahw32, ___swahb32
 */

static __inline__  __u16 __fswab16(__u16 val)
{
#ifdef __HAVE_BUILTIN_BSWAP16__
	return __builtin_bswap16(val);
#elif defined (__arch_swab16)
	return __arch_swab16(val);
#else
	return ___constant_swab16(val);
#endif
}

static __inline__  __u32 __fswab32(__u32 val)
{
#ifdef __HAVE_BUILTIN_BSWAP32__
	return __builtin_bswap32(val);
#elif defined(__arch_swab32)
	return __arch_swab32(val);
#else
	return ___constant_swab32(val);
#endif
}

static __inline__  __u64 __fswab64(__u64 val)
{
#ifdef __HAVE_BUILTIN_BSWAP64__
	return __builtin_bswap64(val);
#elif defined (_ASM_X86_SWAB_H)
	return __arch_swab64(val);
#elif defined(__SWAB_64_THRU_32__)
	__u32 h = val >> 32;
	__u32 l = val & ((1ULL << 32) - 1);
	return (((__u64)__fswab32(l)) << 32) | ((__u64)(__fswab32(h)));
#else
	return ___constant_swab64(val);
#endif
}

static __inline__  __u32 __fswahw32(__u32 val)
{
#ifdef __arch_swahw32
	return __arch_swahw32(val);
#else
	return ___constant_swahw32(val);
#endif
}

static __inline__  __u32 __fswahb32(__u32 val)
{
#ifdef __arch_swahb32
	return __arch_swahb32(val);
#else
	return ___constant_swahb32(val);
#endif
}

/**
 * __swab16 - return a byteswapped 16-bit value
 * @x: value to byteswap
 */
#define __swab16(x)				\
	(__builtin_constant_p((__u16)(x)) ?	\
	___constant_swab16(x) :			\
	__fswab16(x))

/**
 * __swab32 - return a byteswapped 32-bit value
 * @x: value to byteswap
 */
#define __swab32(x)				\
	(__builtin_constant_p((__u32)(x)) ?	\
	___constant_swab32(x) :			\
	__fswab32(x))

/**
 * __swab64 - return a byteswapped 64-bit value
 * @x: value to byteswap
 */
#define __swab64(x)				\
	(__builtin_constant_p((__u64)(x)) ?	\
	___constant_swab64(x) :			\
	__fswab64(x))

/**
 * __swahw32 - return a word-swapped 32-bit value
 * @x: value to wordswap
 *
 * __swahw32(0x12340000) is 0x00001234
 */
#define __swahw32(x)				\
	(__builtin_constant_p((__u32)(x)) ?	\
	___constant_swahw32(x) :		\
	__fswahw32(x))

/**
 * __swahb32 - return a high and low byte-swapped 32-bit value
 * @x: value to byteswap
 *
 * __swahb32(0x12345678) is 0x34127856
 */
#define __swahb32(x)				\
	(__builtin_constant_p((__u32)(x)) ?	\
	___constant_swahb32(x) :		\
	__fswahb32(x))

/**
 * __swab16p - return a byteswapped 16-bit value from a pointer
 * @p: pointer to a naturally-aligned 16-bit value
 */
static __inline__ __u16 __swab16p(const __u16 *p)
{
#ifdef __arch_swab16p
	return __arch_swab16p(p);
#else
	return __swab16(*p);
#endif
}

/**
 * __swab32p - return a byteswapped 32-bit value from a pointer
 * @p: pointer to a naturally-aligned 32-bit value
 */
static __inline__ __u32 __swab32p(const __u32 *p)
{
#ifdef __arch_swab32p
	return __arch_swab32p(p);
#else
	return __swab32(*p);
#endif
}

/**
 * __swab64p - return a byteswapped 64-bit value from a pointer
 * @p: pointer to a naturally-aligned 64-bit value
 */
static __inline__ __u64 __swab64p(const __u64 *p)
{
#ifdef __arch_swab64p
	return __arch_swab64p(p);
#else
	return __swab64(*p);
#endif
}

/**
 * __swahw32p - return a wordswapped 32-bit value from a pointer
 * @p: pointer to a naturally-aligned 32-bit value
 *
 * See __swahw32() for details of wordswapping.
 */
static __inline__ __u32 __swahw32p(const __u32 *p)
{
#ifdef __arch_swahw32p
	return __arch_swahw32p(p);
#else
	return __swahw32(*p);
#endif
}

/**
 * __swahb32p - return a high and low byteswapped 32-bit value from a pointer
 * @p: pointer to a naturally-aligned 32-bit value
 *
 * See __swahb32() for details of high/low byteswapping.
 */
static __inline__ __u32 __swahb32p(const __u32 *p)
{
#ifdef __arch_swahb32p
	return __arch_swahb32p(p);
#else
	return __swahb32(*p);
#endif
}

/**
 * __swab16s - byteswap a 16-bit value in-place
 * @p: pointer to a naturally-aligned 16-bit value
 */
static __inline__ void __swab16s(__u16 *p)
{
#ifdef __arch_swab16s
	__arch_swab16s(p);
#else
	*p = __swab16p(p);
#endif
}
/**
 * __swab32s - byteswap a 32-bit value in-place
 * @p: pointer to a naturally-aligned 32-bit value
 */
static __inline__ void __swab32s(__u32 *p)
{
#ifdef __arch_swab32s
	__arch_swab32s(p);
#else
	*p = __swab32p(p);
#endif
}

/**
 * __swab64s - byteswap a 64-bit value in-place
 * @p: pointer to a naturally-aligned 64-bit value
 */
static __inline__ void __swab64s(__u64 *p)
{
#ifdef __arch_swab64s
	__arch_swab64s(p);
#else
	*p = __swab64p(p);
#endif
}

/**
 * __swahw32s - wordswap a 32-bit value in-place
 * @p: pointer to a naturally-aligned 32-bit value
 *
 * See __swahw32() for details of wordswapping
 */
static __inline__ void __swahw32s(__u32 *p)
{
#ifdef __arch_swahw32s
	__arch_swahw32s(p);
#else
	*p = __swahw32p(p);
#endif
}

/**
 * __swahb32s - high and low byteswap a 32-bit value in-place
 * @p: pointer to a naturally-aligned 32-bit value
 *
 * See __swahb32() for details of high and low byte swapping
 */
static __inline__ void __swahb32s(__u32 *p)
{
#ifdef __arch_swahb32s
	__arch_swahb32s(p);
#else
	*p = __swahb32p(p);
#endif
}


#endif /* _LINUX_SWAB_H */


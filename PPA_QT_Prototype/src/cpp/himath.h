#ifndef HIMATH_H
#define HIMATH_H

#ifdef WIN32 /* HACK Windows math.h may not define fminf so pretend it does */
#define HI_FMIN(a,b) (a) < (b) ? (a) : (b)
#define HI_FMAX(a,b) (a) > (b) ? (a) : (b)
#else
# define HI_FMIN(x, y) (fminf((x), (y))) /*!< minimum of two floats */
# define HI_FMAX(x, y) (fmaxf((x), (y)))
#endif

#endif // HIMATH_H
